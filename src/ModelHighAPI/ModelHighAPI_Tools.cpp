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

#include "ModelHighAPI_Tools.h"
#include <ModelHighAPI_FeatureStore.h>
//--------------------------------------------------------------------------------------
#include <GeomAPI_Dir.h>
#include <GeomAPI_Pnt.h>
#include <GeomAPI_Pnt2d.h>
//--------------------------------------------------------------------------------------
#include <GeomDataAPI_Dir.h>
#include <GeomDataAPI_Point.h>
#include <GeomDataAPI_Point2D.h>
//--------------------------------------------------------------------------------------
#include <ModelAPI_AttributeBoolean.h>
#include <ModelAPI_AttributeDocRef.h>
#include <ModelAPI_AttributeDouble.h>
#include <ModelAPI_AttributeIntArray.h>
#include <ModelAPI_AttributeInteger.h>
#include <ModelAPI_AttributeRefAttr.h>
#include <ModelAPI_AttributeRefAttrList.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeRefList.h>
#include <ModelAPI_AttributeSelection.h>
#include <ModelAPI_AttributeSelectionList.h>
#include <ModelAPI_AttributeString.h>
#include <ModelAPI_AttributeStringArray.h>
#include <ModelAPI_AttributeDoubleArray.h>
#include <ModelAPI_Session.h>
#include <ModelAPI_Tools.h>
#include <ModelAPI_ResultPart.h>
#include <ModelAPI_Events.h>
//--------------------------------------------------------------------------------------
#include <Config_ModuleReader.h>
//--------------------------------------------------------------------------------------
#include "ModelHighAPI_Double.h"
#include "ModelHighAPI_Integer.h"
#include "ModelHighAPI_RefAttr.h"
#include "ModelHighAPI_Reference.h"
#include "ModelHighAPI_Selection.h"

#include <Events_InfoMessage.h>

// Have to be included before std headers
#include <Python.h>

#include <algorithm>
#include <iostream>

//--------------------------------------------------------------------------------------
void fillAttribute(const std::shared_ptr<GeomAPI_Pnt2d> & theValue,
                   const std::shared_ptr<GeomDataAPI_Point2D> & theAttribute)
{
  theAttribute->setValue(theValue);
}

