# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Enable compiler warnings by default. All compilers except MSVC support -Wall -Wextra
#
# You can disable the warnings for specific targets (for instance containing 3rd party code)
# by calling bobui_disable_warnings(target). This will set the BOBUI_COMPILE_OPTIONS_DISABLE_WARNINGS
# property checked below, and is equivalent to qmake's CONFIG += warn_off.

set(_bobui_compiler_warning_flags_on "")
set(_bobui_compiler_warning_flags_off -w)

if (MSVC)
    list(APPEND _bobui_compiler_warning_flags_on /W3)
    # MSVC warns about macros expanding to `defined` when using the
    # new preprocessor (so far, default for C11 code, but not C++).
    # Suppress the warning, see also the comment below for GCC.
    list(APPEND _bobui_compiler_warning_flags_on /wd5105)
else()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GHS") # There is no -Wextra flag for GHS compiler.
        list(APPEND _bobui_compiler_warning_flags_on -Wall)
    else()
        list(APPEND _bobui_compiler_warning_flags_on -Wall -Wextra)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "17.0.0")
            # GCC warns if a macro is expanded to `defined`, but doesn't
            # differentiate between object-like and function-like macros.
            # The latter generally work everywhere. We don't have fine-grained
            # control, so disable the warning (tst_qglobal tests for this
            # behavior.)
            # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118542
            list(APPEND _bobui_compiler_warning_flags_on -Wno-expansion-to-defined)
        endif()
    endif()
endif()

set(_bobui_compiler_warning_flags_condition
    "$<BOOL:$<TARGET_PROPERTY:BOBUI_COMPILE_OPTIONS_DISABLE_WARNINGS>>")
set(_bobui_compiler_warning_flags_genex
    "$<IF:${_bobui_compiler_warning_flags_condition},${_bobui_compiler_warning_flags_off},${_bobui_compiler_warning_flags_on}>")

set(_bobui_compiler_warning_flags_language_condition
    "$<COMPILE_LANGUAGE:CXX,C,OBJC,OBJCXX>")
set(_bobui_compiler_warning_flags_language_conditional_genex
    "$<${_bobui_compiler_warning_flags_language_condition}:${_bobui_compiler_warning_flags_genex}>")


# Need to replace semicolons so that the list is not wrongly expanded in the add_compile_options
# call.
string(REPLACE ";" "$<SEMICOLON>"
       _bobui_compiler_warning_flags_language_conditional_genex
       "${_bobui_compiler_warning_flags_language_conditional_genex}")
add_compile_options(${_bobui_compiler_warning_flags_language_conditional_genex})
