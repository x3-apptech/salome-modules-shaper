// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        PartSetPlugin_Part.cxx
// Created:     27 Mar 2014
// Author:      Mikhail PONIKAROV

#include "PartSetPlugin_Part.h"
#include "ModelAPI_Session.h"
#include "ModelAPI_Document.h"
#include "ModelAPI_Data.h"
#include "ModelAPI_AttributeDocRef.h"
#include <ModelAPI_ResultPart.h>

using namespace std;

PartSetPlugin_Part::PartSetPlugin_Part()
{
}

void PartSetPlugin_Part::initAttributes()
{  // all is in part result
}

void PartSetPlugin_Part::execute()
{
  ResultPartPtr aResult = std::dynamic_pointer_cast<ModelAPI_ResultPart>(firstResult());
  if (!aResult) {
    aResult = document()->createPart(data());
    setResult(aResult);
    // do not activate part by simple execution if it is not loaded yet: it must be explicitly
    // activated for this
    if (!ModelAPI_Session::get()->isLoadByDemand(aResult->data()->name())) {
      aResult->activate();
    }
  }
}

const std::string& PartSetPlugin_Part::documentToAdd()
{
  // part must be added only to the module document
  return ModelAPI_Session::get()->moduleDocument()->kind();
}
