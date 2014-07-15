// File:        Model_Update.cxx
// Created:     25 Jun 2014
// Author:      Mikhail PONIKAROV

#include <Model_Update.h>
#include <ModelAPI_Feature.h>
#include <ModelAPI_Data.h>
#include <ModelAPI_Document.h>
#include <ModelAPI_Events.h>
#include <ModelAPI_AttributeReference.h>
#include <ModelAPI_AttributeRefList.h>
#include <ModelAPI_Result.h>
#include <Events_Loop.h>

using namespace std;

Model_Update MY_INSTANCE; /// the only one instance initialized on load of the library

Model_Update::Model_Update()
{
  Events_Loop::loop()->registerListener(this, Events_Loop::eventByName(EVENT_OBJECT_UPDATED));
}

void Model_Update::processEvent(const Events_Message* theMessage)
{
  const ModelAPI_ObjectUpdatedMessage* aMsg = 
    dynamic_cast<const ModelAPI_ObjectUpdatedMessage*>(theMessage);
  myInitial = aMsg->objects();
  // collect all documents involved into the update
  set<boost::shared_ptr<ModelAPI_Document> > aDocs;
  set<boost::shared_ptr<ModelAPI_Object> >::iterator aFIter = myInitial.begin();
  for(; aFIter != myInitial.end(); aFIter++) {
    aDocs.insert((*aFIter)->document());
  }
  // iterate all features of features-documents to update them
  set<boost::shared_ptr<ModelAPI_Document> >::iterator aDIter = aDocs.begin();
  for(; aDIter != aDocs.end(); aDIter++) {
    int aNbFeatures = (*aDIter)->size(ModelAPI_Feature::group());
    for(int aFIndex = 0; aFIndex < aNbFeatures; aFIndex++) {
      boost::shared_ptr<ModelAPI_Object> aFeature = boost::dynamic_pointer_cast<ModelAPI_Object>
        ((*aDIter)->object(ModelAPI_Feature::group(), aFIndex));
      if (aFeature)
        updateObject(aFeature);
    }
  }
  myUpdated.clear();
  // flush
  static Events_ID EVENT_DISP = Events_Loop::loop()->eventByName(EVENT_OBJECT_TO_REDISPLAY);
  Events_Loop::loop()->flush(EVENT_DISP);
}

bool Model_Update::updateObject(boost::shared_ptr<ModelAPI_Object> theObject)
{
  // check it is already processed
  if (myUpdated.find(theObject) != myUpdated.end())
    return myUpdated[theObject];
  // check all features this feature depended on (recursive call of updateFeature)
  bool anExecute = myInitial.find(theObject) != myInitial.end();
  bool aMustbeUpdated = myInitial.find(theObject) != myInitial.end();
  FeaturePtr aRealFeature = boost::dynamic_pointer_cast<ModelAPI_Feature>(theObject);
  if (aRealFeature) { // only real feature contains references to other objects
    // references
    list<boost::shared_ptr<ModelAPI_Attribute> > aRefs = 
      theObject->data()->attributes(ModelAPI_AttributeReference::type());
    list<boost::shared_ptr<ModelAPI_Attribute> >::iterator aRefsIter = aRefs.begin();
    for(; aRefsIter != aRefs.end(); aRefsIter++) {
      boost::shared_ptr<ModelAPI_Object> aSub =
        boost::dynamic_pointer_cast<ModelAPI_AttributeReference>(*aRefsIter)->value();
      if (aSub && aSub != theObject && updateObject(aSub))
        aMustbeUpdated = true;
    }
    // lists of references
    aRefs = theObject->data()->attributes(ModelAPI_AttributeRefList::type());
    for(aRefsIter = aRefs.begin(); aRefsIter != aRefs.end(); aRefsIter++) {
      list<ObjectPtr> aListRef = 
        boost::dynamic_pointer_cast<ModelAPI_AttributeRefList>(*aRefsIter)->list();
      list<ObjectPtr>::iterator aListIter = aListRef.begin();
      for(; aListIter != aListRef.end(); aListIter++) {
        boost::shared_ptr<ModelAPI_Object> aSub = *aListIter;
        if (aSub && updateObject(aSub))
          aMustbeUpdated = true;
      }
    }
    // execute feature if it must be updated
    anExecute = aMustbeUpdated || anExecute;
    if (anExecute) {
      aRealFeature->execute();
      // redisplay all results
      static Events_ID EVENT_DISP = Events_Loop::loop()->eventByName(EVENT_OBJECT_TO_REDISPLAY);
      const std::list<boost::shared_ptr<ModelAPI_Result> >& aResults = aRealFeature->results();
      std::list<boost::shared_ptr<ModelAPI_Result> >::const_iterator aRIter = aResults.begin();
      for(; aRIter != aResults.cend(); aRIter++) {
        boost::shared_ptr<ModelAPI_Result> aRes = *aRIter;
        ModelAPI_EventCreator::get()->sendUpdated(aRes, EVENT_DISP);
      }
    }
  }
  myUpdated[theObject] = anExecute;
  return anExecute;
}
