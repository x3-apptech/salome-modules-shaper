// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        GeomAPI_IPresentable.hxx
// Created:     17 July 2014
// Author:      Vitaly SMETANNIKOV

#ifndef GeomAPI_IPresentable_H_
#define GeomAPI_IPresentable_H_

#include "GeomAPI_AISObject.h"

/**
 * A class which defines an interface of object which is able to create its own presentation
 */
class GeomAPI_IPresentable
{
 public:
  virtual ~GeomAPI_IPresentable();
  /** Returns the AIS preview
   *   \param thePrevious - defines a presentation if it was created previously
   */
  virtual AISObjectPtr getAISObject(AISObjectPtr thePrevious) = 0;
};

typedef std::shared_ptr<GeomAPI_IPresentable> GeomPresentablePtr;

#endif
