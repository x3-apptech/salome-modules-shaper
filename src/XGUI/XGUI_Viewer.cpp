#include "XGUI_Viewer.h"
#include "XGUI_MainWindow.h"
#include "XGUI_ViewWindow.h"
#include "XGUI_ViewPort.h"
#include "XGUI_Preferences.h"

#include <SUIT_ResourceMgr.h>

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QApplication>
#include <QMouseEvent>
#include <QMenu>

#include <V3d_View.hxx>

#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Drawer.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <V3d_View.hxx>
#include <Visual3d_View.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_Shape.hxx>

#ifdef WIN32
#include <WNT_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

XGUI_Viewer::InteractionStyle2StatesMap XGUI_Viewer::myStateMap;
XGUI_Viewer::InteractionStyle2ButtonsMap XGUI_Viewer::myButtonMap;
static bool isInitialized = false;

/*!
 Creates viewer 3d [ static ]
 */
Handle(V3d_Viewer) CreateViewer(const Standard_ExtString name, const Standard_CString displayName,
                                const Standard_CString domain, const Standard_Real viewSize,
                                const V3d_TypeOfOrientation viewProjection,
                                const Standard_Boolean computedMode,
                                const Standard_Boolean defaultComputedMode)
{
  static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
  if (aGraphicDriver.IsNull()) {
    Handle(Aspect_DisplayConnection) aDisplayConnection;
#ifndef WIN32
    aDisplayConnection = new Aspect_DisplayConnection( displayName );
#else
    aDisplayConnection = new Aspect_DisplayConnection();
#endif
    aGraphicDriver = Graphic3d::InitGraphicDriver(aDisplayConnection);
  }

  return new V3d_Viewer(aGraphicDriver, name, domain, viewSize, viewProjection, Quantity_NOC_GRAY30,
                        V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT, computedMode, defaultComputedMode,
                        V3d_TEX_NONE);
}

// VSR: Uncomment below line to allow texture background support in OCC viewer
#define OCC_ENABLE_TEXTURED_BACKGROUND

/*!
 Get data for supported background modes: gradient types, identifiers and supported image formats
 */
QString XGUI_Viewer::backgroundData(QStringList& gradList, QIntList& idList, QIntList& txtList)
{
  gradList << tr("Horizontal gradient") << tr("Vertical gradient") << tr("First diagonal gradient")
           << tr("Second diagonal gradient") << tr("First corner gradient")
           << tr("Second corner gradient") << tr("Third corner gradient")
           << tr("Fourth corner gradient");
  idList << XGUI::HorizontalGradient << XGUI::VerticalGradient << XGUI::Diagonal1Gradient
         << XGUI::Diagonal2Gradient << XGUI::Corner1Gradient << XGUI::Corner2Gradient
         << XGUI::Corner3Gradient << XGUI::Corner4Gradient;
#ifdef OCC_ENABLE_TEXTURED_BACKGROUND
  txtList << XGUI::CenterTexture << XGUI::TileTexture << XGUI::StretchTexture;
#endif
  return tr("Image files (*.bmp *.gif *.pix *.xwd *.rgb *.rs)");
}

