from ModelAPI import *
from SketchResult import *
import sketch
import extrusion
# reload(sketch) # Pour tester plus facilement
# reload(extrusion) # Pour tester plus facilement


def makeBox(aLength, aWidth, aHeight):
    # Getting the active document
    session = ModelAPI_Session.get()
    part = session.activeDocument()

    # Starting the Sketch
    base = sketch.addTo(part)
    sketch.setXOYPlane(base)

    # Creating the lines
    l1 = sketch.addLine(10, 10, 10, 50, base)
    l2 = sketch.addLine(10, 50, 60, 60, base)
    l3 = sketch.addLine(60, 60, 50, 10, base)
    l4 = sketch.addLine(50, 10, 10, 10, base)
    base.execute()

    # Creating the constraints
    sketch.makeCoincident(sketch.getEndPoint(l1),
                          sketch.getStartPoint(l2), base)
    sketch.makeCoincident(sketch.getEndPoint(l2),
                          sketch.getStartPoint(l3), base)
    sketch.makeCoincident(sketch.getEndPoint(l3),
                          sketch.getStartPoint(l4), base)
    sketch.makeCoincident(sketch.getEndPoint(l4),
                          sketch.getStartPoint(l1), base)

    sketch.makeParallel(sketch.getGeometry(l1), sketch.getGeometry(l3), base)
    sketch.makeParallel(sketch.getGeometry(l2), sketch.getGeometry(l4), base)

    sketch.makePerpendicular(sketch.getGeometry(l1),
                             sketch.getGeometry(l4), base)
    # Set to 0X and 0Y lines defined length
    sketch.makeConstantLength(sketch.getGeometry(l1), aLength, base)
    sketch.makeConstantLength(sketch.getGeometry(l4), aWidth, base)

    # Finalisation of the operation
    builder = SketchResult(base)

    # Creating a feature Extrusion
    box = extrusion.addNew(builder, aHeight, part)

    # return base.lastResult()
    return extrusion.getBody(box)
