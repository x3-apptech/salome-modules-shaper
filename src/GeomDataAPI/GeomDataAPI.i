/* GeomDataAPI.i */
%module GeomDataAPI
%{
  #include "ModelAPI_Attribute.h"
  #include "GeomDataAPI.h"
  #include "GeomDataAPI_Point.h"
  #include "GeomDataAPI_Dir.h"
  #include "GeomDataAPI_Point2D.h"

  #include <memory>
  #include <string>
  #include <list>
  
  template<class T> std::shared_ptr<T> castTo(std::shared_ptr<ModelAPI_Attribute> theObject) 
  { 
    return std::dynamic_pointer_cast<T>(theObject); 
  } 
%}

// to avoid error on this
#define GEOMDATAAPI_EXPORT
#define MODELAPI_EXPORT

// standard definitions
%include "typemaps.i"
%include "std_string.i"
%include "std_list.i"
%include "std_shared_ptr.i"

%shared_ptr(ModelAPI_Attribute)
%shared_ptr(GeomDataAPI_Point)
%shared_ptr(GeomDataAPI_Dir)
%shared_ptr(GeomDataAPI_Point2D)

// all supported interfaces
%include "ModelAPI_Attribute.h"
%include "GeomDataAPI_Point.h"
%include "GeomDataAPI_Dir.h"
%include "GeomDataAPI_Point2D.h"

template<class T> std::shared_ptr<T> castTo(std::shared_ptr<ModelAPI_Attribute> theObject);
%template(geomDataAPI_Point) castTo<GeomDataAPI_Point>;
%template(geomDataAPI_Dir) castTo<GeomDataAPI_Dir>;
%template(geomDataAPI_Point2D) castTo<GeomDataAPI_Point2D>;
