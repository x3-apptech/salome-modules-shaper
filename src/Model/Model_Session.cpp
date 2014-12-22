// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        Model_Session.cxx
// Created:     20 Mar 2014
// Author:      Mikhail PONIKAROV

#include <Model_Session.h>
#include <ModelAPI_Feature.h>
#include <ModelAPI_Plugin.h>
#include <Model_Data.h>
#include <Model_Document.h>
#include <Model_Application.h>
#include <Model_Events.h>
#include <Model_Validator.h>
#include <Events_Loop.h>
#include <Events_Error.h>
#include <Config_FeatureMessage.h>
#include <Config_AttributeMessage.h>
#include <Config_ValidatorMessage.h>
#include <Config_ModuleReader.h>

#include <TDF_CopyTool.hxx>
#include <TDF_DataSet.hxx>
#include <TDF_RelocationTable.hxx>
#include <TDF_ClosureTool.hxx>

using namespace std;

static Model_Session* myImpl = new Model_Session();

// t oredirect all calls to the root document
#define ROOT_DOC std::dynamic_pointer_cast<Model_Document>(moduleDocument())

bool Model_Session::load(const char* theFileName)
{
  return ROOT_DOC->load(theFileName);
}

bool Model_Session::save(const char* theFileName, std::list<std::string>& theResults)
{
  return ROOT_DOC->save(theFileName, theResults);
}

void Model_Session::closeAll()
{
  ROOT_DOC->close(true);
  Model_Application::getApplication()->deleteAllDocuments();
}

void Model_Session::startOperation()
{
  ROOT_DOC->startOperation();
  static std::shared_ptr<Events_Message> aStartedMsg
    (new Events_Message(Events_Loop::eventByName("StartOperation")));
  Events_Loop::loop()->send(aStartedMsg);
}

void Model_Session::finishOperation()
{
  ROOT_DOC->finishOperation();
}

void Model_Session::abortOperation()
{
  ROOT_DOC->abortOperation();
  // here the update mechanism may work after abort, so, supress the warnings about
  // modifications outside of the transactions
  bool aWasCheck = myCheckTransactions;
  myCheckTransactions = false;
  static std::shared_ptr<Events_Message> anAbortMsg
    (new Events_Message(Events_Loop::eventByName("AbortOperation")));
  Events_Loop::loop()->send(anAbortMsg);
  myCheckTransactions = true;
  myCheckTransactions = aWasCheck;
}

bool Model_Session::isOperation()
{
  return ROOT_DOC->isOperation();
}

bool Model_Session::isModified()
{
  return ROOT_DOC->isModified();
}

bool Model_Session::canUndo()
{
  return ROOT_DOC->canUndo();
}

void Model_Session::undo()
{
  ROOT_DOC->undo();
}

bool Model_Session::canRedo()
{
  return ROOT_DOC->canRedo();
}

void Model_Session::redo()
{
  ROOT_DOC->redo();
}

FeaturePtr Model_Session::createFeature(string theFeatureID)
{
  if (this != myImpl) {
    return myImpl->createFeature(theFeatureID);
  }

  // load all information about plugins, features and attributes
  LoadPluginsInfo();

  if (myPlugins.find(theFeatureID) != myPlugins.end()) {
    std::pair<std::string, std::string>& aPlugin = myPlugins[theFeatureID]; // plugin and doc kind
    if (!aPlugin.second.empty() && aPlugin.second != activeDocument()->kind()) {
      Events_Error::send(
          string("Feature '") + theFeatureID + "' can be created only in document '"
              + aPlugin.second + "' by the XML definition");
      return FeaturePtr();
    }
    myCurrentPluginName = aPlugin.first;
    if (myPluginObjs.find(myCurrentPluginName) == myPluginObjs.end()) {
      // load plugin library if not yet done
      Config_ModuleReader::loadPlugin(myCurrentPluginName);
    }
    if (myPluginObjs.find(myCurrentPluginName) != myPluginObjs.end()) {
      FeaturePtr aCreated = myPluginObjs[myCurrentPluginName]->createFeature(theFeatureID);
      if (!aCreated) {
        Events_Error::send(
            string("Can not initialize feature '") + theFeatureID + "' in plugin '"
                + myCurrentPluginName + "'");
      }
      return aCreated;
    } else {
      Events_Error::send(string("Can not load plugin '") + myCurrentPluginName + "'");
    }
  } else {
    Events_Error::send(string("Feature '") + theFeatureID + "' not found in any plugin");
  }

  return FeaturePtr();  // return nothing
}

