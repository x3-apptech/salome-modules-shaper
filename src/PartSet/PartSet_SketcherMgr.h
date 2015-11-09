// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        PartSet_SketcherMgr.h
// Created:     19 Dec 2014
// Author:      Vitaly SMETANNIKOV

#ifndef PartSet_SketcherMgr_H
#define PartSet_SketcherMgr_H

#include "PartSet.h"

#include <ModelAPI_Feature.h>
#include <ModelAPI_Attribute.h>
#include <ModelAPI_CompositeFeature.h>
#include <ModelAPI_Result.h>

#include <ModuleBase_ViewerFilters.h>
#include <ModuleBase_Definitions.h>
#include <ModuleBase_ModelWidget.h>

#include <GeomAPI_Pln.h>
#include <SelectMgr_IndexedMapOfOwner.hxx>

#include <QObject>
#include <QList>
#include <QMap>

class PartSet_Module;
class ModuleBase_IViewWindow;
class ModuleBase_ModelWidget;
class ModuleBase_Operation;
class XGUI_OperationMgr;
class QMouseEvent;

/**
* \ingroup Modules
* A class for management of sketch operations
  At the time of the sketcher operation active, only the sketch sub-feature results are
  displayed in the viewer. After the sketch create/edit operation is finished, the sub-feature
  are hidden, the sketch feature result is displayed
*/
class PARTSET_EXPORT PartSet_SketcherMgr : public QObject
{
  Q_OBJECT
  /// Struct to define gp point, with the state is the point is initialized
  struct Point
  {
    /// Constructor
    Point()
    {
      myIsInitialized = false;
    }
    /// Destructor
    ~Point()
    {
    }

    /// clear the initialized flag.
    void clear()
    {
      myIsInitialized = false;
    }
    /// set the point and switch on the initialized flag
    /// \param thePoint the point
    void setValue(const double theX, const double theY)
    {
      myIsInitialized = true;
      myCurX = theX;
      myCurY = theY;
    }

    bool myIsInitialized;  /// the state whether the point is set
    double myCurX, myCurY; /// the point coordinates
  };
public:
  /// Constructor
  /// \param theModule a pointer to PartSet module
  PartSet_SketcherMgr(PartSet_Module* theModule);

  virtual ~PartSet_SketcherMgr();

  /// Returns true if the operation is the sketch
  /// \param theOperation an operation
  /// \return the boolean result
  static bool isSketchOperation(ModuleBase_Operation* theOperation);

  /// Returns true if the operation id is in the sketch operation id list
  /// \param theOperation an operation
  /// \return the boolean result
  static bool isNestedSketchOperation(ModuleBase_Operation* theOperation);

  /// Returns true if the operation is a create nested feature one
  /// \param theOperation a checked operation
  //// \return boolean value
  static bool isNestedCreateOperation(ModuleBase_Operation* theOperation);

  /// Returns whether the current operation is a sketch entity - line, point, arc or circle
  /// \param theId is an id of object
  /// \return a boolean value
  static bool isEntity(const std::string& theId);

  /// Returns whether the current operation is a sketch distance - lenght, distance or radius
  /// \param theOperation the operation
  /// \return a boolean value
  static bool isDistanceOperation(ModuleBase_Operation* theOperation);

  /// Returns true if a mouse cursor is over viewer window
  bool isMouseOverWindow() { return myIsMouseOverWindow; }

  /// Returns current Sketch feature/ Returns NULL if there is no launched sketch operation
  CompositeFeaturePtr activeSketch() const { return myCurrentSketch; }

  /// Starts sketch operation
  void startSketch(ModuleBase_Operation* );

  /// Stops sketch operation
  void stopSketch(ModuleBase_Operation* );

  /// Starts sketch operation, connects to the opeation property panel
  /// \param theOperation a committed operation
  void startNestedSketch(ModuleBase_Operation* theOperation);

  /// Stop sketch operation, disconnects from the opeation property panel
  /// \param theOperation a stopped operation
  void stopNestedSketch(ModuleBase_Operation* theOperation);

  /// Visualizes the operation feature if it is a creation nested feature operation
  /// \param theOperation a committed operation
  void commitNestedSketch(ModuleBase_Operation* theOperation);

