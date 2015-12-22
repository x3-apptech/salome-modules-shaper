// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:    SolveSpaceSolver_Storage.cpp
// Created: 18 Mar 2015
// Author:  Artem ZHIDKOV

#include <SolveSpaceSolver_Storage.h>
#include <SolveSpaceSolver_ConstraintWrapper.h>
#include <SolveSpaceSolver_EntityWrapper.h>
#include <SolveSpaceSolver_ParameterWrapper.h>
#include <SolveSpaceSolver_Builder.h>

#include <GeomAPI_Dir2d.h>
#include <GeomAPI_Pnt2d.h>
#include <GeomAPI_XY.h>
#include <math.h>
#include <assert.h>

#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Point2D.h>
#include <ModelAPI_AttributeDouble.h>
#include <ModelAPI_AttributeRefAttr.h>
#include <SketchPlugin_Arc.h>
#include <SketchPlugin_ConstraintCoincidence.h>

/** \brief Search the entity/parameter with specified ID in the list of elements
 *  \param[in] theEntityID unique ID of the element
 *  \param[in] theEntities list of elements
 *  \return position of the found element or -1 if the element is not found
 */
template<typename T>
static int Search(const uint32_t& theEntityID, const std::vector<T>& theEntities);

/// \brief Compare two parameters to be different
static bool IsNotEqual(const Slvs_Param& theParam1, const Slvs_Param& theParam2);
/// \brief Compare two entities to be different
static bool IsNotEqual(const Slvs_Entity& theEntity1, const Slvs_Entity& theEntity2);
/// \brief Compare two constraints to be different
static bool IsNotEqual(const Slvs_Constraint& theConstraint1, const Slvs_Constraint& theConstraint2);


SolveSpaceSolver_Storage::SolveSpaceSolver_Storage(const GroupID& theGroup)
  : SketchSolver_Storage(theGroup),
    myWorkplaneID(SLVS_E_UNKNOWN),
    myParamMaxID(SLVS_E_UNKNOWN),
    myEntityMaxID(SLVS_E_UNKNOWN),
    myConstrMaxID(SLVS_C_UNKNOWN),
    myFixed(SLVS_E_UNKNOWN),
    myDuplicatedConstraint(false)
{
}

bool SolveSpaceSolver_Storage::update(ConstraintWrapperPtr theConstraint)
{
  bool isUpdated = false;
  std::shared_ptr<SolveSpaceSolver_ConstraintWrapper> aConstraint =
      std::dynamic_pointer_cast<SolveSpaceSolver_ConstraintWrapper>(theConstraint);
  Slvs_Constraint aSlvsConstr = getConstraint((Slvs_hConstraint)aConstraint->id());
  if (aSlvsConstr.h == SLVS_C_UNKNOWN)
    aSlvsConstr = aConstraint->constraint();

  // update value of constraint if exist
  if (fabs(aSlvsConstr.valA - theConstraint->value()) > tolerance) {
    aSlvsConstr.valA = theConstraint->value();
    isUpdated = true;
  }

  // update constrained entities
  Slvs_hEntity* aPnts[2] = {&aSlvsConstr.ptA, &aSlvsConstr.ptB};
  Slvs_hEntity* anEnts[4] = {&aSlvsConstr.entityA, &aSlvsConstr.entityB,
                             &aSlvsConstr.entityC, &aSlvsConstr.entityD};
  int aPtInd = 0;
  int aEntInd = 0;

  std::list<EntityWrapperPtr> anEntities = theConstraint->entities();
  std::list<EntityWrapperPtr>::iterator anIt = anEntities.begin();
  for (; anIt != anEntities.end(); ++anIt) {
    isUpdated = update(*anIt) || isUpdated;
    // do not update constrained entities for Multi constraints
    if (aSlvsConstr.type == SLVS_C_MULTI_ROTATION || aSlvsConstr.type == SLVS_C_MULTI_TRANSLATION)
      continue;

    Slvs_hEntity anID = (Slvs_hEntity)(*anIt)->id();
    if ((*anIt)->type() == ENTITY_POINT) {
      if (*(aPnts[aPtInd]) != anID) {
        *(aPnts[aPtInd]) = anID;
        isUpdated = true;
      }
      ++aPtInd;
    } else {
      if (*(anEnts[aEntInd]) != anID) {
        *(anEnts[aEntInd]) = anID;
        isUpdated = true;
      }
      ++aEntInd;
    }
  }

  // update constraint itself (do not update constraints Multi)
  if (aSlvsConstr.type != SLVS_C_MULTI_ROTATION && aSlvsConstr.type != SLVS_C_MULTI_TRANSLATION) {
    if (aSlvsConstr.wrkpl == SLVS_E_UNKNOWN && myWorkplaneID != SLVS_E_UNKNOWN)
      aSlvsConstr.wrkpl = myWorkplaneID;
    if (aSlvsConstr.group == SLVS_G_UNKNOWN)
      aSlvsConstr.group = (Slvs_hGroup)myGroupID;
    Slvs_hConstraint aConstrID = updateConstraint(aSlvsConstr);
    if (aSlvsConstr.h == SLVS_C_UNKNOWN) {
      aConstraint->changeConstraint() = getConstraint(aConstrID);
      isUpdated = true;
    }
  }
  return isUpdated;
}

bool SolveSpaceSolver_Storage::update(EntityWrapperPtr theEntity)
{
  bool isUpdated = false;
  std::shared_ptr<SolveSpaceSolver_EntityWrapper> anEntity = 
      std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(theEntity);
  Slvs_Entity aSlvsEnt = getEntity((Slvs_hEntity)anEntity->id());
  if (aSlvsEnt.h == SLVS_E_UNKNOWN)
    aSlvsEnt = anEntity->entity();

  std::list<ParameterWrapperPtr> aParams = theEntity->parameters();
  std::list<ParameterWrapperPtr>::iterator aPIt;
  // if the entity is an attribute, need to update its coordinates
  if (anEntity->baseAttribute()) {
    BuilderPtr aBuilder = SolveSpaceSolver_Builder::getInstance();
    EntityWrapperPtr anUpdAttr = aBuilder->createAttribute(anEntity->baseAttribute(), GID_UNKNOWN);
    if (anUpdAttr) {
      std::list<ParameterWrapperPtr> anUpdParams = anUpdAttr->parameters();
      std::list<ParameterWrapperPtr>::iterator anUpdIt = anUpdParams.begin();
      for (aPIt = aParams.begin(); aPIt != aParams.end() && anUpdIt != anUpdParams.end();
          ++aPIt, ++anUpdIt) {
        (*aPIt)->update(*anUpdIt);
      }
    }
  }

  // update parameters
  int anInd = 0;
  for (aPIt = aParams.begin(); aPIt != aParams.end(); ++aPIt, ++anInd) {
    assert(anInd < 4);
    isUpdated = update(*aPIt) || isUpdated;
    if (aSlvsEnt.param[anInd] != (Slvs_hEntity)(*aPIt)->id()) {
      isUpdated = true;
      aSlvsEnt.param[anInd] = (Slvs_hEntity)(*aPIt)->id();
    }
  }

  // update sub-entities
  std::list<EntityWrapperPtr> aSubEntities = theEntity->subEntities();
  std::list<EntityWrapperPtr>::iterator aSIt = aSubEntities.begin();
  for (anInd = 0; aSIt != aSubEntities.end(); ++aSIt, ++anInd) {
    assert(anInd < 4);
    isUpdated = update(*aSIt) || isUpdated;

    Slvs_hEntity anID = Slvs_hEntity((*aSIt)->id());
    if ((*aSIt)->type() == ENTITY_NORMAL)
      aSlvsEnt.normal = anID;
    else if ((*aSIt)->type() == ENTITY_SCALAR)
      aSlvsEnt.distance = anID;
    else if (aSlvsEnt.point[anInd] != anID) {
      aSlvsEnt.point[anInd] = anID;
      isUpdated = true;
    }
  }
  if (theEntity->type() == ENTITY_POINT && aSubEntities.size() == 1) {
    // theEntity is based on SketchPlugin_Point => need to substitute its attribute instead
    bool isNew = (aSlvsEnt.h == SLVS_E_UNKNOWN);
    aSlvsEnt = getEntity(aSlvsEnt.point[0]);
    if (isNew) {
      anEntity->changeEntity() = aSlvsEnt;
      isUpdated = true;
    }
  }

  // update entity itself
  if (aSlvsEnt.wrkpl == SLVS_E_UNKNOWN && myWorkplaneID != SLVS_E_UNKNOWN)
    aSlvsEnt.wrkpl = myWorkplaneID;
  if (aSlvsEnt.group == SLVS_G_UNKNOWN)
    aSlvsEnt.group = (Slvs_hGroup)myGroupID;
  Slvs_hEntity anEntID = updateEntity(aSlvsEnt);
  if (aSlvsEnt.h == SLVS_E_UNKNOWN || isUpdated) {
    anEntity->changeEntity() = getEntity(anEntID);
    isUpdated = true;

    if (anEntity->type() == ENTITY_SKETCH)
      storeWorkplane(anEntity);
  }

  return isUpdated;
}

bool SolveSpaceSolver_Storage::update(ParameterWrapperPtr theParameter)
{
  std::shared_ptr<SolveSpaceSolver_ParameterWrapper> aParameter = 
      std::dynamic_pointer_cast<SolveSpaceSolver_ParameterWrapper>(theParameter);
  const Slvs_Param& aParam = getParameter((Slvs_hParam)aParameter->id());
  if (aParam.h != SLVS_E_UNKNOWN && fabs(aParam.val - aParameter->value()) < tolerance)
    return false;
  Slvs_Param aParamToUpd = aParameter->parameter();
  if (aParamToUpd.group == SLVS_G_UNKNOWN)
    aParamToUpd.group = aParameter->isParametric() ? (Slvs_hGroup)GID_OUTOFGROUP : (Slvs_hGroup)myGroupID;
  Slvs_hParam anID = updateParameter(aParamToUpd);
  if (aParam.h == SLVS_E_UNKNOWN) // new parameter
    aParameter->changeParameter() = getParameter(anID);
  return true;
}

