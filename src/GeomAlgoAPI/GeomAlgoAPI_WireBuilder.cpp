// Copyright (C) 2014-2020  CEA/DEN, EDF R&D
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "GeomAlgoAPI_WireBuilder.h"

#include <GeomAPI_Edge.h>
#include <GeomAPI_Pnt.h>
#include <GeomAPI_Vertex.h>
#include <GeomAPI_ShapeExplorer.h>

#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Geom_Curve.hxx>
#include <Precision.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>

#include <cmath>
#include <map>
#include <set>

class SetOfEdges
{
  class DoubleCompare {
  public:
    bool operator() (const double d1, const double d2) const {
      return d1 + Precision::Confusion() < d2;
    }
  };

  typedef std::map<double, std::set<double, DoubleCompare>, DoubleCompare> ParamMap;
  std::map<Handle(Geom_Curve), ParamMap> myShapes;

public:
  bool add(const TopoDS_Shape& theEdge)
  {
    const TopoDS_Edge& anEdge = TopoDS::Edge(theEdge);
    if (anEdge.IsNull())
      return true;

    double aFirst, aLast;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve(anEdge, aFirst, aLast);

    bool isAdded = true;
    std::map<Handle(Geom_Curve), ParamMap>::iterator
        aFound = myShapes.find(aCurve);
    if (aFound == myShapes.end())
      myShapes[aCurve][aFirst].insert(aLast);
    else {
      ParamMap::iterator aFoundPar = aFound->second.find(aFirst);
      if (aFoundPar == aFound->second.end())
        aFound->second[aFirst].insert(aLast);
      else if (aFoundPar->second.find(aLast) == aFoundPar->second.end())
        aFoundPar->second.insert(aLast);
      else
        isAdded = false;
    }
    return isAdded;
  }

  static bool isEqual(const TopoDS_Shape& theShape1, const TopoDS_Shape& theShape2)
  {
    const TopoDS_Edge& anEdge1 = TopoDS::Edge(theShape1);
    const TopoDS_Edge& anEdge2 = TopoDS::Edge(theShape2);
    if (anEdge1.IsNull() || anEdge2.IsNull())
      return false;

    double aFirst1, aLast1;
    Handle(Geom_Curve) aCurve1 = BRep_Tool::Curve(anEdge1, aFirst1, aLast1);
    double aFirst2, aLast2;
    Handle(Geom_Curve) aCurve2 = BRep_Tool::Curve(anEdge2, aFirst2, aLast2);
    return aCurve1 == aCurve2 && fabs(aFirst1 - aFirst2) < Precision::Confusion() &&
                                 fabs(aLast1 - aLast2) < Precision::Confusion();
  }
};

//=================================================================================================
GeomShapePtr GeomAlgoAPI_WireBuilder::wire(const ListOfShape& theShapes)
{
  TopTools_ListOfShape aListOfEdges;
  SetOfEdges aProcessedEdges;

  ListOfShape::const_iterator anIt = theShapes.cbegin();
  for(; anIt != theShapes.cend(); ++anIt) {
    TopoDS_Shape aShape = (*anIt)->impl<TopoDS_Shape>();
    switch(aShape.ShapeType()) {
      case TopAbs_EDGE: {
        aShape.Orientation(TopAbs_FORWARD);
        if (aProcessedEdges.add(aShape))
          aListOfEdges.Append(aShape);
        break;
      }
      case TopAbs_WIRE: {
        for(TopExp_Explorer anExp(aShape, TopAbs_EDGE); anExp.More(); anExp.Next()) {
          TopoDS_Shape anEdge = anExp.Current();
          anEdge.Orientation(TopAbs_FORWARD);
          // if the edge was already processed, remove it to keep original order of the current wire
          if (!aProcessedEdges.add(anEdge)) {
            for (TopTools_ListIteratorOfListOfShape anIt(aListOfEdges); anIt.More(); anIt.Next())
              if (SetOfEdges::isEqual(anEdge, anIt.Value())) {
                aListOfEdges.Remove(anIt);
                break;
              }
          }
          aListOfEdges.Append(anEdge);
        }
        break;
      }
      default: {
        return GeomShapePtr();
      }
    }
  }

  BRepBuilderAPI_MakeWire aWireBuilder;
  aWireBuilder.Add(aListOfEdges);
  if(aWireBuilder.Error() != BRepBuilderAPI_WireDone) {
    return GeomShapePtr();
  }

  GeomShapePtr aResultShape(new GeomAPI_Shape());
  aResultShape->setImpl(new TopoDS_Shape(aWireBuilder.Wire()));
  return aResultShape;
}

//=================================================================================================
bool GeomAlgoAPI_WireBuilder::isSelfIntersected(const GeomShapePtr& theWire)
{
  // Collect edges.
  ListOfShape anEdges;

  GeomAPI_ShapeExplorer anExp(theWire, GeomAPI_Shape::EDGE);
  for (; anExp.more(); anExp.next()) {
    GeomShapePtr anEdge = anExp.current();
    anEdges.push_back(anEdge);
  }

  // Check intersections between edges pair-wise
  int aNbEdges = (int)anEdges.size();
  std::list<GeomShapePtr>::const_iterator anEdgesIt = anEdges.begin();
  for (int i = 0; anEdgesIt != anEdges.end(); ++anEdgesIt, i++) {
    GeomEdgePtr anEdge1(new GeomAPI_Edge(*anEdgesIt));

    std::list<GeomShapePtr>::const_iterator anOtherEdgesIt = std::next(anEdgesIt);
    for (int j = i + 1; anOtherEdgesIt != anEdges.end(); ++anOtherEdgesIt, j++) {
      GeomEdgePtr anEdge2(new GeomAPI_Edge(*anOtherEdgesIt));
      GeomShapePtr anInter = anEdge1->intersect(anEdge2);
      if (!anInter.get()) {
        continue;
      }

      bool isOk = false;

      if (anInter->isVertex()) {
        GeomVertexPtr aVertex(new GeomAPI_Vertex(anInter));
        GeomPointPtr aPnt = aVertex->point();

        GeomPointPtr aFirstPnt1 = anEdge1->orientation() == GeomAPI_Shape::FORWARD ?
                                  anEdge1->firstPoint() : anEdge1->lastPoint();
        GeomPointPtr aLastPnt1 = anEdge1->orientation() == GeomAPI_Shape::FORWARD ?
                                 anEdge1->lastPoint() : anEdge1->firstPoint();
        GeomPointPtr aFirstPnt2 = anEdge2->orientation() == GeomAPI_Shape::FORWARD ?
                                  anEdge2->firstPoint() : anEdge2->lastPoint();
        GeomPointPtr aLastPnt2 = anEdge2->orientation() == GeomAPI_Shape::FORWARD ?
                                 anEdge2->lastPoint() : anEdge2->firstPoint();

        GeomPointPtr aCommonEndPnt;
        if (aFirstPnt1->isEqual(aLastPnt2)) {
          aCommonEndPnt = aFirstPnt1;
        } else if(aLastPnt1->isEqual(aFirstPnt2)) {
          aCommonEndPnt = aLastPnt1;
        }

        isOk = aCommonEndPnt && aPnt->isEqual(aCommonEndPnt);
      }

      if (!isOk) {
        return true;
      }
    }
  }

  return false;
}