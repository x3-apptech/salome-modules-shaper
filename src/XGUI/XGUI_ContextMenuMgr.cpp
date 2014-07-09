
#include "XGUI_ContextMenuMgr.h"
#include "XGUI_Workshop.h"
#include "XGUI_ObjectsBrowser.h"
#include "XGUI_SelectionMgr.h"
#include "XGUI_Displayer.h"
#include "XGUI_MainWindow.h"
#include "XGUI_ViewerProxy.h"
#include "XGUI_Selection.h"

#include "PartSetPlugin_Part.h"

#include <ModelAPI_Data.h>
#include <ModelAPI_AttributeDocRef.h>
#include <ModelAPI_Object.h>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMdiArea>

XGUI_ContextMenuMgr::XGUI_ContextMenuMgr(XGUI_Workshop* theParent) :
QObject(theParent), myWorkshop(theParent)
{
}

XGUI_ContextMenuMgr::~XGUI_ContextMenuMgr()
{
}

void XGUI_ContextMenuMgr::createActions()
{
  QAction* aAction = new QAction(QIcon(":pictures/edit.png"), tr("Edit..."), this);
  addAction("EDIT_CMD", aAction);

  aAction = new QAction(QIcon(":pictures/activate.png"), tr("Activate"), this);
  addAction("ACTIVATE_PART_CMD", aAction);

  aAction = new QAction(QIcon(":pictures/assembly.png"), tr("Deactivate"), this);
  addAction("DEACTIVATE_PART_CMD", aAction);

  aAction = new QAction(QIcon(":pictures/delete.png"), tr("Delete"), this);
  addAction("DELETE_CMD", aAction);

  aAction = new QAction(QIcon(":pictures/eye_pencil.png"), tr("Show"), this);
  addAction("SHOW_CMD", aAction);

  aAction = new QAction(QIcon(":pictures/eye_pencil_closed.png"), tr("Hide"), this);
  addAction("HIDE_CMD", aAction);
}

void XGUI_ContextMenuMgr::addAction(const QString& theId, QAction* theAction)
{
  if (myActions.contains(theId))
    qCritical("A command with Id = '%s' already defined!", qPrintable(theId));
  theAction->setData(theId);
  connect(theAction, SIGNAL(triggered(bool)), this, SLOT(onAction(bool)));
  myActions[theId] = theAction;
}

QAction* XGUI_ContextMenuMgr::action(const QString& theId) const
{
  if (myActions.contains(theId))
    return myActions[theId];
  return 0;
}

QStringList XGUI_ContextMenuMgr::actionIds() const
{
  return myActions.keys();
}

void XGUI_ContextMenuMgr::onAction(bool isChecked)
{
  QAction* aAction = static_cast<QAction*>(sender());
  emit actionTriggered(aAction->data().toString(), isChecked);
}

void XGUI_ContextMenuMgr::updateCommandsStatus()
{
}

void XGUI_ContextMenuMgr::onContextMenuRequest(QContextMenuEvent* theEvent)
{
  QMenu* aMenu = 0;
  if (sender() == myWorkshop->objectBrowser())
    aMenu = objectBrowserMenu();
  else if (sender() == myWorkshop->viewer()) {
    aMenu = viewerMenu();
  }
  
  if (aMenu && (aMenu->actions().size() > 0)) {
    aMenu->exec(theEvent->globalPos());
    delete aMenu;
  }
}

QMenu* XGUI_ContextMenuMgr::objectBrowserMenu() const
{
  QMenu* aMenu = new QMenu();
  XGUI_SelectionMgr* aSelMgr = myWorkshop->selector();
  QFeatureList aFeatures = aSelMgr->selection()->selectedFeatures();
  if (aFeatures.size() == 1) {
    PluginManagerPtr aMgr = ModelAPI_PluginManager::get();
    FeaturePtr aFeature = aFeatures.first();
    //Process Feature
    if (aFeature) {
      if (aFeature->getKind() == PARTSET_PART_KIND) {
        ObjectPtr aObject = boost::dynamic_pointer_cast<ModelAPI_Object>(aFeature);
        DocumentPtr aFeaDoc = aObject->featureRef()->data()->docRef("PartDocument")->value();
        if (aMgr->currentDocument() == aFeaDoc)
          aMenu->addAction(action("DEACTIVATE_PART_CMD"));
        else 
          aMenu->addAction(action("ACTIVATE_PART_CMD"));
      } else {
        aMenu->addAction(action("EDIT_CMD"));

        XGUI_Displayer* aDisplayer = myWorkshop->displayer();
        if (aDisplayer->isVisible(aFeature))
          aMenu->addAction(action("HIDE_CMD"));
        else
          aMenu->addAction(action("SHOW_CMD"));
      }
      aMenu->addAction(action("DELETE_CMD"));
      aMenu->addSeparator();

    // Process Root object (document)
    } else { // If feature is 0 the it means that selected root object (document)
      if (aMgr->currentDocument() != aMgr->rootDocument()) {
        aMenu->addAction(action("ACTIVATE_PART_CMD"));
      }
    }
  }
  aMenu->addActions(myWorkshop->objectBrowser()->actions());
  if (aMenu->actions().size() > 0) {
    return aMenu;
  }
  delete aMenu;
  return 0;
}

QMenu* XGUI_ContextMenuMgr::viewerMenu() const
{
  QMenu* aMenu = new QMenu();
  addViewerItems(aMenu);
  if (aMenu->actions().size() > 0) {
    return aMenu;
  }
  delete aMenu;
  return 0;
}

void XGUI_ContextMenuMgr::addViewerItems(QMenu* theMenu) const
{
  XGUI_SelectionMgr* aSelMgr = myWorkshop->selector();
  QFeatureList aFeatures = aSelMgr->selection()->selectedFeatures();
  if (aFeatures.size() > 0) {
    if (aFeatures.size() == 1)
      theMenu->addAction(action("EDIT_CMD"));
    bool isVisible = false;
    foreach(FeaturePtr aFeature, aFeatures) {
      if (myWorkshop->displayer()->isVisible(aFeature)) {
        isVisible = true;
        break;
      }
    }
    if (isVisible)
      theMenu->addAction(action("HIDE_CMD"));
    else 
      theMenu->addAction(action("SHOW_CMD"));
    theMenu->addAction(action("DELETE_CMD"));
  }
  if (!myWorkshop->isSalomeMode()) {
    QMdiArea* aMDI = myWorkshop->mainWindow()->mdiArea();
    if (aMDI->actions().size() > 0) {
      QMenu* aSubMenu = theMenu->addMenu(tr("Windows"));
      aSubMenu->addActions(aMDI->actions());
    }
  }
}

void XGUI_ContextMenuMgr::connectObjectBrowser() const
{
  connect(myWorkshop->objectBrowser(), SIGNAL(contextMenuRequested(QContextMenuEvent*)), 
    this, SLOT(onContextMenuRequest(QContextMenuEvent*)));
}

void XGUI_ContextMenuMgr::connectViewer() const
{
  connect(myWorkshop->viewer(), SIGNAL(contextMenuRequested(QContextMenuEvent*)), 
    this, SLOT(onContextMenuRequest(QContextMenuEvent*)));
}

