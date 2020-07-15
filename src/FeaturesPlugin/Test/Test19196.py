# Copyright (C) 2020  CEA/DEN, EDF R&D
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

from GeomAPI import *
from SketchAPI import *

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
model.addParameter(Part_1_doc, "r_ext", "80")
model.addParameter(Part_1_doc, "r_int", "60")
model.addParameter(Part_1_doc, "r_cut", "30")
model.addParameter(Part_1_doc, "h", "200")
ParamSize = model.addParameter(Part_1_doc, "size", "r_int*3/4")
Cylinder_1 = model.addCylinder(Part_1_doc, model.selection("VERTEX", "PartSet/Origin"), model.selection("EDGE", "PartSet/OZ"), "r_int", "h")
Cylinder_2 = model.addCylinder(Part_1_doc, model.selection("VERTEX", "PartSet/Origin"), model.selection("EDGE", "PartSet/OZ"), "r_ext", "h")
Sphere_1 = model.addSphere(Part_1_doc, model.selection("VERTEX", "PartSet/Origin"), "r_ext")
Sphere_2 = model.addSphere(Part_1_doc, model.selection("VERTEX", "PartSet/Origin"), "r_int")
Rotation_1 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Sphere_1_1")], axis = model.selection("EDGE", "PartSet/OY"), angle = 90, keepSubResults = True)
Rotation_2 = model.addRotation(Part_1_doc, [model.selection("SOLID", "Sphere_2_1")], axis = model.selection("EDGE", "PartSet/OY"), angle = 90, keepSubResults = True)
Translation_1 = model.addTranslation(Part_1_doc, [model.selection("SOLID", "Rotation_1_1"), model.selection("SOLID", "Rotation_2_1")], axis = model.selection("EDGE", "PartSet/OZ"), distance = "h", keepSubResults = True)
Fuse_1 = model.addFuse(Part_1_doc, [model.selection("SOLID", "Cylinder_1_1"), model.selection("SOLID", "Translation_1_2")], removeEdges = True, keepSubResults = True)
Fuse_2 = model.addFuse(Part_1_doc, [model.selection("SOLID", "Cylinder_2_1"), model.selection("SOLID", "Translation_1_1")], removeEdges = True, keepSubResults = True)
Partition_1 = model.addPartition(Part_1_doc, [model.selection("SOLID", "Fuse_1_1"), model.selection("SOLID", "Fuse_2_1")], keepSubResults = True)
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchLine_1 = Sketch_1.addLine(0, 124.9999999998943, 0, 45.00000000000152)
SketchProjection_1 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OY"), False)
SketchLine_2 = SketchProjection_1.createdFeature()
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_1.result())
SketchLine_3 = Sketch_1.addLine(0, 45.00000000000152, -45, 0)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(-45, 0, 0, -45.00000000000152)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(0, -45.00000000000152, 44.99999999998636, 0)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchLine_6 = Sketch_1.addLine(44.99999999998636, 0, 0, 45.00000000000152)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_6.endPoint())
SketchLine_7 = Sketch_1.addLine(0, -45.00000000000152, 0, -124.9999999997331)
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_7.startPoint())
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_7.result())
SketchLine_8 = Sketch_1.addLine(-45, 0, -125, 0)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_8.startPoint())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_8.result())
SketchLine_9 = Sketch_1.addLine(44.99999999998636, 0, 124.9999999999994, 0)
SketchProjection_2 = Sketch_1.addProjection(model.selection("EDGE", "PartSet/OX"), False)
SketchLine_10 = SketchProjection_2.createdFeature()
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_9.endPoint(), SketchLine_10.result())
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_9.startPoint())
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_10.result())
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchLine_8.startPoint(), SketchLine_10.result())
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_2.result())
SketchConstraintParallel_1 = Sketch_1.setParallel(SketchLine_3.result(), SketchLine_5.result())
SketchConstraintParallel_2 = Sketch_1.setParallel(SketchLine_4.result(), SketchLine_6.result())
SketchConstraintPerpendicular_1 = Sketch_1.setPerpendicular(SketchLine_3.result(), SketchLine_4.result())
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_8.result(), "r_ext")
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchLine_8.startPoint(), SketchLine_7.result(), "size", True)
SketchConstraintEqual_1 = Sketch_1.setEqual(SketchLine_8.result(), SketchLine_1.result())
SketchConstraintEqual_2 = Sketch_1.setEqual(SketchLine_1.result(), SketchLine_9.result())
SketchConstraintEqual_3 = Sketch_1.setEqual(SketchLine_7.result(), SketchLine_9.result())
model.do()
Wire_1_objects = [model.selection("EDGE", "Sketch_1/SketchLine_8"), model.selection("EDGE", "Sketch_1/SketchLine_4"), model.selection("EDGE", "Sketch_1/SketchLine_7"), model.selection("EDGE", "Sketch_1/SketchLine_5"), model.selection("EDGE", "Sketch_1/SketchLine_9"), model.selection("EDGE", "Sketch_1/SketchLine_6"), model.selection("EDGE", "Sketch_1/SketchLine_3"), model.selection("EDGE", "Sketch_1/SketchLine_1")]
Wire_1 = model.addWire(Part_1_doc, Wire_1_objects, False)
Extrusion_1 = model.addExtrusion(Part_1_doc, [model.selection("WIRE", "Wire_1_1")], model.selection(), "h", 0)
Plane_4 = model.addPlane(Part_1_doc, model.selection("FACE", "PartSet/XOY"), "h", False)
Split_1 = model.addSplit(Part_1_doc, [model.selection("COMPSOLID", "Partition_1_1")], [model.selection("FACE", "Plane_1"), model.selection("SHELL", "Extrusion_1_1")], keepSubResults = True)
Sketch_2 = model.addSketch(Part_1_doc, model.selection("FACE", "Split_1_1_7/Modified_Face&Plane_1/Plane_1&new_weak_name_3"))
SketchProjection_3 = Sketch_2.addProjection(model.selection("EDGE", "Extrusion_1_1/To_Edge_2"), True)
SketchLine_11 = SketchProjection_3.createdFeature()
SketchProjection_4 = Sketch_2.addProjection(model.selection("EDGE", "Extrusion_1_1/To_Edge_5"), True)
SketchLine_12 = SketchProjection_4.createdFeature()
SketchProjection_5 = Sketch_2.addProjection(model.selection("EDGE", "Extrusion_1_1/To_Edge_6"), True)
SketchLine_13 = SketchProjection_5.createdFeature()
SketchProjection_6 = Sketch_2.addProjection(model.selection("EDGE", "Extrusion_1_1/To_Edge_3"), True)
SketchLine_14 = SketchProjection_6.createdFeature()
model.do()
Extrusion_2 = model.addExtrusion(Part_1_doc, [model.selection("COMPOUND", "Sketch_2")], model.selection(), "r_cut/sqrt(2)", 0)
Sketch_3 = model.addSketch(Part_1_doc, model.defaultPlane("YOZ"))
SketchProjection_7 = Sketch_3.addProjection(model.selection("EDGE", "Split_1_1_3/Modified_Edge&Extrusion_1_1/To_Edge_4"), True)
SketchLine_15 = SketchProjection_7.createdFeature()
SketchProjection_8 = Sketch_3.addProjection(model.selection("EDGE", "Split_1_1_9/Modified_Edge&Extrusion_1_1/To_Edge_4"), True)
SketchLine_16 = SketchProjection_8.createdFeature()
SketchProjection_9 = Sketch_3.addProjection(model.selection("EDGE", "[Extrusion_2_1/Generated_Face&Sketch_2/SketchProjection_3][Extrusion_2_1/Generated_Face&Sketch_2/SketchProjection_4]"), True)
SketchLine_17 = SketchProjection_9.createdFeature()
SketchLine_18 = Sketch_3.addLine(-45.00000000000152, 221.2132034355964, -99.71358592313776, 275.9267893638306)
SketchLine_19 = Sketch_3.addLine(-99.71358592313776, 275.9267893638306, -80, 200)
SketchConstraintCoincidence_14 = Sketch_3.setCoincident(SketchLine_18.endPoint(), SketchLine_19.startPoint())
SketchConstraintCoincidence_15 = Sketch_3.setCoincident(SketchAPI_Line(SketchLine_16).endPoint(), SketchLine_19.endPoint())
SketchConstraintCoincidence_16 = Sketch_3.setCoincident(SketchLine_18.startPoint(), SketchAPI_Line(SketchLine_17).endPoint())
SketchConstraintAngle_1 = Sketch_3.setAngle(SketchLine_18.result(), SketchLine_16.result(), 45, type = "")
model.do()
Face_1 = model.addFace(Part_1_doc, [model.selection("FACE", "Sketch_3/Face-SketchProjection_9f-SketchLine_18f-SketchLine_19f-SketchProjection_8r-SketchProjection_7r")])
AngularCopy_1 = model.addMultiRotation(Part_1_doc, [model.selection("FACE", "Face_1_1")], model.selection("EDGE", "PartSet/OZ"), 4, keepSubResults = True)
Plane_5 = model.addPlane(Part_1_doc, model.selection("VERTEX", "AngularCopy_1_1_1/MV:Rotated&Sketch_3/SketchLine_19_StartVertex&Sketch_3/SketchLine_18_EndVertex"), model.selection("VERTEX", "AngularCopy_1_1_4/MV:Rotated&Sketch_3/SketchLine_19_StartVertex&Sketch_3/SketchLine_18_EndVertex"), model.selection("VERTEX", "AngularCopy_1_1_4/MV:Rotated&Sketch_3/SketchLine_18_StartVertex&Sketch_3/SketchProjection_9_EndVertex"))
Sketch_4 = model.addSketch(Part_1_doc, model.selection("FACE", "Plane_2"))
SketchLine_20 = Sketch_4.addLine(-71.93873905691365, 265.6178566586798, -194.0624420262377, 195.1097038792864)
SketchProjection_10 = Sketch_4.addProjection(model.selection("VERTEX", "Sketch_3/SketchLine_18_EndVertex"), False)
SketchPoint_1 = SketchProjection_10.createdFeature()
SketchConstraintCoincidence_17 = Sketch_4.setCoincident(SketchLine_20.startPoint(), SketchPoint_1.result())
SketchProjection_11 = Sketch_4.addProjection(model.selection("VERTEX", "AngularCopy_1_1_4/MV:Rotated&Sketch_3/SketchLine_19_StartVertex&Sketch_3/SketchLine_18_EndVertex"), False)
SketchPoint_2 = SketchProjection_11.createdFeature()
SketchConstraintCoincidence_18 = Sketch_4.setCoincident(SketchLine_20.endPoint(), SketchPoint_2.result())
SketchProjection_12 = Sketch_4.addProjection(model.selection("EDGE", "Sketch_3/SketchLine_18"), True)
SketchLine_21 = SketchProjection_12.createdFeature()
SketchProjection_13 = Sketch_4.addProjection(model.selection("EDGE", "[Extrusion_2_1/Generated_Face&Sketch_2/SketchProjection_3][Extrusion_2_1/To_Face]"), True)
SketchLine_22 = SketchProjection_13.createdFeature()
SketchProjection_14 = Sketch_4.addProjection(model.selection("EDGE", "AngularCopy_1_1_4/ME:Rotated&Sketch_3/SketchLine_18"), True)
SketchLine_23 = SketchProjection_14.createdFeature()
model.do()
Face_2 = model.addFace(Part_1_doc, [model.selection("FACE", "Sketch_4/Face-SketchLine_20r-SketchProjection_14r-SketchProjection_13f-SketchProjection_12f")])
AngularCopy_2 = model.addMultiRotation(Part_1_doc, [model.selection("FACE", "Face_2_1")], model.selection("EDGE", "PartSet/OZ"), 4, keepSubResults = True)
Split_2_objects_2 = [model.selection("SOLID", "Extrusion_2_1"), model.selection("COMPOUND", "AngularCopy_1_1"), model.selection("COMPOUND", "AngularCopy_2_1")]
Split_2 = model.addSplit(Part_1_doc, [model.selection("COMPSOLID", "Split_1_1")], Split_2_objects_2, keepSubResults = True)
Group_1_objects = [model.selection("FACE", "Split_2_1_7/Modified_Face&Cylinder_1_1/Face_1"), model.selection("FACE", "Split_2_1_8/Modified_Face&Cylinder_1_1/Face_1"), model.selection("FACE", "Split_2_1_9/Modified_Face&Cylinder_1_1/Face_1"), model.selection("FACE", "Split_2_1_10/Modified_Face&Cylinder_1_1/Face_1")]
Group_1 = model.addGroup(Part_1_doc, "Faces", Group_1_objects)
Group_1.setName("shared_face_cyl")
Group_1.result().setName("shared_face_cyl")
model.end()

model.testNbResults(Split_2, 1)
model.testNbSubResults(Split_2, [20])
model.testNbSubShapes(Split_2, GeomAPI_Shape.SOLID, [20])
model.testNbSubShapes(Split_2, GeomAPI_Shape.FACE, [120])
model.testNbSubShapes(Split_2, GeomAPI_Shape.EDGE, [492])
model.testNbSubShapes(Split_2, GeomAPI_Shape.VERTEX, [984])
model.testResultsVolumes(Split_2, [5093568.888962713])

model.begin()
ParamSize.expression().setValue("r_int/2")
model.end()

model.testNbResults(Split_2, 1)
model.testNbSubResults(Split_2, [20])
model.testNbSubShapes(Split_2, GeomAPI_Shape.SOLID, [20])
model.testNbSubShapes(Split_2, GeomAPI_Shape.FACE, [120])
model.testNbSubShapes(Split_2, GeomAPI_Shape.EDGE, [492])
model.testNbSubShapes(Split_2, GeomAPI_Shape.VERTEX, [984])
model.testResultsVolumes(Split_2, [5093568.888962713])

assert(model.checkPythonDump())