void SolveSpaceSolver_Storage::storeWorkplane(EntityWrapperPtr theSketch)
{
  myWorkplaneID = (Slvs_hEntity)theSketch->id();

  // Update sub-entities of the sketch
  std::list<EntityWrapperPtr> aSubEntities = theSketch->subEntities();
  std::list<EntityWrapperPtr>::iterator aSIt = aSubEntities.begin();
  for (; aSIt != aSubEntities.end(); ++aSIt) {
    std::shared_ptr<SolveSpaceSolver_EntityWrapper> aSub =
        std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(*aSIt);
    aSub->changeEntity().wrkpl = myWorkplaneID;
  }

  // Update all stored entities
  std::vector<Slvs_Entity>::iterator anIt = myEntities.begin();
  for (; anIt != myEntities.end(); ++anIt)
    anIt->wrkpl = myWorkplaneID;
}

void SolveSpaceSolver_Storage::changeGroup(EntityWrapperPtr theEntity, const GroupID& theGroup)
{
  std::list<ParameterWrapperPtr> aParams = theEntity->parameters();
  std::list<ParameterWrapperPtr>::iterator aPIt = aParams.begin();
  for (; aPIt != aParams.end(); ++aPIt)
    changeGroup(*aPIt, theGroup);

  std::list<EntityWrapperPtr> aSubs = theEntity->subEntities();
  std::list<EntityWrapperPtr>::iterator aSIt = aSubs.begin();
  for (; aSIt != aSubs.end(); ++aSIt)
    changeGroup(*aSIt, theGroup);

  if (theEntity->group() != theGroup) {
    theEntity->setGroup(theGroup);
    int aPos = Search((Slvs_hEntity)theEntity->id(), myEntities);
    if (aPos >= 0 && aPos < (int)myEntities.size()) {
      myEntities[aPos].group = (Slvs_hGroup)theGroup;
      setNeedToResolve(true);
    }
  }
}

void SolveSpaceSolver_Storage::changeGroup(ParameterWrapperPtr theParam, const GroupID& theGroup)
{
  GroupID aGroup = theGroup;
  if (theParam->isParametric())
    aGroup = GID_OUTOFGROUP;
  if (theParam->group() == aGroup)
    return;

  theParam->setGroup(aGroup);
  int aPos = Search((Slvs_hParam)theParam->id(), myParameters);
  if (aPos >= 0 && aPos < (int)myParameters.size()) {
    myParameters[aPos].group = (Slvs_hGroup)aGroup;
    setNeedToResolve(true);
  }
}

void SolveSpaceSolver_Storage::addCoincidentPoints(
    EntityWrapperPtr theMaster, EntityWrapperPtr theSlave)
{
  if (theMaster->type() != ENTITY_POINT || theSlave->type() != ENTITY_POINT)
    return;

  // Search available coincidence
  CoincidentPointsMap::iterator aMasterFound = myCoincidentPoints.find(theMaster);
  CoincidentPointsMap::iterator aSlaveFound = myCoincidentPoints.find(theSlave);
  if (aMasterFound == myCoincidentPoints.end() &&  aSlaveFound == myCoincidentPoints.end()) {
    // try to find master and slave points in the lists of slaves of already existent coincidences
    CoincidentPointsMap::iterator anIt = myCoincidentPoints.begin();
    for (; anIt != myCoincidentPoints.end(); ++anIt) {
      if (anIt->second.find(theMaster) != anIt->second.end())
        aMasterFound = anIt;
      else if (anIt->second.find(theSlave) != anIt->second.end())
        aSlaveFound = anIt;

      if (aMasterFound != myCoincidentPoints.end() &&  aSlaveFound != myCoincidentPoints.end())
        break;
    }
  }

  if (aMasterFound == myCoincidentPoints.end()) {
    // create new group
    myCoincidentPoints[theMaster] = std::set<EntityWrapperPtr>();
    aMasterFound = myCoincidentPoints.find(theMaster);
  } else if (aMasterFound == aSlaveFound)
    return; // already coincident

  if (aSlaveFound != myCoincidentPoints.end()) {
    // A slave has been found, we need to attach all points coincident with it to the new master
    std::set<EntityWrapperPtr> aNewSlaves = aSlaveFound->second;
    aNewSlaves.insert(aSlaveFound->first);
    myCoincidentPoints.erase(aSlaveFound);

    std::set<EntityWrapperPtr>::const_iterator aSlIt = aNewSlaves.begin();
    for (; aSlIt != aNewSlaves.end(); ++aSlIt)
      addCoincidentPoints(theMaster, *aSlIt);
  } else {
    // Update the slave if it was used in constraints and features
    replaceInFeatures(theSlave, theMaster);
    replaceInConstraints(theSlave, theMaster);

    // Remove slave entity (if the IDs are equal no need to remove slave entity, just update it)
    if (theMaster->id() != theSlave->id())
      removeEntity((Slvs_hEntity)theSlave->id());

    std::shared_ptr<SolveSpaceSolver_EntityWrapper> aPointMaster = 
        std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(theMaster);
    std::shared_ptr<SolveSpaceSolver_EntityWrapper> aPointSlave = 
        std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(theSlave);
    aPointSlave->changeEntity() = aPointMaster->entity();
    aPointSlave->setParameters(aPointMaster->parameters());

    aMasterFound->second.insert(theSlave);
  }
}

void SolveSpaceSolver_Storage::replaceInFeatures(
    EntityWrapperPtr theSource, EntityWrapperPtr theDest)
{
  std::map<FeaturePtr, EntityWrapperPtr>::const_iterator anIt = myFeatureMap.begin();
  for (; anIt != myFeatureMap.end(); ++anIt) {
    if (!anIt->second)
      continue;
    bool isUpdated = false;
    std::list<EntityWrapperPtr> aSubs = anIt->second->subEntities();
    std::list<EntityWrapperPtr>::iterator aSubIt = aSubs.begin();
    for (; aSubIt != aSubs.end(); ++aSubIt)
      if ((*aSubIt)->id() == theSource->id()) {
        (*aSubIt)->update(theDest);
        isUpdated = true;
      }

    if (!isUpdated)
      continue;

    std::shared_ptr<SolveSpaceSolver_EntityWrapper> aWrapper =
        std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(anIt->second);
    // update SolveSpace entity
    Slvs_Entity anEnt = aWrapper->entity();
    for (int i = 0; i < 4; ++i)
      if (anEnt.point[i] == (Slvs_hEntity)theSource->id())
        anEnt.point[i] = (Slvs_hEntity)theDest->id();
    anEnt.h = updateEntity(anEnt);
    aWrapper->changeEntity() = anEnt;

    // update sub-entities
    aWrapper->setSubEntities(aSubs);
  }
}

void SolveSpaceSolver_Storage::replaceInConstraints(
    EntityWrapperPtr theSource, EntityWrapperPtr theDest)
{
  std::map<ConstraintPtr, std::list<ConstraintWrapperPtr> >::const_iterator
      anIt = myConstraintMap.begin();
  std::list<ConstraintWrapperPtr>::const_iterator aCIt;
  for (; anIt != myConstraintMap.end(); ++anIt)
    for (aCIt = anIt->second.begin(); aCIt != anIt->second.end(); ++aCIt) {
      // Do not process coincidence between points
      // (these constraints are stored to keep the structure of constraints).
      if ((*aCIt)->type() == CONSTRAINT_PT_PT_COINCIDENT)
        continue;

      bool isUpdated = false;
      std::list<EntityWrapperPtr> aSubs = (*aCIt)->entities();
      std::list<EntityWrapperPtr>::iterator aSubIt = aSubs.begin();
      for (; aSubIt != aSubs.end(); ++aSubIt)
        if ((*aSubIt)->id() == theSource->id()) {
          (*aSubIt)->update(theDest);
          isUpdated = true;
        }

      if (!isUpdated)
        continue;

      std::shared_ptr<SolveSpaceSolver_ConstraintWrapper> aWrapper =
          std::dynamic_pointer_cast<SolveSpaceSolver_ConstraintWrapper>(*aCIt);
      // change constraint entities
      Slvs_Constraint aConstr = aWrapper->constraint();
      if (aConstr.ptA == (Slvs_hEntity)theSource->id())
        aConstr.ptA = (Slvs_hEntity)theDest->id();
      if (aConstr.ptB == (Slvs_hEntity)theSource->id())
        aConstr.ptB = (Slvs_hEntity)theDest->id();

      // check the constraint is duplicated
      std::vector<Slvs_Constraint>::const_iterator aSlvsCIt = myConstraints.begin();
      for (; aSlvsCIt != myConstraints.end(); ++aSlvsCIt)
        if (aConstr.h != aSlvsCIt->h &&
            aConstr.type == aSlvsCIt->type &&
            aConstr.ptA == aSlvsCIt->ptA && aConstr.ptB == aSlvsCIt->ptB &&
            aConstr.entityA == aSlvsCIt->entityA && aConstr.entityB == aSlvsCIt->entityB &&
            aConstr.entityC == aSlvsCIt->entityC && aConstr.entityD == aSlvsCIt->entityD) {
          removeConstraint(aConstr.h);
          aConstr = *aSlvsCIt;
          break;
        }

      if (aSlvsCIt != myConstraints.end()) {
        // constraint is duplicated, search its wrapper to add the mapping
        std::map<ConstraintPtr, std::list<ConstraintWrapperPtr> >::const_iterator
            anIt2 = myConstraintMap.begin();
        std::list<ConstraintWrapperPtr>::const_iterator aCIt2;
        for (; anIt2 != myConstraintMap.end(); ++anIt2)
          for (aCIt2 = anIt2->second.begin(); aCIt2 != anIt2->second.end(); ++aCIt2)
            if ((Slvs_hConstraint)(*aCIt2)->id() == aConstr.h) {
              addSameConstraints(*aCIt2, aWrapper);
              break;
            }
      } else 
        aConstr.h = updateConstraint(aConstr);
      aWrapper->changeConstraint() = aConstr;

      // update sub-entities
      aWrapper->setEntities(aSubs);
    }
}

void SolveSpaceSolver_Storage::addSameConstraints(ConstraintWrapperPtr theConstraint1,
                                                  ConstraintWrapperPtr theConstraint2)
{
  SameConstraintMap::iterator anIt = myEqualConstraints.begin();
  for (; anIt != myEqualConstraints.end(); ++anIt) {
    if (anIt->find(theConstraint1) != anIt->end()) {
      anIt->insert(theConstraint2);
      return;
    }
    else if (anIt->find(theConstraint2) != anIt->end()) {
      anIt->insert(theConstraint1);
      return;
    }
  }
  // group not found => create new one
  std::set<ConstraintWrapperPtr> aNewGroup;
  aNewGroup.insert(theConstraint1);
  aNewGroup.insert(theConstraint2);
  myEqualConstraints.push_back(aNewGroup);
}


