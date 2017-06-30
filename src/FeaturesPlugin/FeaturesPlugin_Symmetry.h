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

#ifndef FEATURESPLUGIN_SYMMETRY_H_
#define FEATURESPLUGIN_SYMMETRY_H_

#include <FeaturesPlugin.h>

#include <ModelAPI_Feature.h>

#include <GeomAlgoAPI_Symmetry.h>

/** \class FeaturesPlugin_Symmetry
 *  \ingroup Plugins
 *  \brief Feature that performs reflection with respect to a point, axis, or plane.
 */
class FeaturesPlugin_Symmetry : public ModelAPI_Feature
{
 public:
  /// Symmetry kind.
  inline static const std::string& ID()
  {
    static const std::string MY_SYMMETRY_ID("Symmetry");
    return MY_SYMMETRY_ID;
  }

  /// Attribute name for creation method.
  inline static const std::string& CREATION_METHOD()
  {
    static const std::string MY_CREATION_METHOD_ID("CreationMethod");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByPoint".
  inline static const std::string& CREATION_METHOD_BY_POINT()
  {
    static const std::string MY_CREATION_METHOD_ID("ByPoint");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByAxis".
  inline static const std::string& CREATION_METHOD_BY_AXIS()
  {
    static const std::string MY_CREATION_METHOD_ID("ByAxis");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByPlane".
  inline static const std::string& CREATION_METHOD_BY_PLANE()
  {
    static const std::string MY_CREATION_METHOD_ID("ByPlane");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name of referenced objects.
  inline static const std::string& OBJECTS_LIST_ID()
  {
    static const std::string MY_OBJECTS_LIST_ID("main_objects");
    return MY_OBJECTS_LIST_ID;
  }

  /// Attribute name of a point.
  inline static const std::string& POINT_OBJECT_ID()
  {
    static const std::string MY_POINT_OBJECT_ID("point_object");
    return MY_POINT_OBJECT_ID;
  }

  /// Attribute name of an axis.
  inline static const std::string& AXIS_OBJECT_ID()
  {
    static const std::string MY_AXIS_OBJECT_ID("axis_object");
    return MY_AXIS_OBJECT_ID;
  }

  /// Attribute name of a plane.
  inline static const std::string& PLANE_OBJECT_ID()
  {
    static const std::string MY_PLANE_OBJECT_ID("plane_object");
    return MY_PLANE_OBJECT_ID;
  }

  /// \return the kind of a feature.
  FEATURESPLUGIN_EXPORT virtual const std::string& getKind()
  {
    static std::string MY_KIND = FeaturesPlugin_Symmetry::ID();
    return MY_KIND;
  }

  /// Creates a new part document if needed.
  FEATURESPLUGIN_EXPORT virtual void execute();

  /// Request for initialization of data model of the feature: adding all attributes.
  FEATURESPLUGIN_EXPORT virtual void initAttributes();

  /// Use plugin manager for features creation.
  FeaturesPlugin_Symmetry();

private:
  /// Perform symmetry with respect to a point.
  void performSymmetryByPoint();

  /// Perform symmetry with respect to an axis.
  void performSymmetryByAxis();

  /// Perform symmetry with respect to a plane.
  void performSymmetryByPlane();

  /// Perform the naming
  void loadNamingDS(GeomAlgoAPI_Symmetry& theSymmetryAlgo,
                    std::shared_ptr<ModelAPI_ResultBody> theResultBody,
                    std::shared_ptr<GeomAPI_Shape> theBaseShape);
};

#endif // FEATURESPLUGIN_SYMMETRY_H_