std::shared_ptr<ModelAPI_Document> Model_Session::moduleDocument()
{
  return std::shared_ptr<ModelAPI_Document>(
      Model_Application::getApplication()->getDocument("root"));
}

std::shared_ptr<ModelAPI_Document> Model_Session::document(std::string theDocID)
{
  return std::shared_ptr<ModelAPI_Document>(
      Model_Application::getApplication()->getDocument(theDocID));
}

bool Model_Session::hasModuleDocument()
{
  return Model_Application::getApplication()->hasDocument("root");
}

std::shared_ptr<ModelAPI_Document> Model_Session::activeDocument()
{
  if (!myCurrentDoc || !Model_Application::getApplication()->hasDocument(myCurrentDoc->id()))
    myCurrentDoc = moduleDocument();
  return myCurrentDoc;
}

void Model_Session::setActiveDocument(
  std::shared_ptr<ModelAPI_Document> theDoc, bool theSendSignal)
{
  if (myCurrentDoc != theDoc) {
    myCurrentDoc = theDoc;
    if (theSendSignal) {
      static std::shared_ptr<Events_Message> aMsg(new Events_Message(Events_Loop::eventByName("CurrentDocumentChanged")));
      Events_Loop::loop()->send(aMsg);
    }
  }
}

std::list<std::shared_ptr<ModelAPI_Document> > Model_Session::allOpenedDocuments()
{
  list<std::shared_ptr<ModelAPI_Document> > aResult;
  aResult.push_back(moduleDocument());
  // add subs recursively
  list<std::shared_ptr<ModelAPI_Document> >::iterator aDoc = aResult.begin();
  for(; aDoc != aResult.end(); aDoc++) {
    DocumentPtr anAPIDoc = *aDoc;
    std::shared_ptr<Model_Document> aDoc = std::dynamic_pointer_cast<Model_Document>(anAPIDoc);
    if (aDoc) {
      std::set<std::string>::const_iterator aSubIter = aDoc->subDocuments().cbegin();
      for(; aSubIter != aDoc->subDocuments().cend(); aSubIter++) {
        if (!Model_Application::getApplication()->isLoadByDemand(*aSubIter)) {
          aResult.push_back(Model_Application::getApplication()->getDocument(*aSubIter));
        }
      }
    }
  }
  return aResult;
}

std::shared_ptr<ModelAPI_Document> Model_Session::copy(
    std::shared_ptr<ModelAPI_Document> theSource, std::string theID)
{
  // create a new document
  std::shared_ptr<Model_Document> aNew = std::dynamic_pointer_cast<Model_Document>(
      Model_Application::getApplication()->getDocument(theID));
  // make a copy of all labels
  TDF_Label aSourceRoot = std::dynamic_pointer_cast<Model_Document>(theSource)->document()->Main()
      .Father();
  TDF_Label aTargetRoot = aNew->document()->Main().Father();
  Handle(TDF_DataSet) aDS = new TDF_DataSet;
  aDS->AddLabel(aSourceRoot);
  TDF_ClosureTool::Closure(aDS);
  Handle(TDF_RelocationTable) aRT = new TDF_RelocationTable;
  aRT->SetRelocation(aSourceRoot, aTargetRoot);
  TDF_CopyTool::Copy(aDS, aRT);

  aNew->synchronizeFeatures(false, true);
  return aNew;
}

