#!/bin/sh

source salome_env.sh

mkdir -p ${ROOT_DIR}/build
cd ${ROOT_DIR}/build

CMAKE_ARGS="-D_ECLIPSE_VERSION=4.3"
CMAKE_ARGS="${CMAKE_ARGS} -DCMAKE_BUILD_TYPE=Debug"
CMAKE_ARGS="${CMAKE_ARGS} -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=ON"
CMAKE_ARGS="${CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX:PATH=${ROOT_DIR}/install"
CMAKE_ARGS="${CMAKE_ARGS} ${SRC_DIR}"


cmake -G "Eclipse CDT4 - Unix Makefiles" ${CMAKE_ARGS}

/misc/dn48/newgeom/common/eclipse-4.3.0/eclipse -Dosgi.locking=none &
