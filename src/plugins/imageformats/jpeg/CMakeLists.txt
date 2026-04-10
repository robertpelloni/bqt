# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(WrapJpeg MODULE PROVIDED_TARGETS WrapJpeg::WrapJpeg)

#####################################################################
## QJpegPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QJpegPlugin
    OUTPUT_NAME qjpeg
    PLUGIN_TYPE imageformats
    SOURCES
        main.cpp
        qjpeghandler.cpp qjpeghandler_p.h
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        WrapJpeg::WrapJpeg
)

# Fails to build on Windows with a static BobUI, PCH enabled and
# the vendored libjpeg sources, due to 'boolean'
# redefinition in jmorecfg.h and rpcndr.h.
if(WIN32 AND NOT BUILD_SHARED_LIBS)
    bobui_update_ignore_pch_source(QJpegPlugin "qjpeghandler.cpp")
endif()
