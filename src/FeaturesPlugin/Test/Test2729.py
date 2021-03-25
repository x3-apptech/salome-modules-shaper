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
model.addParameter(Part_1_doc, "B", "46")
model.addParameter(Part_1_doc, "B1", "36")
model.addParameter(Part_1_doc, "B2", "25.5")
model.addParameter(Part_1_doc, "B3", "10")
model.addParameter(Part_1_doc, "D", "4.2")
model.addParameter(Part_1_doc, "D1", "4")
model.addParameter(Part_1_doc, "D2", "0")
model.addParameter(Part_1_doc, "H", "95")
model.addParameter(Part_1_doc, "H1", "86")
model.addParameter(Part_1_doc, "H2", "4.7")
model.addParameter(Part_1_doc, "H4", "86")
model.addParameter(Part_1_doc, "H5", "71")
model.addParameter(Part_1_doc, "H6", "40")
model.addParameter(Part_1_doc, "H7", "26.75")
model.addParameter(Part_1_doc, "H8", "51.5")
model.addParameter(Part_1_doc, "H9", "15")
model.addParameter(Part_1_doc, "H10", "1.5")
model.addParameter(Part_1_doc, "L1", "23")
model.addParameter(Part_1_doc, "L2", "30")
model.addParameter(Part_1_doc, "L3", "4")
model.addParameter(Part_1_doc, "L4", "2.3")
model.addParameter(Part_1_doc, "L5", "6.5")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))
SketchLine_1 = Sketch_1.addLine(29.99999999999995, 0, -16.00000000000005, 0)
SketchLine_2 = Sketch_1.addLine(-16.00000000000005, 0, -16.00000000000005, 4.7)
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.startPoint())
SketchLine_3 = Sketch_1.addLine(-16.00000000000005, 4.7, 1.946471204593173e-17, 4.7)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_2.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(1.946471204593173e-17, 4.7, 4.235164736271502e-22, 95)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(4.235164736271502e-22, 95, 7, 95)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchLine_6 = Sketch_1.addLine(7, 95, 7, 80)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchLine_7 = Sketch_1.addLine(7, 80, 9, 80)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.startPoint())
SketchLine_8 = Sketch_1.addLine(9, 80, 9, 8.999999999999996)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_8.startPoint())
SketchLine_9 = Sketch_1.addLine(9, 8.999999999999996, 17, 9.000000000000002)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_8.endPoint(), SketchLine_9.startPoint())
SketchLine_10 = Sketch_1.addLine(17, 9.000000000000002, 17, 4.7)
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_9.endPoint(), SketchLine_10.startPoint())
SketchLine_11 = Sketch_1.addLine(17, 4.7, 30, 4.7)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_10.endPoint(), SketchLine_11.startPoint())
SketchLine_12 = Sketch_1.addLine(30, 4.7, 29.99999999999995, 0)
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchLine_11.endPoint(), SketchLine_12.startPoint())
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_12.endPoint())
SketchProjection_1 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_13 = SketchProjection_1.createdFeature()
SketchConstraintCollinear_1 = Sketch_1.setCollinear(SketchLine_13.result(), SketchLine_1.result())
SketchProjection_2 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OZ"), False)
SketchLine_14 = SketchProjection_2.createdFeature()
SketchConstraintCollinear_2 = Sketch_1.setCollinear(SketchLine_4.result(), SketchLine_14.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_2.result())
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_6.result())
SketchConstraintVertical_3 = Sketch_1.setVertical(SketchLine_8.result())
SketchConstraintVertical_4 = Sketch_1.setVertical(SketchLine_10.result())
SketchConstraintVertical_5 = Sketch_1.setVertical(SketchLine_12.result())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_3.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_9.result())
SketchConstraintHorizontal_3 = Sketch_1.setHorizontal(SketchLine_11.result())
SketchConstraintHorizontal_4 = Sketch_1.setHorizontal(SketchLine_5.result())
SketchConstraintHorizontal_5 = Sketch_1.setHorizontal(SketchLine_7.result())
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_2.result(), "H2")
SketchConstraintLength_2 = Sketch_1.setLength(SketchLine_1.result(), "B")
SketchConstraintDistanceHorizontal_1 = Sketch_1.setHorizontalDistance(SketchLine_3.endPoint(), SketchLine_1.startPoint(), "L2")
SketchConstraintDistanceHorizontal_2 = Sketch_1.setHorizontalDistance(SketchLine_5.endPoint(), SketchLine_11.endPoint(), "L1")
SketchConstraintLength_3 = Sketch_1.setLength(SketchLine_7.result(), 2)
SketchConstraintDistanceVertical_1 = Sketch_1.setVerticalDistance(SketchLine_6.startPoint(), SketchLine_1.startPoint(), "H")
SketchConstraintDistanceVertical_2 = Sketch_1.setVerticalDistance(SketchLine_8.startPoint(), SketchLine_9.endPoint(), "H5")
SketchConstraintDistanceVertical_3 = Sketch_1.setVerticalDistance(SketchLine_6.startPoint(), SketchLine_9.endPoint(), "H1")
SketchConstraintLength_4 = Sketch_1.setLength(SketchLine_12.result(), "H2")
SketchConstraintLength_5 = Sketch_1.setLength(SketchLine_11.result(), "L5*2")
model.do()
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_12r-SketchLine_11r-SketchLine_10r-SketchLine_9r-SketchLine_8r-SketchLine_7r-SketchLine_6r-SketchLine_5r-SketchLine_4r-SketchLine_3r-SketchLine_2r-SketchLine_1r")], model.selection(), "B/2", "B/2")
ExtrusionCut_1 = model.addExtrusionCut(Part_1_doc, [], model.selection(), 0, "L5", [model.selection("SOLID", "Extrusion_1_1")])
Sketch_2 = model.addSketch(Part_1_doc, model.selection("FACE", "Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8"))
SketchLine_15 = Sketch_2.addLine(-8.985867605559861e-16, 8.999999999999996, 1.535230276239474e-15, 80)
SketchLine_15.setAuxiliary(True)
SketchProjection_3 = Sketch_2.addProjection(model.selection("EDGE", "[Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_7]"), False)
SketchLine_16 = SketchProjection_3.createdFeature()
SketchConstraintMiddle_1 = Sketch_2.setMiddlePoint(SketchLine_16.result(), SketchLine_15.endPoint())
SketchProjection_4 = Sketch_2.addProjection(model.selection("EDGE", "[Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_9][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8]"), False)
SketchLine_17 = SketchProjection_4.createdFeature()
SketchConstraintMiddle_2 = Sketch_2.setMiddlePoint(SketchLine_15.startPoint(), SketchLine_17.result())
SketchLine_18 = Sketch_2.addLine(-4.999999999999998, 13.00000000000012, -4.999999999999999, 94.99999999999999)
SketchLine_19 = Sketch_2.addLine(-4.999999999999999, 94.99999999999999, -13.00000000000988, 95.00000000000222)
SketchConstraintCoincidence_13 = Sketch_2.setCoincident(SketchLine_18.endPoint(), SketchLine_19.startPoint())
SketchLine_20 = Sketch_2.addLine(-13.00000000000988, 95.00000000000222, -13, 78.5)
SketchConstraintCoincidence_14 = Sketch_2.setCoincident(SketchLine_19.endPoint(), SketchLine_20.startPoint())
SketchLine_21 = Sketch_2.addLine(-13, 78.5, -23.00000000000001, 78.5)
SketchConstraintCoincidence_15 = Sketch_2.setCoincident(SketchLine_20.endPoint(), SketchLine_21.startPoint())
SketchLine_22 = Sketch_2.addLine(-23.00000000000001, 78.5, -23.00000000000001, 72.5)
SketchConstraintCoincidence_16 = Sketch_2.setCoincident(SketchLine_21.endPoint(), SketchLine_22.startPoint())
SketchLine_23 = Sketch_2.addLine(-23.00000000000001, 72.5, -15, 72.5)
SketchConstraintCoincidence_17 = Sketch_2.setCoincident(SketchLine_22.endPoint(), SketchLine_23.startPoint())
SketchLine_24 = Sketch_2.addLine(-13, 70.5, -13, 23)
SketchLine_25 = Sketch_2.addLine(-15, 21, -22.99999999999999, 21)
SketchLine_26 = Sketch_2.addLine(-22.99999999999999, 21, -22.99999999999999, 15)
SketchConstraintCoincidence_18 = Sketch_2.setCoincident(SketchLine_25.endPoint(), SketchLine_26.startPoint())
SketchConstraintCoincidence_18.setName("SketchConstraintCoincidence_20")
SketchLine_27 = Sketch_2.addLine(-22.99999999999999, 15, -13, 15)
SketchConstraintCoincidence_19 = Sketch_2.setCoincident(SketchLine_26.endPoint(), SketchLine_27.startPoint())
SketchConstraintCoincidence_19.setName("SketchConstraintCoincidence_21")
SketchLine_28 = Sketch_2.addLine(-13, 15, -13.00000000000024, 13.00000000000012)
SketchConstraintCoincidence_20 = Sketch_2.setCoincident(SketchLine_27.endPoint(), SketchLine_28.startPoint())
SketchConstraintCoincidence_20.setName("SketchConstraintCoincidence_22")
SketchArc_1 = Sketch_2.addArc(-9.000000000000117, 13.00000000000012, -4.999999999999998, 13.00000000000012, -13.00000000000024, 13.00000000000012, True)
SketchConstraintCoincidence_21 = Sketch_2.setCoincident(SketchLine_18.startPoint(), SketchArc_1.startPoint())
SketchConstraintCoincidence_21.setName("SketchConstraintCoincidence_23")
SketchConstraintTangent_1 = Sketch_2.setTangent(SketchLine_18.result(), SketchArc_1.results()[1])
SketchConstraintCoincidence_22 = Sketch_2.setCoincident(SketchLine_28.endPoint(), SketchArc_1.endPoint())
SketchConstraintCoincidence_22.setName("SketchConstraintCoincidence_24")
SketchConstraintTangent_2 = Sketch_2.setTangent(SketchLine_28.result(), SketchArc_1.results()[1])
SketchConstraintVertical_6 = Sketch_2.setVertical(SketchLine_18.result())
SketchConstraintVertical_7 = Sketch_2.setVertical(SketchLine_28.result())
SketchConstraintCollinear_3 = Sketch_2.setCollinear(SketchLine_28.result(), SketchLine_24.result())
SketchConstraintCollinear_4 = Sketch_2.setCollinear(SketchLine_20.result(), SketchLine_24.result())
SketchConstraintHorizontal_6 = Sketch_2.setHorizontal(SketchLine_27.result())
SketchConstraintHorizontal_7 = Sketch_2.setHorizontal(SketchLine_25.result())
SketchConstraintHorizontal_8 = Sketch_2.setHorizontal(SketchLine_23.result())
SketchConstraintHorizontal_9 = Sketch_2.setHorizontal(SketchLine_21.result())
SketchProjection_5 = Sketch_2.addProjection(model.selection("EDGE", "[Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_8][Extrusion_1_1/To_Face]"), False)
SketchLine_29 = SketchProjection_5.createdFeature()
SketchConstraintCollinear_5 = Sketch_2.setCollinear(SketchLine_22.result(), SketchLine_29.result())
SketchConstraintCollinear_5.setName("SketchConstraintCollinear_7")
SketchConstraintCollinear_6 = Sketch_2.setCollinear(SketchLine_26.result(), SketchLine_29.result())
SketchConstraintCollinear_6.setName("SketchConstraintCollinear_8")
SketchConstraintTangent_3 = Sketch_2.setTangent(SketchArc_1.results()[1], SketchLine_17.result())
SketchConstraintLength_6 = Sketch_2.setLength(SketchLine_22.result(), 6)
SketchConstraintLength_7 = Sketch_2.setLength(SketchLine_26.result(), 6)
SketchConstraintDistanceHorizontal_3 = Sketch_2.setHorizontalDistance(SketchLine_18.endPoint(), SketchLine_15.endPoint(), "B3/2")
SketchConstraintLength_8 = Sketch_2.setLength(SketchLine_19.result(), 8)
SketchConstraintLength_8.setName("SketchConstraintLength_9")
SketchArc_2 = Sketch_2.addArc(-15, 70.5, -13, 70.5, -15, 72.5, False)
SketchPoint_1 = Sketch_2.addPoint(-13, 72.5)
SketchPoint_1.setAuxiliary(True)
SketchConstraintCoincidence_23 = Sketch_2.setCoincident(SketchPoint_1.coordinates(), SketchLine_23.result())
SketchConstraintCoincidence_23.setName("SketchConstraintCoincidence_25")
SketchConstraintCoincidence_24 = Sketch_2.setCoincident(SketchPoint_1.coordinates(), SketchLine_24.result())
SketchConstraintCoincidence_24.setName("SketchConstraintCoincidence_26")
SketchConstraintCoincidence_25 = Sketch_2.setCoincident(SketchArc_2.startPoint(), SketchLine_24.startPoint())
SketchConstraintCoincidence_25.setName("SketchConstraintCoincidence_27")
SketchConstraintCoincidence_26 = Sketch_2.setCoincident(SketchArc_2.endPoint(), SketchLine_23.endPoint())
SketchConstraintCoincidence_26.setName("SketchConstraintCoincidence_28")
SketchConstraintTangent_4 = Sketch_2.setTangent(SketchArc_2.results()[1], SketchLine_23.result())
SketchConstraintTangent_5 = Sketch_2.setTangent(SketchArc_2.results()[1], SketchLine_24.result())
SketchArc_3 = Sketch_2.addArc(-15, 23, -15, 21, -13, 23, False)
SketchPoint_2 = Sketch_2.addPoint(-13, 21)
SketchPoint_2.setAuxiliary(True)
SketchConstraintDistance_1 = Sketch_2.setDistance(SketchPoint_1.coordinates(), SketchPoint_2.coordinates(), "H8", False)
SketchConstraintCoincidence_27 = Sketch_2.setCoincident(SketchPoint_2.coordinates(), SketchLine_25.result())
SketchConstraintCoincidence_27.setName("SketchConstraintCoincidence_29")
SketchConstraintCoincidence_28 = Sketch_2.setCoincident(SketchPoint_2.coordinates(), SketchLine_24.result())
SketchConstraintCoincidence_28.setName("SketchConstraintCoincidence_30")
SketchConstraintCoincidence_29 = Sketch_2.setCoincident(SketchArc_3.startPoint(), SketchLine_25.startPoint())
SketchConstraintCoincidence_29.setName("SketchConstraintCoincidence_31")
SketchConstraintCoincidence_30 = Sketch_2.setCoincident(SketchArc_3.endPoint(), SketchLine_24.endPoint())
SketchConstraintCoincidence_30.setName("SketchConstraintCoincidence_32")
SketchConstraintTangent_6 = Sketch_2.setTangent(SketchArc_3.results()[1], SketchLine_25.result())
SketchConstraintTangent_7 = Sketch_2.setTangent(SketchArc_3.results()[1], SketchLine_24.result())
SketchConstraintRadius_1 = Sketch_2.setRadius(SketchArc_3.results()[1], 2)
SketchConstraintRadius_2 = Sketch_2.setRadius(SketchArc_2.results()[1], 2)
SketchLine_30 = Sketch_2.addLine(13.00000000000001, 78.49999999999866, 23, 78.49999999999866)
SketchLine_30.setName("SketchLine_32")
SketchLine_30.result().setName("SketchLine_32")
SketchLine_31 = Sketch_2.addLine(23, 78.49999999999866, 23, 72.49999999999866)
SketchLine_31.setName("SketchLine_33")
SketchLine_31.result().setName("SketchLine_33")
SketchLine_32 = Sketch_2.addLine(23, 72.49999999999866, 14.99999999999993, 72.5)
SketchLine_32.setName("SketchLine_34")
SketchLine_32.result().setName("SketchLine_34")
SketchArc_4 = Sketch_2.addArc(14.99999999999997, 70.5, 12.99999999999997, 70.50000000000004, 14.99999999999993, 72.5, True)
SketchLine_33 = Sketch_2.addLine(12.99999999999997, 70.50000000000004, 12.99999999999995, 22.99999999999997)
SketchLine_33.setName("SketchLine_35")
SketchLine_33.result().setName("SketchLine_35")
SketchArc_5 = Sketch_2.addArc(14.99999999999996, 23.00000000000001, 14.99999999999992, 21, 12.99999999999995, 22.99999999999997, True)
SketchLine_34 = Sketch_2.addLine(14.99999999999992, 21, 23, 20.99999999999866)
SketchLine_34.setName("SketchLine_36")
SketchLine_34.result().setName("SketchLine_36")
SketchLine_35 = Sketch_2.addLine(23, 20.99999999999866, 23, 14.99999999999866)
SketchLine_35.setName("SketchLine_37")
SketchLine_35.result().setName("SketchLine_37")
SketchLine_36 = Sketch_2.addLine(23, 14.99999999999866, 13.00000000000001, 14.99999999999866)
SketchLine_36.setName("SketchLine_38")
SketchLine_36.result().setName("SketchLine_38")
SketchLine_37 = Sketch_2.addLine(13.00000000000001, 14.99999999999866, 13.00000000000024, 13.00000000000011)
SketchLine_37.setName("SketchLine_39")
SketchLine_37.result().setName("SketchLine_39")
SketchArc_6 = Sketch_2.addArc(9.000000000000117, 13.00000000000012, 4.999999999999996, 13.00000000000012, 13.00000000000024, 13.00000000000011, False)
SketchProjection_6 = Sketch_2.addProjection(model.selection("EDGE", "[Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_6][Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_5]"), True)
SketchLine_38 = SketchProjection_6.createdFeature()
SketchLine_38.setName("SketchLine_41")
SketchLine_38.result().setName("SketchLine_41")
SketchConstraintCollinear_7 = Sketch_2.setCollinear(SketchLine_19.result(), SketchLine_38.result())
SketchConstraintCollinear_7.setName("SketchConstraintCollinear_9")
SketchConstraintDistanceVertical_4 = Sketch_2.setVerticalDistance(SketchAPI_Line(SketchLine_29).startPoint(), SketchLine_22.startPoint(), "H10")
SketchConstraintMirror_1_objects = [SketchLine_18.result(), SketchLine_20.result(), SketchLine_19.result()]
SketchConstraintMirror_1 = Sketch_2.addMirror(SketchLine_15.result(), SketchConstraintMirror_1_objects)
[SketchLine_39, SketchLine_40, SketchLine_41] = SketchConstraintMirror_1.mirrored()
SketchLine_41.setName("SketchLine_43")
SketchLine_41.result().setName("SketchLine_43")
SketchLine_40.setName("SketchLine_42")
SketchLine_40.result().setName("SketchLine_42")
SketchLine_39.setName("SketchLine_40")
SketchLine_39.result().setName("SketchLine_40")
ExtrusionCut_1.setNestedSketch(Sketch_2)
ExtrusionCut_2 = model.addExtrusionCut(Part_1_doc, [], model.selection(), 0, 20, [model.selection("SOLID", "ExtrusionCut_1_1")])
Sketch_3 = model.addSketch(Part_1_doc, model.selection("FACE", "(ExtrusionCut_1_1/Modified_Face&Extrusion_1_1/To_Face)(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchLine_23)(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchLine_25)(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchArc_2_2)(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchLine_24)(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchArc_3_2)(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_12)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_11)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_1)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_10)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_2)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_3)2(Extrusion_1_1/Generated_Face&Sketch_1/SketchLine_4)2(ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_5)2(ExtrusionCut_1_1/Modified_Face&Sketch_1/SketchLine_9)2(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchLine_21)2(ExtrusionCut_1_1/From_Face_1)2(ExtrusionCut_1_1/Generated_Face&Sketch_2/SketchLine_27)2"))
SketchCircle_1 = Sketch_3.addCircle(-19.86097902097902, 65.28094405594405, 2.5)
SketchCircle_2 = Sketch_3.addCircle(-18.51902097902098, 27.57192307692308, 2)
SketchCircle_3 = Sketch_3.addCircle(-19.0558041958042, 85.54451048951049, 3)
SketchConstraintRadius_3 = Sketch_3.setRadius(SketchCircle_2.results()[1], 2)
SketchConstraintRadius_4 = Sketch_3.setRadius(SketchCircle_1.results()[1], 2.5)
SketchConstraintRadius_5 = Sketch_3.setRadius(SketchCircle_3.results()[1], 3)
ExtrusionCut_2.setNestedSketch(Sketch_3)
model.end()

# check that radius constraint is valid
from ModelAPI import *
aFactory = ModelAPI_Session.get().validators()
assert(aFactory.validate(SketchConstraintRadius_3.feature()))

assert(model.checkPythonDump())
