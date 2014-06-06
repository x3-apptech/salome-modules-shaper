// File:        PartSet_FeaturePrs.h
// Created:     04 Jun 2014
// Author:      Natalia ERMOLAEVA

#ifndef PartSet_FeaturePrs_H
#define PartSet_FeaturePrs_H

#include "PartSet.h"

#include "PartSet_Constants.h"

class GeomDataAPI_Point2D;

/*!
 \class PartSet_FeaturePrs
 * \brief The abstract class to define the specific feature manipulation. It is created for
 * the feature create operation to move out the feature properties set and use one operation
 * for any type of features.
*/
class PARTSET_EXPORT PartSet_FeaturePrs
{
public:
  /// Constructor
  /// \param theSketch the sketch feature
  PartSet_FeaturePrs(FeaturePtr theSketch);
  /// Destructor
  virtual ~PartSet_FeaturePrs();

  /// Initializes some fields of feature accorging to the source feature
  /// Saves the fiature as the presentation internal feature
  /// \param theFeature the presentation feature
  /// \param theSourceFeature the feature, which attributes are used to initialize the feature
  void init(FeaturePtr theFeature, FeaturePtr theSourceFeature);

  /// Returns the operation sketch feature
  /// \returns the sketch instance
  FeaturePtr sketch() const;

  /// Sets the point to the feature in an attribute depending on the selection mode
  /// \param theX the 2D point horizontal coordinate
  /// \param theY the 2D point vertical coordinate
  /// \param theMode the selection mode
  /// \return the new selection mode
  virtual PartSet_SelectionMode setPoint(double theX, double theY,
                                         const PartSet_SelectionMode& theMode) = 0;

  /// Returns the feature attribute name for the selection mode
  /// \param theMode the current operation selection mode. The feature attribute depends on the mode
  virtual std::string getAttribute(const PartSet_SelectionMode& theMode) const = 0;

  /// Returns the next selection mode after the attribute
  /// \param theAttribute the feature attribute name
  /// \return next attribute selection mode
  virtual PartSet_SelectionMode getNextMode(const std::string& theAttribute) const = 0;

  /// Creates constrains of the current 
  /// \param theX the horizontal coordnate of the point
  /// \param theY the vertical coordnate of the point
  /// \param theMode the current operation selection mode. The feature attribute depends on the mode
  void setConstraints(double theX, double theY, const PartSet_SelectionMode& theMode);

protected:
  /// Returns the operation feature
  /// \return the feature
  FeaturePtr feature() const;

  /// Initializes current feature by the given
  /// \param theSourceFeature the feature, which attributes are used to initialize the current feature
  virtual void initFeature(FeaturePtr theSourceFeature) {};

  /// Returns the feature point in the selection mode position.
  /// \param theMode the current operation selection mode. The feature attribute depends on the mode
  virtual boost::shared_ptr<GeomDataAPI_Point2D> featurePoint(const PartSet_SelectionMode& theMode) = 0;

private:
  FeaturePtr mySketch; ///< the sketch of the feature
  FeaturePtr myFeature; ///< the feature
};

#endif
