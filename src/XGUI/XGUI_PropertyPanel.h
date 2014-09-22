/*
 * XGUI_PropertyPanel.h
 *
 *  Created on: Apr 29, 2014
 *      Author: sbh
 */

#ifndef XGUI_PROPERTYPANEL_H_
#define XGUI_PROPERTYPANEL_H_

#include "XGUI.h"

#include <ModuleBase_ModelWidget.h>

#include <QDockWidget>
#include <QList>

class QKeyEvent;
class QVBoxLayout;

class XGUI_EXPORT XGUI_PropertyPanel : public QDockWidget
{
Q_OBJECT
 public:
  XGUI_PropertyPanel(QWidget* theParent);
  virtual ~XGUI_PropertyPanel();

  QWidget* contentWidget();
  void setModelWidgets(const QList<ModuleBase_ModelWidget*>& theWidgets);
  const QList<ModuleBase_ModelWidget*>& modelWidgets() const;

  void cleanContent();

  virtual bool eventFilter(QObject *theObject, QEvent *theEvent);

 public slots:
  void updateContentWidget(FeaturePtr theFeature);
  /// slot to activate the next widget in the property panel
  /// \param theWidget a widget. The next widget should be activated
  void onActivateNextWidget(ModuleBase_ModelWidget* theWidget);
  // Enables / disables "ok" ("accept") button
  void setAcceptEnabled(bool);

signals:
  /// The signal about key release on the control, that corresponds to the attribute
  /// \param theEvent key release event
  void keyReleased(QKeyEvent* theEvent);
  /// The signal about the widget activation
  /// \param theWidget the activated widget
  void widgetActivated(ModuleBase_ModelWidget* theWidget);

  /// Signal about the point 2d set to the feature
  /// \param the feature
  /// \param the attribute of the feature
  void storedPoint2D(ObjectPtr theFeature, const std::string& theAttribute);

 private:
  QWidget* myCustomWidget;

  QList<ModuleBase_ModelWidget*> myWidgets;

  QVBoxLayout* myMainLayout;
};

#endif /* XGUI_PROPERTYPANEL_H_ */
