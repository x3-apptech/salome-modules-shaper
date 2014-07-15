// File:    SketchSolver_Constraint.cpp
// Created: 27 May 2014
// Author:  Artem ZHIDKOV

#include "SketchSolver_Constraint.h"
#include <SketchSolver_Solver.h>

#include <SketchPlugin_Line.h>
#include <SketchPlugin_Point.h>
#include <SketchPlugin_Circle.h>
#include <SketchPlugin_Arc.h>
#include <SketchPlugin_ConstraintDistance.h>
#include <SketchPlugin_ConstraintLength.h>
#include <SketchPlugin_ConstraintParallel.h>
#include <SketchPlugin_ConstraintPerpendicular.h>
#include <SketchPlugin_ConstraintRadius.h>

#include <ModelAPI_AttributeRefAttr.h>
#include <ModelAPI_Data.h>

#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Point2D.h>


SketchSolver_Constraint::SketchSolver_Constraint()
  : myConstraint(boost::shared_ptr<SketchPlugin_Constraint>()),
    myType(SLVS_C_UNKNOWN),
    myAttributesList()
{
}

SketchSolver_Constraint::SketchSolver_Constraint(
                boost::shared_ptr<SketchPlugin_Constraint> theConstraint)
  : myConstraint(theConstraint),
    myAttributesList()
{
  myType = getType(myConstraint);
}

