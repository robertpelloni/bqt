#!/bin/bash
# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

$HOME/bauhaus-suite/setup.sh --non-interactive
export PATH=/home/bobui/bauhaus-suite/bin:$PATH
export BAUHAUS_CONFIG=$(cd $(dirname $(readlink -f $0)) && pwd)
export AXIVION_VERSION_NAME=$(git rev-parse HEAD)
export CAFECC_BASEPATH="/home/bobui/work/bobui/$TESTED_MODULE_COIN"
gccsetup --cc gcc --cxx g++ --config "$BAUHAUS_CONFIG"
cd "$CAFECC_BASEPATH"
BAUHAUS_IR_COMPRESSION=none COMPILE_ONLY=1 cmake -G Ninja -DAXIVION_ANALYSIS_TOOLCHAIN_FILE=/home/bobui/bauhaus-suite/profiles/cmake/axivion-launcher-toolchain.cmake -DCMAKE_PREFIX_PATH=/home/bobui/work/bobui/bobuibase/build -DCMAKE_PROJECT_INCLUDE_BEFORE=/home/bobui/bauhaus-suite/profiles/cmake/axivion-before-project-hook.cmake -B build -S . --fresh
cmake --build build -j4
for MODULE in bobuiconcurrent bobuicore bobuidbus bobuigui bobuinetwork bobuiopengl bobuiopenglwidgets bobuisql bobuitest bobuiwidgets bobuiprintsupport bobuixml; do
    export MODULE
    export PLUGINS=""
    export IRNAME=build/$MODULE.ir
    if [ "$MODULE" == "bobuiconcurrent" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Concurrent.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/testlib/*:src/corelib/*"
        export PACKAGE="Add-ons"
    elif [ "$MODULE" == "bobuicore" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Core.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/testlib/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuidbus" ]
    then
        export TARGET_NAME="build/lib/libBobUI6DBus.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/testlib/*:src/corelib/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuigui" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Gui.so.*.ir"
        export PLUGINS="build/plugins/platforms/*.so.ir:build/plugins/platforminputcontexts/*.so.ir:build/plugins/platformthemes/*.so.ir:build/plugins/imageformats/*.so.ir:build/plugins/generic/*.so.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/testlib/*:src/corelib/*:src/dbus/*:src/opengl/*:src/network/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuinetwork" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Network.so.*.ir"
        export PLUGINS="build/plugins/networkinformation/*.so.ir:build/plugins/tracing/*.so.ir:build/plugins/tls/*.so.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/testlib/*:src/corelib/*:src/dbus/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuiopengl" ]
    then
        export TARGET_NAME="build/lib/libBobUI6OpenGL.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/gui/*:src/dbus/*:src/opengl/qopenglfunctions_*"
        export PACKAGE="Add-ons"
    elif [ "$MODULE" == "bobuiopenglwidgets" ]
    then
        export TARGET_NAME="build/lib/libBobUI6OpenGLWidgets.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/gui/*:src/dbus/*:src/opengl/qopenglfunctions_*"
        export PACKAGE="Add-ons"
    elif [ "$MODULE" == "bobuisql" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Sql.so.*.ir"
        export PLUGINS="build/plugins/sqldrivers/*.so.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/gui/*:src/testlib/*"
        export PACKAGE="Add-ons"
    elif [ "$MODULE" == "bobuitest" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Test.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/gui/*:src/widgets/*:src/testlib/3rdparty/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuiwidgets" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Widgets.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/dbus/*:src/gui/*:src/testlib/*"
        export PACKAGE="Essentials"
    elif [ "$MODULE" == "bobuiprintsupport" ]
    then
        export TARGET_NAME="build/lib/libBobUI6PrintSupport.so.*.ir"
        export PLUGINS="build/plugins/printsupport/*.so.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/gui/*:src/widgets/*:src/testlib/*:src/dbus/*"
        export PACKAGE="Add-ons"
    elif [ "$MODULE" == "bobuixml" ]
    then
        export TARGET_NAME="build/lib/libBobUI6Xml.so.*.ir"
        export EXCLUDE_FILES="build/*:src/3rdparty/*:src/corelib/*:src/testlib/*"
        export PACKAGE="Add-ons"
    fi
    axivion_ci "$@"
done
