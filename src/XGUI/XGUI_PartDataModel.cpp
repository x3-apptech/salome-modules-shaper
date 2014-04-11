#include "XGUI_PartDataModel.h"

#include <ModelAPI_PluginManager.h>
#include <ModelAPI_Iterator.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_Feature.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_AttributeDocRef.h>

#include <QIcon>

XGUI_TopDataModel::XGUI_TopDataModel(const std::shared_ptr<ModelAPI_Document>& theDocument, QObject* theParent)
  : XGUI_FeaturesModel(theDocument, theParent)
{
}
  
XGUI_TopDataModel::~XGUI_TopDataModel()
{
}


QVariant XGUI_TopDataModel::data(const QModelIndex& theIndex, int theRole) const
{
  switch (theRole) {
  case Qt::DisplayRole:
    // return a name
    switch (theIndex.internalId()) {
    case ParamsFolder:
      return tr("Parameters");
    case ParamObject:
      {
        std::shared_ptr<ModelAPI_Feature> aFeature = myDocument->feature(PARAMETERS_GROUP, theIndex.row());
        return aFeature->data()->getName().c_str();
      } 
    case ConstructFolder:
        return tr("Constructions");
    case ConstructObject:
      {
        std::shared_ptr<ModelAPI_Feature> aFeature = myDocument->feature(CONSTRUCTIONS_GROUP, theIndex.row());
        return aFeature->data()->getName().c_str();
      }
    }
    break;

  case Qt::DecorationRole:
    // return an Icon
    switch (theIndex.internalId()) {
    case ParamsFolder:
        return QIcon(":pictures/params_folder.png");
    case ConstructFolder:
        return QIcon(":pictures/constr_folder.png");
    case ConstructObject:
        return QIcon(":pictures/point_ico.png");
    }
    break;

  case Qt::ToolTipRole:
    // return Tooltip
    break;
  }
  return QVariant();
}

QVariant XGUI_TopDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  return QVariant();
}

int XGUI_TopDataModel::rowCount(const QModelIndex& theParent) const
{
  if (!theParent.isValid()) 
    return 2;

  if (theParent.internalId() == ParamsFolder)
    return myDocument->featuresIterator(PARAMETERS_GROUP)->numIterationsLeft();

  if (theParent.internalId() == ConstructFolder)
    return myDocument->featuresIterator(CONSTRUCTIONS_GROUP)->numIterationsLeft();

  return 0;
}

int XGUI_TopDataModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QModelIndex XGUI_TopDataModel::index(int theRow, int theColumn, const QModelIndex& theParent) const
{
  if (!theParent.isValid()) {
    switch (theRow) {
    case 0:
      return createIndex(theRow, theColumn, (qint32) ParamsFolder);
    case 1:
      return createIndex(theRow, theColumn, (qint32) ConstructFolder);
    }
  } else {
    if (theParent.internalId() == ParamsFolder)
      return createIndex(theRow, theColumn, (qint32) ParamObject);

    if (theParent.internalId() == ConstructFolder)
      return createIndex(theRow, theColumn, (qint32) ConstructObject);
  }
  return QModelIndex();
}

QModelIndex XGUI_TopDataModel::parent(const QModelIndex& theIndex) const
{
  int aId = (int)theIndex.internalId();
  switch (aId) {
  case ParamsFolder:
  case ConstructFolder:
    return QModelIndex();
  case ParamObject:
    return createIndex(0, 0, (qint32) ParamsFolder);
  case ConstructObject:
    return createIndex(1, 0, (qint32) ConstructFolder);
  }
  return QModelIndex();
}

bool XGUI_TopDataModel::hasChildren(const QModelIndex& theParent) const
{
  return rowCount(theParent) > 0;
}

FeaturePtr XGUI_TopDataModel::feature(const QModelIndex& theIndex) const
{
  switch (theIndex.internalId()) {
  case ParamsFolder:
  case ConstructFolder:
    return FeaturePtr();
  case ParamObject:
    return myDocument->feature(PARAMETERS_GROUP, theIndex.row());
  case ConstructObject:
    return myDocument->feature(CONSTRUCTIONS_GROUP, theIndex.row());
  }
  return FeaturePtr();
}


//******************************************************************
//******************************************************************
//******************************************************************
XGUI_PartDataModel::XGUI_PartDataModel(const std::shared_ptr<ModelAPI_Document>& theDocument, QObject* theParent)
  : XGUI_PartModel(theDocument, theParent)
{
}


