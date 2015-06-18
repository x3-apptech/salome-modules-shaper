﻿// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        ModuleBase_WidgetShapeSelector.h
// Created:     2 June 2014
// Author:      Vitaly Smetannikov

#ifndef ModuleBase_WidgetShapeSelector_H
#define ModuleBase_WidgetShapeSelector_H

#include "ModuleBase.h"
#include "ModuleBase_WidgetValidated.h"
#include "ModuleBase_ViewerFilters.h"
#include <ModuleBase_ViewerPrs.h>

#include <ModelAPI_Object.h>
#include <ModelAPI_Attribute.h>
#include <GeomAPI_Shape.h>

#include <TopAbs_ShapeEnum.hxx>

#include <QStringList>

class Config_WidgetAPI;
class QWidget;
class QLabel;
class QLineEdit;
class QToolButton;
class ModuleBase_IWorkshop;
class ModelAPI_Validator;
class GeomValidators_ShapeType;

/**
* \ingroup GUI
* Implementation of widget for shapes selection.
* This type of widget can be defined in XML file with 'shape_selector' keyword.
* For example:
* \code
*   <shape_selector id="main_object" 
*    label="Main object" 
*    icon=":icons/cut_shape.png" 
*    tooltip="Select an object solid"
*    shape_types="solid shell"
*    concealment="true"
*  />
* \endcode
* It can use following parameters:
* - id - name of object attribute
* - label - content of widget's label
* - icon - icon which can be used instead label
* - tooltip - the witget's tooltip text
* - shape_types - list of shape types for selection. 
*       Possible values: face, vertex, wire, edge, shell, solid
* - object_types - types of selectable objects. 
*       For today it supports only one type "construction" 
*        which corresponds to ModelAPI_ResultConstruction object type
* - concealment - hide or not hide selected object after operation
*/
class MODULEBASE_EXPORT ModuleBase_WidgetShapeSelector : public ModuleBase_WidgetValidated
{
Q_OBJECT
 public:

  /// Constructor
  /// \param theParent the parent object
  /// \param theWorkshop instance of workshop interface
  /// \param theData the widget configuation. The attribute of the model widget is obtained from
  /// \param theParentId is Id of a parent of the current attribute
  ModuleBase_WidgetShapeSelector(QWidget* theParent, ModuleBase_IWorkshop* theWorkshop,
                            const Config_WidgetAPI* theData, const std::string& theParentId);

  virtual ~ModuleBase_WidgetShapeSelector();

  virtual bool restoreValue();

  /// Defines if it is supposed that the widget should interact with the viewer.
  virtual bool isViewerSelector() { return true; }

  /// Returns list of widget controls
  /// \return a control list
  virtual QList<QWidget*> getControls() const;

  /// Checks the widget validity. By default, it returns true.
  /// \param theValue a selected presentation in the view
  /// \return a boolean value
  virtual bool isValidSelectionCustom(const ModuleBase_ViewerPrs& thePrs);

  /// Fills the attribute with the value of the selected owner
  /// \param theOwner a selected owner
  virtual bool setSelectionCustom(const ModuleBase_ViewerPrs& thePrs);

  /// The methiod called when widget is deactivated
  virtual void deactivate();

protected:
  /// Activate or deactivate selection
  void activateSelection(bool toActivate);

 private slots:
   /// Slot which is called on selection event
  void onSelectionChanged();

 protected:
  /// Saves the internal parameters to the given feature
  /// \return True in success
  virtual bool storeValueCustom() const;

  /// The methiod called when widget is activated
  virtual void activateCustom();

  /// Creates a backup of the current values of the attribute
  /// It should be realized in the specific widget because of different
  /// parameters of the current attribute
  virtual void storeAttributeValue();

  /// Creates a backup of the current values of the attribute
  /// It should be realized in the specific widget because of different
  /// parameters of the current attribute
  /// \param theValid a boolean flag, if restore happens for valid parameters
  virtual void restoreAttributeValue(const bool theValid);

  /// Computes and updates name of selected object in the widget
  void updateSelectionName();

  /// Raise panel which contains this widget
  void raisePanel() const;

  /// Returns true if selected shape corresponds to requested shape types
  /// \param theShape a shape
  bool acceptSubShape(const TopoDS_Shape& theShape) const;

  /// Clear attribute
  void clearAttribute();

  /// Store the values to the model attribute of the widget. It casts this attribute to
  /// the specific type and set the given values
  /// \param theSelectedObject an object
  /// \param theShape a selected shape, which is used in the selection attribute
  /// \return true if it is succeed
  virtual void setObject(ObjectPtr theSelectedObject, GeomShapePtr theShape);

  /// Get the shape from the attribute if the attribute contains a shape, e.g. selection attribute
  /// \return a shape
  virtual GeomShapePtr getShape() const;

  /// Return the attribute values wrapped in a list of viewer presentations
  /// \return a list of viewer presentations, which contains an attribute result and
  /// a shape. If the attribute do not uses the shape, it is empty
  QList<ModuleBase_ViewerPrs> getAttributeSelection() const;

  /// Return an object and geom shape by the viewer presentation
  /// \param thePrs a selection
  /// \param theObject an output object
  /// \param theShape a shape of the selection
  virtual void getGeomSelection(const ModuleBase_ViewerPrs& thePrs,
                                ObjectPtr& theObject,
                                GeomShapePtr& theShape);

  //----------- Class members -------------
  protected:
  /// Label of the widget
  QLabel* myLabel;

  /// Input control of the widget
  QLineEdit* myTextLine;

  /// Reference to workshop
  ModuleBase_IWorkshop* myWorkshop;

  /// List of accepting shapes types
  QStringList myShapeTypes;

  /// backup parameters of the model attribute. The class processes three types of attribute:
  /// Reference, RefAttr and Selection. Depending on the attribute type, only the attribute parameter
  /// values are reserved in the backup
  /// An attribute object
  ObjectPtr myObject;
  /// An attribute shape
  GeomShapePtr myShape;
  /// A reference of the attribute
  AttributePtr myRefAttribute;
  /// A boolean value whether refAttr uses reference of object
  bool myIsObject;

  /// An instance of the "shape_type" validator. It is returns on validating in customValidator()
  GeomValidators_ShapeType* myShapeValidator;
};

#endif
