// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#ifndef GeomAPI_Edge_H_
#define GeomAPI_Edge_H_

#include <GeomAPI_Shape.h>

class GeomAPI_Pln;
class GeomAPI_Pnt;
class GeomAPI_Circ;
class GeomAPI_Lin;
class GeomAPI_Ellipse;

/**\class GeomAPI_Edge
* \ingroup DataModel
 * \brief Interface to the edge object
 */

class GeomAPI_Edge : public GeomAPI_Shape
{
public:
   /// Makes an undefined Edge (no geometry).
  GEOMAPI_EXPORT
   GeomAPI_Edge();

   /// Creation of edge by the edge-shape
  GEOMAPI_EXPORT
   GeomAPI_Edge(const std::shared_ptr<GeomAPI_Shape>& theShape);

  /// Verifies that the edge is a line
  GEOMAPI_EXPORT
  bool isLine() const;

  /// Verifies that the edge is a circle
  GEOMAPI_EXPORT
  bool isCircle() const;

  /// Verifies that the edge is an arc of circle
  GEOMAPI_EXPORT
  bool isArc() const;

  /// Verifies that the edge is an arc of circle
  GEOMAPI_EXPORT
  bool isEllipse() const;

  /// Returns the first vertex coordinates of the edge
  GEOMAPI_EXPORT
  std::shared_ptr<GeomAPI_Pnt> firstPoint();

  /// Returns the Last vertex coordinates of the edge
  GEOMAPI_EXPORT
  std::shared_ptr<GeomAPI_Pnt> lastPoint();

  /// Returns a circle if edge is based on the circle curve
  GEOMAPI_EXPORT
  std::shared_ptr<GeomAPI_Circ> circle() const;

  /// Returns an ellipse if edge is based on the ellipse curve
  GEOMAPI_EXPORT
  std::shared_ptr<GeomAPI_Ellipse> ellipse() const;

  /// Returns a line if edge is based on the linear curve
  GEOMAPI_EXPORT
  std::shared_ptr<GeomAPI_Lin> line() const;

  /// Returns true if the current edge is geometrically equal to the given edge
  GEOMAPI_EXPORT
  bool isEqual(const std::shared_ptr<GeomAPI_Shape> theEdge) const;

  /// Returns range of parameter on the curve
  GEOMAPI_EXPORT
  void getRange(double& theFirst, double& theLast) const;

  /// Returns true, if the edge is fully placed in the specified plane
  GEOMAPI_EXPORT
  bool isInPlane(const std::shared_ptr<GeomAPI_Pln> thePlane) const;

  /// Returns edge length.
  GEOMAPI_EXPORT
  double length() const;

  /// Returns true if the edge is closed (like full circle)
  GEOMAPI_EXPORT
  bool isClosed() const;

  /// Returns true if the edge is degenerated (has no 3D curve)
  GEOMAPI_EXPORT
  bool isDegenerated() const;
};

//! Pointer on attribute object
typedef std::shared_ptr<GeomAPI_Edge> GeomEdgePtr;

#endif