EntityWrapperPtr SolveSpaceSolver_Storage::calculateMiddlePoint(
    EntityWrapperPtr theBase, double theCoeff)
{
  BuilderPtr aBuilder = SolveSpaceSolver_Builder::getInstance();

  std::shared_ptr<GeomAPI_XY> aMidPoint;
  if (theBase->type() == ENTITY_LINE) {
    std::shared_ptr<GeomAPI_Pnt2d> aPoints[2];
    const std::list<EntityWrapperPtr>& aSubs = theBase->subEntities();
    std::list<EntityWrapperPtr>::const_iterator anIt = aSubs.begin();
    for (int i = 0; i < 2; ++i, ++anIt)
      aPoints[i] = aBuilder->point(*anIt);
    aMidPoint = aPoints[0]->xy()->multiplied(1.0 - theCoeff)->added(
        aPoints[1]->xy()->multiplied(theCoeff));
  }
  else if (theBase->type() == ENTITY_ARC) {
    double theX, theY;
    double anArcPoint[3][2];
    const std::list<EntityWrapperPtr>& aSubs = theBase->subEntities();
    std::list<EntityWrapperPtr>::const_iterator anIt = aSubs.begin();
    for (int i = 0; i < 3; ++i, ++anIt) {
      std::shared_ptr<GeomAPI_Pnt2d> aPoint = aBuilder->point(*anIt);
      anArcPoint[i][0] = aPoint->x();
      anArcPoint[i][1] = aPoint->y();
    }
    // project last point of arc on the arc
    double x = anArcPoint[1][0] - anArcPoint[0][0];
    double y = anArcPoint[1][1] - anArcPoint[0][1];
    double aRad = sqrt(x*x + y*y);
    x = anArcPoint[2][0] - anArcPoint[0][0];
    y = anArcPoint[2][1] - anArcPoint[0][1];
    double aNorm = sqrt(x*x + y*y);
    if (aNorm >= tolerance) {
      anArcPoint[2][0] = x * aRad / aNorm;
      anArcPoint[2][1] = y * aRad / aNorm;
    }
    anArcPoint[1][0] -= anArcPoint[0][0];
    anArcPoint[1][1] -= anArcPoint[0][1];
    if (theCoeff < tolerance) {
      theX = anArcPoint[0][0] + anArcPoint[1][0];
      theY = anArcPoint[0][1] + anArcPoint[1][1];
    } else if (1 - theCoeff < tolerance) {
      theX = anArcPoint[0][0] + anArcPoint[2][0];
      theY = anArcPoint[0][1] + anArcPoint[2][1];
    } else {
      std::shared_ptr<GeomAPI_Dir2d> aStartDir(new GeomAPI_Dir2d(anArcPoint[1][0], anArcPoint[1][1]));
      std::shared_ptr<GeomAPI_Dir2d> aEndDir(new GeomAPI_Dir2d(anArcPoint[2][0], anArcPoint[2][1]));
      double anAngle = aStartDir->angle(aEndDir);
      if (anAngle < 0)
        anAngle += 2.0 * PI;
      anAngle *= theCoeff;
      double aCos = cos(anAngle);
      double aSin = sin(anAngle);
      theX = anArcPoint[0][0] + anArcPoint[1][0] * aCos - anArcPoint[1][1] * aSin;
      theY = anArcPoint[0][1] + anArcPoint[1][0] * aSin + anArcPoint[1][1] * aCos;
    }
    aMidPoint = std::shared_ptr<GeomAPI_XY>(new GeomAPI_XY(theX, theY));
  }

  if (!aMidPoint)
    return EntityWrapperPtr();

  std::list<ParameterWrapperPtr> aParameters;
  Slvs_Param aParam1 = Slvs_MakeParam(SLVS_E_UNKNOWN, (Slvs_hGroup)myGroupID, aMidPoint->x());
  aParam1.h = addParameter(aParam1);
  aParameters.push_back(ParameterWrapperPtr(new SolveSpaceSolver_ParameterWrapper(aParam1)));
  Slvs_Param aParam2 = Slvs_MakeParam(SLVS_E_UNKNOWN, (Slvs_hGroup)myGroupID, aMidPoint->y());
  aParam2.h = addParameter(aParam2);
  aParameters.push_back(ParameterWrapperPtr(new SolveSpaceSolver_ParameterWrapper(aParam2)));
  // Create entity (parameters are not filled)
  Slvs_Entity anEntity = Slvs_MakePoint2d(SLVS_E_UNKNOWN, (Slvs_hGroup)myGroupID,
      (Slvs_hEntity)myWorkplaneID, aParam1.h, aParam2.h);
  anEntity.h = addEntity(anEntity);

  EntityWrapperPtr aResult(new SolveSpaceSolver_EntityWrapper(AttributePtr(), anEntity));
  aResult->setParameters(aParameters);
  return aResult;
}






Slvs_hParam SolveSpaceSolver_Storage::addParameter(const Slvs_Param& theParam)
{
  if (theParam.h > 0 && theParam.h <= myParamMaxID) {
    // parameter is already used, rewrite it
    return updateParameter(theParam);
  }

  Slvs_Param aParam = theParam;
  if (aParam.h > myParamMaxID)
    myParamMaxID = aParam.h;
  else
    aParam.h = ++myParamMaxID;
  myParameters.push_back(aParam);
  myNeedToResolve = true;
  return aParam.h;
}

Slvs_hParam SolveSpaceSolver_Storage::updateParameter(const Slvs_Param& theParam)
{
  if (theParam.h > 0 && theParam.h <= myParamMaxID) {
    // parameter already used, rewrite it
    int aPos = Search(theParam.h, myParameters);
    if (aPos >= 0 && aPos < (int)myParameters.size()) {
      if (IsNotEqual(myParameters[aPos], theParam)) {
        myUpdatedParameters.insert(theParam.h);
        setNeedToResolve(true);
      }
      myParameters[aPos] = theParam;
      return theParam.h;
    }
  }

  // Parameter is not found, add new one
  Slvs_Param aParam = theParam;
  aParam.h = 0;
  return addParameter(aParam);
}

bool SolveSpaceSolver_Storage::removeParameter(const Slvs_hParam& theParamID)
{
  int aPos = Search(theParamID, myParameters);
  if (aPos >= 0 && aPos < (int)myParameters.size()) {
    // Firstly, search the parameter is not used elsewhere
    std::vector<Slvs_Entity>::const_iterator anEntIter = myEntities.begin();
    for (; anEntIter != myEntities.end(); anEntIter++) {
      for (int i = 0; i < 4; i++)
        if (anEntIter->param[i] == theParamID)
          return false;
    }
    // Remove parameter
    myParameters.erase(myParameters.begin() + aPos);
    myParamMaxID = myParameters.empty() ? SLVS_E_UNKNOWN : myParameters.back().h;
    myNeedToResolve = true;
  }
  return true;
}

const Slvs_Param& SolveSpaceSolver_Storage::getParameter(const Slvs_hParam& theParamID) const
{
  int aPos = Search(theParamID, myParameters);
  if (aPos >= 0 && aPos < (int)myParameters.size())
    return myParameters[aPos];

  // Parameter is not found, return empty object
  static Slvs_Param aDummy;
  aDummy.h = 0;
  return aDummy;
}


Slvs_hEntity SolveSpaceSolver_Storage::addEntity(const Slvs_Entity& theEntity)
{
  if (theEntity.h > 0 && theEntity.h <= myEntityMaxID) {
    // Entity is already used, rewrite it
    return updateEntity(theEntity);
  }

  Slvs_Entity aEntity = theEntity;
  if (aEntity.h > myEntityMaxID)
    myEntityMaxID = aEntity.h;
  else
    aEntity.h = ++myEntityMaxID;
  myEntities.push_back(aEntity);
  myNeedToResolve = true;
  return aEntity.h;
}

Slvs_hEntity SolveSpaceSolver_Storage::updateEntity(const Slvs_Entity& theEntity)
{
  if (theEntity.h > 0 && theEntity.h <= myEntityMaxID) {
    // Entity already used, rewrite it
    int aPos = Search(theEntity.h, myEntities);
    if (aPos >= 0 && aPos < (int)myEntities.size()) {
      myNeedToResolve = myNeedToResolve || IsNotEqual(myEntities[aPos], theEntity);
      myEntities[aPos] = theEntity;
      return theEntity.h;
    }
  }

  // Entity is not found, add new one
  Slvs_Entity aEntity = theEntity;
  aEntity.h = 0;
  return addEntity(aEntity);
}

bool SolveSpaceSolver_Storage::removeEntity(const Slvs_hEntity& theEntityID)
{
  bool aResult = true;
  int aPos = Search(theEntityID, myEntities);
  if (aPos >= 0 && aPos < (int)myEntities.size()) {
    // Firstly, check the entity and its attributes is not used elsewhere
    std::set<Slvs_hEntity> anEntAndSubs;
    anEntAndSubs.insert(theEntityID);
    for (int i = 0; i < 4; i++)
      if (myEntities[aPos].point[i] != SLVS_E_UNKNOWN)
        anEntAndSubs.insert(myEntities[aPos].point[i]);

    std::vector<Slvs_Entity>::const_iterator anEntIter = myEntities.begin();
    for (; anEntIter != myEntities.end(); anEntIter++) {
      if (anEntAndSubs.find(anEntIter->h) != anEntAndSubs.end())
        continue;
      for (int i = 0; i < 4; i++)
        if (anEntAndSubs.find(anEntIter->point[i]) != anEntAndSubs.end())
          return false;
      if (anEntAndSubs.find(anEntIter->distance) != anEntAndSubs.end())
        return false;
    }
    std::vector<Slvs_Constraint>::const_iterator aConstrIter = myConstraints.begin();
    for (; aConstrIter != myConstraints.end(); aConstrIter++) {
      Slvs_hEntity anEntIDs[6] = {aConstrIter->ptA, aConstrIter->ptB,
          aConstrIter->entityA, aConstrIter->entityB,
          aConstrIter->entityC, aConstrIter->entityD};
      for (int i = 0; i < 6; i++)
        if (anEntAndSubs.find(anEntIDs[i]) != anEntAndSubs.end())
          return false;
    }
    // The entity is not used, remove it and its parameters
    Slvs_Entity anEntity = myEntities[aPos];
    myEntities.erase(myEntities.begin() + aPos);
    myEntityMaxID = myEntities.empty() ? SLVS_E_UNKNOWN : myEntities.back().h;
    if (anEntity.distance != SLVS_E_UNKNOWN)
      aResult = aResult && removeParameter(anEntity.distance);
    for (int i = 0; i < 4; i++)
      if (anEntity.param[i] != SLVS_E_UNKNOWN)
        aResult = removeParameter(anEntity.param[i]) && aResult;
    for (int i = 0; i < 4; i++)
      if (anEntity.point[i] != SLVS_E_UNKNOWN)
        aResult = removeEntity(anEntity.point[i]) && aResult;
    myNeedToResolve = true;
  }
  return aResult;
}