XGUI_Viewer::XGUI_Viewer(XGUI_MainWindow* theParent, bool DisplayTrihedron)
    : QObject(theParent),
      myMainWindow(theParent),
      myPreselectionEnabled(true),
      mySelectionEnabled(true),
      myMultiSelectionEnabled(true),
      myIsRelative(true),
      myInteractionStyle(XGUI::STANDARD),
      myTrihedronSize(100),
      myActiveView(0),
      myWndIdCount(0)
{
  if (!isInitialized) {
    isInitialized = true;

    // standard interaction style
    XGUI_Viewer::myStateMap[XGUI::STANDARD][XGUI::ZOOM] = Qt::ControlModifier;
    XGUI_Viewer::myButtonMap[XGUI::STANDARD][XGUI::ZOOM] = Qt::LeftButton;

    XGUI_Viewer::myStateMap[XGUI::STANDARD][XGUI::PAN] = Qt::ControlModifier;
    XGUI_Viewer::myButtonMap[XGUI::STANDARD][XGUI::PAN] = Qt::MidButton;

    XGUI_Viewer::myStateMap[XGUI::STANDARD][XGUI::ROTATE] = Qt::ControlModifier;
    XGUI_Viewer::myButtonMap[XGUI::STANDARD][XGUI::ROTATE] = Qt::RightButton;

    XGUI_Viewer::myStateMap[XGUI::STANDARD][XGUI::FIT_AREA] = Qt::ControlModifier;
    XGUI_Viewer::myButtonMap[XGUI::STANDARD][XGUI::FIT_AREA] = Qt::RightButton;

    // "key free" interaction style
    XGUI_Viewer::myStateMap[XGUI::KEY_FREE][XGUI::ZOOM] = Qt::NoModifier;
    XGUI_Viewer::myButtonMap[XGUI::KEY_FREE][XGUI::ZOOM] = Qt::RightButton;

    XGUI_Viewer::myStateMap[XGUI::KEY_FREE][XGUI::PAN] = Qt::NoModifier;
    XGUI_Viewer::myButtonMap[XGUI::KEY_FREE][XGUI::PAN] = Qt::MidButton;

    XGUI_Viewer::myStateMap[XGUI::KEY_FREE][XGUI::ROTATE] = Qt::NoModifier;
    XGUI_Viewer::myButtonMap[XGUI::KEY_FREE][XGUI::ROTATE] = Qt::LeftButton;

    XGUI_Viewer::myStateMap[XGUI::KEY_FREE][XGUI::FIT_AREA] = Qt::NoModifier;  // unused
    XGUI_Viewer::myButtonMap[XGUI::KEY_FREE][XGUI::FIT_AREA] = Qt::NoButton;  // unused
  }

  // init CasCade viewers
  myV3dViewer = CreateViewer(TCollection_ExtendedString("Viewer3d").ToExtString(), "", "", 1000.0,
                             V3d_XposYnegZpos, Standard_True, Standard_True);
  myV3dViewer->SetDefaultLights();

  // init selector
  myAISContext = new AIS_InteractiveContext(myV3dViewer);
  myAISContext->SelectionColor(Quantity_NOC_WHITE);

  // display isoline on planar faces (box for ex.)
  myAISContext->IsoOnPlane(true);

  if (DisplayTrihedron) {
    Handle(Geom_Axis2Placement) anAxis = new Geom_Axis2Placement(gp::XOY());
    myTrihedron = new AIS_Trihedron(anAxis);
    myTrihedron->SetInfiniteState( Standard_True);

    Quantity_Color Col(193 / 255., 205 / 255., 193 / 255., Quantity_TOC_RGB);
    myTrihedron->SetArrowColor(Col.Name());
    myTrihedron->SetSize(myTrihedronSize);
    Handle(AIS_Drawer) drawer = myTrihedron->Attributes();
    if (drawer->HasDatumAspect()) {
      Handle(Prs3d_DatumAspect) daspect = drawer->DatumAspect();
      daspect->FirstAxisAspect()->SetColor(Quantity_Color(1.0, 0.0, 0.0, Quantity_TOC_RGB));
      daspect->SecondAxisAspect()->SetColor(Quantity_Color(0.0, 1.0, 0.0, Quantity_TOC_RGB));
      daspect->ThirdAxisAspect()->SetColor(Quantity_Color(0.0, 0.0, 1.0, Quantity_TOC_RGB));
    }
  }
  // set zooming style to standard
  //myZoomingStyle = 0;

  QMdiArea* aMDI = myMainWindow->mdiArea();
  connect(aMDI, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
          SLOT(onWindowActivated(QMdiSubWindow*)));

}

XGUI_Viewer::~XGUI_Viewer(void)
{
  myAISContext.Nullify();
  myV3dViewer.Nullify();
}

