#include <SketchSolver_ConstraintParametric.h>
#include <SketchSolver_Error.h>
#include <SketchSolver_Group.h>

#include <GeomDataAPI_Point2D.h>

SketchSolver_ConstraintParametric::SketchSolver_ConstraintParametric(AttributePtr theAttribute)
  : SketchSolver_ConstraintRigid(ConstraintPtr()),
    myBaseAttribute(theAttribute)
{
  process();
}

void SketchSolver_ConstraintParametric::process()
{
  cleanErrorMsg();
  if (!myBaseAttribute || !myStorage || myGroup == 0) {
    /// TODO: Put error message here
    return;
  }
  if (!mySlvsConstraints.empty()) // some data is changed, update constraint
    update(myBaseConstraint);

  Slvs_hEntity anAttrID;
  getAttributes(anAttrID);
  if (!myErrorMsg.empty() || (myFeatureMap.empty() && myAttributeMap.empty()))
    return;

  myHorizLineID = SLVS_E_UNKNOWN;
  myVertLineID = SLVS_E_UNKNOWN;

  std::shared_ptr<GeomDataAPI_Point2D> aPoint =
      std::dynamic_pointer_cast<GeomDataAPI_Point2D>(myBaseAttribute);
  if (!aPoint)
    return;
  if (!aPoint->textX().empty()) {
    // Create vertical line with fixed boundary point
    Slvs_Entity aLine = createLine(aPoint->x(), -100.0, aPoint->x(), 100.0);
    // Place point on line
    Slvs_Constraint aConstraint = Slvs_MakeConstraint(SLVS_C_UNKNOWN, myGroup->getId(),
        SLVS_C_PT_ON_LINE, myGroup->getWorkplaneId(), 0.0, anAttrID, SLVS_E_UNKNOWN,
        aLine.h, SLVS_E_UNKNOWN);
    aConstraint.h = myStorage->addConstraint(aConstraint);
    mySlvsConstraints.push_back(aConstraint.h);
    myVertLineID = aLine.h;
    myX = aPoint->x();
  }
  if (!aPoint->textY().empty()) {
    // Create horizontal line with fixed boundary points
    Slvs_Entity aLine = createLine(-100.0, aPoint->y(), 100.0, aPoint->y());
    // Place point on line
    Slvs_Constraint aConstraint = Slvs_MakeConstraint(SLVS_C_UNKNOWN, myGroup->getId(),
        SLVS_C_PT_ON_LINE, myGroup->getWorkplaneId(), 0.0, anAttrID, SLVS_E_UNKNOWN,
        aLine.h, SLVS_E_UNKNOWN);
    aConstraint.h = myStorage->addConstraint(aConstraint);
    mySlvsConstraints.push_back(aConstraint.h);
    myHorizLineID = aLine.h;
    myY = aPoint->y();
  }
}


void SketchSolver_ConstraintParametric::getAttributes(Slvs_hEntity& theAttributeID)
{
  int aType = SLVS_E_UNKNOWN; // type of created entity
  theAttributeID = SLVS_E_UNKNOWN;
  theAttributeID = myGroup->getAttributeId(myBaseAttribute);
  if (theAttributeID == SLVS_E_UNKNOWN) {
    theAttributeID = changeEntity(myBaseAttribute, aType);
    if (theAttributeID == SLVS_E_UNKNOWN) {
      myErrorMsg = SketchSolver_Error::NOT_INITIALIZED();
      return;
    }
  }
  else
     myAttributeMap[myBaseAttribute] = theAttributeID;
}


void SketchSolver_ConstraintParametric::update(ConstraintPtr theConstraint)
{
  cleanErrorMsg();
  if (!theConstraint || theConstraint == myBaseConstraint) {
    std::shared_ptr<GeomDataAPI_Point2D> aPoint =
        std::dynamic_pointer_cast<GeomDataAPI_Point2D>(myBaseAttribute);
    if (aPoint && ((!aPoint->textX().empty() && myVertLineID == SLVS_E_UNKNOWN) || 
        (!aPoint->textY().empty() && myHorizLineID == SLVS_E_UNKNOWN))) {
      remove();
      process();
      return;
    }
  }
  adjustConstraint();
}

