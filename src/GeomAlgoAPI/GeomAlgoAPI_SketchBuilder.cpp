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

#include <GeomAlgoAPI_SketchBuilder.h>
#include <GeomAPI_PlanarEdges.h>

#include <BOPAlgo_Builder.hxx>
#include <BRep_Builder.hxx>
#include <BRepTopAdaptor_FClass2d.hxx>
#include <Geom_Plane.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Precision.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

#include <list>
#include <cmath>
#include <algorithm>

static TopoDS_Vertex findStartVertex(const TopoDS_Shape& theShape)
{
  static const double aTol = Precision::PConfusion();

  TopExp_Explorer anExp(theShape, TopAbs_VERTEX);
  TopoDS_Vertex aStart = TopoDS::Vertex(anExp.Current());
  gp_Pnt aStartPnt(BRep_Tool::Pnt(aStart));
  TopoDS_Vertex aCurrent;
  gp_Pnt aCurrentPnt;

  for (anExp.Next(); anExp.More(); anExp.Next()) {
    aCurrent = TopoDS::Vertex(anExp.Current());
    aCurrentPnt = BRep_Tool::Pnt(aCurrent);
    if ((aCurrentPnt.X() > aStartPnt.X() + aTol) ||
        (aCurrentPnt.X() > aStartPnt.X() - aTol && aCurrentPnt.Y() > aStartPnt.Y() + aTol) ||
        (aCurrentPnt.X() > aStartPnt.X() - aTol && aCurrentPnt.Y() > aStartPnt.Y() - aTol &&
            aCurrentPnt.Z() > aStartPnt.Z() + aTol)) {
      aStart = aCurrent;
      aStartPnt = aCurrentPnt;
    }
  }
  return aStart;
}

static TopoDS_Vertex findStartVertex(const TopoDS_Shape& theShape,
    const std::list<std::shared_ptr<GeomAPI_Shape> >& theInitialShapes)
{
  // Try to find edge lying on the one of original edges.
  // First found edge will be taken as a start edge for the result wire
  std::list<std::shared_ptr<GeomAPI_Shape> >::const_iterator aFeatIt = theInitialShapes.begin();
  for (; aFeatIt != theInitialShapes.end(); aFeatIt++) {
    std::shared_ptr<GeomAPI_Shape> aShape(*aFeatIt);
    const TopoDS_Edge& anEdge = aShape->impl<TopoDS_Edge>();
    if (anEdge.ShapeType() != TopAbs_EDGE)
      continue;

    double aFirst, aLast;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(anEdge, aFirst, aLast);
    if (aCurve->DynamicType() == STANDARD_TYPE(Geom_TrimmedCurve))
      aCurve = Handle(Geom_TrimmedCurve)::DownCast(aCurve)->BasisCurve();

    TopExp_Explorer anExp(theShape, TopAbs_EDGE);
    for (; anExp.More(); anExp.Next()) {
      const TopoDS_Edge& aShapeEdge = TopoDS::Edge(anExp.Current());
      double aF, aL;
      Handle(Geom_Curve) aShapeCurve = BRep_Tool::Curve(aShapeEdge, aF, aL);
      if (aShapeCurve->DynamicType() == STANDARD_TYPE(Geom_TrimmedCurve))
        aShapeCurve = Handle(Geom_TrimmedCurve)::DownCast(aShapeCurve)->BasisCurve();

      if (aCurve != aShapeCurve)
        continue;

      // the edge is found, search vertex
      TopoDS_Vertex aV1, aV2;
      TopExp::Vertices(aShapeEdge, aV1, aV2);
      return fabs(aF - aFirst) <= fabs(aL - aFirst) ? aV1 : aV2;
    }
  }

  // start vertex is not found, use algorithm to search vertex with the greatest coordinates
  return findStartVertex(theShape);
}

