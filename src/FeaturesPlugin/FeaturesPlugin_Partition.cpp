// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#include "FeaturesPlugin_Partition.h"

#include <ModelAPI_AttributeBoolean.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeSelectionList.h>
#include <ModelAPI_BodyBuilder.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_ResultBody.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Tools.h>
#include <ModelAPI_Validator.h>

#include <GeomAlgoAPI_Boolean.h>
#include <GeomAlgoAPI_CompoundBuilder.h>
#include <GeomAlgoAPI_MakeShapeCustom.h>
#include <GeomAlgoAPI_MakeShapeList.h>
#include <GeomAlgoAPI_Partition.h>
#include <GeomAlgoAPI_ShapeTools.h>

#include <GeomAPI_Face.h>
#include <GeomAPI_ShapeExplorer.h>
#include <GeomAPI_ShapeIterator.h>

#include <iostream>
#include <list>
#include <sstream>

typedef std::list<std::pair<GeomShapePtr, ListOfShape> > CompsolidSubs;

static GeomShapePtr findBase(const GeomShapePtr theObjectShape,
                             const GeomShapePtr theResultShape,
                             const GeomAPI_Shape::ShapeType theShapeType,
                             const std::shared_ptr<GeomAlgoAPI_MakeShape> theMakeShape);

static void pullObjectsAndPlanes(const AttributeSelectionListPtr& theSelectedList,
                                 CompsolidSubs& theObjects, ListOfShape& thePlanes);

static void resizePlanes(const CompsolidSubs& theObjects, ListOfShape& thePlanes,
                         std::shared_ptr<GeomAlgoAPI_MakeShapeList>& theMakeShapeList);

static void unusedSubsOfComposolid(const CompsolidSubs& theObjects, CompsolidSubs& theNotUsed);

static bool cutUnusedSubs(CompsolidSubs& theObjects, CompsolidSubs& theNotUsed,
                          std::shared_ptr<GeomAlgoAPI_MakeShapeList>& theMakeShapeList,
                          std::string& theError);

static bool isAlgoFailed(const std::shared_ptr<GeomAlgoAPI_MakeShape>& theAlgo,
                         std::string& theError);


//=================================================================================================
FeaturesPlugin_Partition::FeaturesPlugin_Partition()
{
}

//=================================================================================================
void FeaturesPlugin_Partition::initAttributes()
{
  data()->addAttribute(BASE_OBJECTS_ID(), ModelAPI_AttributeSelectionList::typeId());
}

//=================================================================================================
void FeaturesPlugin_Partition::execute()
{
  CompsolidSubs anObjects;
  ListOfShape aPlanes;

  // Getting objects.
  pullObjectsAndPlanes(selectionList(BASE_OBJECTS_ID()), anObjects, aPlanes);
  if(anObjects.empty()) {
    static const std::string aFeatureError = "Error: No objects for partition.";
    setError(aFeatureError);
    return;
  }

  ListOfShape aBaseObjects;
  for (CompsolidSubs::iterator anIt = anObjects.begin(); anIt != anObjects.end(); ++anIt)
    aBaseObjects.insert(aBaseObjects.end(), anIt->second.begin(), anIt->second.end());
  aBaseObjects.insert(aBaseObjects.end(), aPlanes.begin(), aPlanes.end());

  // resize planes to the bounding box of operated shapes
  std::shared_ptr<GeomAlgoAPI_MakeShapeList> aMakeShapeList(new GeomAlgoAPI_MakeShapeList());
  resizePlanes(anObjects, aPlanes, aMakeShapeList);

  // cut unused solids of composolids from the objects of partition
  CompsolidSubs anUnusedSubs;
  unusedSubsOfComposolid(anObjects, anUnusedSubs);
  for (CompsolidSubs::iterator anIt = anUnusedSubs.begin(); anIt != anUnusedSubs.end(); ++anIt)
    aBaseObjects.insert(aBaseObjects.end(), anIt->second.begin(), anIt->second.end());

  std::string aError;
  if (!cutUnusedSubs(anObjects, anUnusedSubs, aMakeShapeList, aError)) {
    setError(aError);
    return;
  }

  // perform partition first time to split target solids
  ListOfShape aTargetObjects;
  for (CompsolidSubs::iterator anIt = anObjects.begin(); anIt != anObjects.end(); ++anIt)
    aTargetObjects.insert(aTargetObjects.end(), anIt->second.begin(), anIt->second.end());

  std::shared_ptr<GeomAlgoAPI_Partition> aPartitionAlgo(
    new GeomAlgoAPI_Partition(aTargetObjects, aPlanes));

  // Checking that the algorithm worked properly.
  if (isAlgoFailed(aPartitionAlgo, aError)) {
    setError(aError);
    return;
  }

  aMakeShapeList->appendAlgo(aPartitionAlgo);
  GeomShapePtr aResultShape = aPartitionAlgo->shape();

  if (!anUnusedSubs.empty()) {
    // second pass of a partition to split shared faces of compsolids
    aTargetObjects.clear();
    aTargetObjects.push_back(aResultShape);
    for (CompsolidSubs::iterator anIt = anUnusedSubs.begin(); anIt != anUnusedSubs.end(); ++anIt)
      aTargetObjects.insert(aTargetObjects.end(), anIt->second.begin(), anIt->second.end());

    aPartitionAlgo.reset(new GeomAlgoAPI_Partition(aTargetObjects, ListOfShape()));

    // Checking that the algorithm worked properly.
    if (isAlgoFailed(aPartitionAlgo, aError)) {
      setError(aError);
      return;
    }

    aMakeShapeList->appendAlgo(aPartitionAlgo);
    aResultShape = aPartitionAlgo->shape();
  }

  int aResultIndex = 0;
  if(aResultShape->shapeType() == GeomAPI_Shape::COMPOUND) {
    for(GeomAPI_ShapeIterator anIt(aResultShape); anIt.more(); anIt.next()) {
      storeResult(aBaseObjects, aPlanes, anIt.current(), aMakeShapeList, aResultIndex);
      ++aResultIndex;
    }
  } else {
    storeResult(aBaseObjects, aPlanes, aResultShape, aMakeShapeList, aResultIndex);
    ++aResultIndex;
  }

  // Remove the rest results if there were produced in the previous pass.
  removeResults(aResultIndex);
}

