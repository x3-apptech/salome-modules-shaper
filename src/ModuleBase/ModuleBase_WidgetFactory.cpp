/*
 * ModuleBase_WidgetFactory.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: sbh
 */

#include <ModuleBase_WidgetFactory.h>

#include <ModuleBase_Operation.h>
#include <ModuleBase_OperationDescription.h>
#include <ModuleBase_WidgetPoint2D.h>
#include <ModuleBase_WidgetFeatureOrAttribute.h>
#include <ModuleBase_WidgetFeature.h>
#include <ModuleBase_WidgetEditor.h>
#include <ModuleBase_WidgetSwitch.h>
#include <ModuleBase_WidgetShapeSelector.h>
#include <ModuleBase_WidgetDoubleValue.h>
#include <ModuleBase_WidgetBoolValue.h>
#include <ModuleBase_WidgetPoint2dDistance.h>
#include <ModuleBase_WidgetFileSelector.h>
#include <ModuleBase_WidgetChoice.h>
#include <ModuleBase_IWorkshop.h>
#include <ModuleBase_IModule.h>
#include <ModuleBase_Tools.h>
#include <ModuleBase_WidgetLineEdit.h>
#include <ModuleBase_WidgetMultiSelector.h>

#include <ModelAPI_Validator.h>

#include <Config_Keywords.h>
#include <Config_WidgetAPI.h>

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QMetaProperty>
#include <QLabel>
#include <QPixmap>
#include <QGroupBox>
#include <QToolBox>

#ifdef _DEBUG
#include <QDebug>
#endif

#include <cfloat>
#include <climits>

ModuleBase_WidgetFactory::ModuleBase_WidgetFactory(const std::string& theXmlRepresentation,
                                                   ModuleBase_IWorkshop* theWorkshop)
    : myWorkshop(theWorkshop)
{
  myWidgetApi = new Config_WidgetAPI(theXmlRepresentation);
}

ModuleBase_WidgetFactory::~ModuleBase_WidgetFactory()
{
}

void ModuleBase_WidgetFactory::createWidget(QWidget* theParent)
{
  myParentId = myWidgetApi->widgetId();
  if (!myWidgetApi->toChildWidget())
    return;

  QVBoxLayout* aWidgetLay = new QVBoxLayout(theParent);
  do {  //Iterate over each node
    std::string aWdgType = myWidgetApi->widgetType();
    //Create a widget (doublevalue, groupbox, toolbox, etc.
    QWidget* aWidget = createWidgetByType(aWdgType, theParent);
    if (aWidget) {
      if (!myWidgetApi->getBooleanAttribute(FEATURE_INTERNAL, false)) {
        aWidgetLay->addWidget(aWidget);
      } else {
        aWidget->setVisible(false);
      }
    }
    if (myWidgetApi->isContainerWidget()) {
      //if current widget is groupbox (container) process it's children recursively
      QString aGroupName = qs(myWidgetApi->getProperty(CONTAINER_PAGE_NAME));
      createWidget(aWidget);
      ModuleBase_Tools::adjustMargins(aWidget);
      QGroupBox* aGrBox = qobject_cast<QGroupBox*>(aWidget);
      aGrBox->setTitle(aGroupName);
    }
    if (myWidgetApi->isPagedWidget()) {
      //If current widget is toolbox or switch-casebox then fetch all
      //it's pages recursively and setup into the widget.
      myWidgetApi->toChildWidget();
      do {
        QString aPageName = qs(myWidgetApi->getProperty(CONTAINER_PAGE_NAME));
        QWidget* aPage = new QWidget(aWidget);
        ModuleBase_Tools::adjustMargins(aPage);
        createWidget(aPage);
        if (aWdgType == WDG_SWITCH) {
          ModuleBase_WidgetSwitch* aSwitch = qobject_cast<ModuleBase_WidgetSwitch*>(aWidget);
          aSwitch->addPage(aPage, aPageName);
        } else if (aWdgType == WDG_TOOLBOX) {
          QToolBox* aToolbox = qobject_cast<QToolBox*>(aWidget);
          aToolbox->addItem(aPage, aPageName);
        }
      } while (myWidgetApi->toNextWidget());
    }
  } while (myWidgetApi->toNextWidget());
  theParent->setLayout(aWidgetLay);
}

