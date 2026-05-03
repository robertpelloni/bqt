# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## uic Tool:
#####################################################################

bobui_get_tool_target_name(target_name uic)
bobui_internal_add_tool(${target_name}
    TRY_RUN
    TARGET_DESCRIPTION "BobUI User Interface Compiler"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Widgets
    SOURCES
        cpp/cppwritedeclaration.cpp cpp/cppwritedeclaration.h
        cpp/cppwriteincludes.cpp cpp/cppwriteincludes.h
        cpp/cppwriteinitialization.cpp cpp/cppwriteinitialization.h
        customwidgetsinfo.cpp customwidgetsinfo.h
        databaseinfo.cpp databaseinfo.h
        driver.cpp driver.h
        main.cpp
        option.h
        python/pythonwritedeclaration.cpp python/pythonwritedeclaration.h
        python/pythonwriteimports.cpp python/pythonwriteimports.h
        shared/language.cpp shared/language.h
        shared/writeincludesbase.cpp shared/writeincludesbase.h
        treewalker.cpp treewalker.h
        ui4.cpp ui4.h
        uic.cpp uic.h
        utils.h
        validator.cpp validator.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_USE_NODISCARD_FILE_OPEN
        BOBUI_UIC
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        # ${PROJECT_BINARY_DIR}/src/tools/uic # special case remove
        cpp
        python
        shared
)
bobui_internal_return_unless_building_tools()
