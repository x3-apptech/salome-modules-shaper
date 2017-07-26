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

#include "SketcherPrs_PositionMgr.h"
#include "SketcherPrs_Tools.h"

#include <GeomAPI_Edge.h>
#include <GeomAPI_Curve.h>
#include <GeomAPI_Vertex.h>
#include <GeomAPI_Dir.h>
#include <GeomAPI_Ax3.h>
#include <GeomAPI_Circ.h>

#include <GeomDataAPI_Point2D.h>

#include <SketchPlugin_Line.h>
#include <SketchPlugin_Circle.h>
#include <SketchPlugin_Arc.h>
#include <SketchPlugin_ConstraintTangent.h>
#include <SketchPlugin_ConstraintPerpendicular.h>

#include <BRepExtrema_ExtPC.hxx>
#include <TopoDS_Vertex.hxx>
#include <Geom_Curve.hxx>
#include <TColGeom_SequenceOfCurve.hxx>
#include <gp_Dir.hxx>

#include <array>

static SketcherPrs_PositionMgr* MyPosMgr = NULL;

#define PI 3.1415926535897932

// The class is implemented as a singlton
SketcherPrs_PositionMgr* SketcherPrs_PositionMgr::get()
{
  if (MyPosMgr == NULL)
    MyPosMgr = new SketcherPrs_PositionMgr();
  return MyPosMgr;
}

SketcherPrs_PositionMgr::SketcherPrs_PositionMgr()
{
}


int SketcherPrs_PositionMgr::getPositionIndex(ObjectPtr theLine,
                                              const SketcherPrs_SymbolPrs* thePrs)
{
  if (myShapes.count(theLine) == 1) {
    // Find the map and add new [Presentation - Index] pair
    PositionsMap& aPosMap = myShapes[theLine];
    if (aPosMap.count(thePrs) == 1) {
      // return existing index
      return aPosMap[thePrs];
    } else {
      // Add a new [Presentation - Index] pair
      int aInd = int(aPosMap.size());
      aPosMap[thePrs] = aInd;
      return aInd;
    }
  } else {
    // Create a new map with initial index
    PositionsMap aPosMap;
    aPosMap[thePrs] = 0;
    myShapes[theLine] = aPosMap;
    return 0;
  }
}


bool SketcherPrs_PositionMgr::isPntConstraint(const std::string& theName)
{
  static std::list<std::string> aConstraints;
  if (aConstraints.size() == 0) {
    aConstraints.push_back(SketchPlugin_ConstraintTangent::ID());
    aConstraints.push_back(SketchPlugin_ConstraintPerpendicular::ID());
  }
  std::list<std::string>::const_iterator aIt;
  for (aIt = aConstraints.cbegin(); aIt != aConstraints.cend(); ++aIt) {
    if ((*aIt) == theName)
      return true;
  }
  return false;
}

bool containsPoint(const FeaturePtr& theFeature, GeomPnt2dPtr thePnt2d, GeomPointPtr thePos)
{
  if (theFeature->getKind() == SketchPlugin_Line::ID()) {
    AttributePoint2DPtr aSPnt1 = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
      theFeature->data()->attribute(SketchPlugin_Line::START_ID()));
    AttributePoint2DPtr aSPnt2 = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
      theFeature->data()->attribute(SketchPlugin_Line::END_ID()));

    GeomPnt2dPtr aPnt1 = aSPnt1->pnt();
    GeomPnt2dPtr aPnt2 = aSPnt2->pnt();

    if (aPnt1->isEqual(thePnt2d) || aPnt2->isEqual(thePnt2d))
      return true;
  } else if ((theFeature->getKind() == SketchPlugin_Circle::ID()) ||
             (theFeature->getKind() == SketchPlugin_Arc::ID())) {
    GeomCurvePtr aCurve;
    ObjectPtr aResObj;
    std::list<ResultPtr> aResults = theFeature->results();
    std::list<ResultPtr>::const_iterator aIt;
    for (aIt = aResults.cbegin(); aIt != aResults.cend(); aIt++) {
      GeomShapePtr aShp = SketcherPrs_Tools::getShape((*aIt));
      if (aShp->isEdge()) {
        aResObj = (*aIt);
        aCurve = std::shared_ptr<GeomAPI_Curve>(new GeomAPI_Curve(aShp));
        break;
      }
    }
    if (aCurve.get()) {
      double aStart = aCurve->startParam();
      double aEnd = aCurve->endParam();
      GeomCirclePtr  aCircle = GeomCirclePtr(new GeomAPI_Circ(aCurve));
      double aParam;
      if (aCircle->parameter(thePos, 1.e-4, aParam) && (aParam >= aStart) && (aParam <= aEnd))
        return true;
    }
  }
  return false;
}

