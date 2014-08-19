# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

IF(NOT CAS_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for OpenCascade (CAS) ...")
ENDIF()

SET(_CAS_VERSION_FILE_NAME Standard_Version.hxx)

# Include directories:
FIND_PATH(CAS_INCLUDE_DIRS ${_CAS_VERSION_FILE_NAME} PATH_SUFFIXES inc)
FIND_FILE(CAS_VERSION_FILE ${_CAS_VERSION_FILE_NAME} PATH_SUFFIXES inc)
MARK_AS_ADVANCED(CAS_VERSION_FILE)

# Is this a development version:
SET(CAS_VERSION_DEVELOPMENT 0)
IF(CAS_VERSION_FILE)
  FILE(STRINGS ${CAS_VERSION_FILE} CAS_VERSION_DEVELOPMENT_STR
      REGEX "^ *#define OCC_VERSION_DEVELOPMENT *\"dev\".*$")
  IF(CAS_VERSION_DEVELOPMENT_STR)
    SET(CAS_VERSION_DEVELOPMENT 1)
  ENDIF(CAS_VERSION_DEVELOPMENT_STR)
  
  # Extract normal version:
  FILE(STRINGS ${CAS_VERSION_FILE} _tmp
      REGEX "^ *#define OCC_VERSION_COMPLETE *\"[^\"]*\".*$")
  STRING(REGEX REPLACE ".*\"([^\"]*)\".*" "\\1" CAS_VERSION_STR "${_tmp}")
      
ENDIF()

IF(CAS_VERSION_DEVELOPMENT AND (NOT CAS_FIND_QUIETLY))
  MESSAGE(STATUS "OpenCascade: development version is being used!")
ENDIF()

# Win specific stuff:
IF(WIN32)
  IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    SET(CMAKE_LIBRARY_PATH $ENV{CAS_ROOT_DIR}/win32/libd)
  ELSE()
    SET(CMAKE_LIBRARY_PATH $ENV{CAS_ROOT_DIR}/win32/lib)
  ENDIF()
ENDIF()

# Definitions:
SET(CAS_DEFINITIONS "-DLIN -DLINTEL -DCSFDB")
SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -DNo_exception")

# No config.h file in the OPEN CASCADE on WINDOWS platform 
IF(NOT WIN32)
 SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -DHAVE_CONFIG_H")
ELSE()
 SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -DWNT")
ENDIF()

SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -DOCC_CONVERT_SIGNALS")

# Test for 64 bit machine:
IF(CMAKE_SIZEOF_VOID_P STREQUAL 8)
  SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -D_OCC64")
ENDIF()

IF(CAS_VERSION_DEVELOPMENT STREQUAL 1)
  SET(CAS_DEFINITIONS "${CAS_DEFINITIONS} -DCAS_VERSION_DEVELOPMENT")
ENDIF()

# Find Xmu library (X11 widgets?)
IF(NOT WIN32)
  FIND_LIBRARY(CAS_Xmu Xmu)
  IF(Xmu)
    SET(CAS_LDPATH ${Xmu})
  ENDIF()
ENDIF()

FIND_LIBRARY(CAS_FWOSPlugin FWOSPlugin )

MESSAGE(STATUS "FWOS " ${CAS_FWOSPlugin})

FIND_LIBRARY(CAS_PTKernel PTKernel )
FIND_LIBRARY(CAS_TKAdvTools TKAdvTools )
FIND_LIBRARY(CAS_TKBin TKBin )
FIND_LIBRARY(CAS_TKBinL TKBinL )
FIND_LIBRARY(CAS_TKBinTObj TKBinTObj )
FIND_LIBRARY(CAS_TKBinXCAF TKBinXCAF )
FIND_LIBRARY(CAS_TKBO TKBO )
FIND_LIBRARY(CAS_TKBool TKBool )
FIND_LIBRARY(CAS_TKBRep TKBRep )
FIND_LIBRARY(CAS_TKCAF TKCAF )
FIND_LIBRARY(CAS_TKCDF TKCDF )
FIND_LIBRARY(CAS_TKernel TKernel )
FIND_LIBRARY(CAS_TKFeat TKFeat )
FIND_LIBRARY(CAS_TKFillet TKFillet )
FIND_LIBRARY(CAS_TKG2d TKG2d )
FIND_LIBRARY(CAS_TKG3d TKG3d )
FIND_LIBRARY(CAS_TKGeomAlgo TKGeomAlgo )
FIND_LIBRARY(CAS_TKGeomBase TKGeomBase )
FIND_LIBRARY(CAS_TKHLR TKHLR )
FIND_LIBRARY(CAS_TKIGES TKIGES )
FIND_LIBRARY(CAS_TKLCAF TKLCAF )
FIND_LIBRARY(CAS_TKMath TKMath )
FIND_LIBRARY(CAS_TKMesh TKMesh )
FIND_LIBRARY(CAS_TKMeshVS TKMeshVS )
FIND_LIBRARY(CAS_TKNIS TKNIS )
FIND_LIBRARY(CAS_TKOffset TKOffset )
FIND_LIBRARY(CAS_TKOpenGl TKOpenGl )
FIND_LIBRARY(CAS_TKPCAF TKPCAF )
FIND_LIBRARY(CAS_TKPLCAF TKPLCAF )
FIND_LIBRARY(CAS_TKPrim TKPrim )
FIND_LIBRARY(CAS_TKPShape TKPShape )
FIND_LIBRARY(CAS_TKService TKService )
FIND_LIBRARY(CAS_TKShapeSchema TKShapeSchema )
FIND_LIBRARY(CAS_TKShHealing TKShHealing )
FIND_LIBRARY(CAS_TKStdLSchema TKStdLSchema )
FIND_LIBRARY(CAS_TKStdSchema TKStdSchema )
FIND_LIBRARY(CAS_TKSTEP TKSTEP )
FIND_LIBRARY(CAS_TKSTEP209 TKSTEP209 )
FIND_LIBRARY(CAS_TKSTEPAttr TKSTEPAttr )
FIND_LIBRARY(CAS_TKSTEPBase TKSTEPBase )
FIND_LIBRARY(CAS_TKSTL TKSTL )
FIND_LIBRARY(CAS_TKTObj TKTObj )
FIND_LIBRARY(CAS_TKTopAlgo TKTopAlgo )
FIND_LIBRARY(CAS_TKV3d TKV3d )
FIND_LIBRARY(CAS_TKVRML TKVRML )
FIND_LIBRARY(CAS_TKXCAF TKXCAF )
FIND_LIBRARY(CAS_TKXCAFSchema TKXCAFSchema )
FIND_LIBRARY(CAS_TKXDEIGES TKXDEIGES )
FIND_LIBRARY(CAS_TKXDESTEP TKXDESTEP )
FIND_LIBRARY(CAS_TKXMesh TKXMesh )
FIND_LIBRARY(CAS_TKXml TKXml )
FIND_LIBRARY(CAS_TKXmlL TKXmlL )
FIND_LIBRARY(CAS_TKXmlTObj TKXmlTObj )
FIND_LIBRARY(CAS_TKXmlXCAF TKXmlXCAF )
FIND_LIBRARY(CAS_TKXSBase TKXSBase )

SET(CAS_KERNEL ${CAS_TKernel} ${CAS_TKMath})
SET(CAS_OCAF ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKCDF} ${CAS_TKLCAF})
SET(CAS_VIEWER ${CAS_TKService} ${CAS_TKV3d} ${CAS_TKG3d} ${CAS_TKGeomBase} ${CAS_TKBRep})
SET(CAS_OCAFVIS ${CAS_TKCAF} ${CAS_TKBRep} ${CAS_TKG2d})
SET(CAS_MODELER ${CAS_TKG3d} ${CAS_TKGeomBase} ${CAS_TKGeomAlgo} ${CAS_TKBRep} ${CAS_TKTopAlgo} ${CAS_TKG2d})
SET(CAS_SHAPE ${CAS_TKShHealing} ${CAS_TKMesh} ${CAS_TKHLR})

