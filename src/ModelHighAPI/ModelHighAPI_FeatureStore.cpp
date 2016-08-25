// Copyright (C) 2016-20xx CEA/DEN, EDF R&D -->

// File:        ModelHighAPI_FeatureStore.cpp
// Created:     12 August 2016
// Author:      Mikhail PONIKAROV

#include <ModelHighAPI_FeatureStore.h>

#include <ModelAPI_Tools.h>
#include <ModelAPI_ResultPart.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_AttributeBoolean.h>
#include <ModelAPI_AttributeDocRef.h>
#include <ModelAPI_AttributeDouble.h>
#include <ModelAPI_AttributeIntArray.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_AttributeRefAttr.h>
#include <ModelAPI_AttributeRefAttrList.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeRefList.h>
#include <ModelAPI_AttributeSelection.h>
#include <ModelAPI_AttributeSelectionList.h>
#include <ModelAPI_AttributeString.h>
#include <ModelAPI_AttributeDoubleArray.h>
#include <ModelAPI_Validator.h>

#include <GeomDataAPI_Dir.h>
#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Point2D.h>
#include <GeomAlgoAPI_ShapeTools.h>
#include <GeomAPI_Pnt.h>

#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>

#include <ios>
#include <cmath>

#define PRECISION 6
#define TOLERANCE (1.e-7)

ModelHighAPI_FeatureStore::ModelHighAPI_FeatureStore(FeaturePtr theFeature) {
  storeData(theFeature->data(), myAttrs);
  // iterate results to store
  std::list<ResultPtr> allResults;
  ModelAPI_Tools::allResults(theFeature, allResults);
  std::list<ResultPtr>::iterator aRes = allResults.begin();
  for(; aRes != allResults.end(); aRes++) {
    std::map<std::string, std::string> aResDump;
    storeData((*aRes)->data(), aResDump);
    myRes.push_back(aResDump);
  }
}

std::string ModelHighAPI_FeatureStore::compare(FeaturePtr theFeature) {
  std::string anError = compareData(theFeature->data(), myAttrs);
  if (!anError.empty()) {
    return "Features '" + theFeature->name() + "' differ:" + anError;
  }
  std::list<ResultPtr> allResults;
  ModelAPI_Tools::allResults(theFeature, allResults);
  std::list<ResultPtr>::iterator aRes = allResults.begin();
  std::list<std::map<std::string, std::string> >::iterator aResIter = myRes.begin();
  for(; aRes != allResults.end() && aResIter != myRes.end(); aRes++, aResIter++) {
    anError = compareData((*aRes)->data(), *aResIter);
    if (!anError.empty())
      return "Results of feature '" + theFeature->name() + "' '" + (*aRes)->data()->name() + 
      "' differ:" + anError;
  }
  if (aRes != allResults.end()) {
    return "Current model has more results '" + (*aRes)->data()->name() + "'";
  }
  if (aResIter != myRes.end()) {
    return "Original model had more results '" + (*aResIter)["__name__"] + "'";
  }
  return ""; // ok
}

void ModelHighAPI_FeatureStore::storeData(std::shared_ptr<ModelAPI_Data> theData, 
  std::map<std::string, std::string>& theAttrs)
{
  theAttrs["__name__"] = theData->name(); // store name to keep also this information and output if needed
  std::list<std::shared_ptr<ModelAPI_Attribute> > allAttrs = theData->attributes("");
  std::list<std::shared_ptr<ModelAPI_Attribute> >::iterator anAttr = allAttrs.begin();
  for(; anAttr != allAttrs.end(); anAttr++) {
    theAttrs[(*anAttr)->id()] = dumpAttr(*anAttr);
  }
  ResultPtr aShapeOwner = std::dynamic_pointer_cast<ModelAPI_Result>(theData->owner());
  if (aShapeOwner.get() && aShapeOwner->shape().get()) {
    std::shared_ptr<GeomAPI_Shape> aShape = aShapeOwner->shape();
    theAttrs["__shape__"] = dumpShape(aShape);
  }
}

std::string ModelHighAPI_FeatureStore::compareData(std::shared_ptr<ModelAPI_Data> theData, 
  std::map<std::string, std::string>& theAttrs)
{
  std::map<std::string, std::string> aThis;
  storeData(theData, aThis);
  std::map<std::string, std::string>::iterator aThisIter = aThis.begin();
  for(; aThisIter != aThis.end(); aThisIter++) {
    if (theAttrs.find(aThisIter->first) == theAttrs.end()) {
      return "original model had no attribute '" + aThisIter->first + "'";
    }
    if (theAttrs[aThisIter->first] != aThisIter->second) {
      return "attribute '" + aThisIter->first + "' is different (original != current) '" + 
        theAttrs[aThisIter->first] + "' != '" + aThisIter->second + "'";
    }
  }
  // iterate back to find lack attribute in the current model
  std::map<std::string, std::string>::iterator anOrigIter = theAttrs.begin();
  for(; anOrigIter != theAttrs.end(); anOrigIter++) {
    if (aThis.find(anOrigIter->first) == aThis.end()) {
      return "current model had no attribute '" + anOrigIter->first + "'";
    }
  }
  return "";
}

