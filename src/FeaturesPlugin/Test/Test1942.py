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
Point_2 = model.addPoint(partSet, 100, 40, 70)
Axis_4 = model.addAxis(partSet, model.selection("VERTEX", "Origin"), model.selection("VERTEX", "Point_2"))
Plane_4 = model.addPlane(partSet, model.selection("EDGE", "Axis_4"), model.selection("VERTEX", "Point_2"), True)
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
model.addParameter(Part_1_doc, "a", "100")
Sketch_1 = model.addSketch(Part_1_doc, model.selection("FACE", "PartSet/Plane_4"))
SketchLine_1 = Sketch_1.addLine(-5.024014514861164, -46.57477619916804, -15.03025649976126, -42.91660461599292)
SketchLine_1.setName("SketchLine_9")
SketchLine_1.result().setName("SketchLine_9")
SketchLine_2 = Sketch_1.addLine(-34.15308715195452, -20.5925185611531, -39.51293324824182, 6.356619200673476)
SketchLine_2.setName("SketchLine_10")
SketchLine_2.result().setName("SketchLine_10")
SketchLine_3 = Sketch_1.addLine(-5.024014514861164, -46.57477619916804, -30.86442523156243, -37.12780736470863)
SketchLine_3.setName("SketchLine_1")
SketchLine_3.result().setName("SketchLine_1")
SketchLine_3.setAuxiliary(True)
SketchLine_4 = Sketch_1.addLine(-30.86442523156243, -37.12780736470863, -39.51293324824182, 6.356619200673476)
SketchLine_4.setName("SketchLine_2")
SketchLine_4.result().setName("SketchLine_2")
SketchLine_4.setAuxiliary(True)
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(-21.88471935002519, 39.79243387785534, 5.516674902531107, 51.5099134867221)
SketchLine_5.setName("SketchLine_4")
SketchLine_5.result().setName("SketchLine_4")
SketchLine_6 = Sketch_1.addLine(5.516674902531107, 51.5099134867221, 32.11790618078412, 35.7788047045564)
SketchLine_6.setName("SketchLine_5")
SketchLine_6.result().setName("SketchLine_5")
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchConstraintCoincidence_2.setName("SketchConstraintCoincidence_4")
SketchLine_7 = Sketch_1.addLine(32.11790618078412, 35.7788047045564, 51.23593736581751, -10.42185420425237)
SketchLine_7.setName("SketchLine_6")
SketchLine_7.result().setName("SketchLine_6")
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.startPoint())
SketchConstraintCoincidence_3.setName("SketchConstraintCoincidence_5")
SketchLine_8 = Sketch_1.addLine(51.23593736581751, -10.42185420425237, 33.52471395405271, -30.09043472300023)
SketchLine_8.setName("SketchLine_7")
SketchLine_8.result().setName("SketchLine_7")
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_8.startPoint())
SketchConstraintCoincidence_4.setName("SketchConstraintCoincidence_6")
SketchLine_9 = Sketch_1.addLine(33.52471395405271, -30.09043472300023, -5.024014514861164, -46.57477619916804)
SketchLine_9.setName("SketchLine_8")
SketchLine_9.result().setName("SketchLine_8")
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_8.endPoint(), SketchLine_9.startPoint())
SketchConstraintCoincidence_5.setName("SketchConstraintCoincidence_7")
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_3.startPoint(), SketchLine_9.endPoint())
SketchConstraintCoincidence_6.setName("SketchConstraintCoincidence_8")
SketchArc_1 = Sketch_1.addArc(-4.72938863253667, -14.74051331180418, -34.15308715195452, -20.5925185611531, -15.03025649976126, -42.91660461599292, False)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchArc_1.startPoint(), SketchLine_2.startPoint())
SketchConstraintCoincidence_7.setName("SketchConstraintCoincidence_9")
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchArc_1.endPoint(), SketchLine_1.endPoint())
SketchConstraintCoincidence_8.setName("SketchConstraintCoincidence_10")
SketchConstraintTangent_1 = Sketch_1.setTangent(SketchArc_1.results()[1], SketchLine_1.result())
SketchConstraintTangent_2 = Sketch_1.setTangent(SketchArc_1.results()[1], SketchLine_2.result())
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_3.startPoint(), SketchLine_1.startPoint())
SketchConstraintCoincidence_9.setName("SketchConstraintCoincidence_11")
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_2.endPoint())
SketchConstraintCoincidence_10.setName("SketchConstraintCoincidence_12")
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_3.result())
SketchConstraintCoincidence_11.setName("SketchConstraintCoincidence_13")
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchLine_2.startPoint(), SketchLine_4.result())
SketchConstraintCoincidence_12.setName("SketchConstraintCoincidence_14")
SketchConstraintRadius_1 = Sketch_1.setRadius(SketchArc_1.results()[1], 30)
SketchArc_2 = Sketch_1.addArc(-10.08923472882396, 12.20862445002239, -39.51293324824182, 6.356619200673476, -21.88471935002519, 39.79243387785534, True)
SketchConstraintRadius_2 = Sketch_1.setRadius(SketchArc_2.results()[1], 30)
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchArc_2.startPoint(), SketchLine_2.endPoint())
SketchConstraintCoincidence_13.setName("SketchConstraintCoincidence_15")
SketchConstraintTangent_3 = Sketch_1.setTangent(SketchArc_2.results()[1], SketchLine_2.result())
SketchConstraintCoincidence_14 = Sketch_1.setCoincident(SketchLine_5.startPoint(), SketchArc_2.endPoint())
SketchConstraintCoincidence_14.setName("SketchConstraintCoincidence_16")
SketchConstraintTangent_4 = Sketch_1.setTangent(SketchLine_5.result(), SketchArc_2.results()[1])
SketchConstraintParallel_1 = Sketch_1.setParallel(SketchLine_5.result(), SketchLine_9.result())
SketchCircle_1 = Sketch_1.addCircle(2.333185352801025, 5.209035955269018, 34.7006180228472)
SketchCircle_1.setName("SketchCircle_2")
SketchCircle_1.result().setName("SketchCircle_2")
SketchCircle_1.results()[1].setName("SketchCircle_2_2")
SketchCircle_1.setAuxiliary(True)
SketchArc_3 = Sketch_1.addArc(2.333185352801025, 5.209035955269018, 9.336485740095949, 23.71540254943274, 4.745637396078732, -14.43051332599376, True)
SketchArc_3.setName("SketchArc_4")
SketchArc_3.result().setName("SketchArc_4")
SketchArc_3.results()[1].setName("SketchArc_4_2")
SketchConstraintCoincidence_15 = Sketch_1.setCoincident(SketchArc_3.center(), SketchCircle_1.center())
SketchConstraintCoincidence_15.setName("SketchConstraintCoincidence_17")
SketchPoint_1 = Sketch_1.addPoint(-30.48122876352205, 16.49385395867538)
SketchConstraintCoincidence_16 = Sketch_1.setCoincident(SketchPoint_1.coordinates(), SketchCircle_1.results()[1])
SketchConstraintCoincidence_16.setName("SketchConstraintCoincidence_18")
SketchPoint_2 = Sketch_1.addPoint(33.38569114717736, 20.69692719513029)
SketchConstraintCoincidence_17 = Sketch_1.setCoincident(SketchPoint_2.coordinates(), SketchCircle_1.results()[1])
SketchConstraintCoincidence_17.setName("SketchConstraintCoincidence_19")
SketchPoint_3 = Sketch_1.addPoint(4.745637396078732, -14.43051332599376)
SketchConstraintCoincidence_18 = Sketch_1.setCoincident(SketchPoint_3.coordinates(), SketchArc_3.endPoint())
SketchConstraintCoincidence_18.setName("SketchConstraintCoincidence_20")
SketchPoint_4 = Sketch_1.addPoint(9.336485740095949, 23.71540254943274)
SketchConstraintCoincidence_19 = Sketch_1.setCoincident(SketchPoint_4.coordinates(), SketchArc_3.startPoint())
SketchConstraintCoincidence_19.setName("SketchConstraintCoincidence_21")
SketchArc_4 = Sketch_1.addArc(2.333185352801027, 5.209035955269019, 9.336485740095949, 23.71540254943274, 4.745637396078732, -14.43051332599376, False)
SketchArc_4.setName("SketchArc_3")
SketchArc_4.result().setName("SketchArc_3")
SketchArc_4.results()[1].setName("SketchArc_3_2")
SketchConstraintCoincidence_20 = Sketch_1.setCoincident(SketchArc_3.endPoint(), SketchArc_4.endPoint())
SketchConstraintCoincidence_20.setName("SketchConstraintCoincidence_22")
SketchConstraintCoincidence_21 = Sketch_1.setCoincident(SketchArc_3.startPoint(), SketchArc_4.startPoint())
SketchConstraintCoincidence_21.setName("SketchConstraintCoincidence_23")
SketchConstraintTangent_5 = Sketch_1.setTangent(SketchArc_4.results()[1], SketchArc_3.results()[1])
SketchConstraintCoincidence_22 = Sketch_1.setCoincident(SketchArc_4.endPoint(), SketchPoint_3.coordinates())
SketchConstraintCoincidence_22.setName("SketchConstraintCoincidence_24")
SketchConstraintCoincidence_23 = Sketch_1.setCoincident(SketchArc_4.startPoint(), SketchPoint_4.coordinates())
SketchConstraintCoincidence_23.setName("SketchConstraintCoincidence_25")
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_7.result(), "a/2")
model.do()
Sketch_1.result().setColor(85, 255, 0)
Sketch_2 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchPoint_5 = Sketch_2.addPoint(27.94100551732842, 60.77708330244815)
SketchPoint_5.setAuxiliary(True)
SketchArc_5 = Sketch_2.addArc(29.50596093963414, 45.389553391128, 31.03624924456759, 29.99853718200911, 27.94100551732842, 60.77708330244815, True)
SketchArc_5.setName("SketchArc_6")
SketchArc_5.result().setName("SketchArc_6")
SketchArc_5.results()[1].setName("SketchArc_6_2")
SketchConstraintCoincidence_24 = Sketch_2.setCoincident(SketchPoint_5.coordinates(), SketchArc_5.endPoint())
SketchConstraintCoincidence_24.setName("SketchConstraintCoincidence_26")
SketchPoint_6 = Sketch_2.addPoint(31.03624924456759, 29.99853718200911)
SketchPoint_6.setAuxiliary(True)
SketchConstraintCoincidence_25 = Sketch_2.setCoincident(SketchPoint_6.coordinates(), SketchArc_5.startPoint())
SketchConstraintCoincidence_25.setName("SketchConstraintCoincidence_27")
SketchArc_6 = Sketch_2.addArc(29.50596093963414, 45.389553391128, 31.03624924456759, 29.99853718200911, 27.94100551732842, 60.77708330244815, False)
SketchArc_6.setName("SketchArc_5")
SketchArc_6.result().setName("SketchArc_5")
SketchArc_6.results()[1].setName("SketchArc_5_2")
SketchConstraintCoincidence_26 = Sketch_2.setCoincident(SketchArc_5.endPoint(), SketchArc_6.endPoint())
SketchConstraintCoincidence_26.setName("SketchConstraintCoincidence_28")
SketchConstraintCoincidence_27 = Sketch_2.setCoincident(SketchArc_5.startPoint(), SketchArc_6.startPoint())
SketchConstraintCoincidence_27.setName("SketchConstraintCoincidence_29")
SketchConstraintTangent_6 = Sketch_2.setTangent(SketchArc_6.results()[1], SketchArc_5.results()[1])
SketchConstraintCoincidence_28 = Sketch_2.setCoincident(SketchArc_6.startPoint(), SketchPoint_6.coordinates())
SketchConstraintCoincidence_28.setName("SketchConstraintCoincidence_30")
SketchConstraintCoincidence_29 = Sketch_2.setCoincident(SketchArc_6.endPoint(), SketchPoint_5.coordinates())
SketchConstraintCoincidence_29.setName("SketchConstraintCoincidence_31")
SketchLine_10 = Sketch_2.addLine(28.46272147483239, 70.73361683339229, 38.77955906494226, 70.73651278326217)
SketchLine_10.setName("SketchLine_11")
SketchLine_10.result().setName("SketchLine_11")
SketchLine_10.setAuxiliary(True)
SketchPoint_7 = Sketch_2.addPoint(28.46272147483239, 70.73361683339229)
SketchConstraintCoincidence_30 = Sketch_2.setCoincident(SketchPoint_7.coordinates(), SketchLine_10.startPoint())
SketchConstraintCoincidence_30.setName("SketchConstraintCoincidence_32")
SketchLine_11 = Sketch_2.addLine(16.42641095303756, 70.73023822521078, 28.46272147483239, 70.73361683339229)
SketchLine_11.setName("SketchLine_12")
SketchLine_11.result().setName("SketchLine_12")
SketchConstraintCoincidence_31 = Sketch_2.setCoincident(SketchLine_11.endPoint(), SketchLine_10.startPoint())
SketchConstraintCoincidence_31.setName("SketchConstraintCoincidence_33")
SketchConstraintParallel_2 = Sketch_2.setParallel(SketchLine_10.result(), SketchLine_11.result())
SketchConstraintCoincidence_32 = Sketch_2.setCoincident(SketchLine_11.endPoint(), SketchPoint_7.coordinates())
SketchConstraintCoincidence_32.setName("SketchConstraintCoincidence_34")
model.do()
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_8r-SketchLine_7r-SketchLine_6r-SketchLine_5r-SketchLine_4r-SketchArc_2_2f-SketchLine_10r-SketchArc_1_2f-SketchLine_9r-SketchArc_3_2r-SketchArc_4_2r")], model.selection("EDGE", "Sketch_2/SketchLine_12"), 360, 0)
Partition_1 = model.addPartition(Part_1_doc, [model.selection("FACE", "PartSet/Plane_4"), model.selection("SOLID", "Revolution_1_1")])
Recover_1 = model.addRecover(Part_1_doc, Partition_1, [Revolution_1.result()])
Plane_5 = model.addPlane(Part_1_doc, model.selection("EDGE", "PartSet/Axis_4"), model.selection("VERTEX", "PartSet/Point_2"), True)
Face_1_objects = [model.selection("EDGE", "Sketch_1/SketchArc_2_2"), model.selection("EDGE", "Sketch_1/SketchLine_4"), model.selection("EDGE", "Sketch_1/SketchLine_5"), model.selection("EDGE", "Sketch_1/SketchLine_6"), model.selection("EDGE", "Sketch_1/SketchLine_7"), model.selection("EDGE", "Sketch_1/SketchLine_8"), model.selection("EDGE", "Sketch_1/SketchLine_1"), model.selection("EDGE", "Sketch_1/SketchLine_2")]
Face_1 = model.addFace(Part_1_doc, Face_1_objects)
Intersection_1 = model.addIntersection(Part_1_doc, [model.selection("SOLID", "Recover_1_1"), model.selection("FACE", "Face_1_1")])
Group_1_objects = [
model.selection("VERTEX", "Intersection_1_1_3/Modified_Vertex&Sketch_1/SketchLine_1_StartVertex&Sketch_1/SketchLine_8_EndVertex"),
model.selection("VERTEX", "[Intersection_1_1_1/Intersection_1_1_1]e[weak_name_2]"),
model.selection("VERTEX", "Intersection_1_1_9/Modified_Vertex&Sketch_1/SketchLine_5_StartVertex&Sketch_1/SketchLine_4_EndVertex"),
model.selection("VERTEX", "Intersection_1_1_7/Modified_Vertex&Sketch_1/SketchLine_7_StartVertex&Sketch_1/SketchLine_6_EndVertex"),
model.selection("VERTEX", "[Intersection_1_1_4/Intersection_1_1_4]e[weak_name_2]"),
model.selection("VERTEX", "[Intersection_1_1_1/Intersection_1_1_1]e[weak_name_1]"),
model.selection("VERTEX", "Intersection_1_1_2/Modified_Vertex&Sketch_1/SketchLine_2_EndVertex&Sketch_1/SketchArc_2_2_EndVertex"),
model.selection("VERTEX", "[Intersection_1_1_4/Intersection_1_1_4]e[weak_name_1]"),
model.selection("VERTEX", "Intersection_1_1_6/Modified_Vertex&Sketch_1/SketchLine_8_StartVertex&Sketch_1/SketchLine_7_EndVertex"),
model.selection("VERTEX", "Intersection_1_1_10/Modified_Vertex&Sketch_1/SketchLine_4_StartVertex&Sketch_1/SketchArc_2_2_StartVertex"),
model.selection("VERTEX", "Intersection_1_1_8/Modified_Vertex&Sketch_1/SketchLine_6_StartVertex&Sketch_1/SketchLine_5_EndVertex")]

