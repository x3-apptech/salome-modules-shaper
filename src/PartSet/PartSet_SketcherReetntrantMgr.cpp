// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

#include "PartSet_SketcherReetntrantMgr.h"
#include "PartSet_Module.h"
#include "PartSet_SketcherMgr.h"
#include "PartSet_WidgetPoint2d.h"

#include "ModelAPI_Session.h"
#include "ModelAPI_AttributeString.h"
#include "ModelAPI_AttributeRefAttr.h"

#include "GeomDataAPI_Point2D.h"

#include <ModuleBase_IPropertyPanel.h>
#include <ModuleBase_OperationFeature.h>
#include <ModuleBase_ModelWidget.h>
#include <ModuleBase_ViewerPrs.h>
#include <ModuleBase_WidgetSelector.h>
#include <ModuleBase_PageWidget.h>
#include <ModuleBase_PageBase.h>
#include <ModuleBase_WidgetFactory.h>
#include <ModuleBase_OperationDescription.h>
#include "ModuleBase_ToolBox.h"
#include "ModuleBase_ISelection.h"

#include <SketchPlugin_Feature.h>
#include <SketchPlugin_Line.h>
#include <SketchPlugin_Arc.h>
#include <SketchPlugin_Circle.h>
#include <SketchPlugin_Point.h>

#include <XGUI_Workshop.h>
#include <XGUI_ModuleConnector.h>
#include <XGUI_OperationMgr.h>
#include <XGUI_PropertyPanel.h>
#include <XGUI_ErrorMgr.h>
#include <XGUI_SelectionMgr.h>

#include <QToolButton>

PartSet_SketcherReetntrantMgr::PartSet_SketcherReetntrantMgr(ModuleBase_IWorkshop* theWorkshop)
: QObject(theWorkshop),
  myWorkshop(theWorkshop),
  myRestartingMode(RM_None),
  myIsFlagsBlocked(false),
  myIsInternalEditOperation(false),
  myNoMoreWidgetsAttribute("")
{
}

PartSet_SketcherReetntrantMgr::~PartSet_SketcherReetntrantMgr()
{
}

ModuleBase_ModelWidget* PartSet_SketcherReetntrantMgr::internalActiveWidget() const
{
  ModuleBase_ModelWidget* aWidget = 0;
  if (!isActiveMgr())
    return aWidget;

  ModuleBase_Operation* anOperation = myWorkshop->currentOperation();
  if (anOperation) {
    ModuleBase_IPropertyPanel* aPanel = anOperation->propertyPanel();
    if (aPanel) { // check for case when the operation is started but property panel is not filled
      ModuleBase_ModelWidget* anActiveWidget = aPanel->activeWidget();
      if (myIsInternalEditOperation && (!anActiveWidget || !anActiveWidget->isViewerSelector()))
        aWidget = myInternalActiveWidget;
    }
  }
  return aWidget;
}

bool PartSet_SketcherReetntrantMgr::isInternalEditActive() const
{
  return myIsInternalEditOperation;
}

void PartSet_SketcherReetntrantMgr::updateInternalEditActiveState()
{
  if  (myIsInternalEditOperation) {
    ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                         (myWorkshop->currentOperation());
    if (aFOperation) {
      FeaturePtr aFeature = aFOperation->feature();
      QString anError = myWorkshop->module()->getFeatureError(aFeature);
      // stop started internal edit operation as soon as the operation becomes invalid
      // it is especially important for the sketch tangent arc feature
      if (!anError.isEmpty()) {
        aFOperation->setEditOperation(false);
        //workshop()->operationMgr()->updateApplyOfOperations();
        myIsInternalEditOperation = false;
        updateAcceptAllAction();
      }
    }
  }
}

bool PartSet_SketcherReetntrantMgr::operationCommitted(ModuleBase_Operation* theOperation)
{
  bool aProcessed = false;
  if (!isActiveMgr())
    return aProcessed;

  aProcessed = myIsInternalEditOperation;
  resetFlags();

  return aProcessed;
}

