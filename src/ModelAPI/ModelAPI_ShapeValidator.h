// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ModelAPI_ShapeValidator.h
// Created:     2 Feb 2015
// Author:      Natalia ERMOLAEVA

#ifndef ModelAPI_ShapeValidator_H
#define ModelAPI_ShapeValidator_H

#include <ModelAPI.h>

#include <ModelAPI_AttributeValidator.h>
#include <ModelAPI_Attribute.h>

/**
 * Generic validator for any attribute of a feature.
 */
class ModelAPI_ShapeValidator : public ModelAPI_AttributeValidator
{
public:
  /// returns True if the attribute is valid. It checks whether the feature of the attribute
  /// does not contain a selection attribute filled with the same shape
  /// \param theAttribute an attribute to check
  /// \param theArguments a filter parameters
  MODELAPI_EXPORT virtual bool isValid(const AttributePtr& theAttribute,
                                       const std::list<std::string>& theArguments) const;
};

#endif
