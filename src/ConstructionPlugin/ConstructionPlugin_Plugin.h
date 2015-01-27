// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ConstructionPlugin_Plugin.hxx
// Created:     31 Mar 2014
// Author:      Mikhail PONIKAROV

#ifndef ConstructionPlugin_Plugin_H_
#define ConstructionPlugin_Plugin_H_

#include "ConstructionPlugin.h"
#include "ModelAPI_Plugin.h"
#include "ModelAPI_Feature.h"

/**\class ConstructionPlugin_Plugin
 * \ingroup Plugins
 * \brief The main class for management the construction features as plugin.
 */
class CONSTRUCTIONPLUGIN_EXPORT ConstructionPlugin_Plugin : public ModelAPI_Plugin
{
public:
  /// Creates the feature object of this plugin by the feature string ID
  virtual FeaturePtr createFeature(std::string theFeatureID);

  /// Default constructor
  ConstructionPlugin_Plugin();
};

#endif
