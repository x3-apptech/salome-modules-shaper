// File:        GeomAPI_PlanarEdges.hxx
// Created:     24 Jul 2014
// Author:      Artem ZHIDKOV

#ifndef GEOMAPI_WIRE_H_
#define GEOMAPI_WIRE_H_

#include "GeomAPI.h"
#include "GeomAPI_Edge.h"
#include "GeomAPI_Pnt.h"
#include "GeomAPI_Dir.h"

#include <memory>

#include <list>

/**\class GeomAPI_PlanarEdges
 * \ingroup DataModel
 * \brief Interface to the edge object
 */

class GeomAPI_PlanarEdges : public GeomAPI_Shape
{
 public:
  /// Creation of empty (null) shape
  GEOMAPI_EXPORT GeomAPI_PlanarEdges();

  GEOMAPI_EXPORT virtual bool isVertex() const
  {
    return false;
  }

  /// Returns whether the shape is an edge
  GEOMAPI_EXPORT virtual bool isEdge() const
  {
    return false;
  }

  GEOMAPI_EXPORT void addEdge(std::shared_ptr<GeomAPI_Shape> theEdge);
  GEOMAPI_EXPORT std::list<std::shared_ptr<GeomAPI_Shape> > getEdges();

  /// Returns True if the wire is defined in a plane
  GEOMAPI_EXPORT bool hasPlane() const { return myOrigin && myNorm && myDirX && myDirY; }

  /// Set/Get origin point
  GEOMAPI_EXPORT void setOrigin(const std::shared_ptr<GeomAPI_Pnt>& theOrigin) 
  { myOrigin = theOrigin; }
  GEOMAPI_EXPORT std::shared_ptr<GeomAPI_Pnt> origin() const { return myOrigin; }

  /// Set/Get X direction vector
  GEOMAPI_EXPORT void setDirX(const std::shared_ptr<GeomAPI_Dir>& theDirX) { myDirX = theDirX; }
  GEOMAPI_EXPORT std::shared_ptr<GeomAPI_Dir> dirX() const { return myDirX; }

  /// Set/Get Y direction vector
  GEOMAPI_EXPORT void setDirY(const std::shared_ptr<GeomAPI_Dir>& theDirY) { myDirY = theDirY; }
  GEOMAPI_EXPORT std::shared_ptr<GeomAPI_Dir> dirY() const { return myDirY; }

  /// Set/Get Normal direction vector
  GEOMAPI_EXPORT void setNorm(const std::shared_ptr<GeomAPI_Dir>& theNorm) { myNorm = theNorm; }
  GEOMAPI_EXPORT std::shared_ptr<GeomAPI_Dir> norm() const { return myNorm; }

private:
  std::shared_ptr<GeomAPI_Pnt> myOrigin;
  std::shared_ptr<GeomAPI_Dir> myDirX;
  std::shared_ptr<GeomAPI_Dir> myDirY;
  std::shared_ptr<GeomAPI_Dir> myNorm;
};

#endif