void PartSet_SketcherReetntrantMgr::operationStarted(ModuleBase_Operation* theOperation)
{
  if (!isActiveMgr())
    return;

  if (myPreviousFeature.get() && myRestartingMode == RM_LastFeatureUsed) {
    ModuleBase_OperationFeature* aCurrentOperation = dynamic_cast<ModuleBase_OperationFeature*>(
                                                                myWorkshop->currentOperation());
    CompositeFeaturePtr aSketch = module()->sketchMgr()->activeSketch();
    copyReetntrantAttributes(myPreviousFeature, aCurrentOperation->feature(), aSketch);
  }
  resetFlags();
}

void PartSet_SketcherReetntrantMgr::operationAborted(ModuleBase_Operation* theOperation)
{
  if (!isActiveMgr())
    return;

  resetFlags();
}

bool PartSet_SketcherReetntrantMgr::processMouseMoved(ModuleBase_IViewWindow* theWnd,
                                                      QMouseEvent* theEvent)
{
  bool aProcessed = false;
  if (!isActiveMgr())
    return aProcessed;

  if  (myIsInternalEditOperation) {
    ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                         (myWorkshop->currentOperation());
    FeaturePtr aLastFeature = myRestartingMode == RM_LastFeatureUsed ? aFOperation->feature()
                                                                     : FeaturePtr();
    if (aLastFeature) {
      ModuleBase_ModelWidget* anActiveWidget = module()->activeWidget();
      ModuleBase_IPropertyPanel* aPanel = myWorkshop->currentOperation()->propertyPanel();

      FeaturePtr aCurrentFeature = aFOperation->feature();
      bool isLineFeature = false, isArcFeature = false;
      std::string anAttributeOnStart;
      if (aCurrentFeature->getKind() == SketchPlugin_Line::ID()) {
        anAttributeOnStart = SketchPlugin_Line::START_ID();
        isLineFeature = anActiveWidget->attributeID() == anAttributeOnStart;
      }
      else if (isTangentArc(aFOperation, module()->sketchMgr()->activeSketch())) {
        anAttributeOnStart = SketchPlugin_Arc::TANGENT_POINT_ID();
        isArcFeature = anActiveWidget->attributeID() == anAttributeOnStart;
      }
      bool aCanBeActivatedByMove = isLineFeature || isArcFeature;
      if (aCanBeActivatedByMove) {
        /// before restarting of operation we need to clear selection, as it may take part in
        /// new feature creation, e.g. tangent arc. But it is not necessary as it was processed
        /// by mouse release when the operation was restarted.
        workshop()->selector()->clearSelection();

        myPreviousFeature = aFOperation->feature();
        restartOperation();
        myPreviousFeature = FeaturePtr();

        anActiveWidget = module()->activeWidget();
        aCurrentFeature = anActiveWidget->feature();
        aProcessed = true;
        if (anActiveWidget->attributeID() == anAttributeOnStart) {
          // it was not deactivated by preselection processing
          aPanel->activateNextWidget(anActiveWidget);
        }
      } else {
        // processing mouse move in active widget of restarted operation
        ModuleBase_ModelWidget* anActiveWidget = module()->activeWidget();
        PartSet_MouseProcessor* aProcessor = dynamic_cast<PartSet_MouseProcessor*>(anActiveWidget);
        if (aProcessor)
          aProcessor->mouseMoved(theWnd, theEvent);
      }
    }
  }
  return aProcessed;
}

bool PartSet_SketcherReetntrantMgr::processMousePressed(ModuleBase_IViewWindow* /* theWnd*/,
                                                        QMouseEvent* /* theEvent*/)
{
  return isActiveMgr() && myIsInternalEditOperation;
}

