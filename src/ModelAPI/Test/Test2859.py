# Copyright (C) 2014-2021  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

from SketchAPI import *
from ModelAPI import *
from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
model.addParameter(Part_1_doc, "h1", "15")
param_h2 = model.addParameter(Part_1_doc, "h2", "70")
model.addParameter(Part_1_doc, "h3", "7")
model.addParameter(Part_1_doc, "d", "15")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchProjection_1 = Sketch_1.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_1 = SketchProjection_1.createdFeature()
SketchLine_1 = Sketch_1.addLine(-7.500000000000083, -15.81138830084192, -7.500000000000083, -26.99999999999995)
SketchLine_1.setName("SketchLine_2")
SketchLine_1.result().setName("SketchLine_2")
SketchLine_2 = Sketch_1.addLine(-4.499999999999853, -30, 4.499999999999912, -30)
SketchLine_2.setName("SketchLine_3")
SketchLine_2.result().setName("SketchLine_3")
SketchLine_3 = Sketch_1.addLine(7.499999999999912, -27, 7.499999999999912, -15.81138830084196)
SketchLine_3.setName("SketchLine_4")
SketchLine_3.result().setName("SketchLine_4")
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_1.result())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_2.result())
SketchConstraintHorizontal_1.setName("SketchConstraintHorizontal_2")
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_3.result())
SketchPoint_2 = Sketch_1.addPoint(0, -30)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_2.result())
SketchConstraintCoincidence_2.setName("SketchConstraintCoincidence_6")
SketchProjection_2 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OY"), False)
SketchLine_4 = SketchProjection_2.createdFeature()
SketchLine_4.setName("SketchLine_5")
SketchLine_4.result().setName("SketchLine_5")
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchLine_4.result())
SketchConstraintCoincidence_3.setName("SketchConstraintCoincidence_7")
SketchProjection_3 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_5 = SketchProjection_3.createdFeature()
SketchLine_5.setName("SketchLine_6")
SketchLine_5.result().setName("SketchLine_6")
SketchArc_1 = Sketch_1.addArc(0, 0, -7.500000000000083, -15.81138830084192, -17.44306393762917, 1.410503622037771, True)
SketchArc_1.setName("SketchArc_2")
SketchArc_1.result().setName("SketchArc_2")
SketchArc_1.results()[1].setName("SketchArc_2_2")
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchPoint_1.result(), SketchArc_1.center())
SketchConstraintRadius_1 = Sketch_1.setRadius(SketchArc_1.results()[1], 17.5)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchArc_1.startPoint(), SketchLine_1.startPoint())
SketchConstraintCoincidence_5.setName("SketchConstraintCoincidence_14")
SketchArc_2 = Sketch_1.addArc(-4.499999999999853, -27, -7.500000000000083, -26.99999999999995, -4.499999999999853, -30, False)
SketchArc_2.setName("SketchArc_3")
SketchArc_2.result().setName("SketchArc_3")
SketchArc_2.results()[1].setName("SketchArc_3_2")
SketchPoint_3 = Sketch_1.addPoint(-7.500000000000085, -30)
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchPoint_3.coordinates(), SketchLine_5.result(), 30, True)
SketchPoint_3.setAuxiliary(True)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchLine_2.result())
SketchConstraintCoincidence_6.setName("SketchConstraintCoincidence_8")
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchLine_1.result())
SketchConstraintCoincidence_7.setName("SketchConstraintCoincidence_9")
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchArc_2.startPoint(), SketchLine_1.endPoint())
SketchConstraintCoincidence_8.setName("SketchConstraintCoincidence_10")
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchArc_2.endPoint(), SketchLine_2.startPoint())
SketchConstraintCoincidence_9.setName("SketchConstraintCoincidence_11")
SketchConstraintTangent_1 = Sketch_1.setTangent(SketchArc_2.results()[1], SketchLine_2.result())
SketchConstraintTangent_2 = Sketch_1.setTangent(SketchArc_2.results()[1], SketchLine_1.result())
SketchArc_3 = Sketch_1.addArc(4.499999999999912, -27, 4.499999999999912, -30, 7.499999999999912, -27, False)
SketchArc_3.setName("SketchArc_4")
SketchArc_3.result().setName("SketchArc_4")
SketchArc_3.results()[1].setName("SketchArc_4_2")
SketchPoint_4 = Sketch_1.addPoint(7.499999999999914, -30)
SketchConstraintDistance_2 = Sketch_1.setDistance(SketchPoint_3.coordinates(), SketchPoint_4.coordinates(), 15, False)
SketchPoint_4.setAuxiliary(True)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_3.result())
SketchConstraintCoincidence_10.setName("SketchConstraintCoincidence_13")
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchLine_2.result())
SketchConstraintCoincidence_11.setName("SketchConstraintCoincidence_15")
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchArc_3.startPoint(), SketchLine_2.endPoint())
SketchConstraintCoincidence_12.setName("SketchConstraintCoincidence_16")
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchArc_3.endPoint(), SketchLine_3.startPoint())
SketchConstraintCoincidence_13.setName("SketchConstraintCoincidence_17")
SketchConstraintTangent_3 = Sketch_1.setTangent(SketchArc_3.results()[1], SketchLine_3.result())
SketchConstraintTangent_4 = Sketch_1.setTangent(SketchArc_3.results()[1], SketchLine_2.result())
SketchConstraintRadius_2 = Sketch_1.setRadius(SketchArc_2.results()[1], 3)
SketchConstraintEqual_1 = Sketch_1.setEqual(SketchArc_3.results()[1], SketchArc_2.results()[1])
SketchConstraintEqual_2 = Sketch_1.setEqual(SketchLine_3.result(), SketchLine_1.result())
SketchMultiRotation_1_objects = [SketchLine_1.result(), SketchArc_2.results()[1], SketchLine_2.result(), SketchArc_3.results()[1], SketchLine_3.result()]
SketchMultiRotation_1 = Sketch_1.addRotation(SketchMultiRotation_1_objects, SketchAPI_Line(SketchLine_5).startPoint(), 360, 3, True)
[SketchLine_6, SketchLine_7, SketchArc_4, SketchArc_5, SketchLine_8, SketchLine_9, SketchArc_6, SketchArc_7, SketchLine_10, SketchLine_11] = SketchMultiRotation_1.rotated()
SketchLine_11.setName("SketchLine_14")
SketchLine_11.result().setName("SketchLine_14")
SketchLine_10.setName("SketchLine_13")
SketchLine_10.result().setName("SketchLine_13")
SketchArc_7.setName("SketchArc_9")
SketchArc_7.result().setName("SketchArc_9")
SketchArc_7.results()[1].setName("SketchArc_9_2")
SketchArc_6.setName("SketchArc_8")
SketchArc_6.result().setName("SketchArc_8")
SketchArc_6.results()[1].setName("SketchArc_8_2")
SketchLine_9.setName("SketchLine_11")
SketchLine_9.result().setName("SketchLine_11")
SketchLine_8.setName("SketchLine_10")
SketchLine_8.result().setName("SketchLine_10")
SketchArc_5.setName("SketchArc_6")
SketchArc_5.result().setName("SketchArc_6")
SketchArc_5.results()[1].setName("SketchArc_6_2")
SketchArc_4.setName("SketchArc_5")
SketchArc_4.result().setName("SketchArc_5")
SketchArc_4.results()[1].setName("SketchArc_5_2")
SketchLine_7.setName("SketchLine_8")
SketchLine_7.result().setName("SketchLine_8")
SketchLine_6.setName("SketchLine_7")
SketchLine_6.result().setName("SketchLine_7")
SketchArc_8 = Sketch_1.addArc(0, 0, -9.943063937629109, 14.40088467880429, 9.94306393762926, 14.40088467880419, True)
SketchArc_8.setName("SketchArc_10")
SketchArc_8.result().setName("SketchArc_10")
SketchArc_8.results()[1].setName("SketchArc_10_2")
SketchConstraintEqual_3 = Sketch_1.setEqual(SketchArc_1.results()[1], SketchArc_8.results()[1])
SketchConstraintCoincidence_14 = Sketch_1.setCoincident(SketchArc_1.center(), SketchArc_8.center())
SketchConstraintCoincidence_14.setName("SketchConstraintCoincidence_18")
SketchConstraintCoincidence_15 = Sketch_1.setCoincident(SketchArc_1.endPoint(), SketchLine_11.result())
SketchConstraintCoincidence_15.setName("SketchConstraintCoincidence_19")
SketchConstraintCoincidence_16 = Sketch_1.setCoincident(SketchArc_8.startPoint(), SketchLine_7.result())
SketchConstraintCoincidence_16.setName("SketchConstraintCoincidence_20")
SketchArc_9 = Sketch_1.addArc(0, 0, 17.44306393762918, 1.410503622037589, 7.499999999999912, -15.81138830084196, True)
SketchArc_9.setName("SketchArc_11")
SketchArc_9.result().setName("SketchArc_11")
SketchArc_9.results()[1].setName("SketchArc_11_2")
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchArc_9.endPoint(), SketchLine_3.endPoint())
SketchConstraintCoincidence_4.setName("SketchConstraintCoincidence_12")
SketchConstraintEqual_4 = Sketch_1.setEqual(SketchArc_8.results()[1], SketchArc_9.results()[1])
SketchConstraintCoincidence_17 = Sketch_1.setCoincident(SketchArc_8.center(), SketchArc_9.center())
SketchConstraintCoincidence_17.setName("SketchConstraintCoincidence_21")
SketchConstraintCoincidence_18 = Sketch_1.setCoincident(SketchArc_8.endPoint(), SketchLine_10.result())
SketchConstraintCoincidence_18.setName("SketchConstraintCoincidence_22")
SketchConstraintCoincidence_19 = Sketch_1.setCoincident(SketchArc_9.startPoint(), SketchLine_6.result())
SketchConstraintCoincidence_19.setName("SketchConstraintCoincidence_23")
SketchCircle_1 = Sketch_1.addCircle(0, 0, 24)
SketchCircle_1.setAuxiliary(True)
SketchConstraintCoincidence_20 = Sketch_1.setCoincident(SketchAPI_Point(SketchPoint_1).coordinates(), SketchCircle_1.center())
SketchConstraintCoincidence_20.setName("SketchConstraintCoincidence_24")
SketchConstraintRadius_3 = Sketch_1.setRadius(SketchCircle_1.results()[1], 24)
SketchCircle_2 = Sketch_1.addCircle(0, -24, 4)
SketchConstraintCoincidence_21 = Sketch_1.setCoincident(SketchCircle_1.results()[1], SketchCircle_2.center())
SketchConstraintCoincidence_21.setName("SketchConstraintCoincidence_25")
SketchConstraintRadius_4 = Sketch_1.setRadius(SketchCircle_2.results()[1], 4)
SketchConstraintCoincidence_22 = Sketch_1.setCoincident(SketchCircle_2.center(), SketchLine_4.result())
SketchConstraintCoincidence_22.setName("SketchConstraintCoincidence_26")
SketchMultiRotation_2 = Sketch_1.addRotation([SketchCircle_2.results()[1]], SketchAPI_Line(SketchLine_4).startPoint(), 360, 3, True)
[SketchCircle_3, SketchCircle_4] = SketchMultiRotation_2.rotated()
model.do()
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_2f-SketchArc_3_2f-SketchLine_3f-SketchArc_4_2f-SketchLine_4f-SketchArc_11_2f-SketchLine_7f-SketchArc_5_2f-SketchLine_10f-SketchArc_8_2f-SketchLine_13f-SketchArc_10_2f-SketchLine_8f-SketchArc_6_2f-SketchLine_11f-SketchArc_9_2f-SketchLine_14f-SketchArc_2_2f-SketchCircle_2_2r-SketchCircle_3_2r-SketchCircle_4_2r")], model.selection(), "h1", 0)
Sketch_2 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_1_1/To_Face"))
SketchProjection_4 = Sketch_2.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_12 = SketchProjection_4.createdFeature()
SketchLine_12.setName("SketchLine_12")
SketchLine_12.result().setName("SketchLine_12")
SketchProjection_5 = Sketch_2.addProjection(model.selection("EDGE", "PartSet/OY"), False)
SketchLine_13 = SketchProjection_5.createdFeature()
SketchLine_13.setName("SketchLine_15")
SketchLine_13.result().setName("SketchLine_15")
SketchLine_14 = Sketch_2.addLine(7.500000000000332, 12.99038105676728, 15, 0)
SketchLine_14.setName("SketchLine_16")
SketchLine_14.result().setName("SketchLine_16")
SketchConstraintCoincidence_23 = Sketch_2.setCoincident(SketchLine_14.endPoint(), SketchLine_12.result())
SketchConstraintCoincidence_23.setName("SketchConstraintCoincidence_28")
SketchConstraintDistance_3 = Sketch_2.setDistance(SketchAPI_Line(SketchLine_12).startPoint(), SketchLine_14.startPoint(), "d", True)
SketchConstraintDistance_4 = Sketch_2.setDistance(SketchAPI_Line(SketchLine_12).startPoint(), SketchLine_14.endPoint(), "d", True)
SketchLine_15 = Sketch_2.addLine(0, 0, 7.500000000000332, 12.99038105676728)
SketchLine_15.setName("SketchLine_17")
SketchLine_15.result().setName("SketchLine_17")
SketchLine_15.setAuxiliary(True)
SketchConstraintCoincidence_24 = Sketch_2.setCoincident(SketchAPI_Line(SketchLine_12).startPoint(), SketchLine_15.startPoint())
SketchConstraintCoincidence_24.setName("SketchConstraintCoincidence_27")
SketchConstraintCoincidence_25 = Sketch_2.setCoincident(SketchLine_14.startPoint(), SketchLine_15.endPoint())
SketchConstraintCoincidence_25.setName("SketchConstraintCoincidence_29")
SketchConstraintAngle_1 = Sketch_2.setAngle(SketchLine_12.result(), SketchLine_15.result(), "360/6")
SketchMultiRotation_3 = Sketch_2.addRotation([SketchLine_14.result()], SketchAPI_Line(SketchLine_12).startPoint(), 360, 6, True)
[SketchLine_16, SketchLine_17, SketchLine_18, SketchLine_19, SketchLine_20] = SketchMultiRotation_3.rotated()
SketchLine_20.setName("SketchLine_22")
SketchLine_20.result().setName("SketchLine_22")
SketchLine_19.setName("SketchLine_21")
SketchLine_19.result().setName("SketchLine_21")
SketchLine_18.setName("SketchLine_20")
SketchLine_18.result().setName("SketchLine_20")
SketchLine_17.setName("SketchLine_19")
SketchLine_17.result().setName("SketchLine_19")
SketchLine_16.setName("SketchLine_18")
SketchLine_16.result().setName("SketchLine_18")
model.do()
Extrusion_2 = model.addExtrusion(Part_1_doc, [model.selection("WIRE", "Sketch_2/Face-SketchLine_18r-SketchLine_19r-SketchLine_20r-SketchLine_21r-SketchLine_22r-SketchLine_16r_wire")], model.selection(), "h2-h3-h1", 0)
Sketch_3 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_2_1/To_Face"))
SketchProjection_6 = Sketch_3.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_21 = SketchProjection_6.createdFeature()
SketchLine_21.setName("SketchLine_23")
SketchLine_21.result().setName("SketchLine_23")
SketchCircle_5 = Sketch_3.addCircle(0, 0, 10)
SketchConstraintCoincidence_26 = Sketch_3.setCoincident(SketchAPI_Line(SketchLine_21).startPoint(), SketchCircle_5.center())
SketchConstraintCoincidence_26.setName("SketchConstraintCoincidence_30")
SketchConstraintRadius_5 = Sketch_3.setRadius(SketchCircle_5.results()[1], 10)
model.do()
Fuse_1 = model.addFuse(Part_1_doc, [model.selection("SOLID", "Extrusion_1_1"), model.selection("SOLID", "Extrusion_2_1")])
ExtrusionCut_1 = model.addExtrusionCut(Part_1_doc, [model.selection("FACE", "Sketch_3/Face-SketchCircle_5_2f")], model.selection(), 0, 60, [model.selection("SOLID", "Fuse_1_1")])
Sketch_4 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_1_1/From_Face"))
SketchProjection_7 = Sketch_4.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_22 = SketchProjection_7.createdFeature()
SketchLine_22.setName("SketchLine_24")
SketchLine_22.result().setName("SketchLine_24")
SketchCircle_6 = Sketch_4.addCircle(0, 0, 12.5)
SketchConstraintCoincidence_27 = Sketch_4.setCoincident(SketchAPI_Line(SketchLine_22).startPoint(), SketchCircle_6.center())
SketchConstraintCoincidence_27.setName("SketchConstraintCoincidence_31")
SketchConstraintRadius_6 = Sketch_4.setRadius(SketchCircle_6.results()[1], 12.5)
model.do()
ExtrusionCut_2 = model.addExtrusionCut(Part_1_doc, [model.selection("COMPOUND", "Sketch_4")], model.selection(), model.selection("FACE", "Sketch_1/Face-SketchLine_2f-SketchArc_3_2f-SketchLine_3f-SketchArc_4_2f-SketchLine_4f-SketchArc_11_2f-SketchLine_7f-SketchArc_5_2f-SketchLine_10f-SketchArc_8_2f-SketchLine_13f-SketchArc_10_2f-SketchLine_8f-SketchArc_6_2f-SketchLine_11f-SketchArc_9_2f-SketchLine_14f-SketchArc_2_2f-SketchCircle_2_2r-SketchCircle_3_2r-SketchCircle_4_2r"), 0, model.selection(), 0, [model.selection("SOLID", "ExtrusionCut_1_1")])
Fillet_1_objects = [model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_20]"), model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_21]"), model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_22]"), model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_16]"), model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_19]"), model.selection("EDGE", "[Fuse_1_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_2_1/Generated_Face&Sketch_2/SketchLine_18]")]
Fillet_1 = model.addFillet(Part_1_doc, Fillet_1_objects, 1)
Sketch_5 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_1_1/Modified_Face&Extrusion_2_1/To_Face"))
SketchProjection_8 = Sketch_5.addProjection(model.selection("VERTEX", "[ExtrusionCut_1_1/Modified_Face&Extrusion_2_1/To_Face][ExtrusionCut_1_1/Generated_Face&Sketch_3/SketchCircle_5_2]__cc"), False)
SketchPoint_5 = SketchProjection_8.createdFeature()
SketchCircle_7 = Sketch_5.addCircle(0, 0, 22)
SketchConstraintCoincidence_28 = Sketch_5.setCoincident(SketchPoint_5.result(), SketchCircle_7.center())
SketchConstraintCoincidence_28.setName("SketchConstraintCoincidence_32")
SketchConstraintRadius_7 = Sketch_5.setRadius(SketchCircle_7.results()[1], 22)
model.do()
Extrusion_3 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_5/Face-SketchCircle_7_2f")], model.selection(), "h3", 0)
Fuse_2 = model.addFuse(Part_1_doc, [model.selection("SOLID", "Fillet_1_1"), model.selection("SOLID", "Extrusion_3_1")])
Sketch_6 = model.addSketch(Part_1_doc, model.standardPlane("XOZ"))
SketchLine_23 = Sketch_6.addLine(22, 70, 20, 70)
SketchLine_23.setName("SketchLine_25")
SketchLine_23.result().setName("SketchLine_25")
SketchProjection_9 = Sketch_6.addProjection(model.selection("VERTEX", "[Extrusion_3_1/Generated_Face&Sketch_5/SketchCircle_7_2][Extrusion_3_1/To_Face]"), False)
SketchPoint_6 = SketchProjection_9.createdFeature()
SketchConstraintCoincidence_29 = Sketch_6.setCoincident(SketchLine_23.startPoint(), SketchPoint_6.result())
SketchConstraintCoincidence_29.setName("SketchConstraintCoincidence_33")
SketchLine_24 = Sketch_6.addLine(20, 70, 22, 68.84529946162075)
SketchLine_24.setName("SketchLine_26")
SketchLine_24.result().setName("SketchLine_26")
SketchConstraintCoincidence_30 = Sketch_6.setCoincident(SketchLine_23.endPoint(), SketchLine_24.startPoint())
SketchConstraintCoincidence_30.setName("SketchConstraintCoincidence_34")
SketchProjection_10 = Sketch_6.addProjection(model.selection("EDGE", "([Extrusion_3_1/Generated_Face&Sketch_5/SketchCircle_7_2][Extrusion_3_1/To_Face])_Fuse_2_1"), False)
SketchLine_25 = SketchProjection_10.createdFeature()
SketchLine_25.setName("SketchLine_27")
SketchLine_25.result().setName("SketchLine_27")
SketchConstraintCoincidence_31 = Sketch_6.setCoincident(SketchLine_24.endPoint(), SketchLine_25.result())
SketchConstraintCoincidence_31.setName("SketchConstraintCoincidence_35")
SketchLine_26 = Sketch_6.addLine(22, 68.84529946162075, 22, 70)
SketchLine_26.setName("SketchLine_28")
SketchLine_26.result().setName("SketchLine_28")
SketchConstraintCoincidence_32 = Sketch_6.setCoincident(SketchLine_24.endPoint(), SketchLine_26.startPoint())
SketchConstraintCoincidence_32.setName("SketchConstraintCoincidence_36")
SketchConstraintCoincidence_33 = Sketch_6.setCoincident(SketchLine_23.startPoint(), SketchLine_26.endPoint())
SketchConstraintCoincidence_33.setName("SketchConstraintCoincidence_37")
SketchLine_27 = Sketch_6.addLine(21.99999999999999, 64, 22, 63)
SketchLine_27.setName("SketchLine_29")
SketchLine_27.result().setName("SketchLine_29")
SketchConstraintCoincidence_34 = Sketch_6.setCoincident(SketchLine_27.startPoint(), SketchLine_25.result())
SketchConstraintCoincidence_34.setName("SketchConstraintCoincidence_38")
SketchConstraintCoincidence_35 = Sketch_6.setCoincident(SketchAPI_Line(SketchLine_25).startPoint(), SketchLine_27.endPoint())
SketchConstraintCoincidence_35.setName("SketchConstraintCoincidence_39")
SketchLine_28 = Sketch_6.addLine(21.99999999999999, 64, 21.73205080756888, 63)
SketchLine_28.setName("SketchLine_30")
SketchLine_28.result().setName("SketchLine_30")
SketchConstraintCoincidence_36 = Sketch_6.setCoincident(SketchLine_27.startPoint(), SketchLine_28.startPoint())
SketchConstraintCoincidence_36.setName("SketchConstraintCoincidence_40")
SketchLine_29 = Sketch_6.addLine(21.73205080756888, 63, 22, 63)
SketchLine_29.setName("SketchLine_31")
SketchLine_29.result().setName("SketchLine_31")
SketchConstraintCoincidence_37 = Sketch_6.setCoincident(SketchLine_28.endPoint(), SketchLine_29.startPoint())
SketchConstraintCoincidence_37.setName("SketchConstraintCoincidence_41")
SketchConstraintCoincidence_38 = Sketch_6.setCoincident(SketchAPI_Line(SketchLine_25).startPoint(), SketchLine_29.endPoint())
SketchConstraintCoincidence_38.setName("SketchConstraintCoincidence_42")
SketchConstraintLength_1 = Sketch_6.setLength(SketchLine_23.result(), 2)
SketchConstraintAngle_2 = Sketch_6.setAngleBackward(SketchLine_23.result(), SketchLine_24.result(), 30)
SketchConstraintHorizontal_2 = Sketch_6.setHorizontal(SketchLine_23.result())
SketchConstraintHorizontal_2.setName("SketchConstraintHorizontal_3")
SketchConstraintHorizontal_3 = Sketch_6.setHorizontal(SketchLine_29.result())
SketchConstraintHorizontal_3.setName("SketchConstraintHorizontal_4")
SketchConstraintLength_2 = Sketch_6.setLength(SketchLine_27.result(), 1)
SketchConstraintAngle_3 = Sketch_6.setAngle(SketchLine_28.result(), SketchLine_27.result(), 15)
model.do()
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("COMPOUND", "Sketch_6")], model.selection("EDGE", "PartSet/OZ"), 360, 0)
Cut_1 = model.addCut(Part_1_doc, [model.selection("SOLID", "Fuse_2_1")], [model.selection("SOLID", "Revolution_1_2"), model.selection("SOLID", "Revolution_1_1")])
Sketch_7 = model.addSketch(Part_1_doc, model.selection("FACE", "Cut_1_1/Modified_Face&Extrusion_3_1/To_Face"))
SketchProjection_11 = Sketch_7.addProjection(model.selection("VERTEX", "[Revolution_1_1/Generated_Face&Sketch_6/SketchLine_26][Cut_1_1/Modified_Face&Extrusion_3_1/To_Face]__cc"), False)
SketchPoint_7 = SketchProjection_11.createdFeature()
SketchCircle_8 = Sketch_7.addCircle(0, 0, 26)
SketchCircle_8.setAuxiliary(True)
SketchConstraintCoincidence_39 = Sketch_7.setCoincident(SketchPoint_7.result(), SketchCircle_8.center())
SketchConstraintCoincidence_39.setName("SketchConstraintCoincidence_43")
SketchConstraintRadius_8 = Sketch_7.setRadius(SketchCircle_8.results()[1], 26)
SketchCircle_9 = Sketch_7.addCircle(0, -26, 5.25)
SketchConstraintCoincidence_40 = Sketch_7.setCoincident(SketchCircle_8.results()[1], SketchCircle_9.center())
SketchConstraintCoincidence_40.setName("SketchConstraintCoincidence_44")
SketchProjection_12 = Sketch_7.addProjection(model.selection("EDGE", "PartSet/OY"), False)
SketchLine_30 = SketchProjection_12.createdFeature()
SketchLine_30.setName("SketchLine_32")
SketchLine_30.result().setName("SketchLine_32")
SketchConstraintRadius_9 = Sketch_7.setRadius(SketchCircle_9.results()[1], 5.25)
SketchConstraintCoincidence_41 = Sketch_7.setCoincident(SketchCircle_9.center(), SketchLine_30.result())
SketchConstraintCoincidence_41.setName("SketchConstraintCoincidence_45")
SketchMultiRotation_4 = Sketch_7.addRotation([SketchCircle_9.results()[1]], SketchAPI_Line(SketchLine_30).startPoint(), 360, 16, True)
[SketchCircle_10, SketchCircle_11, SketchCircle_12, SketchCircle_13, SketchCircle_14, SketchCircle_15, SketchCircle_16, SketchCircle_17, SketchCircle_18, SketchCircle_19, SketchCircle_20, SketchCircle_21, SketchCircle_22, SketchCircle_23, SketchCircle_24] = SketchMultiRotation_4.rotated()
model.do()
Extrusion_4_objects = [model.selection("FACE", "Sketch_7/Face-SketchCircle_19_2f-SketchCircle_20_2r-SketchCircle_19_2f-SketchCircle_19_2f-SketchCircle_18_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_18_2f-SketchCircle_19_2r-SketchCircle_18_2f-SketchCircle_17_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_17_2f-SketchCircle_18_2r-SketchCircle_18_2r-SketchCircle_17_2f-SketchCircle_16_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_15_2f-SketchCircle_15_2f-SketchCircle_16_2r-SketchCircle_15_2f-SketchCircle_14_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_16_2f-SketchCircle_16_2f-SketchCircle_17_2r-SketchCircle_17_2r-SketchCircle_16_2f-SketchCircle_15_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_14_2f-SketchCircle_14_2f-SketchCircle_15_2r-SketchCircle_14_2f-SketchCircle_13_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_13_2f-SketchCircle_13_2f-SketchCircle_14_2r-SketchCircle_13_2f-SketchCircle_12_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_12_2f-SketchCircle_12_2f-SketchCircle_13_2r-SketchCircle_12_2f-SketchCircle_11_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_11_2f-SketchCircle_11_2f-SketchCircle_12_2r-SketchCircle_11_2f-SketchCircle_10_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_9_2r-SketchCircle_10_2f-SketchCircle_10_2f-SketchCircle_11_2r-SketchCircle_10_2f-SketchCircle_9_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_9_2f-SketchCircle_24_2r-SketchCircle_24_2r-SketchCircle_9_2f-SketchCircle_10_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_24_2f-SketchCircle_23_2r-SketchCircle_24_2f-SketchCircle_9_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_23_2f-SketchCircle_24_2r-SketchCircle_23_2f-SketchCircle_23_2f-SketchCircle_22_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_22_2f-SketchCircle_23_2r-SketchCircle_22_2f-SketchCircle_22_2f-SketchCircle_21_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_21_2f-SketchCircle_22_2r-SketchCircle_21_2f-SketchCircle_21_2f-SketchCircle_20_2r"), model.selection("FACE", "Sketch_7/Face-SketchCircle_20_2f-SketchCircle_21_2r-SketchCircle_20_2f-SketchCircle_20_2f-SketchCircle_19_2r")]
Extrusion_4 = model.addExtrusion(Part_1_doc, Extrusion_4_objects, model.selection(), model.selection("FACE", "Sketch_5/Face-SketchCircle_7_2f"), 0, model.selection(), 0)
model.do()
# change the parameter value
param_h2.setValue(90)
model.end()
# check the final extrusion is valid:
aFactory = ModelAPI_Session.get().validators()
assert(aFactory.validate(Extrusion_4.feature()))

model.testResultsVolumes(Extrusion_4, [597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447, 597.118881248058755772945005447])
