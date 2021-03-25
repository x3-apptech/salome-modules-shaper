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

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
model.addParameter(Part_1_doc, "angle2", "85")
model.addParameter(Part_1_doc, "inclinaison", "-19")
model.addParameter(Part_1_doc, "angle1", "30")
model.addParameter(Part_1_doc, "l_percage", "50")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))
SketchLine_1 = Sketch_1.addLine(0, 263.502735943608, 0, -36.43621897530445)
SketchLine_1.setAuxiliary(True)
SketchPoint_1 = Sketch_1.addPoint(model.selection("VERTEX", "PartSet/Origin"))
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.result(), SketchPoint_1.coordinates())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_1.result())
SketchLine_2 = Sketch_1.addLine(-90, 0, -90, 29.20204102886729)
SketchLine_3 = Sketch_1.addLine(-90, 29.20204102886729, -85, 29.20204102886729)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_2.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(-85, 29.20204102886729, -85, 118.2020410288673)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(-85, 118.2020410288673, -90, 128)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchLine_6 = Sketch_1.addLine(-90, 128, -90, 183)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchLine_7 = Sketch_1.addLine(-90, 183, -80, 183)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.startPoint())
SketchLine_8 = Sketch_1.addLine(-80, 183, -80, 190)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_8.startPoint())
SketchLine_9 = Sketch_1.addLine(-80, 190, -70, 190)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_8.endPoint(), SketchLine_9.startPoint())
SketchLine_10 = Sketch_1.addLine(-70, 190, -70, 0)
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_9.endPoint(), SketchLine_10.startPoint())
SketchLine_11 = Sketch_1.addLine(-70, 0, -90, 0)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_10.endPoint(), SketchLine_11.startPoint())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_9.result())
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchPoint_1.coordinates(), SketchLine_10.result(), 70, False)
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_10.result())
SketchConstraintVertical_3 = Sketch_1.setVertical(SketchLine_2.result())
SketchConstraintVertical_4 = Sketch_1.setVertical(SketchLine_4.result())
SketchConstraintVertical_5 = Sketch_1.setVertical(SketchLine_6.result())
SketchConstraintVertical_6 = Sketch_1.setVertical(SketchLine_8.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_11.result())
SketchConstraintHorizontal_2.setName("SketchConstraintHorizontal_3")
SketchConstraintHorizontal_3 = Sketch_1.setHorizontal(SketchLine_3.result())
SketchConstraintHorizontal_3.setName("SketchConstraintHorizontal_4")
SketchConstraintHorizontal_4 = Sketch_1.setHorizontal(SketchLine_7.result())
SketchConstraintHorizontal_4.setName("SketchConstraintHorizontal_5")
SketchConstraintDistance_2 = Sketch_1.setDistance(SketchLine_9.startPoint(), SketchLine_1.result(), 80, False)
SketchConstraintDistance_3 = Sketch_1.setDistance(SketchLine_5.endPoint(), SketchLine_1.result(), 90, False)
SketchConstraintDistance_4 = Sketch_1.setDistance(SketchLine_3.endPoint(), SketchLine_1.result(), 85, False)
SketchConstraintDistance_5 = Sketch_1.setDistance(SketchLine_2.endPoint(), SketchLine_1.result(), 90, False)
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchLine_11.endPoint(), SketchLine_2.startPoint())
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_10.result(), 190)
SketchConstraintLength_1.setName("SketchConstraintLength_3")
SketchConstraintLength_2 = Sketch_1.setLength(SketchLine_5.result(), 11)
SketchConstraintLength_2.setName("SketchConstraintLength_4")
SketchConstraintLength_3 = Sketch_1.setLength(SketchLine_4.result(), 89)
SketchConstraintLength_3.setName("SketchConstraintLength_5")
SketchLine_12 = Sketch_1.addLine(model.selection("EDGE", "PartSet/OX"))
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchLine_11.startPoint(), SketchLine_12.result())
SketchConstraintLength_4 = Sketch_1.setLength(SketchLine_8.result(), 7)
SketchConstraintLength_4.setName("SketchConstraintLength_6")
SketchConstraintLength_5 = Sketch_1.setLength(SketchLine_6.result(), 55)
SketchConstraintLength_5.setName("SketchConstraintLength_7")
SketchLine_13 = Sketch_1.addLine(-130.660278375518, 118.202041028867, 110.462869194241, 118.202041028867)
SketchLine_13.setName("SketchLine_19")
SketchLine_13.result().setName("SketchLine_19")
SketchLine_13.setAuxiliary(True)
SketchConstraintHorizontal_5 = Sketch_1.setHorizontal(SketchLine_13.result())
SketchConstraintHorizontal_5.setName("SketchConstraintHorizontal_8")
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchLine_13.result(), SketchLine_4.endPoint())
SketchConstraintCoincidence_13.setName("SketchConstraintCoincidence_17")
model.do()
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_11r-SketchLine_10r-SketchLine_9r-SketchLine_8r-SketchLine_7r-SketchLine_6r-SketchLine_5r-SketchLine_4r-SketchLine_3r-SketchLine_2r")], model.selection("EDGE", "Sketch_1/SketchLine_1"), 180, 0)
Plane_4 = model.addPlane(Part_1_doc, model.selection("FACE", "Revolution_1_1/From_Face"), 120, True)
Sketch_2 = model.addSketch(Part_1_doc, model.selection("FACE", "Plane_1"))
SketchProjection_1 = Sketch_2.addProjection(model.selection("EDGE", "Sketch_1/SketchLine_1"), False)
SketchLine_14 = SketchProjection_1.createdFeature()
SketchLine_14.setName("SketchLine_14")
SketchLine_14.result().setName("SketchLine_14")
SketchProjection_2 = Sketch_2.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_15 = SketchProjection_2.createdFeature()
SketchLine_15.setName("SketchLine_15")
SketchLine_15.result().setName("SketchLine_15")
SketchLine_16 = Sketch_2.addLine(-15, 44.202041028867, 15, 44.20204102886701)
SketchLine_16.setName("SketchLine_24")
SketchLine_16.result().setName("SketchLine_24")
SketchLine_17 = Sketch_2.addLine(-25, 88.20204102886699, -25, 54.202041028867)
SketchLine_17.setName("SketchLine_28")
SketchLine_17.result().setName("SketchLine_28")
SketchLine_18 = Sketch_2.addLine(25, 54.20204102886701, 25, 88.202041028867)
SketchLine_18.setName("SketchLine_26")
SketchLine_18.result().setName("SketchLine_26")
SketchLine_19 = Sketch_2.addLine(15, 98.202041028867, -15.00000000000001, 98.20204102886699)
SketchLine_19.setName("SketchLine_27")
SketchLine_19.result().setName("SketchLine_27")
SketchLine_20 = Sketch_2.addLine(25, 98.20204102886706, -25, 98.20204102886706)
SketchLine_20.setName("SketchLine_15")
SketchLine_20.result().setName("SketchLine_15")
SketchLine_20.setAuxiliary(True)
SketchLine_21 = Sketch_2.addLine(25, 44.20204102886705, 25, 98.20204102886706)
SketchLine_21.setName("SketchLine_18")
SketchLine_21.result().setName("SketchLine_18")
SketchLine_21.setAuxiliary(True)
SketchConstraintCoincidence_14 = Sketch_2.setCoincident(SketchLine_21.endPoint(), SketchLine_20.startPoint())
SketchConstraintCoincidence_14.setName("SketchConstraintCoincidence_13")
SketchLine_22 = Sketch_2.addLine(-25, 98.20204102886706, -25, 44.20204102886705)
SketchLine_22.setName("SketchLine_16")
SketchLine_22.result().setName("SketchLine_16")
SketchLine_22.setAuxiliary(True)
SketchConstraintCoincidence_15 = Sketch_2.setCoincident(SketchLine_20.endPoint(), SketchLine_22.startPoint())
SketchConstraintCoincidence_15.setName("SketchConstraintCoincidence_14")
SketchLine_23 = Sketch_2.addLine(-25, 44.20204102886705, 25, 44.20204102886705)
SketchLine_23.setName("SketchLine_17")
SketchLine_23.result().setName("SketchLine_17")
SketchLine_23.setAuxiliary(True)
SketchConstraintCoincidence_16 = Sketch_2.setCoincident(SketchLine_22.endPoint(), SketchLine_23.startPoint())
SketchConstraintCoincidence_16.setName("SketchConstraintCoincidence_15")
SketchConstraintCoincidence_17 = Sketch_2.setCoincident(SketchLine_23.endPoint(), SketchLine_21.startPoint())
SketchConstraintCoincidence_17.setName("SketchConstraintCoincidence_16")
SketchConstraintHorizontal_6 = Sketch_2.setHorizontal(SketchLine_20.result())
SketchConstraintHorizontal_6.setName("SketchConstraintHorizontal_6")
SketchConstraintVertical_7 = Sketch_2.setVertical(SketchLine_22.result())
SketchConstraintHorizontal_7 = Sketch_2.setHorizontal(SketchLine_23.result())
SketchConstraintHorizontal_7.setName("SketchConstraintHorizontal_7")
SketchConstraintVertical_8 = Sketch_2.setVertical(SketchLine_21.result())
SketchConstraintLength_6 = Sketch_2.setLength(SketchLine_20.result(), "l_percage")
SketchConstraintLength_6.setName("SketchConstraintLength_8")
SketchConstraintLength_7 = Sketch_2.setLength(SketchLine_21.result(), 54)
SketchConstraintLength_7.setName("SketchConstraintLength_9")
SketchConstraintDistance_6 = Sketch_2.setDistance(SketchLine_14.result(), SketchLine_22.startPoint(), "l_percage/2", False)
SketchProjection_3 = Sketch_2.addProjection(model.selection("EDGE", "Sketch_1/SketchLine_19"), False)
SketchLine_24 = SketchProjection_3.createdFeature()
SketchLine_24.setName("SketchLine_25")
SketchLine_24.result().setName("SketchLine_25")
SketchConstraintDistance_7 = Sketch_2.setDistance(SketchLine_24.result(), SketchLine_20.startPoint(), 20, False)
SketchArc_1 = Sketch_2.addArc(-15, 54.202041028867, -25, 54.202041028867, -15, 44.202041028867, False)
SketchLine_25 = Sketch_2.addLine(-25, 98.20204102886706, -25, 54.202041028867)
SketchLine_25.setName("SketchLine_22")
SketchLine_25.result().setName("SketchLine_22")
SketchLine_25.setAuxiliary(True)
SketchConstraintCoincidence_18 = Sketch_2.setCoincident(SketchArc_1.startPoint(), SketchLine_25.endPoint())
SketchLine_26 = Sketch_2.addLine(-15, 44.202041028867, 25, 44.20204102886705)
SketchLine_26.setName("SketchLine_21")
SketchLine_26.result().setName("SketchLine_21")
SketchLine_26.setAuxiliary(True)
SketchConstraintCoincidence_19 = Sketch_2.setCoincident(SketchArc_1.endPoint(), SketchLine_26.startPoint())
SketchConstraintTangent_1 = Sketch_2.setTangent(SketchArc_1.results()[1], SketchLine_26.result())
SketchConstraintTangent_2 = Sketch_2.setTangent(SketchArc_1.results()[1], SketchLine_25.result())
SketchConstraintCoincidence_20 = Sketch_2.setCoincident(SketchLine_23.endPoint(), SketchLine_26.endPoint())
SketchConstraintCoincidence_21 = Sketch_2.setCoincident(SketchLine_22.startPoint(), SketchLine_25.startPoint())
SketchConstraintCoincidence_22 = Sketch_2.setCoincident(SketchLine_26.startPoint(), SketchLine_23.result())
SketchConstraintCoincidence_23 = Sketch_2.setCoincident(SketchLine_25.endPoint(), SketchLine_22.result())
SketchArc_2 = Sketch_2.addArc(15, 54.20204102886701, 15, 44.20204102886701, 25, 54.20204102886701, False)
SketchConstraintCoincidence_24 = Sketch_2.setCoincident(SketchArc_2.startPoint(), SketchLine_16.endPoint())
SketchLine_27 = Sketch_2.addLine(25, 54.20204102886701, 25, 98.20204102886706)
SketchLine_27.setName("SketchLine_23")
SketchLine_27.result().setName("SketchLine_23")
SketchLine_27.setAuxiliary(True)
SketchConstraintCoincidence_25 = Sketch_2.setCoincident(SketchArc_2.endPoint(), SketchLine_27.startPoint())
SketchConstraintTangent_3 = Sketch_2.setTangent(SketchArc_2.results()[1], SketchLine_27.result())
SketchConstraintTangent_4 = Sketch_2.setTangent(SketchArc_2.results()[1], SketchLine_16.result())
SketchConstraintCoincidence_26 = Sketch_2.setCoincident(SketchLine_21.endPoint(), SketchLine_27.endPoint())
SketchConstraintCoincidence_27 = Sketch_2.setCoincident(SketchLine_26.startPoint(), SketchLine_16.startPoint())
SketchConstraintCoincidence_28 = Sketch_2.setCoincident(SketchLine_27.startPoint(), SketchLine_21.result())
SketchConstraintCoincidence_29 = Sketch_2.setCoincident(SketchLine_16.endPoint(), SketchLine_26.result())
SketchArc_3 = Sketch_2.addArc(15, 88.202041028867, 25, 88.202041028867, 15, 98.202041028867, False)
SketchConstraintCoincidence_30 = Sketch_2.setCoincident(SketchArc_3.startPoint(), SketchLine_18.endPoint())
SketchLine_28 = Sketch_2.addLine(15, 98.202041028867, -25, 98.20204102886706)
SketchLine_28.setName("SketchLine_25")
SketchLine_28.result().setName("SketchLine_25")
SketchLine_28.setAuxiliary(True)
SketchConstraintCoincidence_31 = Sketch_2.setCoincident(SketchArc_3.endPoint(), SketchLine_28.startPoint())
SketchConstraintTangent_5 = Sketch_2.setTangent(SketchArc_3.results()[1], SketchLine_28.result())
SketchConstraintTangent_6 = Sketch_2.setTangent(SketchArc_3.results()[1], SketchLine_18.result())
SketchConstraintCoincidence_32 = Sketch_2.setCoincident(SketchLine_20.endPoint(), SketchLine_28.endPoint())
SketchConstraintCoincidence_33 = Sketch_2.setCoincident(SketchLine_27.startPoint(), SketchLine_18.startPoint())
SketchConstraintCoincidence_34 = Sketch_2.setCoincident(SketchLine_28.startPoint(), SketchLine_20.result())
SketchConstraintCoincidence_35 = Sketch_2.setCoincident(SketchLine_18.endPoint(), SketchLine_27.result())
SketchArc_4 = Sketch_2.addArc(-15, 88.20204102886699, -15.00000000000001, 98.20204102886699, -25, 88.20204102886699, False)
SketchConstraintCoincidence_36 = Sketch_2.setCoincident(SketchArc_4.startPoint(), SketchLine_19.endPoint())
SketchConstraintCoincidence_37 = Sketch_2.setCoincident(SketchArc_4.endPoint(), SketchLine_17.startPoint())
SketchConstraintTangent_7 = Sketch_2.setTangent(SketchArc_4.results()[1], SketchLine_19.result())
SketchConstraintTangent_8 = Sketch_2.setTangent(SketchArc_4.results()[1], SketchLine_17.result())
SketchConstraintCoincidence_38 = Sketch_2.setCoincident(SketchLine_28.startPoint(), SketchLine_19.startPoint())
SketchConstraintCoincidence_39 = Sketch_2.setCoincident(SketchLine_25.endPoint(), SketchLine_17.endPoint())
SketchConstraintCoincidence_40 = Sketch_2.setCoincident(SketchLine_19.endPoint(), SketchLine_28.result())
SketchConstraintCoincidence_41 = Sketch_2.setCoincident(SketchLine_17.startPoint(), SketchLine_25.result())
SketchConstraintRadius_1 = Sketch_2.setRadius(SketchArc_3.results()[1], 10)
SketchConstraintRadius_2 = Sketch_2.setRadius(SketchArc_4.results()[1], 10)
SketchConstraintRadius_3 = Sketch_2.setRadius(SketchArc_2.results()[1], 10)
SketchConstraintRadius_4 = Sketch_2.setRadius(SketchArc_1.results()[1], 10)
model.do()
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("FACE", "Sketch_2/Face-SketchLine_24f-SketchArc_2_2f-SketchLine_26f-SketchArc_3_2f-SketchLine_27f-SketchArc_4_2f-SketchLine_28f-SketchArc_1_2f")], model.selection(), model.selection("FACE", "Revolution_1_1/To_Face"), -30, model.selection(), 0)
Rotation_1 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Extrusion_1_1")], model.selection("EDGE", "[Extrusion_1_1/Generated_Face&Sketch_2/SketchLine_24][Extrusion_1_1/From_Face]"), "inclinaison")
Rotation_2 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Rotation_1_1")], model.selection("EDGE", "PartSet/OZ"), "angle1")
Recover_1 = model.addRecover(Part_1_doc, Rotation_2, [Rotation_1.result()])
Rotation_3 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Recover_1_1")], model.selection("EDGE", "PartSet/OZ"), "angle2")
Rotation_3.setName("Rotation_4")
Rotation_3.result().setName("Rotation_4_1")
Recover_2 = model.addRecover(Part_1_doc, Rotation_2, [Rotation_1.result()])
Rotation_4 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Recover_2_1")], model.selection("EDGE", "PartSet/OZ"), "-angle2")
Rotation_4.setName("Rotation_5")
Rotation_4.result().setName("Rotation_5_1")
Recover_3 = model.addRecover(Part_1_doc, Rotation_2, [Rotation_1.result()])
Rotation_5 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Recover_3_1")], model.selection("EDGE", "PartSet/OZ"), "-angle1")
Rotation_5.setName("Rotation_6")
Rotation_5.result().setName("Rotation_6_1")
Cut_1_objects_2 = [model.selection("SOLID", "Rotation_4_1"), model.selection("SOLID", "Rotation_2_1"), model.selection("SOLID", "Rotation_6_1"), model.selection("SOLID", "Rotation_5_1")]
Cut_1 = model.addCut(Part_1_doc, [model.selection("SOLID", "Revolution_1_1")], Cut_1_objects_2)
model.do()
model.end()

assert(Cut_1.result().shapeType() == 'SOLID')
assert(Part_1_doc.size("Bodies") == 1)
