// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ModelAPI_Feature.cpp
// Created:     17 Jul 2014
// Author:      Mikhail PONIKAROV

#include "ModelAPI_Feature.h"
#include <ModelAPI_Events.h>
#include <ModelAPI_Result.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_Session.h>
#include <Events_Loop.h>

const std::list<std::shared_ptr<ModelAPI_Result> >& ModelAPI_Feature::results()
{
  return myResults;
}

std::shared_ptr<ModelAPI_Result> ModelAPI_Feature::firstResult()
{
  return myResults.empty() ? std::shared_ptr<ModelAPI_Result>() : *(myResults.begin());
}

std::shared_ptr<ModelAPI_Result> ModelAPI_Feature::lastResult()
{
  return myResults.empty() ? std::shared_ptr<ModelAPI_Result>() : *(myResults.rbegin());
}

void ModelAPI_Feature::setResult(const std::shared_ptr<ModelAPI_Result>& theResult)
{
  if (firstResult() == theResult) {  // just updated
    static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_UPDATED);
    ModelAPI_EventCreator::get()->sendUpdated(theResult, anEvent);
    return;
  }
  // created
  while (!myResults.empty()) {  // remove one by one with messages
    std::shared_ptr<ModelAPI_Result> aRes = *(myResults.begin());
    myResults.erase(myResults.begin());
    ModelAPI_EventCreator::get()->sendDeleted(aRes->document(), aRes->groupName());
  }
  myResults.push_back(theResult);
  static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_CREATED);
  ModelAPI_EventCreator::get()->sendUpdated(theResult, anEvent);
  // Create event for first Feature 
  Events_Loop::loop()->flush(anEvent);
}

void ModelAPI_Feature::setResult(const std::shared_ptr<ModelAPI_Result>& theResult,
                                 const int theIndex)
{
  std::list<std::shared_ptr<ModelAPI_Result> >::iterator aResIter = myResults.begin();
  for (int anIndex = 0; anIndex < theIndex; anIndex++) {
    aResIter++;
  }
  if (aResIter == myResults.end()) {  // append
    myResults.push_back(theResult);
    static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_CREATED);
    ModelAPI_EventCreator::get()->sendUpdated(theResult, anEvent);
    // Create event for first Feature, send it to make "created" earlier than "updated"
    // VSV: Commenting out of this statement causes problems with circle operation for example
    Events_Loop::loop()->flush(anEvent);
  } else {  // update
    *aResIter = theResult;
    static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_UPDATED);
    ModelAPI_EventCreator::get()->sendUpdated(theResult, anEvent);
  }
}

void ModelAPI_Feature::removeResult(const std::shared_ptr<ModelAPI_Result>& theResult)
{
  std::list<std::shared_ptr<ModelAPI_Result> >::iterator aResIter = myResults.begin();
  for(; aResIter != myResults.end(); aResIter++) {
    ResultPtr aRes = *aResIter;
    if (aRes == theResult) {
      std::string aGroup = aRes->groupName();
      aRes->data()->erase();
      myResults.erase(aResIter);

      static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_DELETED);
      static Events_Loop* aLoop = Events_Loop::loop();
      static Events_ID EVENT_DISP = aLoop->eventByName(EVENT_OBJECT_TO_REDISPLAY);
      static const ModelAPI_EventCreator* aECreator = ModelAPI_EventCreator::get();
      ModelAPI_EventCreator::get()->sendDeleted(document(), aGroup);
      aECreator->sendUpdated(aRes, EVENT_DISP);
      break;
    }
  }
}

void ModelAPI_Feature::removeResults(const int theSinceIndex)
{
  if (theSinceIndex == 0) {
    eraseResults();
    return;
  }

  std::list<std::shared_ptr<ModelAPI_Result> >::iterator aResIter = myResults.begin();
  for(int anIndex = 0; anIndex < theSinceIndex && aResIter != myResults.end(); anIndex++)
    aResIter++;
  std::list<std::shared_ptr<ModelAPI_Result> >::iterator aNextIter = aResIter;
  for(; aNextIter != myResults.end(); aNextIter++) {
    static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_DELETED);
    static Events_Loop* aLoop = Events_Loop::loop();
    static Events_ID EVENT_DISP = aLoop->eventByName(EVENT_OBJECT_TO_REDISPLAY);
    static const ModelAPI_EventCreator* aECreator = ModelAPI_EventCreator::get();
    ModelAPI_EventCreator::get()->sendDeleted(document(), (*aNextIter)->groupName());
    aECreator->sendUpdated(*aNextIter, EVENT_DISP);
  }
  myResults.erase(aResIter, myResults.end());
}

void ModelAPI_Feature::eraseResults()
{
  if (!myResults.empty()) {
    static Events_Loop* aLoop = Events_Loop::loop();
    static Events_ID EVENT_DISP = aLoop->eventByName(EVENT_OBJECT_TO_REDISPLAY);
    static const ModelAPI_EventCreator* aECreator = ModelAPI_EventCreator::get();

    std::list<std::shared_ptr<ModelAPI_Result> >::iterator aResIter = myResults.begin();
    for(; aResIter != myResults.end(); aResIter++) {
      (*aResIter)->data()->erase();
      ModelAPI_EventCreator::get()->sendDeleted(document(), (*aResIter)->groupName());
      aECreator->sendUpdated(*aResIter, EVENT_DISP);
    }
    myResults.clear();
    // flush it to avoid left presentations after input of invalid arguments (radius=0)
    static Events_ID anEvent = Events_Loop::eventByName(EVENT_OBJECT_DELETED);
    Events_Loop::loop()->flush(anEvent);
    Events_Loop::loop()->flush(EVENT_DISP);
  }
}

const std::string& ModelAPI_Feature::documentToAdd()
{
  // empty to use the current document
  static const std::string anEmpty;
  return anEmpty;
}

void ModelAPI_Feature::erase()
{
  static Events_Loop* aLoop = Events_Loop::loop();
  static Events_ID EVENT_DISP = aLoop->eventByName(EVENT_OBJECT_TO_REDISPLAY);
  static const ModelAPI_EventCreator* aECreator = ModelAPI_EventCreator::get();

  while (!myResults.empty()) {  // remove one by one with messages
    std::shared_ptr<ModelAPI_Result> aRes = *(myResults.begin());
    myResults.erase(myResults.begin());
    aECreator->sendDeleted(aRes->document(), aRes->groupName());
    aECreator->sendUpdated(aRes, EVENT_DISP);
  }
  ModelAPI_Object::erase();
}

ModelAPI_Feature::~ModelAPI_Feature()
{
  erase();
}

FeaturePtr ModelAPI_Feature::feature(ObjectPtr theObject)
{
  FeaturePtr aFeature = std::dynamic_pointer_cast<ModelAPI_Feature>(theObject);
  if (!aFeature) {
    ResultPtr aResult = std::dynamic_pointer_cast<ModelAPI_Result>(theObject);
    if (aResult) {
      DocumentPtr aDoc = aResult->document();
      return aDoc->feature(aResult);
    }
  }
  return aFeature;
}


bool ModelAPI_Feature::isMacro() const
{
  return false;
}
