## Copyright (C) 2014-20xx CEA/DEN, EDF R&D

#
# NewGeom unit test system
# How to use:
# INCLUDE(UnitTest)
# ...
# ADD_UNIT_TESTS(TestUndoRedo.py
#                TestResultBody.py)
# or
# SET(PROJECT_UNIT_TESTS
#     TestUndoRedo.py
#     TestWhatever.py  
#    )
# ADD_UNIT_TESTS(${PROJECT_UNIT_TESTS})
#
# Where "TestUndoRedo.py" and "TestResultBody.py" are names 
# of python test scripts in the "./Test" directory.
# This macro will add "Subproject_" as prefix and 
# remove ".py" for the test name automaticaly. 
# 
# Start building of the "RUN_TESTS" project in the Visual studio
# or run "make test" on linux stations to start the testing procedure.
#

ENABLE_TESTING()

MACRO(ADD_UNIT_TESTS)
  if (WIN32) # different separators and path to libraries variable name
    SET(_JUSTPATH "${CMAKE_INSTALL_PREFIX}/bin;${CMAKE_INSTALL_PREFIX}/swig;${CMAKE_INSTALL_PREFIX}/plugins;${SUIT_LIB_DIR};$ENV{PATH}")
    STRING(REPLACE "\\" "/" _JUSTPATH "${_JUSTPATH}")
    STRING(REPLACE ";" "\\;" _JUSTPATH "${_JUSTPATH}")
    SET(_PYTHONPATH "${CMAKE_INSTALL_PREFIX}/swig;${CMAKE_INSTALL_PREFIX}/plugins;${CMAKE_INSTALL_PREFIX}/addons;${CMAKE_INSTALL_PREFIX}/pythonAPI;$ENV{PYTHONPATH}")
    STRING(REPLACE "\\" "/" _PYTHONPATH "${_PYTHONPATH}")
    STRING(REPLACE ";" "\\;" _PYTHONPATH "${_PYTHONPATH}")
  else()
    SET(_LD_LIBRARY_PATH "${CMAKE_INSTALL_PREFIX}/bin:${CMAKE_INSTALL_PREFIX}/swig:${CMAKE_INSTALL_PREFIX}/plugins:${SUIT_LIB_DIR}:$ENV{LD_LIBRARY_PATH}")
    SET(_PYTHONPATH "${CMAKE_INSTALL_PREFIX}/swig:${CMAKE_INSTALL_PREFIX}/plugins:${CMAKE_INSTALL_PREFIX}/addons:${CMAKE_INSTALL_PREFIX}/pythonAPI:$ENV{PYTHONPATH}")
  endif()

  SET(_CONFIG_FILE "${CMAKE_INSTALL_PREFIX}/plugins")
  foreach(eachFileName ${ARGN})
    # Strip the ".py" suffix 
    GET_FILENAME_COMPONENT(aTestName ${eachFileName} NAME_WE)

    # Add "SubprojectName_" prefix
    GET_FILENAME_COMPONENT(aSubprojectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    SET(aTestName "${aSubprojectName}_${aTestName}")

    # Full path to the python test file beeing executed 
    SET(aTestFileName "${CMAKE_CURRENT_SOURCE_DIR}/Test/${eachFileName}")
    IF(EXISTS ${aTestFileName})
      ADD_TEST(NAME ${aTestName}
               COMMAND ${PYTHON_EXECUTABLE} ${aTestFileName})
      if (WIN32) # different path to libraries variable name
        SET_TESTS_PROPERTIES(${aTestName} PROPERTIES
               ENVIRONMENT "PATH=${_JUSTPATH};PYTHONPATH=${_PYTHONPATH};NEW_GEOM_CONFIG_FILE=${_CONFIG_FILE}")
      else()
        SET_TESTS_PROPERTIES(${aTestName} PROPERTIES
               ENVIRONMENT "LD_LIBRARY_PATH=${_LD_LIBRARY_PATH};PYTHONPATH=${_PYTHONPATH};NEW_GEOM_CONFIG_FILE=${_CONFIG_FILE}")
      endif()
      # Debug output...
      #MESSAGE(STATUS "Test added: ${aTestName} file: ${aTestFileName}")
    ELSE(EXISTS ${aTestFileName})
      MESSAGE(WARNING "Can not find the test file: ${aTestFileName}")
    ENDIF(EXISTS ${aTestFileName})
  endforeach(eachFileName ${ARGN})
ENDMACRO(ADD_UNIT_TESTS)

