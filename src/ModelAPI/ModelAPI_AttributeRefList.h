// File:        ModelAPI_AttributeRefList.h
// Created:     8 May 2014
// Author:      Mikhail PONIKAROV

#ifndef ModelAPI_AttributeRefList_HeaderFile
#define ModelAPI_AttributeRefList_HeaderFile

#include "ModelAPI_Attribute.h"
#include <list>

/**\class ModelAPI_AttributeRefList
 * \ingroup DataModel
 * \brief Attribute that contains list of references to features (located in the same document).
 */

class ModelAPI_AttributeRefList : public ModelAPI_Attribute
{
public:
  /// Returns the type of this class of attributes
  MODELAPI_EXPORT static std::string type() {return "RefList";}

  /// Returns the type of this class of attributes, not static method
  MODELAPI_EXPORT virtual std::string attributeType() {return type();}

  /// Appends the feature to the end of a list
  MODELAPI_EXPORT virtual void append(boost::shared_ptr<ModelAPI_Feature> theFeature) = 0;

  /// Erases the first meet of the feature in the list
  MODELAPI_EXPORT virtual void remove(boost::shared_ptr<ModelAPI_Feature> theFeature) = 0;

  /// Returns number of features in the list
  MODELAPI_EXPORT virtual int size() = 0;

  /// Returns the list of features
  MODELAPI_EXPORT virtual std::list<boost::shared_ptr<ModelAPI_Feature> > list() = 0;

protected:
  /// Objects are created for features automatically
  MODELAPI_EXPORT ModelAPI_AttributeRefList()
  {}
};

#endif