void SolveSpaceSolver_Storage::removeUnusedEntities()
{
  std::set<Slvs_hEntity> anUnusedEntities;
  std::vector<Slvs_Entity>::const_iterator aEIt = myEntities.begin();
  for (; aEIt != myEntities.end(); ++aEIt) {
    if (aEIt->h == aEIt->wrkpl) {
      // don't remove workplane
      anUnusedEntities.erase(aEIt->point[0]);
      anUnusedEntities.erase(aEIt->normal);
      continue;
    }
    anUnusedEntities.insert(aEIt->h);
  }

  std::vector<Slvs_Constraint>::const_iterator aCIt = myConstraints.begin();
  for (; aCIt != myConstraints.end(); ++aCIt) {
    Slvs_hEntity aSubs[6] = {
        aCIt->entityA, aCIt->entityB,
        aCIt->entityC, aCIt->entityD,
        aCIt->ptA,     aCIt->ptB};
    for (int i = 0; i < 6; i++) {
      if (aSubs[i] != SLVS_E_UNKNOWN) {
        anUnusedEntities.erase(aSubs[i]);
        int aPos = Search(aSubs[i], myEntities);
        if (aPos >= 0 && aPos < (int)myEntities.size()) {
          for (int j = 0; j < 4; j++)
            if (myEntities[aPos].point[j] != SLVS_E_UNKNOWN)
              anUnusedEntities.erase(myEntities[aPos].point[j]);
          if (myEntities[aPos].distance != SLVS_E_UNKNOWN)
            anUnusedEntities.erase(myEntities[aPos].distance);
        }
      }
    }
  }

  std::set<Slvs_hEntity>::const_iterator anEntIt = anUnusedEntities.begin();
  while (anEntIt != anUnusedEntities.end()) {
    int aPos = Search(*anEntIt, myEntities);
    if (aPos < 0 && aPos >= (int)myEntities.size())
      continue;
    Slvs_Entity anEntity = myEntities[aPos];
    // Remove entity if and only if all its parameters unused
    bool isUsed = false;
    if (anEntity.distance != SLVS_E_UNKNOWN && 
      anUnusedEntities.find(anEntity.distance) == anUnusedEntities.end())
      isUsed = true;
    for (int i = 0; i < 4 && !isUsed; i++)
      if (anEntity.point[i] != SLVS_E_UNKNOWN &&
          anUnusedEntities.find(anEntity.point[i]) == anUnusedEntities.end())
        isUsed = true;
    if (isUsed) {
      anUnusedEntities.erase(anEntity.distance);
      for (int i = 0; i < 4; i++)
        if (anEntity.point[i] != SLVS_E_UNKNOWN)
          anUnusedEntities.erase(anEntity.point[i]);
      std::set<Slvs_hEntity>::iterator aRemoveIt = anEntIt++;
      anUnusedEntities.erase(aRemoveIt);
      continue;
    }
    ++anEntIt;
  }

  for (anEntIt = anUnusedEntities.begin(); anEntIt != anUnusedEntities.end(); ++anEntIt) {
    int aPos = Search(*anEntIt, myEntities);
    if (aPos >= 0 && aPos < (int)myEntities.size()) {
      // Remove entity and its parameters
      Slvs_Entity anEntity = myEntities[aPos];
      myEntities.erase(myEntities.begin() + aPos);
      myEntityMaxID = myEntities.empty() ? SLVS_E_UNKNOWN : myEntities.back().h;
      if (anEntity.distance != SLVS_E_UNKNOWN)
        removeParameter(anEntity.distance);
      for (int i = 0; i < 4; i++)
        if (anEntity.param[i] != SLVS_E_UNKNOWN)
          removeParameter(anEntity.param[i]);
      for (int i = 0; i < 4; i++)
        if (anEntity.point[i] != SLVS_E_UNKNOWN)
          removeEntity(anEntity.point[i]);
    }
  }

  if (!anUnusedEntities.empty())
    myNeedToResolve = true;
}

bool SolveSpaceSolver_Storage::isUsedByConstraints(const Slvs_hEntity& theEntityID) const
{
  std::vector<Slvs_Constraint>::const_iterator aCIt = myConstraints.begin();
  for (; aCIt != myConstraints.end(); ++aCIt) {
    Slvs_hEntity aSubs[6] = {
        aCIt->entityA, aCIt->entityB,
        aCIt->entityC, aCIt->entityD,
        aCIt->ptA,     aCIt->ptB};
    for (int i = 0; i < 6; i++)
      if (aSubs[i] != SLVS_E_UNKNOWN && aSubs[i] == theEntityID)
        return true;
  }
  return false;
}

const Slvs_Entity& SolveSpaceSolver_Storage::getEntity(const Slvs_hEntity& theEntityID) const
{
  int aPos = Search(theEntityID, myEntities);
  if (aPos >= 0 && aPos < (int)myEntities.size())
    return myEntities[aPos];

  // Entity is not found, return empty object
  static Slvs_Entity aDummy;
  aDummy.h = SLVS_E_UNKNOWN;
  return aDummy;
}

Slvs_hEntity SolveSpaceSolver_Storage::copyEntity(const Slvs_hEntity& theCopied)
{
  int aPos = Search(theCopied, myEntities);
  if (aPos < 0 || aPos >= (int)myEntities.size())
    return SLVS_E_UNKNOWN;

  Slvs_Entity aCopy = myEntities[aPos];
  aCopy.h = SLVS_E_UNKNOWN;
  int i = 0;
  while (aCopy.point[i] != SLVS_E_UNKNOWN) {
    aCopy.point[i] = copyEntity(aCopy.point[i]);
    i++;
  }
  if (aCopy.param[0] != SLVS_E_UNKNOWN) {
    aPos = Search(aCopy.param[0], myParameters);
    i = 0;
    while (aCopy.param[i] != SLVS_E_UNKNOWN) {
      Slvs_Param aNewParam = myParameters[aPos];
      aNewParam.h = SLVS_E_UNKNOWN;
      aCopy.param[i] = addParameter(aNewParam);
      i++;
      aPos++;
    }
  }
  return addEntity(aCopy);
}

void SolveSpaceSolver_Storage::copyEntity(const Slvs_hEntity& theFrom, const Slvs_hEntity& theTo)
{
  int aPosFrom = Search(theFrom, myEntities);
  int aPosTo = Search(theTo, myEntities);
  if (aPosFrom < 0 || aPosFrom >= (int)myEntities.size() || 
      aPosTo < 0 || aPosTo >= (int)myEntities.size())
    return;

  Slvs_Entity aEntFrom = myEntities[aPosFrom];
  Slvs_Entity aEntTo = myEntities[aPosTo];
  int i = 0;
  while (aEntFrom.point[i] != SLVS_E_UNKNOWN) {
    copyEntity(aEntFrom.point[i], aEntTo.point[i]);
    i++;
  }
  if (aEntFrom.param[0] != SLVS_E_UNKNOWN) {
    aPosFrom = Search(aEntFrom.param[0], myParameters);
    aPosTo = Search(aEntTo.param[0], myParameters);
    i = 0;
    while (aEntFrom.param[i] != SLVS_E_UNKNOWN) {
      myParameters[aPosTo++].val = myParameters[aPosFrom++].val;
      i++;
    }
  }
}


bool SolveSpaceSolver_Storage::isPointFixed(
    const Slvs_hEntity& thePointID, Slvs_hConstraint& theFixed, bool theAccurate) const
{
  // Search the set of coincident points
  std::set<Slvs_hEntity> aCoincident;
  aCoincident.insert(thePointID);

  // Check whether one of coincident points is out-of-group
  std::set<Slvs_hEntity>::const_iterator aCoincIt = aCoincident.begin();
  for (; aCoincIt != aCoincident.end(); ++aCoincIt) {
    Slvs_Entity aPoint = getEntity(*aCoincIt);
    if (aPoint.group == SLVS_G_OUTOFGROUP)
      return true;
  }

  // Search the Rigid constraint
  theFixed = SLVS_C_UNKNOWN;
  std::vector<Slvs_Constraint>::const_iterator aConstrIter = myConstraints.begin();
  for (; aConstrIter != myConstraints.end(); aConstrIter++)
    if (aConstrIter->type == SLVS_C_WHERE_DRAGGED &&
        aCoincident.find(aConstrIter->ptA) != aCoincident.end()) {
      theFixed = aConstrIter->h;
      if (aConstrIter->ptA == thePointID)
        return true;
    }
  if (theFixed != SLVS_C_UNKNOWN)
    return true;

  if (theAccurate) {
    // Try to find the fixed entity which uses such point or its coincidence
    std::vector<Slvs_Entity>::const_iterator anEntIter = myEntities.begin();
    for (; anEntIter != myEntities.end(); anEntIter++) {
      for (int i = 0; i < 4; i++) {
        Slvs_hEntity aPt = anEntIter->point[i];
        if (aPt != SLVS_E_UNKNOWN &&
            (aPt == thePointID || aCoincident.find(aPt) != aCoincident.end())) {
          if (isEntityFixed(anEntIter->h, true))
            return true;
        }
      }
    }
  }
  return SLVS_E_UNKNOWN;
}

