// Copyright (C) 2018-20xx  CEA/DEN, EDF R&D
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

#include "FeaturesPlugin_Measurement.h"

#include <ModelAPI_AttributeDoubleArray.h>
#include <ModelAPI_AttributeSelection.h>
#include <ModelAPI_AttributeString.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_Session.h>

#include <GeomAPI_Angle.h>
#include <GeomAPI_Circ.h>
#include <GeomAPI_Edge.h>
#include <GeomAPI_Face.h>
#include <GeomAPI_Pnt.h>
#include <GeomAPI_Shape.h>
#include <GeomAPI_ShapeIterator.h>
#include <GeomAPI_Vertex.h>

#include <GeomAlgoAPI_ShapeTools.h>

#include <iomanip>
#include <sstream>

FeaturesPlugin_Measurement::FeaturesPlugin_Measurement()
{
}

void FeaturesPlugin_Measurement::initAttributes()
{
  data()->addAttribute(FeaturesPlugin_Measurement::MEASURE_KIND(),
                       ModelAPI_AttributeString::typeId());

  // attribute for length
  data()->addAttribute(EDGE_FOR_LENGTH_ID(), ModelAPI_AttributeSelection::typeId());
  // attributes for distance
  data()->addAttribute(DISTANCE_FROM_OBJECT_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(DISTANCE_TO_OBJECT_ID(), ModelAPI_AttributeSelection::typeId());
  // attribute for radius
  data()->addAttribute(CIRCULAR_OBJECT_ID(), ModelAPI_AttributeSelection::typeId());
  // attributes for angle
  data()->addAttribute(ANGLE_FROM_EDGE_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(ANGLE_TO_EDGE_ID(), ModelAPI_AttributeSelection::typeId());
  // attributes for angle by 3 points
  data()->addAttribute(ANGLE_POINT1_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(ANGLE_POINT2_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(ANGLE_POINT3_ID(), ModelAPI_AttributeSelection::typeId());
  // attributes for result message and values
  data()->addAttribute(RESULT_ID(), ModelAPI_AttributeString::typeId());
  data()->addAttribute(RESULT_VALUES_ID(), ModelAPI_AttributeDoubleArray::typeId());
}

void FeaturesPlugin_Measurement::execute()
{
}

void FeaturesPlugin_Measurement::attributeChanged(const std::string& theID)
{
  if (theID == MEASURE_KIND()) {
    // clear results
    string(RESULT_ID())->setValue("");
    std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(
      attribute(RESULT_VALUES_ID()))->setSize(0);
  }
  if (theID != RESULT_ID()) {
    std::string aKind = string(MEASURE_KIND())->value();
    if (aKind == MEASURE_LENGTH())
      computeLength();
    else if (aKind == MEASURE_DISTANCE())
      computeDistance();
    else if (aKind == MEASURE_RADIUS())
      computeRadius();
    else if (aKind == MEASURE_ANGLE())
      computeAngle();
    else if (aKind == MEASURE_ANGLE_POINTS())
      computeAngleByPoints();
  }
}

void FeaturesPlugin_Measurement::computeLength()
{
  AttributeSelectionPtr aSelectedFeature = selection(EDGE_FOR_LENGTH_ID());

  GeomShapePtr aShape;
  GeomEdgePtr anEdge;
  if (aSelectedFeature && aSelectedFeature->isInitialized()) {
    aShape = aSelectedFeature->value();
    if (!aShape && aSelectedFeature->context())
      aShape = aSelectedFeature->context()->shape();
  }
  if (aShape && aShape->isEdge())
    anEdge = GeomEdgePtr(new GeomAPI_Edge(aShape));
  if (!anEdge) {
    string(RESULT_ID())->setValue("");
    return;
  }

  std::ostringstream anOutput;
  anOutput << "Length = " << std::setprecision(10) << anEdge->length();
  string(RESULT_ID())->setValue(anOutput.str());

  AttributeDoubleArrayPtr aValues =
      std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(attribute(RESULT_VALUES_ID()));
  aValues->setSize(1);
  aValues->setValue(0, anEdge->length());
}

void FeaturesPlugin_Measurement::computeDistance()
{
  AttributeSelectionPtr aFirstFeature = selection(DISTANCE_FROM_OBJECT_ID());
  GeomShapePtr aShape1;
  if (aFirstFeature && aFirstFeature->isInitialized()) {
    aShape1 = aFirstFeature->value();
    if (!aShape1 && aFirstFeature->context())
      aShape1 = aFirstFeature->context()->shape();
  }

  AttributeSelectionPtr aSecondFeature = selection(DISTANCE_TO_OBJECT_ID());
  GeomShapePtr aShape2;
  if (aSecondFeature && aSecondFeature->isInitialized()) {
    aShape2 = aSecondFeature->value();
    if (!aShape2 && aSecondFeature->context())
      aShape2 = aSecondFeature->context()->shape();
  }

  if (!aShape1 || !aShape2) {
    string(RESULT_ID())->setValue("");
    return;
  }

  double aDistance = GeomAlgoAPI_ShapeTools::minimalDistance(aShape1, aShape2);

  std::ostringstream anOutput;
  anOutput << "Distance = " << std::setprecision(10) << aDistance;
  string(RESULT_ID())->setValue(anOutput.str());

  AttributeDoubleArrayPtr aValues =
      std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(attribute(RESULT_VALUES_ID()));
  aValues->setSize(1);
  aValues->setValue(0, aDistance);
}

void FeaturesPlugin_Measurement::computeRadius()
{
  AttributeSelectionPtr aSelectedFeature = selection(CIRCULAR_OBJECT_ID());

  GeomShapePtr aShape;
  if (aSelectedFeature && aSelectedFeature->isInitialized()) {
    aShape = aSelectedFeature->value();
    if (!aShape && aSelectedFeature->context())
      aShape = aSelectedFeature->context()->shape();
  }

  double aRadius = -1.0;
  if (aShape) {
    if (aShape->isEdge()) {
      GeomEdgePtr anEdge(new GeomAPI_Edge(aShape));
      if (anEdge->isCircle() || anEdge->isArc()) {
        aRadius = anEdge->circle()->radius();
      }
    } else if (aShape->isFace()) {
      GeomFacePtr aFace(new GeomAPI_Face(aShape));
      aRadius = GeomAlgoAPI_ShapeTools::radius(aFace);
    }
  }

  if (aRadius < 0.0) {
    string(RESULT_ID())->setValue("");
    return;
  }

  std::ostringstream anOutput;
  anOutput << "Radius = " << std::setprecision(10) << aRadius;
  string(RESULT_ID())->setValue(anOutput.str());

  AttributeDoubleArrayPtr aValues =
      std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(attribute(RESULT_VALUES_ID()));
  aValues->setSize(1);
  aValues->setValue(0, aRadius);
}

void FeaturesPlugin_Measurement::computeAngle()
{
  AttributeSelectionPtr aFirstFeature = selection(ANGLE_FROM_EDGE_ID());
  GeomShapePtr aShape1;
  GeomEdgePtr anEdge1;
  if (aFirstFeature && aFirstFeature->isInitialized()) {
    aShape1 = aFirstFeature->value();
    if (!aShape1 && aFirstFeature->context())
      aShape1 = aFirstFeature->context()->shape();
  }
  if (aShape1 && aShape1->isEdge())
    anEdge1 = GeomEdgePtr(new GeomAPI_Edge(aShape1));

  AttributeSelectionPtr aSecondFeature = selection(ANGLE_TO_EDGE_ID());
  GeomShapePtr aShape2;
  GeomEdgePtr anEdge2;
  if (aSecondFeature && aSecondFeature->isInitialized()) {
    aShape2 = aSecondFeature->value();
    if (!aShape2 && aSecondFeature->context())
      aShape2 = aSecondFeature->context()->shape();
  }
  if (aShape2 && aShape2->isEdge())
    anEdge2 = GeomEdgePtr(new GeomAPI_Edge(aShape2));

  if (!anEdge1 || !anEdge2) {
    string(RESULT_ID())->setValue("");
    return;
  }

  GeomShapePtr anInter = anEdge1->intersect(anEdge2);

  std::ostringstream anOutput;
  anOutput << std::setprecision(10);
  std::list<double> aValuesList;
  if (anInter) {
    if (anInter->isVertex()) {
      std::shared_ptr<GeomAPI_Vertex> aVertex(new GeomAPI_Vertex(anInter));
      std::shared_ptr<GeomAPI_Angle> anAngle(
          new GeomAPI_Angle(anEdge1, anEdge2, aVertex->point()));
      double anAngleValue = anAngle->angleDegree();
      anOutput << "Angle = " << std::setprecision(10) << anAngleValue << std::endl;
      aValuesList.push_back(anAngleValue);
    }
    else {
      GeomAPI_ShapeIterator anIt(anInter);
      for (int anIndex = 1; anIt.more(); anIt.next(), ++anIndex) {
        GeomShapePtr aCurrent = anIt.current();
        if (!aCurrent->isVertex())
          continue;
        std::shared_ptr<GeomAPI_Vertex> aVertex(new GeomAPI_Vertex(aCurrent));
        std::shared_ptr<GeomAPI_Angle> anAngle(
            new GeomAPI_Angle(anEdge1, anEdge2, aVertex->point()));
        double anAngleValue = anAngle->angleDegree();
        anOutput << "Angle" << anIndex << " = "
                 << std::setprecision(10) << anAngleValue << std::endl;
        aValuesList.push_back(anAngleValue);
      }
    }
  }

  string(RESULT_ID())->setValue(anOutput.str());

  AttributeDoubleArrayPtr aValues =
      std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(attribute(RESULT_VALUES_ID()));
  aValues->setSize((int)aValuesList.size());
  int anIndex = 0;
  for (std::list<double>::iterator anIt = aValuesList.begin(); anIt != aValuesList.end(); ++anIt)
    aValues->setValue(anIndex++, *anIt);
}

static GeomVertexPtr selectionToVertex(const AttributeSelectionPtr& aSelection)
{
  GeomShapePtr aShape;
  GeomVertexPtr aVertex;
  if (aSelection && aSelection->isInitialized()) {
    aShape = aSelection->value();
    if (!aShape && aSelection->context())
      aShape = aSelection->context()->shape();
  }
  if (aShape && aShape->isVertex())
    aVertex = GeomVertexPtr(new GeomAPI_Vertex(aShape));
  return aVertex;
}

void FeaturesPlugin_Measurement::computeAngleByPoints()
{
  GeomVertexPtr aVertex1 = selectionToVertex(selection(ANGLE_POINT1_ID()));
  GeomVertexPtr aVertex2 = selectionToVertex(selection(ANGLE_POINT2_ID()));
  GeomVertexPtr aVertex3 = selectionToVertex(selection(ANGLE_POINT3_ID()));

  if (!aVertex1 || !aVertex2 || ! aVertex3) {
    string(RESULT_ID())->setValue("");
    return;
  }

  std::shared_ptr<GeomAPI_Angle> anAngle(
      new GeomAPI_Angle(aVertex1->point(), aVertex2->point(), aVertex3->point()));
  double anAngleValue = anAngle->angleDegree();

  std::ostringstream anOutput;
  anOutput << "Angle = " << std::setprecision(10) << anAngleValue;
  string(RESULT_ID())->setValue(anOutput.str());

  AttributeDoubleArrayPtr aValues =
    std::dynamic_pointer_cast<ModelAPI_AttributeDoubleArray>(attribute(RESULT_VALUES_ID()));
  aValues->setSize(1);
  aValues->setValue(0, anAngleValue);
}