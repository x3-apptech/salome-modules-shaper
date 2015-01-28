// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ConstructionPlugin_Plane.h
// Created:     12 Dec 2014
// Author:      Vitaly Smetannikov

#ifndef ConstructionPlugin_Plane_H
#define ConstructionPlugin_Plane_H

#include "ConstructionPlugin.h"
#include <ModelAPI_Feature.h>
#include <GeomAPI_ICustomPrs.h>

/// Point kind
const std::string CONSTRUCTION_PLANE_KIND("Plane");

/// attribute name for base face
const std::string FACE_ATTR = "planeFace";

/// attribute name for distance
const std::string DISTANCE_ATTR = "distance";

/**\class ConstructionPlugin_Plane
 * \ingroup Plugins
 * \brief Feature for creation of the new planar surface in PartSet.
 */
class ConstructionPlugin_Plane : public ModelAPI_Feature, public GeomAPI_ICustomPrs
{
 public:
  /// Returns the kind of a feature
  CONSTRUCTIONPLUGIN_EXPORT virtual const std::string& getKind()
  {
    static std::string MY_KIND = CONSTRUCTION_PLANE_KIND;
    return MY_KIND;
  }

  /// Creates a new part document if needed
  CONSTRUCTIONPLUGIN_EXPORT virtual void execute();

  /// Request for initialization of data model of the feature: adding all attributes
  CONSTRUCTIONPLUGIN_EXPORT virtual void initAttributes();

  /// Construction result is allways recomuted on the fly
  CONSTRUCTIONPLUGIN_EXPORT virtual bool isPersistentResult() {return false;}

  /// Use plugin manager for features creation
  ConstructionPlugin_Plane();

  /// Customize presentation of the feature
  virtual void customisePresentation(AISObjectPtr thePrs);
};

#endif