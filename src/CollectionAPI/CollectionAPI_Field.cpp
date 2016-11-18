// Copyright (C) 2014-20xx CEA/DEN, EDF R&D -->

// File:        CollectionAPI_Field.cpp
// Created:     16 Nov 2016
// Author:      Mikhail Ponikarov

#include "CollectionAPI_Field.h"

#include <ModelHighAPI_Dumper.h>
#include <ModelHighAPI_Integer.h>
#include <ModelHighAPI_Selection.h>
#include <ModelHighAPI_Tools.h>
#include <ModelAPI_AttributeTables.h>
#include <ModelAPI_AttributeStringArray.h>

#include <algorithm> // for std::transform

//=================================================================================================
CollectionAPI_Field::CollectionAPI_Field(const std::shared_ptr<ModelAPI_Feature>& theFeature)
: ModelHighAPI_Interface(theFeature)
{
  initialize();
}

//=================================================================================================
CollectionAPI_Field::~CollectionAPI_Field()
{
}

//=================================================================================================
void CollectionAPI_Field::setSelection(const std::list<ModelHighAPI_Selection>& theFieldList)
{
  fillAttribute(theFieldList, myselection);
  execute();
}

//=================================================================================================
void CollectionAPI_Field::setComponentsNames(const std::list<std::string>& theNames)
{
  fillAttribute(theNames, mycomponentsNames);
  execute();
}

//=================================================================================================
void CollectionAPI_Field::setValuesType(const std::string& theType)
{
  fillAttribute(int(valueTypeByStr(theType)), myvaluesType);
  myvalues->setType(valueTypeByStr(theType));
  execute();
}

//=================================================================================================
void CollectionAPI_Field::setStepsNum(const ModelHighAPI_Integer& theSteps)
{
  fillAttribute(theSteps, mystepsNum);
  mystamps->setSize(theSteps.intValue());
  execute();
}

//=================================================================================================
void CollectionAPI_Field::setStamps(const std::list<ModelHighAPI_Integer>& theStamps)
{
  fillAttribute(theStamps, mystamps);
  execute();
}

//=================================================================================================
#define addStepImplementation(type, fieldType, type2, fieldType2, type3, fieldType3) \
void CollectionAPI_Field::addStep(const ModelHighAPI_Integer& theStepNum, \
  const ModelHighAPI_Integer& theStamp, \
  const std::list<std::list<type> >& theComponents) \
{ \
  myvalues->setSize(myselection->size() + 1, \
    mycomponentsNames->size(), mystepsNum->value()); \
  mystamps->setValue(theStepNum.intValue(), theStamp.intValue()); \
  int aRowIndex = 0; \
  std::list<std::list<type> >::const_iterator \
    aRowsIter = theComponents.begin(); \
  for(; aRowsIter != theComponents.end(); aRowsIter++, aRowIndex++) { \
    int aColIndex = 0; \
    std::list<type>::const_iterator aColIter = aRowsIter->begin(); \
    for(; aColIter != aRowsIter->end(); aColIter++, aColIndex++) { \
      ModelAPI_AttributeTables::Value aVal; \
      aVal.fieldType = *aColIter; \
      aVal.fieldType2 = type2(*aColIter); \
      aVal.fieldType3 = type3(*aColIter); \
      myvalues->setValue(aVal, aRowIndex, aColIndex, theStepNum.intValue()); \
    } \
  } \
  execute(); \
}

addStepImplementation(double, myDouble, int, myInt, bool, myBool);
addStepImplementation(int, myInt, double, myDouble, bool, myBool);
addStepImplementation(bool, myBool, int, myInt, double, myDouble);
addStepImplementation(std::string, myStr, std::string, myStr, std::string, myStr);

//=================================================================================================
void CollectionAPI_Field::dump(ModelHighAPI_Dumper& theDumper) const
{
  FeaturePtr aBase = feature();
  const std::string& aDocName = theDumper.name(aBase->document());

  theDumper<<aBase<<" = model.addField("<<aDocName<<", "<<mystepsNum->value()<<", \""
    <<strByValueType(ModelAPI_AttributeTables::ValueType(myvaluesType->value()))<<"\", "
    <<mycomponentsNames->size()<<", ";
  theDumper<<mycomponentsNames<<", ";
  theDumper<<myselection<<")"<<std::endl;
  // set values step by step
  for(int aStep = 0; aStep < myvalues->tables(); aStep++) {
    theDumper<<aBase<<".addStep("<<aStep<<", "<<mystamps->value(aStep)<<", [";
    for(int aRow = 0; aRow < myvalues->rows(); aRow++) {
      if (aRow != 0)
        theDumper<<", ";
      theDumper<<"[";
      for(int aCol = 0; aCol < myvalues->columns(); aCol++) {
        if (aCol != 0)
          theDumper<<", ";
        switch(myvaluesType->value()) {
        case ModelAPI_AttributeTables::BOOLEAN:
          theDumper<<myvalues->value(aRow, aCol, aStep).myBool;
          break;
        case ModelAPI_AttributeTables::INTEGER:
          theDumper<<myvalues->value(aRow, aCol, aStep).myInt;
          break;
        case ModelAPI_AttributeTables::DOUBLE:
          theDumper<<myvalues->value(aRow, aCol, aStep).myDouble;
          break;
        case ModelAPI_AttributeTables::STRING:
          theDumper<<'"'<<myvalues->value(aRow, aCol, aStep).myStr<<'"';
          break;
        }
      }
      theDumper<<"]";
    }
    theDumper<<"])"<<std::endl;
  }
}

//=================================================================================================
FieldPtr addField(const std::shared_ptr<ModelAPI_Document>& thePart,
                  const ModelHighAPI_Integer& theStepsNum,
                  const std::string& theComponentType,
                  const int theComponentsNum,
                  const std::list<std::string>& theComponentNames,
                  const std::list<ModelHighAPI_Selection>& theSelectionList)
{
  std::shared_ptr<ModelAPI_Feature> aFeature = thePart->addFeature(CollectionAPI_Field::ID());
  std::shared_ptr<CollectionAPI_Field> aField(new CollectionAPI_Field(aFeature));
  aField->setStepsNum(theStepsNum);
  aField->setValuesType(theComponentType);
  aField->setComponentsNames(theComponentNames);
  aField->setSelection(theSelectionList);

  return aField;
}
