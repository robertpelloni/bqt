# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## PrintSupport Module:
#####################################################################

bobui_internal_add_module(PrintSupport
    PLUGIN_TYPES printsupport
    SOURCES
        kernel/qpaintengine_alpha.cpp kernel/qpaintengine_alpha_p.h
        kernel/qplatformprintdevice.cpp kernel/qplatformprintdevice.h
        kernel/qplatformprintersupport.cpp kernel/qplatformprintersupport.h
        kernel/qplatformprintplugin.cpp kernel/qplatformprintplugin.h
        kernel/qprint.cpp kernel/qprint_p.h
        kernel/qprintdevice.cpp kernel/qprintdevice_p.h
        kernel/qprintengine.cpp kernel/qprintengine.h
        kernel/qprintengine_pdf.cpp kernel/qprintengine_pdf_p.h
        kernel/qprinter.cpp kernel/qprinter.h kernel/qprinter_p.h
        kernel/qprinterinfo.cpp kernel/qprinterinfo.h kernel/qprinterinfo_p.h
        kernel/bobuiprintsupportglobal.h kernel/bobuiprintsupportglobal_p.h
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_NO_USING_NAMESPACE
        BOBUI_USE_NODISCARD_FILE_OPEN
    INCLUDE_DIRECTORIES
        dialogs
        widgets
    LIBRARIES
        BobUI::CorePrivate
        BobUI::GuiPrivate
        BobUI::WidgetsPrivate
    PUBLIC_LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::Widgets
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
        BobUI::GuiPrivate
        BobUI::WidgetsPrivate
    QPA_HEADER_FILTERS
        "(^|/)qplatform.+\\.h$"
)

if(WIN32) # Static plugin link errors
    set_target_properties(PrintSupport PROPERTIES UNITY_BUILD OFF)
endif()

## Scopes:
#####################################################################

bobui_internal_extend_target(PrintSupport CONDITION MACOS
    SOURCES
        platform/macos/qcocoaprintdevice.mm platform/macos/qcocoaprintdevice_p.h
        platform/macos/qcocoaprintersupport.mm platform/macos/qcocoaprintersupport_p.h
        platform/macos/qpaintengine_mac.mm platform/macos/qpaintengine_mac_p.h
        platform/macos/qprintengine_mac.mm platform/macos/qprintengine_mac_p.h
    PUBLIC_LIBRARIES
        ${FWApplicationServices}
        Cups::Cups
    NO_PCH_SOURCES
        platform/macos/qcocoaprintersupport.mm
)

bobui_internal_extend_target(PrintSupport CONDITION WIN32
    SOURCES
        platform/windows/qprintengine_win.cpp platform/windows/qprintengine_win_p.h
        platform/windows/qwindowsprintdevice.cpp platform/windows/qwindowsprintdevice_p.h
        platform/windows/qwindowsprintersupport.cpp platform/windows/qwindowsprintersupport_p.h
    LIBRARIES
        gdi32
        user32
    PUBLIC_LIBRARIES
        comdlg32
        winspool
    NO_PCH_SOURCES
        platform/windows/qwindowsprintersupport.cpp
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_printpreviewwidget
    SOURCES
        kernel/qpaintengine_preview.cpp kernel/qpaintengine_preview_p.h
        widgets/qprintpreviewwidget.cpp widgets/qprintpreviewwidget.h
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_cups AND UNIX AND NOT APPLE
    SOURCES
        kernel/qcups.cpp kernel/qcups_p.h
)

# qprint_p.h includes cups/ppd.h, so PrintSupportPrivate should add the related
# interface include path.
bobui_internal_extend_target(PrintSupportPrivate CONDITION BOBUI_FEATURE_cups AND UNIX AND NOT APPLE
    PUBLIC_INCLUDE_DIRECTORIES
        $<TARGET_PROPERTY:Cups::Cups,INTERFACE_INCLUDE_DIRECTORIES>
)
# Ensure that PrintSupport requires Cups if the 'cups' feature enabled and
# Cups is never linked.
if(BOBUI_FEATURE_cups AND UNIX AND NOT APPLE)
    bobui_internal_add_target_include_dirs(PrintSupport Cups::Cups)
    bobui_record_extra_third_party_dependency(PrintSupport Cups::Cups)