int SketcherPrs_PositionMgr::getPositionIndex(GeomPointPtr thePos,
                                              const SketcherPrs_SymbolPrs* thePrs)
{
  if (myPntShapes.count(thePrs->feature()) == 0) {
    // Renumerate positions around the specified constraint point for all constraints
    GeomAx3Ptr aAx3 = thePrs->plane();
    ModelAPI_CompositeFeature* aOwner = thePrs->sketcher();
    GeomPnt2dPtr aPnt2d = thePos->to2D(aAx3->origin(), aAx3->dirX(), aAx3->dirY());

    int aNbSubs = aOwner->numberOfSubs();
    int aId = 0;
    for (int i = 0; i < aNbSubs; i++) {
      FeaturePtr aFeature = aOwner->subFeature(i);

      if (myPntShapes.count(aFeature.get()) == 1) {
        myPntShapes[aFeature.get()] = aId;
        aId++;
      } else {
        if (isPntConstraint(aFeature->getKind())) {
          DataPtr aData = aFeature->data();
          AttributeRefAttrPtr aObjRef = aData->refattr(SketchPlugin_Constraint::ENTITY_A());
          FeaturePtr aObj = ModelAPI_Feature::feature(aObjRef->object());
          bool aContains = false;
          if (containsPoint(aObj, aPnt2d, thePos)) {
            aContains = true;
          } else {
            aObjRef = aData->refattr(SketchPlugin_Constraint::ENTITY_B());
            aObj = ModelAPI_Feature::feature(aObjRef->object());
            if (containsPoint(aObj, aPnt2d, thePos)) {
              aContains = true;
            }
          }
          if (aContains) {
            myPntShapes[aFeature.get()] = aId;
            aId++;
          }
        }
      }
    }
  }
  return myPntShapes[thePrs->feature()];
}


gp_Vec getVector(ObjectPtr theShape, GeomDirPtr theDir, gp_Pnt theP)
{
  gp_Vec aVec;
  std::shared_ptr<GeomAPI_Shape> aShape = SketcherPrs_Tools::getShape(theShape);
  if (aShape->isEdge()) {
    std::shared_ptr<GeomAPI_Curve> aCurve =
      std::shared_ptr<GeomAPI_Curve>(new GeomAPI_Curve(aShape));

    if (aCurve->isCircle()) {
      GeomEdgePtr aEdgePtr(new GeomAPI_Edge(aShape));
      GeomVertexPtr aVertexPtr(new GeomAPI_Vertex(theP.X(), theP.Y(), theP.Z()));
      BRepExtrema_ExtPC aExtrema(aVertexPtr->impl<TopoDS_Vertex>(),
                                 aEdgePtr->impl<TopoDS_Edge>());
      int aNb = aExtrema.NbExt();
      if (aNb > 0) {
        for (int i = 1; i <= aNb; i++) {
          if (aExtrema.IsMin(i)) {
            double aParam = aExtrema.Parameter(i);
            Handle(Geom_Curve) aCurv = aCurve->impl<Handle_Geom_Curve>();
            gp_Pnt aP;
            aCurv->D1(aParam, aP, aVec);
            break;
          }
        }
      }
    } else {
      GeomPointPtr aPnt1 = aCurve->getPoint(aCurve->endParam());
      GeomPointPtr aPnt2 = aCurve->getPoint(aCurve->startParam());

      gp_Pnt aPn2 = aPnt2->impl<gp_Pnt>();
      if (aPn2.IsEqual(theP, Precision::Confusion()))
        aVec = gp_Vec(aPn2, aPnt1->impl<gp_Pnt>());
      else
        aVec = gp_Vec(aPnt1->impl<gp_Pnt>(), aPn2);
    }
  } else {
    aVec = gp_Vec(theDir->impl<gp_Dir>());
  }
  return aVec;
}

