// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAlgoAPI_Pipe.cpp
// Created:     16 March 2016
// Author:      Dmitry Bobylev

#include "GeomAlgoAPI_Pipe.h"

#include <GeomAPI_Dir.h>
#include <GeomAPI_Edge.h>
#include <GeomAPI_Lin.h>

#include <BRep_Tool.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <gp_Lin.hxx>
#include <NCollection_List.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

static bool getBase(TopoDS_Shape& theBaseOut,
                    TopAbs_ShapeEnum& theBaseTypeOut,
                    const std::shared_ptr<GeomAPI_Shape> theBaseShape);
static bool getPath(TopoDS_Wire& thePathOut,
                    const std::shared_ptr<GeomAPI_Shape> thePathShape);
static bool buildPipe(BRepOffsetAPI_MakePipeShell* thePipeBuilder);

//=================================================================================================
GeomAlgoAPI_Pipe::GeomAlgoAPI_Pipe(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                                   const std::shared_ptr<GeomAPI_Shape> thePathShape)
: /*myIsPipeShellUsed(false),*/
  myBaseShape(theBaseShape),
  myPathShape(thePathShape)
{
  build(theBaseShape, thePathShape);
}

//=================================================================================================
GeomAlgoAPI_Pipe::GeomAlgoAPI_Pipe(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                                   const std::shared_ptr<GeomAPI_Shape> thePathShape,
                                   const std::shared_ptr<GeomAPI_Shape> theBiNormal)
//: myIsPipeShellUsed(true)
{
  build(theBaseShape, thePathShape, theBiNormal);
}

//=================================================================================================
GeomAlgoAPI_Pipe::GeomAlgoAPI_Pipe(const ListOfShape& theBaseShapes,
                                   const ListOfShape& theLocations,
                                   const std::shared_ptr<GeomAPI_Shape> thePathShape)
//: myIsPipeShellUsed(true)
{
  build(theBaseShapes, theLocations, thePathShape);
}

//=================================================================================================
void GeomAlgoAPI_Pipe::build(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                             const std::shared_ptr<GeomAPI_Shape> thePathShape)
{
  // Getting base shape.
  if(!theBaseShape.get()) {
    return;
  }
  TopoDS_Shape aBaseShape = theBaseShape->impl<TopoDS_Shape>();
  if(aBaseShape.IsNull()) {
    return;
  }
  TopAbs_ShapeEnum aBaseShapeType = aBaseShape.ShapeType();
  if(aBaseShapeType != TopAbs_VERTEX && aBaseShapeType != TopAbs_EDGE &&
     aBaseShapeType != TopAbs_WIRE && aBaseShapeType != TopAbs_FACE &&
     aBaseShapeType != TopAbs_SHELL) {
    return;
  }

  // Getting path.
  TopoDS_Wire aPathWire;
  if(!getPath(aPathWire, thePathShape)) {
    return;
  }

  // Making pipe.
  BRepOffsetAPI_MakePipe* aPipeBuilder = new BRepOffsetAPI_MakePipe(aPathWire, aBaseShape);
  if(!aPipeBuilder) {
    return;
  }
  aPipeBuilder->Build();

  // Checking result.
  if(!aPipeBuilder->IsDone() || aPipeBuilder->Shape().IsNull()) {
    delete aPipeBuilder;
    return;
  }
  this->initialize(aPipeBuilder);

  // Setting naming.
  std::shared_ptr<GeomAPI_Shape> aFromShape(new GeomAPI_Shape), aToShape(new GeomAPI_Shape);
  aFromShape->setImpl(new TopoDS_Shape(aPipeBuilder->FirstShape()));
  aToShape->setImpl(new TopoDS_Shape(aPipeBuilder->LastShape()));
  this->addFromShape(aFromShape);
  this->addToShape(aToShape);

  // Setting result.
  TopoDS_Shape aResultShape = aPipeBuilder->Shape();
  std::shared_ptr<GeomAPI_Shape> aResultGeomShape(new GeomAPI_Shape());
  aResultGeomShape->setImpl(new TopoDS_Shape(aResultShape));
  this->setShape(aResultGeomShape);
  this->setDone(true);
}