QMdiSubWindow* XGUI_Viewer::createView(V3d_TypeOfView theType)
{
  // create view frame
  XGUI_ViewWindow* view = new XGUI_ViewWindow(this, theType);
  // get main view window (created by view frame)
  //OCCViewer_ViewWindow* vw = view->getView(OCCViewer_ViewFrame::MAIN_VIEW);
  // initialize main view window
  //initView( vw );
  // set default background for view window
  //vw->setBackground( background(0) ); // 0 means MAIN_VIEW (other views are not yet created here)
  // connect signal from viewport
  //connect(view->viewPort(), SIGNAL(vpClosed()), this, SLOT(onViewClosed()));
  //connect(view->viewPort(), SIGNAL(vpMapped()), this, SLOT(onViewMapped()));
  if (myViews.size() == 0)
    setTrihedronShown(true);

  Qtx::BackgroundData aBk = XGUI_Preferences::resourceMgr()->backgroundValue("Viewer",
                                                                             "background");
  view->setBackground(aBk);
  view->updateEnabledDrawMode();

  QMdiArea* aMDI = myMainWindow->mdiArea();
  QMdiSubWindow* aWnd = aMDI->addSubWindow(view, Qt::FramelessWindowHint);
  addView(aWnd);
  aWnd->setGeometry(0, 0, aMDI->width() / 2, aMDI->height() / 2);
  aWnd->show();
  aWnd->setWindowTitle(QString("Viewer #%1").arg(++myWndIdCount));
  emit viewCreated(view);
  return aWnd;
}

void XGUI_Viewer::updateFromResources()
{
  Qtx::BackgroundData aBk = XGUI_Preferences::resourceMgr()->backgroundValue("Viewer",
                                                                             "background");
  foreach (QMdiSubWindow* aWnd, myViews)
  {
    XGUI_ViewWindow* aView = dynamic_cast<XGUI_ViewWindow*>(aWnd->widget());
    if (aView)
      aView->setBackground(aBk);
  }
}

XGUI_ViewWindow* XGUI_Viewer::activeViewWindow() const
{
  if (myActiveView)
    return dynamic_cast<XGUI_ViewWindow*>(myActiveView->widget());
  return 0;
}

void XGUI_Viewer::getSelectedObjects(AIS_ListOfInteractive& theList)
{
  theList.Clear();
  for (myAISContext->InitSelected(); myAISContext->MoreSelected(); myAISContext->NextSelected())
    theList.Append(myAISContext->SelectedInteractive());
}

void XGUI_Viewer::getSelectedShapes(NCollection_List<TopoDS_Shape>& theList)
{
  Handle(AIS_InteractiveContext) ic = AISContext();

  for (ic->InitSelected(); ic->MoreSelected(); ic->NextSelected()) {
    TopoDS_Shape aShape = ic->SelectedShape();
    if (!aShape.IsNull())
      theList.Append(aShape);
  }
}

void XGUI_Viewer::setObjectsSelected(const AIS_ListOfInteractive& theList)
{
  AIS_ListIteratorOfListOfInteractive aIt;
  for (aIt.Initialize(theList); aIt.More(); aIt.Next())
    myAISContext->AddOrRemoveSelected(aIt.Value(), false);
  myAISContext->UpdateCurrentViewer();
}

/*! Sets hot button
 *\param theOper - hot operation
 *\param theState - adding state to state map operations.
 *\param theButton - adding state to button map operations.
 */
void XGUI_Viewer::setHotButton(XGUI::InteractionStyle theInteractionStyle,
                               XGUI::HotOperation theOper, Qt::KeyboardModifiers theState,
                               Qt::MouseButtons theButton)
{
  myStateMap[theInteractionStyle][theOper] = theState;
  myButtonMap[theInteractionStyle][theOper] = theButton;
}

/*! Gets hot button for operation \a theOper.
 *\param theOper - input hot operation
 *\param theState - output state from state map operations.
 *\param theButton - output state from button map operations.
 */
void XGUI_Viewer::getHotButton(XGUI::InteractionStyle theInteractionStyle,
                               XGUI::HotOperation theOper, Qt::KeyboardModifiers& theState,
                               Qt::MouseButtons& theButton)
{
  theState = myStateMap[theInteractionStyle][theOper];
  theButton = myButtonMap[theInteractionStyle][theOper];
}

/*!
 Changes visibility of trihedron to opposite
 */
void XGUI_Viewer::toggleTrihedron()
{
  setTrihedronShown(!isTrihedronVisible());
}

/*!
 \return true if trihedron is visible
 */
bool XGUI_Viewer::isTrihedronVisible() const
{
  return !myTrihedron.IsNull() && !myAISContext.IsNull() && myAISContext->IsDisplayed(myTrihedron);
}

/*!
 Sets visibility state of trihedron
 \param on - new state
 */

void XGUI_Viewer::setTrihedronShown(bool on)
{
  if (myTrihedron.IsNull())
    return;

  if (on) {
    myAISContext->Display(myTrihedron);
    myAISContext->Deactivate(myTrihedron);
  } else {
    myAISContext->Erase(myTrihedron);
  }
}

