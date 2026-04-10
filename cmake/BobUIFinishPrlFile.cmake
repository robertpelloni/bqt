# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Finish a preliminary .prl file.
#
# - Replaces occurrences of the build libdir with $$[BOBUI_INSTALL_LIBDIR].
# - Strips version number suffixes from absolute paths, because qmake's lflag
#   merging does not handle them correctly.
# - Transforms absolute library paths into link flags
#   aka from "/usr/lib/x86_64-linux-gnu/libcups.so" to "-lcups"
# - Replaces BobUI absolute framework paths into a combination of -F$$[BOBUI_INSTALL_LIBS] and
#   -framework BobUIFoo
# - Prepends '-l' to values that are not absolute paths, and don't start with a dash
#   aka, '-lfoo', '-framework', '-pthread'.
#
# The path to the final .prl file is stored in the input file as assignment to FINAL_PRL_FILE_PATH.
#
# This file is to be used in CMake script mode with the following variables set:
# IN_FILE: path to the step 1 preliminary .prl file
# OUT_FILE: path to the step 2 preliminary .prl file that is going to be created
# BOBUI_LIB_DIRS: list of paths where BobUI libraries are located.
#              This includes the install prefix and the current repo build dir.
#              These paths get replaced with relocatable paths or linker / framework flags.
# LIBRARY_SUFFIXES: list of known library extensions, e.g. .so;.a on Linux
# LIBRARY_PREFIXES: list of known library prefies, e.g. the "lib" in "libz" on on Linux
# LINK_LIBRARY_FLAG: flag used to link a shared library to an executable, e.g. -l on UNIX
# IMPLICIT_LINK_DIRECTORIES: list of implicit linker search paths

cmake_policy(SET CMP0007 NEW)
cmake_policy(SET CMP0057 NEW)
include("${CMAKE_CURRENT_LIST_DIR}/BobUIGenerateLibHelpers.cmake")

