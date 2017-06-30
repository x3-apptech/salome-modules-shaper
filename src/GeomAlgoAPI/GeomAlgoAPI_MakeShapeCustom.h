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

#ifndef GeomAlgoAPI_MakeShapeCustom_H_
#define GeomAlgoAPI_MakeShapeCustom_H_

#include <GeomAlgoAPI_MakeShape.h>

#include <GeomAPI_DataMapOfShapeMapOfShapes.h>
#include <GeomAPI_DataMapOfShapeShape.h>

/** \class GeomAlgoAPI_MakeShapeCustom
 *  \ingroup DataAlgo
 *  \brief Interface to the root class of all topological shapes constructions
 */
class GeomAlgoAPI_MakeShapeCustom : public GeomAlgoAPI_MakeShape
{
public:
  /// Default constructor
  GEOMALGOAPI_EXPORT GeomAlgoAPI_MakeShapeCustom();

  /// Sets result shape.
  GEOMALGOAPI_EXPORT void setResult(const std::shared_ptr<GeomAPI_Shape> theShape);

  /// Adds modified shape.
  GEOMALGOAPI_EXPORT bool addModified(const std::shared_ptr<GeomAPI_Shape> theBase,
                                      const std::shared_ptr<GeomAPI_Shape> theResult);

  /// Adds generated shape.
  GEOMALGOAPI_EXPORT bool addGenerated(const std::shared_ptr<GeomAPI_Shape> theBase,
                                       const std::shared_ptr<GeomAPI_Shape> theResult);

  /// Adds deleted shape.
  GEOMALGOAPI_EXPORT bool addDeleted(const std::shared_ptr<GeomAPI_Shape> theShape);

  /// \return the list of shapes generated from the shape theShape
  GEOMALGOAPI_EXPORT void generated(const std::shared_ptr<GeomAPI_Shape> theShape,
                                            ListOfShape& theHistory);

  /// \return the list of shapes modified from the shape theShape
  GEOMALGOAPI_EXPORT void modified(const std::shared_ptr<GeomAPI_Shape> theShape,
                                           ListOfShape& theHistory);

  /// \return whether the shape is deleted
  GEOMALGOAPI_EXPORT bool isDeleted(const std::shared_ptr<GeomAPI_Shape> theShape);

private:
  GeomAPI_DataMapOfShapeMapOfShapes myGenerated;
  GeomAPI_DataMapOfShapeMapOfShapes myModified;
  GeomAPI_DataMapOfShapeShape       myDeleted;
};

#endif