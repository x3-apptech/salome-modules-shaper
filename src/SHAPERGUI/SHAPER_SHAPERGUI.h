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

#ifndef SHAPER_SHAPERGUI_H
#define SHAPER_SHAPERGUI_H

#if defined SHAPERGUI_EXPORTS
#if defined WIN32
#define SHAPERGUI_EXPORT     __declspec( dllexport )
#else
#define SHAPERGUI_EXPORT
#endif
#else
#if defined WIN32
#define SHAPERGUI_EXPORT     __declspec( dllimport )
#else
#define SHAPERGUI_EXPORT
#endif
#endif

#endif

