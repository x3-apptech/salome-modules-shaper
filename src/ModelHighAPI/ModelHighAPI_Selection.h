// Name   : ModelHighAPI_Selection.h
// Purpose: 
//
// History:
// 06/06/16 - Sergey POKHODENKO - Creation of the file

#ifndef SRC_MODELHIGHAPI_MODELHIGHAPI_SELECTION_H_
#define SRC_MODELHIGHAPI_MODELHIGHAPI_SELECTION_H_

//--------------------------------------------------------------------------------------
#include "ModelHighAPI.h"

#include <memory>
#include <string>
#include <utility>
//--------------------------------------------------------------------------------------
class GeomAPI_Shape;
class ModelAPI_AttributeSelection;
class ModelAPI_AttributeSelectionList;
class ModelAPI_Result;
//--------------------------------------------------------------------------------------
typedef std::pair<std::shared_ptr<ModelAPI_Result>, std::shared_ptr<GeomAPI_Shape> > ResultSubShapePair;
typedef std::pair<std::string, std::string> TypeSubShapeNamePair;
//--------------------------------------------------------------------------------------
/**\class ModelHighAPI_Selection
 * \ingroup CPPHighAPI
 * \brief Class for filling ModelAPI_AttributeSelection
 */
class ModelHighAPI_Selection
{
public:
  enum VariantType {
    VT_Empty,
    VT_ResultSubShapePair,
    VT_TypeSubShapeNamePair
  };

public:
  /// Default constructor with empty selection.
  MODELHIGHAPI_EXPORT
  ModelHighAPI_Selection();

  /// Constructor for result and sub-shape
  MODELHIGHAPI_EXPORT
  ModelHighAPI_Selection(const std::shared_ptr<ModelAPI_Result>& theContext,
                         const std::shared_ptr<GeomAPI_Shape>& theSubShape = std::shared_ptr<GeomAPI_Shape>());
  /// Constructor for sub-shape by the textual Name
  MODELHIGHAPI_EXPORT
  ModelHighAPI_Selection(const std::string& theType,
                         const std::string& theSubShapeName);
  /// Destructor
  MODELHIGHAPI_EXPORT
  virtual ~ModelHighAPI_Selection();

  /// Fill attribute values
  MODELHIGHAPI_EXPORT
  virtual void fillAttribute(const std::shared_ptr<ModelAPI_AttributeSelection> & theAttribute) const;

  /// Append to list attribute
  MODELHIGHAPI_EXPORT
  virtual void appendToList(const std::shared_ptr<ModelAPI_AttributeSelectionList> & theAttribute) const;

  /// \return variant type.
  MODELHIGHAPI_EXPORT
  virtual VariantType variantType() const;

  /// \return pair of result and sub-shape.
  MODELHIGHAPI_EXPORT
  virtual ResultSubShapePair resultSubShapePair() const;

  /// \return pair of sub-shape type and name.
  MODELHIGHAPI_EXPORT
  virtual TypeSubShapeNamePair typeSubShapeNamePair() const;

  /// \return shape type.
  MODELHIGHAPI_EXPORT
  virtual std::string shapeType() const;

  /// \return Result if exists.
  MODELHIGHAPI_EXPORT
  virtual std::shared_ptr<ModelAPI_Result> result() const;

private:
  VariantType myVariantType;
  ResultSubShapePair myResultSubShapePair;
  TypeSubShapeNamePair myTypeSubShapeNamePair;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#endif /* SRC_MODELHIGHAPI_MODELHIGHAPI_SELECTION_H_ */
