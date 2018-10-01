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
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchPoint_1 = Sketch_1.addPoint(-22.97883597883597, 19.23809523809524)
SketchPoint_2 = Sketch_1.addPoint(18.16931216931219, 17.1005291005291)
SketchPoint_3 = Sketch_1.addPoint(-19.77248677248678, -16.03174603174603)
SketchPoint_4 = Sketch_1.addPoint(21.37566137566138, -16.03174603174603)
model.do()
Vertex_1_objects = [model.selection("VERTEX", "Sketch_1/Vertex-SketchPoint_3"), model.selection("VERTEX", "Sketch_1/Vertex-SketchPoint_1"), model.selection("VERTEX", "Sketch_1/Vertex-SketchPoint_4"), model.selection("VERTEX", "Sketch_1/Vertex-SketchPoint_2")]
Vertex_1 = model.addVertex(Part_1_doc, Vertex_1_objects)
Compound_1 = model.addCompound(Part_1_doc, [model.selection("VERTEX", "Vertex_1_1"), model.selection("VERTEX", "Vertex_1_2")])
Compound_2 = model.addCompound(Part_1_doc, [model.selection("VERTEX", "Vertex_1_3"), model.selection("VERTEX", "Vertex_1_4")])
Fuse_1 = model.addFuse(Part_1_doc, [model.selection("COMPOUND", "Compound_1_1"), model.selection("COMPOUND", "Compound_2_1")])
model.testHaveNamingSubshapes(Fuse_1, model, Part_1_doc)
model.do()
model.end()

from GeomAPI import  GeomAPI_Shape

model.testNbResults(Fuse_1, 1)
model.testNbSubResults(Fuse_1, [4])
model.testNbSubShapes(Fuse_1, GeomAPI_Shape.SOLID, [0])
model.testNbSubShapes(Fuse_1, GeomAPI_Shape.FACE, [0])
model.testNbSubShapes(Fuse_1, GeomAPI_Shape.EDGE, [0])
model.testNbSubShapes(Fuse_1, GeomAPI_Shape.VERTEX, [4])

assert(model.checkPythonDump())