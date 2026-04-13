# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QWindowsIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QWindowsIntegrationPlugin
    OUTPUT_NAME qwindows
    PLUGIN_TYPE platforms
    DEFAULT_IF "windows" IN_LIST BOBUI_QPA_PLATFORMS
    EXCEPTIONS
    SOURCES
        main.cpp
        bobuiwindowsglobal.h
        qwin10helpers.cpp qwin10helpers.h
        qwindowsapplication.cpp qwindowsapplication.h
        qwindowsbackingstore.cpp qwindowsbackingstore.h
        qwindowscontext.cpp qwindowscontext.h
        qwindowscursor.cpp qwindowscursor.h
        qwindowsdialoghelpers.cpp qwindowsdialoghelpers.h
        qwindowsdropdataobject.cpp qwindowsdropdataobject.h
        qwindowsgdiintegration.cpp qwindowsgdiintegration.h
        qwindowsgdinativeinterface.cpp qwindowsgdinativeinterface.h
        qwindowsiconengine.cpp qwindowsiconengine.h
        qwindowsinputcontext.cpp qwindowsinputcontext.h
        qwindowsintegration.cpp qwindowsintegration.h
        qwindowsinternalmimedata.cpp qwindowsinternalmimedata.h
        qwindowskeymapper.cpp qwindowskeymapper.h
        qwindowsmenu.cpp qwindowsmenu.h
        qwindowsmimeregistry.cpp qwindowsmimeregistry.h
        qwindowsnativeinterface.cpp qwindowsnativeinterface.h
        qwindowsole.cpp qwindowsole.h
        qwindowsopengltester.cpp qwindowsopengltester.h
        qwindowspointerhandler.cpp qwindowspointerhandler.h
        qwindowsscreen.cpp qwindowsscreen.h
        qwindowsservices.cpp qwindowsservices.h
        qwindowstheme.cpp qwindowstheme.h
        qwindowsthreadpoolrunner.h
        qwindowswindow.cpp qwindowswindow.h
        qwindowswindowclassdescription.cpp qwindowswindowclassdescription.h
        qwindowswindowclassregistry.cpp qwindowswindowclassregistry.h
    NO_UNITY_BUILD_SOURCES
        qwindowspointerhandler.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_FOREACH
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        advapi32
        dwmapi
        gdi32
        uxtheme
        imm32
        ole32
        oleaut32
        setupapi
        shell32
        shlwapi
        user32
        winmm
        winspool
        wtsapi32
        shcore
        comdlg32
        d3d9
        runtimeobject
)

# Resources:
set_source_files_properties("openglblacklists/default.json"
    PROPERTIES BOBUI_RESOURCE_ALIAS "default.json"
)
set(openglblacklists_resource_files
    "openglblacklists/default.json"
)

bobui_internal_add_resource(QWindowsIntegrationPlugin "openglblacklists"
    PREFIX
        "/bobui-project.org/windows/openglblacklists"
    FILES
        ${openglblacklists_resource_files}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_opengl
    SOURCES
        qwindowsglcontext.cpp qwindowsglcontext.h
        qwindowsopenglcontext.h
    LIBRARIES
        BobUI::OpenGLPrivate
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_opengl AND NOT BOBUI_FEATURE_dynamicgl
    LIBRARIES
        opengl32
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION MINGW
    LIBRARIES
        uuid
    NO_PCH_SOURCES
        qwindowspointerhandler.cpp
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_systemtrayicon
    SOURCES
        qwindowssystemtrayicon.cpp qwindowssystemtrayicon.h
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_vulkan
    SOURCES
        qwindowsvulkaninstance.cpp qwindowsvulkaninstance.h
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_clipboard
    SOURCES
        qwindowsclipboard.cpp qwindowsclipboard.h
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_clipboard AND BOBUI_FEATURE_draganddrop
    SOURCES
        qwindowsdrag.cpp qwindowsdrag.h
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_tabletevent
    SOURCES
        qwindowstabletsupport.cpp qwindowstabletsupport.h
    INCLUDE_DIRECTORIES
        ${BobUIBase_SOURCE_DIR}/src/3rdparty/wintab
    ATTRIBUTION_FILE_DIR_PATHS
        ../../../3rdparty/wintab
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_sessionmanager
    SOURCES
        qwindowssessionmanager.cpp qwindowssessionmanager.h
)

