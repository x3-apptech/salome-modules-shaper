// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:        FeaturesPlugin_Boolean.cpp
// Created:     02 Sept 2014
// Author:      Vitaly SMETANNIKOV

#include "FeaturesPlugin_Boolean.h"

#include <ModelAPI_Data.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_ResultBody.h>
#include <ModelAPI_AttributeSelectionList.h>

#include <GeomAlgoAPI_Boolean.h>
#include <GeomAlgoAPI_MakeShapeList.h>
#include <GeomAlgoAPI_ShapeProps.h>

#define FACE 4
#define _MODIFY_TAG 1
#define _DELETED_TAG 2

//=================================================================================================
FeaturesPlugin_Boolean::FeaturesPlugin_Boolean()
{
}

//=================================================================================================
void FeaturesPlugin_Boolean::initAttributes()
{
  data()->addAttribute(FeaturesPlugin_Boolean::TYPE_ID(), ModelAPI_AttributeInteger::typeId());

  AttributeSelectionListPtr aSelection = 
    std::dynamic_pointer_cast<ModelAPI_AttributeSelectionList>(data()->addAttribute(
    FeaturesPlugin_Boolean::OBJECT_LIST_ID(), ModelAPI_AttributeSelectionList::typeId()));
  // extrusion works with faces always
  aSelection->setSelectionType("SOLID");

  aSelection = std::dynamic_pointer_cast<ModelAPI_AttributeSelectionList>(data()->addAttribute(
    FeaturesPlugin_Boolean::TOOL_LIST_ID(), ModelAPI_AttributeSelectionList::typeId()));
  // extrusion works with faces always
  aSelection->setSelectionType("SOLID");
}

//=================================================================================================
std::shared_ptr<GeomAPI_Shape> FeaturesPlugin_Boolean::getShape(const std::string& theAttrName)
{
  std::shared_ptr<ModelAPI_AttributeReference> aObjRef = std::dynamic_pointer_cast<
      ModelAPI_AttributeReference>(data()->attribute(theAttrName));
  if (aObjRef) {
    std::shared_ptr<ModelAPI_ResultBody> aConstr = std::dynamic_pointer_cast<
        ModelAPI_ResultBody>(aObjRef->value());
    if (aConstr)
      return aConstr->shape();
  }
  return std::shared_ptr<GeomAPI_Shape>();
}

