// File:        Model_Data.hxx
// Created:     21 Mar 2014
// Author:      Mikhail PONIKAROV

#ifndef Model_Data_HeaderFile
#define Model_Data_HeaderFile

#include "Model.h"
#include <ModelAPI_Data.h>
#include <TDF_Label.hxx>

#include <map>

class ModelAPI_Attribute;
class ModelAPI_Feature;

/**\class Model_Data
 * \ingroup DataModel
 * \brief General object of the application that allows
 * to get/set attributes from the document and compute result of an operation.
 */

class Model_Data: public ModelAPI_Data
{
  TDF_Label myLab; ///< label of the feature in the document
  /// All attributes of the object identified by the attribute ID
  std::map<std::string, boost::shared_ptr<ModelAPI_Attribute> > myAttrs;

  /// needed here to emit signal that feature changed on change of the attribute
  boost::shared_ptr<ModelAPI_Feature> myFeature;

  Model_Data();

  TDF_Label label() {return myLab;}

  friend class Model_Document;

public:
  /// Returns the name of the feature visible by the user in the object browser
  MODEL_EXPORT virtual std::string getName();
  /// Defines the name of the feature visible by the user in the object browser
  MODEL_EXPORT virtual void setName(std::string theName);
  /// Returns the attribute that references to another document
  MODEL_EXPORT virtual boost::shared_ptr<ModelAPI_AttributeDocRef> docRef(const std::string theID);
  /// Returns the attribute that contains real value with double precision
  MODEL_EXPORT virtual boost::shared_ptr<ModelAPI_AttributeDouble> real(const std::string theID);
  /// Returns the generic attribute by identifier
  /// \param theID identifier of the attribute
  MODEL_EXPORT virtual boost::shared_ptr<ModelAPI_Attribute> attribute(const std::string theID);

  /// Initializes object by the attributes: must be called just after the object is created
  /// for each attribute of the object
  /// \param theID identifier of the attribute that can be referenced by this ID later
  /// \param theAttrType type of the created attribute (received from the type method)
  MODEL_EXPORT virtual void addAttribute(std::string theID, std::string theAttrType);

  /// Puts feature to the document data sub-structure
  MODEL_EXPORT void setLabel(TDF_Label& theLab);

  /// Sets the feature of this data
  MODEL_EXPORT virtual void setFeature(boost::shared_ptr<ModelAPI_Feature> theFeature)
    {myFeature = theFeature;}
};

#endif
