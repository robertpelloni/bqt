# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QWasmIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWasmIntegrationPlugin
    OUTPUT_NAME qwasm
    DEFAULT_IF "wasm" IN_LIST BOBUI_QPA_PLATFORMS
    PLUGIN_TYPE platforms
    SOURCES
        main.cpp
        qwasmaccessibility.cpp qwasmaccessibility.h
        qwasmbase64iconstore.cpp qwasmbase64iconstore.h
        qwasmcompositor.cpp qwasmcompositor.h
        qwasmcssstyle.cpp qwasmcssstyle.h
        qwasmcursor.cpp qwasmcursor.h
        qwasmdom.cpp qwasmdom.h
        qwasmevent.cpp qwasmevent.h
        qwasmeventdispatcher.cpp qwasmeventdispatcher.h
        qwasmfontdatabase.cpp qwasmfontdatabase.h
        qwasmintegration.cpp qwasmintegration.h
        qwasmkeytranslator.cpp qwasmkeytranslator.h
        qwasmoffscreensurface.cpp qwasmoffscreensurface.h
        qwasmopenglcontext.cpp qwasmopenglcontext.h
        qwasmplatform.cpp qwasmplatform.h
        qwasmscreen.cpp qwasmscreen.h
        qwasmservices.cpp qwasmservices.h
        qwasmtheme.cpp qwasmtheme.h
        qwasmwindow.cpp qwasmwindow.h
        qwasmwindowtreenode.cpp qwasmwindowtreenode.h
        qwasmwindownonclientarea.cpp qwasmwindownonclientarea.h
        qwasminputcontext.cpp qwasminputcontext.h
        qwasmwindowstack.h
        qwasmfiledialoghelper.cpp qwasmfiledialoghelper.h
    DEFINES
        BOBUI_EGL_NO_X11
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    ATTRIBUTION_FILE_DIR_PATHS
        ../../../3rdparty/wasm
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_GPL3
)

# Resources:
set(wasmfonts_resource_files
    "${BobUIBase_SOURCE_DIR}/src/3rdparty/wasm/DejaVuSans.ttf"
    "${BobUIBase_SOURCE_DIR}/src/3rdparty/wasm/DejaVuSansMono.ttf"
)

bobui_internal_add_resource(QWasmIntegrationPlugin "wasmfonts"
    PREFIX
        "/fonts"
    BASE
        "${BobUIBase_SOURCE_DIR}/src/3rdparty/wasm"
    FILES
        ${wasmfonts_resource_files}
)

bobui_internal_extend_target(QWasmIntegrationPlugin CONDITION BOBUI_FEATURE_clipboard
    SOURCES
        qwasmclipboard.cpp qwasmclipboard.h
)

bobui_internal_extend_target(QWasmIntegrationPlugin CONDITION BOBUI_FEATURE_draganddrop
    SOURCES
        qwasmdrag.cpp qwasmdrag.h
)

bobui_internal_extend_target(QWasmIntegrationPlugin CONDITION BOBUI_FEATURE_opengl
    SOURCES
        qwasmbackingstore.cpp qwasmbackingstore.h
    LIBRARIES
        BobUI::OpenGL
        BobUI::OpenGLPrivate
)

# PLUGIN_EXTENDS = "-"

set(wasm_support_files
    wasm_shell.html
    bobuiloader.js
    resources/bobuilogo.svg
)

set(wasmwindow_resource_files
    "resources/maximize.svg"
    "resources/bobuilogo.svg"
    "resources/restore.svg"
    "resources/x.svg"
)

bobui_internal_add_resource(QWasmIntegrationPlugin "wasmwindow"
    PREFIX
        "/wasm-window"
    BASE
        "resources"
    FILES
        ${wasmwindow_resource_files}
)

bobui_path_join(destination ${BOBUI_INSTALL_DIR} "plugins/platforms")
bobui_copy_or_install(FILES
    ${wasm_support_files}
    DESTINATION "${destination}"
)
# Need to copy the support files to the build dir in a top-level prefix build
# So _bobui_internal_wasm_add_target_helpers finds them.
if(BOBUI_WILL_INSTALL)
    foreach(wasm_support_file ${wasm_support_files})
        file(COPY "${wasm_support_file}" DESTINATION "${BOBUI_BUILD_DIR}/plugins/platforms")
    endforeach()
endif()
