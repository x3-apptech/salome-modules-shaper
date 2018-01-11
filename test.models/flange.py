## Copyright (C) 2014-2017  CEA/DEN, EDF R&D
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##
## See http:##www.salome-platform.org/ or
## email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
##

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Parameter_dint = model.addParameter(Part_1_doc, "dint", "58")
Parameter_thick = model.addParameter(Part_1_doc, "thick", "16")
Parameter_length = model.addParameter(Part_1_doc, "length", "172")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))
SketchLine_1 = Sketch_1.addLine(model.selection("EDGE", "PartSet/OZ"))
SketchArc_1 = Sketch_1.addArc(-1.656176621148808e-020, 61, -36.69142117459658, 34.94736842105263, -1.927105092898643e-024, 106, True)
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.result(), SketchArc_1.center())
SketchLine_2 = Sketch_1.addLine(model.selection("EDGE", "PartSet/OX"))
SketchLine_3 = Sketch_1.addLine(-46.47580015448901, 16.00000000000001, -86, 16)
SketchLine_4 = Sketch_1.addLine(-86, 16, -86, -1.232595164407831e-032)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(-86, -1.232595164407831e-032, -46.47580015448901, 7.497132070176856e-024)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchArc_2 = Sketch_1.addArc(5.604803279647422e-025, 61, -23.64558253474002, 44.21052631578948, -7.759334428649666e-022, 90, True)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchArc_1.center(), SketchArc_2.center())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_3.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_5.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_4.result())
SketchArc_3 = Sketch_1.addArc(-46.47580015448901, 28.00000000000001, -36.69142117459658, 34.94736842105264, -46.47580015448901, 16.00000000000001, True)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchArc_1.startPoint(), SketchArc_3.startPoint())
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_3.startPoint(), SketchArc_3.endPoint())
SketchArc_4 = Sketch_1.addArc(-46.475800154489, 28, -23.64558253474002, 44.21052631578947, -46.475800154489, -2.594279994652913e-031, True)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchArc_2.startPoint(), SketchArc_4.startPoint())
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchArc_4.endPoint(), SketchLine_5.endPoint())
SketchConstraintRadius_1 = Sketch_1.setRadius(SketchArc_3.results()[1], 12)
SketchConstraintTangent_1 = Sketch_1.setTangent(SketchArc_1.results()[1], SketchArc_3.results()[1])
SketchConstraintTangent_2 = Sketch_1.setTangent(SketchArc_3.results()[1], SketchLine_3.result())
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchArc_4.center(), SketchArc_3.center())
SketchConstraintTangent_3 = Sketch_1.setTangent(SketchArc_4.results()[1], SketchLine_5.result())
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchArc_2.center(), SketchLine_5.result(), 61)
SketchConstraintTangent_4 = Sketch_1.setTangent(SketchArc_4.results()[1], SketchArc_2.results()[1])
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_5.result())
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_4.result(), "thick")
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchArc_1.endPoint(), SketchLine_1.result())
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchArc_2.endPoint(), SketchLine_1.result())
SketchConstraintDistance_2 = Sketch_1.setDistance(SketchLine_1.startPoint(), SketchLine_4.result(), "length/2")
SketchConstraintMirror_1_objects = [SketchArc_2.results()[1], SketchArc_4.results()[1], SketchLine_5.result(), SketchLine_4.result(), SketchLine_3.result(), SketchArc_3.results()[1], SketchArc_1.results()[1]]
SketchConstraintMirror_1 = Sketch_1.addMirror(SketchLine_1.result(), SketchConstraintMirror_1_objects)
[SketchArc_5, SketchArc_6, SketchLine_6, SketchLine_7, SketchLine_8, SketchArc_7, SketchArc_8] = SketchConstraintMirror_1.mirrored()
SketchConstraintRadius_2 = Sketch_1.setRadius(SketchArc_2.results()[1], "dint/2")
model.do()
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchArc_1_2f-SketchLine_3f-SketchLine_4f-SketchLine_5f-SketchArc_2_2r-SketchArc_3_2r-SketchArc_4_2f-SketchArc_5_2r-SketchArc_6_2f-SketchLine_6r-SketchLine_7r-SketchLine_8r-SketchArc_7_2r-SketchArc_8_2f")], model.selection(), 35, 35)
Sketch_2 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))
SketchArc_9 = Sketch_2.addArc(0, 106, 0, 78, 0, 134, True)
SketchConstraintRadius_3 = Sketch_2.setRadius(SketchArc_9.results()[1], 28)
SketchLine_9 = Sketch_2.addLine(model.selection("EDGE", "PartSet/OX"))
SketchConstraintDistance_3 = Sketch_2.setDistance(SketchArc_9.center(), SketchLine_9.result(), "61+dint/2+thick")
SketchLine_10 = Sketch_2.addLine(model.selection("EDGE", "PartSet/OZ"))
SketchConstraintCoincidence_13 = Sketch_2.setCoincident(SketchArc_9.startPoint(), SketchLine_10.result())
SketchConstraintCoincidence_14 = Sketch_2.setCoincident(SketchArc_9.endPoint(), SketchLine_10.result())
SketchConstraintCoincidence_15 = Sketch_2.setCoincident(SketchArc_9.center(), SketchLine_10.result())
SketchLine_11 = Sketch_2.addLine(0, 78, 0, 134)
SketchConstraintCoincidence_16 = Sketch_2.setCoincident(SketchArc_9.startPoint(), SketchLine_11.startPoint())
SketchConstraintCoincidence_17 = Sketch_2.setCoincident(SketchArc_9.endPoint(), SketchLine_11.endPoint())
model.do()
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("FACE", "Sketch_2/Face-SketchArc_9_2f-SketchLine_11f")], model.selection("EDGE", "PartSet/OZ"), 360, 0)
Boolean_1 = model.addCut(Part_1_doc, [model.selection("SOLID", "Extrusion_1_1")], [model.selection("SOLID", "Revolution_1_1")])
Sketch_3 = model.addSketch(Part_1_doc, model.selection("FACE", "PartSet/YOZ"))
SketchLine_12 = Sketch_3.addLine(0, 106, -4, 106)
SketchLine_13 = Sketch_3.addLine(-4, 106, -4, 67.34428877022477)
SketchConstraintCoincidence_18 = Sketch_3.setCoincident(SketchLine_12.endPoint(), SketchLine_13.startPoint())
SketchConstraintHorizontal_3 = Sketch_3.setHorizontal(SketchLine_12.result())
SketchConstraintVertical_2 = Sketch_3.setVertical(SketchLine_13.result())
SketchPoint_1 = Sketch_3.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchConstraintDistance_4 = Sketch_3.setDistance(SketchPoint_1.coordinates(), SketchLine_12.result(), "61+90/2")
SketchLine_14 = Sketch_3.addLine(model.selection("EDGE", "PartSet/OZ"))
SketchLine_14.setName("SketchLine_15")
SketchLine_14.result().setName("SketchLine_15")
SketchConstraintCoincidence_19 = Sketch_3.setCoincident(SketchLine_12.startPoint(), SketchLine_14.result())
SketchConstraintLength_2 = Sketch_3.setLength(SketchLine_12.result(), "8/2")
SketchArc_10 = Sketch_3.addArc(0, 61, -4, 67.34428877022477, 4, 67.34428877022477, False)
SketchConstraintCoincidence_20 = Sketch_3.setCoincident(SketchLine_14.result(), SketchArc_10.center())
SketchConstraintRadius_4 = Sketch_3.setRadius(SketchArc_10.results()[1], "15/2")
SketchLine_15 = Sketch_3.addLine(model.selection("EDGE", "PartSet/OY"))
SketchLine_15.setName("SketchLine_16")
SketchLine_15.result().setName("SketchLine_16")
SketchConstraintDistance_5 = Sketch_3.setDistance(SketchArc_10.center(), SketchLine_15.result(), 61)
SketchConstraintCoincidence_21 = Sketch_3.setCoincident(SketchLine_13.endPoint(), SketchArc_10.startPoint())
SketchConstraintMirror_2 = Sketch_3.addMirror(SketchLine_14.result(), [SketchLine_13.result(), SketchLine_12.result()])
[SketchLine_16, SketchLine_17] = SketchConstraintMirror_2.mirrored()
SketchLine_17.setName("SketchLine_18")
SketchLine_17.result().setName("SketchLine_18")
SketchLine_16.setName("SketchLine_17")
SketchLine_16.result().setName("SketchLine_17")
SketchConstraintCoincidence_22 = Sketch_3.setCoincident(SketchArc_10.endPoint(), SketchLine_16.result())
model.do()
Extrusion_2 = model.addExtrusion(Part_1_doc, [model.selection("WIRE", "Sketch_3/Wire-SketchLine_12f-SketchLine_13f-SketchLine_17r-SketchLine_18r-SketchArc_10_2f")], model.selection(), "172/2", "172/2")
Boolean_2 = model.addCut(Part_1_doc, [model.selection("SOLID", "Boolean_1_1")], [model.selection("SOLID", "Extrusion_2_1")])
Sketch_4 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_1_1/Generated_Face_10"))
SketchLine_18 = Sketch_4.addLine(-65, 16, -65, -16)
SketchLine_18.setName("SketchLine_19")
SketchLine_18.result().setName("SketchLine_19")
SketchLine_18.setAuxiliary(True)
SketchConstraintVertical_3 = Sketch_4.setVertical(SketchLine_18.result())
SketchConstraintLength_3 = Sketch_4.setLength(SketchLine_18.result(), 32)
SketchPoint_2 = Sketch_4.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchConstraintDistance_6 = Sketch_4.setDistance(SketchPoint_2.coordinates(), SketchLine_18.result(), "130/2")
SketchPoint_3 = Sketch_4.addPoint(-65, 0)
SketchPoint_3.setAuxiliary(True)
SketchConstraintMiddle_1 = Sketch_4.setMiddlePoint(SketchPoint_3.coordinates(), SketchLine_18.result())
SketchLine_19 = Sketch_4.addLine(model.selection("EDGE", "PartSet/OX"))
SketchLine_19.setName("SketchLine_20")
SketchLine_19.result().setName("SketchLine_20")
SketchConstraintCoincidence_23 = Sketch_4.setCoincident(SketchPoint_3.coordinates(), SketchLine_19.result())
SketchArc_11 = Sketch_4.addArc(-65, 16, -59, 16, -71, 16, False)
SketchConstraintCoincidence_24 = Sketch_4.setCoincident(SketchLine_18.startPoint(), SketchArc_11.center())
SketchArc_12 = Sketch_4.addArc(-65, -16, -59, -16.00003459934639, -71, -16, True)
SketchConstraintCoincidence_25 = Sketch_4.setCoincident(SketchLine_18.endPoint(), SketchArc_12.center())
SketchConstraintRadius_5 = Sketch_4.setRadius(SketchArc_11.results()[1], 6)
SketchLine_20 = Sketch_4.addLine(-71, 16, -71, -16)
SketchLine_20.setName("SketchLine_21")
SketchLine_20.result().setName("SketchLine_21")
SketchConstraintCoincidence_26 = Sketch_4.setCoincident(SketchArc_11.endPoint(), SketchLine_20.startPoint())
SketchConstraintCoincidence_27 = Sketch_4.setCoincident(SketchArc_12.endPoint(), SketchLine_20.endPoint())
SketchLine_21 = Sketch_4.addLine(-59, 16, -59, -16.00003459934639)
SketchLine_21.setName("SketchLine_22")
SketchLine_21.result().setName("SketchLine_22")
SketchConstraintCoincidence_28 = Sketch_4.setCoincident(SketchArc_11.startPoint(), SketchLine_21.startPoint())
SketchConstraintCoincidence_29 = Sketch_4.setCoincident(SketchArc_12.startPoint(), SketchLine_21.endPoint())
SketchConstraintVertical_4 = Sketch_4.setVertical(SketchLine_20.result())
SketchConstraintVertical_5 = Sketch_4.setVertical(SketchLine_21.result())
SketchConstraintTangent_5 = Sketch_4.setTangent(SketchLine_20.result(), SketchArc_11.results()[1])
SketchConstraintTangent_6 = Sketch_4.setTangent(SketchLine_21.result(), SketchArc_11.results()[1])
SketchConstraintTangent_7 = Sketch_4.setTangent(SketchArc_12.results()[1], SketchLine_20.result())
model.do()
Extrusion_3 = model.addExtrusion(Part_1_doc, [model.selection("WIRE", "Sketch_4/Wire-SketchArc_11_2f-SketchArc_12_2f-SketchLine_21f-SketchLine_22r")], model.selection(), model.selection(), 0, model.selection("FACE", "Extrusion_1_1/Generated_Face_12"), 0)
MultiTranslation_1 = model.addMultiTranslation(Part_1_doc, [model.selection("SOLID", "Extrusion_3_1")], model.selection("EDGE", "Extrusion_1_1/To_Face_1&Extrusion_1_1/Generated_Face_12"), -130, 2)
Boolean_3 = model.addCut(Part_1_doc, [model.selection("SOLID", "Boolean_2_1")], [model.selection("COMPOUND", "LinearCopy_1_1")])
model.do()

# Test reexecution after parameter change
Parameter_dint.setValue(62)
Parameter_thick.setValue(20)
Parameter_length.setValue(200)
model.do()
model.testResultsVolumes(Boolean_3, [409550.515091864974237978458404541])
Parameter_dint.setValue(58)
Parameter_thick.setValue(16)
Parameter_length.setValue(172)

model.end()

from GeomAPI import GeomAPI_Shape

model.testNbResults(Boolean_3, 1)
model.testNbSubResults(Boolean_3, [0])
model.testNbSubShapes(Boolean_3, GeomAPI_Shape.SOLID, [1])
model.testNbSubShapes(Boolean_3, GeomAPI_Shape.FACE, [32])
model.testNbSubShapes(Boolean_3, GeomAPI_Shape.EDGE, [192])
model.testNbSubShapes(Boolean_3, GeomAPI_Shape.VERTEX, [384])
model.testResultsVolumes(Boolean_3, [307084.534467286430299282073974609])

assert(model.checkPythonDump())
