// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAPI_Curve.hxx
// Created:     04 Sep 2014
// Author:      Mikhail PONIKAROV

#ifndef GeomAPI_Curve_H_
#define GeomAPI_Curve_H_

#include <GeomAPI_Shape.h>
#include <memory>

/**\class GeomAPI_Curve
 * \ingroup DataModel
 * \brief Interface to the generic curve object
 */

class GEOMAPI_EXPORT GeomAPI_Curve : public GeomAPI_Interface
{
 public:
  /// Creation of empty (null) shape
  GeomAPI_Curve();

  /// Creates a curve from the shape (edge)
  GeomAPI_Curve(const std::shared_ptr<GeomAPI_Shape>& theShape);

  /// Returns true if curve is not initialized
  bool isNull() const;

  /// Returns whether the curve is linear
  virtual bool isLine() const;

  /// Returns whether the curve is circular
  virtual bool isCircle() const;

};

#endif