bool PartSet_SketcherReetntrantMgr::processMouseReleased(ModuleBase_IViewWindow* theWnd,
                                                         QMouseEvent* theEvent)
{
  bool aProcessed = false;
  if (!isActiveMgr())
    return aProcessed;

  if (myIsInternalEditOperation) {
    ModuleBase_Operation* anOperation = myWorkshop->currentOperation();
    ModuleBase_IPropertyPanel* aPanel = anOperation->propertyPanel();

    ModuleBase_ModelWidget* anActiveWidget = aPanel->activeWidget();
    if (!anActiveWidget || !anActiveWidget->isViewerSelector()) {

      // block of viewer update
      // we need to block update content of the viewer because of Sketch Point feature
      // in activate() the value of the point is initialized and it can be displayed
      // but the default value is [0, 0]. So, we block update viewer contentent until
      // onMouseRelease happens, which correct the point position
      ModuleBase_Tools::blockUpdateViewer(true);

      ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                           (myWorkshop->currentOperation());
      myPreviousFeature = aFOperation->feature();

      /// selection should be obtained from workshop before ask if the operation can be started as
      /// the canStartOperation method performs commit/abort of previous operation.
      /// Sometimes commit/abort
      /// may cause selection clear(Sketch operation) as a result
      /// it will be lost and is not used for preselection.
      ModuleBase_ISelection* aSelection = myWorkshop->selection();
      QList<ModuleBase_ViewerPrsPtr> aPreSelected =
        aSelection->getSelected(ModuleBase_ISelection::AllControls);

      restartOperation();
      myPreviousFeature = FeaturePtr();
      aProcessed = true;

      // fill the first widget by the mouse event point
      // if the active widget is not the first, it means that the restarted operation is filled by
      // the current preselection.
      PartSet_WidgetPoint2D* aPoint2DWdg =
        dynamic_cast<PartSet_WidgetPoint2D*>(module()->activeWidget());
      ModuleBase_ModelWidget* aFirstWidget = aPanel->findFirstAcceptingValueWidget();
      if (aPoint2DWdg && aPoint2DWdg == aFirstWidget) {
        if (!aPreSelected.empty())
          aPoint2DWdg->setPreSelection(aPreSelected.front());
        aPoint2DWdg->mouseReleased(theWnd, theEvent);
        if (!aPreSelected.empty())
          aPoint2DWdg->setPreSelection(ModuleBase_ViewerPrsPtr());
      }
      // unblock viewer update
      ModuleBase_Tools::blockUpdateViewer(false);
    }
  }

  return aProcessed;
}

void PartSet_SketcherReetntrantMgr::onWidgetActivated()
{
  if (!isActiveMgr())
    return;
  if (!myIsInternalEditOperation)
    return;

  PartSet_Module* aModule = module();
  ModuleBase_ModelWidget* aFirstWidget = aModule->activeWidget();
  ModuleBase_IPropertyPanel* aPanel = aModule->currentOperation()->propertyPanel();
  if (aFirstWidget != aPanel->activeWidget()) {
    ModuleBase_WidgetSelector* aWSelector = dynamic_cast<ModuleBase_WidgetSelector*>(aFirstWidget);
    if (aWSelector)
      aWSelector->activateSelectionAndFilters(true);
  }
}

void PartSet_SketcherReetntrantMgr::onNoMoreWidgets(const std::string& thePreviousAttributeID)
{
  if (!isActiveMgr())
    return;

  // we should avoid processing of the signal about no more widgets attributes and
  // do this after the restart operaion is finished if it was called
  // onNoMoreWidgets depends on myIsFlagsBlocked and fill myNoMoreWidgetsAttribute
  // if it should be called after restart
  if (myIsFlagsBlocked) {
    myNoMoreWidgetsAttribute = thePreviousAttributeID;
    return;
  }

  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                       (myWorkshop->currentOperation());
  if (!myWorkshop->module()->getFeatureError(aFOperation->feature()).isEmpty())
    return;

  if (aFOperation && module()->sketchMgr()->isNestedSketchOperation(aFOperation)) {
    bool isStarted = false;
    if (!module()->sketchMgr()->sketchSolverError()) {
      if (myRestartingMode != RM_Forbided) {
        myRestartingMode = RM_LastFeatureUsed;
        isStarted = startInternalEdit(thePreviousAttributeID);
      }
    }
    if (!isStarted)
      aFOperation->commit();
  }
}

