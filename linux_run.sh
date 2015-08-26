#!/bin/bash

a_dir=$(dirname $0)

cd ${a_dir}

source ${a_dir}/env.sh
source ${a_dir}/env_standalone.sh

if [ -f GDB ]; then
  ${INSTALL_DIR}/bin/GeomApp &
  APP_PID=$!
  echo "Connect GDB to PID ${APP_PID}"
  gdb --command=commands.gdb - ${APP_PID}
else
  ${INSTALL_DIR}/bin/GeomApp
fi