static void dumpArray(std::ostringstream& theOutput, const double theArray[], int theSize, int thePrecision = PRECISION)
{
  for (int i = 0; i < theSize; ++i) {
    if (i > 0)
      theOutput << " ";
    theOutput << std::fixed << setprecision(thePrecision)
              << (fabs(theArray[i]) < TOLERANCE ? 0.0 : theArray[i]);
  }
}

std::string ModelHighAPI_FeatureStore::dumpAttr(const AttributePtr& theAttr) {
  static ModelAPI_ValidatorsFactory* aFactory = ModelAPI_Session::get()->validators();
  FeaturePtr aFeatOwner = std::dynamic_pointer_cast<ModelAPI_Feature>(theAttr->owner());
  if (aFeatOwner.get() && !aFactory->isCase(aFeatOwner, theAttr->id())) {
    return "__notcase__";
  }
  std::string aType = theAttr->attributeType();
  std::ostringstream aResult;
  if (!theAttr->isInitialized()) {
    if (aType == ModelAPI_AttributeBoolean::typeId()) {
      // special case for Boolean attribute (mean it false if not initialized)
      aResult << false;
      return aResult.str();
    } else if (aType == ModelAPI_AttributeString::typeId()) {
      // special case for attribute "SolverError"
      if (theAttr->id() == "SolverError" && 
          std::dynamic_pointer_cast<ModelAPI_Feature>(theAttr->owner())->getKind() == "Sketch")
        return "";
    }

    return "__notinitialized__";
  }
  if (aType == ModelAPI_AttributeDocRef::typeId()) {
    AttributeDocRefPtr anAttr = std::dynamic_pointer_cast<ModelAPI_AttributeDocRef>(theAttr);
    DocumentPtr aDoc = ModelAPI_Session::get()->moduleDocument();
    if (anAttr->value() != aDoc) {
      ResultPtr aRes = ModelAPI_Tools::findPartResult(aDoc, anAttr->value());
      if (aRes.get()) {
        aResult<<aRes->data()->name(); // Part result name (the same as saved file name)
      }
    } else {
      aResult<<aDoc->kind(); // PartSet
    }
  } else if (aType == ModelAPI_AttributeInteger::typeId()) {
    AttributeIntegerPtr anAttr = std::dynamic_pointer_cast<ModelAPI_AttributeInteger>(theAttr);
    if (anAttr->text().empty())
      aResult<<anAttr->value();
    else
      aResult<<anAttr->text();
  } else if (aType == ModelAPI_AttributeDouble::typeId()) {
    AttributeDoublePtr anAttr = std::dynamic_pointer_cast<ModelAPI_AttributeDouble>(theAttr);
    int aPrecision = PRECISION;
    // Special case - precision for the arc angle. It is calculated with tolerance 1e-4,
    // so the value has only 4 correct digits
    if (anAttr->id() == "ArcAngle")
      aPrecision = 1;
    if (anAttr->text().empty()) {
      double aVal = anAttr->value();
      dumpArray(aResult, &aVal, 1, aPrecision);
    } else
      aResult<<anAttr->text();
  } else if (aType == ModelAPI_AttributeBoolean::typeId()) {
    AttributeBooleanPtr anAttr = std::dynamic_pointer_cast<ModelAPI_AttributeBoolean>(theAttr);
    aResult<<anAttr->value();
  } else if (aType == ModelAPI_AttributeString::typeId()) {
    AttributeStringPtr anAttr = std::dynamic_pointer_cast<ModelAPI_AttributeString>(theAttr);
    aResult<<anAttr->value();
  } else if (aType == ModelAPI_AttributeReference::typeId()) {
    AttributeReferencePtr anAttr =
      std::dynamic_pointer_cast<ModelAPI_AttributeReference>(theAttr);
    if (anAttr->value().get()) {
      aResult<<anAttr->value()->data()->name();
    } else {
      aResult<<"__empty__";
    }
  } else if (aType == ModelAPI_AttributeSelection::typeId()) {
    AttributeSelectionPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeSelection>(theAttr);
    aResult<<anAttr->namingName();
  } else if (aType == ModelAPI_AttributeSelectionList::typeId()) {
    AttributeSelectionListPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeSelectionList>(theAttr);
    for(int a = 0; a < anAttr->size(); a++) {
      if (a != 0)
        aResult<<" ";
      aResult<<anAttr->value(a)->namingName();
    }
  } else if (aType == ModelAPI_AttributeRefAttr::typeId()) {
    AttributeRefAttrPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(theAttr);
    ObjectPtr anObj = anAttr->isObject() ? anAttr->object() : anAttr->attr()->owner();
    if (anObj.get()) {
      aResult<<anObj->data()->name();
      if (!anAttr->isObject()) {
        aResult<<" "<<anAttr->attr()->id();
      }
    } else {
      aResult<<"__empty__";
    }
  } else if (aType == ModelAPI_AttributeRefList::typeId()) {
    AttributeRefListPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeRefList>(theAttr);
    // for sketch sub-features the empty values may be skipped and order is not important
    bool isSketchFeatures = anAttr->id() == "Features" && 
      std::dynamic_pointer_cast<ModelAPI_Feature>(anAttr->owner())->getKind() == "Sketch";
    std::list<ObjectPtr> aList = anAttr->list();
    std::list<std::string> aResList; // list of resulting strings
    for(std::list<ObjectPtr>::iterator aL = aList.begin(); aL != aList.end(); aL++) {
      if (aL->get()) {
        aResList.push_back((*aL)->data()->name());
      } else if (!isSketchFeatures) {
        aResList.push_back("__empty__");
      }
    }
    if (isSketchFeatures)
      aResList.sort();
    for(std::list<std::string>::iterator aR = aResList.begin(); aR != aResList.end(); aR++) {
      aResult<<*aR<<" ";
    }
  } else if (aType == ModelAPI_AttributeRefAttrList::typeId()) {
    AttributeRefAttrListPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeRefAttrList>(theAttr);
    std::list<std::pair<ObjectPtr, AttributePtr> > aList = anAttr->list();
    std::list<std::pair<ObjectPtr, AttributePtr> >::iterator aL = aList.begin();
    for(; aL != aList.end(); aL++) {
      if (aL != aList.begin())
        aResult<<" ";
      ObjectPtr anObj = aL->second.get() ? aL->second->owner() : aL->first;
      if (anObj.get()) {
        aResult<<anObj->data()->name();
        if (aL->second.get()) {
          aResult<<" "<<aL->second->id();
        }
      } else {
        aResult<<"__empty__";
      }
    }
  } else if (aType == ModelAPI_AttributeIntArray::typeId()) {
    AttributeIntArrayPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeIntArray>(theAttr);
    for(int a = 0; a < anAttr->size(); a++)
      aResult<<anAttr->value(a)<<" ";
  } else if (aType == ModelAPI_AttributeDoubleArray::typeId()) {
    AttributeDoubleArrayPtr anAttr = 
      std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(theAttr);
    for(int a = 0; a < anAttr->size(); a++)
      aResult<<anAttr->value(a)<<" ";
  } else if (aType == GeomDataAPI_Point::typeId()) {
    AttributePointPtr anAttr = std::dynamic_pointer_cast<GeomDataAPI_Point>(theAttr);
    double aValues[3] = {anAttr->x(), anAttr->y(), anAttr->z()};
    dumpArray(aResult, aValues, 3);
  } else if (aType == GeomDataAPI_Dir::typeId()) {
    AttributeDirPtr anAttr = std::dynamic_pointer_cast<GeomDataAPI_Dir>(theAttr);
    double aValues[3] = {anAttr->x(), anAttr->y(), anAttr->z()};
    dumpArray(aResult, aValues, 3);
  } else if (aType == GeomDataAPI_Point2D::typeId()) {
    // do not dump flyout point for constraints as it may be changed unexpectedly
    if (theAttr->id() == "ConstraintFlyoutValuePnt")
      return "";
    AttributePoint2DPtr anAttr = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(theAttr);
    double aValues[2] = {anAttr->x(), anAttr->y()};
    dumpArray(aResult, aValues, 2);
  } else {
    aResult<<"__unknownattribute__";
  }
  return aResult.str();
}

