// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:    SketchPlugin_ConstraintRadius.h
// Created: 26 May 2014
// Author:  Artem ZHIDKOV

#ifndef SketchPlugin_ConstraintRadius_H_
#define SketchPlugin_ConstraintRadius_H_

#include "SketchPlugin.h"
#include <SketchPlugin_Sketch.h>
#include "SketchPlugin_ConstraintBase.h"

/** \class SketchPlugin_ConstraintRadius
 *  \ingroup Plugins
 *  \brief Feature for creation of a new constraint which defines 
 *         a radius of a circle or an arc of circle
 *
 *  This constraint has two attributes:
 *  SketchPlugin_Constraint::VALUE() (radius), SketchPlugin_Constraint::ENTITY_A() (a circle)
 */
class SketchPlugin_ConstraintRadius : public SketchPlugin_ConstraintBase
{
 public:
  /// Radius constraint kind
  inline static const std::string& ID()
  {
    static const std::string MY_CONSTRAINT_RADIUS_ID("SketchConstraintRadius");
    return MY_CONSTRAINT_RADIUS_ID;
  }
  /// \brief Returns the kind of a feature
  SKETCHPLUGIN_EXPORT virtual const std::string& getKind()
  {
    static std::string MY_KIND = SketchPlugin_ConstraintRadius::ID();
    return MY_KIND;
  }

  /// \brief Creates a new part document if needed
  SKETCHPLUGIN_EXPORT virtual void execute();

  /// Computes the attribute value on the base of other attributes if the value can be computed
  /// \param theAttributeId an attribute index to be computed
  /// \return a boolean value about it is computed
  SKETCHPLUGIN_EXPORT virtual bool compute(const std::string& theAttributeId);

  /// \brief Request for initialization of data model of the feature: adding all attributes
  SKETCHPLUGIN_EXPORT virtual void initAttributes();

  /// Retuns the parameters of color definition in the resources config manager
  SKETCHPLUGIN_EXPORT virtual void colorConfigInfo(std::string& theSection, std::string& theName,
                                                   std::string& theDefault);

  /// Returns the AIS preview
  SKETCHPLUGIN_EXPORT virtual AISObjectPtr getAISObject(AISObjectPtr thePrevious);

  /// Moves the feature
  /// \param theDeltaX the delta for X coordinate is moved
  /// \param theDeltaY the delta for Y coordinate is moved
  SKETCHPLUGIN_EXPORT virtual void move(const double theDeltaX, const double theDeltaY);

  /// Called on change of any argument-attribute of this object
  /// \param theID identifier of changed attribute
  SKETCHPLUGIN_EXPORT virtual void attributeChanged(const std::string& theID);

  /// \brief Use plugin manager for features creation
  SketchPlugin_ConstraintRadius();

private:
  /// Checks and gets the radius of referenced circle (or arc) otherwise returns -1.
  /// \param theCircData the found referenced circle returned by this method
  double circleRadius(std::shared_ptr<ModelAPI_Feature>& theCirc);

private:
  bool myFlyoutUpdate; ///< to avoid cyclic dependencies on automatic updates of flyout point
};

#endif
