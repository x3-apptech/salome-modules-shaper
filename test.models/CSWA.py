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

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Parameter_A = model.addParameter(Part_1_doc, "a", "63")
model.addParameter(Part_1_doc, "b", "50")
model.addParameter(Part_1_doc, "c", "100")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))
SketchLine_1 = Sketch_1.addLine(-45, 0, 0, 0)
SketchLine_2 = Sketch_1.addLine(0, 0, 23.33965093306752, 8.494938217797719)
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.startPoint())
SketchLine_3 = Sketch_1.addLine(33.83092017818969, 6.16907982180411, 40, 0)
SketchLine_4 = Sketch_1.addLine(40, 0, 55, 0)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchConstraintCoincidence_2.setName("SketchConstraintCoincidence_3")
SketchLine_5 = Sketch_1.addLine(55, 0, 55, 48)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchConstraintCoincidence_3.setName("SketchConstraintCoincidence_4")
SketchLine_6 = Sketch_1.addLine(55, 48, 15, 48)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchConstraintCoincidence_4.setName("SketchConstraintCoincidence_5")
SketchLine_7 = Sketch_1.addLine(15, 48, 15, 63)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.startPoint())
SketchConstraintCoincidence_5.setName("SketchConstraintCoincidence_6")
SketchLine_8 = Sketch_1.addLine(15, 63, -15, 63)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_8.startPoint())
SketchConstraintCoincidence_6.setName("SketchConstraintCoincidence_7")
SketchLine_9 = Sketch_1.addLine(-15, 63, -15, 35.00000000000001)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_8.endPoint(), SketchLine_9.startPoint())
SketchConstraintCoincidence_7.setName("SketchConstraintCoincidence_8")
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_1.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_6.result())
SketchConstraintHorizontal_3 = Sketch_1.setHorizontal(SketchLine_8.result())
SketchConstraintCollinear_1 = Sketch_1.setCollinear(SketchLine_1.result(), SketchLine_4.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_5.result())
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_9.result())
SketchConstraintVertical_3 = Sketch_1.setVertical(SketchLine_7.result())
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchLine_3.endPoint(), SketchLine_4.endPoint(), 15)
SketchConstraintDistance_2 = Sketch_1.setDistance(SketchLine_3.endPoint(), SketchLine_1.endPoint(), 40)
SketchConstraintDistance_3 = Sketch_1.setDistance(SketchLine_1.startPoint(), SketchLine_4.endPoint(), "c")
SketchConstraintDistance_4 = Sketch_1.setDistance(SketchLine_8.startPoint(), SketchLine_6.result(), 15)
SketchConstraintDistance_5 = Sketch_1.setDistance(SketchLine_8.startPoint(), SketchLine_5.result(), 40)
SketchLine_10 = Sketch_1.addLine(-45, 4.999999999999999, -45, 0)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_10.endPoint())
SketchConstraintCoincidence_8.setName("SketchConstraintCoincidence_9")
SketchConstraintVertical_4 = Sketch_1.setVertical(SketchLine_10.result())
SketchConstraintDistance_6 = Sketch_1.setDistance(SketchLine_9.startPoint(), SketchLine_1.result(), "a")
SketchArc_1 = Sketch_1.addArc(-45, 35, -45, 4.999999999999999, -15, 35.00000000000001, False)
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchArc_1.center(), SketchLine_10.result())
SketchConstraintCoincidence_9.setName("SketchConstraintCoincidence_10")
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchArc_1.startPoint(), SketchLine_10.startPoint())
SketchConstraintCoincidence_10.setName("SketchConstraintCoincidence_11")
SketchConstraintAngle_1 = Sketch_1.setAngle(SketchLine_3.result(), SketchLine_1.result(), 45)
SketchConstraintAngle_2 = Sketch_1.setAngleBackward(SketchLine_2.result(), SketchLine_4.result(), 20)
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchArc_1.endPoint(), SketchLine_9.endPoint())
SketchConstraintCoincidence_11.setName("SketchConstraintCoincidence_12")
SketchConstraintTangent_1 = Sketch_1.setTangent(SketchArc_1.results()[1], SketchLine_9.result())
SketchConstraintDistance_7 = Sketch_1.setDistance(SketchArc_1.center(), SketchLine_1.result(), 35)
SketchConstraintRadius_1 = Sketch_1.setRadius(SketchArc_1.results()[1], 30)
SketchArc_2 = Sketch_1.addArc(26.75985236632421, -0.9019879900613652, 33.83092017818969, 6.16907982180411, 23.33965093306752, 8.494938217797719, False)
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchArc_2.startPoint(), SketchLine_3.startPoint())
SketchConstraintCoincidence_12.setName("SketchConstraintCoincidence_13")
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchArc_2.endPoint(), SketchLine_2.endPoint())
SketchConstraintCoincidence_13.setName("SketchConstraintCoincidence_14")
SketchConstraintTangent_2 = Sketch_1.setTangent(SketchArc_2.results()[1], SketchLine_2.result())
SketchConstraintTangent_3 = Sketch_1.setTangent(SketchArc_2.results()[1], SketchLine_3.result())
SketchConstraintRadius_2 = Sketch_1.setRadius(SketchArc_2.results()[1], 10)
SketchPoint_1 = Sketch_1.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchConstraintCoincidence_14 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchPoint_1.coordinates())
SketchConstraintCoincidence_14.setName("SketchConstraintCoincidence_15")
model.do()
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_1r-SketchLine_2f-SketchArc_2_2r-SketchLine_3f-SketchLine_4f-SketchLine_5f-SketchLine_6f-SketchLine_7f-SketchLine_8f-SketchLine_9f-SketchArc_1_2r-SketchLine_10f")], model.selection(), "b/2", "b/2")
Sketch_2 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_1_1/To_Face"))
SketchPoint_2 = Sketch_2.addPoint(model.selection("VERTEX", "[Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_9][Extrusion_1_1/Generated_Face&Sketch_1/SketchArc_1_2][Extrusion_1_1/To_Face]"))
SketchCircle_1 = Sketch_2.addCircle(-15, 35.00000000000001, 25)
SketchConstraintCoincidence_15 = Sketch_2.setCoincident(SketchPoint_2.result(), SketchCircle_1.center())
SketchConstraintCoincidence_15.setName("SketchConstraintCoincidence_16")
SketchConstraintRadius_3 = Sketch_2.setRadius(SketchCircle_1.results()[1], "50/2")
model.do()
ExtrusionCut_1 = model.addExtrusionCut(Part_1_doc, [model.selection("WIRE", "Sketch_2/Face-SketchCircle_1_2f_wire")], model.selection(), 0, 13, [model.selection("SOLID", "Extrusion_1_1")])
Sketch_3 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_1_1/Modified_Face&ExtrusionCut_1_1/From_Face"))
SketchPoint_3 = Sketch_3.addPoint(model.selection("VERTEX", "ExtrusionCut_1_1/Generated_Vertex&ExtrusionCut_1_1/From_Face"))
SketchLine_11 = Sketch_3.addLine(model.selection("EDGE", "([ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchArc_1_2][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9])([Extrusion_1_1/From_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9])2([Extrusion_1_1/From_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchArc_1_2])2([ExtrusionCut_1_1/Modified_Face&Extrusion_1_1/To_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9])2"))
SketchArc_3 = Sketch_3.addArc(-15, 35.00000000000001, -15, 46.0103340429751, -17.02045759563166, 24.17663606626138, True)
SketchConstraintCoincidence_16 = Sketch_3.setCoincident(SketchPoint_3.result(), SketchArc_3.center())
SketchConstraintCoincidence_16.setName("SketchConstraintCoincidence_17")
SketchConstraintCoincidence_17 = Sketch_3.setCoincident(SketchLine_11.result(), SketchArc_3.startPoint())
SketchConstraintCoincidence_17.setName("SketchConstraintCoincidence_18")
SketchArc_4 = Sketch_3.addArc(model.selection("EDGE", "([ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchArc_1_2][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9])([Extrusion_1_1/From_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9])2([Extrusion_1_1/From_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchArc_1_2])2([ExtrusionCut_1_1/Modified_Face&Extrusion_1_1/To_Face][ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchArc_1_2])2"))
SketchConstraintCoincidence_18 = Sketch_3.setCoincident(SketchArc_3.endPoint(), SketchArc_4.results()[1])
SketchConstraintCoincidence_18.setName("SketchConstraintCoincidence_19")
SketchLine_12 = Sketch_3.addLine(-15, 46.0103340429751, -15, 35.00000000000001)
SketchConstraintCoincidence_19 = Sketch_3.setCoincident(SketchArc_3.startPoint(), SketchLine_12.startPoint())
SketchConstraintCoincidence_19.setName("SketchConstraintCoincidence_20")
SketchConstraintCoincidence_20 = Sketch_3.setCoincident(SketchPoint_3.coordinates(), SketchLine_12.endPoint())
SketchConstraintCoincidence_20.setName("SketchConstraintCoincidence_21")
SketchArc_5 = Sketch_3.addArc(-45, 35, -15, 35.00000000000001, -17.02045759563121, 24.17663606626137, True)
SketchConstraintCoincidence_21 = Sketch_3.setCoincident(SketchArc_4.result(), SketchArc_5.center())
SketchConstraintCoincidence_21.setName("SketchConstraintCoincidence_22")
SketchConstraintCoincidence_22 = Sketch_3.setCoincident(SketchPoint_3.coordinates(), SketchArc_5.startPoint())
SketchConstraintCoincidence_22.setName("SketchConstraintCoincidence_23")
SketchConstraintCoincidence_23 = Sketch_3.setCoincident(SketchArc_3.results()[1], SketchArc_5.endPoint())
SketchConstraintCoincidence_23.setName("SketchConstraintCoincidence_24")
model.do()
ExtrusionFuse_1 = model.addExtrusionFuse(Part_1_doc, [model.selection("FACE", "Sketch_3/Face-SketchArc_3_2f-SketchLine_12f-SketchArc_5_2r")], model.selection(), 5, 0, [model.selection("SOLID", "ExtrusionCut_1_1")])
Sketch_4 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionFuse_1_1/To_Face"))
SketchPoint_4 = Sketch_4.addPoint(model.selection("VERTEX", "[ExtrusionFuse_1_1/Generated_Face&Sketch_3/SketchArc_5_2][ExtrusionFuse_1_1/Generated_Face&Sketch_3/SketchLine_12][ExtrusionFuse_1_1/To_Face]"))
SketchCircle_2 = Sketch_4.addCircle(-15, 35.00000000000001, 5)
SketchConstraintCoincidence_24 = Sketch_4.setCoincident(SketchPoint_4.result(), SketchCircle_2.center())
SketchConstraintCoincidence_24.setName("SketchConstraintCoincidence_25")
SketchConstraintRadius_4 = Sketch_4.setRadius(SketchCircle_2.results()[1], "10/2")
model.do()
ExtrusionFuse_2 = model.addExtrusionFuse(Part_1_doc, [model.selection("FACE", "Sketch_4/Face-SketchCircle_2_2f")], model.selection(), 8, 0, [model.selection("SOLID", "ExtrusionFuse_1_1")])
Sketch_5 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_1_1/Modified_Face&Extrusion_1_1/To_Face"))
SketchArc_6 = Sketch_5.addArc(model.selection("EDGE", "[ExtrusionCut_1_1/Modified_Face&Extrusion_1_1/To_Face][ExtrusionFuse_1_1/Modified_Face&Sketch_1/SketchArc_1_2]"))
SketchArc_7 = Sketch_5.addArc(-45, 35, -45, 5, -25.41666666666737, 12.27351642784166, False)
SketchConstraintCoincidence_25 = Sketch_5.setCoincident(SketchArc_6.result(), SketchArc_7.center())
SketchConstraintCoincidence_25.setName("SketchConstraintCoincidence_26")
SketchCircle_3 = Sketch_5.addCircle(model.selection("EDGE", "[ExtrusionFuse_2_1/Generated_Face&Sketch_4/SketchCircle_2_2][ExtrusionFuse_2_1/To_Face]"))
SketchArc_8 = Sketch_5.addArc(-15, 35.00000000000001, -25.41666666666737, 12.27351642784166, -30.00000000000075, 14.99999999999944, True)
SketchConstraintCoincidence_26 = Sketch_5.setCoincident(SketchCircle_3.result(), SketchArc_8.center())
SketchConstraintCoincidence_26.setName("SketchConstraintCoincidence_27")
SketchConstraintCoincidence_27 = Sketch_5.setCoincident(SketchArc_6.endPoint(), SketchArc_8.startPoint())
SketchConstraintCoincidence_27.setName("SketchConstraintCoincidence_28")
SketchConstraintCoincidence_28 = Sketch_5.setCoincident(SketchArc_7.endPoint(), SketchArc_8.results()[1])
SketchConstraintCoincidence_28.setName("SketchConstraintCoincidence_29")
SketchConstraintCoincidence_29 = Sketch_5.setCoincident(SketchArc_7.startPoint(), SketchArc_6.startPoint())
SketchConstraintCoincidence_29.setName("SketchConstraintCoincidence_30")
SketchLine_13 = Sketch_5.addLine(-45, 5, -45, 10)
SketchConstraintCoincidence_30 = Sketch_5.setCoincident(SketchArc_6.startPoint(), SketchLine_13.startPoint())
SketchConstraintCoincidence_30.setName("SketchConstraintCoincidence_31")
SketchConstraintVertical_5 = Sketch_5.setVertical(SketchLine_13.result())
SketchConstraintLength_1 = Sketch_5.setLength(SketchLine_13.result(), 5)
SketchArc_9 = Sketch_5.addArc(-45, 35, -45, 10, -30.00000000000075, 14.99999999999944, False)
SketchConstraintCoincidence_31 = Sketch_5.setCoincident(SketchArc_6.center(), SketchArc_9.center())
SketchConstraintCoincidence_31.setName("SketchConstraintCoincidence_32")
SketchConstraintCoincidence_32 = Sketch_5.setCoincident(SketchArc_8.endPoint(), SketchArc_9.endPoint())
SketchConstraintCoincidence_32.setName("SketchConstraintCoincidence_33")
SketchConstraintCoincidence_33 = Sketch_5.setCoincident(SketchArc_9.startPoint(), SketchLine_13.endPoint())
SketchConstraintCoincidence_33.setName("SketchConstraintCoincidence_34")
model.do()
ExtrusionFuse_3 = model.addExtrusionFuse(Part_1_doc, [model.selection("FACE", "Sketch_5/Face-SketchArc_7_2f-SketchArc_8_2r-SketchArc_9_2r-SketchLine_13r")], model.selection(), 0, 8, [model.selection("SOLID", "ExtrusionFuse_2_1")])
Sketch_6 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionFuse_3_1/Modified_Face&Extrusion_1_1/To_Face"))
SketchLine_14 = Sketch_6.addLine(model.selection("EDGE", "[ExtrusionFuse_3_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_5]"))
SketchArc_10 = Sketch_6.addArc(55, 35, 55, 15, 35, 35, True)
SketchConstraintCoincidence_34 = Sketch_6.setCoincident(SketchLine_14.result(), SketchArc_10.center())
SketchConstraintCoincidence_34.setName("SketchConstraintCoincidence_35")
SketchConstraintCoincidence_35 = Sketch_6.setCoincident(SketchLine_14.result(), SketchArc_10.startPoint())
SketchConstraintCoincidence_35.setName("SketchConstraintCoincidence_36")
SketchLine_15 = Sketch_6.addLine(model.selection("EDGE", "[ExtrusionFuse_3_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_4]"))
SketchConstraintDistance_8 = Sketch_6.setDistance(SketchArc_10.center(), SketchLine_15.result(), 35, False)
SketchLine_16 = Sketch_6.addLine(35, 35, 35, 47.99999999999999)
SketchConstraintCoincidence_36 = Sketch_6.setCoincident(SketchArc_10.endPoint(), SketchLine_16.startPoint())
SketchConstraintCoincidence_36.setName("SketchConstraintCoincidence_37")
SketchLine_17 = Sketch_6.addLine(model.selection("EDGE", "[ExtrusionFuse_3_1/Modified_Face&Extrusion_1_1/To_Face][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_6]"))
SketchConstraintCoincidence_37 = Sketch_6.setCoincident(SketchLine_16.endPoint(), SketchLine_17.result())
SketchConstraintCoincidence_37.setName("SketchConstraintCoincidence_38")
SketchConstraintVertical_6 = Sketch_6.setVertical(SketchLine_16.result())
SketchConstraintTangent_4 = Sketch_6.setTangent(SketchLine_16.result(), SketchArc_10.results()[1])
SketchConstraintRadius_5 = Sketch_6.setRadius(SketchArc_10.results()[1], 20)
SketchLine_18 = Sketch_6.addLine(55, 15, 55, 47.99999999999999)
SketchConstraintCoincidence_38 = Sketch_6.setCoincident(SketchArc_10.startPoint(), SketchLine_18.startPoint())
SketchConstraintCoincidence_38.setName("SketchConstraintCoincidence_39")
SketchConstraintCoincidence_39 = Sketch_6.setCoincident(SketchLine_14.endPoint(), SketchLine_18.endPoint())
SketchConstraintCoincidence_39.setName("SketchConstraintCoincidence_40")
SketchLine_19 = Sketch_6.addLine(35, 47.99999999999999, 55, 47.99999999999999)
SketchConstraintCoincidence_40 = Sketch_6.setCoincident(SketchLine_16.endPoint(), SketchLine_19.startPoint())
SketchConstraintCoincidence_40.setName("SketchConstraintCoincidence_41")
SketchConstraintCoincidence_41 = Sketch_6.setCoincident(SketchLine_14.endPoint(), SketchLine_19.endPoint())
SketchConstraintCoincidence_41.setName("SketchConstraintCoincidence_42")
model.do()
ExtrusionCut_2 = model.addExtrusionCut(Part_1_doc, [model.selection("WIRE", "Sketch_6/Face-SketchArc_10_2f-SketchLine_18f-SketchLine_19r-SketchLine_16r_wire")], model.selection(), 0, 9, [model.selection("SOLID", "ExtrusionFuse_3_1")])
Sketch_7 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_2_1/From_Face"))
SketchCircle_4 = Sketch_7.addCircle(45, -35, 5)
SketchLine_20 = Sketch_7.addLine(model.selection("EDGE", "[ExtrusionCut_2_1/Modified_Face&Sketch_1/SketchLine_5][ExtrusionCut_2_1/From_Face]"))
SketchConstraintDistance_9 = Sketch_7.setDistance(SketchCircle_4.center(), SketchLine_20.result(), 10, False)
SketchConstraintRadius_6 = Sketch_7.setRadius(SketchCircle_4.results()[1], "10/2")
SketchLine_21 = Sketch_7.addLine(35, -35, 45, -35)
SketchLine_21.setAuxiliary(True)
SketchPoint_5 = Sketch_7.addPoint(model.selection("VERTEX", "[ExtrusionCut_2_1/Generated_Face&Sketch_6/SketchLine_16][ExtrusionCut_2_1/Generated_Face&Sketch_6/SketchArc_10_2][ExtrusionCut_2_1/From_Face]"))
SketchConstraintCoincidence_42 = Sketch_7.setCoincident(SketchLine_21.startPoint(), SketchPoint_5.result())
SketchConstraintCoincidence_42.setName("SketchConstraintCoincidence_43")
SketchConstraintCoincidence_43 = Sketch_7.setCoincident(SketchCircle_4.center(), SketchLine_21.endPoint())
SketchConstraintCoincidence_43.setName("SketchConstraintCoincidence_44")
SketchConstraintHorizontal_4 = Sketch_7.setHorizontal(SketchLine_21.result())
model.do()
ExtrusionCut_3 = model.addExtrusionCut(Part_1_doc, [model.selection("FACE", "Sketch_7/Face-SketchCircle_4_2r")], model.selection(), model.selection(), 0, model.selection("FACE", "Extrusion_1_1/From_Face"), 0, [model.selection("SOLID", "ExtrusionCut_2_1")])
Sketch_8 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8"))
SketchLine_22 = Sketch_8.addLine(55, 25, 5.000000000000002, 25)
SketchLine_23 = Sketch_8.addLine(5.000000000000002, 25, 5.000000000000002, 13)
SketchLine_24 = Sketch_8.addLine(5.000000000000002, 13, 55, 13)
SketchLine_25 = Sketch_8.addLine(55, 13, 55, 25)
SketchConstraintCoincidence_44 = Sketch_8.setCoincident(SketchLine_25.endPoint(), SketchLine_22.startPoint())
SketchConstraintCoincidence_44.setName("SketchConstraintCoincidence_45")
SketchConstraintCoincidence_45 = Sketch_8.setCoincident(SketchLine_22.endPoint(), SketchLine_23.startPoint())
SketchConstraintCoincidence_45.setName("SketchConstraintCoincidence_46")
SketchConstraintCoincidence_46 = Sketch_8.setCoincident(SketchLine_23.endPoint(), SketchLine_24.startPoint())
SketchConstraintCoincidence_46.setName("SketchConstraintCoincidence_47")
SketchConstraintCoincidence_47 = Sketch_8.setCoincident(SketchLine_24.endPoint(), SketchLine_25.startPoint())
SketchConstraintCoincidence_47.setName("SketchConstraintCoincidence_48")
SketchConstraintHorizontal_5 = Sketch_8.setHorizontal(SketchLine_22.result())
SketchConstraintVertical_7 = Sketch_8.setVertical(SketchLine_23.result())
SketchConstraintHorizontal_6 = Sketch_8.setHorizontal(SketchLine_24.result())
SketchConstraintVertical_8 = Sketch_8.setVertical(SketchLine_25.result())
SketchConstraintLength_2 = Sketch_8.setLength(SketchLine_22.result(), 50)
SketchConstraintLength_3 = Sketch_8.setLength(SketchLine_25.result(), 12)
SketchProjection_1 = Sketch_8.addProjection(model.selection("EDGE", "[ExtrusionCut_2_1/Modified_Face&Sketch_1/SketchLine_5][ExtrusionCut_2_1/Modified_Face&Sketch_1/SketchLine_6]"), False)
SketchLine_26 = SketchProjection_1.createdFeature()
SketchConstraintCoincidence_48 = Sketch_8.setCoincident(SketchLine_22.startPoint(), SketchLine_26.result())
SketchConstraintCoincidence_48.setName("SketchConstraintCoincidence_49")
SketchLine_27 = Sketch_8.addLine(model.selection("EDGE", "[ExtrusionCut_3_1/Modified_Face&Extrusion_1_1/From_Face][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8]"))
SketchConstraintCoincidence_49 = Sketch_8.setCoincident(SketchLine_22.startPoint(), SketchLine_27.result())
SketchConstraintCoincidence_49.setName("SketchConstraintCoincidence_50")
model.do()
ExtrusionCut_4 = model.addExtrusionCut(Part_1_doc, [model.selection("FACE", "Sketch_8/Face-SketchLine_22r-SketchLine_23f-SketchLine_24f-SketchLine_25f")], model.selection(), model.selection(), 0, model.selection("FACE", "Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_4"), 0, [model.selection("SOLID", "ExtrusionCut_3_1")])
Sketch_9 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionFuse_1_1/Modified_Face&Sketch_1/SketchLine_9"))
SketchLine_28 = Sketch_9.addLine(25, -45, 18.44853578320833, -63)
SketchLine_29 = Sketch_9.addLine(model.selection("EDGE", "[ExtrusionFuse_1_1/Modified_Face&Sketch_1/SketchLine_9][ExtrusionCut_4_1/Modified_Face&Sketch_1/SketchLine_8]"))
SketchConstraintCoincidence_50 = Sketch_9.setCoincident(SketchLine_28.endPoint(), SketchLine_29.result())
SketchConstraintCoincidence_50.setName("SketchConstraintCoincidence_51")
SketchLine_30 = Sketch_9.addLine(model.selection("EDGE", "[ExtrusionCut_4_1/Modified_Face&Extrusion_1_1/From_Face][ExtrusionFuse_1_1/Modified_Face&Sketch_1/SketchLine_9]"))
SketchConstraintCoincidence_51 = Sketch_9.setCoincident(SketchLine_28.startPoint(), SketchLine_30.result())
SketchConstraintCoincidence_51.setName("SketchConstraintCoincidence_52")
SketchConstraintAngle_3 = Sketch_9.setAngle(SketchLine_28.result(), SketchLine_30.result(), 20.00000000000009)
SketchConstraintDistance_10 = Sketch_9.setDistance(SketchLine_28.startPoint(), SketchLine_29.result(), 18, False)
SketchLine_31 = Sketch_9.addLine(18.44853578320833, -63, 25, -63)
SketchConstraintCoincidence_52 = Sketch_9.setCoincident(SketchLine_28.endPoint(), SketchLine_31.startPoint())
SketchConstraintCoincidence_52.setName("SketchConstraintCoincidence_53")
SketchConstraintCoincidence_53 = Sketch_9.setCoincident(SketchLine_29.startPoint(), SketchLine_31.endPoint())
SketchConstraintCoincidence_53.setName("SketchConstraintCoincidence_54")
SketchLine_32 = Sketch_9.addLine(25, -63, 25, -45)
SketchConstraintCoincidence_54 = Sketch_9.setCoincident(SketchLine_29.startPoint(), SketchLine_32.startPoint())
SketchConstraintCoincidence_54.setName("SketchConstraintCoincidence_55")
SketchConstraintCoincidence_55 = Sketch_9.setCoincident(SketchLine_28.startPoint(), SketchLine_32.endPoint())
SketchConstraintCoincidence_55.setName("SketchConstraintCoincidence_56")
model.do()
ExtrusionCut_5 = model.addExtrusionCut(Part_1_doc, [model.selection("FACE", "Sketch_9/Face-SketchLine_28r-SketchLine_31f-SketchLine_32f")], model.selection(), model.selection(), 0, model.selection("FACE", "ExtrusionCut_4_1/Modified_Face&Sketch_1/SketchLine_5"), 0, [model.selection("SOLID", "ExtrusionCut_4_1")])
Sketch_10 = model.addSketch(Part_1_doc, model.selection("FACE", "ExtrusionCut_5_1/Modified_Face&Sketch_1/SketchLine_8"))
SketchCircle_5 = Sketch_10.addCircle(-6.000000000000002, 4.999999999999997, 5)
SketchLine_33 = Sketch_10.addLine(model.selection("EDGE", "[ExtrusionCut_2_1/Modified_Face&Extrusion_1_1/To_Face][ExtrusionCut_5_1/Modified_Face&Sketch_1/SketchLine_8]"))
SketchConstraintDistance_11 = Sketch_10.setDistance(SketchCircle_5.center(), SketchLine_33.result(), 30, False)
SketchLine_34 = Sketch_10.addLine(model.selection("EDGE", "[ExtrusionCut_5_1/Modified_Face&Sketch_1/SketchLine_9][ExtrusionCut_5_1/Modified_Face&Sketch_1/SketchLine_8]"))
SketchConstraintDistance_12 = Sketch_10.setDistance(SketchCircle_5.result(), SketchLine_34.result(), 9, False)
SketchConstraintRadius_7 = Sketch_10.setRadius(SketchCircle_5.results()[1], "10/2")
model.do()
ExtrusionCut_6 = model.addExtrusionCut(Part_1_doc, [model.selection("FACE", "Sketch_10/Face-SketchCircle_5_2r")], model.selection(), model.selection(), 0, model.selection("FACE", "Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_1"), 0, [model.selection("SOLID", "ExtrusionCut_5_1")])
model.do()

# Test reexecution after parameter change
Parameter_A.setValue(62)
model.do()
model.testResultsVolumes(ExtrusionCut_6, [141392.1882461419])
Parameter_A.setValue(63)
model.do()

from GeomAPI import GeomAPI_Shape

model.testNbResults(ExtrusionCut_6, 1)
model.testNbSubResults(ExtrusionCut_6, [0])
model.testNbSubShapes(ExtrusionCut_6, GeomAPI_Shape.SOLID, [1])
model.testNbSubShapes(ExtrusionCut_6, GeomAPI_Shape.FACE, [37])
model.testNbSubShapes(ExtrusionCut_6, GeomAPI_Shape.EDGE, [192])
model.testNbSubShapes(ExtrusionCut_6, GeomAPI_Shape.VERTEX, [384])
model.testResultsVolumes(ExtrusionCut_6, [144033.64842978157685])

model.end()

assert(model.checkPythonDump())