if(BOBUI_FEATURE_imageformat_png)
    # Resources:
    set(cursors_resource_files
        "images/closedhandcursor_32.png"
        "images/closedhandcursor_48.png"
        "images/closedhandcursor_64.png"
        "images/dragcopycursor_32.png"
        "images/dragcopycursor_48.png"
        "images/dragcopycursor_64.png"
        "images/draglinkcursor_32.png"
        "images/draglinkcursor_48.png"
        "images/draglinkcursor_64.png"
        "images/dragmovecursor_32.png"
        "images/dragmovecursor_48.png"
        "images/dragmovecursor_64.png"
        "images/openhandcursor_32.png"
        "images/openhandcursor_48.png"
        "images/openhandcursor_64.png"
        "images/splithcursor_32.png"
        "images/splithcursor_48.png"
        "images/splithcursor_64.png"
        "images/splitvcursor_32.png"
        "images/splitvcursor_48.png"
        "images/splitvcursor_64.png"
    )

    bobui_internal_add_resource(QWindowsIntegrationPlugin "cursors"
        PREFIX
            "/bobui-project.org/windows/cursors"
        FILES
            ${cursors_resource_files}
    )
endif()

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION BOBUI_FEATURE_accessibility
    SOURCES
        uiautomation/qwindowsuiautomation.cpp uiautomation/qwindowsuiautomation.h
        uiautomation/qwindowsuiaaccessibility.cpp uiautomation/qwindowsuiaaccessibility.h
        uiautomation/qwindowsuiabaseprovider.cpp uiautomation/qwindowsuiabaseprovider.h
        uiautomation/qwindowsuiaexpandcollapseprovider.cpp uiautomation/qwindowsuiaexpandcollapseprovider.h
        uiautomation/qwindowsuiagriditemprovider.cpp uiautomation/qwindowsuiagriditemprovider.h
        uiautomation/qwindowsuiagridprovider.cpp uiautomation/qwindowsuiagridprovider.h
        uiautomation/qwindowsuiainvokeprovider.cpp uiautomation/qwindowsuiainvokeprovider.h
        uiautomation/qwindowsuiamainprovider.cpp uiautomation/qwindowsuiamainprovider.h
        uiautomation/qwindowsuiaprovidercache.cpp uiautomation/qwindowsuiaprovidercache.h
        uiautomation/qwindowsuiarangevalueprovider.cpp uiautomation/qwindowsuiarangevalueprovider.h
        uiautomation/qwindowsuiaselectionitemprovider.cpp uiautomation/qwindowsuiaselectionitemprovider.h
        uiautomation/qwindowsuiaselectionprovider.cpp uiautomation/qwindowsuiaselectionprovider.h
        uiautomation/qwindowsuiatableitemprovider.cpp uiautomation/qwindowsuiatableitemprovider.h
        uiautomation/qwindowsuiatableprovider.cpp uiautomation/qwindowsuiatableprovider.h
        uiautomation/qwindowsuiatextprovider.cpp uiautomation/qwindowsuiatextprovider.h
        uiautomation/qwindowsuiatextrangeprovider.cpp uiautomation/qwindowsuiatextrangeprovider.h
        uiautomation/qwindowsuiatoggleprovider.cpp uiautomation/qwindowsuiatoggleprovider.h
        uiautomation/qwindowsuiautils.cpp uiautomation/qwindowsuiautils.h
        uiautomation/qwindowsuiavalueprovider.cpp uiautomation/qwindowsuiavalueprovider.h
        uiautomation/qwindowsuiawindowprovider.cpp uiautomation/qwindowsuiawindowprovider.h
)

# This is loaded dynamically when using MINGW as its import library is incomplete
bobui_internal_extend_target(
    QWindowsIntegrationPlugin
        CONDITION (MSVC OR CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC") AND BOBUI_FEATURE_accessibility
    LIBRARIES
        uiautomationcore
)

bobui_internal_extend_target(QWindowsIntegrationPlugin CONDITION MINGW AND BOBUI_FEATURE_accessibility
    LIBRARIES
        uuid
)
