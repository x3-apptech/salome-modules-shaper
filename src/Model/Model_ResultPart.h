// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ModelAPI_ResultPart.hxx
// Created:     07 Jul 2014
// Author:      Mikhail PONIKAROV

#ifndef Model_ResultPart_H_
#define Model_ResultPart_H_

#include "Model.h"
#include <ModelAPI_ResultPart.h>

/**\class Model_ResultPart
 * \ingroup DataModel
 * \brief The Part document, result of a creation of new part feature.
 *
 * This result leaves without feature: no parametricity for this element,
 * only add/remove, undo/redo.
 */
class Model_ResultPart : public ModelAPI_ResultPart
{
 public:
  /// Returns the part-document of this result
  MODEL_EXPORT virtual std::shared_ptr<ModelAPI_Document> partDoc();
  /// Part has no stored feature: this method returns NULL
  MODEL_EXPORT virtual std::shared_ptr<ModelAPI_Feature> owner();

  /// Sets this document as current and if it is not loaded yet, loads it
  MODEL_EXPORT virtual void activate();

  /// disable all feature of the part on disable of the part result
  MODEL_EXPORT virtual bool setDisabled(std::shared_ptr<ModelAPI_Result> theThis,
    const bool theFlag);

  /// Result shape of part document is compound of bodies inside of this part
  MODEL_EXPORT virtual std::shared_ptr<GeomAPI_Shape> shape();

  /// Returns the parameters of color definition in the resources config manager
  MODEL_EXPORT virtual void colorConfigInfo(std::string& theSection, std::string& theName,
                                            std::string& theDefault);

protected:
  /// makes a result on a temporary feature (an action)
  Model_ResultPart();

  /// Sets the data manager of an object (document does), here also attributes are initialized
  virtual void setData(std::shared_ptr<ModelAPI_Data> theData);

  /// Returns true if document is activated (loaded into the memory)
  virtual bool isActivated();

  friend class Model_Objects;
};

#endif
