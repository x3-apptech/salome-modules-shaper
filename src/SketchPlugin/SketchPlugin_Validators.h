// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:        SketchPlugin_Validators.h
// Created:     01 Aug 2014
// Author:      Vitaly SMETANNIKOV

#ifndef SketchPlugin_Validators_H
#define SketchPlugin_Validators_H

#include "SketchPlugin.h"
#include <ModelAPI_AttributeValidator.h>

/**\class SketchPlugin_DistanceAttrValidator
 * \ingroup Validators
 * \brief Validator for the distance input.
 *
 * It just checks that distance is greater than zero.
 */
class SketchPlugin_DistanceAttrValidator : public ModelAPI_AttributeValidator
{
 public:
  //! returns true if attribute is valid
  //! \param theAttribute the checked attribute
  //! \param theArguments arguments of the attribute
  virtual bool isValid(const AttributePtr& theAttribute,
                       const std::list<std::string>& theArguments) const;
};

#endif
