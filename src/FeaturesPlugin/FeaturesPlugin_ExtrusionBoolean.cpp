// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        FeaturesPlugin_ExtrusionBoolean.h
// Created:     11 June 2015
// Author:      Dmitry Bobylev

#include <FeaturesPlugin_ExtrusionBoolean.h>

#include <ModelAPI_AttributeDouble.h>
#include <ModelAPI_AttributeSelection.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Validator.h>

#include <GeomAlgoAPI_Prism.h>

//=================================================================================================
void FeaturesPlugin_ExtrusionBoolean::initMakeSolidsAttributes()
{
  data()->addAttribute(FROM_OBJECT_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(FROM_SIZE_ID(), ModelAPI_AttributeDouble::typeId());

  data()->addAttribute(TO_OBJECT_ID(), ModelAPI_AttributeSelection::typeId());
  data()->addAttribute(TO_SIZE_ID(), ModelAPI_AttributeDouble::typeId());

  ModelAPI_Session::get()->validators()->registerNotObligatory(getKind(), FROM_OBJECT_ID());
  ModelAPI_Session::get()->validators()->registerNotObligatory(getKind(), TO_OBJECT_ID());
}

//=================================================================================================
ListOfShape FeaturesPlugin_ExtrusionBoolean::MakeSolids(const ListOfShape& theFaces)
{
  // Getting extrusion bounding planes.
  std::shared_ptr<GeomAPI_Shape> aFromShape;
  std::shared_ptr<GeomAPI_Shape> aToShape;
  std::shared_ptr<ModelAPI_AttributeSelection> anObjRef = selection(FROM_OBJECT_ID());
  if (anObjRef) {
    aFromShape = std::dynamic_pointer_cast<GeomAPI_Shape>(anObjRef->value());
  }
  anObjRef = selection(TO_OBJECT_ID());
  if (anObjRef) {
    aToShape = std::dynamic_pointer_cast<GeomAPI_Shape>(anObjRef->value());
  }

  // Getting extrusion sizes.
  double aFromSize = real(FROM_SIZE_ID())->value();
  double aToSize = real(TO_SIZE_ID())->value();

  // Extrude faces.
  ListOfShape anExtrusionList;
  for(ListOfShape::const_iterator aFacesIt = theFaces.begin(); aFacesIt != theFaces.end(); aFacesIt++) {
    std::shared_ptr<GeomAPI_Shape> aBaseShape = *aFacesIt;
    GeomAlgoAPI_Prism aPrismAlgo(aBaseShape, aFromShape, aFromSize, aToShape, aToSize);

    // Checking that the algorithm worked properly.
    if(!aPrismAlgo.isDone() || aPrismAlgo.shape()->isNull() || !aPrismAlgo.isValid()) {
      setError("Extrusion algorithm failed");
      return ListOfShape();
    }
    anExtrusionList.push_back(aPrismAlgo.shape());
  }

  return anExtrusionList;
}