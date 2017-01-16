// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        Model_ResultField.h
// Created:     16 Nov 2016
// Author:      Mikhail PONIKAROV

#ifndef Model_ResultField_H_
#define Model_ResultField_H_

#include "Model.h"
#include <ModelAPI_ResultField.h>
#include <vector>

/**\class Model_ResultField
 * \ingroup DataModel
 * \brief The fields result.
 *
 * Provides a compound of selected elements, without storage, one the fly.
 */
class Model_ResultField : public ModelAPI_ResultField
{
  std::shared_ptr<ModelAPI_Data> myOwnerData;  ///< data of owner of this result
public:

  class Model_FieldStep : public ModelAPI_ResultField::ModelAPI_FieldStep
  {
  public:
    Model_FieldStep(ModelAPI_ResultField* theParent, int theId)
      : myParent(theParent), myId(theId) {};

    virtual ModelAPI_ResultField* field() const { return myParent; }

    virtual int id() const { return myId; }

  private:
    ModelAPI_ResultField* myParent;
    int myId;
  };

  /// Retuns the parameters of color definition in the resources config manager
  MODEL_EXPORT virtual void colorConfigInfo(std::string& theSection, std::string& theName,
                                            std::string& theDefault);

  /// Returns the compound of selected entities
  MODEL_EXPORT virtual std::shared_ptr<GeomAPI_Shape> shape();

  /// Returns number of steps
  MODEL_EXPORT virtual int stepsSize() const;

  /// Returns a text line by its number
  /// \param theLine a number of line
  MODEL_EXPORT virtual std::string textLine(int theLine) const;

  /// Returns step object
  /// \param theId an id of the object
  MODEL_EXPORT virtual ModelAPI_ResultField::ModelAPI_FieldStep* step(int theId) const;

  /// Removes the stored builders
  MODEL_EXPORT virtual ~Model_ResultField();

protected:
  /// Makes a body on the given feature data
  Model_ResultField(std::shared_ptr<ModelAPI_Data> theOwnerData);

  friend class Model_Objects;

private:
  void updateSteps();

  std::vector<ModelAPI_FieldStep*> mySteps;
};

#endif