bool PartSet_SketcherReetntrantMgr::processEnter(const std::string& thePreviousAttributeID)
{
  bool isDone = false;

  if (!isActiveMgr())
    return isDone;

  // empty previous attribute means that the Apply/Ok button has focus and the enter
  // should not lead to start edition mode of the previous operation
  if (thePreviousAttributeID.empty())
    return isDone;

  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                       (myWorkshop->currentOperation());
  if (!myWorkshop->module()->getFeatureError(aFOperation->feature()).isEmpty())
    return isDone;

  bool isSketchSolverError = module()->sketchMgr()->sketchSolverError();

  if (!isSketchSolverError) {
    myRestartingMode = RM_EmptyFeatureUsed;
    isDone = startInternalEdit(thePreviousAttributeID);
  }

  return isDone;
}

void PartSet_SketcherReetntrantMgr::onVertexSelected()
{
  if (!isActiveMgr())
    return;

  ModuleBase_Operation* anOperation = myWorkshop->currentOperation();
  std::string anOperationId = anOperation->id().toStdString();
  if (anOperationId == SketchPlugin_Line::ID() ||
      isTangentArc(anOperation, module()->sketchMgr()->activeSketch())) {
    /// If last line finished on vertex the lines creation sequence has to be break
    ModuleBase_IPropertyPanel* aPanel = anOperation->propertyPanel();
    ModuleBase_ModelWidget* anActiveWidget = aPanel->activeWidget();
    const QList<ModuleBase_ModelWidget*>& aWidgets = aPanel->modelWidgets();
    QList<ModuleBase_ModelWidget*>::const_iterator anIt = aWidgets.begin(), aLast = aWidgets.end();
    bool aFoundWidget = false;
    bool aFoundObligatory = false;
    for (; anIt != aLast && !aFoundObligatory; anIt++) {
      if (!aFoundWidget)
        aFoundWidget = *anIt == anActiveWidget;
      else
        aFoundObligatory = (*anIt)->isObligatory();
    }
    if (!aFoundObligatory)
      myRestartingMode = RM_Forbided;
  }
}

void PartSet_SketcherReetntrantMgr::onBeforeStopped()
{
  if (!isActiveMgr() || !myIsInternalEditOperation)
    return;

  beforeStopInternalEdit();
}

bool PartSet_SketcherReetntrantMgr::canBeCommittedByPreselection()
{
  return !isActiveMgr() || myRestartingMode == RM_None;
}

bool PartSet_SketcherReetntrantMgr::isInternalEditStarted() const
{
  return myIsInternalEditOperation;
}

bool PartSet_SketcherReetntrantMgr::isActiveMgr() const
{
  ModuleBase_Operation* aCurrentOperation = myWorkshop->currentOperation();

  bool anActive = PartSet_SketcherMgr::isSketchOperation(aCurrentOperation);
  if (!anActive) {
    anActive = module()->sketchMgr()->isNestedSketchOperation(aCurrentOperation);
    if (anActive) { // the manager is not active when the current operation is a usual Edit
      ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                       (myWorkshop->currentOperation());
      if (aFOperation->isEditOperation())
        anActive = myIsInternalEditOperation;
    }
  }
  return anActive;
}