QWidget* ModuleBase_WidgetFactory::labelControl(QWidget* theParent)
{
  QWidget* result = new QWidget(theParent);
  QVBoxLayout* aLabelLay = new QVBoxLayout(result);
  QLabel* aLabel = new QLabel(result);
  aLabel->setWordWrap(true);
  aLabel->setText(qs(myWidgetApi->getProperty(INFO_WDG_TEXT)));
  aLabel->setToolTip(qs(myWidgetApi->getProperty(INFO_WDG_TOOLTIP)));
  aLabelLay->addWidget(aLabel);
  aLabelLay->addStretch(1);
  result->setLayout(aLabelLay);
  return result;
}

QWidget* ModuleBase_WidgetFactory::createWidgetByType(const std::string& theType,
                                                      QWidget* theParent)
{
  QWidget* result = NULL;
  if (theType == WDG_DOUBLEVALUE) {
    result = doubleSpinBoxControl(theParent);

  } else if (theType == WDG_INFO) {
    result = labelControl(theParent);

  } else if (theType == WDG_SHAPE_SELECTOR) {
    result = shapeSelectorControl(theParent);

  } else if (theType == WDG_BOOLVALUE) {
    result = booleanControl(theParent);

  } else if (theType == WDG_POINT_SELECTOR) {
    result = pointSelectorControl(theParent);

  } else if (theType == WDG_FEATURE_SELECTOR) {
    result = featureSelectorControl(theParent);

  } else if (theType == WDG_FEATURE_OR_ATTRIBUTE_SELECTOR) {
    result = featureOrAttributeSelectorControl(theParent);

  } else if (theType == WDG_DOUBLEVALUE_EDITOR) {
    result = doubleValueEditor(theParent);

  } else if (theType == WDG_POINT2D_DISTANCE) {
    result = point2dDistanceControl(theParent);

  } else if (theType == WDG_FILE_SELECTOR) {
    result = fileSelectorControl(theParent);

  } else if (theType == WDG_CHOICE) {
    result = choiceControl(theParent);

  } else if (theType == WDG_STRINGVALUE) {
    result = lineEditControl(theParent);

  } else if (theType == WDG_MULTISELECTOR) {
    result = multiSelectorControl(theParent);

  } else if (myWidgetApi->isContainerWidget() || myWidgetApi->isPagedWidget()) {
    result = createContainer(theType, theParent);
  } else {
    result = myWorkshop->module()->createWidgetByType(theType, theParent, myWidgetApi,
                                                      myModelWidgets);
#ifdef _DEBUG
    if (!result) {qDebug("ModuleBase_WidgetFactory::fillWidget: find bad widget type");}
#endif
  }
  if (result) {
    // register that this attribute in feature is not obligatory for the feature execution
    // so, it is not needed for the standard validation mechanism
    bool isObligatory = 
      myWidgetApi ? myWidgetApi->getBooleanAttribute(FEATURE_OBLIGATORY, true) : true;
    if (!isObligatory) {
      ModelAPI_Session::get()->validators()->registerNotObligatory(
        myParentId, myWidgetApi->widgetId());
    }
  }

  return result;
}

