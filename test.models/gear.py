from SketchAPI import *

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Sketch_1 = model.addSketch(partSet, model.defaultPlane("XOY"))
SketchLine_1 = Sketch_1.addLine(0, 0, 30, 0)
SketchPoint_1 = Sketch_1.addPoint(model.selection("VERTEX", "Origin"))
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchPoint_1.result())
SketchLine_2 = Sketch_1.addLine(model.selection("EDGE", "OX"))
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.result())
SketchLine_3 = Sketch_1.addLine(30, 0, 30, 37.5)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(30, 37.5, 17, 37.5)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(17, 37.5, 17, 15.5)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchLine_6 = Sketch_1.addLine(17, 15.5, 0, 15.5)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchLine_7 = Sketch_1.addLine(model.selection("EDGE", "OY"))
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.result())
SketchLine_8 = Sketch_1.addLine(0, 15.5, 0, 0)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_8.startPoint())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_4.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_3.result())
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_5.result())
SketchConstraintVertical_3 = Sketch_1.setVertical(SketchLine_8.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_6.result())
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchLine_1.endPoint(), SketchLine_8.result(), 30)
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_3.result(), 37.5)
SketchConstraintLength_2 = Sketch_1.setLength(SketchLine_8.result(), 15.5)
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchPoint_1.coordinates())
SketchConstraintLength_3 = Sketch_1.setLength(SketchLine_4.result(), 13)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_8.endPoint(), SketchLine_1.startPoint())
model.do()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Sketch_2 = model.addSketch(Part_1_doc, model.defaultPlane("YOZ"))
SketchPoint_2 = Sketch_2.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchLine_9 = Sketch_2.addLine(0, 0, 0, 45.59203984275747)
SketchLine_9.setAuxiliary(True)
SketchConstraintCoincidence_11 = Sketch_2.setCoincident(SketchPoint_2.coordinates(), SketchLine_9.startPoint())
SketchConstraintVertical_4 = Sketch_2.setVertical(SketchLine_9.result())
SketchLine_10 = Sketch_2.addLine(-2.5, 37.41657386773942, -0.9611800001338426, 34.11656372860793)
SketchArc_1 = Sketch_2.addArc(0, 34.39248583236681, -0.9611800001338426, 34.11656372860793, 0, 33.39248583236681, False)
SketchConstraintCoincidence_12 = Sketch_2.setCoincident(SketchLine_10.endPoint(), SketchArc_1.startPoint())
SketchConstraintCoincidence_13 = Sketch_2.setCoincident(SketchArc_1.center(), SketchLine_9.result())
SketchConstraintAngle_1 = Sketch_2.setAngleBackward(SketchLine_10.result(), SketchLine_9.result(), 205)
SketchConstraintRadius_1 = Sketch_2.setRadius(SketchArc_1.results()[1], 1)
SketchConstraintDistance_2 = Sketch_2.setDistance(SketchLine_10.startPoint(), SketchLine_9.result(), 2.5)
SketchConstraintCoincidence_14 = Sketch_2.setCoincident(SketchArc_1.endPoint(), SketchLine_9.result())
SketchArc_2 = Sketch_2.addArc(0, 0, -2.5, 37.41657386773942, 0, 37.5, True)
SketchConstraintCoincidence_15 = Sketch_2.setCoincident(SketchArc_2.center(), SketchPoint_2.result())
SketchConstraintCoincidence_16 = Sketch_2.setCoincident(SketchLine_10.startPoint(), SketchArc_2.startPoint())
SketchConstraintCoincidence_17 = Sketch_2.setCoincident(SketchLine_9.result(), SketchArc_2.endPoint())
SketchConstraintMirror_1 = Sketch_2.addMirror(SketchLine_9.result(), [SketchLine_10.result(), SketchArc_1.results()[1], SketchArc_2.results()[1]])
[SketchLine_11, SketchArc_3, SketchArc_4] = SketchConstraintMirror_1.mirrored()
SketchMultiRotation_1_objects = [SketchArc_1.results()[1], SketchLine_10.result(), SketchArc_2.results()[1], SketchArc_3.results()[1], SketchLine_11.result(), SketchArc_4.results()[1]]
SketchMultiRotation_1 = Sketch_2.addRotation(SketchMultiRotation_1_objects, SketchArc_2.center(), 12, 30)
SketchConstraintRadius_2 = Sketch_2.setRadius(SketchArc_2.results()[1], 37.5)
model.do()
Sketch_3 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchLine_70 = Sketch_3.addLine(0, 0, 30, 0)
SketchLine_70.setAuxiliary(True)
SketchPoint_3 = Sketch_3.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchConstraintCoincidence_18 = Sketch_3.setCoincident(SketchLine_70.startPoint(), SketchPoint_3.result())
SketchLine_71 = Sketch_3.addLine(30, 0, 30, 37.5)
SketchLine_71.setAuxiliary(True)
SketchConstraintCoincidence_19 = Sketch_3.setCoincident(SketchLine_70.endPoint(), SketchLine_71.startPoint())
SketchLine_72 = Sketch_3.addLine(30, 37.5, 17, 37.5)
SketchLine_72.setAuxiliary(True)
SketchConstraintCoincidence_20 = Sketch_3.setCoincident(SketchLine_71.endPoint(), SketchLine_72.startPoint())
SketchConstraintHorizontal_3 = Sketch_3.setHorizontal(SketchLine_72.result())
SketchConstraintHorizontal_4 = Sketch_3.setHorizontal(SketchLine_70.result())
SketchConstraintVertical_5 = Sketch_3.setVertical(SketchLine_71.result())
SketchConstraintLength_4 = Sketch_3.setLength(SketchLine_70.result(), 30)
SketchConstraintLength_5 = Sketch_3.setLength(SketchLine_71.result(), 37.5)
SketchConstraintLength_6 = Sketch_3.setLength(SketchLine_72.result(), 13)
SketchLine_73 = Sketch_3.addLine(29, 37.49999999999999, 30, 36.49999999999999)
SketchConstraintCoincidence_21 = Sketch_3.setCoincident(SketchLine_73.startPoint(), SketchLine_72.result())
SketchConstraintCoincidence_22 = Sketch_3.setCoincident(SketchLine_73.endPoint(), SketchLine_71.result())
SketchLine_74 = Sketch_3.addLine(17, 37.5, 17, 36.5)
SketchLine_75 = Sketch_3.addLine(17, 36.5, 17.99999999999994, 37.5)
SketchConstraintCoincidence_23 = Sketch_3.setCoincident(SketchLine_74.endPoint(), SketchLine_75.startPoint())
SketchConstraintCoincidence_24 = Sketch_3.setCoincident(SketchLine_75.endPoint(), SketchLine_72.result())
SketchConstraintCoincidence_25 = Sketch_3.setCoincident(SketchLine_72.endPoint(), SketchLine_74.startPoint())
SketchConstraintVertical_6 = Sketch_3.setVertical(SketchLine_74.result())
SketchConstraintLength_7 = Sketch_3.setLength(SketchLine_74.result(), 1)
SketchConstraintDistance_3 = Sketch_3.setDistance(SketchLine_75.endPoint(), SketchLine_74.result(), 1)
SketchConstraintDistance_4 = Sketch_3.setDistance(SketchLine_73.startPoint(), SketchLine_71.result(), 1)
SketchConstraintDistance_5 = Sketch_3.setDistance(SketchLine_73.endPoint(), SketchLine_72.result(), 1)
SketchLine_76 = Sketch_3.addLine(17, 37.5, 17.99999999999994, 37.5)
SketchConstraintCoincidence_26 = Sketch_3.setCoincident(SketchLine_72.endPoint(), SketchLine_76.startPoint())
SketchConstraintCoincidence_27 = Sketch_3.setCoincident(SketchLine_74.startPoint(), SketchLine_76.startPoint())
SketchConstraintCoincidence_28 = Sketch_3.setCoincident(SketchLine_75.endPoint(), SketchLine_76.endPoint())
SketchLine_77 = Sketch_3.addLine(29, 37.49999999999999, 30, 37.5)
SketchConstraintCoincidence_29 = Sketch_3.setCoincident(SketchLine_73.startPoint(), SketchLine_77.startPoint())
SketchConstraintCoincidence_30 = Sketch_3.setCoincident(SketchLine_71.endPoint(), SketchLine_77.endPoint())
SketchConstraintCoincidence_31 = Sketch_3.setCoincident(SketchLine_72.startPoint(), SketchLine_77.endPoint())
SketchLine_78 = Sketch_3.addLine(30, 37.5, 30, 36.49999999999999)
SketchConstraintCoincidence_32 = Sketch_3.setCoincident(SketchLine_71.endPoint(), SketchLine_78.startPoint())
SketchConstraintCoincidence_33 = Sketch_3.setCoincident(SketchLine_72.startPoint(), SketchLine_78.startPoint())
SketchConstraintCoincidence_34 = Sketch_3.setCoincident(SketchLine_77.endPoint(), SketchLine_78.startPoint())
SketchConstraintCoincidence_35 = Sketch_3.setCoincident(SketchLine_73.endPoint(), SketchLine_78.endPoint())
SketchLine_79 = Sketch_3.addLine(0, 0, 0, 15)
SketchLine_79.setAuxiliary(True)
SketchConstraintCoincidence_36 = Sketch_3.setCoincident(SketchLine_79.startPoint(), SketchPoint_3.result())
SketchConstraintVertical_7 = Sketch_3.setVertical(SketchLine_79.result())
SketchConstraintLength_8 = Sketch_3.setLength(SketchLine_79.result(), 15)
SketchLine_80 = Sketch_3.addLine(0, 15, 0.5, 15)
SketchLine_80.setAuxiliary(True)
SketchConstraintCoincidence_37 = Sketch_3.setCoincident(SketchLine_79.endPoint(), SketchLine_80.startPoint())
SketchConstraintHorizontal_5 = Sketch_3.setHorizontal(SketchLine_80.result())
SketchArc_61 = Sketch_3.addArc(0.5, 15, 0, 15, 0.4999999999999999, 15.5, True)
SketchConstraintCoincidence_38 = Sketch_3.setCoincident(SketchLine_80.endPoint(), SketchArc_61.center())
SketchConstraintCoincidence_39 = Sketch_3.setCoincident(SketchLine_79.endPoint(), SketchArc_61.startPoint())
SketchConstraintCoincidence_40 = Sketch_3.setCoincident(SketchLine_80.startPoint(), SketchArc_61.startPoint())
SketchLine_81 = Sketch_3.addLine(0.5, 15, 0.5, 15.66051714754979)
SketchLine_81.setAuxiliary(True)
SketchConstraintCoincidence_41 = Sketch_3.setCoincident(SketchLine_80.endPoint(), SketchLine_81.startPoint())
SketchConstraintCoincidence_42 = Sketch_3.setCoincident(SketchArc_61.center(), SketchLine_81.startPoint())
SketchConstraintVertical_8 = Sketch_3.setVertical(SketchLine_81.result())
SketchConstraintCoincidence_43 = Sketch_3.setCoincident(SketchArc_61.endPoint(), SketchLine_81.result())
SketchConstraintRadius_3 = Sketch_3.setRadius(SketchArc_61.results()[1], 0.5)
SketchLine_82 = Sketch_3.addLine(0.4999999999999999, 15.5, 0.4988301186899768, 15.74914940994379)
SketchConstraintCoincidence_44 = Sketch_3.setCoincident(SketchArc_61.endPoint(), SketchLine_82.startPoint())
SketchLine_83 = Sketch_3.addLine(0.4988301186899768, 15.74914940994379, -0.3081878496471824, 15.75231418629021)
SketchConstraintCoincidence_45 = Sketch_3.setCoincident(SketchLine_82.endPoint(), SketchLine_83.startPoint())
SketchLine_84 = Sketch_3.addLine(-0.3081878496471824, 15.75231418629021, -0.3081878496471824, 14.99276786314936)
SketchConstraintCoincidence_46 = Sketch_3.setCoincident(SketchLine_83.endPoint(), SketchLine_84.startPoint())
SketchLine_85 = Sketch_3.addLine(-0.3081878496471824, 14.99276786314936, 0, 15)
SketchConstraintCoincidence_47 = Sketch_3.setCoincident(SketchLine_84.endPoint(), SketchLine_85.startPoint())
SketchConstraintCoincidence_48 = Sketch_3.setCoincident(SketchLine_79.endPoint(), SketchLine_85.endPoint())
SketchConstraintCoincidence_49 = Sketch_3.setCoincident(SketchLine_80.startPoint(), SketchLine_85.endPoint())
SketchConstraintCoincidence_50 = Sketch_3.setCoincident(SketchArc_61.startPoint(), SketchLine_85.endPoint())
model.do()
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("FACE", "PartSet/Sketch_1")], model.selection("EDGE", "PartSet/Sketch_1/Edge-SketchLine_1"), 360, 0)
Sketch_4 = model.addSketch(Part_1_doc, model.selection("FACE", "Revolution_1_1/Generated_Face_5"))
SketchPoint_4 = Sketch_4.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchLine_86 = Sketch_4.addLine(0, -13.49999999999519, 3.1, -13.49999999999519)
SketchLine_87 = Sketch_4.addLine(model.selection("EDGE", "PartSet/OZ"))
SketchConstraintCoincidence_51 = Sketch_4.setCoincident(SketchLine_86.startPoint(), SketchLine_87.result())
SketchLine_88 = Sketch_4.addLine(3.1, -13.49999999999519, 3.1, -10.55414610473955)
SketchConstraintCoincidence_52 = Sketch_4.setCoincident(SketchLine_86.endPoint(), SketchLine_88.startPoint())
SketchLine_89 = Sketch_4.addLine(0, -10.5290802744499, 0, 12.80434395246332)
SketchLine_89.setAuxiliary(True)
SketchConstraintCoincidence_53 = Sketch_4.setCoincident(SketchLine_89.startPoint(), SketchLine_87.result())
SketchConstraintVertical_9 = Sketch_4.setVertical(SketchLine_89.result())
SketchConstraintHorizontal_6 = Sketch_4.setHorizontal(SketchLine_86.result())
SketchConstraintVertical_10 = Sketch_4.setVertical(SketchLine_88.result())
SketchConstraintLength_9 = Sketch_4.setLength(SketchLine_86.result(), 3.1)
SketchConstraintMirror_2 = Sketch_4.addMirror(SketchLine_87.result(), [SketchLine_86.result(), SketchLine_88.result()])
[SketchLine_90, SketchLine_91] = SketchConstraintMirror_2.mirrored()
SketchArc_62 = Sketch_4.addArc(0, 0, 3.1, -10.55414610473955, -3.100000000000001, -10.55414610473344, False)
SketchConstraintCoincidence_54 = Sketch_4.setCoincident(SketchPoint_4.coordinates(), SketchArc_62.center())
SketchConstraintCoincidence_55 = Sketch_4.setCoincident(SketchLine_87.startPoint(), SketchArc_62.center())
SketchConstraintCoincidence_56 = Sketch_4.setCoincident(SketchArc_62.startPoint(), SketchLine_88.result())
SketchConstraintCoincidence_57 = Sketch_4.setCoincident(SketchArc_62.endPoint(), SketchLine_91.result())
SketchConstraintRadius_4 = Sketch_4.setRadius(SketchArc_62.results()[1], 11)
SketchConstraintCoincidence_58 = Sketch_4.setCoincident(SketchLine_88.endPoint(), SketchArc_62.startPoint())
SketchPoint_5 = Sketch_4.addPoint(3.1, -10.55414610473955)
SketchConstraintCoincidence_59 = Sketch_4.setCoincident(SketchLine_88.endPoint(), SketchPoint_5.coordinates())
SketchConstraintCoincidence_60 = Sketch_4.setCoincident(SketchArc_62.startPoint(), SketchPoint_5.coordinates())
SketchPoint_6 = Sketch_4.addPoint(0, 11.00000000000481)
SketchPoint_6.setAuxiliary(True)
SketchConstraintCoincidence_61 = Sketch_4.setCoincident(SketchPoint_6.coordinates(), SketchArc_62.results()[1])
SketchConstraintCoincidence_62 = Sketch_4.setCoincident(SketchPoint_6.coordinates(), SketchLine_89.result())
SketchConstraintDistance_6 = Sketch_4.setDistance(SketchPoint_6.coordinates(), SketchLine_86.result(), 24.5)
model.do()
ExtrusionCut_1 = model.addExtrusionCut(Part_1_doc, [model.selection("COMPOUND", "Sketch_1")], model.selection(), model.selection("FACE", "Revolution_1_1/Generated_Face_1"), 0, model.selection(), 0, [model.selection("SOLID", "Revolution_1_1")])
RevolutionCut_1 = model.addRevolutionCut(Part_1_doc, [model.selection("COMPOUND", "Sketch_2")], model.selection("EDGE", "PartSet/OX"), 0, 360, [model.selection("SOLID", "ExtrusionCut_1_1")])
model.do()
model.end()

from GeomAPI import GeomAPI_Shape

model.testNbResults(RevolutionCut_1, 1)
model.testNbSubResults(RevolutionCut_1, [0])
model.testNbSubShapes(RevolutionCut_1, GeomAPI_Shape.SOLID, [1])
model.testNbSubShapes(RevolutionCut_1, GeomAPI_Shape.FACE, [218])
model.testNbSubShapes(RevolutionCut_1, GeomAPI_Shape.EDGE, [1284])
model.testNbSubShapes(RevolutionCut_1, GeomAPI_Shape.VERTEX, [2568])
model.testResultsVolumes(RevolutionCut_1, [65202.414255050614883657544851303])

assert(model.checkPythonDump())