Group_1 = model.addGroup(Part_1_doc, Group_1_objects)
Group_2_objects = [model.selection("EDGE", "Intersection_1_1_7"), model.selection("EDGE", "Intersection_1_1_6"), model.selection("EDGE", "Intersection_1_1_3"), model.selection("EDGE", "Intersection_1_1_11"), model.selection("EDGE", "Intersection_1_1_5"), model.selection("EDGE", "Intersection_1_1_1"), model.selection("EDGE", "Intersection_1_1_4"), model.selection("EDGE", "Intersection_1_1_2"), model.selection("EDGE", "Intersection_1_1_10"), model.selection("EDGE", "Intersection_1_1_9"), model.selection("EDGE", "Intersection_1_1_8")]
Group_2 = model.addGroup(Part_1_doc, Group_2_objects)
model.do()
model.end()

aGroupFeature = Group_1.feature()
aSelectionList = aGroupFeature.selectionList("group_list")
assert(aSelectionList.size() == 11)
for index in range(0, aSelectionList.size()):
  attrSelection = aSelectionList.value(index)
  shape = attrSelection.value()
  name = attrSelection.namingName()
  assert(shape.isVertex())
  assert(name)

aGroupFeature = Group_2.feature()
aSelectionList = aGroupFeature.selectionList("group_list")
assert(aSelectionList.size() == 11)
for index in range(0, aSelectionList.size()):
  attrSelection = aSelectionList.value(index)
  shape = attrSelection.value()
  name = attrSelection.namingName()
  assert(shape.isEdge())
  assert(name)

assert(model.checkPythonDump())