/*!
 \return trihedron size
 */
double XGUI_Viewer::trihedronSize() const
{
  double sz = 0;
  if (!myTrihedron.IsNull())
    sz = myTrihedron->Size();
  return sz;
}

/*!
 Changes trihedron size
 \param sz - new size
 */
void XGUI_Viewer::setTrihedronSize(const double sz, bool isRelative)
{
  if (myTrihedronSize != sz || isRelative != myIsRelative) {
    myTrihedronSize = sz;
    myIsRelative = isRelative;
    updateTrihedron();
  }
}

/*! 
 * Update the size of the trihedron
 */
void XGUI_Viewer::updateTrihedron()
{
  if (myTrihedron.IsNull())
    return;

  if (myIsRelative) {
    double newSz, oldSz;

    if (computeTrihedronSize(newSz, oldSz))
      myTrihedron->SetSize(newSz);

  } else if (myTrihedron->Size() != myTrihedronSize) {
    myTrihedron->SetSize(myTrihedronSize);
  }
}

/*!
 Get new and current trihedron size corresponding to the current model size
 */
bool XGUI_Viewer::computeTrihedronSize(double& theNewSize, double& theSize)
{
  theNewSize = 100;
  theSize = 100;

  //SRN: BUG IPAL8996, a usage of method ActiveView without an initialization
  Handle(V3d_Viewer) viewer = v3dViewer();
  viewer->InitActiveViews();
  if (!viewer->MoreActiveViews())
    return false;

  Handle(V3d_View) view3d = viewer->ActiveView();
  //SRN: END of fix

  if (view3d.IsNull())
    return false;

  double Xmin = 0, Ymin = 0, Zmin = 0, Xmax = 0, Ymax = 0, Zmax = 0;
  double aMaxSide;

  view3d->View()->MinMaxValues(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);

  if (Xmin == RealFirst() || Ymin == RealFirst() || Zmin == RealFirst() || Xmax == RealLast()
      || Ymax == RealLast() || Zmax == RealLast())
    return false;

  aMaxSide = Xmax - Xmin;
  if (aMaxSide < Ymax - Ymin)
    aMaxSide = Ymax - Ymin;
  if (aMaxSide < Zmax - Zmin)
    aMaxSide = Zmax - Zmin;

  // IPAL21687
  // The boundary box of the view may be initialized but nullified
  // (case of infinite objects)
  if (aMaxSide < Precision::Confusion())
    return false;

  static float EPS = (float) 5.0E-3;
  theSize = trihedron()->Size();
  //theNewSize = aMaxSide*aSizeInPercents / 100.0;

  return fabs(theNewSize - theSize) > theSize * EPS || fabs(theNewSize - theSize) > theNewSize * EPS;
}

void XGUI_Viewer::onViewClosed(QMdiSubWindow* theView)
{
  if (!theView)
    return;

  emit deleteView(static_cast<XGUI_ViewWindow*>(theView->widget()));
  removeView(theView);

  // if this is last view
  if (myViews.size() == 0) {
    Standard_Integer aViewsNb = 0;
    for (myV3dViewer->InitActiveViews(); myV3dViewer->MoreActiveViews();
        myV3dViewer->NextActiveViews())
      ++aViewsNb;
    if (aViewsNb < 2) {
      //clean up presentations before last view is closed
      myAISContext->RemoveAll(Standard_False);
    }
  }
}

/*!Remove view window \a theView from view manager.
 *And close the last view, if it has \a theView.
 */
void XGUI_Viewer::removeView(QMdiSubWindow* theView)
{
  XGUI_ViewWindow* aWindow = static_cast<XGUI_ViewWindow*>(theView->widget());

  aWindow->disconnect(this);
  myViews.removeAt(myViews.indexOf(theView));
  if (myActiveView == theView)
    myActiveView = 0;
  if (myViews.size() == 0)
    emit lastViewClosed();
}

/*void XGUI_Viewer::onViewMapped()
 {
 setTrihedronShown(true);
 }*/

