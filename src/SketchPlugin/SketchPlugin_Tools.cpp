// Copyright (C) 2014-2019  CEA/DEN, EDF R&D
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

#include "SketchPlugin_Tools.h"

#include "SketchPlugin_ConstraintCoincidence.h"
#include "SketchPlugin_ConstraintLength.h"
#include "SketchPlugin_ConstraintTangent.h"
#include "SketchPlugin_Line.h"
#include "SketchPlugin_Point.h"
#include "SketchPlugin_SketchEntity.h"

#include <SketcherPrs_Tools.h>

#include <ModelAPI_AttributeDouble.h>

#include <GeomAPI_Dir2d.h>
#include <GeomAPI_Pnt2d.h>
#include <GeomAPI_XY.h>

#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Point2D.h>

#ifdef DEBUG_TRIM
#include <iostream>
#endif

namespace SketchPlugin_Tools {

void clearExpressions(AttributeDoublePtr theAttribute)
{
  theAttribute->setText(std::string());
}

void clearExpressions(AttributePointPtr theAttribute)
{
  theAttribute->setText(std::string(), std::string(), std::string());
}

void clearExpressions(AttributePoint2DPtr theAttribute)
{
  theAttribute->setText(std::string(), std::string());
}

void clearExpressions(AttributePtr theAttribute)
{
  // Double
  AttributeDoublePtr anAttributeDouble =
      std::dynamic_pointer_cast<ModelAPI_AttributeDouble>(theAttribute);
  if (anAttributeDouble.get())
    clearExpressions(anAttributeDouble);
  // Point
  AttributePointPtr anAttributePoint =
      std::dynamic_pointer_cast<GeomDataAPI_Point>(theAttribute);
  if (anAttributePoint.get())
    clearExpressions(anAttributePoint);
  // Point2D
  AttributePoint2DPtr anAttributePoint2D =
      std::dynamic_pointer_cast<GeomDataAPI_Point2D>(theAttribute);
  if (anAttributePoint2D.get())
    clearExpressions(anAttributePoint2D);
}

void clearExpressions(FeaturePtr theFeature)
{
  if (!theFeature.get())
    return;

  std::list<AttributePtr> anAttributes = theFeature->data()->attributes(std::string());
  std::list<AttributePtr>::iterator anAttributeIt = anAttributes.begin();
  for (; anAttributeIt != anAttributes.end(); ++anAttributeIt) {
    clearExpressions(*anAttributeIt);
  }
}

std::shared_ptr<GeomAPI_Pnt2d> getCoincidencePoint(const FeaturePtr theStartCoin)
{
  std::shared_ptr<GeomAPI_Pnt2d> aPnt = SketcherPrs_Tools::getPoint(theStartCoin.get(),
                                                          SketchPlugin_Constraint::ENTITY_A());
  if (aPnt.get() == NULL)
    aPnt = SketcherPrs_Tools::getPoint(theStartCoin.get(), SketchPlugin_Constraint::ENTITY_B());
  return aPnt;
}

std::set<FeaturePtr> findCoincidentConstraints(const FeaturePtr& theFeature)
{
  std::set<FeaturePtr> aCoincident;
  const std::set<AttributePtr>& aRefsList = theFeature->data()->refsToMe();
  std::set<AttributePtr>::const_iterator aIt;
  for (aIt = aRefsList.cbegin(); aIt != aRefsList.cend(); ++aIt) {
    FeaturePtr aConstrFeature = std::dynamic_pointer_cast<ModelAPI_Feature>((*aIt)->owner());
    if (aConstrFeature && aConstrFeature->getKind() == SketchPlugin_ConstraintCoincidence::ID())
      aCoincident.insert(aConstrFeature);
  }
  return aCoincident;
}

void findCoincidences(const FeaturePtr theStartCoin,
                      const std::string& theAttr,
                      std::set<FeaturePtr>& theList,
                      const bool theIsAttrOnly)
{
  AttributeRefAttrPtr aPnt = theStartCoin->refattr(theAttr);
  if(!aPnt) {
    return;
  }
  FeaturePtr aObj = ModelAPI_Feature::feature(aPnt->object());
  if(theList.find(aObj) == theList.end()) {
    std::shared_ptr<GeomAPI_Pnt2d> aOrig = getCoincidencePoint(theStartCoin);
    if(aOrig.get() == NULL) {
      return;
    }
    if(!theIsAttrOnly || !aPnt->isObject()) {
      theList.insert(aObj);
    }
    std::set<FeaturePtr> aCoincidences = findCoincidentConstraints(aObj);
    std::set<FeaturePtr>::const_iterator aCIt = aCoincidences.begin();
    for (; aCIt != aCoincidences.end(); ++aCIt) {
      FeaturePtr aConstrFeature = *aCIt;
      std::shared_ptr<GeomAPI_Pnt2d> aPnt = getCoincidencePoint(aConstrFeature);
      if(aPnt.get() && aOrig->isEqual(aPnt)) {
        findCoincidences(aConstrFeature, SketchPlugin_ConstraintCoincidence::ENTITY_A(),
                         theList, theIsAttrOnly);
        findCoincidences(aConstrFeature, SketchPlugin_ConstraintCoincidence::ENTITY_B(),
                         theList, theIsAttrOnly);
      }
    }
  }
}

std::set<FeaturePtr> findFeaturesCoincidentToPoint(const AttributePoint2DPtr& thePoint)
{
  std::set<FeaturePtr> aCoincidentFeatures;

  FeaturePtr anOwner = ModelAPI_Feature::feature(thePoint->owner());
  aCoincidentFeatures.insert(anOwner);

  std::set<FeaturePtr> aCoincidences = findCoincidentConstraints(anOwner);
  std::set<FeaturePtr>::const_iterator aCIt = aCoincidences.begin();
  for (; aCIt != aCoincidences.end(); ++aCIt) {
    bool isPointUsedInCoincidence = false;
    AttributeRefAttrPtr anOtherCoincidentAttr;
    for (int i = 0; i < CONSTRAINT_ATTR_SIZE; ++i) {
      AttributeRefAttrPtr aRefAttr = (*aCIt)->refattr(SketchPlugin_Constraint::ATTRIBUTE(i));
      if (!aRefAttr)
        continue;
      if (!aRefAttr->isObject() && aRefAttr->attr() == thePoint)
        isPointUsedInCoincidence = true;
      else
        anOtherCoincidentAttr = aRefAttr;
    }

    if (isPointUsedInCoincidence) {
      ObjectPtr anObj;
      if (anOtherCoincidentAttr->isObject())
        anObj = anOtherCoincidentAttr->object();
      else
        anObj = anOtherCoincidentAttr->attr()->owner();
      aCoincidentFeatures.insert(ModelAPI_Feature::feature(anObj));
    }
  }

  return aCoincidentFeatures;
}

// Container for point-point coincidences.
// Useful to find points coincident to a given point.
class CoincidentPoints
{
public:
  void addCoincidence(const AttributePoint2DPtr& thePoint1,
                      const AttributePoint2DPtr& thePoint2 = AttributePoint2DPtr())
  {
    std::list< std::set<AttributePoint2DPtr> >::iterator aFound1 = find(thePoint1);
    std::list< std::set<AttributePoint2DPtr> >::iterator aFound2 = find(thePoint2);
    if (aFound1 == myCoincidentPoints.end()) {
      if (aFound2 == myCoincidentPoints.end()) {
        std::set<AttributePoint2DPtr> aNewSet;
        aNewSet.insert(thePoint1);
        if (thePoint2)
          aNewSet.insert(thePoint2);
        myCoincidentPoints.push_back(aNewSet);
      } else
        aFound2->insert(thePoint1);
    } else if (aFound2 == myCoincidentPoints.end()) {
      if (thePoint2)
        aFound1->insert(thePoint2);
    } else {
      aFound1->insert(aFound2->begin(), aFound2->end());
      myCoincidentPoints.erase(aFound2);
    }
  }

