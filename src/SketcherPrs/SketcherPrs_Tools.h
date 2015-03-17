// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        SketcherPrs_Tools.h
// Created:     10 March 2015
// Author:      Vitaly SMETANNIKOV

#ifndef SketcherPrs_Tools_H
#define SketcherPrs_Tools_H

#include "SketcherPrs.h"
#include <GeomAPI_Shape.h>
#include <GeomAPI_Pnt2d.h>
#include <ModelAPI_Object.h>
#include <string>

class SketchPlugin_Constraint;

namespace SketcherPrs_Tools {

  ObjectPtr getResult(SketchPlugin_Constraint* theFeature,
                      const std::string& theAttrName);

  std::shared_ptr<GeomAPI_Shape> getShape(ObjectPtr theObject);

  std::shared_ptr<GeomAPI_Pnt2d> getPoint(SketchPlugin_Constraint* theFeature,
                                          const std::string& theAttrName);

};

#endif