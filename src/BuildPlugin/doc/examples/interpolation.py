from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Box_1 = model.addBox(Part_1_doc, 10, 10, 10)
Interpolation_1_objects = [model.selection("VERTEX", "[Box_1_1/Back][Box_1_1/Left][Box_1_1/Top]"), model.selection("VERTEX", "[Box_1_1/Front][Box_1_1/Left][Box_1_1/Top]"), model.selection("VERTEX", "[Box_1_1/Front][Box_1_1/Right][Box_1_1/Top]"), model.selection("VERTEX", "[Box_1_1/Back][Box_1_1/Right][Box_1_1/Top]")]
Interpolation_1 = model.addInterpolation(Part_1_doc, Interpolation_1_objects, True, False)
model.do()
model.end()
