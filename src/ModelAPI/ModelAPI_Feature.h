// File:        ModelAPI_Feature.hxx
// Created:     21 Mar 2014
// Author:      Mikhail PONIKAROV

#ifndef ModelAPI_Feature_HeaderFile
#define ModelAPI_Feature_HeaderFile

#include "ModelAPI.h"
#include "ModelAPI_PluginManager.h"

#include <string>
#include <memory>

class ModelAPI_Data;
class ModelAPI_Document;

/**\class ModelAPI_Feature
 * \ingroup DataModel
 * \brief Functionality of the model object: to update result,
 * to initialize attributes, etc.
 */
class ModelAPI_Feature
{
  std::shared_ptr<ModelAPI_Data> myData; ///< manager of the data model of a feature

public:
  /// Returns the kind of a feature (like "Point")
  MODELAPI_EXPORT virtual const std::string& getKind() = 0;

  /// Returns to which group in the document must be added feature
  MODELAPI_EXPORT virtual const std::string& getGroup() = 0;

  /// Request for initialization of data model of the feature: adding all attributes
  MODELAPI_EXPORT virtual void initAttributes() = 0;

  /// Computes or recomputes the result
  MODELAPI_EXPORT virtual void execute() = 0;

  /// Returns the data manager of this feature
  MODELAPI_EXPORT virtual std::shared_ptr<ModelAPI_Data> data() {return myData;}

  /// Must return document where the new feature must be added to
  /// By default it is current document
  MODELAPI_EXPORT virtual std::shared_ptr<ModelAPI_Document> documentToAdd()
  {return ModelAPI_PluginManager::get()->currentDocument();}

  /// To virtually destroy the fields of successors
  virtual ~ModelAPI_Feature() {}

protected:
  /// Use plugin manager for features creation: this method is 
  /// defined here only for SWIG-wrapping
  ModelAPI_Feature()
  {}

  /// Sets the data manager of an object (document does)
  MODELAPI_EXPORT void setData(std::shared_ptr<ModelAPI_Data> theData) {myData = theData;}
  friend class Model_Document;
};

#endif