Model_Session::Model_Session()
{
  myPluginsInfoLoaded = false;
  myCheckTransactions = true;
  ModelAPI_Session::setSession(std::shared_ptr<ModelAPI_Session>(this));
  // register the configuration reading listener
  Events_Loop* aLoop = Events_Loop::loop();
  static const Events_ID kFeatureEvent = Events_Loop::eventByName(Config_FeatureMessage::MODEL_EVENT());
  aLoop->registerListener(this, kFeatureEvent);
  aLoop->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_CREATED), 0, true);
  aLoop->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_UPDATED), 0, true);
  aLoop->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_DELETED), 0, true);
  aLoop->registerListener(this, Events_Loop::eventByName(EVENT_VALIDATOR_LOADED));
}

void Model_Session::processEvent(const std::shared_ptr<Events_Message>& theMessage)
{
  static const Events_ID kFeatureEvent = Events_Loop::eventByName(Config_FeatureMessage::MODEL_EVENT());
  static const Events_ID kValidatorEvent = Events_Loop::eventByName(EVENT_VALIDATOR_LOADED);
  if (theMessage->eventID() == kFeatureEvent) {
    const std::shared_ptr<Config_FeatureMessage> aMsg = 
      std::dynamic_pointer_cast<Config_FeatureMessage>(theMessage);
    if (aMsg) {
      // proccess the plugin info, load plugin
      if (myPlugins.find(aMsg->id()) == myPlugins.end()) {
        myPlugins[aMsg->id()] = std::pair<std::string, std::string>(
          aMsg->pluginLibrary(), aMsg->documentKind());
      }
    } else {
      const std::shared_ptr<Config_AttributeMessage> aMsgAttr = 
        std::dynamic_pointer_cast<Config_AttributeMessage>(theMessage);
      if (aMsgAttr) {
        if (!aMsgAttr->isObligatory()) {
          validators()->registerNotObligatory(aMsgAttr->featureId(), aMsgAttr->attributeId());
        }
        if(aMsgAttr->isConcealment()) {
          validators()->registerConcealment(aMsgAttr->featureId(), aMsgAttr->attributeId());
        }
        
      }
    }
    // plugins information was started to load, so, it will be loaded
    myPluginsInfoLoaded = true;
  } else if (theMessage->eventID() == kValidatorEvent) {
    std::shared_ptr<Config_ValidatorMessage> aMsg = 
      std::dynamic_pointer_cast<Config_ValidatorMessage>(theMessage);
    if (aMsg) {
      if (aMsg->attributeId().empty()) {  // feature validator
        validators()->assignValidator(aMsg->validatorId(), aMsg->featureId(), aMsg->parameters());
      } else {  // attribute validator
        validators()->assignValidator(aMsg->validatorId(), aMsg->featureId(), aMsg->attributeId(),
                                      aMsg->parameters());
      }
    }
  } else {  // create/update/delete
    if (myCheckTransactions && !isOperation())
      Events_Error::send("Modification of data structure outside of the transaction");
  }
}

void Model_Session::LoadPluginsInfo()
{
  if (myPluginsInfoLoaded)  // nothing to do
    return;

  // Read plugins information from XML files
  Config_ModuleReader aXMLReader(Config_FeatureMessage::MODEL_EVENT());
  aXMLReader.readAll();
}

void Model_Session::registerPlugin(ModelAPI_Plugin* thePlugin)
{
  myPluginObjs[myCurrentPluginName] = thePlugin;
  static Events_ID EVENT_LOAD = Events_Loop::loop()->eventByName(EVENT_PLUGIN_LOADED);
  ModelAPI_EventCreator::get()->sendUpdated(ObjectPtr(), EVENT_LOAD);
  Events_Loop::loop()->flush(EVENT_LOAD);
  // If the plugin has an ability to process GUI events, register it
  Events_Listener* aListener = dynamic_cast<Events_Listener*>(thePlugin);
  if (aListener) {
    Events_Loop* aLoop = Events_Loop::loop();
    static Events_ID aStateRequestEventId =
        Events_Loop::loop()->eventByName(EVENT_FEATURE_STATE_REQUEST);
    aLoop->registerListener(aListener, aStateRequestEventId);
  }
}

ModelAPI_ValidatorsFactory* Model_Session::validators()
{
  static Model_ValidatorsFactory* aFactory = new Model_ValidatorsFactory;
  return aFactory;
}
