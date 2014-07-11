// File:        ModelAPI_ResultConstruction.h
// Created:     07 Jul 2014
// Author:      Mikhail PONIKAROV

#ifndef ModelAPI_ResultConstruction_HeaderFile
#define ModelAPI_ResultConstruction_HeaderFile

#include "ModelAPI_Result.h"
#include <GeomAPI_Shape.h>

/**\class ModelAPI_ResultConstruction
 * \ingroup DataModel
 * \brief The construction element result of a feature.
 *
 * Provides a shape that may be displayed in the viewer.
 * Intermediate, light result that in many cases produces a result on the fly.
 */
class ModelAPI_ResultConstruction : public ModelAPI_Result
{
public:
  /// Returns the group identifier of this result
  virtual std::string group()
    {static std::string MY_GROUP = "Construction"; return MY_GROUP;}

  /// Returns the shape-result produced by this feature
  virtual boost::shared_ptr<GeomAPI_Shape>& shape() = 0;

  /// Sets the result
  virtual void setShape(boost::shared_ptr<GeomAPI_Shape> theShape) = 0;
};

//! Pointer on feature object
typedef boost::shared_ptr<ModelAPI_ResultConstruction> ResultConstructionPtr;

#endif
