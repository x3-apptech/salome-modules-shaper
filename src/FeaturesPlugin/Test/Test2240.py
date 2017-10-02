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
model.addParameter(Part_1_doc, "length_assemblage", "10")
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchLine_1 = Sketch_1.addLine(0, 60, 10, 60)
SketchLine_2 = Sketch_1.addLine(model.selection("EDGE", "PartSet/OY"))
SketchConstraintCoincidence_1 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_2.result())
SketchLine_3 = Sketch_1.addLine(10, 60, 10, 40)
SketchConstraintCoincidence_2 = Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_3.startPoint())
SketchLine_4 = Sketch_1.addLine(10, 40, 20, 40)
SketchConstraintCoincidence_3 = Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
SketchLine_5 = Sketch_1.addLine(20, 40, 20, 20)
SketchConstraintCoincidence_4 = Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_5.startPoint())
SketchLine_6 = Sketch_1.addLine(20, 20, 30, 20)
SketchConstraintCoincidence_5 = Sketch_1.setCoincident(SketchLine_5.endPoint(), SketchLine_6.startPoint())
SketchLine_7 = Sketch_1.addLine(30, 20, 30, 0)
SketchConstraintCoincidence_6 = Sketch_1.setCoincident(SketchLine_6.endPoint(), SketchLine_7.startPoint())
SketchLine_8 = Sketch_1.addLine(model.selection("EDGE", "PartSet/OX"))
SketchConstraintCoincidence_7 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_8.result())
SketchLine_9 = Sketch_1.addLine(30, 0, 81.26217508737108, 0)
SketchConstraintCoincidence_8 = Sketch_1.setCoincident(SketchLine_7.endPoint(), SketchLine_9.startPoint())
SketchConstraintCoincidence_9 = Sketch_1.setCoincident(SketchLine_9.endPoint(), SketchLine_8.result())
SketchLine_10 = Sketch_1.addLine(0, 60, 0, 81.26217508737108)
SketchConstraintCoincidence_10 = Sketch_1.setCoincident(SketchLine_1.startPoint(), SketchLine_10.startPoint())
SketchConstraintCoincidence_11 = Sketch_1.setCoincident(SketchLine_10.endPoint(), SketchLine_2.result())
SketchConstraintHorizontal_1 = Sketch_1.setHorizontal(SketchLine_1.result())
SketchConstraintHorizontal_2 = Sketch_1.setHorizontal(SketchLine_4.result())
SketchConstraintHorizontal_3 = Sketch_1.setHorizontal(SketchLine_6.result())
SketchConstraintVertical_1 = Sketch_1.setVertical(SketchLine_3.result())
SketchConstraintVertical_2 = Sketch_1.setVertical(SketchLine_5.result())
SketchArc_1 = Sketch_1.addArc(4.151438510550382e-034, -3.089278765476956e-034, 81.26217508737108, 0, 0, 81.26217508737108, False)
SketchConstraintCoincidence_12 = Sketch_1.setCoincident(SketchLine_2.startPoint(), SketchArc_1.center())
SketchConstraintCoincidence_13 = Sketch_1.setCoincident(SketchLine_9.endPoint(), SketchArc_1.startPoint())
SketchConstraintCoincidence_14 = Sketch_1.setCoincident(SketchLine_2.result(), SketchArc_1.endPoint())
SketchConstraintCoincidence_15 = Sketch_1.setCoincident(SketchLine_10.endPoint(), SketchArc_1.endPoint())
SketchConstraintVertical_3 = Sketch_1.setVertical(SketchLine_7.result())
SketchArc_2 = Sketch_1.addArc(4.151438510550382e-034, -3.089278765476956e-034, 76.26217508737108, 0, 0, 76.26217508737108, False)
SketchConstraintCoincidence_16 = Sketch_1.setCoincident(SketchLine_2.startPoint(), SketchArc_2.center())
SketchConstraintCoincidence_16.setName("SketchConstraintCoincidence_23")
SketchConstraintCoincidence_17 = Sketch_1.setCoincident(SketchLine_9.result(), SketchArc_2.startPoint())
SketchConstraintCoincidence_17.setName("SketchConstraintCoincidence_24")
SketchConstraintCoincidence_18 = Sketch_1.setCoincident(SketchLine_10.result(), SketchArc_2.endPoint())
SketchConstraintCoincidence_18.setName("SketchConstraintCoincidence_25")
SketchConstraintDistance_1 = Sketch_1.setDistance(SketchArc_1.endPoint(), SketchArc_2.endPoint(), 5)
SketchConstraintLength_1 = Sketch_1.setLength(SketchLine_7.result(), "2*length_assemblage")
SketchConstraintLength_1.setName("SketchConstraintLength_2")
SketchConstraintLength_2 = Sketch_1.setLength(SketchLine_6.result(), "length_assemblage")
SketchConstraintLength_2.setName("SketchConstraintLength_3")
SketchConstraintEqual_1 = Sketch_1.setEqual(SketchLine_5.result(), SketchLine_7.result())
SketchConstraintEqual_1.setName("SketchConstraintEqual_2")
SketchConstraintEqual_2 = Sketch_1.setEqual(SketchLine_6.result(), SketchLine_4.result())
SketchConstraintEqual_2.setName("SketchConstraintEqual_3")
SketchConstraintEqual_3 = Sketch_1.setEqual(SketchLine_5.result(), SketchLine_3.result())
SketchConstraintEqual_3.setName("SketchConstraintEqual_4")
SketchConstraintEqual_4 = Sketch_1.setEqual(SketchLine_4.result(), SketchLine_1.result())
SketchConstraintEqual_4.setName("SketchConstraintEqual_5")
model.do()
Sketch_2 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
SketchCircle_1 = Sketch_2.addCircle(9.082839147404137, 71.32948234489588, 2.005056553640547)
SketchCircle_2 = Sketch_2.addCircle(20.11065019242717, 63.7102674410618, 2.268462537186828)
SketchCircle_3 = Sketch_2.addCircle(26.52683116407693, 51.67992811921848, 2.727171758268866)
SketchCircle_4 = Sketch_2.addCircle(38.35666483055617, 58.29661474623231, 3.007584830460834)
SketchCircle_5 = Sketch_2.addCircle(33.94554041254697, 44.0607132153844, 3.054011155390377)
SketchCircle_6 = Sketch_2.addCircle(48.58245325412298, 42.45666797247195, 2.346858526438435)
SketchCircle_7 = Sketch_2.addCircle(38.95818179664835, 31.42885692744893, 3.547307159201095)
SketchCircle_8 = Sketch_2.addCircle(63.82088306179116, 27.41874382016783, 4.536925074373651)
SketchCircle_9 = Sketch_2.addCircle(50.18649849703544, 28.4212720969881, 4.820482891521984)
SketchCircle_10 = Sketch_2.addCircle(49.58498153094326, 15.18789884296047, 2.552020354335177)
SketchCircle_11 = Sketch_2.addCircle(66.22695092615982, 10.77677442495125, 4.536925074373645)
SketchCircle_12 = Sketch_2.addCircle(50.98852111849166, 6.165144351577979, 3.547307159201085)
model.do()
Face_1 = model.addFace(Part_1_doc, [model.selection("WIRE", "Sketch_1/Wire-SketchLine_1f-SketchLine_3f-SketchLine_4f-SketchLine_5f-SketchLine_6f-SketchLine_7f-SketchLine_9f-SketchLine_10r-SketchArc_2_2f")])
Face_2 = model.addFace(Part_1_doc, [model.selection("WIRE", "Sketch_1/Wire-SketchLine_9f-SketchLine_10r-SketchArc_1_2f-SketchArc_2_2r")])
Face_3 = model.addFace(Part_1_doc, [model.selection("EDGE", "Sketch_2/Edge-SketchCircle_1_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_2_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_3_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_4_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_5_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_6_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_7_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_9_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_8_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_10_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_12_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_11_2")])
Face_4 = model.addFace(Part_1_doc, [model.selection("EDGE", "Sketch_2/Edge-SketchCircle_1_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_2_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_3_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_4_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_5_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_7_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_6_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_9_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_8_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_10_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_12_2"), model.selection("EDGE", "Sketch_2/Edge-SketchCircle_11_2"), model.selection("WIRE", "Sketch_1/Wire-SketchLine_1f-SketchLine_3f-SketchLine_4f-SketchLine_5f-SketchLine_6f-SketchLine_7f-SketchLine_9f-SketchLine_10r-SketchArc_2_2f")])
Partition_1 = model.addPartition(Part_1_doc, [model.selection("FACE", "Face_1_1"), model.selection("FACE", "Face_2_1"), model.selection("FACE", "Face_3_1"), model.selection("FACE", "Face_3_2"), model.selection("FACE", "Face_3_3"), model.selection("FACE", "Face_3_4"), model.selection("FACE", "Face_3_5"), model.selection("FACE", "Face_3_6"), model.selection("FACE", "Face_3_7"), model.selection("FACE", "Face_3_8"), model.selection("FACE", "Face_3_9"), model.selection("FACE", "Face_3_10"), model.selection("FACE", "Face_3_11"), model.selection("FACE", "Face_3_12"), model.selection("FACE", "Face_4_1")])
Group_1 = model.addGroup(Part_1_doc, [model.selection("FACE", "Face_2_1"), model.selection("FACE", "Face_3_7"), model.selection("FACE", "Face_3_5"), model.selection("FACE", "Face_3_3"), model.selection("FACE", "Face_3_4"), model.selection("FACE", "Face_3_1"), model.selection("FACE", "Face_3_2"), model.selection("FACE", "Face_3_6"), model.selection("FACE", "Face_3_11"), model.selection("FACE", "Face_3_10"), model.selection("FACE", "Face_3_8"), model.selection("FACE", "Face_3_9"), model.selection("FACE", "Face_3_12")])
Group_1.result().setName("eau")
Group_1.result().setColor(0, 170, 255)
Group_2 = model.addGroup(Part_1_doc, [model.selection("FACE", "Face_4_1")])
Group_2.result().setName("acier")
Group_2.result().setColor(170, 85, 0)
Sketch_3 = model.addSketch(Part_1_doc, model.selection("FACE", "PartSet/XOY"))
SketchLine_11 = Sketch_3.addLine(40, 0, 30, 0)
SketchPoint_1 = Sketch_3.addPoint(model.selection("VERTEX", "Partition_1_1_1/Modified_Edge_15_6&Partition_1_1_1/Modified_Edge_15_5"))
SketchConstraintCoincidence_19 = Sketch_3.setCoincident(SketchLine_11.endPoint(), SketchPoint_1.result())
SketchLine_12 = Sketch_3.addLine(30, 0, 30, 10)
SketchLine_13 = Sketch_3.addLine(30, 10, 40, 10)
SketchLine_14 = Sketch_3.addLine(40, 10, 40, 0)
SketchConstraintCoincidence_20 = Sketch_3.setCoincident(SketchLine_14.endPoint(), SketchLine_11.startPoint())
SketchConstraintCoincidence_21 = Sketch_3.setCoincident(SketchLine_11.endPoint(), SketchLine_12.startPoint())
SketchConstraintCoincidence_22 = Sketch_3.setCoincident(SketchLine_12.endPoint(), SketchLine_13.startPoint())
SketchConstraintCoincidence_23 = Sketch_3.setCoincident(SketchLine_13.endPoint(), SketchLine_14.startPoint())
SketchConstraintCoincidence_23.setName("SketchConstraintCoincidence_26")
SketchConstraintHorizontal_4 = Sketch_3.setHorizontal(SketchLine_11.result())
SketchConstraintVertical_4 = Sketch_3.setVertical(SketchLine_12.result())
SketchConstraintHorizontal_5 = Sketch_3.setHorizontal(SketchLine_13.result())
SketchConstraintVertical_5 = Sketch_3.setVertical(SketchLine_14.result())
SketchConstraintLength_3 = Sketch_3.setLength(SketchLine_13.result(), "length_assemblage")
SketchConstraintLength_3.setName("SketchConstraintLength_1")
SketchConstraintEqual_5 = Sketch_3.setEqual(SketchLine_13.result(), SketchLine_14.result())
SketchConstraintEqual_5.setName("SketchConstraintEqual_1")
model.do()
Face_5 = model.addFace(Part_1_doc, [model.selection("WIRE", "Sketch_3/Wire-SketchLine_11r-SketchLine_12r-SketchLine_13r-SketchLine_14r")])
MultiTranslation_1 = model.addMultiTranslation(Part_1_doc, [model.selection("FACE", "Face_5_1")], model.selection("EDGE", "Sketch_1/Edge-SketchLine_9"), "length_assemblage", 4, model.selection("EDGE", "PartSet/OY"), "length_assemblage", 2)
Recover_1 = model.addRecover(Part_1_doc, MultiTranslation_1, [Face_5.result()])
Translation_1 = model.addTranslation(Part_1_doc, [model.selection("FACE", "Recover_1_1")], model.selection("VERTEX", "Sketch_3/Vertex-SketchPoint_1-SketchLine_12s-SketchLine_11e"), model.selection("VERTEX", "Sketch_1/Vertex-SketchLine_6s-SketchLine_5e"))
MultiTranslation_2 = model.addMultiTranslation(Part_1_doc, [model.selection("FACE", "Translation_1_1")], model.selection("EDGE", "PartSet/OX"), "length_assemblage", 4, model.selection("EDGE", "PartSet/OY"), "length_assemblage", 2)
Recover_2 = model.addRecover(Part_1_doc, MultiTranslation_1, [Face_5.result()])
Translation_2 = model.addTranslation(Part_1_doc, [model.selection("FACE", "Recover_2_1")], model.selection("VERTEX", "Sketch_3/Vertex-SketchPoint_1-SketchLine_12s-SketchLine_11e"), model.selection("VERTEX", "Sketch_1/Vertex-SketchLine_4s-SketchLine_3e"))
MultiTranslation_3 = model.addMultiTranslation(Part_1_doc, [model.selection("FACE", "Translation_2_1")], model.selection("EDGE", "PartSet/OX"), "length_assemblage", 4)
Recover_3 = model.addRecover(Part_1_doc, MultiTranslation_1, [Face_5.result()])
Translation_3 = model.addTranslation(Part_1_doc, [model.selection("FACE", "Recover_3_1")], model.selection("VERTEX", "Sketch_1/Vertex-SketchLine_9s-SketchLine_7e"), model.selection("VERTEX", "MultiTranslation_3_1/Translated_Edge_1_4&MultiTranslation_3_1/Translated_Edge_1_1"))
MultiTranslation_4 = model.addMultiTranslation(Part_1_doc, [model.selection("FACE", "Translation_3_1")], model.selection("EDGE", "PartSet/OX"), "length_assemblage", 3)
Recover_4 = model.addRecover(Part_1_doc, MultiTranslation_1, [Face_5.result()])
Translation_4 = model.addTranslation(Part_1_doc, [model.selection("FACE", "Recover_4_1")], model.selection("VERTEX", "Sketch_1/Vertex-SketchLine_9s-SketchLine_7e"), model.selection("VERTEX", "Sketch_1/Vertex-SketchLine_1s-SketchLine_10s"))
MultiTranslation_5 = model.addMultiTranslation(Part_1_doc, [model.selection("FACE", "Translation_4_1")], model.selection("EDGE", "PartSet/OX"), "length_assemblage", 2)
Partition_2 = model.addPartition(Part_1_doc, [model.selection("COMPOUND", "Partition_1_1"), model.selection("COMPOUND", "MultiTranslation_1_1"), model.selection("COMPOUND", "MultiTranslation_2_1"), model.selection("COMPOUND", "MultiTranslation_3_1"), model.selection("COMPOUND", "MultiTranslation_4_1"), model.selection("COMPOUND", "MultiTranslation_5_1")])
model.end()

# check that partition constructed correctly
from ModelAPI import *
aFactory = ModelAPI_Session.get().validators()
assert(aFactory.validate(Partition_2.feature()))
assert(Partition_2.feature().results().size() == 1)

assert(model.checkPythonDump())