//=================================================================================================
void FeaturesPlugin_Partition::storeResult(
  ListOfShape& theObjects, ListOfShape& thePlanes,
  const GeomShapePtr theResultShape,
  const std::shared_ptr<GeomAlgoAPI_MakeShape> theMakeShape,
  const int theIndex)
{
  // Find base. The most complicated is the real modified object (#1799 if box is partitioned by
  // two planes the box is the base, not planes, independently on the order in the list).
  GeomShapePtr aBaseShape;
  for(ListOfShape::const_iterator anIt = theObjects.cbegin(); anIt != theObjects.cend(); ++anIt) {
    GeomShapePtr anObjectShape = *anIt;
    GeomShapePtr aCandidate =
      findBase(anObjectShape, theResultShape, GeomAPI_Shape::VERTEX, theMakeShape);
    if(!aCandidate.get()) {
      aCandidate = findBase(anObjectShape, theResultShape, GeomAPI_Shape::EDGE, theMakeShape);
    }
    if (!aCandidate.get())
      aCandidate = findBase(anObjectShape, theResultShape, GeomAPI_Shape::FACE, theMakeShape);

    if(aCandidate.get()) {
      if (!aBaseShape.get() || aBaseShape->shapeType() > aCandidate->shapeType()) {
        aBaseShape = aCandidate;
      }
    }
  }

  // Create result body.
  ResultBodyPtr aResultBody = document()->createBody(data(), theIndex);

  // Store modified shape.
  if(!aBaseShape.get() || aBaseShape->isEqual(theResultShape)) {
    aResultBody->store(theResultShape, false);
    setResult(aResultBody, theIndex);
    return;
  }

  const int aDelTag = 1;
  /// sub solids will be placed at labels 3, 4, etc. if result is compound of solids
  const int aSubTag = 2;
  int aModTag = aSubTag + 10000;
  const std::string aModName = "Modified";

  aResultBody->storeModified(aBaseShape, theResultShape, aSubTag);

  std::shared_ptr<GeomAPI_DataMapOfShapeShape> aMapOfSubShapes = theMakeShape->mapOfSubShapes();
  theObjects.insert(theObjects.end(), thePlanes.begin(), thePlanes.end());
  int anIndex = 1;
  for(ListOfShape::const_iterator anIt = theObjects.cbegin(); anIt != theObjects.cend(); ++anIt) {
    GeomShapePtr aShape = *anIt;
    std::string aModEdgeName = aModName + "_Edge_" + std::to_string((long long)anIndex);
    aResultBody->loadAndOrientModifiedShapes(theMakeShape.get(), aShape, GeomAPI_Shape::EDGE,
      aModTag, aModEdgeName, *aMapOfSubShapes.get(), false, true, true);
    std::string aModFaceName = aModName + "_Face_" + std::to_string((long long)anIndex++);
    aResultBody->loadAndOrientModifiedShapes(theMakeShape.get(), aShape, GeomAPI_Shape::FACE,
      aModTag + 1, aModFaceName, *aMapOfSubShapes.get(), false, true, true);
    aResultBody->loadDeletedShapes(theMakeShape.get(), aShape, GeomAPI_Shape::FACE, aDelTag);
  }

  setResult(aResultBody, theIndex);
}


