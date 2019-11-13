// Copyright (C) 2019  CEA/DEN, EDF R&D
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef GeomAlgoAPI_Projection_H_
#define GeomAlgoAPI_Projection_H_

#include <GeomAlgoAPI.h>

#include <GeomAPI_Pln.h>

#include <memory>

class GeomAPI_Curve;
class GeomAPI_Edge;

/**\class GeomAlgoAPI_Projection
 * \ingroup DataAlgo
 * \brief Project curve onto a plane
 */
class GeomAlgoAPI_Projection
{
public:
  GEOMALGOAPI_EXPORT GeomAlgoAPI_Projection(const std::shared_ptr<GeomAPI_Pln>& thePlane);

  /// Project curve to the plane
  GEOMALGOAPI_EXPORT
  std::shared_ptr<GeomAPI_Curve> project(const std::shared_ptr<GeomAPI_Curve>& theCurve);
  /// Project edge to the plane
  GEOMALGOAPI_EXPORT
  std::shared_ptr<GeomAPI_Curve> project(const std::shared_ptr<GeomAPI_Edge>& theEdge);

private:
  std::shared_ptr<GeomAPI_Pln> myPlane;
};

#endif