"""
      TestExtrusion.py
      Unit test of FeaturesPlugin_Extrusion class
      
      class FeaturesPlugin_Extrusion : public ModelAPI_Feature
        static const std::string MY_EXTRUSION_ID("Extrusion");
        static const std::string MY_FACE_ID("base");
        static const std::string MY_SIZE_ID("size");
        static const std::string MY_REVERSE_ID("reverse");
          
        data()->addAttribute(FeaturesPlugin_Extrusion::FACE_ID(), ModelAPI_AttributeSelection::typeId());
        data()->addAttribute(FeaturesPlugin_Extrusion::SIZE_ID(), ModelAPI_AttributeDouble::typeId());
        data()->addAttribute(FeaturesPlugin_Extrusion::REVERSE_ID(), ModelAPI_AttributeBoolean::typeId());
"""
#=========================================================================
# Initialization of the test
#=========================================================================
from ModelAPI import *
from GeomDataAPI import *
from GeomAlgoAPI import *
from GeomAPI import *

__updated__ = "2014-12-16"

aSession = ModelAPI_Session.get()
aDocument = aSession.moduleDocument()
# Create a part for extrusion
aSession.startOperation()
aPartFeature = aDocument.addFeature("Part")
aSession.finishOperation()
assert (len(aPartFeature.results()) == 1)
# Another way is:
# aPart = aSession.activeDocument()
aPartResult = modelAPI_ResultPart(aPartFeature.firstResult())
aPart = aPartResult.partDoc()
#=========================================================================
# Create a sketch circle to extrude
#=========================================================================
aSession.startOperation()
aSketchFeature = featureToCompositeFeature(aPart.addFeature("Sketch"))
origin = geomDataAPI_Point(aSketchFeature.attribute("Origin"))
origin.setValue(0, 0, 0)
dirx = geomDataAPI_Dir(aSketchFeature.attribute("DirX"))
dirx.setValue(1, 0, 0)
norm = geomDataAPI_Dir(aSketchFeature.attribute("Norm"))
norm.setValue(0, 0, 1)
# Create circle
aSketchCircle = aSketchFeature.addFeature("SketchCircle")
anCircleCentr = geomDataAPI_Point2D(aSketchCircle.attribute("CircleCenter"))
aCircleRadius = aSketchCircle.real("CircleRadius")
anCircleCentr.setValue(50., 50)
aCircleRadius.setValue(20.)
aSession.finishOperation()
#=========================================================================
# Make extrusion on circle
#=========================================================================
# Build shape from sketcher results
aSketchResult = aSketchFeature.firstResult()
aSketchEdges = modelAPI_ResultConstruction(aSketchResult).shape()
origin = geomDataAPI_Point(aSketchFeature.attribute("Origin")).pnt()
dirX = geomDataAPI_Dir(aSketchFeature.attribute("DirX")).dir()
norm = geomDataAPI_Dir(aSketchFeature.attribute("Norm")).dir()
aSketchFaces = ShapeList()
GeomAlgoAPI_SketchBuilder.createFaces(
    origin, dirX, norm, aSketchEdges, aSketchFaces)
assert (len(aSketchFaces) > 0)
assert (aSketchFaces[0] is not None)
# Create extrusion
aSession.startOperation()
anExtrusionFt = aPart.addFeature("Extrusion")
assert (anExtrusionFt.getKind() == "Extrusion")
# selection type FACE=4
anExtrusionFt.selectionList("base").append(
    aSketchResult, aSketchFaces[0])
anExtrusionFt.real("size").setValue(50)
anExtrusionFt.boolean("reverse").setValue(False)
anExtrusionFt.execute()
aSession.finishOperation()
assert (anExtrusionFt.real("size").value() == 50.0)
assert (anExtrusionFt.boolean("reverse").value() == False)

# Check extrusion results
assert (len(anExtrusionFt.results()) > 0)
anExtrusionResult = modelAPI_ResultBody(anExtrusionFt.firstResult())
assert (anExtrusionResult is not None)
#=========================================================================
# End of test
#=========================================================================
