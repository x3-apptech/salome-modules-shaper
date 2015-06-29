// Copyright (C) 2014-20xx CEA/DEN, EDF R&D

// File:    GEOMALGOAPI_STEPExport.cpp
// Created: May 14, 2015
// Author:  Sergey POKHODENKO

#include <GeomAlgoAPI_STEPExport.h>

#include "GeomAlgoAPI_Tools.h"

// OOCT includes
#include <IFSelect_ReturnStatus.hxx>
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>

namespace STEPExport {

bool Export(const std::string& theFileName,
            const std::string& theFormatName,
            const TopoDS_Shape& theShape,
            std::string& theError)
{
  #ifdef _DEBUG
  std::cout << "Export STEP into file " << theFileName << std::endl;
  #endif

  try
  {
    // Set "C" numeric locale to save numbers correctly
    GeomAlgoAPI_Tools::Localizer loc;

    IFSelect_ReturnStatus status ;
    //VRV: OCC 4.0 migration
    STEPControl_Writer aWriter;
    Interface_Static::SetCVal("xstep.cascade.unit","M");
    Interface_Static::SetCVal("write.step.unit", "M");
    Interface_Static::SetIVal("write.step.nonmanifold", 1);
    status = aWriter.Transfer( theShape, STEPControl_AsIs );
    //VRV: OCC 4.0 migration
    if( status == IFSelect_RetDone )
      status = aWriter.Write( theFileName.c_str() );

    // Return previous locale
    if( status == IFSelect_RetDone )
      return true;
  }
  catch (Standard_Failure)
  {
    theError = "Exception catched in STEPExport";
  }
  return false;
}

} // namespace STEPExport