SET(CAS_TKV3d_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKV3d})
SET(CAS_TKBool_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKBRep} ${CAS_TKG2d} ${CAS_TKG3d} ${CAS_TKGeomBase} ${CAS_TKGeomAlgo} ${CAS_TKTopAlgo} ${CAS_TKBool})
SET(CAS_TKBRep_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKBRep})
SET(CAS_TKIGES_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKXSBase} ${CAS_TKBRep} ${CAS_TKIGES})
SET(CAS_TKSTEP_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKXSBase} ${CAS_TKBRep} ${CAS_TKSTEP})
SET(CAS_TKSTL_EA ${CAS_TKernel} ${CAS_TKMath} ${CAS_TKSTL})
SET(CAS_TKCAF_EA ${CAS_TKPrim} ${CAS_TKCAF})

SET(CAS_TKV3d ${CAS_TKV3d_EA})
SET(CAS_TKBool ${CAS_TKBool_EA})
SET(CAS_TKBRep ${CAS_TKBRep_EA})
SET(CAS_TKIGES ${CAS_TKIGES_EA})
SET(CAS_TKSTEP ${CAS_TKSTEP_EA})
SET(CAS_TKSTL ${CAS_TKSTL_EA})
SET(CAS_TKCAF ${CAS_TKCAF_EA})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CAS 
      REQUIRED_VARS CAS_INCLUDE_DIRS  
        CAS_FWOSPlugin
        CAS_PTKernel
        CAS_TKAdvTools
        CAS_TKBin
        CAS_TKBinL
        CAS_TKBinTObj
        CAS_TKBinXCAF
        CAS_TKBO
        CAS_TKBool
        CAS_TKBRep
        CAS_TKCAF
        CAS_TKCDF
        CAS_TKernel
        CAS_TKFeat
        CAS_TKFillet
        CAS_TKG2d
        CAS_TKG3d
        CAS_TKGeomAlgo
        CAS_TKGeomBase
        CAS_TKHLR
        CAS_TKIGES
        CAS_TKLCAF
        CAS_TKMath
        CAS_TKMesh
        CAS_TKMeshVS
        CAS_TKNIS
        CAS_TKOffset
        CAS_TKOpenGl
        CAS_TKPCAF
        CAS_TKPLCAF
        CAS_TKPrim
        CAS_TKPShape
        CAS_TKService
        CAS_TKShapeSchema
        CAS_TKShHealing
        CAS_TKStdLSchema
        CAS_TKStdSchema
        CAS_TKSTEP
        CAS_TKSTEP209
        CAS_TKSTEPAttr
        CAS_TKSTEPBase
        CAS_TKSTL
        CAS_TKTObj
        CAS_TKTopAlgo
        CAS_TKV3d
        CAS_TKVRML
        CAS_TKXCAF
        CAS_TKXCAFSchema
        CAS_TKXDEIGES
        CAS_TKXDESTEP
        CAS_TKXMesh
        CAS_TKXml
        CAS_TKXmlL
        CAS_TKXmlTObj
        CAS_TKXmlXCAF
        CAS_TKXSBase
)

IF(CAS_FOUND AND NOT CAS_FIND_QUIETLY)
  MESSAGE(STATUS "Found OpenCascade version: ${CAS_VERSION_STR}")
ENDIF()


