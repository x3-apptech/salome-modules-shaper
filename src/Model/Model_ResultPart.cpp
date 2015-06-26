// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ModelAPI_ResultPart.cpp
// Created:     07 Jul 2014
// Author:      Mikhail PONIKAROV

#include <Model_ResultPart.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_AttributeDocRef.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Feature.h>
#include <ModelAPI_ResultBody.h>

#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>

std::shared_ptr<ModelAPI_Document> Model_ResultPart::partDoc()
{
  return data()->document("PartDocument")->value();
}

std::shared_ptr<ModelAPI_Feature> Model_ResultPart::owner()
{
  return std::shared_ptr<ModelAPI_Feature>();  // return empty pointer
}

Model_ResultPart::Model_ResultPart()
{
  myIsDisabled = true; // by default it is not initialized and false to be after created
  setIsConcealed(false);
}

void Model_ResultPart::setData(std::shared_ptr<ModelAPI_Data> theData)
{
  ModelAPI_Result::setData(theData);
  if (theData) {
    data()->addAttribute(DOC_REF(), ModelAPI_AttributeDocRef::typeId());
  }
}

void Model_ResultPart::activate()
{
  std::shared_ptr<ModelAPI_AttributeDocRef> aDocRef = data()->document(DOC_REF());
  
  if (!aDocRef->value().get()) {  // create (or open) a document if it is not yet created
    std::shared_ptr<ModelAPI_Document> aDoc = document()->subDocument(data()->name());
    if (aDoc) {
      aDocRef->setValue(aDoc);
    }
  }
  if (aDocRef->value().get()) {
    SessionPtr aMgr = ModelAPI_Session::get();
    bool isNewTransaction = !aMgr->isOperation();
    // activation may cause changes in current features in document, so it must be in transaction
    if (isNewTransaction) {
      aMgr->startOperation("Activation");
    }
    ModelAPI_Session::get()->setActiveDocument(aDocRef->value());
    if (isNewTransaction) {
      aMgr->finishOperation();
    }
  }
}

bool Model_ResultPart::isActivated() 
{
  std::shared_ptr<ModelAPI_AttributeDocRef> aDocRef = data()->document(DOC_REF());
  return aDocRef->value().get();
}

bool Model_ResultPart::setDisabled(std::shared_ptr<ModelAPI_Result> theThis,
    const bool theFlag)
{
  if (ModelAPI_ResultPart::setDisabled(theThis, theFlag)) {
    DocumentPtr aDoc = Model_ResultPart::partDoc();
    if (aDoc.get()) {
      if (theFlag) { // disable, so make all features disabled too
        aDoc->setCurrentFeature(FeaturePtr(), false);
      } else { // enabled, so make the current feature the last inside of this document
        FeaturePtr aLastFeature = std::dynamic_pointer_cast<ModelAPI_Feature>(aDoc->object(
          ModelAPI_Feature::group(), aDoc->size(ModelAPI_Feature::group()) - 1));
        aDoc->setCurrentFeature(aLastFeature, false);
      }
    }
    return true;
  }
  return false;
}

std::shared_ptr<GeomAPI_Shape> Model_ResultPart::shape()
{
  DocumentPtr aDoc = Model_ResultPart::partDoc();
  if (aDoc.get()) {
    const std::string& aBodyGroup = ModelAPI_ResultBody::group();
    TopoDS_Compound aResultComp;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aResultComp);
    int aNumSubs = 0;
    for(int a = aDoc->size(aBodyGroup) - 1; a >= 0; a--) {
      ResultPtr aBody = std::dynamic_pointer_cast<ModelAPI_Result>(aDoc->object(aBodyGroup, a));
      if (aBody.get() && aBody->shape().get()) {
        TopoDS_Shape aShape = *(aBody->shape()->implPtr<TopoDS_Shape>());
        if (!aShape.IsNull()) {
          aBuilder.Add(aResultComp, aShape);
          aNumSubs++;
        }
      }
    }
    if (aNumSubs) {
      std::shared_ptr<GeomAPI_Shape> aResult(new GeomAPI_Shape);
      aResult->setImpl(new TopoDS_Shape(aResultComp));
      return aResult;
    }
  }
  return std::shared_ptr<GeomAPI_Shape>();
}


void Model_ResultPart::colorConfigInfo(std::string& theSection, std::string& theName,
  std::string& theDefault)
{
  theSection = "Visualization";
  theName = "result_part_color";
  theDefault = DEFAULT_COLOR();
}