// returns true if the first shape must be located earlier than the second
bool isFirst(const TopoDS_Shape& theFirst, const TopoDS_Shape& theSecond,
  NCollection_DataMap<TopoDS_Shape, NCollection_Array1<int> >& theAreaToIndex,
  const NCollection_DataMap<Handle(Geom_Curve), int>& theCurveToIndex)
{
  // fill theAreaToIndex for both shapes if needed
  for(int aShapeNum = 1; aShapeNum <= 2; aShapeNum++) {
    TopoDS_Shape aShape = aShapeNum == 1 ? theFirst : theSecond;
    if (!theAreaToIndex.IsBound(aShape)) { // fill the list of curve indices
      NCollection_List<int> aNewList;
      TopExp_Explorer anEdgesExp(aShape, TopAbs_EDGE);
      for(; anEdgesExp.More(); anEdgesExp.Next()) {
        double aFirst, aLast;
        Handle(Geom_Curve) aCurve = BRep_Tool::Curve(
          TopoDS::Edge(anEdgesExp.Current()), aFirst, aLast);
        if (aCurve->DynamicType() == STANDARD_TYPE(Geom_TrimmedCurve))
          aCurve = Handle(Geom_TrimmedCurve)::DownCast(aCurve)->BasisCurve();
        if (theCurveToIndex.IsBound(aCurve)) {
          aNewList.Append(theCurveToIndex.Find(aCurve));
        }
      }
      NCollection_Array1<int> aNewArray(1, aNewList.Extent());
      NCollection_List<int>::Iterator aListIter(aNewList);
      for(int anIndex = 1; aListIter.More(); aListIter.Next(), anIndex++) {
        aNewArray.SetValue(anIndex, aListIter.Value());
      }
      std::sort(aNewArray.begin(), aNewArray.end());
      theAreaToIndex.Bind(aShape, aNewArray);
    }
  }
  // compare lists of indices one by one to find chich list indices are lower
  NCollection_Array1<int>::Iterator aFirstList(theAreaToIndex.ChangeFind(theFirst));
  NCollection_Array1<int>::Iterator aSecondList(theAreaToIndex.ChangeFind(theSecond));
  for(; aFirstList.More() && aSecondList.More(); aFirstList.Next(), aSecondList.Next()) {
    if (aFirstList.Value() < aSecondList.Value()) return true;
    if (aFirstList.Value() > aSecondList.Value()) return false;
  }
  // if faces are identical by curves names (circle splitted by line in seam-point), use parameters
  if (!aFirstList.More() && !aSecondList.More()) {
    GProp_GProps aGProps;
    BRepGProp::SurfaceProperties(theFirst, aGProps);
    gp_Pnt aCentre1 = aGProps.CentreOfMass();
    BRepGProp::SurfaceProperties(theSecond, aGProps);
    gp_Pnt aCentre2 = aGProps.CentreOfMass();
    return aCentre1.X() + aCentre1.Y() + aCentre1.Z() < aCentre2.X() + aCentre2.Y() + aCentre2.Z();
  }
  // if in first list there is no elements left, it is the first
  return !aFirstList.More();
}

// sorts faces (in theAreas list) to make persistent order: by initial shapes edges
static void sortFaces(TopTools_ListOfShape& theAreas,
  const std::list<std::shared_ptr<GeomAPI_Shape> >& theInitialShapes)
{
  // collect indices of all edges to operate them quickly
  NCollection_DataMap<Handle(Geom_Curve), int> aCurveToIndex; // curve -> index in initial shapes
  std::list<std::shared_ptr<GeomAPI_Shape> >::const_iterator aFeatIt = theInitialShapes.begin();
  for (int anIndex = 0; aFeatIt != theInitialShapes.end(); aFeatIt++) {
    std::shared_ptr<GeomAPI_Shape> aShape(*aFeatIt);
    const TopoDS_Edge& anEdge = aShape->impl<TopoDS_Edge>();
    if (anEdge.ShapeType() != TopAbs_EDGE)
      continue;

    double aFirst, aLast;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(anEdge, aFirst, aLast);
    if (aCurve->DynamicType() == STANDARD_TYPE(Geom_TrimmedCurve))
      aCurve = Handle(Geom_TrimmedCurve)::DownCast(aCurve)->BasisCurve();
    if (!aCurveToIndex.IsBound(aCurve))
      aCurveToIndex.Bind(aCurve, anIndex++);
  }
  // map from area to the most first indices of curves (to compare) in it
  NCollection_DataMap<TopoDS_Shape, NCollection_Array1<int> > anAreaToIndex;
  // sort areas
  TopTools_ListOfShape::Iterator anArea1(theAreas);
  for(; anArea1.More(); anArea1.Next()) {
    TopTools_ListOfShape::Iterator anArea2 = anArea1;
    for(anArea2.Next(); anArea2.More(); anArea2.Next()) {
      if (!isFirst(anArea1.Value(), anArea2.Value(), anAreaToIndex, aCurveToIndex)) { // exchange
        TopoDS_Shape aTmp = anArea1.Value();
        anArea1.ChangeValue() = anArea2.Value();
        anArea2.ChangeValue() = aTmp;
      }
    }
  }
}