XGUI_PartDataModel::~XGUI_PartDataModel()
{
}

QVariant XGUI_PartDataModel::data(const QModelIndex& theIndex, int theRole) const
{
  switch (theRole) {
  case Qt::DisplayRole:
    // return a name
    switch (theIndex.internalId()) {
    case MyRoot:
      {
        std::shared_ptr<ModelAPI_Feature> aFeature = myDocument->feature(PARTS_GROUP, myId);
        return aFeature->data()->getName().c_str();
      }
    case ParamsFolder:
      return tr("Parameters");
    case ConstructFolder:
      return tr("Constructions");
    case ParamObject:
      {
        std::shared_ptr<ModelAPI_Feature> aFeature = 
          featureDocument()->feature(PARAMETERS_GROUP, theIndex.row());
        return aFeature->data()->getName().c_str();
      }
    case ConstructObject:
      {
        std::shared_ptr<ModelAPI_Feature> aFeature = 
          featureDocument()->feature(CONSTRUCTIONS_GROUP, theIndex.row());
        return aFeature->data()->getName().c_str();
      }
    }
    break;
  case Qt::DecorationRole:
    // return an Icon
    switch (theIndex.internalId()) {
    case MyRoot:
      return QIcon(":pictures/part_ico.png");
    case ParamsFolder:
      return QIcon(":pictures/params_folder.png");
    case ConstructFolder:
      return QIcon(":pictures/constr_folder.png");
    case ConstructObject:
        return QIcon(":pictures/point_ico.png");
    }
   break;
  case Qt::ToolTipRole:
    // return Tooltip
    break;
  }
  return QVariant();
}

QVariant XGUI_PartDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  return QVariant();
}

int XGUI_PartDataModel::rowCount(const QModelIndex& parent) const
{
  if (!parent.isValid()) 
    if (myDocument->feature(PARTS_GROUP, myId))
      return 1;
    else 
      return 0;
  switch (parent.internalId()) {
  case MyRoot:
    return 2;
  case ParamsFolder:
    return featureDocument()->featuresIterator(PARAMETERS_GROUP)->numIterationsLeft();
  case ConstructFolder:
    return featureDocument()->featuresIterator(CONSTRUCTIONS_GROUP)->numIterationsLeft();
  }
  return 0;
}

int XGUI_PartDataModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QModelIndex XGUI_PartDataModel::index(int theRow, int theColumn, const QModelIndex &theParent) const
{
  if (!theParent.isValid())
    return createIndex(theRow, 0, (qint32) MyRoot);

  int aId = (int)theParent.internalId();
  switch (aId) {
  case MyRoot:
    switch (theRow) {
    case 0:
      return createIndex(0, 0, (qint32) ParamsFolder);
    case 1:
      return createIndex(1, 0, (qint32) ConstructFolder);
    }
  case ParamsFolder:
    return createIndex(theRow, 0, (qint32) ParamObject);
  case ConstructFolder:
    return createIndex(theRow, 0, (qint32) ConstructObject);
  }
  return QModelIndex();
}

QModelIndex XGUI_PartDataModel::parent(const QModelIndex& theIndex) const
{
  switch (theIndex.internalId()) {
  case MyRoot:
    return QModelIndex();
  case ParamsFolder:
  case ConstructFolder:
    return createIndex(0, 0, (qint32) MyRoot);
  case ParamObject:
    return createIndex(0, 0, (qint32) ParamsFolder);
  case ConstructObject:
    return createIndex(1, 0, (qint32) ConstructFolder);
  }
  return QModelIndex();
}

bool XGUI_PartDataModel::hasChildren(const QModelIndex& theParent) const
{
  return rowCount(theParent) > 0;
}


std::shared_ptr<ModelAPI_Document> XGUI_PartDataModel::featureDocument() const
{
  std::shared_ptr<ModelAPI_Feature> aFeature = myDocument->feature(PARTS_GROUP, myId);
  return aFeature->data()->docRef("PartDocument")->value();
}

FeaturePtr XGUI_PartDataModel::feature(const QModelIndex& theIndex) const
{
  switch (theIndex.internalId()) {
  case MyRoot:
    return myDocument->feature(PARTS_GROUP, myId);
  case ParamsFolder:
  case ConstructFolder:
    return FeaturePtr();
  case ParamObject:
    return featureDocument()->feature(PARAMETERS_GROUP, theIndex.row());
  case ConstructObject:
    return featureDocument()->feature(CONSTRUCTIONS_GROUP, theIndex.row());
  }
  return FeaturePtr();
}