bool SolveSpaceSolver_Storage::isEntityFixed(const Slvs_hEntity& theEntityID, bool theAccurate) const
{
  int aPos = Search(theEntityID, myEntities);
  if (aPos < 0 || aPos >= (int)myEntities.size())
    return false;

  // Firstly, find how many points are under Rigid constraint
  int aNbFixed = 0;
  for (int i = 0; i < 4; i++) {
    Slvs_hEntity aPoint = myEntities[aPos].point[i];
    if (aPoint == SLVS_E_UNKNOWN)
      continue;

    std::set<Slvs_hEntity> aCoincident;
    aCoincident.insert(aPoint);

    // Search the Rigid constraint
    std::vector<Slvs_Constraint>::const_iterator aConstrIter = myConstraints.begin();
    for (; aConstrIter != myConstraints.end(); aConstrIter++)
      if (aConstrIter->type == SLVS_C_WHERE_DRAGGED &&
          aCoincident.find(aConstrIter->ptA) != aCoincident.end())
        aNbFixed++;
  }

  std::list<Slvs_Constraint> aList;
  std::list<Slvs_Constraint>::iterator anIt;
  Slvs_hConstraint aTempID; // used in isPointFixed() method

  if (myEntities[aPos].type == SLVS_E_LINE_SEGMENT) {
    if (aNbFixed == 2)
      return true;
    else if (aNbFixed == 0 || !theAccurate)
      return false;
    // Additional check (the line may be fixed if it is used by different constraints):
    // 1. The line is used in Equal constraint, another entity is fixed and there is a fixed point on line
    aList = getConstraintsByType(SLVS_C_PT_ON_LINE);
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID && isPointFixed(anIt->ptA, aTempID))
        break;
    if (anIt != aList.end()) {
      aList = getConstraintsByType(SLVS_C_EQUAL_LENGTH_LINES);
      aList.splice(aList.end(), getConstraintsByType(SLVS_C_EQUAL_LINE_ARC_LEN));
      for (anIt = aList.begin(); anIt != aList.end(); anIt++)
        if (anIt->entityA == theEntityID || anIt->entityB == theEntityID) {
          Slvs_hEntity anOther = anIt->entityA == theEntityID ? anIt->entityB : anIt->entityA;
          if (isEntityFixed(anOther, false))
            return true;
        }
    }
    // 2. The line is used in Parallel/Perpendicular/Vertical/Horizontal and Length constraints
    aList = getConstraintsByType(SLVS_C_PARALLEL);
    aList.splice(aList.end(), getConstraintsByType(SLVS_C_PERPENDICULAR));
    aList.splice(aList.end(), getConstraintsByType(SLVS_C_VERTICAL));
    aList.splice(aList.end(), getConstraintsByType(SLVS_C_HORIZONTAL));
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID || anIt->entityB == theEntityID) {
        Slvs_hEntity anOther = anIt->entityA == theEntityID ? anIt->entityB : anIt->entityA;
        if (isEntityFixed(anOther, false))
          break;
      }
    if (anIt != aList.end()) {
      aList = getConstraintsByType(SLVS_C_PT_PT_DISTANCE);
      for (anIt = aList.begin(); anIt != aList.end(); anIt++)
        if ((anIt->ptA == myEntities[aPos].point[0] && anIt->ptB == myEntities[aPos].point[1]) ||
            (anIt->ptA == myEntities[aPos].point[1] && anIt->ptB == myEntities[aPos].point[0]))
          return true;
    }
    // 3. Another verifiers ...
  } else if (myEntities[aPos].type == SLVS_E_CIRCLE) {
    if (aNbFixed == 0)
      return false;
    // Search for Diameter constraint
    aList = getConstraintsByType(SLVS_C_DIAMETER);
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID)
        return true;
    if (!theAccurate)
      return false;
    // Additional check (the circle may be fixed if it is used by different constraints):
    // 1. The circle is used in Equal constraint and another entity is fixed
    aList = getConstraintsByType(SLVS_C_EQUAL_RADIUS);
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID || anIt->entityB == theEntityID) {
        Slvs_hEntity anOther = anIt->entityA == theEntityID ? anIt->entityB : anIt->entityA;
        if (isEntityFixed(anOther, false))
          return true;
      }
    // 2. Another verifiers ...
  } else if (myEntities[aPos].type == SLVS_E_ARC_OF_CIRCLE) {
    if (aNbFixed > 2)
      return true;
    else if (aNbFixed <= 1)
      return false;
    // Search for Diameter constraint
    aList = getConstraintsByType(SLVS_C_DIAMETER);
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID)
        return true;
    if (!theAccurate)
      return false;
    // Additional check (the arc may be fixed if it is used by different constraints):
    // 1. The arc is used in Equal constraint and another entity is fixed
    aList = getConstraintsByType(SLVS_C_EQUAL_RADIUS);
    aList.splice(aList.end(), getConstraintsByType(SLVS_C_EQUAL_LINE_ARC_LEN));
    for (anIt = aList.begin(); anIt != aList.end(); anIt++)
      if (anIt->entityA == theEntityID || anIt->entityB == theEntityID) {
        Slvs_hEntity anOther = anIt->entityA == theEntityID ? anIt->entityB : anIt->entityA;
        if (isEntityFixed(anOther, false))
          return true;
      }
    // 2. Another verifiers ...
  }
  return false;
}


Slvs_hConstraint SolveSpaceSolver_Storage::addConstraint(const Slvs_Constraint& theConstraint)
{
  if (theConstraint.h > 0 && theConstraint.h <= myConstrMaxID) {
    // Constraint is already used, rewrite it
    return updateConstraint(theConstraint);
  }

  Slvs_Constraint aConstraint = theConstraint;

  // Find a constraint with same type uses same arguments to show user overconstraint situation
  std::vector<Slvs_Constraint>::iterator aCIt = myConstraints.begin();
  for (; aCIt != myConstraints.end(); aCIt++) {
    if (aConstraint.type != aCIt->type)
      continue;
    if (aConstraint.ptA == aCIt->ptA && aConstraint.ptB == aCIt->ptB &&
        aConstraint.entityA == aCIt->entityA && aConstraint.entityB == aCIt->entityB &&
        aConstraint.entityC == aCIt->entityC && aConstraint.entityD == aCIt->entityD)
      myDuplicatedConstraint = true;
  }

  if (aConstraint.h > myConstrMaxID)
    myConstrMaxID = aConstraint.h;
  else
    aConstraint.h = ++myConstrMaxID;
  myConstraints.push_back(aConstraint);
  myNeedToResolve = true;
  return aConstraint.h;
}

Slvs_hConstraint SolveSpaceSolver_Storage::updateConstraint(const Slvs_Constraint& theConstraint)
{
  if (theConstraint.h > 0 && theConstraint.h <= myConstrMaxID) {
    // Constraint already used, rewrite it
    int aPos = Search(theConstraint.h, myConstraints);
    if (aPos >= 0 && aPos < (int)myConstraints.size()) {
      myNeedToResolve = myNeedToResolve || IsNotEqual(myConstraints[aPos], theConstraint);
      myConstraints[aPos] = theConstraint;
      return theConstraint.h;
    }
  }

  // Constraint is not found, add new one
  Slvs_Constraint aConstraint = theConstraint;
  aConstraint.h = 0;
  return addConstraint(aConstraint);
}

bool SolveSpaceSolver_Storage::removeConstraint(const Slvs_hConstraint& theConstraintID)
{
  bool aResult = true;
  int aPos = Search(theConstraintID, myConstraints);
  if (aPos >= 0 && aPos < (int)myConstraints.size()) {
    Slvs_Constraint aConstraint = myConstraints[aPos];
    myConstraints.erase(myConstraints.begin() + aPos);
    myConstrMaxID = myConstraints.empty() ? SLVS_E_UNKNOWN : myConstraints.back().h;
    myNeedToResolve = true;

    // Remove all entities
    Slvs_hEntity anEntities[6] = {aConstraint.ptA, aConstraint.ptB,
        aConstraint.entityA, aConstraint.entityB,
        aConstraint.entityC, aConstraint.entityD};
    for (int i = 0; i < 6; i++)
      if (anEntities[i] != SLVS_E_UNKNOWN)
        aResult = removeEntity(anEntities[i]) && aResult;
    // remove temporary fixed point, if available
    if (myFixed == theConstraintID)
      myFixed = SLVS_E_UNKNOWN;
    if (myDuplicatedConstraint) {
      // Check the duplicated constraints are still available
      myDuplicatedConstraint = false;
      std::vector<Slvs_Constraint>::const_iterator anIt1 = myConstraints.begin();
      std::vector<Slvs_Constraint>::const_iterator anIt2 = myConstraints.begin();
      for (; anIt1 != myConstraints.end() && !myDuplicatedConstraint; anIt1++)
        for (anIt2 = anIt1+1; anIt2 != myConstraints.end() && !myDuplicatedConstraint; anIt2++) {
          if (anIt1->type != anIt2->type)
            continue;
          if (anIt1->ptA == anIt2->ptA && anIt1->ptB == anIt2->ptB &&
              anIt1->entityA == anIt2->entityA && anIt1->entityB == anIt2->entityB &&
              anIt1->entityC == anIt2->entityC && anIt1->entityD == anIt2->entityD)
            myDuplicatedConstraint = true;
        }
    }
  }
  return aResult;
}

const Slvs_Constraint& SolveSpaceSolver_Storage::getConstraint(const Slvs_hConstraint& theConstraintID) const
{
  int aPos = Search(theConstraintID, myConstraints);
  if (aPos >= 0 && aPos < (int)myConstraints.size())
    return myConstraints[aPos];

  // Constraint is not found, return empty object
  static Slvs_Constraint aDummy;
  aDummy.h = 0;
  return aDummy;
}

std::list<Slvs_Constraint> SolveSpaceSolver_Storage::getConstraintsByType(int theConstraintType) const
{
  std::list<Slvs_Constraint> aResult;
  std::vector<Slvs_Constraint>::const_iterator aCIter = myConstraints.begin();
  for (; aCIter != myConstraints.end(); aCIter++)
    if (aCIter->type == theConstraintType)
      aResult.push_back(*aCIter);
  return aResult;
}


void SolveSpaceSolver_Storage::addConstraintWhereDragged(const Slvs_hConstraint& theConstraintID)
{
  if (myFixed != SLVS_E_UNKNOWN)
    return; // the point is already fixed
  int aPos = Search(theConstraintID, myConstraints);
  if (aPos >= 0 && aPos < (int)myConstraints.size())
    myFixed = theConstraintID;
}