void fillAttribute(const std::shared_ptr<GeomDataAPI_Point2D> & theAttribute,
                   double theX, double theY)
{
  theAttribute->setValue(theX, theY);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::shared_ptr<GeomAPI_Dir> & theValue,
                   const std::shared_ptr<GeomDataAPI_Dir> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::shared_ptr<GeomAPI_Pnt> & theValue,
                   const std::shared_ptr<GeomDataAPI_Point> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(bool theValue,
                   const std::shared_ptr<ModelAPI_AttributeBoolean> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const ModelHighAPI_Double & theValue,
                   const std::shared_ptr<ModelAPI_AttributeDouble> & theAttribute)
{
  theValue.fillAttribute(theAttribute);
}
void fillAttribute(double theValue,
                   const std::shared_ptr<ModelAPI_AttributeDouble> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const ModelHighAPI_Integer & theValue,
                   const std::shared_ptr<ModelAPI_AttributeInteger> & theAttribute)
{
  theValue.fillAttribute(theAttribute);
}
void fillAttribute(int theValue,
                   const std::shared_ptr<ModelAPI_AttributeInteger> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const ModelHighAPI_RefAttr & theValue,
                   const std::shared_ptr<ModelAPI_AttributeRefAttr> & theAttribute)
{
  theValue.fillAttribute(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<ModelHighAPI_RefAttr> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeRefAttrList> & theAttribute)
{
  theAttribute->clear();
  for (auto it = theValue.begin(); it != theValue.end(); ++it)
    it->appendToList(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const ModelHighAPI_Reference & theValue,
                   const std::shared_ptr<ModelAPI_AttributeReference> & theAttribute)
{
  theValue.fillAttribute(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<ModelHighAPI_Reference> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeRefList> & theAttribute)
{
  theAttribute->clear();
  for (auto it = theValue.begin(); it != theValue.end(); ++it)
    it->appendToList(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::shared_ptr<ModelAPI_Object> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeReference> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<std::shared_ptr<ModelAPI_Object> > & theValue,
                   const std::shared_ptr<ModelAPI_AttributeRefList> & theAttribute)
{
  theAttribute->clear();
  for (auto it = theValue.begin(); it != theValue.end(); ++it)
    theAttribute->append(*it);
}

MODELHIGHAPI_EXPORT
void fillAttribute(const std::list<ModelHighAPI_Selection> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeRefList> & theAttribute)
{
  theAttribute->clear();
  for (auto it = theValue.begin(); it != theValue.end(); ++it) {
    if (it->resultSubShapePair().first)
      theAttribute->append(it->resultSubShapePair().first); // use only context
  }
}

//--------------------------------------------------------------------------------------
void fillAttribute(const ModelHighAPI_Selection & theValue,
                   const std::shared_ptr<ModelAPI_AttributeSelection> & theAttribute)
{
  theValue.fillAttribute(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<ModelHighAPI_Selection> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeSelectionList> & theAttribute)
{
  theAttribute->clear();

  if(!theValue.empty()) {
    const ModelHighAPI_Selection& aSelection = theValue.front();
    GeomAPI_Shape::ShapeType aSelectionType = getShapeType(aSelection);
    theAttribute->setSelectionType(strByShapeType(aSelectionType));
  }

  for (auto it = theValue.begin(); it != theValue.end(); ++it)
    it->appendToList(theAttribute);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::string & theValue,
                   const std::shared_ptr<ModelAPI_AttributeString> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const char * theValue,
                   const std::shared_ptr<ModelAPI_AttributeString> & theAttribute)
{
  theAttribute->setValue(theValue);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<std::string> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeStringArray> & theAttribute)
{
  theAttribute->setSize(int(theValue.size()));

  int anIndex = 0;
  for (auto it = theValue.begin(); it != theValue.end(); ++it, ++anIndex)
    theAttribute->setValue(anIndex, *it);
}

//--------------------------------------------------------------------------------------
void fillAttribute(const std::list<ModelHighAPI_Integer> & theValue,
                   const std::shared_ptr<ModelAPI_AttributeIntArray> & theAttribute)
{
  theAttribute->setSize(int(theValue.size()));

  int anIndex = 0;
  for (auto it = theValue.begin(); it != theValue.end(); ++it, ++anIndex)
    theAttribute->setValue(anIndex, it->intValue()); // use only values, no text support in array
}

void fillAttribute(const ModelHighAPI_Double & theX,
                   const ModelHighAPI_Double & theY,
                   const ModelHighAPI_Double & theZ,
                   const std::shared_ptr<GeomDataAPI_Point> & theAttribute)
{
  theX.fillAttribute(theAttribute, theX, theY, theZ);
}


//==================================================================================================
GeomAPI_Shape::ShapeType shapeTypeByStr(std::string theShapeTypeStr)
{
  GeomAPI_Shape::ShapeType aShapeType = GeomAPI_Shape::SHAPE;

  std::transform(theShapeTypeStr.begin(), theShapeTypeStr.end(),
                 theShapeTypeStr.begin(), ::tolower);

  if(theShapeTypeStr == "compound") {
    aShapeType = GeomAPI_Shape::COMPOUND;
  } else if(theShapeTypeStr == "compsolid") {
    aShapeType = GeomAPI_Shape::COMPSOLID;
  } else if(theShapeTypeStr == "solid") {
    aShapeType = GeomAPI_Shape::SOLID;
  } else if(theShapeTypeStr == "shell") {
    aShapeType = GeomAPI_Shape::SHELL;
  } else if(theShapeTypeStr == "face") {
    aShapeType = GeomAPI_Shape::FACE;
  } else if(theShapeTypeStr == "wire") {
    aShapeType = GeomAPI_Shape::WIRE;
  } else if(theShapeTypeStr == "edge") {
    aShapeType = GeomAPI_Shape::EDGE;
  } else if(theShapeTypeStr == "vertex") {
    aShapeType = GeomAPI_Shape::VERTEX;
  } else if(theShapeTypeStr == "shape") {
    aShapeType = GeomAPI_Shape::SHAPE;
  }

  return aShapeType;
}

std::string strByShapeType(GeomAPI_Shape::ShapeType theShapeType)
{
  std::string aShapeTypeStr;
  switch (theShapeType) {
  case GeomAPI_Shape::COMPOUND:
    aShapeTypeStr = "COMPOUND";
    break;
  case GeomAPI_Shape::COMPSOLID:
    aShapeTypeStr = "COMPSOLID";
    break;
  case GeomAPI_Shape::SOLID:
    aShapeTypeStr = "SOLID";
    break;
  case GeomAPI_Shape::SHELL:
    aShapeTypeStr = "SHELL";
    break;
  case GeomAPI_Shape::FACE:
    aShapeTypeStr = "FACE";
    break;
  case GeomAPI_Shape::WIRE:
    aShapeTypeStr = "WIRE";
    break;
  case GeomAPI_Shape::EDGE:
    aShapeTypeStr = "EDGE";
    break;
  case GeomAPI_Shape::VERTEX:
    aShapeTypeStr = "VERTEX";
    break;
  default:
    aShapeTypeStr = "SHAPE";
    break;
  }
  return aShapeTypeStr;
}

//==================================================================================================
GeomAPI_Shape::ShapeType getShapeType(const ModelHighAPI_Selection& theSelection)
{
  GeomAPI_Shape::ShapeType aShapeType = GeomAPI_Shape::SHAPE;

  switch(theSelection.variantType()) {
    case ModelHighAPI_Selection::VT_ResultSubShapePair: {
      ResultSubShapePair aPair = theSelection.resultSubShapePair();
      GeomShapePtr aShape = aPair.second;
      if(!aShape.get()) {
        aShape = aPair.first->shape();
      }
      if(!aShape.get()) {
        return aShapeType;
      }
      aShapeType = aShape->shapeType();
      break;
    }
    case ModelHighAPI_Selection::VT_TypeSubShapeNamePair: {
      TypeSubShapeNamePair aPair = theSelection.typeSubShapeNamePair();
      std::string aType = aPair.first;
      aShapeType = shapeTypeByStr(aType);
      break;
    }
    case ModelHighAPI_Selection::VT_TypeInnerPointPair: {
      TypeInnerPointPair aPair = theSelection.typeInnerPointPair();
      std::string aType = aPair.first;
      aType = aType.substr(0, aType.find_first_of('_'));
      aShapeType = shapeTypeByStr(aType);
      break;
    }
    case ModelHighAPI_Selection::VT_WeakNamingPair: {
      TypeWeakNamingPair aPair = theSelection.typeWeakNamingPair();
      std::string aType = aPair.first;
      aShapeType = shapeTypeByStr(aType);
      break;
    }
  }

  return aShapeType;
}

//--------------------------------------------------------------------------------------
ModelAPI_AttributeTables::ValueType valueTypeByStr(const std::string& theValueTypeStr)
{
  std::string aType = theValueTypeStr;
  std::transform(aType.begin(), aType.end(), aType.begin(), ::tolower);
  if (aType == "boolean")
    return ModelAPI_AttributeTables::BOOLEAN;
  else if (aType == "integer")
    return ModelAPI_AttributeTables::INTEGER;
  else if (aType == "string")
    return ModelAPI_AttributeTables::STRING;
  return ModelAPI_AttributeTables::DOUBLE; // default
}

//--------------------------------------------------------------------------------------
std::string strByValueType(const ModelAPI_AttributeTables::ValueType theType)
{
  switch(theType) {
  case ModelAPI_AttributeTables::BOOLEAN: return "BOOLEAN";
  case ModelAPI_AttributeTables::INTEGER: return "INTEGER";
  case ModelAPI_AttributeTables::DOUBLE: return "DOUBLE";
  case ModelAPI_AttributeTables::STRING: return "STRING";
  }
  return ""; // bad case
}

/// stores the features information, recursively stores sub-documents features
std::string storeFeatures(const std::string& theDocName, DocumentPtr theDoc,
  std::map<std::string, std::map<std::string, ModelHighAPI_FeatureStore> >& theStore,
  const bool theCompare) // if false => store
{
  std::map<std::string, std::map<std::string, ModelHighAPI_FeatureStore> >::iterator aDocFind;
  if (theCompare) {
     aDocFind = theStore.find(theDocName);
     if (aDocFind == theStore.end()) {
       return "Document '" + theDocName + "' not found";
     }
  }
  // store the model features information: iterate all features
  int anObjectsCount = 0; // stores the number of compared features for this document to compare
  std::set<std::string> aProcessed; // processed features names (that are in the current document)

  // process all objects (features and folders)
  std::list<ObjectPtr> allObjects = theDoc->allObjects();
  std::list<ObjectPtr>::iterator allIter = allObjects.begin();
  for(; allIter != allObjects.end(); allIter++) {
    ObjectPtr anObject = *allIter;
    if (theCompare) {
      std::map<std::string, ModelHighAPI_FeatureStore>::iterator
        anObjFind = aDocFind->second.find(anObject->data()->name());
      if (anObjFind == aDocFind->second.end()) {
        return "Document '" + theDocName + "' feature '" + anObject->data()->name() + "' not found";
      }
      std::string anError = anObjFind->second.compare(anObject);
      if (!anError.empty()) {
        anError = "Document " + theDocName + " " + anError;
        return anError;
      }
      anObjectsCount++;
      aProcessed.insert(anObject->data()->name());
    } else {
      theStore[theDocName][anObject->data()->name()] = ModelHighAPI_FeatureStore(anObject);
    }

    FeaturePtr aFeature = std::dynamic_pointer_cast<ModelAPI_Feature>(anObject);
    if (aFeature) {
      // iterate all results of this feature
      std::list<ResultPtr> allResults;
      ModelAPI_Tools::allResults(aFeature, allResults);
      std::list<ResultPtr>::iterator aRes = allResults.begin();
      for(; aRes != allResults.end(); aRes++) {
        // recursively store features of sub-documents
        if ((*aRes)->groupName() == ModelAPI_ResultPart::group()) {
          DocumentPtr aDoc = std::dynamic_pointer_cast<ModelAPI_ResultPart>(*aRes)->partDoc();
          if (aDoc.get()) {
            std::string anError =
                storeFeatures((*aRes)->data()->name(), aDoc, theStore, theCompare);
            if (!anError.empty())
              return anError;
          }
        }
      }
    }
  }
  // checks the number of compared features
  if (theCompare) {
    if (aDocFind->second.size() != anObjectsCount) {
      // search for disappeared feature
      std::string aLostName;
      std::map<std::string, ModelHighAPI_FeatureStore>::iterator aLostIter;
      for(aLostIter = aDocFind->second.begin(); aLostIter != aDocFind->second.end(); aLostIter++) {
        if (aProcessed.find(aLostIter->first) == aProcessed.end()) {
          aLostName = aLostIter->first;
        }
      }
      return "For document '" + theDocName +
        "' the number of features is decreased, there is no feature '" + aLostName + "'";
    }
  }
  return ""; // ok
}

//==================================================================================================
typedef std::map<std::string, std::map<std::string, ModelHighAPI_FeatureStore> > Storage;

static bool dumpToPython(SessionPtr theSession,
                         const char* theFilename,
                         const char* theSelectionType,
                         const std::string& theErrorMsgContext)
{
  // 2431: set PartSet as a current document
  theSession->setActiveDocument(theSession->moduleDocument(), true);
  // dump all to the python file
  theSession->startOperation("Check python dump");
  FeaturePtr aDump = theSession->moduleDocument()->addFeature("Dump");
  if (aDump.get()) {
    aDump->string("file_path")->setValue(theFilename);
    aDump->string("file_format")->setValue("py");
    aDump->string("selection_type")->setValue(theSelectionType);
    aDump->execute();
  }
  bool isProblem = !aDump.get() || !aDump->error().empty(); // after "finish" dump will be removed
  if (isProblem && aDump.get()) {
    std::cout << "Dump feature error " << aDump->error() << std::endl;
    Events_InfoMessage anErrorMsg(theErrorMsgContext, aDump->error());
    anErrorMsg.send();
  }
  theSession->finishOperation();
  return !isProblem;
}

static bool checkDump(SessionPtr theSession,
                      char* theFilename,
                      Storage& theStorage,
                      const std::string& theErrorMsgContext)
{

  // close all before importation of the script
  theSession->closeAll();

  // execute the dumped
  PyGILState_STATE gstate = PyGILState_Ensure(); /* acquire python thread */
  static char aReadMode[] = "r";
  FILE* PyFileObject = _Py_fopen(theFilename, aReadMode);
  PyRun_SimpleFileEx(PyFileObject, theFilename, 1);
  PyGILState_Release(gstate); /* release python thread */

  // compare with the stored data
  std::string anError = storeFeatures(
    theSession->moduleDocument()->kind(), theSession->moduleDocument(), theStorage, true);
  if (!anError.empty()) {
    std::cout << anError << std::endl;
    Events_InfoMessage anErrorMsg(theErrorMsgContext, anError);
    anErrorMsg.send();
    return false;
  }

  return true;
}

bool checkPythonDump(const checkDumpType theCheckType)
{
  static const std::string anErrorByNaming("checkPythonDump by naming");
  static const std::string anErrorByGeometry("checkPythonDump by geometry");
  static const std::string anErrorByWeak("checkPythonDump by weak naming");

  static char aFileForNamingDump[] = "./check_dump_byname.py";
  static char aFileForGeometryDump[] = "./check_dump_bygeom.py";
  static char aFileForWeakDump[] = "./check_dump_weak.py";

  SessionPtr aSession = ModelAPI_Session::get();
  if (theCheckType & CHECK_NAMING) {
    // dump with the selection by names
    if (!dumpToPython(aSession, aFileForNamingDump, "topological_naming", anErrorByNaming))
      return false;
  }
  if (theCheckType & CHECK_GEOMETRICAL) {
    // dump with the selection by geometry
    if (!dumpToPython(aSession, aFileForGeometryDump, "geometric_selection", anErrorByGeometry))
      return false;
  }
  if (theCheckType & CHECK_WEAK) {
    // dump with the selection by weak naming
    if (!dumpToPython(aSession, aFileForWeakDump, "weak_naming", anErrorByWeak))
      return false;
  }

   // map from document name to feature name to feature data
  std::map<std::string, std::map<std::string, ModelHighAPI_FeatureStore> > aStore;
  std::string anError = storeFeatures(
    aSession->moduleDocument()->kind(), aSession->moduleDocument(), aStore, false);
  if (!anError.empty()) {
    Events_InfoMessage anErrorMsg(std::string("checkPythonDump"), anError);
    anErrorMsg.send();
    return false;
  }

  bool isOk = true;
  if (theCheckType & CHECK_NAMING) {
    // check dump with the selection by names
    isOk = checkDump(aSession, aFileForNamingDump, aStore, anErrorByNaming);
  }
  if (theCheckType & CHECK_GEOMETRICAL) {
    // check dump with the selection by geometry
    isOk = isOk && checkDump(aSession, aFileForGeometryDump, aStore, anErrorByGeometry);
  }
  if (theCheckType & CHECK_WEAK) {
    isOk = isOk && checkDump(aSession, aFileForWeakDump, aStore, anErrorByWeak);
  }

  return isOk;
}

//--------------------------------------------------------------------------------------
