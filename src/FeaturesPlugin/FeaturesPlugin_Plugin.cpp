#include "FeaturesPlugin_Plugin.h"
#include "FeaturesPlugin_Extrusion.h"
#include "FeaturesPlugin_Boolean.h"

#include <ModelAPI_Session.h>
#include <ModelAPI_Document.h>

using namespace std;

// the only created instance of this plugin
static FeaturesPlugin_Plugin* MY_INSTANCE = new FeaturesPlugin_Plugin();

FeaturesPlugin_Plugin::FeaturesPlugin_Plugin()
{
  // register this plugin
  ModelAPI_Session::get()->registerPlugin(this);
}

FeaturePtr FeaturesPlugin_Plugin::createFeature(string theFeatureID)
{
  if (theFeatureID == FeaturesPlugin_Extrusion::ID()) {
    return FeaturePtr(new FeaturesPlugin_Extrusion);
  } else
  if (theFeatureID == FeaturesPlugin_Boolean::ID()) {
    return FeaturePtr(new FeaturesPlugin_Boolean);
  }
  // feature of such kind is not found
  return FeaturePtr();
}
