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

/* GeomAPI.i */
%module GeomAPI
%{
  #include "GeomAPI_swig.h"
%}

// to avoid error on this
#define GEOMAPI_EXPORT

// standard definitions
%include "typemaps.i"
%include "std_string.i"
%include "std_shared_ptr.i"

// shared pointers
%shared_ptr(GeomAPI_AISObject)
%shared_ptr(GeomAPI_Ax1)
%shared_ptr(GeomAPI_Ax2)
%shared_ptr(GeomAPI_Ax3)
%shared_ptr(GeomAPI_Circ)
%shared_ptr(GeomAPI_Circ2d)
%shared_ptr(GeomAPI_Curve)
%shared_ptr(GeomAPI_DataMapOfShapeMapOfShapes)
%shared_ptr(GeomAPI_DataMapOfShapeShape)
%shared_ptr(GeomAPI_Dir)
%shared_ptr(GeomAPI_Dir2d)
%shared_ptr(GeomAPI_Edge)
%shared_ptr(GeomAPI_Face)
%shared_ptr(GeomAPI_ICustomPrs)
%shared_ptr(GeomAPI_Interface)
%shared_ptr(GeomAPI_IPresentable)
%shared_ptr(GeomAPI_Lin)
%shared_ptr(GeomAPI_Lin2d)
%shared_ptr(GeomAPI_PlanarEdges)
%shared_ptr(GeomAPI_Pln)
%shared_ptr(GeomAPI_Pnt)
%shared_ptr(GeomAPI_Pnt2d)
%shared_ptr(GeomAPI_Shape)
%shared_ptr(GeomAPI_ShapeExplorer)
%shared_ptr(GeomAPI_ShapeIterator)
%shared_ptr(GeomAPI_Vertex)
%shared_ptr(GeomAPI_XY)
%shared_ptr(GeomAPI_XYZ)
%shared_ptr(GeomAPI_Trsf)
%shared_ptr(GeomAPI_Wire)
%shared_ptr(GeomAPI_Shell)
%shared_ptr(GeomAPI_Solid)
%shared_ptr(GeomAPI_Box)


// all supported interfaces
%include "GeomAPI_Interface.h"
%include "GeomAPI_Shape.h"
%include "GeomAPI_AISObject.h"
%include "GeomAPI_Ax1.h"
%include "GeomAPI_Ax2.h"
%include "GeomAPI_Ax3.h"
%include "GeomAPI_Circ.h"
%include "GeomAPI_Circ2d.h"
%include "GeomAPI_Curve.h"
%include "GeomAPI_DataMapOfShapeMapOfShapes.h"
%include "GeomAPI_DataMapOfShapeShape.h"
%include "GeomAPI_Dir.h"
%include "GeomAPI_Dir2d.h"
%include "GeomAPI_Edge.h"
%include "GeomAPI_Face.h"
%include "GeomAPI_ICustomPrs.h"
%include "GeomAPI_IPresentable.h"
%include "GeomAPI_Lin.h"
%include "GeomAPI_Lin2d.h"
%include "GeomAPI_PlanarEdges.h"
%include "GeomAPI_Pln.h"
%include "GeomAPI_Pnt.h"
%include "GeomAPI_Pnt2d.h"
%include "GeomAPI_ShapeExplorer.h"
%include "GeomAPI_ShapeIterator.h"
%include "GeomAPI_Vertex.h"
%include "GeomAPI_XY.h"
%include "GeomAPI_XYZ.h"
%include "GeomAPI_Trsf.h"
%include "GeomAPI_Wire.h"
%include "GeomAPI_Shell.h"
%include "GeomAPI_Solid.h"
%include "GeomAPI_Box.h"
