// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        SketcherPrs_Radius.cpp
// Created:     26 March 2015
// Author:      Vitaly SMETANNIKOV

#include "SketcherPrs_Radius.h"
#include "SketcherPrs_Tools.h"

#include <SketchPlugin_ConstraintRadius.h>
#include <SketchPlugin_Constraint.h>
#include <SketchPlugin_Circle.h>
#include <SketchPlugin_Arc.h>

#include <GeomDataAPI_Point2D.h>
#include <GeomAPI_Pnt2d.h>
#include <GeomAPI_Circ.h>
#include <GeomAPI_XYZ.h>
#include <ModelAPI_AttributeDouble.h>

#include <gp_Circ.hxx>

static const gp_Circ MyDefCirc(gp_Ax2(gp_Pnt(0,0,0), gp_Dir(0,0,1)), 1);

IMPLEMENT_STANDARD_HANDLE(SketcherPrs_Radius, AIS_RadiusDimension);
IMPLEMENT_STANDARD_RTTIEXT(SketcherPrs_Radius, AIS_RadiusDimension);

static const Standard_ExtCharacter MyEmptySymbol(' ');
static const Standard_ExtCharacter MySummSymbol(0x03A3);

SketcherPrs_Radius::SketcherPrs_Radius(ModelAPI_Feature* theConstraint, 
                                       const std::shared_ptr<GeomAPI_Ax3>& thePlane)
: AIS_RadiusDimension(MyDefCirc), myConstraint(theConstraint), myPlane(thePlane)
{
  // Set default values of the presentation
  myAspect = new Prs3d_DimensionAspect();
  myAspect->MakeArrows3d(false);
  myAspect->MakeText3d(false);
  myAspect->MakeTextShaded(false);
  myAspect->MakeUnitsDisplayed(false);
  myAspect->TextAspect()->SetHeight(SketcherPrs_Tools::getDefaultTextHeight());
  myAspect->ArrowAspect()->SetLength(SketcherPrs_Tools::getArrowSize());
  
  SetDimensionAspect(myAspect);
  SetSelToleranceForText2d(SketcherPrs_Tools::getDefaultTextHeight());
}

void SketcherPrs_Radius::Compute(const Handle(PrsMgr_PresentationManager3d)& thePresentationManager,
                                 const Handle(Prs3d_Presentation)& thePresentation, 
                                 const Standard_Integer theMode)
{
  DataPtr aData = myConstraint->data();

  // Flyout point
  std::shared_ptr<GeomDataAPI_Point2D> aFlyoutAttr = 
    std::dynamic_pointer_cast<GeomDataAPI_Point2D>
    (aData->attribute(SketchPlugin_Constraint::FLYOUT_VALUE_PNT()));
  if (!aFlyoutAttr->isInitialized())
    return; // can not create a good presentation

  // Get circle
  std::shared_ptr<ModelAPI_AttributeRefAttr> anAttr = 
    std::dynamic_pointer_cast<ModelAPI_AttributeRefAttr>
    (aData->attribute(SketchPlugin_Constraint::ENTITY_A()));
  if (!anAttr) return;

  std::shared_ptr<ModelAPI_Feature> aCyrcFeature = ModelAPI_Feature::feature(anAttr->object());
  double aRadius = 1;
  std::shared_ptr<GeomDataAPI_Point2D> aCenterAttr;
  // it is possible that circle result becomes zero, in this case the presentation should disappear
  // for example, it happens when circle radius is set to zero
  if (!aCyrcFeature)
    return;
  if (aCyrcFeature->getKind() == SketchPlugin_Circle::ID()) { // circle
    aCenterAttr = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
        aCyrcFeature->data()->attribute(SketchPlugin_Circle::CENTER_ID()));
    AttributeDoublePtr aCircRadius = 
      std::dynamic_pointer_cast<ModelAPI_AttributeDouble>(
      aCyrcFeature->data()->attribute(SketchPlugin_Circle::RADIUS_ID()));
    aRadius = aCircRadius->value();
  } else { // arc
    aCenterAttr = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
        aCyrcFeature->data()->attribute(SketchPlugin_Arc::CENTER_ID()));
    std::shared_ptr<GeomDataAPI_Point2D> aStartAttr = 
      std::dynamic_pointer_cast<GeomDataAPI_Point2D>
      (aCyrcFeature->data()->attribute(SketchPlugin_Arc::START_ID()));
    aRadius = aCenterAttr->pnt()->distance(aStartAttr->pnt());
  }
  std::shared_ptr<GeomAPI_Pnt> aCenter = myPlane->to3D(aCenterAttr->x(), aCenterAttr->y());
  std::shared_ptr<GeomAPI_Dir> aNormal = myPlane->normal();

  GeomAPI_Circ aCircle(aCenter, aNormal, aRadius);
    
  std::shared_ptr<GeomAPI_Pnt> anAnchor = SketcherPrs_Tools::getAnchorPoint(myConstraint, myPlane);

  gp_Circ aCirc = aCircle.impl<gp_Circ>();
  gp_Pnt anAncorPnt = anAnchor->impl<gp_Pnt>();
  // anchor point should not coincide to the location point of the circle
  // OCCT does not process this case.
  if (anAncorPnt.Distance(aCirc.Location()) < 1e-7)
    return;
  SetMeasuredGeometry(aCirc, anAncorPnt);
  SetCustomValue(aRadius);

  myAspect->SetExtensionSize(myAspect->ArrowAspect()->Length());
  myAspect->SetArrowTailSize(myAspect->ArrowAspect()->Length());
  // The value of vertical aligment is sometimes changed
  myAspect->TextAspect()->SetVerticalJustification(Graphic3d_VTA_CENTER);

  AttributeDoublePtr aValue = myConstraint->data()->real(SketchPlugin_Constraint::VALUE());
  std::set<std::string> aParams = aValue->usedParameters();
  if (aParams.size() > 0) {
    SetSpecialSymbol(MySummSymbol);
    SetDisplaySpecialSymbol(AIS_DSS_Before);
  }
  else {
    SetSpecialSymbol(MyEmptySymbol);
    SetDisplaySpecialSymbol(AIS_DSS_Before);
  }

  AIS_RadiusDimension::Compute(thePresentationManager, thePresentation, theMode);
}

void SketcherPrs_Radius::ComputeSelection(const Handle(SelectMgr_Selection)& aSelection,
                                                   const Standard_Integer theMode)
{
  // Map the application selection modes to standard ones
  Standard_Integer aMode;
  switch (theMode) {
  case 0: // we should use selection of all objects
    aMode = 0;
    break;
  case SketcherPrs_Tools::Sel_Dimension_All:
    aMode = 0;
    break;
  case SketcherPrs_Tools::Sel_Dimension_Line:
    aMode = 1;
    break;
  case SketcherPrs_Tools::Sel_Dimension_Text:
    aMode = 2;
    break;
  default: {
    // there are own selection modes, so the others should be ignored
    // otherwise, the text selection appears in the viewer
    return; 
  }
  }
  AIS_RadiusDimension::ComputeSelection(aSelection, aMode);
}
