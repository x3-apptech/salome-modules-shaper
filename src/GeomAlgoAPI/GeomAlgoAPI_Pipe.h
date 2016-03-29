// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAlgoAPI_Pipe.h
// Created:     16 March 2016
// Author:      Dmitry Bobylev

#ifndef GeomAlgoAPI_Pipe_H_
#define GeomAlgoAPI_Pipe_H_

#include "GeomAlgoAPI.h"

#include "GeomAlgoAPI_MakeSweep.h"

#include <GeomAPI_Shape.h>

/// \class GeomAlgoAPI_Pipe
/// \ingroup DataAlgo
/// \brief Allows to create extrusion of objects along a path. It produces the following results from objects:\n
/// Vertex -> Edge\n
/// Edge -> Face\n
/// Wire -> Shell\n
/// Face -> Solid
class GeomAlgoAPI_Pipe : public GeomAlgoAPI_MakeSweep
{
public:
  /// \brief Creates extrusion for the given shape along a path.
  /// \param[in] theBaseShape base shape(vertex, edge, wire of face).
  /// \param[in] thePathShape path shape(edge or wire).
  GEOMALGOAPI_EXPORT GeomAlgoAPI_Pipe(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                                      const std::shared_ptr<GeomAPI_Shape> thePathShape);

  /// \brief Creates extrusion for the given shape along a path.
  /// \param[in] theBaseShape base shape(vertex, edge, wire of face).
  /// \param[in] thePathShape path shape(edge or wire).
  /// \param[in] theBiNormal edge or wire to preserve the constant angle between the normal vector
  /// to the base object and the BiNormal vector.
  GEOMALGOAPI_EXPORT GeomAlgoAPI_Pipe(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
                                      const std::shared_ptr<GeomAPI_Shape> thePathShape,
                                      const std::shared_ptr<GeomAPI_Shape> theBiNormal);

  /// \brief Creates extrusion for the given shape along a path.
  /// \param[in] theBaseShapes base shape(vertex, edge, wire of face).
  /// \param[in] theLocations vertexes on the path. Should be empty or same size as theBaseShapes.
  /// \param[in] thePathShape path shape(edge or wire).
  /// to the base object and the BiNormal vector.
  GEOMALGOAPI_EXPORT GeomAlgoAPI_Pipe(const ListOfShape& theBaseShapes,
                                      const ListOfShape& theLocations,
                                      const std::shared_ptr<GeomAPI_Shape> thePathShape);

  /// \return the list of shapes generated from theShape.
  /// \param[in] theShape base shape.
  /// \param[out] theHistory generated shapes.
  GEOMALGOAPI_EXPORT void generated(const std::shared_ptr<GeomAPI_Shape> theShape,
                                    ListOfShape& theHistory);

private:
  void build(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
             const std::shared_ptr<GeomAPI_Shape> thePathShape);

  void build(const std::shared_ptr<GeomAPI_Shape> theBaseShape,
             const std::shared_ptr<GeomAPI_Shape> thePathShape,
             const std::shared_ptr<GeomAPI_Shape> theBiNormal);

  void build(const ListOfShape& theBaseShapes,
             const ListOfShape& theLocations,
             const std::shared_ptr<GeomAPI_Shape> thePathShape);

private:
  //bool myIsPipeShellUsed;
  std::shared_ptr<GeomAPI_Shape> myBaseShape;
  std::shared_ptr<GeomAPI_Shape> myPathShape;
};

#endif