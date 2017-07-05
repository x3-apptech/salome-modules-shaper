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

#ifndef Model_ResultCompSolid_H_
#define Model_ResultCompSolid_H_

#include "Model.h"
#include <ModelAPI_ResultCompSolid.h>
#include <vector>

/**\class Model_ResultCompSolid
* \ingroup DataModel
* \brief The compsolid (container of body results) result of a feature.
*
* Provides a container of shapes that may be displayed in the viewer.
*/
class Model_ResultCompSolid : public ModelAPI_ResultCompSolid
{
  /// Sub-bodies if this is compsolid: zero base index to subs
  std::vector<std::shared_ptr<ModelAPI_ResultBody> > mySubs;
  /// Flag that stores the previous state of "concealed": if it is changed,
  /// The event must be generated to redisplay this and all subs.
  bool myLastConcealed;


public:

  /// Removes the stored builders
  MODEL_EXPORT virtual ~Model_ResultCompSolid();

  /// Stores the shape (called by the execution method). Creates sub-results for compsolid.
  MODEL_EXPORT virtual void store(const std::shared_ptr<GeomAPI_Shape>& theShape,
                                  const bool theIsStoreSameShapes = true);

  /// Stores the generated shape.  Creates sub-results for compsolid.
  MODEL_EXPORT virtual void storeGenerated(const std::shared_ptr<GeomAPI_Shape>& theFromShape,
    const std::shared_ptr<GeomAPI_Shape>& theToShape);

  /// Stores the modified shape.  Creates sub-results for compsolid.
  MODEL_EXPORT virtual void storeModified(const std::shared_ptr<GeomAPI_Shape>& theOldShape,
    const std::shared_ptr<GeomAPI_Shape>& theNewShape, const int theDecomposeSolidsTag = 0);

  /// load and orient modified shapes for sub-objects
  MODEL_EXPORT virtual void loadAndOrientModifiedShapes (GeomAlgoAPI_MakeShape* theMS,
    std::shared_ptr<GeomAPI_Shape>  theShapeIn, const int  theKindOfShape, const int  theTag,
    const std::string& theName, GeomAPI_DataMapOfShapeShape& theSubShapes,
    const bool theIsStoreSeparate = false,
    const bool theIsStoreAsGenerated = false);


  /// Returns the number of sub-elements
  MODEL_EXPORT virtual int numberOfSubs(bool forTree = false) const;

  /// Returns the sub-result by zero-base index
  MODEL_EXPORT virtual std::shared_ptr<ModelAPI_ResultBody> subResult(const int theIndex,
    bool forTree = false) const;

  /// Returns true if feature or reuslt belong to this composite feature as subs
  MODEL_EXPORT virtual bool isSub(ObjectPtr theObject) const;

  /// Returns the parameters of color definition in the resources config manager
  MODEL_EXPORT virtual void colorConfigInfo(std::string& theSection, std::string& theName,
                                            std::string& theDefault);

  /// Disables the result body: keeps the resulting shape as selection, but erases the underlaying
  /// naming data structure if theFlag if false. Or restores everything on theFlag is true.
  MODEL_EXPORT virtual bool setDisabled(std::shared_ptr<ModelAPI_Result> theThis,
    const bool theFlag);

  /// The compsolid is concealed if at least one of the sub is concealed
  MODEL_EXPORT virtual bool isConcealed();

  /// Sets all subs as concealed in the data tree (referenced by other objects)
  MODEL_EXPORT virtual void setIsConcealed(const bool theValue);

  /// Returns true if the latest modification of this body in the naming history
  // is equal to the given shape
  MODEL_EXPORT virtual bool isLatestEqual(const std::shared_ptr<GeomAPI_Shape>& theShape);

protected:
  /// Makes a body on the given feature
  Model_ResultCompSolid();

  /// Updates the sub-bodies if shape of this object is composite-solid
  void updateSubs(const std::shared_ptr<GeomAPI_Shape>& theThisShape);

  friend class Model_Objects;
};

#endif
