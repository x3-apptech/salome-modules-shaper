// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

/*
 * PartSet_WidgetFileSelector.cpp
 *
 *  Created on: May 18, 2015
 *      Author: spo
 */

#include "PartSet_WidgetFileSelector.h"

#include <ModelAPI_AttributeString.h>

PartSet_WidgetFileSelector::PartSet_WidgetFileSelector(QWidget* theParent,
                                                       ModuleBase_IWorkshop* theWorkshop,
                                                       const Config_WidgetAPI* theData,
                                                       const std::string& theParentId)
: ModuleBase_WidgetFileSelector(theParent, theData, theParentId)
, myWorkshop(theWorkshop)
{
}

bool PartSet_WidgetFileSelector::restoreValue()
{
  // A rare case when plugin was not loaded.
  if (!myFeature)
    return false;

  DataPtr aData = myFeature->data();
  AttributeStringPtr aStringAttr = aData->string("export_file_format");
  mySelectedFilter = formatToFilter(shortFormatToFullFormat(QString::fromStdString(aStringAttr->value())));

  return ModuleBase_WidgetFileSelector::restoreValue();
}

bool PartSet_WidgetFileSelector::storeValueCustom() const
{
  // A rare case when plugin was not loaded.
  if (!myFeature)
    return false;

  DataPtr aData = myFeature->data();
  AttributeStringPtr aStringAttr = aData->string("export_file_format");
  aStringAttr->setValue(filterToShortFormat(mySelectedFilter).toStdString());

  return ModuleBase_WidgetFileSelector::storeValueCustom();
}

QString PartSet_WidgetFileSelector::filterToShortFormat( const QString & theFilter )
{
  return theFilter.section(' ', 0, 0);
}

QString PartSet_WidgetFileSelector::shortFormatToFullFormat( const QString & theFormat ) const
{
  foreach(const QString & eachFormat, getValidatorFormats())
    if (filterToShortFormat(formatToFilter(eachFormat)) == theFormat)
      return eachFormat;
  return QString();
}
