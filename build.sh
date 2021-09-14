#!/usr/bin/env bash
BUILD_TYPE=${1:-Debug}
[ -d build ] || mkdir build
set -e
cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE && ninja
[ "$1" == "run" ] && cd $BUILD_TYPE && ./bin/ideal --magnum-dpi-scaling 1.0 && exit 0
exit $?