//=================================================================================================
void FeaturesPlugin_Boolean::execute()
{
  // Getting operation type.
  std::shared_ptr<ModelAPI_AttributeInteger> aTypeAttr = std::dynamic_pointer_cast<
      ModelAPI_AttributeInteger>(data()->attribute(FeaturesPlugin_Boolean::TYPE_ID()));
  if (!aTypeAttr)
    return;
  GeomAlgoAPI_Boolean::OperationType aType = (GeomAlgoAPI_Boolean::OperationType)aTypeAttr->value();

  ListOfShape anObjects, aTools;

  // Getting objects.
  AttributeSelectionListPtr anObjectsSelList = selectionList(FeaturesPlugin_Boolean::OBJECT_LIST_ID());
  if (anObjectsSelList->size() == 0) {
    return;
  }
  for(int anObjectsIndex = 0; anObjectsIndex < anObjectsSelList->size(); anObjectsIndex++) {
    std::shared_ptr<ModelAPI_AttributeSelection> anObjectAttr = anObjectsSelList->value(anObjectsIndex);
    std::shared_ptr<GeomAPI_Shape> anObject = anObjectAttr->value();
    if(!anObject.get()) {
      return;
    }
    anObjects.push_back(anObject);
  }

  // Getting tools.
  AttributeSelectionListPtr aToolsSelList = selectionList(FeaturesPlugin_Boolean::TOOL_LIST_ID());
  if (aToolsSelList->size() == 0) {
    return;
  }
  for(int aToolsIndex = 0; aToolsIndex < aToolsSelList->size(); aToolsIndex++) {
    std::shared_ptr<ModelAPI_AttributeSelection> aToolAttr = aToolsSelList->value(aToolsIndex);
    std::shared_ptr<GeomAPI_Shape> aTool = aToolAttr->value();
    if(!aTool.get()) {
      return;
    }
    aTools.push_back(aTool);
  }

  int aResultIndex = 0;
  ListOfMakeShape aListOfMakeShape;
  std::shared_ptr<GeomAPI_DataMapOfShapeShape> aDataMapOfShapes;

  switch(aType) {
    case GeomAlgoAPI_Boolean::BOOL_CUT:
    case GeomAlgoAPI_Boolean::BOOL_COMMON:{
      // Cut each object with all tools
      for(ListOfShape::iterator anObjectsIt = anObjects.begin(); anObjectsIt != anObjects.end(); anObjectsIt++) {
        std::shared_ptr<GeomAPI_Shape> anObject = *anObjectsIt;
        ListOfShape aListWithObject;
        aListWithObject.push_back(anObject);
        GeomAlgoAPI_Boolean aBoolAlgo(aListWithObject, aTools, aType);

        // Checking that the algorithm worked properly.
        if(!aBoolAlgo.isDone()) {
          static const std::string aFeatureError = "Boolean algorithm failed";
          setError(aFeatureError);
          return;
        }
        if(aBoolAlgo.shape()->isNull()) {
          static const std::string aShapeError = "Resulting shape is Null";
          setError(aShapeError);
          return;
        }
        if(!aBoolAlgo.isValid()) {
          std::string aFeatureError = "Warning: resulting shape is not valid";
          setError(aFeatureError);
          return;
        }

        if(GeomAlgoAPI_ShapeProps::volume(aBoolAlgo.shape()) > 1.e-7) {
          std::shared_ptr<ModelAPI_ResultBody> aResultBody = document()->createBody(data(), aResultIndex);
          LoadNamingDS(aResultBody, anObject, aTools, aBoolAlgo);
          setResult(aResultBody, aResultIndex);
          aResultIndex++;
        }
      }
      break;
    }
    case GeomAlgoAPI_Boolean::BOOL_FUSE: {
      // Fuse all objects and all tools.
      GeomAlgoAPI_Boolean aBoolAlgo(anObjects, aTools, aType);

      // Checking that the algorithm worked properly.
      if(!aBoolAlgo.isDone()) {
        static const std::string aFeatureError = "Boolean algorithm failed";
        setError(aFeatureError);
        return;
      }
      if(aBoolAlgo.shape()->isNull()) {
        static const std::string aShapeError = "Resulting shape is Null";
        setError(aShapeError);
        return;
      }
      if(!aBoolAlgo.isValid()) {
        std::string aFeatureError = "Warning: resulting shape is not valid";
        setError(aFeatureError);
        return;
      }

      std::shared_ptr<ModelAPI_ResultBody> aResultBody = document()->createBody(data(), aResultIndex);
      std::shared_ptr<GeomAlgoAPI_MakeShapeList> aMakeShapeList = std::shared_ptr<GeomAlgoAPI_MakeShapeList>(
        new GeomAlgoAPI_MakeShapeList(aListOfMakeShape));

      LoadNamingDS(aResultBody, anObjects.front(), aTools, aBoolAlgo);
      setResult(aResultBody, aResultIndex);
      aResultIndex++;
      break;
    }
    default: {
      std::string anOperationError = "Error: wrong type of operation";
      setError(anOperationError);
      return;
    }
  }
  // remove the rest results if there were produced in the previous pass
  removeResults(aResultIndex);
}

//=================================================================================================
void FeaturesPlugin_Boolean::LoadNamingDS(std::shared_ptr<ModelAPI_ResultBody> theResultBody,
                                          const std::shared_ptr<GeomAPI_Shape>& theBaseShape,
                                          const ListOfShape& theTools,
                                          const GeomAlgoAPI_Boolean& theAlgo)
{
  //load result
  if(theBaseShape->isEqual(theAlgo.shape())) {
    theResultBody->store(theAlgo.shape());
  } else {
    theResultBody->storeModified(theBaseShape, theAlgo.shape());

    GeomAPI_DataMapOfShapeShape* aSubShapes = new GeomAPI_DataMapOfShapeShape();

    std::string aModName = "Modified";
    theResultBody->loadAndOrientModifiedShapes(theAlgo.makeShape().get(), theBaseShape, FACE,
                                               _MODIFY_TAG, aModName, *theAlgo.mapOfShapes().get());
    theResultBody->loadDeletedShapes(theAlgo.makeShape().get(), theBaseShape, FACE, _DELETED_TAG);

    for(ListOfShape::const_iterator anIter = theTools.begin(); anIter != theTools.end(); anIter++) {
      theResultBody->loadAndOrientModifiedShapes(theAlgo.makeShape().get(), *anIter, FACE,
                                                 _MODIFY_TAG, aModName, *theAlgo.mapOfShapes().get());
      theResultBody->loadDeletedShapes(theAlgo.makeShape().get(), *anIter, FACE, _DELETED_TAG);
    }
  }
}