  std::set<AttributePoint2DPtr> coincidentPoints(const AttributePoint2DPtr& thePoint)
  {
    collectCoincidentPoints(thePoint);

    std::list< std::set<AttributePoint2DPtr> >::iterator aFound = find(thePoint);
    if (aFound == myCoincidentPoints.end())
      return std::set<AttributePoint2DPtr>();
    return *aFound;
  }

private:
  void coincidences(const FeaturePtr& theFeature,
                    std::set<FeaturePtr>& theCoincidences) const
  {
    // iterate through coincideces for the given feature
    std::set<FeaturePtr> aCoincidences = SketchPlugin_Tools::findCoincidentConstraints(theFeature);
    std::set<FeaturePtr>::const_iterator aCIt = aCoincidences.begin();
    for (; aCIt != aCoincidences.end(); ++aCIt)
    {
      if (theCoincidences.find(*aCIt) != theCoincidences.end())
        continue; // already processed
      theCoincidences.insert(*aCIt);
      // iterate on coincident attributes
      for (int i = 0, aPtInd = 0; i < CONSTRAINT_ATTR_SIZE; ++i) {
        AttributeRefAttrPtr aRefAttr = (*aCIt)->refattr(SketchPlugin_Constraint::ATTRIBUTE(i));
        if (aRefAttr && !aRefAttr->isObject())
        {
          FeaturePtr anOwner = ModelAPI_Feature::feature(aRefAttr->attr()->owner());
          if (anOwner != theFeature)
            coincidences(anOwner, theCoincidences);
        }
      }
    }
  }