bool PartSet_SketcherReetntrantMgr::startInternalEdit(const std::string& thePreviousAttributeID)
{
  bool isDone = false;
  /// this is workaround for ModuleBase_WidgetEditor, used in SALOME mode. Sometimes key enter
  /// event comes two times, so we should not start another internal edit operation
  /// the Apply button becomes disabled becase the second additional internal feature is created
  if (myIsInternalEditOperation)
    return true;

  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                     (myWorkshop->currentOperation());

  if (aFOperation && module()->sketchMgr()->isNestedSketchOperation(aFOperation)) {
    aFOperation->setEditOperation(true/*, false*/);
    createInternalFeature();

    myIsInternalEditOperation = true;
    updateAcceptAllAction();

    isDone = true;
    connect(aFOperation, SIGNAL(beforeCommitted()), this, SLOT(onBeforeStopped()));
    connect(aFOperation, SIGNAL(beforeAborted()), this, SLOT(onBeforeStopped()));

    // activate selection filters of the first widget in the viewer
    onWidgetActivated();

    // activate the last active widget in the Property Panel
    if (!thePreviousAttributeID.empty()) {
      ModuleBase_Operation* anEditOperation = module()->currentOperation();
      if (anEditOperation) {
        ModuleBase_IPropertyPanel* aPanel = aFOperation->propertyPanel();
        ModuleBase_ModelWidget* aPreviousAttributeWidget = 0;
        QList<ModuleBase_ModelWidget*> aWidgets = aPanel->modelWidgets();
        for (int i = 0, aNb = aWidgets.size(); i < aNb && !aPreviousAttributeWidget; i++) {
          if (aWidgets[i]->attributeID() == thePreviousAttributeID) {
          /// workaround for the same attributes used in different stacked widgets(attribute types)
          if (ModuleBase_ToolBox::isOffToolBoxParent(aWidgets[i]))
            continue;
            aPreviousAttributeWidget = aWidgets[i];
          }
        }
        // If the current widget is a selector, do nothing, it processes the mouse press
        if (aPreviousAttributeWidget) {
          if (!aPreviousAttributeWidget->isViewerSelector()) {
            aPreviousAttributeWidget->focusTo();
            aPreviousAttributeWidget->selectContent();
          }
          else {
            // in case of shape multi selector, the widget does not lose focus by filling
            // like it is in shape selector. So, if enter is pressed, the multi shape selector
            // control should be deactivated. The focus is moved to Apply button and there
            // should not be active control visualized in property panel
            if (aPreviousAttributeWidget == aPanel->activeWidget()) {
              aPanel->activateWidget(NULL, false);
            }
            // if there is no the next widget to be automatically activated,
            // the Ok button in property
            // panel should accept the focus(example is parallel constraint on sketch lines)
            QToolButton* anOkBtn =
              dynamic_cast<XGUI_PropertyPanel*>(aPanel)->findButton(PROP_PANEL_OK);
            if (anOkBtn)
              anOkBtn->setFocus(Qt::TabFocusReason);
          }
        }
      }
    }
  }
  if (isDone)
    module()->sketchMgr()->clearClickedFlags();

  return isDone;
}

void PartSet_SketcherReetntrantMgr::beforeStopInternalEdit()
{
  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                      (myWorkshop->currentOperation());
  if (aFOperation) {
    disconnect(aFOperation, SIGNAL(beforeCommitted()), this, SLOT(onBeforeStopped()));
    disconnect(aFOperation, SIGNAL(beforeAborted()), this, SLOT(onBeforeStopped()));
  }

  deleteInternalFeature();
}

void PartSet_SketcherReetntrantMgr::restartOperation()
{
  if (myIsInternalEditOperation) {
    ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>(
                                                                  myWorkshop->currentOperation());
    if (aFOperation) {
      myNoMoreWidgetsAttribute = "";
      myIsFlagsBlocked = true;
      module()->launchOperation(aFOperation->id());
      myIsFlagsBlocked = false;
      resetFlags();
      // we should avoid processing of the signal about no more widgets attributes and
      // do this after the restart operaion is finished if it was called
      // onNoMoreWidgets depends on myIsFlagsBlocked and fill myNoMoreWidgetsAttribute
      // if it should be called after restart
      if (!myNoMoreWidgetsAttribute.empty()) {
        onNoMoreWidgets(myNoMoreWidgetsAttribute);
        myNoMoreWidgetsAttribute = "";
      }
    }
  }
}

