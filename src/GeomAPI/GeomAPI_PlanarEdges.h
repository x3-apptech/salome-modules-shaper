// File:        GeomAPI_PlanarEdges.hxx
// Created:     24 Jul 2014
// Author:      Artem ZHIDKOV

#ifndef GEOMAPI_WIRE_H_
#define GEOMAPI_WIRE_H_

#include "GeomAPI.h"
#include "GeomAPI_Edge.h"
#include "GeomAPI_Pnt.h"
#include "GeomAPI_Dir.h"

#include <boost/smart_ptr/shared_ptr.hpp>

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

  GEOMAPI_EXPORT virtual bool isVertex() const;

  /// Returns whether the shape is an edge
  GEOMAPI_EXPORT virtual bool isEdge() const;

  GEOMAPI_EXPORT void addEdge(boost::shared_ptr<GeomAPI_Shape> theEdge);
  GEOMAPI_EXPORT std::list<boost::shared_ptr<GeomAPI_Shape> > getEdges();

  /// Returns True if the wire is defined in a plane
  GEOMAPI_EXPORT bool hasPlane() const;

  /// Set/Get origin point
  GEOMAPI_EXPORT void setOrigin(const boost::shared_ptr<GeomAPI_Pnt>& theOrigin);
  GEOMAPI_EXPORT boost::shared_ptr<GeomAPI_Pnt> origin() const;

  /// Set/Get X direction vector
  GEOMAPI_EXPORT void setDirX(const boost::shared_ptr<GeomAPI_Dir>& theDirX);
  GEOMAPI_EXPORT boost::shared_ptr<GeomAPI_Dir> dirX() const;

  /// Set/Get Y direction vector
  GEOMAPI_EXPORT void setDirY(const boost::shared_ptr<GeomAPI_Dir>& theDirY);
  GEOMAPI_EXPORT boost::shared_ptr<GeomAPI_Dir> dirY() const;

  /// Set/Get Normal direction vector
  GEOMAPI_EXPORT void setNorm(const boost::shared_ptr<GeomAPI_Dir>& theNorm);
  GEOMAPI_EXPORT boost::shared_ptr<GeomAPI_Dir> norm() const;

private:
  boost::shared_ptr<GeomAPI_Pnt> myOrigin;
  boost::shared_ptr<GeomAPI_Dir> myDirX;
  boost::shared_ptr<GeomAPI_Dir> myDirY;
  boost::shared_ptr<GeomAPI_Dir> myNorm;
};

#endif

