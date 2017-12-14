// Copyright (C) 2014-2017  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or
// email : webmaster.salome@opencascade.com<mailto:webmaster.salome@opencascade.com>
//

#include "ModuleBase_ListView.h"
#include "ModuleBase_Tools.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QListWidget>
#include <QWidget>

#ifndef WIN32
#include <QResizeEvent>
#include <QTimer>
#endif

const int ATTRIBUTE_SELECTION_INDEX_ROLE = Qt::UserRole + 1;

/**
* Customization of a List Widget to make it to be placed on full width of container
*/
class CustomListWidget : public QListWidget
{
public:
  /// Constructor
  /// \param theParent a parent widget
  CustomListWidget(QWidget* theParent)
    : QListWidget(theParent)
  {
  }

  /// Redefinition of virtual method
  virtual QSize	sizeHint() const
  {
    int aHeight = 2*QFontMetrics(font()).height();
    QSize aSize = QListWidget::sizeHint();
    return QSize(aSize.width(), aHeight);
  }

  /// Redefinition of virtual method
  virtual QSize	minimumSizeHint() const
  {
    int aHeight = 4/*2*/*QFontMetrics(font()).height();
    QSize aSize = QListWidget::minimumSizeHint();
    return QSize(aSize.width(), aHeight);
  }

#ifndef WIN32
// The code is necessary only for Linux because
//it can not update viewport on widget resize
protected:
  void resizeEvent(QResizeEvent* theEvent)
  {
    QListWidget::resizeEvent(theEvent);
    QTimer::singleShot(5, viewport(), SLOT(repaint()));
  }
#endif
};

//********************************************************************
ModuleBase_ListView::ModuleBase_ListView(QWidget* theParent, const QString& theObjectName,
  const QString& theToolTip)
{
  myListControl = new CustomListWidget(theParent);

  myListControl->setObjectName(theObjectName);
  myListControl->setToolTip(theToolTip);
  myListControl->setSelectionMode(QAbstractItemView::ExtendedSelection);

  myCopyAction = ModuleBase_Tools::createAction(QIcon(":pictures/copy.png"), tr("Copy"),
                          theParent, this, SLOT(onCopyItem()));
  myCopyAction->setShortcut(QKeySequence::Copy);
  myCopyAction->setEnabled(false);
  myListControl->addAction(myCopyAction);

  myDeleteAction = ModuleBase_Tools::createAction(QIcon(":pictures/delete.png"), tr("Delete"),
                          theParent, this, SIGNAL(deleteActionClicked()));
  myDeleteAction->setEnabled(false);
  myListControl->addAction(myDeleteAction);

  myListControl->setContextMenuPolicy(Qt::ActionsContextMenu);
  connect(myListControl, SIGNAL(itemSelectionChanged()), SLOT(onListSelection()));
}

//********************************************************************
void ModuleBase_ListView::addItem(const QString& theTextValue, const int theIndex)
{
  QListWidgetItem* anItem = new QListWidgetItem(theTextValue, myListControl);
  anItem->setData(ATTRIBUTE_SELECTION_INDEX_ROLE, theIndex);
  myListControl->addItem(anItem);
}

//********************************************************************
void ModuleBase_ListView::getSelectedIndices(std::set<int>& theIndices)
{
  QList<QListWidgetItem*> aItems = myListControl->selectedItems();
  foreach(QListWidgetItem* anItem, aItems) {
    int anIndex = anItem->data(ATTRIBUTE_SELECTION_INDEX_ROLE).toInt();
    if (theIndices.find(anIndex) == theIndices.end())
      theIndices.insert(anIndex);
  }
}

//********************************************************************
void ModuleBase_ListView::removeSelectedItems()
{
  QList<QListWidgetItem*> aItems = myListControl->selectedItems();
  foreach(QListWidgetItem* anItem, aItems)
    myListControl->takeItem(myListControl->row(anItem));
}

//********************************************************************
void ModuleBase_ListView::removeItems(std::set<int>& theIndices)
{
  QList<QListWidgetItem*> aItems;
  for (int i = 0; i < myListControl->count(); i++) {
    QListWidgetItem* anItem = myListControl->item(i);
    int anIndex = anItem->data(ATTRIBUTE_SELECTION_INDEX_ROLE).toInt();
    if (theIndices.find(anIndex) != theIndices.end())
      aItems.append(anItem);
  }
  foreach(QListWidgetItem* anItem, aItems)
    myListControl->takeItem(myListControl->row(anItem));
}

//********************************************************************
void ModuleBase_ListView::restoreSelection(const QModelIndexList& theIndices)
{
  int aRows = myListControl->model()->rowCount();
  if (aRows > 0) {
    foreach(QModelIndex aIndex, theIndices) {
      if (aIndex.row() < aRows)
        myListControl->selectionModel()->select(aIndex, QItemSelectionModel::Select);
      else {
        QModelIndex aIdx = myListControl->model()->index(aRows - 1, 0);
        myListControl->selectionModel()->select(aIdx, QItemSelectionModel::Select);
      }
    }
  }
}

//********************************************************************
void ModuleBase_ListView::onCopyItem()
{
  QList<QListWidgetItem*> aItems = myListControl->selectedItems();
  QString aRes;
  foreach(QListWidgetItem* aItem, aItems) {
    if (!aRes.isEmpty())
      aRes += "\n";
    aRes += aItem->text();
  }
  if (!aRes.isEmpty()) {
    QClipboard* aClipboard = QApplication::clipboard();
    aClipboard->setText(aRes);
  }
}

//********************************************************************
void ModuleBase_ListView::onListSelection()
{
  QList<QListWidgetItem*> aItems = myListControl->selectedItems();
  myCopyAction->setEnabled(!aItems.isEmpty());
  myDeleteAction->setEnabled(!aItems.isEmpty());
}