from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOY"))
Rectangle_1 = Sketch_1.addRectangle(5.5, 8.5, 31.3, 78.9)
model.do()
model.end()