const int& SketchSolver_Constraint::getType(boost::shared_ptr<SketchPlugin_Constraint> theConstraint)
{
  myType = SLVS_C_UNKNOWN;
  if (!theConstraint)
    return getType();

  // Assign empty names of attributes
  myAttributesList.clear();
  for (int i = 0; i < CONSTRAINT_ATTR_SIZE; i++)
    myAttributesList.push_back(std::string());

  const std::string& aConstraintKind = theConstraint->getKind();
  // Constraint for coincidence of two points
  if (aConstraintKind.compare("SketchConstraintCoincidence") == 0)
  {
    int anAttrPos = 0;
    // Verify the constraint has only two attributes and they are points
    int aPt2d = 0; // bit-mapped field, each bit indicates whether the attribute is 2D point
    int aPt3d = 0; // bit-mapped field, the same information for 3D points
    for (unsigned int indAttr = 0; indAttr < CONSTRAINT_ATTR_SIZE; indAttr++)
    {
      boost::shared_ptr<ModelAPI_AttributeRefAttr> anAttr =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          theConstraint->data()->attribute(SketchPlugin_Constraint::ATTRIBUTE(indAttr))
        );
      if (!anAttr) continue;
      // Verify the attribute is a 2D point
      boost::shared_ptr<GeomDataAPI_Point2D> aPoint2D =
        boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(anAttr->attr());
      if (aPoint2D)
      {
        aPt2d |= (1 << indAttr);
        myAttributesList[anAttrPos++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
        continue;
      }
      // Verify the attribute is a 3D point
      boost::shared_ptr<GeomDataAPI_Point> aPoint3D =
        boost::dynamic_pointer_cast<GeomDataAPI_Point>(anAttr->attr());
      if (aPoint3D)
      {
        aPt3d |= (1 << indAttr);
        myAttributesList[anAttrPos++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
        continue;
      }
      // Attribute neither 2D nor 3D point is not supported by this type of constraint
      return getType();
    }
    // The constrained points should be in first and second positions,
    // so the expected value of aPt2d or aPt3d is 3
    if ((aPt2d == 3 && aPt3d == 0) || (aPt2d == 0 && aPt3d == 3))
      myType = SLVS_C_POINTS_COINCIDENT;
    // Constraint parameters are wrong
    return getType();
  }

  // Constraint for distance between point and another entity
  if (aConstraintKind.compare(SketchPlugin_ConstraintDistance::ID()) == 0)
  {
    int aNbPoints = 0;
    int aNbEntities = 0;
    for (unsigned int indAttr = 0; indAttr < CONSTRAINT_ATTR_SIZE; indAttr++)
    {
      boost::shared_ptr<ModelAPI_AttributeRefAttr> anAttr =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          theConstraint->data()->attribute(SketchPlugin_Constraint::ATTRIBUTE(indAttr))
        );
      if (!anAttr) continue;
      if (anAttr->isObject() && anAttr->object())
      { // verify posiible entities
        const std::string& aKind = boost::dynamic_pointer_cast<ModelAPI_Feature>
          (anAttr->object())->getKind();
        if (aKind.compare(SketchPlugin_Point::ID()) == 0)
        {
          myAttributesList[aNbPoints++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
          continue;
        }
        else if(aKind.compare(SketchPlugin_Line::ID()) == 0)
        {
          // entities are placed starting from SketchPlugin_Constraint::ENTITY_C() attribute
          myAttributesList[2 + aNbEntities++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
          myType = SLVS_C_PT_LINE_DISTANCE;
          continue;
        }
      }
      else
      { // verify points
        // Verify the attribute is a 2D point
        boost::shared_ptr<GeomDataAPI_Point2D> aPoint2D =
          boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(anAttr->attr());
        if (aPoint2D)
        {
          myAttributesList[aNbPoints++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
          continue;
        }
        // Verify the attribute is a 3D point
        boost::shared_ptr<GeomDataAPI_Point> aPoint3D =
          boost::dynamic_pointer_cast<GeomDataAPI_Point>(anAttr->attr());
        if (aPoint3D)
        {
          myAttributesList[aNbPoints++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
          continue;
        }
      }
    }
    // Verify the correctness of constraint arguments
    if (aNbPoints == 2 && aNbEntities ==0)
      myType = SLVS_C_PT_PT_DISTANCE;
    else if (aNbPoints == 1 && aNbEntities == 1)
      myType = SLVS_C_UNKNOWN;
    return getType();
  }

  // Constraint for the given length of a line
  if (aConstraintKind.compare(SketchPlugin_ConstraintLength::ID()) == 0)
  {
    int aNbLines = 0;
    for (unsigned int indAttr = 0; indAttr < CONSTRAINT_ATTR_SIZE; indAttr++)
    {
      boost::shared_ptr<ModelAPI_AttributeRefAttr> anAttr =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          theConstraint->data()->attribute(SketchPlugin_Constraint::ATTRIBUTE(indAttr))
        );
      if (!anAttr) continue;
      if (anAttr->isObject() && anAttr->object() &&
        boost::dynamic_pointer_cast<ModelAPI_Feature>(anAttr->object())->getKind().
        compare(SketchPlugin_Line::ID()) == 0)
      {
        myAttributesList[aNbLines++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
        break;
      }
    }
    if (aNbLines == 1)
      myType = SLVS_C_PT_PT_DISTANCE;
    return getType();
  }

  // Constraint for two parallel/perpendicular lines
  bool isParallel = (aConstraintKind.compare(SketchPlugin_ConstraintParallel::ID()) == 0);
  bool isPerpendicular = (aConstraintKind.compare(SketchPlugin_ConstraintPerpendicular::ID()) == 0);
  if (isParallel || isPerpendicular)
  {
    int aNbEntities = 2; // lines in SolveSpace constraints should started from SketchPlugin_Constraint::ENTITY_C() attribute
    for (unsigned int indAttr = 0; indAttr < CONSTRAINT_ATTR_SIZE; indAttr++)
    {
      boost::shared_ptr<ModelAPI_AttributeRefAttr> anAttr =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          theConstraint->data()->attribute(SketchPlugin_Constraint::ATTRIBUTE(indAttr))
        );
      if (!anAttr || !anAttr->isObject() || !anAttr->object()) continue;
      const std::string& aKind = boost::dynamic_pointer_cast<ModelAPI_Feature>
        (anAttr->object())->getKind();
      if (aKind.compare(SketchPlugin_Line::ID()) == 0)
      {
        myAttributesList[aNbEntities++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
        continue;
      }
    }
    if (aNbEntities == 4)
      myType = isParallel ? SLVS_C_PARALLEL : SLVS_C_PERPENDICULAR;
    return getType();
  }

  // Constraint for radius of a circle or an arc of circle
  if (aConstraintKind.compare(SketchPlugin_ConstraintRadius::ID()) == 0)
  {
    int aNbEntities = 2; // lines in SolveSpace constraints should started from SketchPlugin_Constraint::ENTITY_C() attribute
    for (unsigned int indAttr = 0; indAttr < CONSTRAINT_ATTR_SIZE; indAttr++)
    {
      boost::shared_ptr<ModelAPI_AttributeRefAttr> anAttr =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          theConstraint->data()->attribute(SketchPlugin_Constraint::ATTRIBUTE(indAttr))
        );
      if (!anAttr || !anAttr->isObject() || !anAttr->object()) continue;
      const std::string& aKind = boost::dynamic_pointer_cast<ModelAPI_Feature>
        (anAttr->object())->getKind();
      if (aKind.compare(SketchPlugin_Circle::ID()) == 0 || aKind.compare(SketchPlugin_Arc::ID()) == 0)
      {
        myAttributesList[aNbEntities++] = SketchPlugin_Constraint::ATTRIBUTE(indAttr);
        continue;
      }
    }
    if (aNbEntities == 3)
      myType = SLVS_C_DIAMETER;
    return getType();
  }

  /// \todo Implement other kind of constrtaints

  return getType();
}
