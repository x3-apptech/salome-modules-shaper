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

#include<GeomAPI_Edge.h>
#include<GeomAPI_Pln.h>
#include<GeomAPI_Pnt.h>
#include<GeomAPI_Circ.h>
#include<GeomAPI_Dir.h>
#include<GeomAPI_Lin.h>

#include <BRepAdaptor_Curve.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pln.hxx>

#include <GCPnts_AbscissaPoint.hxx>

GeomAPI_Edge::GeomAPI_Edge()
{
  TopoDS_Edge* anEdge = new TopoDS_Edge;

  BRep_Builder aBuilder;
  aBuilder.MakeEdge(*anEdge);

  setImpl(anEdge);
}

GeomAPI_Edge::GeomAPI_Edge(const std::shared_ptr<GeomAPI_Shape>& theShape)
{
  if (!theShape->isNull() && theShape->isEdge()) {
    setImpl(new TopoDS_Shape(theShape->impl<TopoDS_Shape>()));
  }
}

bool GeomAPI_Edge::isLine() const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve->IsKind(STANDARD_TYPE(Geom_Line)))
    return true;
  return false;
}

bool GeomAPI_Edge::isCircle() const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve->IsKind(STANDARD_TYPE(Geom_Circle)))
  {
    // Check the difference of first and last parameters to be equal to the curve period
    if (Abs(aLast - aFirst - aCurve->Period()) < Precision::PConfusion())
      return true;
  }
  return false;
}

bool GeomAPI_Edge::isArc() const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve->IsKind(STANDARD_TYPE(Geom_Circle)))
  {
    // Check the difference of first and last parameters is not equal the curve period
    if (Abs(aLast - aFirst - aCurve->Period()) >= Precision::PConfusion())
      return true;
  }
  return false;
}

std::shared_ptr<GeomAPI_Pnt> GeomAPI_Edge::firstPoint()
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  gp_Pnt aPoint;
  aCurve->D0(aFirst, aPoint);
  return std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(aPoint.X(), aPoint.Y(), aPoint.Z()));
}

std::shared_ptr<GeomAPI_Pnt> GeomAPI_Edge::lastPoint()
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  gp_Pnt aPoint;
  aCurve->D0(aLast, aPoint);
  return std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(aPoint.X(), aPoint.Y(), aPoint.Z()));
}

std::shared_ptr<GeomAPI_Circ> GeomAPI_Edge::circle()
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve) {
    Handle(Geom_Circle) aCirc = Handle(Geom_Circle)::DownCast(aCurve);
    if (aCirc) {
      gp_Pnt aLoc = aCirc->Location();
      std::shared_ptr<GeomAPI_Pnt> aCenter(new GeomAPI_Pnt(aLoc.X(), aLoc.Y(), aLoc.Z()));
      gp_Dir anAxis = aCirc->Axis().Direction();
      std::shared_ptr<GeomAPI_Dir> aDir(new GeomAPI_Dir(anAxis.X(), anAxis.Y(), anAxis.Z()));
      return std::shared_ptr<GeomAPI_Circ>(new GeomAPI_Circ(aCenter, aDir, aCirc->Radius()));
    }
  }
  return std::shared_ptr<GeomAPI_Circ>(); // not circle
}

std::shared_ptr<GeomAPI_Lin> GeomAPI_Edge::line()
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve) {
    Handle(Geom_Line) aLine = Handle(Geom_Line)::DownCast(aCurve);
    if (aLine) {
      gp_Pnt aStartPnt = aLine->Value(aFirst);
      std::shared_ptr<GeomAPI_Pnt> aStart(
          new GeomAPI_Pnt(aStartPnt.X(), aStartPnt.Y(), aStartPnt.Z()));
      gp_Pnt aEndPnt = aLine->Value(aLast);
      std::shared_ptr<GeomAPI_Pnt> aEnd(
          new GeomAPI_Pnt(aEndPnt.X(), aEndPnt.Y(), aEndPnt.Z()));
      return std::shared_ptr<GeomAPI_Lin>(new GeomAPI_Lin(aStart, aEnd));
    }
  }
  return std::shared_ptr<GeomAPI_Lin>(); // not circle
}


