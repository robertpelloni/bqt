# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT APPLE AND BUILD_SHARED_LIBS)
    set(mark_cups_optional MARK_OPTIONAL)
else()
    set(mark_cups_optional "")
endif()
bobui_find_package(Cups PROVIDED_TARGETS Cups::Cups ${mark_cups_optional})

#####################################################################
## QCupsPrinterSupportPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QCupsPrinterSupportPlugin
    OUTPUT_NAME cupsprintersupport
    PLUGIN_TYPE printsupport
    SOURCES
        main.cpp
        qcupsprintengine.cpp qcupsprintengine_p.h
        qcupsprintersupport.cpp qcupsprintersupport_p.h
        qppdprintdevice.cpp qppdprintdevice.h
    INCLUDE_DIRECTORIES
        ../../../printsupport/kernel
    LIBRARIES
        Cups::Cups
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::PrintSupport
        BobUI::PrintSupportPrivate
)
