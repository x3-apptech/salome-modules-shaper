// Copyright (C) 2014-2019  CEA/DEN, EDF R&D
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

#ifndef ModelAPI_Tools_HeaderFile
#define ModelAPI_Tools_HeaderFile

#include "ModelAPI.h"

class ModelAPI_CompositeFeature;
class ModelAPI_Document;
class ModelAPI_Feature;
class ModelAPI_Result;
class ModelAPI_ResultParameter;
class ModelAPI_ResultBody;

class GeomAPI_Shape;

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace ModelAPI_Tools {
/// Returns shape from the given Result object
MODELAPI_EXPORT std::shared_ptr<GeomAPI_Shape> shape(
                                   const std::shared_ptr<ModelAPI_Result>& theResult);

/*! Returns the feature error generated according to feature error and exec state
 * \param theFeature a feature
 * \return error value or empty string
 */
MODELAPI_EXPORT std::string getFeatureError(const std::shared_ptr<ModelAPI_Feature>& theFeature);

/*!
 * Searches for variable with name \param theName in \param theDocument. 
 * If found, set it value in the \param outValue and returns true.
 * theSearcher must be located later in the history than the found variable.
 */
MODELAPI_EXPORT bool findVariable(const std::shared_ptr<ModelAPI_Document>& theDocument,
                                  std::shared_ptr<ModelAPI_Feature> theSearcher,
                                  const std::string& theName, double& outValue,
                                  std::shared_ptr<ModelAPI_ResultParameter>& theParam);

/*!
 * Searches for variable with name \param theName in the active document (Part), when
 * in the root document (PartSet). If found, set it value in the \param outValue
 * and returns true. If \param theDocument is empty active document is used.
 * theSearcher must be located later in the history than the found variable.
 */
MODELAPI_EXPORT bool findVariable(std::shared_ptr<ModelAPI_Feature> theSearcher,
  const std::string& theName,
  double& outValue, std::shared_ptr<ModelAPI_ResultParameter>& theParam,
  const std::shared_ptr<ModelAPI_Document>& theDocument = std::shared_ptr<ModelAPI_Document>());

/*!
 * Searches for Part result that contains the reference to the given document.
 * The result must be presented in the tree.
 * \param theMain document that contains the searched feature
 * \param theSub document that is searched, the resulting feature references to it
 * \returns null if not found
 */
MODELAPI_EXPORT std::shared_ptr<ModelAPI_Result> findPartResult(
                                              const std::shared_ptr<ModelAPI_Document>& theMain,
                                              const std::shared_ptr<ModelAPI_Document>& theSub);

/*!
 * Searches for Part the feature that contains in result the reference to the given document.
 * The result may be disabled.
 * \param theMain document that contains the searched feature
 * \param theSub document that is searched, the resulting feature references to it
 * \returns null if not found
 */
MODELAPI_EXPORT std::shared_ptr<ModelAPI_Feature> findPartFeature(
                                           const std::shared_ptr<ModelAPI_Document>& theMain,
                                           const std::shared_ptr<ModelAPI_Document>& theSub);

/*!
 * Returns the composite feature - parent of this feature.
 * \param theFeature the sub-element of composite
 * \returns null if it is not sub-element of composite
 */
MODELAPI_EXPORT std::shared_ptr<ModelAPI_CompositeFeature> compositeOwner(
                                        const std::shared_ptr<ModelAPI_Feature>& theFeature);

/*!
 * Returns the result - parent of this result.
 * \param theSub the sub-element of composit result
 * \param theRoot if it is true, returns the root father
 * \returns null if it is not sub-element of composite
 */
MODELAPI_EXPORT std::shared_ptr<ModelAPI_ResultBody>
  bodyOwner(const std::shared_ptr<ModelAPI_Result>& theSub, const bool theRoot = false);
/*!
 * Returns index of this result in parent (if parent exists, returned by bodyOwner)
 * \returns zero-base index, or -1 if not found
 */
MODELAPI_EXPORT int bodyIndex(const std::shared_ptr<ModelAPI_Result>& theSub);

/*!
* Returns true if the result contains a not empty list of sub results.
* It processes result compsolid.
* \param theResult a result object
* \returns boolean value
*/
MODELAPI_EXPORT bool hasSubResults(const std::shared_ptr<ModelAPI_Result>& theResult);

/*!
*  collects recursively all subs of the given result
*/
MODELAPI_EXPORT void allSubs(const std::shared_ptr<ModelAPI_ResultBody>& theResult,
                             std::list<std::shared_ptr<ModelAPI_Result> >& theResults,
                             const bool theLowerOnly = false);

/*!
* Adds the results of the given feature to theResults list: including disabled and sub-results
*/
MODELAPI_EXPORT void allResults(const std::shared_ptr<ModelAPI_Feature>& theFeature,
                                std::list<std::shared_ptr<ModelAPI_Result> >& theResults);

/*!
 Returns true if there are no parts in the document, which are not activated
 \param theNotActivatedNames out string which contains not activated names
 \return a boolean value
 */
MODELAPI_EXPORT bool allDocumentsActivated(std::string& theNotActivatedNames);

/*! Removes features from the document
* \param theFeatures a list of features to be removed
* \param theFlushRedisplay a boolean value if the redisplay signal should be flushed
* \return true if at least one feature is removed
*/
MODELAPI_EXPORT bool removeFeaturesAndReferences(
                       const std::set<std::shared_ptr<ModelAPI_Feature> >& theFeatures,
                                                 const bool theFlushRedisplay = false,
                                                 const bool theUseComposite = false,
                                                 const bool theUseRecursion = true);

/*! Removes features from the document
* \param theFeatures a list of features to be removed
* \param theFlushRedisplay a boolean value if the redisplay signal should be flushed
* \return true if at least one feature is removed
*/
MODELAPI_EXPORT bool removeFeatures(
                            const std::set<std::shared_ptr<ModelAPI_Feature> >& theFeatures,
                            const bool theFlushRedisplay);

/*! Build a map of references for the given set of features
* \param theFeatures a list of features
* \param theReferences a map of all references to the features
* \param theUseComposite state if the composite features of the reference should be in the map
* \param theUseRecursion state if references for features out of the sources feature
         list are put into the result container. E.g. if theFeatures contains "Sketch_2", map will
         contain references to this feature also.
*/
MODELAPI_EXPORT void findAllReferences(
  const std::set<std::shared_ptr<ModelAPI_Feature> >& theFeatures,
  std::map<std::shared_ptr<ModelAPI_Feature>,
  std::set<std::shared_ptr<ModelAPI_Feature> > >& theReferences,
  const bool theUseComposite = true,
  const bool theUseRecursion = true);

/*! In the map of references find all features referenced to the source feature
* \param theFeatures a list of features to find references
* \param theReferences a map of all references to the features
* \param theFeaturesRefsTo an out list of referenced features
*/
MODELAPI_EXPORT void findRefsToFeatures(
  const std::set<std::shared_ptr<ModelAPI_Feature> >& aFeatures,
  const std::map<std::shared_ptr<ModelAPI_Feature>,
                 std::set<std::shared_ptr<ModelAPI_Feature> > >& aReferences,
  std::set<std::shared_ptr<ModelAPI_Feature> >& aFeaturesRefsTo);

/*! Finds referenced of the feature to objects and collects concealed results.
* \param theFeatures a model feature
* \param theResults container for concealed results. It has no duplications
*/
MODELAPI_EXPORT void getConcealedResults(const std::shared_ptr<ModelAPI_Feature>& theFeature,
                                   std::list<std::shared_ptr<ModelAPI_Result> >& theResults);

/*! Return the default name of the result according the features it depends or name of the feature.
 *  Return also whether the name is get from the concealing result of parent object
 *  (means that concealing result has user-defined name).
 */
MODELAPI_EXPORT std::pair<std::string, bool> getDefaultName(
  const std::shared_ptr<ModelAPI_Result>& theResult, const bool theInherited = true);

/*! Collect all parents for the given feature, including the Part
 */
MODELAPI_EXPORT std::set<std::shared_ptr<ModelAPI_Feature> >
    getParents(const std::shared_ptr<ModelAPI_Feature>& theFeature);

/*! Creates a remove result features with the given results
*/
MODELAPI_EXPORT void removeResults(const std::list<std::shared_ptr<ModelAPI_Result> >& theResults);

/*! Returns current deflection in the given result
* \param theResult a result object
* \return a deflection value or -1 if it was not defined
*/
MODELAPI_EXPORT double getDeflection(const std::shared_ptr<ModelAPI_Result>& theResult);

/*! Sets the deflection value
* \param theResult a result object
* \param a deflection value
*/
MODELAPI_EXPORT void setDeflection(std::shared_ptr<ModelAPI_Result> theResult,
  const double theDeflection);

/*! Returns current color of the current result
* \param[in] theResult a result object
* \param[out] theColor a color values if it is defined
*/
MODELAPI_EXPORT void getColor(const std::shared_ptr<ModelAPI_Result>& theResult,
  std::vector<int>& theColor);

/*! Set color of the result
* \param[in] theResult a result object
* \param[in] theColor a color values
*/
MODELAPI_EXPORT void setColor(std::shared_ptr<ModelAPI_Result> theResult,
  const std::vector<int>& theColor);

/*! Returns number of iso-lines of the current result
* \param[in] theResult a result object
* \param[out] theNbLines values of iso-lines
*/
MODELAPI_EXPORT void getIsoLines(const std::shared_ptr<ModelAPI_Result>& theResult,
  std::vector<int>& theNbLines);

/*! Set number of iso-lines of the result
* \param[in] theResult a result object
* \param[in] theIso nb iso-lines
*/
MODELAPI_EXPORT void setIsoLines(std::shared_ptr<ModelAPI_Result> theResult,
  const std::vector<int>& theIso);

/*! Returns current transparency in the given result
* \param theResult a result object
* \return a transparency value or -1 if it was not defined
*/
MODELAPI_EXPORT double getTransparency(const std::shared_ptr<ModelAPI_Result>& theResult);

/*! Set transparency for the given result
* \param theResult a result object
* \param a transparency value
*/
MODELAPI_EXPORT void setTransparency(std::shared_ptr<ModelAPI_Result> theResult,
  double theTransparency);


/*! Copies all visualization attributes from one result to another.
* \param theSource a result that contains the copied attributes
* \param theDest a destination result that takes the visualization attributes
*/
MODELAPI_EXPORT void copyVisualizationAttrs(std::shared_ptr<ModelAPI_Result> theSource,
                                            std::shared_ptr<ModelAPI_Result> theDest);

}

#endif