void SolveSpaceSolver_Storage::initializeSolver(SolverPtr theSolver)
{
  std::shared_ptr<SolveSpaceSolver_Solver> aSolver =
      std::dynamic_pointer_cast<SolveSpaceSolver_Solver>(theSolver);
  if (!aSolver)
    return;

  if (myConstraints.empty()) {
    // Adjust all arc to place their points correctly
    std::vector<Slvs_Entity>::const_iterator anEntIt = myEntities.begin();
    for (; anEntIt != myEntities.end(); ++anEntIt)
      if (anEntIt->type == SLVS_E_ARC_OF_CIRCLE)
        adjustArc(*anEntIt);
  }

  aSolver->setParameters(myParameters.data(), (int)myParameters.size());
  aSolver->setEntities(myEntities.data(), (int)myEntities.size());

  // Copy constraints excluding the fixed one
  std::vector<Slvs_Constraint> aConstraints = myConstraints;
  if (myFixed != SLVS_E_UNKNOWN) {
    Slvs_hEntity aFixedPoint = SLVS_E_UNKNOWN;
    std::vector<Slvs_Constraint>::iterator anIt = aConstraints.begin();
    for (; anIt != aConstraints.end(); anIt++)
      if (anIt->h == myFixed) {
        aFixedPoint = anIt->ptA;
        aConstraints.erase(anIt);
        break;
      }
    // set dragged parameters
    int aPos = Search(aFixedPoint, myEntities);
    aSolver->setDraggedParameters(myEntities[aPos].param);
  }
  aSolver->setConstraints(aConstraints.data(), (int)aConstraints.size());
}


bool SolveSpaceSolver_Storage::isEqual(
    const Slvs_hEntity& thePoint1, const Slvs_hEntity& thePoint2) const
{
  // Precise checking of coincidence: verify that points have equal coordinates
  int aEnt1Pos = Search(thePoint1, myEntities);
  int aEnt2Pos = Search(thePoint2, myEntities);
  if (aEnt1Pos >= 0 && aEnt1Pos < (int)myEntities.size() &&
      aEnt2Pos >= 0 && aEnt2Pos < (int)myEntities.size()) {
    double aDist[2];
    int aParamPos;
    for (int i = 0; i < 2; i++) {
      aParamPos = Search(myEntities[aEnt1Pos].param[i], myParameters);
      aDist[i] = myParameters[aParamPos].val;
      aParamPos = Search(myEntities[aEnt2Pos].param[i], myParameters);
      aDist[i] -= myParameters[aParamPos].val;
    }
    if (aDist[0] * aDist[0] + aDist[1] * aDist[1] < tolerance * tolerance)
      return true;
  }
  return false;
}


std::vector<Slvs_hConstraint> SolveSpaceSolver_Storage::fixEntity(const Slvs_hEntity& theEntity)
{
  std::vector<Slvs_hConstraint> aNewConstraints;

  int aPos = Search(theEntity, myEntities);
  if (aPos >= 0 && aPos < (int)myEntities.size()) {
    switch (myEntities[aPos].type) {
    case SLVS_E_POINT_IN_2D:
    case SLVS_E_POINT_IN_3D:
      fixPoint(myEntities[aPos], aNewConstraints);
      break;
    case SLVS_E_LINE_SEGMENT:
      fixLine(myEntities[aPos], aNewConstraints);
      break;
    case SLVS_E_CIRCLE:
      fixCircle(myEntities[aPos], aNewConstraints);
      break;
    case SLVS_E_ARC_OF_CIRCLE:
      fixArc(myEntities[aPos], aNewConstraints);
      break;
    default:
      break;
    }
  }

  return aNewConstraints;
}

void SolveSpaceSolver_Storage::fixPoint(const Slvs_Entity& thePoint,
    std::vector<Slvs_hConstraint>& theCreated)
{
  Slvs_Constraint aConstraint;
  Slvs_hConstraint aConstrID = SLVS_E_UNKNOWN;
  bool isFixed = isPointFixed(thePoint.h, aConstrID, true);
  bool isForceUpdate = (isFixed /*&& isTemporary(aConstrID)*/);
  if (!isForceUpdate) { // create new constraint
    if (isFixed) return;
    aConstraint = Slvs_MakeConstraint(SLVS_E_UNKNOWN, thePoint.group, SLVS_C_WHERE_DRAGGED, thePoint.wrkpl,
        0.0, thePoint.h, SLVS_E_UNKNOWN, SLVS_E_UNKNOWN, SLVS_E_UNKNOWN);
    aConstraint.h = addConstraint(aConstraint);
    theCreated.push_back(aConstraint.h);
  } else { // update already existent constraint
    if (!isFixed || aConstrID == SLVS_E_UNKNOWN)
      return;
    int aPos = Search(aConstrID, myConstraints);
    if (aPos >= 0 && aPos < (int)myConstraints.size())
      myConstraints[aPos].ptA = thePoint.h;
  }
}

void SolveSpaceSolver_Storage::fixLine(const Slvs_Entity& theLine,
    std::vector<Slvs_hConstraint>& theCreated)
{
  Slvs_Entity aPoint[2] = {
      getEntity(theLine.point[0]),
      getEntity(theLine.point[1])
  };

  Slvs_Constraint anEqual;
  if (isAxisParallel(theLine.h)) {
    // Fix one point and a line length
    Slvs_hConstraint aFixed;
    if (!isPointFixed(theLine.point[0], aFixed, true) &&
        !isPointFixed(theLine.point[1], aFixed, true))
      fixPoint(aPoint[0], theCreated);
    if (!isUsedInEqual(theLine.h, anEqual)) {
      // Check the distance is not set yet
      std::vector<Slvs_Constraint>::const_iterator aDistIt = myConstraints.begin();
      for (; aDistIt != myConstraints.end(); ++aDistIt)
        if ((aDistIt->type == SLVS_C_PT_PT_DISTANCE) &&
           ((aDistIt->ptA == theLine.point[0] && aDistIt->ptB == theLine.point[1]) ||
            (aDistIt->ptA == theLine.point[1] && aDistIt->ptB == theLine.point[0])))
          return;
      // Calculate distance between points on the line
      double aCoords[4];
      for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
          Slvs_Param aParam = getParameter(aPoint[i].param[j]);
          aCoords[2*i+j] = aParam.val;
        }

      double aLength = sqrt((aCoords[2] - aCoords[0]) * (aCoords[2] - aCoords[0]) + 
                            (aCoords[3] - aCoords[1]) * (aCoords[3] - aCoords[1]));
      // fix line length
      Slvs_Constraint aDistance = Slvs_MakeConstraint(SLVS_E_UNKNOWN, theLine.group,
          SLVS_C_PT_PT_DISTANCE, theLine.wrkpl, aLength,
          theLine.point[0], theLine.point[1], SLVS_E_UNKNOWN, SLVS_E_UNKNOWN);
      aDistance.h = addConstraint(aDistance);
      theCreated.push_back(aDistance.h);
    }
    return;
  }
  else if (isUsedInEqual(theLine.h, anEqual)) {
    // Check another entity of Equal is already fixed
    Slvs_hEntity anOtherEntID = anEqual.entityA == theLine.h ? anEqual.entityB : anEqual.entityA;
    if (isEntityFixed(anOtherEntID, true)) {
      // Fix start point of the line (if end point is not fixed yet) ...
      Slvs_hConstraint anEndFixedID = SLVS_E_UNKNOWN;
      bool isFixed = isPointFixed(theLine.point[1], anEndFixedID, true);
      if (isFixed == SLVS_E_UNKNOWN)
        fixPoint(aPoint[0], theCreated);
      // ...  and create fixed point lying on this line
      Slvs_hEntity aPointToCopy = anEndFixedID == SLVS_E_UNKNOWN ? theLine.point[1] : theLine.point[0];
      // Firstly, search already fixed point on line
      bool isPonLineFixed = false;
      Slvs_hEntity aFixedPoint;
      std::vector<Slvs_Constraint>::const_iterator aPLIter = myConstraints.begin();
      for (; aPLIter != myConstraints.end() && !isPonLineFixed; ++aPLIter)
        if (aPLIter->type == SLVS_C_PT_ON_LINE && aPLIter->entityA == theLine.h) {
          isPonLineFixed = isPointFixed(aPLIter->ptA, anEndFixedID);
          aFixedPoint = aPLIter->ptA;
        }

      if (isPonLineFixed) { // update existent constraint
        copyEntity(aPointToCopy, aFixedPoint);
      } else { // create new constraint
        Slvs_hEntity aCopied = copyEntity(aPointToCopy);
        Slvs_Constraint aPonLine = Slvs_MakeConstraint(SLVS_E_UNKNOWN, theLine.group, SLVS_C_PT_ON_LINE,
            theLine.wrkpl, 0.0, aCopied, SLVS_E_UNKNOWN, theLine.h, SLVS_E_UNKNOWN);
        aPonLine.h = addConstraint(aPonLine);
        theCreated.push_back(aPonLine.h);
        fixPoint(getEntity(aCopied), theCreated);
      }
      return;
    }
  }

  // Fix both points
  for (int i = 0; i < 2; i++)
    fixPoint(aPoint[i], theCreated);
}

void SolveSpaceSolver_Storage::fixCircle(const Slvs_Entity& theCircle,
    std::vector<Slvs_hConstraint>& theCreated)
{
  bool isFixRadius = true;
  // Verify the arc is under Equal constraint
  Slvs_Constraint anEqual;
  if (isUsedInEqual(theCircle.h, anEqual)) {
    // Check another entity of Equal is already fixed
    Slvs_hEntity anOtherEntID = anEqual.entityA == theCircle.h ? anEqual.entityB : anEqual.entityA;
    if (isEntityFixed(anOtherEntID, true))
      isFixRadius = false;
  }

  fixPoint(getEntity(theCircle.point[0]), theCreated);

  if (isFixRadius) {
    // Search the radius is already fixed
    std::vector<Slvs_Constraint>::const_iterator aDiamIter = myConstraints.begin();
    for (; aDiamIter != myConstraints.end(); ++aDiamIter)
      if (aDiamIter->type == SLVS_C_DIAMETER && aDiamIter->entityA == theCircle.h)
        return;

    // Fix radius of a circle
    const Slvs_Entity& aRadEnt = getEntity(theCircle.distance);
    double aRadius = getParameter(aRadEnt.param[0]).val;
    Slvs_Constraint aFixedR = Slvs_MakeConstraint(SLVS_E_UNKNOWN, theCircle.group, SLVS_C_DIAMETER,
        theCircle.wrkpl, aRadius * 2.0, SLVS_E_UNKNOWN, SLVS_E_UNKNOWN, theCircle.h, SLVS_E_UNKNOWN);
    aFixedR.h = addConstraint(aFixedR);
    theCreated.push_back(aFixedR.h);
  }
}