  /// Returns True if there are available Undos and the sketch manager allows undo
  /// \return the boolean result
  bool canUndo() const;

  //! Returns True if there are available Redos and the sketch manager allows redo
  /// \return the boolean result
  bool canRedo() const;

  /// Returns False only if the sketch creating feature can not be visualized.
  /// \return a boolean value
  //bool canCommitOperation() const;

  /// Returns whether the object can be erased at the bounds of the active operation.
  /// Sketch sub-entities can not be erased during the sketch operation
  /// \param theObject a model object
  bool canEraseObject(const ObjectPtr& theObject) const;

  /// Returns whether the object can be displayed at the bounds of the active operation.
  /// Display only current operation results for usual operation and ask the sketcher manager
  /// if it is a sketch operation
  /// \param theObject a model object
  bool canDisplayObject(const ObjectPtr& theObject) const;

  /// Returns true if the mouse is over viewer or property panel value is changed
  /// \return boolean result
  bool canDisplayCurrentCreatedFeature() const;

  /// Returns true if the current operation is nested creation or internal reentrant edit
  /// \param theOperation an operation
  bool canChangeCursor(ModuleBase_Operation* theOperation) const;

  /// Returns state of constraints showing flag 
  bool isConstraintsShown() const { return myIsConstraintsShown; }

  /// Returns true if the object is a current sketch sub feature of a result of the feature
  /// \param theObject an object
  /// \return boolean value
  bool isObjectOfSketch(const ObjectPtr& theObject) const;

  /// Saves the current selection in the viewer into an internal container
  /// It obtains the selected attributes. The highlighted objects can be processes as the selected ones
  /// \param theHighlightedOnly a boolean flag
  void storeSelection(const bool theHighlightedOnly = false);

  /// Restores previously saved selection state
  void restoreSelection();

  /// Return error state of the sketch feature, true if the error has happened
  /// \return boolean value
  bool sketchSolverError();

  //! Returns the feature error if the current state of the feature in the sketch is not correct
  //! If the feature is correct, it returns an empty value
  //! Incorrect states: the feature is sketch, the solver error value
  //! The feature value is reset, this is the flag of sketch mgr
  //! \return string value
  QString getFeatureError(const FeaturePtr& theFeature, const bool isCheckGUI = true);

  /// It nullify internal flags concerned to clicked mouse event
  void clearClickedFlags();

  /// Returns list of strings which contains id's of sketch operations
  static const QStringList& sketchOperationIdList();

  /// Returns list of strings which contains id's of constraints operations
  static const QStringList& constraintsIdList();

  /// Returns a list of modes, where the AIS objects should be activated
  /// \param theModes a list of modes
  static void sketchSelectionModes(QIntList& theModes);

  /// Connects or disconnects to the value changed signal of the property panel widgets
  /// \param theWidget a property contol widget
  /// \param isToConnect a boolean value whether connect or disconnect
  void connectToPropertyPanel(ModuleBase_ModelWidget* theWidget, const bool isToConnect);

  /// Visualize the operation feature if the previous state is modified value in property panel
  /// \param thePreviousState the previous widget value state
  void widgetStateChanged(int thePreviousState);

public slots:
  /// Process sketch plane selected event
  void onPlaneSelected(const std::shared_ptr<GeomAPI_Pln>& thePln);

  /// Toggle show constraints
  void onShowConstraintsToggle(bool);

private slots:
  /// Process the enter mouse to the view port. If the current operation is a create of
  /// a nested sketch feature, it updates internal flags to display the feature on mouse move
  void onEnterViewPort();
  /// Process the leave mouse of the view port. If the current operation is a create of
  /// a nested sketch feature, it hides the feature in the viewer
  void onLeaveViewPort();
  /// Listens to the value changed signal and display the current operation feature
  void onBeforeValuesChangedInPropertyPanel();
  /// Listens to the signal about the modification of the values have been done in the property panel
  void onAfterValuesChangedInPropertyPanel();

  void onMousePressed(ModuleBase_IViewWindow*, QMouseEvent*);
  void onMouseReleased(ModuleBase_IViewWindow*, QMouseEvent*);
  void onMouseMoved(ModuleBase_IViewWindow*, QMouseEvent*);
  void onMouseDoubleClick(ModuleBase_IViewWindow*, QMouseEvent*);
  void onApplicationStarted();
  void onBeforeWidgetActivated(ModuleBase_ModelWidget* theWidget);

