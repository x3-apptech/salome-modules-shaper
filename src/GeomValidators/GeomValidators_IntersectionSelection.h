// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#ifndef GeomValidators_IntersectionSelection_H
#define GeomValidators_IntersectionSelection_H

#include <GeomValidators.h>

#include <ModelAPI_AttributeValidator.h>
#include <ModelAPI_Attribute.h>

/// \class GeomValidators_IntersectionSelection
/// \ingroup Validators
/// \brief Validates selection for intersection operation.
class GeomValidators_IntersectionSelection: public ModelAPI_AttributeValidator
{
public:
  /// \return True if the attribute is valid. It checks whether the selection
  /// is acceptable for intersection operation.
  /// \param[in] theAttribute an attribute to check.
  /// \param[in] theArguments a filter parameters.
  /// \param[out] theError error message.
  GEOMVALIDATORS_EXPORT virtual bool isValid(const AttributePtr& theAttribute,
                                             const std::list<std::string>& theArguments,
                                             Events_InfoMessage& theError) const;
};

#endif