std::string ModelHighAPI_FeatureStore::dumpShape(std::shared_ptr<GeomAPI_Shape>& theShape) {
  TopoDS_Shape aShape = theShape->impl<TopoDS_Shape>();
  if (aShape.IsNull()) {
    return "null";
  }
  std::ostringstream aResult;
  // output the number of shapes of different types
  TopAbs_ShapeEnum aType = TopAbs_COMPOUND;
  for(; aType <= TopAbs_VERTEX; aType = TopAbs_ShapeEnum((int)aType + 1)) {
    TopExp_Explorer anExp(aShape, aType);
    int aCount = 0;
    for(; anExp.More(); anExp.Next()) aCount++;
    TopAbs::Print(aType, aResult);
    aResult<<": "<<aCount<<std::endl;
  }
  // output the main characteristics
  if (GeomAlgoAPI_ShapeTools::volume(theShape) > 1.e-7) {
    aResult<<"Volume: "<<std::fixed<<setprecision(3)<<GeomAlgoAPI_ShapeTools::volume(theShape)<<std::endl;
  }
  std::shared_ptr<GeomAPI_Pnt> aCenter = GeomAlgoAPI_ShapeTools::centreOfMass(theShape);
  aResult<<"Center of mass: ";
  double aCenterVals[3] = {aCenter->x(), aCenter->y(), aCenter->z()};
  dumpArray(aResult, aCenterVals, 3);
  aResult<<std::endl;
  return aResult.str();
}