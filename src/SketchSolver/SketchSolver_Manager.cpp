// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#include "SketchSolver_Manager.h"
#include "SketchSolver_Error.h"

#include <Events_Loop.h>
#include <ModelAPI_Events.h>
#include <ModelAPI_ResultConstruction.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Validator.h>
#include <SketchPlugin_Sketch.h>

/// Global constraint manager object
static SketchSolver_Manager* myManager = SketchSolver_Manager::instance();

/// \brief Verifies is the feature valid
static bool isFeatureValid(FeaturePtr theFeature)
{
  if (!theFeature || !theFeature->data() || !theFeature->data()->isValid())
    return false;

  SessionPtr aMgr = ModelAPI_Session::get();
  ModelAPI_ValidatorsFactory* aFactory = aMgr->validators();
  return aFactory->validate(theFeature);
}



// ========================================================
// ========= SketchSolver_Manager ===============
// ========================================================
SketchSolver_Manager* SketchSolver_Manager::instance()
{
  static SketchSolver_Manager* mySelf = 0; // Self pointer to implement singleton functionality
  if (!mySelf)
    mySelf = new SketchSolver_Manager();
  return mySelf;
}

SketchSolver_Manager::SketchSolver_Manager()
{
  myGroups.clear();
  myIsComputed = false;

  // Register in event loop
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_CREATED));
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_UPDATED));
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_DELETED));
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_MOVED));

  ////Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_SOLVER_FAILED));
  ////Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_SOLVER_REPAIRED));
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_SKETCH_PREPARED));
}

SketchSolver_Manager::~SketchSolver_Manager()
{
  myGroups.clear();
}

bool SketchSolver_Manager::groupMessages()
{
  return true;
}

// ============================================================================
//  Function: processEvent
//  Purpose:  listen the event loop and process the message
// ============================================================================
void SketchSolver_Manager::processEvent(
  const std::shared_ptr<Events_Message>& theMessage)
{
  bool needToResolve = false;
  bool isUpdateFlushed = false;
  bool isMovedEvt = false;

  static const Events_ID aCreatedEvent = Events_Loop::eventByName(EVENT_OBJECT_CREATED);
  static const Events_ID anUpdateEvent = Events_Loop::eventByName(EVENT_OBJECT_UPDATED);
  static const Events_ID aSketchPreparedEvent = Events_Loop::eventByName(EVENT_SKETCH_PREPARED);
  // sketch is prepared for resolve: all the needed events
  // are collected and must be processed by the solver
  if (theMessage->eventID() == aSketchPreparedEvent) {
    flushGrouped(anUpdateEvent);
    needToResolve = true;
  }

  if (myIsComputed)
    return;
  myIsComputed = true;

  if (theMessage->eventID() == aCreatedEvent
      || theMessage->eventID() == anUpdateEvent
      || theMessage->eventID() == Events_Loop::loop()->eventByName(EVENT_OBJECT_MOVED)) {
    std::shared_ptr<ModelAPI_ObjectUpdatedMessage> anUpdateMsg =
        std::dynamic_pointer_cast<ModelAPI_ObjectUpdatedMessage>(theMessage);

    isUpdateFlushed = stopSendUpdate();

    isMovedEvt = theMessage->eventID()
          == Events_Loop::loop()->eventByName(EVENT_OBJECT_MOVED);

    // Shows that the message has at least one feature applicable for solver
    bool hasProperFeature = false;

    // update sketch features only
    const std::set<ObjectPtr>& aFeatures = anUpdateMsg->objects();
    // try to keep order as features were created if there are several created features: #2229
    if (theMessage->eventID() == aCreatedEvent && aFeatures.size() > 1) {
      std::map<int, std::shared_ptr<SketchPlugin_Feature>> anOrderedFeatures;
      std::set<ObjectPtr>::iterator aFeatIter;
      for (aFeatIter = aFeatures.begin(); aFeatIter != aFeatures.end(); aFeatIter++) {
        std::shared_ptr<SketchPlugin_Feature> aFeature =
            std::dynamic_pointer_cast<SketchPlugin_Feature>(*aFeatIter);
        if (aFeature && !aFeature->isMacro() && aFeature->data() && aFeature->data()->isValid()) {
          anOrderedFeatures[aFeature->data()->featureId()] = aFeature;
        }
      }
      std::map<int, std::shared_ptr<SketchPlugin_Feature>>::iterator aFeat;
      for(aFeat = anOrderedFeatures.begin(); aFeat != anOrderedFeatures.end(); aFeat++) {
        hasProperFeature = updateFeature(aFeat->second, isMovedEvt) || hasProperFeature;
      }
    } else { // order is not important
      std::set<ObjectPtr>::iterator aFeatIter;
      for (aFeatIter = aFeatures.begin(); aFeatIter != aFeatures.end(); aFeatIter++) {
        std::shared_ptr<SketchPlugin_Feature> aFeature =
            std::dynamic_pointer_cast<SketchPlugin_Feature>(*aFeatIter);
        if (!aFeature || aFeature->isMacro())
          continue;
        hasProperFeature = updateFeature(aFeature, isMovedEvt) || hasProperFeature;
      }
    }

    if (isMovedEvt && hasProperFeature)
      needToResolve = true;

  } else if (theMessage->eventID() == Events_Loop::loop()->eventByName(EVENT_OBJECT_DELETED)) {
    std::shared_ptr<ModelAPI_ObjectDeletedMessage> aDeleteMsg =
      std::dynamic_pointer_cast<ModelAPI_ObjectDeletedMessage>(theMessage);
    const std::set<std::string>& aFeatureGroups = aDeleteMsg->groups();

    // Find SketchPlugin_Sketch::ID() in groups.
    // The constraint groups should be updated when an object removed from Sketch
    std::set<std::string>::const_iterator aFGrIter;
    for (aFGrIter = aFeatureGroups.begin(); aFGrIter != aFeatureGroups.end(); aFGrIter++)
      if (aFGrIter->compare(ModelAPI_ResultConstruction::group()) == 0 ||
          aFGrIter->compare(ModelAPI_Feature::group()) == 0)
        break;

    if (aFGrIter != aFeatureGroups.end()) {
      std::list<SketchGroupPtr>::iterator aGroupIter = myGroups.begin();
      while (aGroupIter != myGroups.end()) {
        if (!(*aGroupIter)->isWorkplaneValid()) {  // the group should be removed
          std::list<SketchGroupPtr>::iterator aRemoveIt = aGroupIter++;
          myGroups.erase(aRemoveIt);
          continue;
        }

        (*aGroupIter)->repairConsistency();
        ++aGroupIter;
      }
    }
    myIsComputed = false;
  }

  // resolve constraints if needed
  bool needToUpdate = needToResolve && resolveConstraints();
  releaseFeaturesIfEventsBlocked();

  // Features may be updated => now send events, but for all changed at once
  if (isUpdateFlushed)
    allowSendUpdate();

  myIsComputed = false;

  // send update for movement in any case
  if (needToUpdate || isMovedEvt)
    Events_Loop::loop()->flush(anUpdateEvent);
}

