// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAlgoAPI_MakeShape.h
// Created:     17 Oct 2014
// Author:      Sergey ZARITCHNY
#ifndef GeomAlgoAPI_MakeShape_H_
#define GeomAlgoAPI_MakeShape_H_

#include <GeomAlgoAPI.h>
#include <GeomAPI_DataMapOfShapeShape.h>

#include <list>
#include <memory>

/** \class GeomAlgoAPI_MakeShape
 *  \ingroup DataAlgo
 *  \brief Interface to the root class of all topological shapes constructions
 */
class GeomAlgoAPI_MakeShape : public GeomAPI_Interface
{
public:
  /// Builder type enum
  enum BuilderType {
    OCCT_BRepBuilderAPI_MakeShape,
    OCCT_BOPAlgo_Builder
  };

public:
  /** \brief Constructor by builder and builder type.
   *  \param[in] theBuilder pointer to the builder.
   *  \param[in] theBuilderType builder type.
   */
  template<class T> explicit GeomAlgoAPI_MakeShape(T* theBuilder, const BuilderType theBuilderType = OCCT_BRepBuilderAPI_MakeShape)
  : GeomAPI_Interface(theBuilder),
    myBuilderType(theBuilderType),
    myShape(new GeomAPI_Shape())
  {
    initialize();
  }


  /// \return status of builder.
  GEOMALGOAPI_EXPORT bool isDone() const;

  /// \return a shape built by the shape construction algorithm.
  GEOMALGOAPI_EXPORT virtual const std::shared_ptr<GeomAPI_Shape> shape() const;

  /** \return the list of shapes generated from the shape \a theShape.
   *  \param[in] theShape base shape.
   *  \param[out] theHistory generated shapes.
   */
  GEOMALGOAPI_EXPORT virtual void generated(const std::shared_ptr<GeomAPI_Shape> theShape,
                                            ListOfShape& theHistory);

  /** \return the list of shapes modified from the shape \a theShape.
   *  \param[in] theShape base shape.
   *  \param[out] theHistory modified shapes.
   */
  GEOMALGOAPI_EXPORT virtual void modified(const std::shared_ptr<GeomAPI_Shape> theShape,
                                           ListOfShape& theHistory);

  /** \return true if theShape was deleted.
   *  \param[in] theShape base shape.
   */
  GEOMALGOAPI_EXPORT virtual bool isDeleted(const std::shared_ptr<GeomAPI_Shape> theShape);

protected:
  /// \brief Default constructor.
  GEOMALGOAPI_EXPORT GeomAlgoAPI_MakeShape();

  /** \brief Sets builder type.
   *  \param[in] theBuilderType new builder type.
   */
  GEOMALGOAPI_EXPORT void setBuilderType(const BuilderType theBuilderType);

  /** \brief Sets status of builder.
   *  \param[in] theFlag new status.
   */
  GEOMALGOAPI_EXPORT void setDone(const bool theFlag);

  /** \brief Sets result shape.
   *  \param[in] theShape new shape.
   */
  GEOMALGOAPI_EXPORT void setShape(const std::shared_ptr<GeomAPI_Shape> theShape);

  /// \brief Initializes internals.
  GEOMALGOAPI_EXPORT void initialize();

private:
  GeomAlgoAPI_MakeShape::BuilderType myBuilderType; ///< Type of make shape builder.
  bool myDone; ///< Builder status.
  std::shared_ptr<GeomAPI_Shape> myShape; ///< Resulting shape.
  std::shared_ptr<GeomAPI_DataMapOfShapeShape> myMap; ///< Data map to keep correct orientation of sub-shapes.
};

typedef std::list<std::shared_ptr<GeomAlgoAPI_MakeShape> > ListOfMakeShape;

#endif
