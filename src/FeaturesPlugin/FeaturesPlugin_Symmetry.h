// Copyright (C) 2014-2016 CEA/DEN, EDF R&D

// File:        FeaturesPlugin_Symmetry.h
// Created:     30 Nov 2016
// Author:      Clarisse Genrault (CEA)

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
    static const std::string MY_SYMMETRY("Symmetry");
    return MY_SYMMETRY;
  }

  /// Attribute name for creation method.
  inline static const std::string& CREATION_METHOD()
  {
    static const std::string MY_CREATION_METHOD_ID("CreationMethod");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByAxisAndDistance".
  inline static const std::string& CREATION_METHOD_BY_POINT()
  {
    static const std::string MY_CREATION_METHOD_ID("ByPoint");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByDimensions".
  inline static const std::string& CREATION_METHOD_BY_AXIS()
  {
    static const std::string MY_CREATION_METHOD_ID("ByAxis");
    return MY_CREATION_METHOD_ID;
  }

  /// Attribute name for creation method "ByTwoPoints".
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
  ///Perform symmetry with respect to a point.
  void performSymmetryByPoint();

  ///Perform symmetry with respect to an axis.
  void performSymmetryByAxis();

  ///Perform symmetry with respect to a plane.
  void performSymmetryByPlane();

  void loadNamingDS(GeomAlgoAPI_Symmetry& theSymmetryAlgo,
                    std::shared_ptr<ModelAPI_ResultBody> theResultBody,
                    std::shared_ptr<GeomAPI_Shape> theBaseShape);
};

#endif // FEATURESPLUGIN_SYMMETRY_H_