void PartSet_SketcherReetntrantMgr::createInternalFeature()
{
  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                     (myWorkshop->currentOperation());

  if (aFOperation && module()->sketchMgr()->isNestedSketchOperation(aFOperation)) {
    FeaturePtr anOperationFeature = aFOperation->feature();

    CompositeFeaturePtr aSketch = module()->sketchMgr()->activeSketch();
    myInternalFeature = aSketch->addFeature(anOperationFeature->getKind());

    bool isFeatureChanged = copyReetntrantAttributes(anOperationFeature, myInternalFeature,
                                                     aSketch, false);
    XGUI_PropertyPanel* aPropertyPanel = dynamic_cast<XGUI_PropertyPanel*>
                                                  (aFOperation->propertyPanel());

    myInternalWidget = new QWidget(aPropertyPanel->contentWidget()->pageWidget());
    myInternalWidget->setVisible(false);

    ModuleBase_PageWidget* anInternalPage = new ModuleBase_PageWidget(myInternalWidget);

    QString aXmlRepr = aFOperation->getDescription()->xmlRepresentation();
    ModuleBase_WidgetFactory aFactory(aXmlRepr.toStdString(), myWorkshop);

    aFactory.createWidget(anInternalPage);
    QList<ModuleBase_ModelWidget*> aWidgets = aFactory.getModelWidgets();

    foreach (ModuleBase_ModelWidget* aWidget, aWidgets) {
      bool isStoreValue = !aFOperation->isEditOperation() &&
                          !aWidget->getDefaultValue().empty() &&
                          !aWidget->isComputedDefault();
      aWidget->setFeature(myInternalFeature, isStoreValue);
      if (!isStoreValue && isFeatureChanged)
        aWidget->restoreValue();
    }

    ModuleBase_ModelWidget* aFirstWidget = ModuleBase_IPropertyPanel::findFirstAcceptingValueWidget
                                                                                        (aWidgets);
    if (aFirstWidget)
      myInternalActiveWidget = aFirstWidget;
  }
}

void PartSet_SketcherReetntrantMgr::deleteInternalFeature()
{
  if (myInternalActiveWidget) {
    ModuleBase_WidgetSelector* aWSelector =
      dynamic_cast<ModuleBase_WidgetSelector*>(myInternalActiveWidget);
    if (aWSelector)
      aWSelector->activateSelectionAndFilters(false);
    myInternalActiveWidget = 0;
  }
  delete myInternalWidget;
  myInternalWidget = 0;

  QObjectPtrList anObjects;
  anObjects.append(myInternalFeature);
  workshop()->deleteFeatures(anObjects);
  myInternalFeature = FeaturePtr();
}

void PartSet_SketcherReetntrantMgr::resetFlags()
{
  if (!myIsFlagsBlocked) {
    myIsInternalEditOperation = false;
    updateAcceptAllAction();
    myRestartingMode = RM_None;
  }
}

