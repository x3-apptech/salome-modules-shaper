#include "ConstructionPlugin_Plugin.h"
#include "ConstructionPlugin_Point.h"

#include <ModelAPI_Session.h>
#include <ModelAPI_Document.h>

using namespace std;

// the only created instance of this plugin
static ConstructionPlugin_Plugin* MY_INSTANCE = new ConstructionPlugin_Plugin();

ConstructionPlugin_Plugin::ConstructionPlugin_Plugin()
{
  // register this plugin
  ModelAPI_Session::get()->registerPlugin(this);
}

FeaturePtr ConstructionPlugin_Plugin::createFeature(string theFeatureID)
{
  if (theFeatureID == CONSTRUCTION_POINT_KIND) {
    return FeaturePtr(new ConstructionPlugin_Point);
  }
  // feature of such kind is not found
  return FeaturePtr();
}
