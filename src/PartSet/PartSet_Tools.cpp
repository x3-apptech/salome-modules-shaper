// File:        PartSet_Tools.h
// Created:     28 Apr 2014
// Author:      Natalia ERMOLAEVA

#include <PartSet_Tools.h>

#include <ModelAPI_Data.h>
#include <ModelAPI_AttributeDouble.h>
#include <ModelAPI_Document.h>

#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Dir.h>
#include <GeomDataAPI_Point2D.h>

#include <GeomAPI_Dir.h>
#include <GeomAPI_XYZ.h>

#include <SketchPlugin_Feature.h>
#include <SketchPlugin_Sketch.h>
#include <SketchPlugin_Point.h>
#include <SketchPlugin_Line.h>
#include <SketchPlugin_ConstraintCoincidence.h>
#include <SketchPlugin_Constraint.h>

#include <XGUI_ViewerPrs.h>

#include <V3d_View.hxx>
#include <gp_Pln.hxx>
#include <ProjLib.hxx>
#include <ElSLib.hxx>
#include <Geom_Line.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>

#ifdef _DEBUG
#include <QDebug>
#endif

const double PRECISION_TOLERANCE = 0.000001;

gp_Pnt PartSet_Tools::convertClickToPoint(QPoint thePoint, Handle(V3d_View) theView)
{
  if (theView.IsNull())
    return gp_Pnt();

  V3d_Coordinate XEye, YEye, ZEye, XAt, YAt, ZAt;
  theView->Eye(XEye, YEye, ZEye);

  theView->At(XAt, YAt, ZAt);
  gp_Pnt EyePoint(XEye, YEye, ZEye);
  gp_Pnt AtPoint(XAt, YAt, ZAt);

  gp_Vec EyeVector(EyePoint, AtPoint);
  gp_Dir EyeDir(EyeVector);

  gp_Pln PlaneOfTheView = gp_Pln(AtPoint, EyeDir);
  Standard_Real X, Y, Z;
  theView->Convert(thePoint.x(), thePoint.y(), X, Y, Z);
  gp_Pnt ConvertedPoint(X, Y, Z);

  gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView, ConvertedPoint);
  gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(), ConvertedPointOnPlane.Y(), PlaneOfTheView);
  return ResultPoint;
}

void PartSet_Tools::convertTo2D(const gp_Pnt& thePoint, FeaturePtr theSketch,
                                Handle(V3d_View) theView, double& theX, double& theY)
{
  if (!theSketch)
    return;

  boost::shared_ptr<ModelAPI_AttributeDouble> anAttr;
  boost::shared_ptr<ModelAPI_Data> aData = theSketch->data();

  boost::shared_ptr<GeomDataAPI_Point> anOrigin = 
    boost::dynamic_pointer_cast<GeomDataAPI_Point>(aData->attribute(SKETCH_ATTR_ORIGIN));

  boost::shared_ptr<GeomDataAPI_Dir> aX = 
    boost::dynamic_pointer_cast<GeomDataAPI_Dir>(aData->attribute(SKETCH_ATTR_DIRX));
  boost::shared_ptr<GeomDataAPI_Dir> anY = 
    boost::dynamic_pointer_cast<GeomDataAPI_Dir>(aData->attribute(SKETCH_ATTR_DIRY));

  gp_Pnt anOriginPnt(anOrigin->x(), anOrigin->y(), anOrigin->z());
  gp_Vec aVec(anOriginPnt, thePoint);

  if (!theView.IsNull())
  {
    V3d_Coordinate XEye, YEye, ZEye, XAt, YAt, ZAt;
    theView->Eye(XEye, YEye, ZEye);

    theView->At(XAt, YAt, ZAt);
    gp_Pnt EyePoint(XEye, YEye, ZEye);
    gp_Pnt AtPoint(XAt, YAt, ZAt);

    gp_Vec anEyeVec(EyePoint, AtPoint);
    anEyeVec.Normalize();

    boost::shared_ptr<GeomDataAPI_Dir> aNormal = 
                  boost::dynamic_pointer_cast<GeomDataAPI_Dir>(aData->attribute(SKETCH_ATTR_NORM));
    gp_Vec aNormalVec(aNormal->x(), aNormal->y(), aNormal->z());

    double aDen = anEyeVec * aNormalVec;
    double aLVec = aDen != 0 ? aVec * aNormalVec / aDen : DBL_MAX;

    gp_Vec aDeltaVec = anEyeVec*aLVec;
    aVec = aVec - aDeltaVec;
  }
  theX = aVec.X() * aX->x() + aVec.Y() * aX->y() + aVec.Z() * aX->z();
  theY = aVec.X() * anY->x() + aVec.Y() * anY->y() + aVec.Z() * anY->z();
}

