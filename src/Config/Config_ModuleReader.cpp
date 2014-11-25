/*
 * Config_ModuleReader.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: sbh
 */

#include <Config_Keywords.h>
#include <Config_Common.h>
#include <Config_ModuleReader.h>
#include <Config_FeatureReader.h>
#include <Events_Error.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

// Have to be included before std headers
#include <Python.h>

//Necessary for cerr
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

std::map<std::string, Config_ModuleReader::PluginType> Config_ModuleReader::myPluginTypes;

Config_ModuleReader::Config_ModuleReader(const char* theEventGenerated)
    : Config_XMLReader(PLUGIN_FILE),
      myEventGenerated(theEventGenerated)
{
}

Config_ModuleReader::~Config_ModuleReader()
{
}

const std::map<std::string, std::string>& Config_ModuleReader::featuresInFiles() const
{
  return myFeaturesInFiles;
}

/*
 * Get module name from plugins.xml
 * (property "module")
 */
std::string Config_ModuleReader::getModuleName()
{
  xmlNodePtr aRoot = findRoot();
  return getProperty(aRoot, PLUGINS_MODULE);
}

/*
 *
 */
void Config_ModuleReader::processNode(xmlNodePtr theNode)
{
  if (isNode(theNode, NODE_PLUGIN, NULL)) {
    std::string aPluginConf = getProperty(theNode, PLUGIN_CONFIG);
    std::string aPluginLibrary = getProperty(theNode, PLUGIN_LIBRARY);
    std::string aPluginScript = getProperty(theNode, PLUGIN_SCRIPT);
    std::string aPluginName = addPlugin(aPluginLibrary, aPluginScript, aPluginConf);

    std::list<std::string> aFeatures = importPlugin(aPluginName, aPluginConf);
    std::list<std::string>::iterator it = aFeatures.begin();
    for (; it != aFeatures.end(); it++) {
      myFeaturesInFiles[*it] = aPluginConf;
    }
  }
}

bool Config_ModuleReader::processChildren(xmlNodePtr theNode)
{
  return isNode(theNode, NODE_PLUGINS, NULL);
}

std::list<std::string> Config_ModuleReader::importPlugin(const std::string& thePluginLibrary,
                                                         const std::string& thePluginXmlConf)
{
  if (thePluginXmlConf.empty()) {  //probably a third party library
    loadLibrary(thePluginLibrary);
    return std::list<std::string>();
  }

  Config_FeatureReader aReader = Config_FeatureReader(thePluginXmlConf,
                                                      thePluginLibrary,
                                                      myEventGenerated);
  aReader.readAll();
  return aReader.features();
}

std::string Config_ModuleReader::addPlugin(const std::string& aPluginLibrary,
                                           const std::string& aPluginScript,
                                           const std::string& aPluginConf)
{
  PluginType aType = Config_ModuleReader::Binary;
  std::string aPluginName;
  if (!aPluginLibrary.empty()) {
    aPluginName = aPluginLibrary;
    if (aPluginConf.empty()) {
      aType = Config_ModuleReader::Intrenal;
    }
  } else if (!aPluginScript.empty()) {
    aPluginName = aPluginScript;
    aType = Config_ModuleReader::Python;
  }
  if(!aPluginName.empty()) {
    myPluginTypes[aPluginName] = aType;

  }
  return aPluginName;
}

void Config_ModuleReader::loadPlugin(const std::string thePluginName)
{
  PluginType aType = Config_ModuleReader::Binary;
  if(myPluginTypes.find(thePluginName) != myPluginTypes.end()) {
    aType = myPluginTypes.at(thePluginName);
  }
  switch (aType) {
    case Config_ModuleReader::Python:
      loadScript(thePluginName);
      break;
    case Config_ModuleReader::Binary:
    case Config_ModuleReader::Intrenal:
    default:
      loadLibrary(thePluginName);
      break;
  }
}

void Config_ModuleReader::loadScript(const std::string theFileName)
{
  std::string aPythonFile = theFileName + ".py";
  /* aquire python thread */
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject* module = PyImport_ImportModule(aPythonFile.c_str());
  /* release python thread */
  PyGILState_Release(gstate);
}

void Config_ModuleReader::loadLibrary(const std::string theLibName)
{
  std::string aFileName = library(theLibName);
  if (aFileName.empty())
    return;

#ifdef WIN32
  HINSTANCE aModLib = ::LoadLibrary(aFileName.c_str());
#else
  void* aModLib = dlopen( aFileName.c_str(), RTLD_LAZY | RTLD_GLOBAL );
#endif
  if(!aModLib && theLibName != "DFBrowser") { // don't show error for internal debugging tool
    std::string anErrorMsg = "Failed to load " + aFileName;
    #ifndef WIN32
    anErrorMsg += ": " + std::string(dlerror());
    #endif
    Events_Error::send(anErrorMsg);
  }
}