gp_Pnt SketcherPrs_PositionMgr::getPosition(ObjectPtr theShape,
                                            const SketcherPrs_SymbolPrs* thePrs,
                                            double theStep, GeomPointPtr thePnt)
{
  std::shared_ptr<GeomAPI_Shape> aShape = SketcherPrs_Tools::getShape(theShape);
  gp_Pnt aP; // Central point

  if (thePnt.get()) {
    return getPointPosition(theShape, thePrs, theStep, thePnt);
  } else {
    if (aShape->isEdge()) {
      std::shared_ptr<GeomAPI_Curve> aCurve =
        std::shared_ptr<GeomAPI_Curve>(new GeomAPI_Curve(aShape));
      // this is a circle or arc
      double aMidParam = (aCurve->startParam() + aCurve->endParam()) / 2.;
      std::shared_ptr<GeomAPI_Pnt> aPnt = aCurve->getPoint(aMidParam);
      aP = aPnt->impl<gp_Pnt>();
    } else {
      // This is a point
      std::shared_ptr<GeomAPI_Vertex> aVertex =
        std::shared_ptr<GeomAPI_Vertex>(new GeomAPI_Vertex(aShape));
      std::shared_ptr<GeomAPI_Pnt> aPnt = aVertex->point();
      aP = aPnt->impl<gp_Pnt>();
    }
  }
  // main vector
  gp_Vec aVec1 = getVector(theShape, thePrs->plane()->dirX(), aP);

  // Compute shifting vector for a one symbol
  gp_Vec aShift = aVec1.Crossed(thePrs->plane()->normal()->impl<gp_Dir>());
  aShift.Normalize();
  aShift.Multiply(theStep * 0.8);

  // Shift the position coordinate according to position index
  int aPos = getPositionIndex(theShape, thePrs);
  int aM = 1;
  if ((aPos % 2) == 0) {
    // Even position
    aP.Translate(aShift);
    if (aPos > 0) {
      if (aPos % 4 == 0)
        aM = aPos / 4;
      else
        aM = -(aPos + 2) / 4;
    }
  } else {
    // Odd position
    aP.Translate(-aShift);
    if (aPos > 1) {
      if ((aPos - 1) % 4 == 0)
        aM = (aPos - 1) / 4;
      else
        aM = -(aPos + 1) / 4;
    }
  }
  if (aPos > 1) {
    // Normalize vector along the line
    aVec1.Normalize();
    aVec1.Multiply(theStep);
    aP.Translate(aVec1.Multiplied(aM));
  }
  return aP;
}


//*****************************************************************
//! Returns curves connected to the given point
std::list<ObjectPtr> getCurves(const GeomPointPtr& thePnt, const SketcherPrs_SymbolPrs* thePrs)
{
  std::list<ObjectPtr> aList;
  GeomAx3Ptr aAx3 = thePrs->plane();
  ModelAPI_CompositeFeature* aOwner = thePrs->sketcher();
  GeomPnt2dPtr aPnt2d = thePnt->to2D(aAx3->origin(), aAx3->dirX(), aAx3->dirY());

  int aNbSubs = aOwner->numberOfSubs();
  for (int i = 0; i < aNbSubs; i++) {
    FeaturePtr aFeature = aOwner->subFeature(i);
    if (!aFeature->firstResult().get()) // There is no result
      continue;

    if (aFeature->getKind() == SketchPlugin_Line::ID()) {
      AttributePoint2DPtr aSPnt1 = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
        aFeature->data()->attribute(SketchPlugin_Line::START_ID()));
      AttributePoint2DPtr aSPnt2 = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
        aFeature->data()->attribute(SketchPlugin_Line::END_ID()));

      GeomPnt2dPtr aPnt1 = aSPnt1->pnt();
      GeomPnt2dPtr aPnt2 = aSPnt2->pnt();

      if (aPnt1->isEqual(aPnt2d) || aPnt2->isEqual(aPnt2d)) {
        GeomShapePtr aShp = SketcherPrs_Tools::getShape(aFeature->firstResult());
        GeomCurvePtr aCurv = std::shared_ptr<GeomAPI_Curve>(new GeomAPI_Curve(aShp));
        aList.push_back(aFeature->firstResult());
      }
    } else if ((aFeature->getKind() == SketchPlugin_Circle::ID()) ||
              (aFeature->getKind() == SketchPlugin_Arc::ID())) {
      GeomCurvePtr aCurve;
      ObjectPtr aResObj;
      std::list<ResultPtr> aResults = aFeature->results();
      std::list<ResultPtr>::const_iterator aIt;
      for (aIt = aResults.cbegin(); aIt != aResults.cend(); aIt++) {
        GeomShapePtr aShp = SketcherPrs_Tools::getShape((*aIt));
        if (aShp->isEdge()) {
          aResObj = (*aIt);
          aCurve = std::shared_ptr<GeomAPI_Curve>(new GeomAPI_Curve(aShp));
          break;
        }
      }
      if (aCurve.get()) {
        double aStart = aCurve->startParam();
        double aEnd = aCurve->endParam();
        GeomCirclePtr  aCircle = GeomCirclePtr(new GeomAPI_Circ(aCurve));
        double aParam;
        if (aCircle->parameter(thePnt, 1.e-4, aParam) && (aParam >= aStart) && (aParam <= aEnd))
          aList.push_back(aResObj);
      }
    }
  }
  return aList;
}

