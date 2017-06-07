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

#ifndef SRC_CONSTRUCTIONAPI_CONSTRUCTIONAPI_POINT_H_
#define SRC_CONSTRUCTIONAPI_CONSTRUCTIONAPI_POINT_H_

#include "ConstructionAPI.h"

#include <ConstructionPlugin_Point.h>

#include <ModelHighAPI_Interface.h>
#include <ModelHighAPI_Macro.h>

class ModelAPI_AttributeDouble;
class ModelAPI_Document;
class ModelHighAPI_Double;

/// \class ConstructionAPI_Point
/// \ingroup CPPHighAPI
/// \brief Interface for Point feature.
class ConstructionAPI_Point: public ModelHighAPI_Interface
{
public:
  /// Constructor without values.
  CONSTRUCTIONAPI_EXPORT
  explicit ConstructionAPI_Point(const std::shared_ptr<ModelAPI_Feature>& theFeature);

  /// Constructor with values.
  CONSTRUCTIONAPI_EXPORT
  ConstructionAPI_Point(const std::shared_ptr<ModelAPI_Feature>& theFeature,
                        const ModelHighAPI_Double& theX,
                        const ModelHighAPI_Double& theY,
                        const ModelHighAPI_Double& theZ);

  /*/// Constructor with values.
  CONSTRUCTIONAPI_EXPORT
  ConstructionAPI_Point(const std::shared_ptr<ModelAPI_Feature>& theFeature,
                        const ModelHighAPI_Selection& theEdge,
                        const ModelHighAPI_Double& theDistanceValue,
                        const bool theDistancePercent = false,
                        const bool theReverse = false);

  /// Constructor with values.
  CONSTRUCTIONAPI_EXPORT
  ConstructionAPI_Point(const std::shared_ptr<ModelAPI_Feature>& theFeature,
                        const ModelHighAPI_Selection& theObject1,
                        const ModelHighAPI_Selection& theObject2);*/

  /// Destructor.
  CONSTRUCTIONAPI_EXPORT
  virtual ~ConstructionAPI_Point();

  INTERFACE_3(ConstructionPlugin_Point::ID(),
              x, ConstructionPlugin_Point::X(), ModelAPI_AttributeDouble, /** X attribute */,
              y, ConstructionPlugin_Point::Y(), ModelAPI_AttributeDouble, /** Y attribute */,
              z, ConstructionPlugin_Point::Z(), ModelAPI_AttributeDouble, /** Z attribute */)


  /// Set point values.
  CONSTRUCTIONAPI_EXPORT
  void setByXYZ(const ModelHighAPI_Double & theX,
                const ModelHighAPI_Double & theY,
                const ModelHighAPI_Double & theZ);

  /*/// Set edge and distance on it for point.
  CONSTRUCTIONAPI_EXPORT
  void setByDistanceOnEdge(const ModelHighAPI_Selection& theEdge,
                           const ModelHighAPI_Double& theDistanceValue,
                           const bool theDistancePercent = false,
                           const bool theReverse = false);

  /// Set point and plane for projection.
  CONSTRUCTIONAPI_EXPORT
  void setByProjection(const ModelHighAPI_Selection& theVertex,
                       const ModelHighAPI_Selection& theFace);

  /// Set lines for intersections.
  CONSTRUCTIONAPI_EXPORT
  void setByLinesIntersection(const ModelHighAPI_Selection& theEdge1,
                              const ModelHighAPI_Selection& theEdge2);

  /// Set line and plane for intersections.
  CONSTRUCTIONAPI_EXPORT
  void setByLineAndPlaneIntersection(const ModelHighAPI_Selection& theEdge,
                                     const ModelHighAPI_Selection& theFace);*/

  /// Dump wrapped feature
  CONSTRUCTIONAPI_EXPORT
  virtual void dump(ModelHighAPI_Dumper& theDumper) const;
};

/// Pointer on Point object.
typedef std::shared_ptr<ConstructionAPI_Point> PointPtr;

/// \ingroup CPPHighAPI
/// \brief Create Point feature
CONSTRUCTIONAPI_EXPORT
PointPtr addPoint(const std::shared_ptr<ModelAPI_Document> & thePart,
                  const ModelHighAPI_Double & theX,
                  const ModelHighAPI_Double & theY,
                  const ModelHighAPI_Double & theZ);

/*/// \ingroup CPPHighAPI
/// \brief Create Point feature
CONSTRUCTIONAPI_EXPORT
PointPtr addPoint(const std::shared_ptr<ModelAPI_Document> & thePart,
                  const ModelHighAPI_Selection& theEdge,
                  const ModelHighAPI_Double& theDistanceValue,
                  const bool theDistancePercent = false,
                  const bool theReverse = false);

/// \ingroup CPPHighAPI
/// \brief Create Point feature
CONSTRUCTIONAPI_EXPORT
PointPtr addPoint(const std::shared_ptr<ModelAPI_Document> & thePart,
                  const ModelHighAPI_Selection& theObject1,
                  const ModelHighAPI_Selection& theObject2);*/

#endif /* SRC_CONSTRUCTIONAPI_CONSTRUCTIONAPI_POINT_H_ */
