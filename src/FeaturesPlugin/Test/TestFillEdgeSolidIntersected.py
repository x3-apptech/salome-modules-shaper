# Copyright (C) 2014-2019  CEA/DEN, EDF R&D
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

# -*- coding: utf-8 -*-

from SketchAPI import *

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Cone_1 = model.addCone(Part_1_doc, model.selection("VERTEX", "PartSet/Origin"), model.selection("EDGE", "PartSet/OZ"), 10, 5, 10)
Plane_4 = model.addPlane(Part_1_doc, model.selection("FACE", "Cone_1_1/Face_3"), 5, True)
Sketch_1 = model.addSketch(Part_1_doc, model.selection("FACE", "Plane_1"))
SketchLine_1 = Sketch_1.addLine(-8.679188755740876, 4.967059748203267, 8.679188755740878, -4.967059748203268)
SketchProjection_1 = Sketch_1.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_1 = SketchProjection_1.createdFeature()
SketchConstraintMiddle_1 = Sketch_1.setMiddlePoint(SketchAPI_Point(SketchPoint_1).coordinates(), SketchLine_1.result())
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_1.result(), 20)
model.do()
Edge_1 = model.addEdge(Part_1_doc, [model.selection("EDGE", "Sketch_1/SketchLine_1")])
Fill_1 = model.addFill(Part_1_doc, [model.selection("EDGE", "Edge_1_1")], [model.selection("SOLID", "Cone_1_1")])
model.do()

model.checkResult(Fill_1, model, 1, [3], [0], [0], [3], [6])
model.testHaveNamingSubshapes(Fill_1, model, Part_1_doc)

model.end()

assert(model.checkPythonDump())