void SolveSpaceSolver_Storage::fixArc(const Slvs_Entity& theArc,
    std::vector<Slvs_hConstraint>& theCreated)
{
  Slvs_Entity aPoint[3] = {
      getEntity(theArc.point[0]),
      getEntity(theArc.point[1]),
      getEntity(theArc.point[2])
  };

  bool isFixRadius = true;
  std::list<Slvs_Entity> aPointsToFix;
  aPointsToFix.push_back(aPoint[1]);
  aPointsToFix.push_back(aPoint[2]);

  // Verify the arc is under Equal constraint
  Slvs_Constraint anEqual;
  if (isUsedInEqual(theArc.h, anEqual)) {
    // Check another entity of Equal is already fixed
    Slvs_hEntity anOtherEntID = anEqual.entityA == theArc.h ? anEqual.entityB : anEqual.entityA;
    if (isEntityFixed(anOtherEntID, true)) {
      isFixRadius = false;
      Slvs_Entity anOtherEntity = getEntity(anOtherEntID);
      if (anOtherEntity.type == SLVS_E_LINE_SEGMENT) {
        aPointsToFix.pop_back();
        aPointsToFix.push_back(aPoint[0]);
      }
    }
  }

  Slvs_hConstraint aConstrID;
  int aNbPointsToFix = 2; // number of fixed points for the arc
  if (isPointFixed(theArc.point[0], aConstrID, true))
    aNbPointsToFix--;

  double anArcPoints[3][2];
  for (int i = 0; i < 3; i++) {
    const Slvs_Entity& aPointOnArc = getEntity(theArc.point[i]);
    for (int j = 0; j < 2; j++)
      anArcPoints[i][j] = getParameter(aPointOnArc.param[j]).val;
  }

  // Radius of the arc
  std::shared_ptr<GeomAPI_Pnt2d> aCenter(new GeomAPI_Pnt2d(anArcPoints[0][0], anArcPoints[0][1]));
  std::shared_ptr<GeomAPI_Pnt2d> aStart(new GeomAPI_Pnt2d(anArcPoints[1][0], anArcPoints[1][1]));
  double aRadius = aCenter->distance(aStart);

  // Update end point of the arc to be on a curve
  std::shared_ptr<GeomAPI_Pnt2d> anEnd(new GeomAPI_Pnt2d(anArcPoints[2][0], anArcPoints[2][1]));
  double aDistance = anEnd->distance(aCenter);
  std::shared_ptr<GeomAPI_XY> aDir = anEnd->xy()->decreased(aCenter->xy());
  if (aDistance < tolerance)
    aDir = aStart->xy()->decreased(aCenter->xy())->multiplied(-1.0);
  else
    aDir = aDir->multiplied(aRadius / aDistance);
  double xy[2] = {aCenter->x() + aDir->x(), aCenter->y() + aDir->y()};
  const Slvs_Entity& aEndPoint = getEntity(theArc.point[2]);
  for (int i = 0; i < 2; i++) {
    Slvs_Param aParam = getParameter(aEndPoint.param[i]);
    aParam.val = xy[i];
    updateParameter(aParam);
  }

  std::list<Slvs_Entity>::iterator aPtIt = aPointsToFix.begin();
  for (; aNbPointsToFix > 0; aPtIt++, aNbPointsToFix--)
    fixPoint(*aPtIt, theCreated);

  if (isFixRadius) {
    // Fix radius of the arc
    bool isExists = false;
    std::vector<Slvs_Constraint>::iterator anIt = myConstraints.begin();
    for (; anIt != myConstraints.end() && !isExists; ++anIt)
      if (anIt->type == SLVS_C_DIAMETER && anIt->entityA == theArc.h)
        isExists = true;
    if (!isExists) {
      Slvs_Constraint aFixedR = Slvs_MakeConstraint(SLVS_E_UNKNOWN, theArc.group, SLVS_C_DIAMETER,
          theArc.wrkpl, aRadius * 2.0, SLVS_E_UNKNOWN, SLVS_E_UNKNOWN, theArc.h, SLVS_E_UNKNOWN);
      aFixedR.h = addConstraint(aFixedR);
      theCreated.push_back(aFixedR.h);
    }
  }
}


bool SolveSpaceSolver_Storage::isAxisParallel(const Slvs_hEntity& theEntity) const
{
  std::vector<Slvs_Constraint>::const_iterator anIter = myConstraints.begin();
  for (; anIter != myConstraints.end(); anIter++)
    if ((anIter->type == SLVS_C_HORIZONTAL || anIter->type == SLVS_C_VERTICAL) && 
        anIter->entityA == theEntity)
      return true;
  return false;
}

bool SolveSpaceSolver_Storage::isUsedInEqual(
    const Slvs_hEntity& theEntity, Slvs_Constraint& theEqual) const
{
  // Check the entity is used in Equal constraint
  std::vector<Slvs_Constraint>::const_iterator anEqIter = myConstraints.begin();
  for (; anEqIter != myConstraints.end(); anEqIter++)
    if ((anEqIter->type == SLVS_C_EQUAL_LENGTH_LINES ||
         anEqIter->type == SLVS_C_EQUAL_LINE_ARC_LEN ||
         anEqIter->type == SLVS_C_EQUAL_RADIUS) &&
       (anEqIter->entityA == theEntity || anEqIter->entityB == theEntity)) {
      theEqual = *anEqIter;
      return true;
    }
  return false;
}


bool SolveSpaceSolver_Storage::removeCoincidence(ConstraintWrapperPtr theConstraint)
{
  std::list<EntityWrapperPtr> aPoints = theConstraint->entities();
  std::list<EntityWrapperPtr>::const_iterator aPIt;

  CoincidentPointsMap::iterator aPtPtIt = myCoincidentPoints.begin();
  for (; aPtPtIt != myCoincidentPoints.end(); ++aPtPtIt) {
    for (aPIt = aPoints.begin(); aPIt != aPoints.end(); ++aPIt)
      if (aPtPtIt->first == *aPIt ||
          aPtPtIt->second.find(*aPIt) != aPtPtIt->second.end())
        break;
    if (aPIt != aPoints.end())
      break;
  }

  if (aPtPtIt == myCoincidentPoints.end())
    return true; // already removed

  // Create new copies of coincident points
  BuilderPtr aBuilder = SolveSpaceSolver_Builder::getInstance();
  std::list<EntityWrapperPtr> aNewPoints;
  for (aPIt = aPoints.begin(); aPIt != aPoints.end(); ++aPIt)
    aNewPoints.push_back(aBuilder->createAttribute(
        (*aPIt)->baseAttribute(), myGroupID, myWorkplaneID));

  // Find all points fallen out of group of coincident points
  std::map<EntityWrapperPtr, EntityWrapperPtr> aNotCoinc;
  aNotCoinc[aPtPtIt->first] = EntityWrapperPtr();
  std::set<EntityWrapperPtr>::const_iterator aTempIt = aPtPtIt->second.begin();
  for (; aTempIt != aPtPtIt->second.end(); ++aTempIt)
    aNotCoinc[*aTempIt] = EntityWrapperPtr();
  std::map<ConstraintPtr, std::list<ConstraintWrapperPtr> >::iterator
      aConstrIt = myConstraintMap.begin();
  for (; aConstrIt != myConstraintMap.end(); ++aConstrIt)
    if (aConstrIt->first->getKind() == SketchPlugin_ConstraintCoincidence::ID()) {
      AttributeRefAttrPtr aRefAttrA = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          aConstrIt->first->attribute(SketchPlugin_Constraint::ENTITY_A()));
      AttributeRefAttrPtr aRefAttrB = std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(
          aConstrIt->first->attribute(SketchPlugin_Constraint::ENTITY_B()));
      if (!aRefAttrA || !aRefAttrB || aRefAttrA->isObject() || aRefAttrB->isObject())
        continue;
      std::map<AttributePtr, EntityWrapperPtr>::iterator
          aFound = myAttributeMap.find(aRefAttrA->attr());
      if (aFound != myAttributeMap.end())
        aNotCoinc.erase(aFound->second);
      aFound = myAttributeMap.find(aRefAttrB->attr());
      if (aFound != myAttributeMap.end())
        aNotCoinc.erase(aFound->second);
    }
  if (aNotCoinc.empty())
    return false;
  std::list<EntityWrapperPtr>::const_iterator aNewPIt;
  for (aPIt = aPoints.begin(), aNewPIt = aNewPoints.begin();
       aPIt != aPoints.end(); ++aPIt, ++aNewPIt) {
    if (aNotCoinc.find(*aPIt) != aNotCoinc.end())
      aNotCoinc[*aPIt] = *aNewPIt;
  }

  // Find all features and constraints uses coincident points
  std::map<EntityWrapperPtr, EntityWrapperPtr>::iterator aNotCIt;
  std::set<EntityWrapperPtr> anUpdFeatures;
  std::map<FeaturePtr, EntityWrapperPtr>::iterator aFIt = myFeatureMap.begin();
  for (; aFIt != myFeatureMap.end(); ++aFIt) {
    for (aNotCIt = aNotCoinc.begin(); aNotCIt != aNotCoinc.end(); ++aNotCIt) {
      if (!aFIt->second->isUsed(aNotCIt->first->baseAttribute()))
        continue;
      std::list<EntityWrapperPtr> aSubs = aFIt->second->subEntities();
      std::list<EntityWrapperPtr>::iterator aSIt = aSubs.begin();
      for (; aSIt != aSubs.end(); ++aSIt)
        if (*aSIt == aNotCIt->first)
          *aSIt = aNotCIt->second;
      aFIt->second->setSubEntities(aSubs);
      anUpdFeatures.insert(aFIt->second);
    }
  }
  // update features
  std::set<EntityWrapperPtr>::iterator anUpdIt = anUpdFeatures.begin();
  for (; anUpdIt != anUpdFeatures.end(); ++anUpdIt)
    update(EntityWrapperPtr(*anUpdIt));

  // remove not coincident points
  for (aNotCIt = aNotCoinc.begin(); aNotCIt != aNotCoinc.end(); ++aNotCIt) {
    if (aPtPtIt->second.size() <= 1) {
      myCoincidentPoints.erase(aPtPtIt);
      break;
    }
    if (aPtPtIt->first == aNotCIt->first) {
      std::set<EntityWrapperPtr> aSlaves = aPtPtIt->second;
      EntityWrapperPtr aNewMaster = *aSlaves.begin();
      aSlaves.erase(aSlaves.begin());
      myCoincidentPoints.erase(aPtPtIt);
      myCoincidentPoints[aNewMaster] = aSlaves;
      aPtPtIt = myCoincidentPoints.find(aNewMaster);
    } else
      aPtPtIt->second.erase(aNotCIt->first);
  }
  return true;
}

