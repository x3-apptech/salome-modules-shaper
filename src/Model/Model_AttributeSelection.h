// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        Model_AttributeSelection.h
// Created:     8 May 2014
// Author:      Mikhail PONIKAROV

#ifndef Model_AttributeSelection_H_
#define Model_AttributeSelection_H_

#include "Model.h"
#include "Model_AttributeReference.h"
#include <ModelAPI_AttributeSelection.h>

/**\class Model_AttributeSelection
 * \ingroup DataModel
 * \brief Attribute that contains reference to the sub-shape of some result, the selected shape.
 */

class Model_AttributeSelection : public ModelAPI_AttributeSelection
{
  Model_AttributeReference myRef;  ///< The reference functionality reusage
public:
  /// Defines the result and its selected sub-shape
  MODEL_EXPORT virtual void setValue(
    const ResultPtr& theContext, const std::shared_ptr<GeomAPI_Shape>& theSubShape);

  /// Returns the selected subshape
  MODEL_EXPORT virtual std::shared_ptr<GeomAPI_Shape> value();

  /// Returns the context of the selection (the whole shape owner)
  MODEL_EXPORT virtual ResultPtr context();

  /// Sets the feature object
  MODEL_EXPORT virtual void setObject(const std::shared_ptr<ModelAPI_Object>& theObject);

  /// Updates the underlied selection due to the changes in the referenced objects
  /// \returns false if update is failed
  MODEL_EXPORT virtual bool update();

protected:
  /// Objects are created for features automatically
  MODEL_EXPORT Model_AttributeSelection(TDF_Label& theLabel);
    /// Performs the selection for the body result (TNaming Selection)

  /// Performs the selection for the body result (TNaming selection)
  virtual void selectBody(
    const ResultPtr& theContext, const std::shared_ptr<GeomAPI_Shape>& theSubShape);
  /// Performs the selection for the construction result (selection by index)
  virtual void selectConstruction(
    const ResultPtr& theContext, const std::shared_ptr<GeomAPI_Shape>& theSubShape);

  /// Returns the label where TNaming_Selection results are stored
  /// Note: there must be no attributes stored at the same label because Selector clears this lab
  TDF_Label selectionLabel();

  friend class Model_Data;
  friend class Model_AttributeSelectionList;
};

#endif