//=================================================================================================
void GeomAlgoAPI_Pipe::build(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                             const std::shared_ptr<GeomAPI_Shape> thePathShape,
                             const std::shared_ptr<GeomAPI_Shape> theBiNormal)
{
  // Getting base shape.
  TopoDS_Shape aBaseShape;
  TopAbs_ShapeEnum aBaseShapeType;
  if(!getBase(aBaseShape, aBaseShapeType, theBaseShape)) {
    return;
  }

  // Getting path.
  TopoDS_Wire aPathWire;
  if(!getPath(aPathWire, thePathShape)) {
    return;
  }

  // Getting Bi-Normal.
  if(!theBiNormal.get()) {
    return;
  }
  TopoDS_Shape aBiNormalShape = theBiNormal->impl<TopoDS_Shape>();
  if(aBiNormalShape.IsNull() || aBiNormalShape.ShapeType() != TopAbs_EDGE) {
    return;
  }
  TopoDS_Edge aBiNormalEdge = TopoDS::Edge(aBiNormalShape);
  Standard_Real aFirst, aLast;
  Handle(Geom_Curve) aBiNormalCurve = BRep_Tool::Curve(aBiNormalEdge, aFirst, aLast);
  Handle(Geom_Line) aBiNormalLine = Handle(Geom_Line)::DownCast(aBiNormalCurve);
  if(aBiNormalLine.IsNull()) {
    return;
  }
  gp_Dir aBiNormalDir = aBiNormalLine->Lin().Direction();

  // Making pipe.
  BRepOffsetAPI_MakePipeShell* aPipeBuilder = new BRepOffsetAPI_MakePipeShell(aPathWire);
  if(!aPipeBuilder) {
    return;
  }
  aPipeBuilder->Add(aBaseShape);
  aPipeBuilder->SetMode(aBiNormalDir);
  if(!buildPipe(aPipeBuilder)) {
    delete aPipeBuilder;
    return;
  }
  this->initialize(aPipeBuilder);

  // Checking result.
  if(aBaseShapeType == TopAbs_FACE) {
    if(aPipeBuilder->MakeSolid() == Standard_False) {
      return;
    }
  }
  if(aPipeBuilder->Shape().IsNull()) {
    return;
  }

  // Setting naming.
  std::shared_ptr<GeomAPI_Shape> aFromShape(new GeomAPI_Shape), aToShape(new GeomAPI_Shape);
  aFromShape->setImpl(new TopoDS_Shape(aPipeBuilder->FirstShape()));
  aToShape->setImpl(new TopoDS_Shape(aPipeBuilder->LastShape()));
  this->addFromShape(aFromShape);
  this->addToShape(aToShape);

  // Setting result.
  TopoDS_Shape aResultShape = aPipeBuilder->Shape();
  std::shared_ptr<GeomAPI_Shape> aResultGeomShape(new GeomAPI_Shape());
  aResultGeomShape->setImpl(new TopoDS_Shape(aResultShape));
  this->setShape(aResultGeomShape);
  this->setDone(true);
}

//=================================================================================================
void GeomAlgoAPI_Pipe::build(const ListOfShape& theBaseShapes,
                             const ListOfShape& theLocations,
                             const std::shared_ptr<GeomAPI_Shape> thePathShape)
{
  if(theBaseShapes.empty() || (!theLocations.empty() && theLocations.size() != theBaseShapes.size())) {
    return;
  }

  bool aHasLocations = false;
  if(!theLocations.empty()) {
    aHasLocations = true;
  }

  // Getting path.
  TopoDS_Wire aPathWire;
  if(!getPath(aPathWire, thePathShape)) {
    return;
  }

  // Making pipe.
  BRepOffsetAPI_MakePipeShell* aPipeBuilder = new BRepOffsetAPI_MakePipeShell(aPathWire);
  if(!aPipeBuilder) {
    return;
  }
  bool anIsSolidNeeded = false;
  ListOfShape::const_iterator aBaseIt = theBaseShapes.cbegin();
  ListOfShape::const_iterator aLocIt = theLocations.cbegin();
  while(aBaseIt != theBaseShapes.cend()) {
    std::shared_ptr<GeomAPI_Shape> aBase = *aBaseIt;
    TopoDS_Shape aBaseShape;
    TopAbs_ShapeEnum aBaseShapeType;
    if(!getBase(aBaseShape, aBaseShapeType, aBase)) {
      delete aPipeBuilder;
      return;
    }
    ++aBaseIt;
    if(aBaseShapeType == TopAbs_FACE) {
      anIsSolidNeeded = true;
    }

    if(aHasLocations) {
      std::shared_ptr<GeomAPI_Shape> aLocation = *aLocIt;
      if(!aLocation.get() || aLocation->shapeType() != GeomAPI_Shape::VERTEX) {
        delete aPipeBuilder;
        return;
      }
      TopoDS_Vertex aLocationVertex = aLocation->impl<TopoDS_Vertex>();
      ++aLocIt;
      aPipeBuilder->Add(aBaseShape, aLocationVertex);
    } else {
      aPipeBuilder->Add(aBaseShape);
    }
  }

  if(aPipeBuilder->IsReady() == Standard_False) {
    delete aPipeBuilder;
    return;
  }

  if(!buildPipe(aPipeBuilder)) {
    delete aPipeBuilder;
    return;
  }
  this->initialize(aPipeBuilder);

  // Checking result.
  if(anIsSolidNeeded) {
    if(aPipeBuilder->MakeSolid() == Standard_False) {
      return;
    }
  }
  if(aPipeBuilder->Shape().IsNull()) {
    return;
  }

  // Setting naming.
  std::shared_ptr<GeomAPI_Shape> aFromShape(new GeomAPI_Shape), aToShape(new GeomAPI_Shape);
  aFromShape->setImpl(new TopoDS_Shape(aPipeBuilder->FirstShape()));
  aToShape->setImpl(new TopoDS_Shape(aPipeBuilder->LastShape()));
  this->addFromShape(aFromShape);
  this->addToShape(aToShape);

  // Setting result.
  TopoDS_Shape aResultShape = aPipeBuilder->Shape();
  std::shared_ptr<GeomAPI_Shape> aResultGeomShape(new GeomAPI_Shape());
  aResultGeomShape->setImpl(new TopoDS_Shape(aResultShape));
  this->setShape(aResultGeomShape);
  this->setDone(true);
}