void GeomAlgoAPI_SketchBuilder::createFaces(
    const std::shared_ptr<GeomAPI_Pnt>& theOrigin,
    const std::shared_ptr<GeomAPI_Dir>& theDirX,
    const std::shared_ptr<GeomAPI_Dir>& theNorm,
    const std::list<std::shared_ptr<GeomAPI_Shape> >& theFeatures,
    std::list<std::shared_ptr<GeomAPI_Shape> >& theResultFaces)
{
  if (theFeatures.empty())
    return;

  BRep_Builder aBuilder;
  // Planar face, where the sketch was built
  Handle(Geom_Surface) aPlane(new Geom_Plane(theOrigin->impl<gp_Pnt>(), theNorm->impl<gp_Dir>()));
  TopoDS_Face aPlnFace;
  aBuilder.MakeFace(aPlnFace, aPlane, Precision::Confusion());

  // Use General Fuse algorithm to prepare all subfaces, bounded by given list of edges
  BOPAlgo_Builder aBB;
  aBB.AddArgument(aPlnFace);

  BOPCol_ListOfShape anEdges;
  BOPCol_ListIteratorOfListOfShape aShapeIt;
  std::list<std::shared_ptr<GeomAPI_Shape> >::const_iterator aFeatIt = theFeatures.begin();
  for (; aFeatIt != theFeatures.end(); aFeatIt++) {
    std::shared_ptr<GeomAPI_Shape> aShape(*aFeatIt);
    const TopoDS_Edge& anEdge = aShape->impl<TopoDS_Edge>();
    if (anEdge.ShapeType() == TopAbs_EDGE)
      aBB.AddArgument(anEdge);
  }
  aBB.Perform();
  if (aBB.ErrorStatus())
    return;

  // Collect faces
  TopTools_ListOfShape anAreas = aBB.Modified(aPlnFace);
  sortFaces(anAreas, theFeatures); // sort faces by the edges in them
  TopTools_ListIteratorOfListOfShape anIt(anAreas);
  for (; anIt.More(); anIt.Next()) {
    TopoDS_Face aFace = TopoDS::Face(anIt.Value());
    // avoid infinite faces
    BRepTopAdaptor_FClass2d aFClass(aFace, Precision::Confusion());
    if (aFClass.PerformInfinitePoint() == TopAbs_IN)
      continue;

    // rebuild face
    TopoDS_Face aNewFace;
    aBuilder.MakeFace(aNewFace, aPlane, Precision::Confusion());

    // iterate on wires
    TopExp_Explorer aWireExp(aFace, TopAbs_WIRE);
    for (; aWireExp.More(); aWireExp.Next()) {
      TopoDS_Wire aWire = TopoDS::Wire(aWireExp.Current());

      // to make faces equal on different platforms, we will find
      // a vertex lying on an edge with the lowest index in the list of initial edges
      TopoDS_Vertex aStartVertex = findStartVertex(aWire, theFeatures);

      TopoDS_Wire aNewWire;
      aBuilder.MakeWire(aNewWire);
      std::list<TopoDS_Edge> aSkippedEdges;
      bool aStartFound = false;

      // remove internal edges from faces and make wire start from found vertex
      TopExp_Explorer anExp(aWire, TopAbs_EDGE);
      for (; anExp.More(); anExp.Next()) {
        if (anExp.Current().Orientation() == TopAbs_INTERNAL)
          continue;
        if (!aStartFound) {
          const TopoDS_Edge& anEdge = TopoDS::Edge(anExp.Current());
          TopoDS_Vertex aV1, aV2;
          TopExp::Vertices(anEdge, aV1, aV2, Standard_True);
          if (aV1.IsSame(aStartVertex) == Standard_True)
            aStartFound = true;
          else
            aSkippedEdges.push_back(anEdge);
        }
        if (aStartFound)
          aBuilder.Add(aNewWire, anExp.Current());
      }
      // add skipped edges to the end of wire
      std::list<TopoDS_Edge>::const_iterator aSkIt = aSkippedEdges.begin();
      for (; aSkIt != aSkippedEdges.end(); ++aSkIt)
        aBuilder.Add(aNewWire, *aSkIt);

      // check the wire is empty
      anExp.Init(aNewWire, TopAbs_EDGE);
      if (anExp.More())
        aBuilder.Add(aNewFace, aNewWire);
    }

    // store face
    aFace = aNewFace;
    std::shared_ptr<GeomAPI_Shape> aResFace(new GeomAPI_Shape);
    aResFace->setImpl(new TopoDS_Face(aFace));
    theResultFaces.push_back(aResFace);
  }
}

void GeomAlgoAPI_SketchBuilder::createFaces(const std::shared_ptr<GeomAPI_Pnt>& theOrigin,
                                            const std::shared_ptr<GeomAPI_Dir>& theDirX,
                                            const std::shared_ptr<GeomAPI_Dir>& theNorm,
                                            const std::shared_ptr<GeomAPI_Shape>& theWire,
                                std::list<std::shared_ptr<GeomAPI_Shape> >& theResultFaces)
{
  std::shared_ptr<GeomAPI_PlanarEdges> aWire =
    std::dynamic_pointer_cast<GeomAPI_PlanarEdges>(theWire);
  if(aWire) {
    // Filter wires, return only faces.
    createFaces(theOrigin, theDirX, theNorm, aWire->getEdges(), theResultFaces);
  } else { // it may be only one circle
    std::shared_ptr<GeomAPI_Edge> anEdge = std::dynamic_pointer_cast<GeomAPI_Edge>(theWire);
    if (anEdge) {
      std::list<std::shared_ptr<GeomAPI_Shape> > aList;
      aList.push_back(anEdge);
      createFaces(theOrigin, theDirX, theNorm, aList, theResultFaces);
    }
  }
}