void SketchSolver_ConstraintParametric::refresh()
{
  Slvs_hEntity aBasePointID = myAttributeMap[myBaseAttribute];
  const Slvs_Entity& aBasePoint = myStorage->getEntity(aBasePointID);
  double aXY[2];
  aXY[0] = myVertLineID  != SLVS_E_UNKNOWN ? myX : myStorage->getParameter(aBasePoint.param[0]).val;
  aXY[1] = myHorizLineID != SLVS_E_UNKNOWN ? myY : myStorage->getParameter(aBasePoint.param[1]).val;

  std::list<Slvs_Constraint> aCoincidence = myStorage->getConstraintsByType(SLVS_C_POINTS_COINCIDENT);
  std::list<Slvs_Constraint>::const_iterator aCIt = aCoincidence.begin();
  for (; aCIt != aCoincidence.end(); ++aCIt) {
    if (aCIt->ptA != aBasePointID && aCIt->ptB != aBasePointID)
      continue;
    Slvs_hEntity anOtherPointID = aCIt->ptA == aBasePointID ? aCIt->ptB : aCIt->ptA;
    const Slvs_Entity& aPoint = myStorage->getEntity(anOtherPointID);
    for (int i = 0; i < 2; i++) {
      Slvs_Param aParam = myStorage->getParameter(aPoint.param[i]);
      aParam.val = aXY[i];
      myStorage->updateParameter(aParam);
    }
  }
}

void SketchSolver_ConstraintParametric::adjustConstraint()
{
  std::shared_ptr<GeomDataAPI_Point2D> aPoint =
      std::dynamic_pointer_cast<GeomDataAPI_Point2D>(myBaseAttribute);
  if (!aPoint)
    return;

  if (!aPoint->textX().empty()) {
    const Slvs_Entity& aLine = myStorage->getEntity(myVertLineID);
    const Slvs_Entity& aStartPoint = myStorage->getEntity(aLine.point[0]);
    Slvs_Param aParX = myStorage->getParameter(aStartPoint.param[0]);
    aParX.val = aPoint->x();
    myStorage->updateParameter(aParX);
    myX = aParX.val;
  }
  if (!aPoint->textY().empty()) {
    const Slvs_Entity& aLine = myStorage->getEntity(myHorizLineID);
    const Slvs_Entity& aStartPoint = myStorage->getEntity(aLine.point[0]);
    Slvs_Param aParY = myStorage->getParameter(aStartPoint.param[1]);
    aParY.val = aPoint->y();
    myStorage->updateParameter(aParY);
    myY = aParY.val;
  }
}


Slvs_Entity SketchSolver_ConstraintParametric::createLine(
    double theStartX, double theStartY, double theEndX, double theEndY)
{
  // Start point
  Slvs_Param aParX = Slvs_MakeParam(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP, theStartX);
  Slvs_Param aParY = Slvs_MakeParam(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP, theStartY);
  aParX.h = myStorage->addParameter(aParX);
  aParY.h = myStorage->addParameter(aParY);
  Slvs_Entity aStartPoint = Slvs_MakePoint2d(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP,
    myGroup->getWorkplaneId(), aParX.h, aParY.h);
  aStartPoint.h = myStorage->addEntity(aStartPoint);

  // End point
  aParX = Slvs_MakeParam(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP, theEndX);
  aParY = Slvs_MakeParam(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP, theEndY);
  aParX.h = myStorage->addParameter(aParX);
  aParY.h = myStorage->addParameter(aParY);
  Slvs_Entity aEndPoint = Slvs_MakePoint2d(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP,
    myGroup->getWorkplaneId(), aParX.h, aParY.h);
  aEndPoint.h = myStorage->addEntity(aEndPoint);

  // Line
  Slvs_Entity aLine = Slvs_MakeLineSegment(SLVS_E_UNKNOWN, SLVS_G_OUTOFGROUP,
    myGroup->getWorkplaneId(), aStartPoint.h, aEndPoint.h);
  aLine.h = myStorage->addEntity(aLine);
  return aLine;
}