bool GeomAPI_Edge::isEqual(const std::shared_ptr<GeomAPI_Shape> theEdge) const
{
  if (!theEdge.get() || ! theEdge->isEdge())
    return false;
  const TopoDS_Shape& aMyShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  const TopoDS_Shape& aInShape = theEdge->impl<TopoDS_Shape>();

  if (aMyShape.IsNull() || aInShape.IsNull())
    return false;

  if (aMyShape.ShapeType() != aInShape.ShapeType())
    return false;

  double aMyStart, aMyEnd;
  Handle(Geom_Curve) aMyCurve = BRep_Tool::Curve(TopoDS::Edge(aMyShape), aMyStart, aMyEnd);
  double aInStart, aInEnd;
  Handle(Geom_Curve) aInCurve = BRep_Tool::Curve(TopoDS::Edge(aInShape), aInStart, aInEnd);

  // Check that curves a the same type
  GeomAdaptor_Curve aMyAdaptor(aMyCurve);
  GeomAdaptor_Curve aInAdaptor(aInCurve);
  if (aMyAdaptor.GetType() != aInAdaptor.GetType())
    return false;

  // Check that end point parameters are the same
  if ((aMyStart != aInStart) || (aMyEnd != aInEnd))
    return false;

  // Check that end points are equal
  gp_Pnt aMyPnt1 = aMyAdaptor.Value(aMyStart);
  gp_Pnt aMyPnt2 = aMyAdaptor.Value(aMyEnd);
  gp_Pnt aInPnt1 = aInAdaptor.Value(aInStart);
  gp_Pnt aInPnt2 = aInAdaptor.Value(aInEnd);

  if ((!aMyPnt1.IsEqual(aInPnt1, Precision::Confusion())) ||
    (!aMyPnt2.IsEqual(aInPnt2, Precision::Confusion())))
    return false;

  return true;
}

void GeomAPI_Edge::getRange(double& theFirst, double& theLast) const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, theFirst, theLast);
}

bool GeomAPI_Edge::isInPlane(std::shared_ptr<GeomAPI_Pln> thePlane) const
{
  double aFirst, aLast;
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve.IsNull())
    return false;

  double A, B, C, D;
  thePlane->coefficients(A, B, C, D);
  gp_Pln aPlane(A, B, C, D);

  bool inPlane = false;
  if (aCurve->IsKind(STANDARD_TYPE(Geom_Line))) {
    // check start and end points on the plane
    gp_Pnt aFirstPnt = aCurve->Value(aFirst);
    gp_Pnt aLastPnt = aCurve->Value(aLast);
    inPlane = aPlane.SquareDistance(aFirstPnt) < Precision::SquareConfusion() &&
              aPlane.SquareDistance(aLastPnt) < Precision::SquareConfusion();
  } else if (aCurve->IsKind(STANDARD_TYPE(Geom_Circle))) {
    // check the center on the plane and normals are collinear
    Handle(Geom_Circle) aCirc = Handle(Geom_Circle)::DownCast(aCurve);
    gp_Pnt aCenter = aCirc->Location();
    Standard_Real aDot = aPlane.Axis().Direction().Dot(aCirc->Axis().Direction());
    inPlane = aPlane.SquareDistance(aCenter) < Precision::SquareConfusion() &&
              Abs(Abs(aDot) - 1.0) < Precision::Confusion();
  } else {
    // three points checking
    gp_Pnt aFirstPnt = aCurve->Value(aFirst);
    gp_Pnt aMidPnt = aCurve->Value((aFirst + aLast) / 2.);
    gp_Pnt aLastPnt = aCurve->Value(aLast);
    inPlane = aPlane.SquareDistance(aFirstPnt) < Precision::SquareConfusion() &&
              aPlane.SquareDistance(aMidPnt) < Precision::SquareConfusion() &&
              aPlane.SquareDistance(aLastPnt) < Precision::SquareConfusion();
  }
  return inPlane;
}

double GeomAPI_Edge::length() const
{
  const TopoDS_Edge& anEdge = TopoDS::Edge(impl<TopoDS_Shape>());
  BRepAdaptor_Curve aBRepAdaptor = BRepAdaptor_Curve(anEdge);
  Adaptor3d_Curve* anAdaptor3d = &aBRepAdaptor;
  return GCPnts_AbscissaPoint::Length(*anAdaptor3d);
}

bool GeomAPI_Edge::isClosed() const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  if (aShape.IsNull())
    return false;
  double aFirst, aLast;
  Handle(Geom_Curve) aCurve = BRep_Tool::Curve((const TopoDS_Edge&)aShape, aFirst, aLast);
  if (aCurve.IsNull() || !aCurve->IsPeriodic())
    return false;
  aLast += aLast > aFirst ? -aCurve->Period() : aCurve->Period();;

  return fabs(aFirst - aLast) < 1.e-9;
}

bool GeomAPI_Edge::isDegenerated() const
{
  const TopoDS_Shape& aShape = const_cast<GeomAPI_Edge*>(this)->impl<TopoDS_Shape>();
  if (aShape.IsNull() || aShape.ShapeType() != TopAbs_EDGE)
    return false;
  return BRep_Tool::Degenerated(TopoDS::Edge(aShape));
}