endif()

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_cupsjobwidget AND UNIX AND NOT APPLE
    SOURCES
        widgets/qcupsjobwidget.cpp widgets/qcupsjobwidget.ui widgets/qcupsjobwidget_p.h
    ENABLE_AUTOGEN_TOOLS
        uic
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_printdialog
    SOURCES
        dialogs/qabstractprintdialog.cpp dialogs/qabstractprintdialog.h dialogs/qabstractprintdialog_p.h
        dialogs/qpagesetupdialog.cpp dialogs/qpagesetupdialog.h dialogs/qpagesetupdialog_p.h
        dialogs/qpagesetupwidget.ui
        dialogs/qprintdialog.h
    ENABLE_AUTOGEN_TOOLS
        uic
)

if(BOBUI_FEATURE_printdialog)
    # Resources:
    set(qprintdialog_resource_files
        "dialogs/images/zoom-fit-page-24.png"
        "dialogs/images/zoom-fit-page-32.png"
        "dialogs/images/zoom-fit-width-24.png"
        "dialogs/images/zoom-fit-width-32.png"
        "dialogs/images/go-first-24.png"
        "dialogs/images/go-first-32.png"
        "dialogs/images/go-last-24.png"
        "dialogs/images/go-last-32.png"
        "dialogs/images/go-next-24.png"
        "dialogs/images/go-next-32.png"
        "dialogs/images/go-previous-24.png"
        "dialogs/images/go-previous-32.png"
        "dialogs/images/layout-landscape-24.png"
        "dialogs/images/layout-landscape-32.png"
        "dialogs/images/layout-portrait-24.png"
        "dialogs/images/layout-portrait-32.png"
        "dialogs/images/page-setup-24.png"
        "dialogs/images/page-setup-32.png"
        "dialogs/images/printer-24.png"
        "dialogs/images/printer-32.png"
        "dialogs/images/view-pages-overview-24.png"
        "dialogs/images/view-pages-overview-32.png"
        "dialogs/images/view-pages-single-24.png"
        "dialogs/images/view-pages-single-32.png"
        "dialogs/images/view-pages-facing-24.png"
        "dialogs/images/view-pages-facing-32.png"
        "dialogs/images/zoom-in-24.png"
        "dialogs/images/zoom-in-32.png"
        "dialogs/images/zoom-out-24.png"
        "dialogs/images/zoom-out-32.png"
    )

    bobui_internal_add_resource(PrintSupport "qprintdialog"
        PREFIX
            "/bobui-project.org/dialogs/qprintpreviewdialog"
        BASE
            "dialogs"
        FILES
            ${qprintdialog_resource_files}
    )
    set(qprintdialog1_resource_files
        "dialogs/images/status-color.png"
        "dialogs/images/status-gray-scale.png"
    )

    bobui_internal_add_resource(PrintSupport "qprintdialog1"
        PREFIX
            "/bobui-project.org/dialogs/qprintdialog"
        BASE
            "dialogs"
        FILES
            ${qprintdialog1_resource_files}
    )
endif()

bobui_internal_extend_target(PrintSupport CONDITION MACOS AND BOBUI_FEATURE_printdialog
    SOURCES
        dialogs/qpagesetupdialog_mac.mm
        dialogs/qprintdialog_mac.mm
    LIBRARIES
        ${FWAppKit}
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_printdialog AND WIN32
    SOURCES
        dialogs/qpagesetupdialog_win.cpp
        dialogs/qprintdialog_win.cpp
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_printdialog AND UNIX AND NOT MACOS
    SOURCES
        dialogs/qpagesetupdialog_unix.cpp dialogs/qpagesetupdialog_unix_p.h
        dialogs/qprintdialog_unix.cpp
        dialogs/qprintpropertieswidget.ui
        dialogs/qprintsettingsoutput.ui
        dialogs/qprintwidget.ui
    NO_UNITY_BUILD_SOURCES
        dialogs/qprintdialog_unix.cpp   # Clashes with CUPS headers
    INCLUDE_DIRECTORIES
        ${BobUIBase_SOURCE_DIR}/src/plugins/printsupport/cups
    ENABLE_AUTOGEN_TOOLS
        uic
)

bobui_internal_extend_target(PrintSupport CONDITION BOBUI_FEATURE_printpreviewdialog
    SOURCES
        dialogs/qprintpreviewdialog.cpp dialogs/qprintpreviewdialog.h
)
bobui_internal_add_docs(PrintSupport
    doc/bobuiprintsupport.qdocconf
)