// ============================================================================
//  Function: changeConstraintOrEntity
//  Purpose:  create/update the constraint or the feature and place it into appropriate group
// ============================================================================
bool SketchSolver_Manager::updateFeature(std::shared_ptr<SketchPlugin_Feature> theFeature,
                                         bool theMoved)
{
  // Check feature validity and find a group to place it.
  // If the feature is not valid, the returned group will be empty.
  // This will protect to deal with wrong (not fully initialized) features.
  SketchGroupPtr aGroup = findGroup(theFeature);
  if (!aGroup)
    return false;
  aGroup->blockEvents(true);

  std::shared_ptr<SketchPlugin_Constraint> aConstraint =
      std::dynamic_pointer_cast<SketchPlugin_Constraint>(theFeature);

  bool isOk = false;
  if (aConstraint)
    isOk = aGroup->changeConstraint(aConstraint);
  else if (theMoved)
    isOk = aGroup->moveFeature(theFeature);
  else
    isOk = aGroup->updateFeature(theFeature);
  return isOk;
}

// ============================================================================
//  Function: findGroup
//  Purpose:  search groups of entities interacting with given feature
// ============================================================================
SketchGroupPtr SketchSolver_Manager::findGroup(
    std::shared_ptr<SketchPlugin_Feature> theFeature)
{
  if (!isFeatureValid(theFeature))
    return SketchGroupPtr(); // do not process wrong features

  // Obtain sketch, containing the feature
  CompositeFeaturePtr aSketch;
  const std::set<AttributePtr>& aRefsList = theFeature->data()->refsToMe();
  std::set<AttributePtr>::const_iterator aRefIt = aRefsList.begin();
  for (; aRefIt != aRefsList.end(); ++aRefIt) {
    FeaturePtr anOwner = ModelAPI_Feature::feature((*aRefIt)->owner());
    if (anOwner && anOwner->getKind() == SketchPlugin_Sketch::ID()) {
      aSketch = std::dynamic_pointer_cast<ModelAPI_CompositeFeature>(anOwner);
      break;
    }
  }

  if (!aSketch)
    return SketchGroupPtr(); // not a sketch's feature

  std::list<SketchGroupPtr>::const_iterator aGroupIt;
  for (aGroupIt = myGroups.begin(); aGroupIt != myGroups.end(); ++aGroupIt)
    if ((*aGroupIt)->getWorkplane() == aSketch)
      return *aGroupIt;

  // group for the sketch does not created yet
  SketchGroupPtr aNewGroup = SketchGroupPtr(new SketchSolver_Group(aSketch));
  myGroups.push_back(aNewGroup);
  return aNewGroup;
}

// ============================================================================
//  Function: resolveConstraints
//  Purpose:  change entities according to available constraints
// ============================================================================
bool SketchSolver_Manager::resolveConstraints()
{
  bool needToUpdate = false;
  std::list<SketchGroupPtr>::const_iterator aGroupIter = myGroups.begin();
  for (; aGroupIter != myGroups.end(); ++aGroupIter) {
    if ((*aGroupIter)->resolveConstraints())
      needToUpdate = true;
  }
  return needToUpdate;
}

void SketchSolver_Manager::releaseFeaturesIfEventsBlocked() const
{
  std::list<SketchGroupPtr>::const_iterator aGroupIter = myGroups.begin();
  for (; aGroupIter != myGroups.end(); ++aGroupIter)
    (*aGroupIter)->blockEvents(false);
}

bool SketchSolver_Manager::stopSendUpdate() const
{
static const Events_ID anUpdateEvent = Events_Loop::eventByName(EVENT_OBJECT_UPDATED);
  // to avoid redisplay of each segment on update by solver one by one in the viewer
  bool isUpdateFlushed = Events_Loop::loop()->isFlushed(anUpdateEvent);
  if (isUpdateFlushed) {
    Events_Loop::loop()->setFlushed(anUpdateEvent, false);
  }
  return isUpdateFlushed;
}

void SketchSolver_Manager::allowSendUpdate() const
{
  Events_Loop::loop()->setFlushed(Events_Loop::eventByName(EVENT_OBJECT_UPDATED), true);
}