QWidget* ModuleBase_WidgetFactory::createContainer(const std::string& theType, QWidget* theParent)
{
  QWidget* result = NULL;
  if (theType == WDG_GROUP || theType == WDG_CHECK_GROUP) {
    QGroupBox* aGroupBox = new QGroupBox(theParent);
    aGroupBox->setCheckable(theType == WDG_CHECK_GROUP);
    result = aGroupBox;
  } else if (theType == WDG_TOOLBOX) {
    result = new QToolBox(theParent);
  } else if (theType == WDG_SWITCH) {
    result = new ModuleBase_WidgetSwitch(theParent);
  } else if (theType == WDG_TOOLBOX_BOX || theType == WDG_SWITCH_CASE) {
    result = NULL;
  }
#ifdef _DEBUG
  else {qDebug() << "ModuleBase_WidgetFactory::fillWidget: find bad container type";}
#endif
  return result;
}

QWidget* ModuleBase_WidgetFactory::doubleSpinBoxControl(QWidget* theParent)
{
  ModuleBase_WidgetDoubleValue* aDblWgt =
      new ModuleBase_WidgetDoubleValue(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aDblWgt);
  return aDblWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::pointSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetPoint2D* aWidget =
      new ModuleBase_WidgetPoint2D(theParent, myWidgetApi,myParentId);
  myModelWidgets.append(aWidget);
  return aWidget->getControl();
}

QWidget* ModuleBase_WidgetFactory::featureSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetFeature* aWidget =
      new ModuleBase_WidgetFeature(theParent, myWidgetApi,myParentId);
  myModelWidgets.append(aWidget);
  return aWidget->getControl();
}

QWidget* ModuleBase_WidgetFactory::featureOrAttributeSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetFeatureOrAttribute* aWidget =
      new ModuleBase_WidgetFeatureOrAttribute(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aWidget);
  return aWidget->getControl();
}

QWidget* ModuleBase_WidgetFactory::doubleValueEditor(QWidget* theParent)
{
  ModuleBase_WidgetEditor* aWidget =
      new ModuleBase_WidgetEditor(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aWidget);
  return aWidget->getControl();
}

QWidget* ModuleBase_WidgetFactory::shapeSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetShapeSelector* aSelector =
      new ModuleBase_WidgetShapeSelector(theParent, myWorkshop, myWidgetApi, myParentId);
  myModelWidgets.append(aSelector);
  return aSelector->getControl();
}

QWidget* ModuleBase_WidgetFactory::booleanControl(QWidget* theParent)
{
  ModuleBase_WidgetBoolValue* aBoolWgt =
      new ModuleBase_WidgetBoolValue(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aBoolWgt);
  return aBoolWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::point2dDistanceControl(QWidget* theParent)
{
  ModuleBase_WidgetPoint2dDistance* aDistWgt =
      new ModuleBase_WidgetPoint2dDistance(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aDistWgt);
  return aDistWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::fileSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetFileSelector* aFileSelectorWgt =
      new ModuleBase_WidgetFileSelector(theParent, myWidgetApi, myParentId);
  myModelWidgets.append(aFileSelectorWgt);
  return aFileSelectorWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::choiceControl(QWidget* theParent)
{
  ModuleBase_WidgetChoice* aChoiceWgt =
      new ModuleBase_WidgetChoice(theParent, myWidgetApi,myParentId);
  myModelWidgets.append(aChoiceWgt);
  return aChoiceWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::lineEditControl(QWidget* theParent)
{
  ModuleBase_WidgetLineEdit* aLineEditWgt =
      new ModuleBase_WidgetLineEdit(theParent, myWidgetApi,myParentId);
  myModelWidgets.append(aLineEditWgt);
  return aLineEditWgt->getControl();
}

QWidget* ModuleBase_WidgetFactory::multiSelectorControl(QWidget* theParent)
{
  ModuleBase_WidgetMultiSelector* aMultiselectorWgt =
      new ModuleBase_WidgetMultiSelector(theParent, myWidgetApi,myParentId);
  myModelWidgets.append(aMultiselectorWgt);
  return aMultiselectorWgt->getControl();
}

QString ModuleBase_WidgetFactory::qs(const std::string& theStdString) const
{
  return QString::fromStdString(theStdString);
}
