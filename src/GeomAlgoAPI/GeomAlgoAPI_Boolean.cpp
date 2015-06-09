// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAlgoAPI_Boolean.cpp
// Created:     02 Sept 2014
// Author:      Vitaly Smetannikov

#include "GeomAlgoAPI_Boolean.h"

#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_ListOfShape.hxx>

//=================================================================================================
std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_Boolean::makeCut(const ListOfShape& theObjects,
                                                            const ListOfShape& theTools)
{
  GeomAlgoAPI_Boolean aBoolAlgo(theObjects, theTools, BOOL_CUT);
  if(aBoolAlgo.isDone() && !aBoolAlgo.shape()->isNull() && aBoolAlgo.isValid()) {
    return aBoolAlgo.shape();
  }
  return std::shared_ptr<GeomAPI_Shape>();
}

//=================================================================================================
std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_Boolean::makeFuse(const ListOfShape& theObjects,
                                                             const ListOfShape& theTools)
{
  GeomAlgoAPI_Boolean aBoolAlgo(theObjects, theTools, BOOL_FUSE);
  if(aBoolAlgo.isDone() && !aBoolAlgo.shape()->isNull() && aBoolAlgo.isValid()) {
    return aBoolAlgo.shape();
  }
  return std::shared_ptr<GeomAPI_Shape>();
}

//=================================================================================================
std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_Boolean::makeCommon(const ListOfShape& theObjects,
                                                               const ListOfShape& theTools)
{
  GeomAlgoAPI_Boolean aBoolAlgo(theObjects, theTools, BOOL_COMMON);
  if(aBoolAlgo.isDone() && !aBoolAlgo.shape()->isNull() && aBoolAlgo.isValid()) {
    return aBoolAlgo.shape();
  }
  return std::shared_ptr<GeomAPI_Shape>();
}

//=================================================================================================
GeomAlgoAPI_Boolean::GeomAlgoAPI_Boolean(const ListOfShape& theObjects,
                                         const ListOfShape& theTools,
                                         const OperationType theOperationType)
: myDone(false),
  myShape(new GeomAPI_Shape()),
  myMap(new GeomAPI_DataMapOfShapeShape()),
  myMkShape(new GeomAlgoAPI_MakeShape())
{
  build(theObjects, theTools, theOperationType);
}


//=================================================================================================
void GeomAlgoAPI_Boolean::build(const ListOfShape& theObjects,
                                const ListOfShape& theTools,
                                const OperationType theOperationType)
{
  if(theObjects.empty() || theTools.empty()) {
    return;
  }

  // Getting objects.
  TopTools_ListOfShape anObjects;
  for(ListOfShape::const_iterator anObjectsIt = theObjects.begin(); anObjectsIt != theObjects.end(); anObjectsIt++)
  {
    anObjects.Append((*anObjectsIt)->impl<TopoDS_Shape>());
  }

  // Getting tools.
  TopTools_ListOfShape aTools;
  for(ListOfShape::const_iterator aToolsIt = theTools.begin(); aToolsIt != theTools.end(); aToolsIt++)
  {
    aTools.Append((*aToolsIt)->impl<TopoDS_Shape>());
  }

  // Creating boolean operation.
  BRepAlgoAPI_BooleanOperation* anOperation;
  switch (theOperationType) {
    case BOOL_CUT: {
      anOperation = new BRepAlgoAPI_Cut();
      break;
    }
    case BOOL_FUSE: {
      anOperation = new BRepAlgoAPI_Fuse();
      break;
    }
    case BOOL_COMMON: {
      anOperation = new BRepAlgoAPI_Common();
      break;
    }
    default: {
      return;
    }
  }
  myMkShape->setImpl(anOperation);
  anOperation->SetArguments(anObjects);
  anOperation->SetTools(aTools);

  // Building and getting result.
  anOperation->Build();
  myDone = anOperation->IsDone() == Standard_True;
  if(!myDone) {
    return;
  }
  TopoDS_Shape aResult = anOperation->Shape();

  // fill data map to keep correct orientation of sub-shapes
  for (TopExp_Explorer Exp(aResult,TopAbs_FACE); Exp.More(); Exp.Next()) {
    std::shared_ptr<GeomAPI_Shape> aCurrentShape(new GeomAPI_Shape());
    aCurrentShape->setImpl(new TopoDS_Shape(Exp.Current()));
    myMap->bind(aCurrentShape, aCurrentShape);
  }
  myShape->setImpl(new TopoDS_Shape(aResult));

}

//=================================================================================================
const bool GeomAlgoAPI_Boolean::isDone() const
{
  return myDone;
}

//=================================================================================================
const bool GeomAlgoAPI_Boolean::isValid() const
{
  BRepCheck_Analyzer aChecker(myShape->impl<TopoDS_Shape>());
  return (aChecker.IsValid() == Standard_True);
}

//=================================================================================================
const std::shared_ptr<GeomAPI_Shape>& GeomAlgoAPI_Boolean::shape() const
{
  return myShape;
}

//=================================================================================================
std::shared_ptr<GeomAPI_DataMapOfShapeShape> GeomAlgoAPI_Boolean::mapOfShapes() const
{
  return myMap;
}

//=================================================================================================
std::shared_ptr<GeomAlgoAPI_MakeShape> GeomAlgoAPI_Boolean::makeShape() const
{
  return myMkShape;
}