void XGUI_Viewer::addView(QMdiSubWindow* theView)
{
  XGUI_ViewWindow* aWindow = dynamic_cast<XGUI_ViewWindow*>(theView->widget());

  connect(aWindow, SIGNAL(closed(QMdiSubWindow*)), this, SLOT(onViewClosed(QMdiSubWindow*)));

  connect(aWindow, SIGNAL(tryClosing(XGUI_ViewWindow*)), this,
          SIGNAL(tryCloseView(XGUI_ViewWindow*)));

  connect(aWindow, SIGNAL(mousePressed(XGUI_ViewWindow*, QMouseEvent*)), this,
          SLOT(onMousePressed(XGUI_ViewWindow*, QMouseEvent*)));

  connect(aWindow, SIGNAL(mouseDoubleClicked(XGUI_ViewWindow*, QMouseEvent*)), this,
          SIGNAL(mouseDoubleClick(XGUI_ViewWindow*, QMouseEvent*)));

  connect(aWindow, SIGNAL(mouseMoving(XGUI_ViewWindow*, QMouseEvent*)), this,
          SIGNAL(mouseMove(XGUI_ViewWindow*, QMouseEvent*)));

  connect(aWindow, SIGNAL(keyPressed(XGUI_ViewWindow*, QKeyEvent*)), this,
          SIGNAL(keyPress(XGUI_ViewWindow*, QKeyEvent*)));

  connect(aWindow, SIGNAL(keyReleased(XGUI_ViewWindow*, QKeyEvent*)), this,
          SLOT(onKeyRelease(XGUI_ViewWindow*, QKeyEvent*)));

  //connect(aWindow, SIGNAL(contextMenuRequested( QContextMenuEvent* )),
  //        this,    SLOT  (onContextMenuRequested( QContextMenuEvent* )));
  connect(aWindow, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this,
          SIGNAL(contextMenuRequested(QContextMenuEvent*)));

  connect(aWindow, SIGNAL(mouseMoving(XGUI_ViewWindow*, QMouseEvent*)), this,
          SLOT(onMouseMove(XGUI_ViewWindow*, QMouseEvent*)));

  connect(aWindow, SIGNAL(mouseReleased(XGUI_ViewWindow*, QMouseEvent*)), this,
          SLOT(onMouseReleased(XGUI_ViewWindow*, QMouseEvent*)));

  myViews.append(theView);
}

/*!
 Emit activated for view \a view.
 */
void XGUI_Viewer::onWindowActivated(QMdiSubWindow* view)
{
  if (view && (view != myActiveView) && (!view->isMinimized())) {
    myActiveView = view;
    ((XGUI_ViewWindow*) myActiveView->widget())->windowActivated();
    QList<QMdiSubWindow*>::iterator aIt;
    for (aIt = myViews.begin(); aIt != myViews.end(); ++aIt) {
      if ((*aIt) != myActiveView) {
        ((XGUI_ViewWindow*) (*aIt)->widget())->windowDeactivated();
      }
    }
  }
}

void XGUI_Viewer::onWindowMinimized(QMdiSubWindow* theWnd)
{
  if (myActiveView == theWnd) {
    myActiveView = 0;
    QList<QMdiSubWindow*>::iterator aIt;
    for (aIt = myViews.begin(); aIt != myViews.end(); ++aIt) {
      if (!(*aIt)->widget()->isMinimized()) {
        (*aIt)->raise();
        onWindowActivated(*aIt);
        break;
      }
    }
  }
}

/*!
 SLOT: called on mouse button press, stores current mouse position as start point for transformations
 */
void XGUI_Viewer::onMousePressed(XGUI_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  myStartPnt.setX(theEvent->x());
  myStartPnt.setY(theEvent->y());
  emit mousePress(theWindow, theEvent);
}

/*!
 SLOT: called on mouse move, processes hilighting
 */
void XGUI_Viewer::onMouseMove(XGUI_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  myCurPnt.setX(theEvent->x());
  myCurPnt.setY(theEvent->y());
  if (!mySelectionEnabled)
    return;

  Handle(V3d_View) aView3d = theWindow->viewPort()->getView();
  if (!aView3d.IsNull()) {
    myAISContext->MoveTo(theEvent->x(), theEvent->y(), aView3d);
    if (myAISContext->HasDetected())
      theWindow->viewPort()->setFocus(Qt::MouseFocusReason);
  }
}

/*!
 SLOT: called on mouse button release, finishes selection
 */
