
#ifndef XGUI_ObjectsBrowser_H
#define XGUI_ObjectsBrowser_H

#include "XGUI_Constants.h"

#include <QTreeView>

class XGUI_DocumentDataModel;

/**\class XGUI_ObjectsBrowser
 * \ingroup GUI
 * \brief Object browser window object. Represents data tree of current data structure
 */
 class XGUI_ObjectsBrowser : public QTreeView
{
  Q_OBJECT
public:
  XGUI_ObjectsBrowser(QWidget* theParent);
  virtual ~XGUI_ObjectsBrowser();

  //! Returns Model which provides access to data objects
  XGUI_DocumentDataModel* dataModel() const { return myDocModel; }

  //! Returns list of currently selected features
  QFeatureList selectedData() const { return mySelectedData; }

signals:
  //! Emited when selection is changed
  void selectionChanged();


private slots:
  //! Called when selection in Data Tree is changed
  void onSelectionChanged(const QItemSelection& theSelected, const QItemSelection& theDeselected);

private:
  //! Internal model
  XGUI_DocumentDataModel* myDocModel;

  //! List of currently selected data
  QFeatureList mySelectedData;
};

#endif