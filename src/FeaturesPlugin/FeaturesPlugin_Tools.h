// Copyright (C) 2014-2021  CEA/DEN, EDF R&D
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef FeaturesPlugin_Tools_H_
#define FeaturesPlugin_Tools_H_

#include <GeomAlgoAPI_MakeShape.h>
#include <GeomAPI_ShapeHierarchy.h>
#include <ModelAPI_ResultBody.h>

#include <vector>

class FeaturesPlugin_Tools {
public:
  struct ResultBaseAlgo {
    ResultBodyPtr resultBody;
    GeomShapePtr baseShape;
    GeomMakeShapePtr makeShape;
  };

public:
  static void loadModifiedShapes(ResultBodyPtr theResultBody,
                                 const ListOfShape& theBaseShapes,
                                 const ListOfShape& theTools,
                                 const GeomMakeShapePtr& theMakeShape,
                                 const GeomShapePtr theResultShape,
                                 const std::string& theNamePrefix = "");

  static void loadModifiedShapes(ResultBodyPtr theResultBody,
                                 const GeomShapePtr& theBaseShape,
                                 const GeomMakeShapePtr& theMakeShape,
                                 const std::string theName);
  /// Stores deleted shapes.
  static void loadDeletedShapes(ResultBodyPtr theResultBody,
                                const GeomShapePtr theBaseShape,
                                const ListOfShape& theTools,
                                const GeomMakeShapePtr& theMakeShape,
                                const GeomShapePtr theResultShapesCompound);

  /// Stores deleted shapes.
  static void loadDeletedShapes(std::vector<ResultBaseAlgo>& theResultBaseAlgoList,
                                const ListOfShape& theTools,
                                const GeomShapePtr theResultShapesCompound);

  /// Obtain shapes from the selection list attribute.
  static bool getShape(const std::shared_ptr<ModelAPI_AttributeSelectionList> theSelectionList,
                       const bool theShareTopology,
                       ListOfShape& theShapesList,
                       std::string& theError);

  /// Collect shapes from the attribute and fill the hierarchy or a list of parts
  static bool shapesFromSelectionList(
      const std::shared_ptr<ModelAPI_AttributeSelectionList> theSelectionList,
      const bool theStoreFullHierarchy,
      GeomAPI_ShapeHierarchy& theHierarchy,
      std::list<ResultPtr>& theParts,
      ResultPtr& theTextureSource);
};

#endif /* FeaturesPlugin_Tools_H_ */
