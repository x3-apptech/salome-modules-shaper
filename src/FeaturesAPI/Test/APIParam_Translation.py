"""
Test case for Translation feature. 
Written on High API.
"""
from ModelAPI import *
from GeomAPI import *

from salome.shaper import model

# Get session
aSession = ModelAPI_Session.get()

# Create a part
aDocument = aSession.activeDocument()
aSession.startOperation()
model.addPart(aDocument)
aDocument = aSession.activeDocument()
aSession.finishOperation()

# Create a box

aSession.startOperation()
aBox =  model.addBox(aDocument, 10, 10, 10)

# Perform a translation by an axis and a distance
aSession.startOperation()
aTranslation1 = model.addTranslation(aDocument, [model.selection("SOLID", "Box_1_1")], model.selection("EDGE", "Box_1_1/Front&Box_1_1/Bottom"), 20).result()
aSession.finishOperation()
assert (aTranslation1 is not None)

# Perform a translation by DX, DY, DZ vector
aSession.startOperation()
aTranslation2 = model.addTranslation(aDocument, [model.selection("SOLID", "Box_1_1")], 10, 10, 10).result()
aSession.finishOperation()
assert (aTranslation2 is not None)

# Perform a translation by two points
aSession.startOperation()
aPoint1 = model.addPoint(aDocument, 0, 0, 0).result()
aPoint2 = model.addPoint(aDocument, 10, 10, 0).result()
aTranslation3 = model.addTranslation(aDocument, [model.selection("SOLID", "Box_1_1")], aPoint1, aPoint2).result()
aSession.finishOperation()
assert (aTranslation3 is not None)
