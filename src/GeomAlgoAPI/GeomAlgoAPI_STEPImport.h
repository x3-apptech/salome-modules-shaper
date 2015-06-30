// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:    GEOMALGOAPI_STEPImport.h
// Created: Dec 24, 2014
// Author:  Sergey BELASH


#ifndef GEOMALGOAPI_STEPIMPORT_H_
#define GEOMALGOAPI_STEPIMPORT_H_

#include <GeomAlgoAPI.h>

#include <string>

#include <GeomAPI_Shape.h>

/// Implementation of the import STEP files algorithms
GEOMALGOAPI_EXPORT
std::shared_ptr<GeomAPI_Shape> STEPImport(const std::string& theFileName,
                                          const std::string& theFormatName,
                                          std::string& theError);

#endif /* GEOMALGOAPI_STEPIMPORT_H_ */
