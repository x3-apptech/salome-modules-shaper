// Copyright (C) 2014-20xx CEA/DEN, EDF R&D
// Name   : ModelHighAPI_Interface.h
// Purpose:
//
// History:
// 17/05/16 - Sergey POKHODENKO - Creation of the file

#ifndef SRC_MODELHIGHAPI_MODELHIGHAPI_INTERFACE_H_
#define SRC_MODELHIGHAPI_MODELHIGHAPI_INTERFACE_H_

//--------------------------------------------------------------------------------------
#include "ModelHighAPI.h"

#include <list>
#include <map>
#include <memory>
#include <string>
#include <iostream>
//--------------------------------------------------------------------------------------
class ModelAPI_Feature;
class ModelAPI_Result;
class ModelHighAPI_Selection;
class ModelHighAPI_Dumper;
//--------------------------------------------------------------------------------------
/**\class ModelHighAPI_Interface
 * \ingroup CPPHighAPI
 * \brief Base class for feature interfaces
 */
class ModelHighAPI_Interface
{
public:
  /// Constructor
  MODELHIGHAPI_EXPORT
  explicit ModelHighAPI_Interface(const std::shared_ptr<ModelAPI_Feature> & theFeature);
  /// Destructor
  MODELHIGHAPI_EXPORT
  virtual ~ModelHighAPI_Interface();

  /// Return ModelAPI_Feature
  MODELHIGHAPI_EXPORT
  std::shared_ptr<ModelAPI_Feature> feature() const;

  /// If feature is composite return intefrace for sub-feature by zero-based index,
  /// or empty pointer if feature not composite or does not have sub-feature with such index.
  MODELHIGHAPI_EXPORT
  std::shared_ptr<ModelHighAPI_Interface> subFeature(const int theIndex) const;

  /// Shortcut for feature()->getKind()
  MODELHIGHAPI_EXPORT
  const std::string& getKind() const;

  /// Shortcut for feature()->execute()
  /// \param isForce start execution of feature instead of sending events
  MODELHIGHAPI_EXPORT
  void execute(bool isForce = false);

  /// Shortcut for feature()->data()->setName()
  MODELHIGHAPI_EXPORT
  void setName(const std::string& theName);

  // TODO(spo): rename to selectAll()
  /// Return all objects of the feature
  MODELHIGHAPI_EXPORT
  virtual std::list<ModelHighAPI_Selection> result() const;

  /// Return default result. Default implementation feature()->lastResult()
  MODELHIGHAPI_EXPORT
  virtual std::shared_ptr<ModelAPI_Result> defaultResult() const;

  /// Throw exception to event loop
  MODELHIGHAPI_EXPORT
  void throwException(const std::string & theDescription);

  /// Return name of getter for specified attribute
  MODELHIGHAPI_EXPORT
  const std::string& attributeGetter(const std::string& theAttrName);

  /// Dump wrapped feature
  MODELHIGHAPI_EXPORT
  virtual void dump(ModelHighAPI_Dumper& theDumper) const {}

protected:
  std::shared_ptr<ModelAPI_Feature> myFeature; ///< feature of this interface

  std::map<std::string, std::string> myAttrGetter; ///< names of attributes and their getters
};

//! Pointer on Interface object
typedef std::shared_ptr<ModelHighAPI_Interface> InterfacePtr;

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#endif /* SRC_MODELHIGHAPI_MODELHIGHAPI_INTERFACE_H_ */
