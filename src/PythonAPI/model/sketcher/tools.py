# Author: Sergey Pokhodenko
# Copyright (C) 2014-20xx CEA/DEN, EDF R&D

import ModelHighAPI
from GeomDataAPI import *
from ModelAPI import *
import math

def addPolyline(sketch, *coords):
    """Add a poly-line to sketch.

    The end of consecutive segments are defined as coincident.
    """
    c0 = coords[0]
    c1 = coords[1]
    polyline = []
    line_1 = sketch.addLine(c0, c1)
    polyline.append(line_1)
    # Adding and connecting next lines
    for c2 in coords[2:]:
        line_2 = sketch.addLine(c1, c2)
        sketch.setCoincident(line_1.endPoint(), line_2.startPoint())
        polyline.append(line_2)
        c1 = c2
        line_1 = line_2
    return polyline


def addPolygon(sketch, *coords):
    """Add a polygon to sketch.

    The end of consecutive segments are defined as coincident.
    """
    pg = addPolyline(sketch, *coords)
    # Closing the poly-line supposed being defined by at least 3 points
    c0 = coords[0]
    cn = coords[len(coords) - 1]
    ln = sketch.addLine(cn, c0)
    sketch.setCoincident(
        pg[len(coords) - 2].endPoint(), ln.startPoint()
        )
    sketch.setCoincident(
        ln.endPoint(), pg[0].startPoint()
        )
    pg.append(ln)
    return pg

def dof(sketch):
    """ Extract degrees of freedom for the given sketch
    """
    aSketch = sketch
    if issubclass(type(aSketch), ModelHighAPI.ModelHighAPI_Interface):
        aSketch = sketch.feature()
    return int(filter(str.isdigit, aSketch.string("SolverDOF").value()))

def distancePointPoint(thePoint1, thePoint2):
    aPnt1 = toList(thePoint1)
    aPnt2 = toList(thePoint2)
    return math.hypot(aPnt1[0] - aPnt2[0], aPnt1[1] - aPnt2[1])

def distancePointLine(thePoint, theLine):
    aPoint = toList(thePoint)
    aLine = toSketchFeature(theLine)

    aLineStart = geomDataAPI_Point2D(aLine.attribute("StartPoint")).pnt().xy()
    aLineEnd = geomDataAPI_Point2D(aLine.attribute("EndPoint")).pnt().xy()
    aLineDir = aLineEnd.decreased(aLineStart)
    aLineLen = aLineEnd.distance(aLineStart)
    aCross = (aPoint[0] - aLineStart.x()) * aLineDir.y() - (aPoint[1] - aLineStart.y()) * aLineDir.x()
    return math.fabs(aCross / aLineLen)

def lastSubFeature(theSketch, theKind):
    """
    obtains last feature of given kind from the sketch
    """
    aSketch = featureToCompositeFeature(toSketchFeature(theSketch))
    for anIndex in range(aSketch.numberOfSubs() - 1, -1, -1):
        aSub = aSketch.subFeature(anIndex)
        if (aSub.getKind() == theKind):
            return aSub

def toSketchFeature(theEntity):
    """ Converts entity to sketch feature if possible
    """
    if issubclass(type(theEntity), ModelHighAPI.ModelHighAPI_Interface):
        return theEntity.feature()
    else:
        return theEntity

def toList(thePoint):
    if issubclass(type(thePoint), list):
        return thePoint
    elif issubclass(type(thePoint), GeomDataAPI_Point2D):
        return [thePoint.x(), thePoint.y()]
    else:
        aFeature = toSketchFeature(thePoint)
        aPoint = geomDataAPI_Point2D(aFeature.attribute("PointCoordinates"))
        return [aPoint.x(), aPoint.y()]