bool SolveSpaceSolver_Storage::remove(ConstraintWrapperPtr theConstraint)
{
  std::shared_ptr<SolveSpaceSolver_ConstraintWrapper> aConstraint =
      std::dynamic_pointer_cast<SolveSpaceSolver_ConstraintWrapper>(theConstraint);

  // verify whether the constraint has duplicated
  SameConstraintMap::iterator anEqIt = myEqualConstraints.begin();
  for (; anEqIt != myEqualConstraints.end(); ++anEqIt)
    if (anEqIt->find(aConstraint) != anEqIt->end()) {
      anEqIt->erase(aConstraint);
      break;
    }
  if (anEqIt != myEqualConstraints.end())
    return true;

  bool isFullyRemoved = removeConstraint((Slvs_hConstraint)aConstraint->id());

  // remove point-point coincidence
  if (aConstraint->type() == CONSTRAINT_PT_PT_COINCIDENT)
    isFullyRemoved = removeCoincidence(theConstraint);

  return SketchSolver_Storage::remove(theConstraint) && isFullyRemoved;
}

bool SolveSpaceSolver_Storage::remove(EntityWrapperPtr theEntity)
{
  if (!theEntity)
    return false;

  std::shared_ptr<SolveSpaceSolver_EntityWrapper> anEntity = 
        std::dynamic_pointer_cast<SolveSpaceSolver_EntityWrapper>(theEntity);
  bool isFullyRemoved = removeEntity((Slvs_hEntity)anEntity->id());
  return SketchSolver_Storage::remove(theEntity) && isFullyRemoved;
}

bool SolveSpaceSolver_Storage::remove(ParameterWrapperPtr theParameter)
{
  return removeParameter((Slvs_hParam)theParameter->id());
}


void SolveSpaceSolver_Storage::refresh(bool theFixedOnly) const
{
  //blockEvents(true);

  std::map<AttributePtr, EntityWrapperPtr>::const_iterator anIt = myAttributeMap.begin();
  std::list<ParameterWrapperPtr> aParams;
  std::list<ParameterWrapperPtr>::const_iterator aParIt;
  for (; anIt != myAttributeMap.end(); ++anIt) {
    if (!anIt->second)
      continue;
    // the external feature always should keep the up to date values, so, 
    // refresh from the solver is never needed
    if (anIt->first.get()) {
      std::shared_ptr<SketchPlugin_Feature> aSketchFeature = 
        std::dynamic_pointer_cast<SketchPlugin_Feature>(anIt->first->owner());
      if (aSketchFeature.get() && aSketchFeature->isExternal())
        continue;
    }

    // update parameter wrappers and obtain values of attributes
    aParams = anIt->second->parameters();
    double aCoords[3];
    bool isUpd[3] = {false};
    int i = 0;
    for (aParIt = aParams.begin(); i < 3 && aParIt != aParams.end(); ++aParIt, ++i) {
      std::shared_ptr<SolveSpaceSolver_ParameterWrapper> aWrapper = 
          std::dynamic_pointer_cast<SolveSpaceSolver_ParameterWrapper>(*aParIt);
      if (!theFixedOnly || aWrapper->group() == GID_OUTOFGROUP || aWrapper->isParametric()) {
        aWrapper->changeParameter().val = getParameter((Slvs_hParam)aWrapper->id()).val;
        aCoords[i] = aWrapper->value();
        isUpd[i] = true;
      }
    }
    if (!isUpd[0] && !isUpd[1] && !isUpd[2])
      continue; // nothing is updated

    std::shared_ptr<GeomDataAPI_Point2D> aPoint2D =
        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(anIt->first);
    if (aPoint2D) {
      if ((isUpd[0] && fabs(aPoint2D->x() - aCoords[0]) > tolerance) ||
          (isUpd[1] && fabs(aPoint2D->y() - aCoords[1]) > tolerance)) {
        if (!isUpd[0]) aCoords[0] = aPoint2D->x();
        if (!isUpd[1]) aCoords[1] = aPoint2D->y();
        aPoint2D->setValue(aCoords[0], aCoords[1]);
        // Find points coincident with this one (probably not in GID_OUTOFGROUP)
        std::map<AttributePtr, EntityWrapperPtr>::const_iterator aLocIt =
            theFixedOnly ? myAttributeMap.begin() : anIt;
        for (++aLocIt; aLocIt != myAttributeMap.end(); ++aLocIt)
          if (anIt->second->id() == aLocIt->second->id()) {
            aPoint2D = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(aLocIt->first);
            aPoint2D->setValue(aCoords[0], aCoords[1]);
          }
      }
      continue;
    }
    AttributeDoublePtr aScalar = std::dynamic_pointer_cast<ModelAPI_AttributeDouble>(anIt->first);
    if (aScalar) {
      if (isUpd[0] && fabs(aScalar->value() - aCoords[0]) > tolerance)
        aScalar->setValue(aCoords[0]);
      continue;
    }
    std::shared_ptr<GeomDataAPI_Point> aPoint =
        std::dynamic_pointer_cast<GeomDataAPI_Point>(anIt->first);
    if (aPoint) {
      if ((isUpd[0] && fabs(aPoint->x() - aCoords[0]) > tolerance) ||
          (isUpd[1] && fabs(aPoint->y() - aCoords[1]) > tolerance) ||
          (isUpd[2] && fabs(aPoint->z() - aCoords[2]) > tolerance))
        if (!isUpd[0]) aCoords[0] = aPoint->x();
        if (!isUpd[1]) aCoords[1] = aPoint->y();
        if (!isUpd[2]) aCoords[2] = aPoint->z();
        aPoint->setValue(aCoords[0], aCoords[1], aCoords[2]);
      continue;
    }
  }

  //blockEvents(false);
}

void SolveSpaceSolver_Storage::verifyFixed()
{
  std::map<AttributePtr, EntityWrapperPtr>::iterator anAttrIt = myAttributeMap.begin();
  for (; anAttrIt != myAttributeMap.end(); ++anAttrIt) {
    if (!anAttrIt->second)
      continue;
    const std::list<ParameterWrapperPtr>& aParameters = anAttrIt->second->parameters();
    std::list<ParameterWrapperPtr>::const_iterator aParIt = aParameters.begin();
    for (; aParIt != aParameters.end(); ++aParIt)
      if ((*aParIt)->group() == GID_OUTOFGROUP) {
        Slvs_Param aParam = getParameter((Slvs_hParam)(*aParIt)->id());
        if (aParam.group != (Slvs_hParam)GID_OUTOFGROUP) {
          aParam.group = (Slvs_hParam)GID_OUTOFGROUP;
          updateParameter(aParam);
        }
      }
  }
}


void SolveSpaceSolver_Storage::adjustArc(const Slvs_Entity& theArc)
{
  double anArcPoints[3][2];
  double aDist[3] = {0.0};
  bool isFixed[3] = {false};
  for (int i = 0; i < 3; ++i) {
    Slvs_Entity aPoint = getEntity(theArc.point[i]);
    isFixed[i] = (aPoint.group != (Slvs_hGroup)myGroupID);
    anArcPoints[i][0] = getParameter(aPoint.param[0]).val;
    anArcPoints[i][1] = getParameter(aPoint.param[1]).val;
    if (i > 0) {
      anArcPoints[i][0] -= anArcPoints[0][0];
      anArcPoints[i][1] -= anArcPoints[0][1];
      aDist[i] = sqrt(anArcPoints[i][0] * anArcPoints[i][0] + 
                      anArcPoints[i][1] * anArcPoints[i][1]);
    }
  }

  if (fabs(aDist[1] - aDist[2]) < tolerance)
    return;

  int anInd = 2;
  while (anInd > 0 && isFixed[anInd])
    --anInd;
  if (anInd < 1)
    return; // adjust only start or end point of the arc

  anArcPoints[anInd][0] /= aDist[anInd];
  anArcPoints[anInd][1] /= aDist[anInd];

  Slvs_Entity aPoint = getEntity(theArc.point[anInd]);
  for (int i = 0; i < 2; ++i) {
    Slvs_Param aParam = getParameter(aPoint.param[i]);
    aParam.val = anArcPoints[0][i] + aDist[3-anInd] * anArcPoints[anInd][i];
    updateParameter(aParam);
  }
}







// ========================================================
// =========      Auxiliary functions       ===============
// ========================================================

template<typename T>
int Search(const uint32_t& theEntityID, const std::vector<T>& theEntities)
{
  int aResIndex = theEntityID <= theEntities.size() ? theEntityID - 1 : 0;
  int aVecSize = theEntities.size();
  if (theEntities.empty())
    return 1;
  while (aResIndex >= 0 && theEntities[aResIndex].h > theEntityID)
    aResIndex--;
  while (aResIndex < aVecSize && aResIndex >= 0 && theEntities[aResIndex].h < theEntityID)
    aResIndex++;
  if (aResIndex == -1 || (aResIndex < aVecSize && theEntities[aResIndex].h != theEntityID))
    aResIndex = aVecSize;
  return aResIndex;
}

bool IsNotEqual(const Slvs_Param& theParam1, const Slvs_Param& theParam2)
{
  return fabs(theParam1.val - theParam2.val) > tolerance;
}

bool IsNotEqual(const Slvs_Entity& theEntity1, const Slvs_Entity& theEntity2)
{
  int i = 0;
  for (; theEntity1.param[i] != 0 && i < 4; i++)
    if (theEntity1.param[i] != theEntity2.param[i])
      return true;
  i = 0;
  for (; theEntity1.point[i] != 0 && i < 4; i++)
    if (theEntity1.point[i] != theEntity2.point[i])
      return true;
  return false;
}

bool IsNotEqual(const Slvs_Constraint& theConstraint1, const Slvs_Constraint& theConstraint2)
{
  return theConstraint1.ptA != theConstraint2.ptA ||
         theConstraint1.ptB != theConstraint2.ptB ||
         theConstraint1.entityA != theConstraint2.entityA ||
         theConstraint1.entityB != theConstraint2.entityB ||
         theConstraint1.entityC != theConstraint2.entityC ||
         theConstraint1.entityD != theConstraint2.entityD ||
         fabs(theConstraint1.valA - theConstraint2.valA) > tolerance;
}
