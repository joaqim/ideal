#!/usr/bin/env bash
BUILD_TYPE=${1:-Debug}
if [ "${BUILD_TYPE}" == "run" ]; then
  BUILD_TYPE="Debug"
elif [ "${BUILD_TYPE}" == "Run" ]; then
  BUILD_TYPE="Release"
fi
[ -d build ] || mkdir build
set -e
cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE && ninja
[ "${1^^}" == "RUN" ] && cd $BUILD_TYPE && ./bin/ideal --magnum-dpi-scaling 1.0 && exit 0
exit $?