  // Iteratively search points coincident to the given point
  // (two points may be coincident through the third point)
  void collectCoincidentPoints(const AttributePoint2DPtr& thePoint)
  {
    AttributePoint2DPtr aPoints[2];

    FeaturePtr anOwner = ModelAPI_Feature::feature(thePoint->owner());
    std::set<FeaturePtr> aCoincidences;
    coincidences(anOwner, aCoincidences);

    std::set<FeaturePtr>::const_iterator aCIt = aCoincidences.begin();
    for (; aCIt != aCoincidences.end(); ++aCIt) {
      aPoints[0] = AttributePoint2DPtr();
      aPoints[1] = AttributePoint2DPtr();
      for (int i = 0, aPtInd = 0; i < CONSTRAINT_ATTR_SIZE; ++i) {
        AttributeRefAttrPtr aRefAttr = (*aCIt)->refattr(SketchPlugin_Constraint::ATTRIBUTE(i));
        if (aRefAttr && !aRefAttr->isObject())
          aPoints[aPtInd++] = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(aRefAttr->attr());
      }

      if (aPoints[0] && aPoints[1])
        addCoincidence(aPoints[0], aPoints[1]);
    }
  }

  std::list< std::set<AttributePoint2DPtr> >::iterator find(const AttributePoint2DPtr& thePoint)
  {
    std::list< std::set<AttributePoint2DPtr> >::iterator aSeek = myCoincidentPoints.begin();
    for (; aSeek != myCoincidentPoints.end(); ++aSeek)
      if (aSeek->find(thePoint) != aSeek->end())
        return aSeek;
    return myCoincidentPoints.end();
  }

private:
  std::list< std::set<AttributePoint2DPtr> > myCoincidentPoints;
};

std::set<AttributePoint2DPtr> findPointsCoincidentToPoint(const AttributePoint2DPtr& thePoint)
{
  CoincidentPoints aCoincidentPoints;
  return aCoincidentPoints.coincidentPoints(thePoint);
}

void resetAttribute(SketchPlugin_Feature* theFeature,
                    const std::string& theId)
{
  AttributePtr anAttr = theFeature->attribute(theId);
  if(anAttr.get()) {
    anAttr->reset();
  }
}

void createCoincidenceOrTangency(SketchPlugin_Feature* theFeature,
                                 const std::string& theId,
                                 const AttributePtr theAttr,
                                 const ObjectPtr theObject,
                                 const bool theIsCanBeTangent)
{
  AttributeRefAttrPtr aRefAttr = theFeature->refattr(theId);
  if(aRefAttr.get() && aRefAttr->isInitialized()) {
    FeaturePtr aConstraint;
    if(!theIsCanBeTangent) {
      aConstraint = theFeature->sketch()
                              ->addFeature(SketchPlugin_ConstraintCoincidence::ID());
    } else {
      if(aRefAttr->isObject()) {
        ObjectPtr anObject = aRefAttr->object();
        FeaturePtr aFeature = ModelAPI_Feature::feature(anObject);
        if(aFeature->getKind() == SketchPlugin_Point::ID()) {
          aConstraint = theFeature->sketch()
                                  ->addFeature(SketchPlugin_ConstraintCoincidence::ID());
        } else {
          aConstraint = theFeature->sketch()
                                  ->addFeature(SketchPlugin_ConstraintTangent::ID());
        }
      } else {
        aConstraint = theFeature->sketch()
                                ->addFeature(SketchPlugin_ConstraintCoincidence::ID());
      }
    }
    AttributeRefAttrPtr aRefAttrA = aConstraint->refattr(SketchPlugin_Constraint::ENTITY_A());
    aRefAttr->isObject() ? aRefAttrA->setObject(aRefAttr->object())
                         : aRefAttrA->setAttr(aRefAttr->attr());
    AttributeRefAttrPtr aRefAttrB = aConstraint->refattr(SketchPlugin_Constraint::ENTITY_B());
    if(theObject.get()) {
      aRefAttrB->setObject(theObject);
    } else if(theAttr.get()) {
      aRefAttrB->setAttr(theAttr);
    }
  }
}

void convertRefAttrToPointOrTangentCurve(const AttributeRefAttrPtr&      theRefAttr,
                                         const AttributePtr&             theDefaultAttr,
                                         std::shared_ptr<GeomAPI_Shape>& theTangentCurve,
                                         std::shared_ptr<GeomAPI_Pnt2d>& thePassingPoint)
{
  AttributePtr anAttr = theDefaultAttr;
  if (theRefAttr->isObject()) {
    FeaturePtr aTgFeature = ModelAPI_Feature::feature(theRefAttr->object());
    if (aTgFeature) {
      if (aTgFeature->getKind() != SketchPlugin_Point::ID()) {
        theTangentCurve = aTgFeature->lastResult()->shape();
        return;
      }
      anAttr = aTgFeature->attribute(SketchPlugin_Point::COORD_ID());
    }
  } else
    anAttr = theRefAttr->attr();

  thePassingPoint = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(anAttr)->pnt();
}


FeaturePtr createConstraintAttrAttr(SketchPlugin_Sketch* theSketch,
                                    const std::string& theConstraintId,
                                    const AttributePtr& theFirstAttribute,
                                    const AttributePtr& theSecondAttribute)
{
  FeaturePtr aConstraint = theSketch->addFeature(theConstraintId);
  AttributeRefAttrPtr aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_A()));
  aRefAttr->setAttr(theFirstAttribute);

  aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_B()));
  aRefAttr->setAttr(theSecondAttribute);

