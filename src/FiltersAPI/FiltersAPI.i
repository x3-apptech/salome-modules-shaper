// Copyright (C) 2014-2019  CEA/DEN, EDF R&D
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

/* FiltersAPI.i */

%module FiltersAPI

%{
  #include "FiltersAPI_swig.h"

  // fix for SWIG v2.0.4
  #define SWIGPY_SLICE_ARG(obj) ((PyObject*)(obj))
%}

%include "doxyhelp.i"

// import other modules
%import "ModelHighAPI.i"

// to avoid error on this
#define FILTERSAPI_EXPORT

// standard definitions
%include "typemaps.i"
%include "std_list.i"
%include "std_shared_ptr.i"

// shared pointers
%shared_ptr(FiltersAPI_Feature)
%shared_ptr(FiltersAPI_Filter)

// std::list -> []
%template(FilterList) std::list<std::shared_ptr<FiltersAPI_Filter> >;

// function with named parameters
%feature("kwargs") addFilter;

// all supported interfaces
%include "FiltersAPI_Feature.h"
%include "FiltersAPI_Filter.h"
%include "FiltersAPI_Selection.h"