void XGUI_Viewer::onMouseReleased(XGUI_ViewWindow* theWindow, QMouseEvent* theEvent)
{
  if (!mySelectionEnabled || theEvent->button() != Qt::LeftButton) {
    emit mouseRelease(theWindow, theEvent);
    return;
  }

  myEndPnt.setX(theEvent->x());
  myEndPnt.setY(theEvent->y());
  bool aHasShift = (theEvent->modifiers() & Qt::ShiftModifier);

  //if (!aHasShift) 
  //  emit deselection();

  if (myStartPnt == myEndPnt) {
    // the MoveTo is necessary for the second click in the same point. Otherwise the selection is lost.
    //Handle(V3d_View) aView3d = theWindow->viewPort()->getView();
    //if (!aView3d.IsNull()) {
    //  myAISContext->MoveTo(theEvent->x(), theEvent->y(), aView3d);
    //}
    if (aHasShift && myMultiSelectionEnabled)
      myAISContext->ShiftSelect();
    else
      myAISContext->Select();
  } else {
    if (aHasShift && myMultiSelectionEnabled)
      myAISContext->ShiftSelect(myStartPnt.x(), myStartPnt.y(), myEndPnt.x(), myEndPnt.y(),
                                theWindow->viewPort()->getView(), false);
    else
      myAISContext->Select(myStartPnt.x(), myStartPnt.y(), myEndPnt.x(), myEndPnt.y(),
                           theWindow->viewPort()->getView(), false);

    int Nb = myAISContext->NbSelected();
    if (Nb > 1 && !myMultiSelectionEnabled) {
      myAISContext->InitSelected();
      Handle( SelectMgr_EntityOwner ) anOwner = myAISContext->SelectedOwner();
      if (!anOwner.IsNull()) {
        myAISContext->ClearSelected( Standard_False);
        myAISContext->AddOrRemoveSelected(anOwner, Standard_False);
      }
    }

    myAISContext->UpdateCurrentViewer();
  }
  emit mouseRelease(theWindow, theEvent);
  emit selectionChanged();
}

//******************************************************
void XGUI_Viewer::setMultiSelectionEnabled(bool toEnable)
{
  myMultiSelectionEnabled = toEnable;
  updateViewsDrawMode();
}

//******************************************************
void XGUI_Viewer::setSelectionEnabled(bool toEnable)
{
  mySelectionEnabled = toEnable;
  updateViewsDrawMode();
}

//******************************************************
void XGUI_Viewer::updateViewsDrawMode() const
{
  foreach(QMdiSubWindow* aWnd, myViews)
  {
    XGUI_ViewWindow* aView = static_cast<XGUI_ViewWindow*>(aWnd->widget());
    aView->updateEnabledDrawMode();
  }
}

//******************************************************
void XGUI_Viewer::onKeyRelease(XGUI_ViewWindow* theView, QKeyEvent* theKey)
{
  Handle(V3d_View) aView = theView->viewPort()->getView();
  bool noModifiers = (theKey->modifiers() == Qt::NoModifier);
  if ((theKey->key() == Qt::Key_N) && noModifiers) {
    myAISContext->HilightNextDetected(aView);
  } else if ((theKey->key() == Qt::Key_P) && noModifiers) {
    myAISContext->HilightPreviousDetected(aView);
  } else {
    emit keyRelease(theView, theKey);
  }
}

//******************************************************
//void XGUI_Viewer::onContextMenuRequested(QContextMenuEvent* theEvent)
//{
//  XGUI_ViewWindow* aWnd = dynamic_cast<XGUI_ViewWindow*>(sender());
//  if (!aWnd) return;
//
//  QMenu aMenu;
//
//  // Include Viewer actions
//  if (myActions.size() > 0) {
//    aMenu.addActions(myActions);
//    aMenu.addSeparator();
//  }
//  if (aWnd->actions().size() > 0) {
//    aMenu.addActions(aWnd->actions());
//    aMenu.addSeparator();
//  }
//
//  QMdiArea* aMDI = myMainWindow->mdiArea();
//  if (aMenu.actions().size() > 0) {
//    QMenu* aSubMenu = aMenu.addMenu(tr("Windows"));
//    aSubMenu->addActions(aMDI->actions());
//  } else {
//    aMenu.addActions(aMDI->actions());
//  }
//  aMenu.exec(theEvent->globalPos());
//}