void PartSet_Tools::convertTo3D(const double theX, const double theY,
                                FeaturePtr theSketch,
                                gp_Pnt& thePoint)
{
  if (!theSketch)
    return;

  boost::shared_ptr<ModelAPI_Data> aData = theSketch->data();

  boost::shared_ptr<GeomDataAPI_Point> aC = 
    boost::dynamic_pointer_cast<GeomDataAPI_Point>(aData->attribute(SKETCH_ATTR_ORIGIN));
  boost::shared_ptr<GeomDataAPI_Dir> aX = 
    boost::dynamic_pointer_cast<GeomDataAPI_Dir>(aData->attribute(SKETCH_ATTR_DIRX));
  boost::shared_ptr<GeomDataAPI_Dir> aY = 
    boost::dynamic_pointer_cast<GeomDataAPI_Dir>(aData->attribute(SKETCH_ATTR_DIRY));

  boost::shared_ptr<GeomAPI_XYZ> aSum = aC->pnt()->xyz()->added(
    aX->dir()->xyz()->multiplied(theX))->added(aY->dir()->xyz()->multiplied(theY));

  boost::shared_ptr<GeomAPI_Pnt> aPoint = boost::shared_ptr<GeomAPI_Pnt>(new GeomAPI_Pnt(aSum));
  thePoint = gp_Pnt(aPoint->x(), aPoint->y(), aPoint->z());
}

void PartSet_Tools::intersectLines(double theX0, double theY0, double theX1, double theY1,
                                   double theX2, double theY2, double theX3, double theY3,
                                   double& theX, double& theY)
{
  double aV1 = theX1 - theX0, aV2 = theY1 - theY0;
  double aW1 = theX3 - theX2, aW2 = theY3 - theY2;

  double aT2 = 0;
  if (aV1  != 0 && aV2 != 0)
    aT2 = (( theY2 - theY0 )/aV2 - ( theX2 - theX0 )/aV1) / ( aW1/aV1 - aW2/aV2 );
  else
    aT2 = DBL_MAX;

  theX  = theX2 + aT2*aW1;
  theY = theY2 + aT2*aW2;

  // the coordinates of two lines are on the common line
  //It is not possible to use Precision::Confusion(), because it is e-0.8, but V is sometimes e-6
  Standard_Real aPrec = PRECISION_TOLERANCE;
  if (fabs(theX - theX0) < aPrec && fabs(theY - theY0) < aPrec) {
    projectPointOnLine(theX2, theY2, theX3, theY3, theX1, theY1, theX, theY);    
  }
}

void PartSet_Tools::projectPointOnLine(double theX1, double theY1, double theX2, double theY2,
                                       double thePointX, double thePointY, double& theX, double& theY)
{
  theX = theY = 0;

  Handle(Geom_Line) aLine = new Geom_Line(gp_Pnt(theX1, theY1, 0),
                                     gp_Dir(gp_Vec(gp_Pnt(theX1, theY1, 0), gp_Pnt(theX2, theY2, 0))));
  GeomAPI_ProjectPointOnCurve aProj(gp_Pnt(thePointX, thePointY, 0), aLine);

  Standard_Integer aNbPoint = aProj.NbPoints();
  if (aNbPoint > 0) {
    gp_Pnt aPoint = aProj.Point(1);
    theX = aPoint.X();
    theY = aPoint.Y();
  }
}

FeaturePtr PartSet_Tools::nearestFeature(QPoint thePoint,
                                                   Handle_V3d_View theView,
                                                   FeaturePtr theSketch,
                                                   const std::list<XGUI_ViewerPrs>& theFeatures)
{
  double aX, anY;
  gp_Pnt aPoint = PartSet_Tools::convertClickToPoint(thePoint, theView);
  PartSet_Tools::convertTo2D(aPoint, theSketch, theView, aX, anY);

  FeaturePtr aFeature;
  std::list<XGUI_ViewerPrs>::const_iterator anIt = theFeatures.begin(), aLast = theFeatures.end();

  FeaturePtr aDeltaFeature;   
  double aMinDelta = -1;
  XGUI_ViewerPrs aPrs;
  for (; anIt != aLast; anIt++) {
    aPrs = *anIt;
    if (!aPrs.feature())
      continue;
    double aDelta = distanceToPoint(aPrs.feature(), aX, anY);
    if (aMinDelta < 0 || aMinDelta > aDelta) {
      aMinDelta = aDelta;
      aDeltaFeature = aPrs.feature();
    }
  }
  return aDeltaFeature;
}

