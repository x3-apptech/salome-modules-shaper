#include "SketchPlugin_Feature.h"
#include "SketchPlugin_Sketch.h"
#include <ModelAPI_Document.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_Object.h>
#include <ModelAPI_AttributeRefList.h>

SketchPlugin_Feature::SketchPlugin_Feature()
{
  mySketch = 0;
}

SketchPlugin_Sketch* SketchPlugin_Feature::sketch()
{
  if (!mySketch) {
    // find sketch that references to this feature
    int aSketches = document()->size("Construction");
    for(int a = 0; a < aSketches && !mySketch; a++) {
      boost::shared_ptr<SketchPlugin_Sketch> aSketch = boost::
        dynamic_pointer_cast<SketchPlugin_Sketch>(document()->object("Construction", a));
      if (aSketch) {
        std::list<FeaturePtr > aList = 
          aSketch->data()->reflist(SKETCH_ATTR_FEATURES)->list();
        std::list<FeaturePtr >::iterator aSub = aList.begin();
        for(; aSub != aList.end(); aSub++) {
          if ((*aSub)->data()->isEqual(data())) {
            mySketch = aSketch.get();
            break;
          }
        }
      }
    }
  }
  return mySketch;
}

boost::shared_ptr<GeomAPI_AISObject> SketchPlugin_Feature::prepareAISShape(
                        boost::shared_ptr<GeomAPI_AISObject> thePrevious)
{
  document()->result();
  boost::shared_ptr<GeomAPI_Shape> aPreview = preview();
  boost::shared_ptr<GeomAPI_AISObject> aResult = thePrevious;
  if (!aResult)
    aResult = boost::shared_ptr<GeomAPI_AISObject>(new GeomAPI_AISObject());
  aResult->createShape(aPreview);
  return aResult;
}