  void onBeforeContextMenu();
  void onAfterContextMenu();

private:
  /// Launches the operation from current highlighting
  void launchEditing();

  /// Converts mouse position to 2d coordinates. 
  /// Member myCurrentSketch has to be correctly defined
  void get2dPoint(ModuleBase_IViewWindow* theWnd, QMouseEvent* theEvent, 
                  Point& thePoint);

  typedef QMap<FeaturePtr, std::pair<std::set<AttributePtr>, std::set<ResultPtr> > >
                                                                       FeatureToSelectionMap;

  /// Obtains the current selection of the object in the workshop viewer 
  /// It includes the selection in all modes of activation, even local context - vertices, edges
  /// It gets all results of the feature, find an AIS object in the viewer and takes all BRep
  /// selection owners. If the owner is vertex, the corresponded attribute is seached in
  /// the feature, if the owner is edge, the current result is added to the container of results.
  /// \param theFeature a feature or result object
  /// \param theSketch a current sketch feature
  /// \param theWorkshop a workshop to have an access to AIS context and displayer
  /// \param theSelection a container for the selection, to save results and attributres for a feature
  static void getCurrentSelection(const FeaturePtr& theFeature,
                                  const FeaturePtr& theSketch,
                                  ModuleBase_IWorkshop* theWorkshop,
                                  FeatureToSelectionMap& theSelection);

  /// Applyes the current selection to the object in the workshop viewer 
  /// It includes the selection in all modes of activation, even local context - vertexes, edges
  /// It gets all results of the feature, find an AIS object in the viewer and takes all BRep
  /// selection owners. If the owner is vertex, the corresponded attribute is seached in
  /// the feature and if it is in the container of selected attributes, the owner is put in the
  /// out container. If the owner is edge and the current result is in the container of selected
  /// results, the owner is put in the out container.
  /// \param theFeature a feature or result object
  /// \param theSketch a current sketch feature
  /// \param theWorkshop a workshop to have an access to AIS context and displayer
  /// \param theSelection a container of the selection, it has results and attributres for a feature
  /// \param theOwnersToSelect an out container of found owners
  static void getSelectionOwners(const FeaturePtr& theFeature,
                                  const FeaturePtr& theSketch,
                                  ModuleBase_IWorkshop* theWorkshop,
                                  const FeatureToSelectionMap& theSelection,
                                  SelectMgr_IndexedMapOfOwner& anOwnersToSelect);

  /// Returns true if the created feature is visible
  /// \param 
  bool isVisibleCreatedFeature() const;

  /// Returns a current operation
  /// \return an operation
  ModuleBase_Operation* getCurrentOperation() const;

  /// Get the active widget in the property panel
  ModuleBase_ModelWidget* getActiveWidget() const;

  /// Erase or display the feature of the current operation. If the mouse over the active view or
  /// a current value is changed by property panel, the feature is displayed otherwise it is hidden
  /// \param theOperation an operation which feature is to be displayed, it is nested create operation
  /// \param isToDisplay a flag about the display or erase the feature
  void visualizeFeature(const FeaturePtr& theFeature, const bool isEditOperation,
                        const bool isToDisplay, const bool isFlushRedisplay = true);

private:
  XGUI_OperationMgr* operationMgr() const;

private:
  PartSet_Module* myModule;

  bool myPreviousDrawModeEnabled; // the previous selection enabled state in the viewer
  bool myIsDragging;
  bool myDragDone;
  bool myIsMouseOverWindow; /// the state that the mouse over the view
  bool myIsMouseOverViewProcessed; /// the state whether the over view state is processed by mouseMove method
  bool myIsPopupMenuActive; /// the state of the popup menu is shown
  Point myCurrentPoint;
  Point myClickedPoint;

  CompositeFeaturePtr myCurrentSketch;

  Handle(ModuleBase_ShapeInPlaneFilter) myPlaneFilter;
  FeatureToSelectionMap myCurrentSelection;
  bool myPreviousUpdateViewerEnabled;

  bool myIsConstraintsShown;
};


#endif