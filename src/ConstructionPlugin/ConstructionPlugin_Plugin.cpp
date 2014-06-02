#include "ConstructionPlugin_Plugin.h"
#include "ConstructionPlugin_Point.h"
#include "ConstructionPlugin_Extrusion.h"

#include <ModelAPI_PluginManager.h>
#include <ModelAPI_Document.h>

using namespace std;

// the only created instance of this plugin
static ConstructionPlugin_Plugin* MY_INSTANCE = new ConstructionPlugin_Plugin();

ConstructionPlugin_Plugin::ConstructionPlugin_Plugin() 
{
  // register this plugin
  ModelAPI_PluginManager::get()->registerPlugin(this);
}

boost::shared_ptr<ModelAPI_Feature> ConstructionPlugin_Plugin::createFeature(string theFeatureID)
{
  if (theFeatureID == "Point") {
    return boost::shared_ptr<ModelAPI_Feature>(new ConstructionPlugin_Point);
  } else if (theFeatureID == "Extrusion") {
    return boost::shared_ptr<ModelAPI_Feature>(new ConstructionPlugin_Extrusion);
  }
  // feature of such kind is not found
  return boost::shared_ptr<ModelAPI_Feature>();
}