bool PartSet_SketcherReetntrantMgr::copyReetntrantAttributes(const FeaturePtr& theSourceFeature,
                                                             const FeaturePtr& theNewFeature,
                                                             const CompositeFeaturePtr& theSketch,
                                                             const bool isTemporary)
{
  bool aChanged = false;
  if (!theSourceFeature.get() || !theSourceFeature->data().get() ||
      !theSourceFeature->data()->isValid())
    return aChanged;

  std::string aFeatureKind = theSourceFeature->getKind();
  if (aFeatureKind == SketchPlugin_Line::ID()) {
    // Initialize new line with first point equal to end of previous
    std::shared_ptr<ModelAPI_Data> aSFData = theSourceFeature->data();
    std::shared_ptr<GeomDataAPI_Point2D> aSPoint = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
                                                 aSFData->attribute(SketchPlugin_Line::END_ID()));
    std::shared_ptr<ModelAPI_Data> aNFData = theNewFeature->data();
    std::shared_ptr<GeomDataAPI_Point2D> aNPoint = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
                                                 aNFData->attribute(SketchPlugin_Line::START_ID()));
    aNPoint->setValue(aSPoint->x(), aSPoint->y());
    PartSet_Tools::createConstraint(theSketch, aSPoint, aNPoint);

    aNPoint = std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
                                                 aSFData->attribute(SketchPlugin_Line::END_ID()));
    aNPoint->setValue(aSPoint->x(), aSPoint->y());
  }
  else if (aFeatureKind == SketchPlugin_Circle::ID()) {
    // set circle type
    std::string aTypeAttributeId = SketchPlugin_Circle::CIRCLE_TYPE();
    AttributeStringPtr aSourceFeatureTypeAttr = theSourceFeature->data()->string(aTypeAttributeId);
    AttributeStringPtr aNewFeatureTypeAttr = theNewFeature->data()->string(aTypeAttributeId);
    aNewFeatureTypeAttr->setValue(aSourceFeatureTypeAttr->value());
    //ModuleBase_Tools::flushUpdated(theNewFeature);
    aChanged = true;
  }
  else if (aFeatureKind == SketchPlugin_Arc::ID()) {
    // set arc type
    std::string aTypeAttributeId = SketchPlugin_Arc::ARC_TYPE();
    AttributeStringPtr aSourceFeatureTypeAttr = theSourceFeature->data()->string(aTypeAttributeId);
    AttributeStringPtr aNewFeatureTypeAttr = theNewFeature->data()->string(aTypeAttributeId);
    aNewFeatureTypeAttr->setValue(aSourceFeatureTypeAttr->value());

    // if the arc is tangent, set coincidence to end point of the previous arc
    std::string anArcType = aSourceFeatureTypeAttr->value();
    if (anArcType == SketchPlugin_Arc::ARC_TYPE_TANGENT()) {
      // get the last point of the previuos arc feature(geom point 2d)
      std::shared_ptr<ModelAPI_Data> aSData = theSourceFeature->data();
      std::shared_ptr<GeomDataAPI_Point2D> aSPointAttr =
                                      std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
                                      aSData->attribute(SketchPlugin_Arc::END_ID()));
      // get point attribute on the current feature
      AttributeRefAttrPtr aTangentPointAttr = theNewFeature->data()->refattr(
                                                    SketchPlugin_Arc::TANGENT_POINT_ID());
      aTangentPointAttr->setAttr(aSPointAttr);

      std::shared_ptr<GeomDataAPI_Point2D> aNPointAttr =
                                    std::dynamic_pointer_cast<GeomDataAPI_Point2D>(
                                    theNewFeature->data()->attribute(SketchPlugin_Arc::END_ID()));
      aNPointAttr->setValue(aSPointAttr->x(), aSPointAttr->y());

    }
    //ModuleBase_Tools::flushUpdated(theNewFeature);
    aChanged = true;
  }
  return aChanged;
}

bool PartSet_SketcherReetntrantMgr::isTangentArc(ModuleBase_Operation* theOperation,
                                                 const CompositeFeaturePtr& /*theSketch*/) const
{
  bool aTangentArc = false;
  ModuleBase_OperationFeature* aFOperation = dynamic_cast<ModuleBase_OperationFeature*>
                                                                        (theOperation);
  if (aFOperation && module()->sketchMgr()->isNestedSketchOperation(aFOperation)) {
    FeaturePtr aFeature = aFOperation->feature();
    if (aFeature.get() && aFeature->getKind() == SketchPlugin_Arc::ID()) {
      AttributeStringPtr aTypeAttr = aFeature->data()->string(SketchPlugin_Arc::ARC_TYPE());
      std::string anArcType = aTypeAttr.get() ? aTypeAttr->value() : "";
      aTangentArc = anArcType == SketchPlugin_Arc::ARC_TYPE_TANGENT();
    }
  }
  return aTangentArc;
}

void PartSet_SketcherReetntrantMgr::updateAcceptAllAction()
{
  CompositeFeaturePtr aSketch = module()->sketchMgr()->activeSketch();
  if (aSketch.get())
    workshop()->errorMgr()->updateAcceptAllAction(aSketch);
}

XGUI_Workshop* PartSet_SketcherReetntrantMgr::workshop() const
{
  XGUI_ModuleConnector* aConnector = dynamic_cast<XGUI_ModuleConnector*>(myWorkshop);
  return aConnector->workshop();
}

PartSet_Module* PartSet_SketcherReetntrantMgr::module() const
{
  return dynamic_cast<PartSet_Module*>(myWorkshop->module());
}