#if defined(DEBUG_TRIM) || defined(DEBUG_SPLIT)
  std::cout << "<createConstraint to attribute> :"
            << " first attribute - " << theFirstAttribute->id()
            << " second attribute - " << theSecondAttribute->id()
            << std::endl;
#endif

  return aConstraint;
}

FeaturePtr createConstraintAttrObject(SketchPlugin_Sketch* theSketch,
                                      const std::string& theConstraintId,
                                      const AttributePtr& theFirstAttribute,
                                      const ObjectPtr& theSecondObject)
{
  FeaturePtr aConstraint = theSketch->addFeature(theConstraintId);
  AttributeRefAttrPtr aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_A()));
  aRefAttr->setAttr(theFirstAttribute);

  aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_B()));
  aRefAttr->setObject(theSecondObject);

#if defined(DEBUG_TRIM) || defined(DEBUG_SPLIT)
  std::cout << "<createConstraint to attribute> :"
            << " first attribute - " << theFirstAttribute->id()
            << " second object - " << ModelAPI_Feature::feature(theSecondObject)->getKind()
            << std::endl;
#endif

  return aConstraint;
}

FeaturePtr createConstraintObjectObject(SketchPlugin_Sketch* theSketch,
                                        const std::string& theConstraintId,
                                        const ObjectPtr& theFirstObject,
                                        const ObjectPtr& theSecondObject)
{
  FeaturePtr aConstraint = theSketch->addFeature(theConstraintId);
  AttributeRefAttrPtr aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_A()));
  aRefAttr->setObject(theFirstObject);

  aRefAttr = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
                                 aConstraint->attribute(SketchPlugin_Constraint::ENTITY_B()));
  aRefAttr->setObject(theSecondObject);

#if defined(DEBUG_TRIM) || defined(DEBUG_SPLIT)
  std::cout << "<createConstraint to attribute> :"
            << " first object - " << ModelAPI_Feature::feature(theFirstObject)->getKind()
            << " second object - " << ModelAPI_Feature::feature(theSecondObject)->getKind()
            << std::endl;
#endif

  return aConstraint;
}

GeomPnt2dPtr flyoutPointCoordinates(const ConstraintPtr& theConstraint)
{
  // currently process Length constraints only
  if (theConstraint->getKind() != SketchPlugin_ConstraintLength::ID())
    return GeomPnt2dPtr();

  AttributeRefAttrPtr aLineAttr = theConstraint->refattr(SketchPlugin_Constraint::ENTITY_A());
  if (!aLineAttr || !aLineAttr->isObject())
    return GeomPnt2dPtr();
  FeaturePtr aLine = ModelAPI_Feature::feature(aLineAttr->object());
  if (!aLine || aLine->getKind() != SketchPlugin_Line::ID())
    return GeomPnt2dPtr();

  std::shared_ptr<GeomAPI_XY> aStartPnt = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
      aLine->attribute(SketchPlugin_Line::START_ID()))->pnt()->xy();
  std::shared_ptr<GeomAPI_XY> aEndPnt = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
      aLine->attribute(SketchPlugin_Line::END_ID()))->pnt()->xy();

  std::shared_ptr<GeomDataAPI_Point2D> aFlyoutAttr =
      std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
      theConstraint->attribute(SketchPlugin_Constraint::FLYOUT_VALUE_PNT()));
  std::shared_ptr<GeomAPI_Pnt2d> aFltPnt = aFlyoutAttr->pnt();

  std::shared_ptr<GeomAPI_Dir2d> aLineDir(new GeomAPI_Dir2d(aEndPnt->decreased(aStartPnt)));

  double X = aStartPnt->x() + aFltPnt->x() * aLineDir->x() - aFltPnt->y() * aLineDir->y();
  double Y = aStartPnt->y() + aFltPnt->x() * aLineDir->y() + aFltPnt->y() * aLineDir->x();

  return GeomPnt2dPtr(new GeomAPI_Pnt2d(X, Y));
}

} // namespace SketchPlugin_Tools
