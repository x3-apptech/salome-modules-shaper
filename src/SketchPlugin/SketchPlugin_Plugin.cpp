#include "SketchPlugin_Plugin.h"
#include "SketchPlugin_Sketch.h"
#include "SketchPlugin_Line.h"
#include "SketchPlugin_Point.h"
#include "SketchPlugin_Circle.h"
#include "SketchPlugin_Arc.h"
#include "SketchPlugin_ConstraintCoincidence.h"
#include "SketchPlugin_ConstraintDistance.h"
#include "SketchPlugin_ConstraintLength.h"
#include "SketchPlugin_ConstraintParallel.h"
#include "SketchPlugin_ConstraintPerpendicular.h"
#include "SketchPlugin_ConstraintRadius.h"
#include <ModelAPI_PluginManager.h>
#include <ModelAPI_Document.h>

using namespace std;

// the only created instance of this plugin
static SketchPlugin_Plugin* MY_INSTANCE = new SketchPlugin_Plugin();

SketchPlugin_Plugin::SketchPlugin_Plugin() 
{
  // register this plugin
  ModelAPI_PluginManager::get()->registerPlugin(this);
}

boost::shared_ptr<ModelAPI_Feature> SketchPlugin_Plugin::createFeature(string theFeatureID)
{
  if (theFeatureID == SKETCH_KIND) {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_Sketch);
  }
  else if (theFeatureID == "SketchPoint") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_Point);
  }
  else if (theFeatureID == SKETCH_LINE_KIND) {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_Line);
  }
  else if (theFeatureID == "SketchCircle") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_Circle);
  }
  else if (theFeatureID == "SketchConstraintCoincidence") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintCoincidence);
  }
  else if (theFeatureID == "SketchConstraintDistance") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintDistance);
  }
  else if (theFeatureID == "SketchConstraintLength") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintLength);
  }
  else if (theFeatureID == "SketchConstraintParallel") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintParallel);
  }
  else if (theFeatureID == "SketchConstraintPerpendicular") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintPerpendicular);
  }
  else if (theFeatureID == "SketchConstraintRadius") {
    return boost::shared_ptr<ModelAPI_Feature>(new SketchPlugin_ConstraintRadius);
  }
  // feature of such kind is not found
  return boost::shared_ptr<ModelAPI_Feature>();
}
