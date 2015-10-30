from GeomDataAPI import geomDataAPI_Point2D
from model.roots import Interface
from model.errors import WrongNumberOfArguments

class Line(Interface):
    """Interface for editing of a sketch line feature."""
    def __init__(self, feature, *args):
        Interface.__init__(self, feature)
        assert(self._feature.getKind() == "SketchLine")

        # Initialize attributes
        self._start_point = geomDataAPI_Point2D(
            self._feature.data().attribute("StartPoint")
            )
        self._end_point = geomDataAPI_Point2D(
            self._feature.data().attribute("EndPoint")
            )

        assert(self._start_point)
        assert(self._end_point)

        if not args:
            return

        assert(len(args) in (1, 2, 4))

        # Set attribute values and execute
        if len(args) == 4:
            self.__createByCoordinates(*args)
        elif len(args) == 2:
            self.__createByPoints(*args)
        elif len(args) == 1:
            self.__createByName(*args)

        self._execute()
        pass

    def __createByCoordinates(self, x1, y1, x2, y2):
        self._start_point.setValue(x1, y1)
        self._end_point.setValue(x2, y2)
        pass

    def __createByPoints(self, p1, p2):
        self._start_point.setValue(p1.x(), p1.y())
        self._end_point.setValue(p2.x(), p2.y())
        pass

    def __createByName(self, name):
        self._feature.data().selection("External").selectSubShape("EDGE", name)
        pass

    def startPointData(self):
        return self._start_point

    def endPointData(self):
        return self._end_point

    def result(self):
        return self._feature.firstResult()