//=================     Auxiliary functions     ===================================================

GeomShapePtr findBase(const GeomShapePtr theObjectShape,
                      const GeomShapePtr theResultShape,
                      const GeomAPI_Shape::ShapeType theShapeType,
                      const std::shared_ptr<GeomAlgoAPI_MakeShape> theMakeShape)
{
  GeomShapePtr aBaseShape;
  std::shared_ptr<GeomAPI_DataMapOfShapeShape> aMapOfSubShapes = theMakeShape->mapOfSubShapes();
  for(GeomAPI_ShapeExplorer anObjectSubShapesExp(theObjectShape, theShapeType);
      anObjectSubShapesExp.more();
      anObjectSubShapesExp.next()) {
    GeomShapePtr anObjectSubShape = anObjectSubShapesExp.current();
    ListOfShape aModifiedShapes;
    theMakeShape->modified(anObjectSubShape, aModifiedShapes);
    for(ListOfShape::const_iterator
        aModIt = aModifiedShapes.cbegin(); aModIt != aModifiedShapes.cend(); ++aModIt) {
      GeomShapePtr aModShape = *aModIt;
      if(aMapOfSubShapes->isBound(aModShape)) {
        aModShape = aMapOfSubShapes->find(aModShape);
      }
      if(theResultShape->isSubShape(aModShape)) {
        aBaseShape = theObjectShape;
        break;
      }
    }
    if(aBaseShape.get()) {
      break;
    }
  }

  return aBaseShape;
}

static CompsolidSubs::iterator findOrAdd(CompsolidSubs& theList, const GeomShapePtr& theCompsolid)
{
  CompsolidSubs::iterator aFound = theList.begin();
  for (; aFound != theList.end(); ++aFound)
    if (aFound->first == theCompsolid)
      break;
  if (aFound == theList.end()) {
    theList.push_back(std::pair<GeomShapePtr, ListOfShape>(theCompsolid, ListOfShape()));
    aFound = --theList.end();
  }
  return aFound;
}

void pullObjectsAndPlanes(const AttributeSelectionListPtr& theSelectedList,
                          CompsolidSubs& theObjects, ListOfShape& thePlanes)
{
  std::map<ResultBodyPtr, GeomShapePtr> aMapCompsolidShape;

  int aSize = theSelectedList->size();
  for (int anIndex = 0; anIndex < aSize; ++anIndex) {
    AttributeSelectionPtr anObjectAttr = theSelectedList->value(anIndex);
    ResultPtr aContext = anObjectAttr->context();
    GeomShapePtr anObject = anObjectAttr->value();
    if (anObject) {
      GeomShapePtr anOwnerShape = anObject;
      // check the result is a compsolid and store all used subs in a single list
      ResultBodyPtr aResCompSolidPtr = ModelAPI_Tools::bodyOwner(aContext);
      if (aResCompSolidPtr && aResCompSolidPtr->shape()->shapeType() == GeomAPI_Shape::COMPSOLID) {
        std::map<ResultBodyPtr, GeomShapePtr>::const_iterator
            aFound = aMapCompsolidShape.find(aResCompSolidPtr);
        if (aFound != aMapCompsolidShape.end())
          anOwnerShape = aFound->second;
        else {
          anOwnerShape = aResCompSolidPtr->shape();
          aMapCompsolidShape[aResCompSolidPtr] = anOwnerShape;
        }
      }

      CompsolidSubs::iterator aFound = findOrAdd(theObjects, anOwnerShape);
      aFound->second.push_back(anObject);
    }
    else {
      // It could be a construction plane.
      thePlanes.push_back(anObjectAttr->context()->shape());
    }
  }
}

