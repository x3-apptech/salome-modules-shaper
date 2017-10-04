// Copyright (C) 2017  CEA/DEN, EDF R&D
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

// File:        GeomAPI_Ellipse2d.cpp
// Created:     26 April 2017
// Author:      Artem ZHIDKOV

#include <GeomAPI_Ellipse2d.h>
#include <GeomAPI_Dir2d.h>
#include <GeomAPI_Pnt2d.h>

#include <gp_Ax22d.hxx>
#include <gp_Elips2d.hxx>
#include <Precision.hxx>

#define MY_ELLIPSE implPtr<gp_Elips2d>()

static gp_Elips2d* newEllipse(const gp_Pnt2d& theCenter,
                              const gp_Dir2d& theXAxis,
                              const double theMajorRadius,
                              const double theMinorRadius)
{
  if (theMajorRadius < theMinorRadius - Precision::Confusion()) {
    return newEllipse(theCenter, gp_Dir2d(-theXAxis.Y(), theXAxis.X()),
                      theMinorRadius, theMajorRadius);
  }

  gp_Ax22d anAxis(theCenter, theXAxis);
  return new gp_Elips2d(anAxis, theMajorRadius, theMinorRadius);
}

static gp_Elips2d* newEllipse(const std::shared_ptr<GeomAPI_Pnt2d>& theCenter,
                              const std::shared_ptr<GeomAPI_Pnt2d>& theAxisPoint,
                              const std::shared_ptr<GeomAPI_Pnt2d>& thePassingPoint)
{
  const gp_Pnt2d& aCenter = theCenter->impl<gp_Pnt2d>();
  const gp_Pnt2d& anAxisPnt = theAxisPoint->impl<gp_Pnt2d>();
  const gp_Pnt2d& aPassedPnt = thePassingPoint->impl<gp_Pnt2d>();

  gp_Dir2d aXAxis(anAxisPnt.XY() - aCenter.XY());
  double aMajorRadius = anAxisPnt.Distance(aCenter);

  gp_XY aPassedDir = aPassedPnt.XY() - aCenter.XY();

  double X = aPassedDir.Dot(aXAxis.XY()) / aMajorRadius;
  if (Abs(X) > 1.0 - Precision::Confusion())
    return 0; // ellipse cannot be created for such parameters

  double Y = aPassedDir.CrossMagnitude(aXAxis.XY());
  double aMinorRadius = Y / Sqrt(1. - X * X);

  return newEllipse(aCenter, aXAxis, aMajorRadius, aMinorRadius);
}


GeomAPI_Ellipse2d::GeomAPI_Ellipse2d(const std::shared_ptr<GeomAPI_Pnt2d>& theCenter,
                                     const std::shared_ptr<GeomAPI_Dir2d>& theXAxis,
                                     const double theMajorRadius,
                                     const double theMinorRadius)
  : GeomAPI_Interface(newEllipse(theCenter->impl<gp_Pnt2d>(), theXAxis->impl<gp_Dir2d>(),
                                 theMajorRadius, theMinorRadius))
{
}

GeomAPI_Ellipse2d::GeomAPI_Ellipse2d(const std::shared_ptr<GeomAPI_Pnt2d>& theCenter,
                                     const std::shared_ptr<GeomAPI_Pnt2d>& theAxisPoint,
                                     const std::shared_ptr<GeomAPI_Pnt2d>& thePassingPoint)
  : GeomAPI_Interface(newEllipse(theCenter, theAxisPoint, thePassingPoint))
{
}

std::shared_ptr<GeomAPI_Pnt2d> GeomAPI_Ellipse2d::center() const
{
  const gp_Pnt2d& aCenter = MY_ELLIPSE->Location();
  return std::shared_ptr<GeomAPI_Pnt2d>(new GeomAPI_Pnt2d(aCenter.X(), aCenter.Y()));
}

std::shared_ptr<GeomAPI_Pnt2d> GeomAPI_Ellipse2d::firstFocus() const
{
  const gp_Pnt2d& aFirst = MY_ELLIPSE->Focus1();
  return std::shared_ptr<GeomAPI_Pnt2d>(new GeomAPI_Pnt2d(aFirst.X(), aFirst.Y()));
}

std::shared_ptr<GeomAPI_Pnt2d> GeomAPI_Ellipse2d::secondFocus() const
{
  const gp_Pnt2d& aSecond = MY_ELLIPSE->Focus2();
  return std::shared_ptr<GeomAPI_Pnt2d>(new GeomAPI_Pnt2d(aSecond.X(), aSecond.Y()));
}

double GeomAPI_Ellipse2d::minorRadius() const
{
  return MY_ELLIPSE->MinorRadius();
}

double GeomAPI_Ellipse2d::majorRadius() const
{
  return MY_ELLIPSE->MajorRadius();
}