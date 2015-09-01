// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:        FeaturesPlugin_Partition.cpp
// Created:     31 Jul 2015
// Author:      Natalia ERMOLAEVA

#include "FeaturesPlugin_Partition.h"

#include <ModelAPI_Data.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_BodyBuilder.h>
#include <ModelAPI_ResultBody.h>
#include <ModelAPI_AttributeSelectionList.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Validator.h>

#include <GeomAlgoAPI_Partition.h>
#include <GeomAlgoAPI_MakeShapeList.h>
#include <GeomAlgoAPI_ShapeTools.h>

//=================================================================================================
FeaturesPlugin_Partition::FeaturesPlugin_Partition()
{
}

//=================================================================================================
void FeaturesPlugin_Partition::initAttributes()
{
  AttributeSelectionListPtr aSelection =
    std::dynamic_pointer_cast<ModelAPI_AttributeSelectionList>(data()->addAttribute(
    FeaturesPlugin_Partition::OBJECT_LIST_ID(), ModelAPI_AttributeSelectionList::typeId()));
  aSelection->setSelectionType("SOLID");

  aSelection =
    std::dynamic_pointer_cast<ModelAPI_AttributeSelectionList>(data()->addAttribute(
    FeaturesPlugin_Partition::TOOL_LIST_ID(), ModelAPI_AttributeSelectionList::typeId()));
  aSelection->setSelectionType("SOLID");

  ModelAPI_Session::get()->validators()->registerNotObligatory(getKind(), OBJECT_LIST_ID());
  ModelAPI_Session::get()->validators()->registerNotObligatory(getKind(), TOOL_LIST_ID());
}

//=================================================================================================
std::shared_ptr<GeomAPI_Shape> FeaturesPlugin_Partition::getShape(const std::string& theAttrName)
{
  std::shared_ptr<ModelAPI_AttributeReference> aObjRef =
      std::dynamic_pointer_cast<ModelAPI_AttributeReference>(data()->attribute(theAttrName));
  if (aObjRef) {
    std::shared_ptr<ModelAPI_ResultBody> aConstr =
        std::dynamic_pointer_cast<ModelAPI_ResultBody>(aObjRef->value());
    if (aConstr)
      return aConstr->shape();
  }
  return std::shared_ptr<GeomAPI_Shape>();
}

//=================================================================================================
void FeaturesPlugin_Partition::execute()
{
  ListOfShape anObjects, aTools;

  // Getting objects.
  AttributeSelectionListPtr anObjectsSelList = selectionList(FeaturesPlugin_Partition::OBJECT_LIST_ID());
  for (int anObjectsIndex = 0; anObjectsIndex < anObjectsSelList->size(); anObjectsIndex++) {
    std::shared_ptr<ModelAPI_AttributeSelection> anObjectAttr = anObjectsSelList->value(anObjectsIndex);
    std::shared_ptr<GeomAPI_Shape> anObject = anObjectAttr->value();
    if (!anObject.get()) {
      return;
    }
    anObjects.push_back(anObject);
  }

  // Getting tools.
  AttributeSelectionListPtr aToolsSelList = selectionList(FeaturesPlugin_Partition::TOOL_LIST_ID());
  for (int aToolsIndex = 0; aToolsIndex < aToolsSelList->size(); aToolsIndex++) {
    std::shared_ptr<ModelAPI_AttributeSelection> aToolAttr = aToolsSelList->value(aToolsIndex);
    std::shared_ptr<GeomAPI_Shape> aTool = aToolAttr->value();
    if (!aTool.get()) {
      // it could be a construction plane
      ResultPtr aContext = aToolAttr->context();
      if (aContext.get()) {
        aTool = GeomAlgoAPI_ShapeTools::faceToInfinitePlane(aContext->shape());
      }
    }
    if (!aTool.get()) {
      return;
    }
    aTools.push_back(aTool);
  }

  int aResultIndex = 0;

  if (anObjects.empty() || aTools.empty()) {
    std::string aFeatureError = "Not enough objects for partition operation";
    setError(aFeatureError);
    return;
  }

  // Cut each object with all tools
  for (ListOfShape::iterator anObjectsIt = anObjects.begin(); anObjectsIt != anObjects.end(); anObjectsIt++) {
    std::shared_ptr<GeomAPI_Shape> anObject = *anObjectsIt;
    ListOfShape aListWithObject; aListWithObject.push_back(anObject);
    GeomAlgoAPI_Partition aPartitionAlgo(aListWithObject, aTools);

    // Checking that the algorithm worked properly.
    if (!aPartitionAlgo.isDone()) {
      static const std::string aFeatureError = "Partition algorithm failed";
      setError(aFeatureError);
      return;
    }
    if (aPartitionAlgo.shape()->isNull()) {
      static const std::string aShapeError = "Resulting shape is Null";
      setError(aShapeError);
      return;
    }
    if (!aPartitionAlgo.isValid()) {
      std::string aFeatureError = "Warning: resulting shape is not valid";
      setError(aFeatureError);
      return;
    }

    if (GeomAlgoAPI_ShapeTools::volume(aPartitionAlgo.shape()) > 1.e-7) {
      std::shared_ptr<ModelAPI_ResultBody> aResultBody = document()->createBody(data(), aResultIndex);
      aResultBody->store(aPartitionAlgo.shape());
      loadNamingDS(aResultBody, anObject, aTools, aPartitionAlgo);
      setResult(aResultBody, aResultIndex);
      aResultIndex++;
    }
  }

  // remove the rest results if there were produced in the previous pass
  removeResults(aResultIndex);
}

//=================================================================================================
void FeaturesPlugin_Partition::loadNamingDS(std::shared_ptr<ModelAPI_ResultBody> theResultBody,
                                            const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                                            const ListOfShape& theTools,
                                            const GeomAlgoAPI_Partition& thePartitionAlgo)
{
  //load result
  if(theBaseShape->isEqual(thePartitionAlgo.shape())) {
    theResultBody->store(thePartitionAlgo.shape());
  } else {
    const int aModifyTag = 1;
    const int aDeletedTag = 2;
    const int aSubsolidsTag = 3; /// sub solids will be placed at labels 3, 4, etc. if result is compound of solids

    theResultBody->storeModified(theBaseShape, thePartitionAlgo.shape(), aSubsolidsTag);

    std::shared_ptr<GeomAlgoAPI_MakeShape> aMkShape = thePartitionAlgo.makeShape();
    std::shared_ptr<GeomAPI_DataMapOfShapeShape> aMapOfShapes = thePartitionAlgo.mapOfShapes();

    std::string aModName = "Modified";
    theResultBody->loadAndOrientModifiedShapes(aMkShape.get(), theBaseShape, GeomAPI_Shape::FACE,
                                               aModifyTag, aModName, *aMapOfShapes.get());
    theResultBody->loadDeletedShapes(aMkShape.get(), theBaseShape, GeomAPI_Shape::FACE, aDeletedTag);

    for(ListOfShape::const_iterator anIter = theTools.begin(); anIter != theTools.end(); anIter++) {
      theResultBody->loadAndOrientModifiedShapes(aMkShape.get(), *anIter, GeomAPI_Shape::FACE,
                                                 aModifyTag, aModName, *aMapOfShapes.get());
      theResultBody->loadDeletedShapes(aMkShape.get(), *anIter, GeomAPI_Shape::FACE, aDeletedTag);
    }
  }
}
