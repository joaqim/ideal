#!/usr/bin/env bash
[ -d build ] || mkdir build
set -e
cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug && ninja
[ "$1" == "run" ] && ./bin/ideal --magnum-dpi-scaling 1.0 && exit 0
exit $?
