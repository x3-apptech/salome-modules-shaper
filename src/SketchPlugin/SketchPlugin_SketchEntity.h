// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:        SketchPlugin_SketchEntity.h
// Created:     05 Mar 2015
// Author:      Natalia ERMOLAEVA

#ifndef SketchPlugin_SketchEntity_H_
#define SketchPlugin_SketchEntity_H_

#include "SketchPlugin.h"
#include "SketchPlugin_Feature.h"

#include <ModelAPI_CompositeFeature.h>
#include <GeomAPI_Shape.h>
#include <GeomAPI_AISObject.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_AttributeSelection.h>
#include <ModelAPI_AttributeBoolean.h>
#include <GeomAPI_ICustomPrs.h>

#include <Config_PropManager.h>

#define SKETCH_EDGE_COLOR "#ff0000"
#define SKETCH_POINT_COLOR "#ff0000"
#define SKETCH_EXTERNAL_EDGE_COLOR "#00ff00"
#define SKETCH_CONSTRUCTION_COLOR "#000000"

/**\class SketchPlugin_SketchEntity
 * \ingroup Plugins
 * \brief Sketch Entity for creation of the new feature in PartSet. This is an abstract class to give
 * an interface to create the entity features such as line, circle, arc and point.
 */
class SketchPlugin_SketchEntity : public SketchPlugin_Feature, public GeomAPI_ICustomPrs
{
 public:
  /// Reference to the construction type of the feature
  inline static const std::string& CONSTRUCTION_ID()
  {
    static const std::string MY_CONSTRUCTION_ID("Construction");
    return MY_CONSTRUCTION_ID;
  }

  /// Reference to the external edge or vertex as a AttributeSelection
  inline static const std::string& EXTERNAL_ID()
  {
    static const std::string MY_EXTERNAL_ID("External");
    return MY_EXTERNAL_ID;
  }

  /// Request for initialization of data model of the feature: adding all attributes
  virtual void initAttributes();

  /// Returns true of the feature is created basing on the external shape of not-this-sketch object
  virtual bool isExternal() const
  {
    AttributeSelectionPtr aAttr = data()->selection(EXTERNAL_ID());
    if (aAttr)
      return aAttr->context().get() != NULL;
    return false;
  }

  /// Customize presentation of the feature
  virtual void customisePresentation(AISObjectPtr thePrs)
  {
    std::vector<int> aRGB;
  
    int aShapeType = thePrs->getShapeType();
    if (aShapeType != 6/*an edge*/ && aShapeType != 7/*a vertex*/)
      return;

    std::shared_ptr<ModelAPI_AttributeBoolean> aConstructionAttr =
                                   data()->boolean(SketchPlugin_SketchEntity::CONSTRUCTION_ID());
    bool isConstruction = aConstructionAttr.get() != NULL && aConstructionAttr->value();
    if (aShapeType == 6) { // if this is an edge
      if (isConstruction) {
        thePrs->setWidth(1);
        thePrs->setLineStyle(3);
        aRGB = Config_PropManager::color("Visualization", "sketch_construction_color",
                                         SKETCH_CONSTRUCTION_COLOR);
      }
      else {
        thePrs->setWidth(3);
        thePrs->setLineStyle(0);
        if (isExternal()) {
          // Set color from preferences
          aRGB = Config_PropManager::color("Visualization", "sketch_external_color",
                                           SKETCH_EXTERNAL_EDGE_COLOR);
        }
        else {
          // Set color from preferences
          aRGB = Config_PropManager::color("Visualization", "sketch_edge_color",
                                           SKETCH_EDGE_COLOR);
        }
      }
    }
    else if (aShapeType == 7) { // otherwise this is a vertex
      //  thePrs->setPointMarker(6, 2.);
      // Set color from preferences
      aRGB = Config_PropManager::color("Visualization", "sketch_point_color",
                                       SKETCH_POINT_COLOR);
    }

    if (!aRGB.empty())
      thePrs->setColor(aRGB[0], aRGB[1], aRGB[2]);
  }

protected:
  /// initializes mySketch
  SketchPlugin_SketchEntity();
};

#endif