//*****************************************************************
gp_Pnt SketcherPrs_PositionMgr::getPointPosition(
  ObjectPtr theLine, const SketcherPrs_SymbolPrs* thePrs,
  double theStep, GeomPointPtr thePnt)
{
  gp_Pnt aP = thePnt->impl<gp_Pnt>();
  GeomDirPtr aNormal = thePrs->plane()->normal();
  gp_Dir aNormDir = aNormal->impl<gp_Dir>();

  std::list<ObjectPtr> aCurves = getCurves(thePnt, thePrs);
  std::list<ObjectPtr>::const_iterator aItCurv;
  std::list<gp_Vec> aVectorsList;
  // Calculate all vectors
  for (aItCurv = aCurves.cbegin(); aItCurv != aCurves.cend(); aItCurv++) {
    aVectorsList.push_back(getVector((*aItCurv), thePrs->plane()->dirX(), aP));
  }

  std::list<gp_Vec>::const_iterator aItVec;
  std::map<double, gp_Vec> aAngVectors;
  // Select closest vectors and calculate angles between base vector and closest vector
  for (aItVec = aVectorsList.cbegin(); aItVec != aVectorsList.cend(); aItVec++) {
    std::list<gp_Vec>::const_iterator aIt;
    double aMinAng = 0;
    gp_Vec aVec = *aItVec;
    for (aIt = aVectorsList.cbegin(); aIt != aVectorsList.cend(); aIt++) {
      double aAng = aVec.AngleWithRef(*aIt, aNormDir);
      if (aAng != 0) {
        if (aAng < 0)
          aAng = 2 * PI + aAng;

        if (aMinAng == 0)
          aMinAng = aAng;
        else if (aAng < aMinAng) {
          aMinAng = aAng;
        }
      }
    }
    aAngVectors[aMinAng] = aVec;
  }

  // Angle size of a symbol for a first level
  static const double aAngleStep = PI * 50./180.;

  // Position of the symbol
  int aPos = getPositionIndex(thePnt, thePrs);

  //std::list<double>::const_iterator aItAng;
  gp_Ax1 aRotAx(aP, aNormDir);
  int aPosId = 0; // Last used position
  double aAng;
  gp_Vec aPrevVec;
  std::map<double, gp_Vec>::const_iterator aItAng;
  for (aItAng = aAngVectors.cbegin(); aItAng != aAngVectors.cend(); ++aItAng) {
    aAng = aItAng->first;
    aPrevVec = aItAng->second;
    if (aAng >= aAngleStep) {
      gp_Vec aShift;
      int Nb = int(aAng / aAngleStep);
      if ((aPos >= aPosId) && (aPos < (aPosId + Nb))) {
        // rotate base vector on a necessary angle
        aShift = aPrevVec.Rotated(aRotAx, aAngleStep + aAngleStep * (aPos - aPosId));
        aShift.Normalize();
        aShift.Multiply(theStep * 1.5);
        return aP.Translated(aShift);
      }
      aPosId += Nb;
    }
  }
  gp_Vec aShift = aPrevVec.Rotated(aRotAx, aAngleStep);
  aShift.Normalize();
  aShift.Multiply(theStep * 1.5);
  return aP.Translated(aShift);
}

//*****************************************************************
void SketcherPrs_PositionMgr::deleteConstraint(const SketcherPrs_SymbolPrs* thePrs)
{
  std::map<ObjectPtr, PositionsMap>::iterator aIt;
  std::list<ObjectPtr> aToDel;
  // Clear map for deleted presentation
  for (aIt = myShapes.begin(); aIt != myShapes.end(); ++aIt) {
    PositionsMap& aPosMap = aIt->second;
    if (aPosMap.count(thePrs) > 0) {
      // Erase index
      aPosMap.erase(aPosMap.find(thePrs));
      if (aPosMap.size() == 0)
        // Delete the map
        aToDel.push_back(aIt->first);
      else {
        // Reindex objects positions in order to avoid spaces
        PositionsMap::iterator aIt;
        int i = 0;
        for (aIt = aPosMap.begin(); aIt != aPosMap.end(); aIt++, i++)
          aIt->second = i;
      }
    }
  }
  std::list<ObjectPtr>::const_iterator aListIt;
  for (aListIt = aToDel.cbegin(); aListIt != aToDel.cend(); ++aListIt) {
    myShapes.erase(*aListIt);
  }
}
