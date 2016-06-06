// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:        Config_Translator.h
// Created:     31 May 2016
// Author:      Vitaly SMETANNIKOV

#ifndef Config_Translator_H
#define Config_Translator_H

#include "Config_def.h"
#include <Events_InfoMessage.h>

#include <string>
#include <map>

//#define MISSED_TRANSLATION

/**
 * \class Config_Translator
 * \ingroup Config
 * \brief Class for messages translation on different languages. It can load TS
 * files wich contain translation string and provides translations of messages from source code
 */
class Config_Translator
{
public:
  /// A data type of dictionary <KeyString, ResultString>
  typedef std::map<std::string, std::string> Dictionary;

  /// A data type of Translator with structure <Context, Dictionary>
  typedef std::map<std::string, Dictionary> Translator;

  /**
  * Load translations from TS file
  * \param theFileName a TS file name with full path
  */
  static CONFIG_EXPORT bool load(const std::string& theFileName);

  /**
  * Returns translation from the given info message.
  * If transdlation is not exists then it returns a string 
  * from the info data without translation
  * \param theInfo an info message
  */
  static CONFIG_EXPORT std::string translate(std::shared_ptr<Events_InfoMessage> theInfo);

  /**
  * Returns translation from the given data.
  * If transdlation is not exists then it returns a string 
  * from the info data without translation
  * \param theContext context of the message (Feature Id)
  * \param theMessage a message which dave to be translated
  * \param theParams a list of parameters (can be empty)
  */
  static CONFIG_EXPORT std::string translate(const std::string& theContext,
    const std::string& theMessage, 
    const std::list<std::string>& theParams = std::list<std::string>());

#ifdef _DEBUG
#ifdef MISSED_TRANSLATION
  static CONFIG_EXPORT void saveMissedTranslations();
#endif
#endif


private:
  static Translator myTranslator;

#ifdef _DEBUG
#ifdef MISSED_TRANSLATION
  static Translator myMissed;
#endif
#endif
};

#endif