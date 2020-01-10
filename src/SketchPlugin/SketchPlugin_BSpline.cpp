// Copyright (C) 2019-2020  CEA/DEN, EDF R&D
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

#include <SketchPlugin_BSpline.h>
#include <SketchPlugin_Sketch.h>

#include <GeomAlgoAPI_EdgeBuilder.h>

#include <GeomAPI_Pnt2d.h>

#include <GeomDataAPI_Point2DArray.h>

#include <ModelAPI_AttributeDoubleArray.h>
#include <ModelAPI_AttributeIntArray.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_ResultConstruction.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Validator.h>


SketchPlugin_BSpline::SketchPlugin_BSpline()
  : SketchPlugin_SketchEntity()
{
}

void SketchPlugin_BSpline::initDerivedClassAttributes()
{
  data()->addAttribute(POLES_ID(), GeomDataAPI_Point2DArray::typeId());
  data()->addAttribute(WEIGHTS_ID(), ModelAPI_AttributeDoubleArray::typeId());
  data()->addAttribute(KNOTS_ID(), ModelAPI_AttributeDoubleArray::typeId());
  data()->addAttribute(MULTS_ID(), ModelAPI_AttributeIntArray::typeId());
  data()->addAttribute(DEGREE_ID(), ModelAPI_AttributeInteger::typeId());

  data()->addAttribute(EXTERNAL_ID(), ModelAPI_AttributeSelection::typeId());
  ModelAPI_Session::get()->validators()->registerNotObligatory(getKind(), EXTERNAL_ID());
}

void SketchPlugin_BSpline::execute()
{
  SketchPlugin_Sketch* aSketch = sketch();
  if(!aSketch) {
    return;
  }

  AttributePoint2DArrayPtr aPolesArray =
      std::dynamic_pointer_cast<GeomDataAPI_Point2DArray>(attribute(POLES_ID()));
  AttributeDoubleArrayPtr aWeightsArray = data()->realArray(WEIGHTS_ID());
  AttributeDoubleArrayPtr aKnotsArray = data()->realArray(KNOTS_ID());
  AttributeIntArrayPtr aMultsArray = data()->intArray(MULTS_ID());
  AttributeIntegerPtr aDegreeAttr = data()->integer(DEGREE_ID());

  // collect poles
  std::list<GeomPnt2dPtr> aPoles2D;
  for (int anIndex = 0; anIndex < aPolesArray->size(); ++anIndex) {
    GeomPnt2dPtr aPole = aPolesArray->pnt(anIndex);
    aPoles2D.push_back(aPole);
  }
  // collect weights
  std::list<double> aWeights;
  for (int anIndex = 0; anIndex < aWeightsArray->size(); ++anIndex)
    aWeights.push_back(aWeightsArray->value(anIndex));
  // collect knots
  std::list<double> aKnots;
  for (int anIndex = 0; anIndex < aKnotsArray->size(); ++anIndex)
    aKnots.push_back(aKnotsArray->value(anIndex));
  // collect multiplicities
  std::list<int> aMults;
  for (int anIndex = 0; anIndex < aMultsArray->size(); ++anIndex)
    aMults.push_back(aMultsArray->value(anIndex));

  // create result non-periodic B-spline curve
  GeomShapePtr anEdge = GeomAlgoAPI_EdgeBuilder::bsplineOnPlane(aSketch->coordinatePlane(),
      aPoles2D, aWeights, aKnots, aMults, aDegreeAttr->value(), false);

  ResultConstructionPtr aResult = document()->createConstruction(data(), 0);
  aResult->setShape(anEdge);
  aResult->setIsInHistory(false);
  setResult(aResult, 0);
}

bool SketchPlugin_BSpline::isFixed() {
  return data()->selection(EXTERNAL_ID())->context().get() != NULL;
}

void SketchPlugin_BSpline::attributeChanged(const std::string& theID) {
  // the second condition for unability to move external segments anywhere
  if (theID == EXTERNAL_ID() || isFixed()) {
    std::shared_ptr<GeomAPI_Shape> aSelection = data()->selection(EXTERNAL_ID())->value();
    if (!aSelection) {
      // empty shape in selection shows that the shape is equal to context
      ResultPtr anExtRes = selection(EXTERNAL_ID())->context();
      if (anExtRes)
        aSelection = anExtRes->shape();
    }
////    // update arguments due to the selection value
////    if (aSelection && !aSelection->isNull() && aSelection->isEdge()) {
////      std::shared_ptr<GeomAPI_Edge> anEdge(new GeomAPI_Edge(aSelection));
////      std::shared_ptr<GeomAPI_Ellipse> anEllipse = anEdge->ellipse();
////
////      bool aWasBlocked = data()->blockSendAttributeUpdated(true);
////      std::shared_ptr<GeomDataAPI_Point2D> aCenterAttr =
////        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(attribute(CENTER_ID()));
////      aCenterAttr->setValue(sketch()->to2D(anEllipse->center()));
////
////      std::shared_ptr<GeomDataAPI_Point2D> aFocusAttr =
////        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(attribute(FIRST_FOCUS_ID()));
////      aFocusAttr->setValue(sketch()->to2D(anEllipse->firstFocus()));
////
////      std::shared_ptr<GeomDataAPI_Point2D> aStartAttr =
////        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(attribute(START_POINT_ID()));
////      aStartAttr->setValue(sketch()->to2D(anEdge->firstPoint()));
////
////      std::shared_ptr<GeomDataAPI_Point2D> aEndAttr =
////        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(attribute(END_POINT_ID()));
////      aEndAttr->setValue(sketch()->to2D(anEdge->lastPoint()));
////
////      real(MAJOR_RADIUS_ID())->setValue(anEllipse->majorRadius());
////      real(MINOR_RADIUS_ID())->setValue(anEllipse->minorRadius());
////
////      double aStartParam, aMidParam, aEndParam;
////      anEllipse->parameter(anEdge->firstPoint(), tolerance, aStartParam);
////      anEllipse->parameter(anEdge->middlePoint(), tolerance, aMidParam);
////      anEllipse->parameter(anEdge->lastPoint(), tolerance, aEndParam);
////      if (aEndParam < aStartParam)
////        aEndParam += 2.0 * PI;
////      if (aMidParam < aStartParam)
////        aMidParam += 2.0 * PI;
////      boolean(REVERSED_ID())->setValue(aMidParam > aEndParam);
////
////      data()->blockSendAttributeUpdated(aWasBlocked, false);
////
////      fillCharacteristicPoints();
////    }
  }
////  else if (theID == CENTER_ID() || theID == FIRST_FOCUS_ID() ||
////           theID == START_POINT_ID() || theID == END_POINT_ID())
////    fillCharacteristicPoints();
////  else if (theID == REVERSED_ID() && myParamDelta == 0.0)
////    myParamDelta = 2.0 * PI;
}
