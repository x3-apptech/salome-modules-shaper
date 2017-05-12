"""
    Test movement of the sketch point
"""

import unittest
from GeomDataAPI import geomDataAPI_Point2D
from salome.shaper import model

__updated__ = "2017-05-11"

class TestMovePoint(unittest.TestCase):
  def setUp(self):
    model.begin()
    self.myDocument = model.moduleDocument()
    self.mySketch = model.addSketch(self.myDocument, model.defaultPlane("XOY"))
    self.myOrigin = self.mySketch.addPoint(model.selection("VERTEX", "Origin"))
    self.myPoint = self.mySketch.addPoint(70, 50)
    self.myPointCoordinates = geomDataAPI_Point2D(self.myPoint.coordinates())
    self.myDOF = 2
    model.do()
    self.checkDOF()

  def tearDown(self):
    self.checkDOF()
    model.end()
    assert(model.checkPythonDump())

  def checkDOF(self):
    self.assertEqual(model.dof(self.mySketch), self.myDOF)

  def test_move_free_point(self):
    """ Test 1. Movement of free point
    """
    newPosition = [100., 100.]
    self.mySketch.move(self.myPoint, newPosition[0], newPosition[1])
    model.do()
    self.assertAlmostEqual(self.myPointCoordinates.x(), newPosition[0])
    self.assertAlmostEqual(self.myPointCoordinates.y(), newPosition[1])

  def test_move_fixed_x(self):
    """ Test 2. Movement of partially fixed point (fixed x coordinate)
    """
    DISTANCE = 50.
    horizDistance = self.mySketch.setHorizontalDistance(self.myOrigin.result(), self.myPoint.coordinates(), DISTANCE)
    self.myDOF -= 1
    model.do()
    self.checkDOF()

    newPosition = [100., 100.]
    self.mySketch.move(self.myPoint, newPosition[0], newPosition[1])
    model.do()
    self.assertAlmostEqual(self.myPointCoordinates.x(), DISTANCE)
    self.assertAlmostEqual(self.myPointCoordinates.y(), newPosition[1])

  def test_move_fixed_y(self):
    """ Test 3. Movement of partially fixed point (fixed y coordinate)
    """
    DISTANCE = 50.
    vertDistance = self.mySketch.setVerticalDistance(self.myOrigin.result(), self.myPoint.coordinates(), DISTANCE)
    self.myDOF -= 1
    model.do()
    self.checkDOF()

    newPosition = [100., 100.]
    self.mySketch.move(self.myPoint, newPosition[0], newPosition[1])
    model.do()
    self.assertAlmostEqual(self.myPointCoordinates.x(), newPosition[0])
    self.assertAlmostEqual(self.myPointCoordinates.y(), DISTANCE)

  def test_move_fully_fixed(self):
    """ Test 4. Movement of fully fixed point (should not be changed)
    """
    coord = [self.myPointCoordinates.x(), self.myPointCoordinates.y()]

    fixed = self.mySketch.setFixed(self.myPoint.result())
    self.myDOF -= 2
    model.do()
    self.checkDOF()

    newPosition = [100., 100.]
    self.mySketch.move(self.myPoint, newPosition[0], newPosition[1])
    model.do()
    self.assertAlmostEqual(self.myPointCoordinates.x(), coord[0])
    self.assertAlmostEqual(self.myPointCoordinates.y(), coord[1])


if __name__ == '__main__':
  unittest.main()
