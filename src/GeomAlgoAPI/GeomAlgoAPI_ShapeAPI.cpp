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

#include "GeomAlgoAPI_ShapeAPI.h"

#include <GeomAlgoAPI_Box.h>
#include <GeomAlgoAPI_CompoundBuilder.h>
#include <GeomAlgoAPI_Cone.h>
#include <GeomAlgoAPI_ConeSegment.h>
#include <GeomAlgoAPI_Cylinder.h>
#include <GeomAlgoAPI_EdgeBuilder.h>
#include <GeomAlgoAPI_Rotation.h>
#include <GeomAlgoAPI_Scale.h>
#include <GeomAlgoAPI_Sphere.h>
#include <GeomAlgoAPI_Symmetry.h>
#include <GeomAlgoAPI_Torus.h>
#include <GeomAlgoAPI_Translation.h>

#include <GeomAPI_Lin.h>

#include <math.h>

namespace GeomAlgoAPI_ShapeAPI
{
  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeBox(
    const double theDx, const double theDy,
    const double theDz) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Box aBoxAlgo(theDx,theDy,theDz);

    if (!aBoxAlgo.check()) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }

    aBoxAlgo.build();

    if(!aBoxAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }
    if (!aBoxAlgo.checkValid("Box builder with dimensions")) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }
    return aBoxAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeBox(
    std::shared_ptr<GeomAPI_Pnt> theFirstPoint,
    std::shared_ptr<GeomAPI_Pnt> theSecondPoint) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Box aBoxAlgo(theFirstPoint, theSecondPoint);

    if (!aBoxAlgo.check()) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }

    aBoxAlgo.build();

    if(!aBoxAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }
    if (!aBoxAlgo.checkValid("Box builder with two points")) {
      throw GeomAlgoAPI_Exception(aBoxAlgo.getError());
    }
    return aBoxAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCylinder(
    std::shared_ptr<GeomAPI_Pnt> theBasePoint, std::shared_ptr<GeomAPI_Edge> theEdge,
    double theRadius, double theHeight) throw (GeomAlgoAPI_Exception)
  {
    // Check if the base point is OK
    if (!theBasePoint) {
      throw GeomAlgoAPI_Exception("Cylinder builder :: the base point is not valid.");
    }
    // Check if the edge is OK
    if (!theEdge) {
      throw GeomAlgoAPI_Exception("Cylinder builder :: the axis is not valid.");
    }

    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(theBasePoint,
                                                          theEdge->line()->direction()));

    GeomAlgoAPI_Cylinder aCylinderAlgo(anAxis, theRadius, theHeight);

    if (!aCylinderAlgo.check()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }

    aCylinderAlgo.build();

    if(!aCylinderAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    if (!aCylinderAlgo.checkValid("Cylinder builder")) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    return aCylinderAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCylinder(
    std::shared_ptr<GeomAPI_Pnt> theBasePoint, std::shared_ptr<GeomAPI_Edge> theEdge,
    double theRadius, double theHeight, double theAngle) throw (GeomAlgoAPI_Exception)
  {
    // Check if the base point is OK
    if (!theBasePoint) {
      throw GeomAlgoAPI_Exception("Cylinder builder :: the base point is not valid.");
    }
    // Check if the edge is OK
    if (!theEdge) {
      throw GeomAlgoAPI_Exception("Cylinder builder :: the axis is not valid.");
    }

    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(theBasePoint,
                                                          theEdge->line()->direction()));

    GeomAlgoAPI_Cylinder aCylinderAlgo(anAxis, theRadius, theHeight, theAngle);

    if (!aCylinderAlgo.check()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }

    aCylinderAlgo.build();

    if(!aCylinderAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    if (!aCylinderAlgo.checkValid("Cylinder portion builder")) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    return aCylinderAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCylinder(
    double theRadius, double theHeight) throw (GeomAlgoAPI_Exception)
  {
    std::shared_ptr<GeomAPI_Pnt> aBasePoint =
      std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(0.,0.,0.));
    std::shared_ptr<GeomAPI_Edge> aEdge = GeomAlgoAPI_EdgeBuilder::line(0., 0., 1.);
    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(aBasePoint,
                                                          aEdge->line()->direction()));

    GeomAlgoAPI_Cylinder aCylinderAlgo(anAxis, theRadius, theHeight);

    if (!aCylinderAlgo.check()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }

    aCylinderAlgo.build();

    if(!aCylinderAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    if (!aCylinderAlgo.checkValid("Cylinder builder")) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    return aCylinderAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCylinder(
    double theRadius, double theHeight, double theAngle) throw (GeomAlgoAPI_Exception)
  {
    std::shared_ptr<GeomAPI_Pnt> aBasePoint =
      std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(0.,0.,0.));
    std::shared_ptr<GeomAPI_Edge> aEdge = GeomAlgoAPI_EdgeBuilder::line(0., 0., 1.);
    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(aBasePoint,
                                                          aEdge->line()->direction()));

    GeomAlgoAPI_Cylinder aCylinderAlgo(anAxis, theRadius, theHeight, theAngle);

    if (!aCylinderAlgo.check()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }

    aCylinderAlgo.build();

    if(!aCylinderAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    if (!aCylinderAlgo.checkValid("Cylinder portion builder")) {
      throw GeomAlgoAPI_Exception(aCylinderAlgo.getError());
    }
    return aCylinderAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeSphere(
      std::shared_ptr<GeomAPI_Pnt> theCenterPoint, double theRadius) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Sphere aSphereAlgo(theCenterPoint, theRadius);

    if (!aSphereAlgo.check()) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }

    aSphereAlgo.build();

    if(!aSphereAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }

    if (!aSphereAlgo.checkValid("Sphere builder")) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }
    return aSphereAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeSphere(double theRadius)
      throw (GeomAlgoAPI_Exception)
  {
    std::shared_ptr<GeomAPI_Pnt> aCenterPoint =
      std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(0.,0.,0.));

    GeomAlgoAPI_Sphere aSphereAlgo(aCenterPoint, theRadius);

    if (!aSphereAlgo.check()) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }

    aSphereAlgo.build();

    if(!aSphereAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }

    if (!aSphereAlgo.checkValid("Sphere builder")) {
      throw GeomAlgoAPI_Exception(aSphereAlgo.getError());
    }
    return aSphereAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeTorus(
      std::shared_ptr<GeomAPI_Pnt> theBasePoint,
      std::shared_ptr<GeomAPI_Edge> theEdge,double theRadius, double theRingRadius)
      throw (GeomAlgoAPI_Exception)
  {
    // Check if the base point is OK
    if (!theBasePoint) {
      throw GeomAlgoAPI_Exception("Torus builder :: the base point is not valid.");
    }
    // Check if the edge is OK
    if (!theEdge) {
      throw GeomAlgoAPI_Exception("Torus builder :: the axis is not valid.");
    }

    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(theBasePoint,
                                                          theEdge->line()->direction()));

    GeomAlgoAPI_Torus aTorusAlgo(anAxis, theRadius, theRingRadius);

    if (!aTorusAlgo.check()) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }

    aTorusAlgo.build();

    if(!aTorusAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }

    if (!aTorusAlgo.checkValid("Torus builder")) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }
    return aTorusAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeTorus(double theRadius,
      double theRingRadius) throw (GeomAlgoAPI_Exception)
  {
    std::shared_ptr<GeomAPI_Pnt> aBasePoint =
      std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(0.,0.,0.));
    std::shared_ptr<GeomAPI_Edge> aEdge = GeomAlgoAPI_EdgeBuilder::line(0., 0., 1.);
    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(aBasePoint,
                                                          aEdge->line()->direction()));

    GeomAlgoAPI_Torus aTorusAlgo(anAxis, theRadius, theRingRadius);

    if (!aTorusAlgo.check()) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }

    aTorusAlgo.build();

    if(!aTorusAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }

    if (!aTorusAlgo.checkValid("Torus builder")) {
      throw GeomAlgoAPI_Exception(aTorusAlgo.getError());
    }
    return aTorusAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCone(
      std::shared_ptr<GeomAPI_Pnt> theBasePoint,
      std::shared_ptr<GeomAPI_Edge> theEdge,
      double theBaseRadius, double theTopRadius,
      double theHeight) throw (GeomAlgoAPI_Exception)
  {
    // Check if the base point is OK
    if (!theBasePoint) {
      throw GeomAlgoAPI_Exception("Cone builder :: the base point is not valid.");
    }
    // Check if the edge is OK
    if (!theEdge) {
      throw GeomAlgoAPI_Exception("Cone builder :: the axis is not valid.");
    }

    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(theBasePoint,
                                                          theEdge->line()->direction()));

    GeomAlgoAPI_Cone aConeAlgo(anAxis, theBaseRadius, theTopRadius, theHeight);

    if (!aConeAlgo.check()) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }

    aConeAlgo.build();

    if(!aConeAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }

    if (!aConeAlgo.checkValid("Cone builder")) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }
    return aConeAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeCone(
      double theBaseRadius, double theTopRadius,
      double theHeight) throw (GeomAlgoAPI_Exception)
  {
    std::shared_ptr<GeomAPI_Pnt> aBasePoint =
      std::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(0.,0.,0.));
    std::shared_ptr<GeomAPI_Edge> aEdge = GeomAlgoAPI_EdgeBuilder::line(0., 0., 1.);
    std::shared_ptr<GeomAPI_Ax2> anAxis;
    anAxis = std::shared_ptr<GeomAPI_Ax2>(new GeomAPI_Ax2(aBasePoint,
                                                          aEdge->line()->direction()));

    GeomAlgoAPI_Cone aConeAlgo(anAxis, theBaseRadius, theTopRadius, theHeight);

    if (!aConeAlgo.check()) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }

    aConeAlgo.build();

    if(!aConeAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }

    if (!aConeAlgo.checkValid("Cone builder")) {
      throw GeomAlgoAPI_Exception(aConeAlgo.getError());
    }
    return aConeAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeTranslation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1>   theAxis,
    const double theDistance) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Translation aTranslationAlgo(theSourceShape, theAxis, theDistance);

    if (!aTranslationAlgo.check()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    aTranslationAlgo.build();

    if(!aTranslationAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    return aTranslationAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeTranslation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    const double theDx,
    const double theDy,
    const double theDz) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Translation aTranslationAlgo(theSourceShape, theDx, theDy, theDz);

    if (!aTranslationAlgo.check()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    aTranslationAlgo.build();

    if(!aTranslationAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    return aTranslationAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeTranslation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Pnt>   theStartPoint,
    std::shared_ptr<GeomAPI_Pnt>   theEndPoint) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Translation aTranslationAlgo(theSourceShape, theStartPoint, theEndPoint);

    if (!aTranslationAlgo.check()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    aTranslationAlgo.build();

    if(!aTranslationAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aTranslationAlgo.getError());
    }

    return aTranslationAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeRotation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1> theAxis,
    const double theAngle) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Rotation aRotationAlgo(theSourceShape, theAxis, theAngle);

    if (!aRotationAlgo.check()) {
      throw GeomAlgoAPI_Exception(aRotationAlgo.getError());
    }

    aRotationAlgo.build();

    if(!aRotationAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aRotationAlgo.getError());
    }

    return aRotationAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeRotation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Pnt> theCenterPoint,
    std::shared_ptr<GeomAPI_Pnt> theStartPoint,
    std::shared_ptr<GeomAPI_Pnt> theEndPoint) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Rotation aRotationAlgo(theSourceShape, theCenterPoint, theStartPoint, theEndPoint);

    if (!aRotationAlgo.check()) {
      throw GeomAlgoAPI_Exception(aRotationAlgo.getError());
    }

    aRotationAlgo.build();

    if(!aRotationAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aRotationAlgo.getError());
    }

    return aRotationAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeSymmetry(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Pnt>   thePoint) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Symmetry aSymmetryAlgo(theSourceShape, thePoint);

    if (!aSymmetryAlgo.check()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    aSymmetryAlgo.build();

    if(!aSymmetryAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    return aSymmetryAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeSymmetry(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1>   theAxis) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Symmetry aSymmetryAlgo(theSourceShape, theAxis);

    if (!aSymmetryAlgo.check()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    aSymmetryAlgo.build();

    if(!aSymmetryAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    return aSymmetryAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeSymmetry(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax2>   thePlane) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Symmetry aSymmetryAlgo(theSourceShape, thePlane);

    if (!aSymmetryAlgo.check()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    aSymmetryAlgo.build();

    if(!aSymmetryAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aSymmetryAlgo.getError());
    }

    return aSymmetryAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeScale(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Pnt>   theCenterPoint,
    const double                   theScaleFactor) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Scale aScaleAlgo(theSourceShape, theCenterPoint, theScaleFactor);

    if (!aScaleAlgo.check()) {
      throw GeomAlgoAPI_Exception(aScaleAlgo.getError());
    }

    aScaleAlgo.build();

    if(!aScaleAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aScaleAlgo.getError());
    }

    return aScaleAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeScale(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Pnt>   theCenterPoint,
    const double                   theScaleFactorX,
    const double                   theScaleFactorY,
    const double                   theScaleFactorZ) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_Scale aScaleAlgo(theSourceShape, theCenterPoint,
                                 theScaleFactorX, theScaleFactorY, theScaleFactorZ);

    if (!aScaleAlgo.check()) {
      throw GeomAlgoAPI_Exception(aScaleAlgo.getError());
    }

    aScaleAlgo.build();

    if(!aScaleAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aScaleAlgo.getError());
    }

    return aScaleAlgo.shape();
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeMultiTranslation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1> theAxis,
    const double theStep,
    const int theNumber) throw (GeomAlgoAPI_Exception)
  {
    if (!theAxis) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the first axis is not valid";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (theNumber <=0) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the number of copies for the first direction is null or negative.";
      throw GeomAlgoAPI_Exception(aError);
    }

    ListOfShape aListOfShape;
    for (int i=0; i<theNumber; i++) {
      aListOfShape.
        push_back(GeomAlgoAPI_ShapeAPI::makeTranslation(theSourceShape, theAxis, i*theStep));
    }
    return GeomAlgoAPI_CompoundBuilder::compound(aListOfShape);
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeMultiTranslation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1> theFirstAxis,
    const double theFirstStep,
    const int theFirstNumber,
    std::shared_ptr<GeomAPI_Ax1> theSecondAxis,
    const double theSecondStep,
    const int theSecondNumber) throw (GeomAlgoAPI_Exception)
  {
    if (!theFirstAxis) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the first axis is not valid";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (!theSecondAxis) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the second axis is not valid";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (theFirstNumber <=0) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the number of copies for the first direction is null or negative.";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (theSecondNumber <=0) {
      std::string aError = "Multitranslation builder ";
      aError+=":: the number of copies for the second direction is null or negative.";
      throw GeomAlgoAPI_Exception(aError);
    }

    // Coord theFirstAxis
    double x1 = theFirstAxis->dir()->x();
    double y1 = theFirstAxis->dir()->y();
    double z1 = theFirstAxis->dir()->z();
    double norm1 = sqrt(x1*x1 + y1*y1 + z1*z1);

    // Coord theSecondAxis
    double x2 = theSecondAxis->dir()->x();
    double y2 = theSecondAxis->dir()->y();
    double z2 = theSecondAxis->dir()->z();
    double norm2 = sqrt(x2*x2 + y2*y2 + z2*z2);

    ListOfShape aListOfShape;
    for (int j=0; j<theSecondStep; j++) {
      for (int i=0; i<theFirstNumber; i++) {
        double dx = i*theFirstStep*x1/norm1+j*theSecondStep*x2/norm2;
        double dy = i*theFirstStep*y1/norm1+j*theSecondStep*y2/norm2;
        double dz = i*theFirstStep*z1/norm1+j*theSecondStep*z2/norm2;
        aListOfShape.push_back(GeomAlgoAPI_ShapeAPI::makeTranslation(theSourceShape, dx, dy, dz));
      }
    }
    return GeomAlgoAPI_CompoundBuilder::compound(aListOfShape);
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeMultiRotation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1> theAxis,
    const int theNumber)
  {
    if (!theAxis) {
      std::string aError = "Multirotation builder ";
      aError+=":: the axis is not valid";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (theNumber <=0) {
      std::string aError = "Multirotation builder ";
      aError+=":: the number of copies is null or negative.";
      throw GeomAlgoAPI_Exception(aError);
    }

    double anAngle = 360./theNumber;

    ListOfShape aListOfShape;
    for (int i=0; i<theNumber; i++) {
      aListOfShape.
        push_back(GeomAlgoAPI_ShapeAPI::makeRotation(theSourceShape, theAxis, i*anAngle));
    }
    return GeomAlgoAPI_CompoundBuilder::compound(aListOfShape);
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeMultiRotation(
    std::shared_ptr<GeomAPI_Shape> theSourceShape,
    std::shared_ptr<GeomAPI_Ax1> theAxis,
    const double theStep,
    const int theNumber)
  {
    if (!theAxis) {
      std::string aError = "Multirotation builder ";
      aError+=":: the axis is not valid";
      throw GeomAlgoAPI_Exception(aError);
    }

    if (theNumber <=0) {
      std::string aError = "Multirotation builder ";
      aError+=":: the number of copies is null or negative.";
      throw GeomAlgoAPI_Exception(aError);
    }

    ListOfShape aListOfShape;
    for (int i=0; i<theNumber; i++) {
      aListOfShape.
        push_back(GeomAlgoAPI_ShapeAPI::makeRotation(theSourceShape, theAxis, i*theStep));
    }
    return GeomAlgoAPI_CompoundBuilder::compound(aListOfShape);
  }

  //===============================================================================================
  std::shared_ptr<GeomAPI_Shape> GeomAlgoAPI_ShapeAPI::makeConeSegment(
    const double theRMin1, const double theRMax1,
    const double theRMin2, const double theRMax2,
    const double theZ,
    const double theStartPhi, const double theDeltaPhi) throw (GeomAlgoAPI_Exception)
  {
    GeomAlgoAPI_ConeSegment aConeSegmentAlgo(theRMin1, theRMax1, theRMin2, theRMax2,
                                             theZ, theStartPhi, theDeltaPhi);

    if (!aConeSegmentAlgo.check()) {
      throw GeomAlgoAPI_Exception(aConeSegmentAlgo.getError());
    }

    aConeSegmentAlgo.build();

    if(!aConeSegmentAlgo.isDone()) {
      throw GeomAlgoAPI_Exception(aConeSegmentAlgo.getError());
    }
    if (!aConeSegmentAlgo.checkValid("Cone Segment builder")) {
      throw GeomAlgoAPI_Exception(aConeSegmentAlgo.getError());
    }
    return aConeSegmentAlgo.shape();
  }
}
