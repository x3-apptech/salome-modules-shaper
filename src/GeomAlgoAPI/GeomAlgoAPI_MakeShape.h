// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAlgoAPI_MakeShape.h
// Created:     17 Oct 2014
// Author:      Sergey ZARITCHNY
//
// Modified by Clarisse Genrault (CEA) : 17 Mar 2016

#ifndef GeomAlgoAPI_MakeShape_H_
#define GeomAlgoAPI_MakeShape_H_

#include <GeomAlgoAPI.h>
#include <GeomAPI_DataMapOfShapeShape.h>

#include <list>
#include <memory>
#include <map>

/// \class GeomAlgoAPI_MakeShape
/// \ingroup DataAlgo
/// \brief Interface to the root class of all topological shapes constructions
class GeomAlgoAPI_MakeShape : public GeomAPI_Interface
{
public:
  /// Builder type enum
  enum BuilderType {
    Unknown,
    OCCT_BRepBuilderAPI_MakeShape,
    OCCT_BOPAlgo_Builder
  };

public:
  /// \brief Empty constructor.
  GEOMALGOAPI_EXPORT GeomAlgoAPI_MakeShape();

  /// \brief Constructor by builder and builder type.
  /// \param[in] theBuilder pointer to the builder.
  /// \param[in] theBuilderType builder type.
  template<class T> explicit GeomAlgoAPI_MakeShape(T* theBuilder, const BuilderType theBuilderType = OCCT_BRepBuilderAPI_MakeShape)
  : GeomAPI_Interface(theBuilder),
    myBuilderType(theBuilderType)
  {
    initialize();
  }

  /// \brief Initializes internals.
  /// \param[in] theBuilder pointer to the builder.
  /// \param[in] theBuilderType builder type.
  template<class T> void initialize(T* theBuilder, const BuilderType theBuilderType = OCCT_BRepBuilderAPI_MakeShape)
  {
    setImpl(theBuilder);
    myBuilderType = theBuilderType;
    initialize();
  }

  /// \return status of builder.
  GEOMALGOAPI_EXPORT bool isDone() const;

  /// \return a shape built by the shape construction algorithm.
  GEOMALGOAPI_EXPORT virtual const std::shared_ptr<GeomAPI_Shape> shape() const;

  /// \return true if resulting shape is valid.
  GEOMALGOAPI_EXPORT bool isValid() const;

  /// \return true if resulting shape has volume.
  GEOMALGOAPI_EXPORT bool hasVolume() const;

  /// \return map of sub-shapes of the result. To be used for History keeping.
  GEOMALGOAPI_EXPORT std::shared_ptr<GeomAPI_DataMapOfShapeShape> mapOfSubShapes() const;

  /// \return the list of shapes generated from the shape \a theShape.
  /// \param[in] theShape base shape.
  /// \param[out] theHistory generated shapes.
  GEOMALGOAPI_EXPORT virtual void generated(const std::shared_ptr<GeomAPI_Shape> theShape,
                                            ListOfShape& theHistory);

  /// \return the list of shapes modified from the shape \a theShape.
  /// \param[in] theShape base shape.
  /// \param[out] theHistory modified shapes.
  GEOMALGOAPI_EXPORT virtual void modified(const std::shared_ptr<GeomAPI_Shape> theShape,
                                           ListOfShape& theHistory);

  /// \return true if theShape was deleted.
  /// \param[in] theShape base shape.
  GEOMALGOAPI_EXPORT virtual bool isDeleted(const std::shared_ptr<GeomAPI_Shape> theShape);

  /// \return true if the data were correct.
  GEOMALGOAPI_EXPORT virtual bool check() { return true; };

  ///  \return the list of created faces.
  GEOMALGOAPI_EXPORT std::map< std::string, std::shared_ptr<GeomAPI_Shape> > getCreatedFaces() {return myCreatedFaces;}

  /// \return the error.
  GEOMALGOAPI_EXPORT std::string getError() { return myError; }

  /// \brief Prepare the naming of faces.
  GEOMALGOAPI_EXPORT virtual void prepareNamingFaces();

  /// \brief Check the validity of the produced shape.
  GEOMALGOAPI_EXPORT bool checkValid(std::string theMessage);

protected:
  /// \brief Sets builder type.
  /// \param[in] theBuilderType new builder type.
  void setBuilderType(const BuilderType theBuilderType);

  /// \brief Sets status of builder.
  /// \param[in] theFlag new status.
  void setDone(const bool theFlag);

  /// \brief Sets result shape.
  /// \param[in] theShape new shape.
  void setShape(const std::shared_ptr<GeomAPI_Shape> theShape);

protected:
  std::shared_ptr<GeomAPI_DataMapOfShapeShape> myMap; ///< Data map to keep correct orientation of sub-shapes.
  std::string myError; /// Error occurred during the execution of an algorithm.
  std::map< std::string, std::shared_ptr<GeomAPI_Shape> > myCreatedFaces; /// Map of created faces with their name for naming.
  
private:
  /// \brief Initializes internals.
  void initialize();

private:
  GeomAlgoAPI_MakeShape::BuilderType myBuilderType; ///< Type of make shape builder.
  bool myDone; ///< Builder status.
  std::shared_ptr<GeomAPI_Shape> myShape; ///< Resulting shape.
};

typedef std::list<std::shared_ptr<GeomAlgoAPI_MakeShape> > ListOfMakeShape;

#endif