//=================================================================================================
void GeomAlgoAPI_Pipe::generated(const std::shared_ptr<GeomAPI_Shape> theShape,
                                 ListOfShape& theHistory)
{
  GeomAlgoAPI_MakeShape::generated(theShape, theHistory);

  //if(myIsPipeShellUsed) {
  //  GeomAlgoAPI_MakeShape::generated(theShape, theHistory);
  //  return;
  //}

  //BRepOffsetAPI_MakePipe* aMakePipe = implPtr<BRepOffsetAPI_MakePipe>();
  //const TopoDS_Shape& aProfile = theShape->impl<TopoDS_Shape>();
  //const TopAbs_ShapeEnum aProfileShapeType = aProfile.ShapeType();
  //if(aProfileShapeType != TopAbs_VERTEX && aProfileShapeType != TopAbs_EDGE) {
  //  return;
  //}
  //const TopoDS_Shape& aBaseShape = myBaseShape->impl<TopoDS_Shape>();
  //TopExp_Explorer anExp(aBaseShape, aProfileShapeType);
  //Standard_Boolean ahasShape = Standard_False;
  //for(; anExp.More(); anExp.Next()) {
  //  if(anExp.Current().IsSame(aProfile)) {
  //    ahasShape = Standard_True;
  //    break;
  //  }
  //}
  //if(!ahasShape) {
  //  return;
  //}
  //TopExp_Explorer aShapeExplorer(myPathShape->impl<TopoDS_Shape>(), TopAbs_EDGE);
  //for(; aShapeExplorer.More(); aShapeExplorer.Next ()) {
  //  const TopoDS_Shape& aSpine = aShapeExplorer.Current();
  //  const TopoDS_Shape& aGeneratedShape = aMakePipe->Generated(aSpine, aProfile);
  //  if(aGeneratedShape.IsNull()) {
  //    continue;
  //  }
  //  std::shared_ptr<GeomAPI_Shape> aShape(new GeomAPI_Shape());
  //  aShape->setImpl(new TopoDS_Shape(aGeneratedShape));
  //  theHistory.push_back(aShape);
  //}
}

// Auxilary functions:
//=================================================================================================
bool getBase(TopoDS_Shape& theBaseOut,
             TopAbs_ShapeEnum& theBaseTypeOut,
             const std::shared_ptr<GeomAPI_Shape> theBaseShape)
{
  if(!theBaseShape.get()) {
    return false;
  }

  theBaseOut = theBaseShape->impl<TopoDS_Shape>();
  if(theBaseOut.IsNull()) {
    return false;
  }
  theBaseTypeOut = theBaseOut.ShapeType();
  if(theBaseTypeOut == TopAbs_VERTEX) {
    // Do nothing.
  } else if(theBaseTypeOut == TopAbs_EDGE) {
    theBaseOut = BRepBuilderAPI_MakeWire(TopoDS::Edge(theBaseOut)).Shape();
  } else if(theBaseTypeOut == TopAbs_WIRE) {
    // Do nothing.
  } else if(theBaseTypeOut == TopAbs_FACE) {
    TopExp_Explorer anExp(theBaseOut, TopAbs_WIRE);
    theBaseOut = anExp.Current();
  } else {
    return false;
  }

  return true;
}

//=================================================================================================
bool getPath(TopoDS_Wire& thePathOut,
             const std::shared_ptr<GeomAPI_Shape> thePathShape)
{
  if(!thePathShape.get()) {
    return false;
  }

  TopoDS_Shape aPathShape = thePathShape->impl<TopoDS_Shape>();
  if(aPathShape.IsNull()) {
    return false;
  }
  TopAbs_ShapeEnum aPathShapeType = aPathShape.ShapeType();
  if(aPathShapeType == TopAbs_EDGE) {
    TopoDS_Edge aPathEdge = TopoDS::Edge(aPathShape);
    thePathOut = BRepBuilderAPI_MakeWire(aPathEdge).Wire();
  } else if(aPathShapeType == TopAbs_WIRE) {
    thePathOut = TopoDS::Wire(aPathShape);
  } else {
    return false;
  }

  return true;
}

//=================================================================================================
bool buildPipe(BRepOffsetAPI_MakePipeShell* thePipeBuilder)
{
  thePipeBuilder->Build();

  Standard_Boolean isDone = thePipeBuilder->IsDone();

  if (!isDone) {
    // Try to use Descrete Trihedron mode.
    thePipeBuilder->SetDiscreteMode();
    thePipeBuilder->Build();
    isDone = thePipeBuilder->IsDone();
  }

  return isDone == Standard_True;
}