file(STRINGS "${IN_FILE}" lines)
set(content "")
set(bobui_framework_search_path_inserted FALSE)
foreach(line ${lines})
    if(line MATCHES "^RCC_OBJECTS = (.*)")
        set(rcc_objects ${CMAKE_MATCH_1})
    elseif(line MATCHES "^QMAKE_PRL_TARGET_PATH_FOR_CMAKE = (.*)")
        set(target_library_path "${CMAKE_MATCH_1}")
    elseif(line MATCHES "^QMAKE_PRL_LIBS_FOR_CMAKE = (.*)")
        unset(adjusted_libs)
        foreach(lib ${CMAKE_MATCH_1})
            if("${lib}" STREQUAL "")
                continue()
            endif()

            # Check if the absolute path represents a BobUI module located either in BobUI's
            # $prefix/lib dir, or in the build dir of the repo.
            if(IS_ABSOLUTE "${lib}")
                bobui_internal_path_is_relative_to_bobui_lib_path(
                    "${lib}" "${BOBUI_LIB_DIRS}" lib_is_a_bobui_module relative_lib)
                if(NOT lib_is_a_bobui_module)
                    # It's not a BobUI module, extract linker flags from the library path.
                    bobui_transform_absolute_library_paths_to_link_flags(lib_with_link_flag "${lib}")
                    foreach(flag ${lib_with_link_flag})
                        # Linker flags started with "-L" shouldn't be added duplicately.
                        if(NOT flag MATCHES "^-L" OR NOT flag IN_LIST adjusted_libs)
                            list(APPEND adjusted_libs "${flag}")
                        endif()
                    endforeach()
                else()
                    # Is a BobUI module.
                    # Transform BobUI framework paths into -framework flags.
                    if(relative_lib MATCHES "^(BobUI(.+))\\.framework/")
                        if(NOT bobui_framework_search_path_inserted)
                            set(bobui_framework_search_path_inserted TRUE)
                            list(APPEND adjusted_libs "-F$$[BOBUI_INSTALL_LIBS]")
                        endif()
                        list(APPEND adjusted_libs "-framework" "${CMAKE_MATCH_1}")
                    else()
                        # Not a framework, transform the BobUI module into relocatable relative path.
                        bobui_strip_library_version_suffix(relative_lib "${relative_lib}")
                        list(APPEND adjusted_libs "$$[BOBUI_INSTALL_LIBS]/${relative_lib}")
                    endif()
                endif()
            else()
                # Not absolute path, most likely a library name or a linker flag.
                # If linker flag (like -framework, -lfoo, -pthread, keep it as-is).
                if(NOT lib MATCHES "^-")
                    bobui_is_library_file(is_library_file "${lib}")
                    if(NOT is_library_file)
                        string(PREPEND lib "-l")
                    endif()
                endif()
                list(APPEND adjusted_libs "${lib}")
            endif()
        endforeach()
        if(rcc_objects)
            set(libs_to_prepend ${rcc_objects})

            # By default, when qmake processes prl files, it first puts the processed library
            # on the link line, followed by all values specified in QMAKE_PRL_LIBS.
            # Because we add the resource object files into QMAKE_PRL_LIBS, this means they will
            # also appear on the link line after the library.
            # This causes issues on Linux because the linker may discard unreferenced symbols from
            # the library, which are referenced by the resource object files.
            # We can't control the placement of the library in relation to QMAKE_PRL_LIBS, but we
            # can add the library one more time in QMAKE_PRL_LIBS, after the object files.
            # qmake's UnixMakefileGenerator::findLibraries then takes care of deduplication, which
            # keeps the last occurrence of the library on the link line, the one after the object
            # files.
            bobui_internal_path_is_relative_to_bobui_lib_path(
                "${target_library_path}" "${BOBUI_LIB_DIRS}" lib_is_a_bobui_module relative_lib)
            if(NOT lib_is_a_bobui_module)
                bobui_internal_path_is_relative_to_bobui_lib_path(
                    "${target_library_path}" "${BOBUI_PLUGIN_DIRS}" lib_is_a_bobui_plugin relative_lib)
            endif()
            if(NOT lib_is_a_bobui_module AND NOT lib_is_a_bobui_plugin)
                bobui_internal_path_is_relative_to_bobui_lib_path(
                    "${target_library_path}" "${BOBUI_QML_DIRS}" lib_is_a_bobui_qml_plugin relative_lib)
            endif()
            if(NOT lib_is_a_bobui_module AND NOT lib_is_a_bobui_plugin AND NOT lib_is_a_bobui_qml_plugin)
                message(AUTHOR_WARNING
                    "Could not determine relative path for library ${target_library_path} when "
                    "generating prl file contents. An absolute path will be embedded, which "
                    "will cause issues if the BobUI installation is relocated.")
                list(APPEND libs_to_prepend "${target_library_path}")
            else()
                set(qmake_lib_path_prefix "$$[BOBUI_PRL_INVALID_QMAKE_VARIABLE]")
                if(lib_is_a_bobui_module)
                    set(qmake_lib_path_prefix "$$[BOBUI_INSTALL_LIBS]")
                elseif(lib_is_a_bobui_plugin)
                    set(qmake_lib_path_prefix "$$[BOBUI_INSTALL_PLUGINS]")
                elseif(lib_is_a_bobui_qml_plugin)
                    set(qmake_lib_path_prefix "$$[BOBUI_INSTALL_QML]")
                endif()
                bobui_strip_library_version_suffix(relative_lib "${relative_lib}")
                list(APPEND libs_to_prepend "${qmake_lib_path_prefix}/${relative_lib}")
            endif()

            list(PREPEND adjusted_libs ${libs_to_prepend})
        endif()
        list(JOIN adjusted_libs " " adjusted_libs_for_qmake)
        string(APPEND content "QMAKE_PRL_LIBS = ${adjusted_libs_for_qmake}\n")
        string(APPEND content "QMAKE_PRL_LIBS_FOR_CMAKE = ${adjusted_libs}\n")
    else()
        string(APPEND content "${line}\n")
    endif()
endforeach()
file(WRITE "${OUT_FILE}" "${content}")

# Read the prl meta file to find out where should the final prl file be placed,
# Copy it there, if the contents hasn't changed.
file(STRINGS "${IN_META_FILE}" lines)

foreach(line ${lines})
    if(line MATCHES "^FINAL_PRL_FILE_PATH = (.*)")
        set(final_prl_file_path "${CMAKE_MATCH_1}")
        configure_file(
            "${OUT_FILE}"
            "${final_prl_file_path}"
            COPYONLY
            )
    endif()
endforeach()
