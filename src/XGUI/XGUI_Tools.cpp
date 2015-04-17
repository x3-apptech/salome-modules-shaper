// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

#include "XGUI_Tools.h"

#include <TopoDS_Shape.hxx>
#include <ModelAPI_Object.h>
#include <ModelAPI_Result.h>
#include <ModelAPI_ResultParameter.h>
#include <ModelAPI_Feature.h>
#include <GeomAPI_Shape.h>

#include <QDir>

#include <iostream>
#include <sstream>

namespace XGUI_Tools {
//******************************************************************
QString dir(const QString& path, bool isAbs)
{
  QDir aDir = QFileInfo(path).dir();
  QString dirPath = isAbs ? aDir.absolutePath() : aDir.path();
  if (dirPath == QString("."))
    dirPath = QString();
  return dirPath;
}

//******************************************************************
QString file(const QString& path, bool withExt)
{
  QString fPath = path;
  while (!fPath.isEmpty() && (fPath[fPath.length() - 1] == '\\' || fPath[fPath.length() - 1] == '/'))
    fPath.remove(fPath.length() - 1, 1);

  if (withExt)
    return QFileInfo(fPath).fileName();
  else
    return QFileInfo(fPath).completeBaseName();
}

//******************************************************************
QString addSlash(const QString& path)
{
  QString res = path;
  if (!res.isEmpty() && res.at(res.length() - 1) != QChar('/')
      && res.at(res.length() - 1) != QChar('\\'))
    res += QDir::separator();
  return res;
}

//******************************************************************
bool isModelObject(FeaturePtr theFeature)
{
  return theFeature && !theFeature->data();
}

//******************************************************************
std::string featureInfo(FeaturePtr theFeature)
{
  std::ostringstream aStream;
  if (theFeature)
    aStream << theFeature.get() << " " << theFeature->getKind();
  return QString(aStream.str().c_str()).toStdString();
}

//******************************************************************
/*FeaturePtr realFeature(const FeaturePtr theFeature)
 {
 if (theFeature->data()) {
 return theFeature;
 } else {
 ObjectPtr aObject = std::dynamic_pointer_cast<ModelAPI_Object>(theFeature);
 return aObject->featureRef();
 }
 }*/


void checkObjects(const QObjectPtrList& theObjects, bool& hasResult, bool& hasFeature, bool& hasParameter)
{
  hasResult = false;
  hasFeature = false;
  hasParameter = false;
  foreach(ObjectPtr aObj, theObjects) {
    FeaturePtr aFeature = std::dynamic_pointer_cast<ModelAPI_Feature>(aObj);
    ResultPtr aResult = std::dynamic_pointer_cast<ModelAPI_Result>(aObj);
    ResultParameterPtr aConstruction = std::dynamic_pointer_cast<ModelAPI_ResultParameter>(aResult);

    hasResult = (aResult.get() != NULL);
    hasFeature = (aFeature.get() != NULL);
    hasParameter = (aConstruction.get() != NULL);
    if (hasFeature && hasResult  && hasParameter)
      break;
  }
}


}