void resizePlanes(const CompsolidSubs& theObjects, ListOfShape& thePlanes,
                  std::shared_ptr<GeomAlgoAPI_MakeShapeList>& theMakeShapeList)
{
  ListOfShape aSolidsInOperation;
  for (CompsolidSubs::const_iterator anIt = theObjects.begin(); anIt != theObjects.end(); ++anIt)
    aSolidsInOperation.insert(aSolidsInOperation.end(), anIt->second.begin(), anIt->second.end());

  std::list<std::shared_ptr<GeomAPI_Pnt> > aBoundingPoints =
      GeomAlgoAPI_ShapeTools::getBoundingBox(aSolidsInOperation, 1.0);

  ListOfShape aPlanesCopy = thePlanes;
  thePlanes.clear();

  // Resize planes to fit in bounding box
  for (ListOfShape::const_iterator anIt = aPlanesCopy.begin(); anIt != aPlanesCopy.end(); ++anIt) {
    GeomShapePtr aPlane = *anIt;
    GeomShapePtr aTool = GeomAlgoAPI_ShapeTools::fitPlaneToBox(aPlane, aBoundingPoints);
    std::shared_ptr<GeomAlgoAPI_MakeShapeCustom> aMkShCustom(new GeomAlgoAPI_MakeShapeCustom);
    aMkShCustom->addModified(aPlane, aTool);
    theMakeShapeList->appendAlgo(aMkShCustom);
    thePlanes.push_back(aTool);
  }
}

void unusedSubsOfComposolid(const CompsolidSubs& theObjects, CompsolidSubs& theNotUsed)
{
  for (CompsolidSubs::const_iterator aCSIt = theObjects.begin();
       aCSIt != theObjects.end(); ++aCSIt) {
    if (aCSIt->first->shapeType() != GeomAPI_Shape::COMPSOLID)
      continue;

    // check the compsolid is selected
    if (aCSIt->second.size() == 1 && aCSIt->first->isEqual(aCSIt->second.front()))
      continue;

    // process all sub-solids of compsolid
    ListOfShape aNotUsedSolids;
    for (GeomAPI_ShapeExplorer anExp(aCSIt->first, GeomAPI_Shape::SOLID);
         anExp.more(); anExp.next()) {
      GeomShapePtr aSolidInCompSolid = anExp.current();
      ListOfShape::const_iterator anIt = aCSIt->second.begin();
      for (; anIt != aCSIt->second.end(); ++anIt)
        if (aSolidInCompSolid->isEqual(*anIt))
          break;

      if (anIt == aCSIt->second.end())
        aNotUsedSolids.push_back(aSolidInCompSolid);
    }

    if (!aNotUsedSolids.empty())
      theNotUsed.push_back(std::pair<GeomShapePtr, ListOfShape>(aCSIt->first, aNotUsedSolids));
  }
}

bool cutUnusedSubs(CompsolidSubs& theObjects, CompsolidSubs& theNotUsed,
                   std::shared_ptr<GeomAlgoAPI_MakeShapeList>& theMakeShapeList,
                   std::string& theError)
{
  std::shared_ptr<GeomAlgoAPI_MakeShape> aCutAlgo;

  for (CompsolidSubs::iterator anObjIt = theObjects.begin();
       anObjIt != theObjects.end(); ++anObjIt) {
    // get list of unused subs of composolids except the current
    ListOfShape aTools;
    for (CompsolidSubs::const_iterator aUIt = theNotUsed.begin();
         aUIt != theNotUsed.end(); ++aUIt) {
      if (aUIt->first != anObjIt->first)
        aTools.insert(aTools.end(), aUIt->second.begin(), aUIt->second.end());
    }
    if (aTools.empty())
      continue;

    // cut from current list of solids
    aCutAlgo.reset(
        new GeomAlgoAPI_Boolean(anObjIt->second, aTools, GeomAlgoAPI_Boolean::BOOL_CUT));
    if (isAlgoFailed(aCutAlgo, theError))
      return false;
    theMakeShapeList->appendAlgo(aCutAlgo);

    // update list of objects of the partition
    GeomAPI_Shape::ShapeType aType = anObjIt->second.front()->shapeType();
    anObjIt->second.clear();
    for (GeomAPI_ShapeExplorer anExp(aCutAlgo->shape(), aType); anExp.more(); anExp.next())
      anObjIt->second.push_back(anExp.current());
  }
  return true;
}

bool isAlgoFailed(const std::shared_ptr<GeomAlgoAPI_MakeShape>& theAlgo, std::string& theError)
{
  if (!theAlgo->isDone()) {
    theError = "Error: Partition algorithm failed.";
    return true;
  }
  if (theAlgo->shape()->isNull()) {
    theError = "Error: Resulting shape is Null.";
    return true;
  }
  if (!theAlgo->isValid()) {
    theError = "Error: Resulting shape is not valid.";
    return true;
  }

  theError.clear();
  return false;
}