double PartSet_Tools::distanceToPoint(FeaturePtr theFeature,
                                      double theX, double theY)
{
  double aDelta = 0;
  if (theFeature->getKind() != SKETCH_LINE_KIND)
    return aDelta;

  boost::shared_ptr<ModelAPI_Data> aData = theFeature->data();

  boost::shared_ptr<GeomDataAPI_Point2D> aPoint1 =
        boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(LINE_ATTR_START));
  boost::shared_ptr<GeomDataAPI_Point2D> aPoint2 =
        boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(LINE_ATTR_END));

  double aX, anY;
  PartSet_Tools::projectPointOnLine(aPoint1->x(), aPoint1->y(), aPoint2->x(), aPoint2->y(), theX, theY, aX, anY);

  aDelta = gp_Pnt(theX, theY, 0).Distance(gp_Pnt(aX, anY, 0));

  return aDelta;
}

boost::shared_ptr<ModelAPI_Document> PartSet_Tools::document()
{
  return ModelAPI_PluginManager::get()->rootDocument();
}

void PartSet_Tools::setFeaturePoint(FeaturePtr theFeature, double theX, double theY,
                                    const std::string& theAttribute)
{
  if (!theFeature)
    return;
  boost::shared_ptr<ModelAPI_Data> aData = theFeature->data();
  boost::shared_ptr<GeomDataAPI_Point2D> aPoint =
        boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(theAttribute));
  if (aPoint)
    aPoint->setValue(theX, theY);
}

void PartSet_Tools::setFeatureValue(FeaturePtr theFeature, double theValue,
                                    const std::string& theAttribute)
{
  if (!theFeature)
    return;
  boost::shared_ptr<ModelAPI_Data> aData = theFeature->data();
  boost::shared_ptr<ModelAPI_AttributeDouble> anAttribute =
        boost::dynamic_pointer_cast<ModelAPI_AttributeDouble>(aData->attribute(theAttribute));
  if (anAttribute)
    anAttribute->setValue(theValue);
}

void PartSet_Tools::createConstraint(FeaturePtr theSketch,
                                     boost::shared_ptr<GeomDataAPI_Point2D> thePoint1,
                                     boost::shared_ptr<GeomDataAPI_Point2D> thePoint2)
{
  boost::shared_ptr<ModelAPI_Document> aDoc = document();
  FeaturePtr aFeature = aDoc->addFeature(SKETCH_CONSTRAINT_COINCIDENCE_KIND);

  if (theSketch) {
    boost::shared_ptr<SketchPlugin_Feature> aSketch = 
                           boost::dynamic_pointer_cast<SketchPlugin_Feature>(theSketch);
    aSketch->addSub(aFeature);
  }

  boost::shared_ptr<ModelAPI_Data> aData = aFeature->data();

  boost::shared_ptr<ModelAPI_AttributeRefAttr> aRef1 =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(aData->attribute(CONSTRAINT_ATTR_ENTITY_A));
  aRef1->setAttr(thePoint1);

  boost::shared_ptr<ModelAPI_AttributeRefAttr> aRef2 =
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>(aData->attribute(CONSTRAINT_ATTR_ENTITY_B));
  aRef2->setAttr(thePoint2);

  if (aFeature) // TODO: generate an error if feature was not created
    aFeature->execute();
}

void PartSet_Tools::getLinePoint(FeaturePtr theFeature, const std::string& theAttribute,
                                 double& theX, double& theY)
{
  if (!theFeature || theFeature->getKind() != SKETCH_LINE_KIND)
    return;
  boost::shared_ptr<ModelAPI_Data> aData = theFeature->data();
  boost::shared_ptr<GeomDataAPI_Point2D> aPoint =
        boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(theAttribute));
  theX = aPoint->x();
  theY = aPoint->y();
}

boost::shared_ptr<GeomDataAPI_Point2D> PartSet_Tools::findPoint(FeaturePtr theFeature,
                                                                double theX, double theY)
{
  boost::shared_ptr<GeomDataAPI_Point2D> aPoint2D;
  if (!theFeature)
    return aPoint2D;

  boost::shared_ptr<ModelAPI_Data> aData = theFeature->data();
  if (theFeature->getKind() == SKETCH_LINE_KIND)
  {
    boost::shared_ptr<GeomDataAPI_Point2D> aPoint =
          boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(LINE_ATTR_START));
    if (fabs(aPoint->x() - theX) < Precision::Confusion() && fabs(aPoint->y() - theY) < Precision::Confusion() )
      aPoint2D = aPoint;
    else {
      aPoint = boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(LINE_ATTR_END));
      if (fabs(aPoint->x() - theX) < Precision::Confusion() && fabs(aPoint->y() - theY) < Precision::Confusion() )
        aPoint2D = aPoint;
    }
  }
  else if (theFeature->getKind() == SKETCH_POINT_KIND)
  {
    boost::shared_ptr<GeomDataAPI_Point2D> aPoint =
          boost::dynamic_pointer_cast<GeomDataAPI_Point2D>(aData->attribute(POINT_ATTR_COORD));
    if (fabs(aPoint->x() - theX) < Precision::Confusion() && fabs(aPoint->y() - theY) < Precision::Confusion() )
      aPoint2D = aPoint;
  }

  return aPoint2D;
}
