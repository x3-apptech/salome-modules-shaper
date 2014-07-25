// File:        ModuleBase_Widgets.h
// Created:     04 June 2014
// Author:      Vitaly Smetannikov

#include <ModuleBase_WidgetBoolValue.h>

#include <ModelAPI_AttributeBoolean.h>
#include <ModelAPI_Data.h>

#include <Config_Keywords.h>
#include <Config_WidgetAPI.h>

#include <Events_Loop.h>
#include <ModelAPI_Events.h>

#include <QWidget>
#include <QLayout>
#include <QCheckBox>

ModuleBase_WidgetBoolValue::ModuleBase_WidgetBoolValue(QWidget* theParent, 
  const Config_WidgetAPI* theData, 
  const std::string& theParentId)
  : ModuleBase_ModelWidget(theParent, theData, theParentId)
{
  QString aText = QString::fromStdString(theData->widgetLabel());
  QString aToolTip = QString::fromStdString(theData->widgetTooltip());
  QString aDefault = QString::fromStdString(theData->getProperty("default"));

  myCheckBox = new QCheckBox(aText, theParent);
  myCheckBox->setToolTip(aToolTip);
  myCheckBox->setChecked(aDefault == "true");

  connect(myCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(valuesChanged()));
}

ModuleBase_WidgetBoolValue::~ModuleBase_WidgetBoolValue()
{
}

QWidget* ModuleBase_WidgetBoolValue::getControl() const 
{ 
  return myCheckBox; 
}

bool ModuleBase_WidgetBoolValue::storeValue(ObjectPtr theObject) const
{
  DataPtr aData = theObject->data();
  boost::shared_ptr<ModelAPI_AttributeBoolean> aBool = aData->boolean(attributeID());

  if (aBool->value() != myCheckBox->isChecked()) {
    aBool->setValue(myCheckBox->isChecked());
    updateObject(theObject);
  }
  return true;
}

bool ModuleBase_WidgetBoolValue::restoreValue(ObjectPtr theObject)
{
  DataPtr aData = theObject->data();
  boost::shared_ptr<ModelAPI_AttributeBoolean> aRef = aData->boolean(attributeID());

  bool isBlocked = myCheckBox->blockSignals(true);
  myCheckBox->setChecked(aRef->value());
  myCheckBox->blockSignals(isBlocked);

  return true;
}

QList<QWidget*> ModuleBase_WidgetBoolValue::getControls() const
{
  QList<QWidget*> aList;
  aList.append(myCheckBox);
  return aList;
}
