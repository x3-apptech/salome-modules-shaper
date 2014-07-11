// File:        ModelAPI_ResultPart.hxx
// Created:     07 Jul 2014
// Author:      Mikhail PONIKAROV

#ifndef Model_ResultPart_HeaderFile
#define Model_ResultPart_HeaderFile

#include "Model.h"
#include <ModelAPI_ResultPart.h>

/**\class ModelAPI_ResultPart
 * \ingroup DataModel
 * \brief The Part document, result of a creation of new part feature.
 *
 * This result leaves without feature: no parametricity for this element,
 * only add/remove, undo/redo.
 */
class Model_ResultPart : public ModelAPI_ResultPart
{
public:
  /// Returns the group identifier of this result
  virtual std::string group()
    {static std::string MY_GROUP = "Parts"; return MY_GROUP;}
  /// Returns the part-document of this result
  MODEL_EXPORT virtual boost::shared_ptr<ModelAPI_Document> partDoc();
  /// Part has no stored feature: this method returns NULL
  MODEL_EXPORT virtual boost::shared_ptr<ModelAPI_Feature> owner();

protected:
  /// makes a result on a temporary feature (an action)
  Model_ResultPart(const boost::shared_ptr<ModelAPI_Feature>& theFeature);

  friend class Model_Document;
};

#endif
