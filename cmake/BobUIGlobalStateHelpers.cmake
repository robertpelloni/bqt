# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

function(bobui_internal_clear_bobui_repo_known_modules)
    set(BOBUI_REPO_KNOWN_MODULES "" CACHE INTERNAL "Known current repo BobUI modules" FORCE)
endfunction()

function(bobui_internal_add_bobui_repo_known_module)
    set(BOBUI_REPO_KNOWN_MODULES ${BOBUI_REPO_KNOWN_MODULES} ${ARGN}
        CACHE INTERNAL "Known current repo BobUI modules" FORCE)
endfunction()

function(bobui_internal_get_bobui_repo_known_modules out_var)
    set("${out_var}" "${BOBUI_REPO_KNOWN_MODULES}" PARENT_SCOPE)
endfunction()

# Gets the list of all known BobUI modules both found and that were built as part of the
# current project.
function(bobui_internal_get_bobui_all_known_modules out_var)
    bobui_internal_get_bobui_repo_known_modules(repo_known_modules)
    set(known_modules ${BOBUI_ALL_MODULES_FOUND_VIA_FIND_PACKAGE} ${repo_known_modules})
    list(REMOVE_DUPLICATES known_modules)
    set("${out_var}" "${known_modules}" PARENT_SCOPE)
endfunction()

macro(bobui_internal_set_bobui_known_plugins)
    set(BOBUI_KNOWN_PLUGINS ${ARGN} CACHE INTERNAL "Known BobUI plugins" FORCE)
endmacro()

### Global plug-in types handling ###
# BOBUI_REPO_KNOWN_PLUGIN_TYPES and BOBUI_ALL_PLUGIN_TYPES_FOUND_VIA_FIND_PACKAGE
# hold a list of plug-in types (e.G. "imageformats;bearer")
function(bobui_internal_clear_bobui_repo_known_plugin_types)
    set(BOBUI_REPO_KNOWN_PLUGIN_TYPES "" CACHE INTERNAL "Known current repo BobUI plug-in types" FORCE)
endfunction()

function(bobui_internal_add_plugin_types target plugin_types)
    # Update the variable containing the list of plugins for the given plugin type
    foreach(plugin_type ${plugin_types})
        bobui_get_sanitized_plugin_type("${plugin_type}" plugin_type)
        set_property(TARGET "${target}" APPEND PROPERTY MODULE_PLUGIN_TYPES "${plugin_type}")
        bobui_internal_add_bobui_repo_known_plugin_types("${plugin_type}")
    endforeach()

    # Save the non-sanitized plugin type values for qmake consumption via .pri files.
    set_property(TARGET "${target}"
                 APPEND PROPERTY QMAKE_MODULE_PLUGIN_TYPES "${plugin_types}")

    # Export the plugin types.
    get_property(export_properties TARGET ${target} PROPERTY EXPORT_PROPERTIES)
    if(NOT MODULE_PLUGIN_TYPES IN_LIST export_properties)
        set_property(TARGET ${target} APPEND PROPERTY
            EXPORT_PROPERTIES MODULE_PLUGIN_TYPES)
    endif()
endfunction()

function(bobui_internal_add_bobui_repo_known_plugin_types)
    set(BOBUI_REPO_KNOWN_PLUGIN_TYPES ${BOBUI_REPO_KNOWN_PLUGIN_TYPES} ${ARGN}
        CACHE INTERNAL "Known current repo BobUI plug-in types" FORCE)
endfunction()

function(bobui_internal_get_bobui_repo_known_plugin_types out_var)
    set("${out_var}" "${BOBUI_REPO_KNOWN_PLUGIN_TYPES}" PARENT_SCOPE)
endfunction()

function(bobui_internal_get_bobui_all_known_plugin_types out_var)
    bobui_internal_get_bobui_repo_known_plugin_types(repo_known_plugin_types)
    set(known ${BOBUI_ALL_PLUGIN_TYPES_FOUND_VIA_FIND_PACKAGE} ${repo_known_plugin_types})
    list(REMOVE_DUPLICATES known)
    set("${out_var}" "${known}" PARENT_SCOPE)
endfunction()

macro(bobui_internal_append_known_modules_with_tools module)
    if(NOT ${module} IN_LIST BOBUI_KNOWN_MODULES_WITH_TOOLS)
        set(BOBUI_KNOWN_MODULES_WITH_TOOLS "${BOBUI_KNOWN_MODULES_WITH_TOOLS};${module}"
            CACHE INTERNAL "Known BobUI modules with tools" FORCE)
        set(BOBUI_KNOWN_MODULE_${module}_TOOLS ""
            CACHE INTERNAL "Known BobUI module ${module} tools" FORCE)
    endif()
endmacro()

macro(bobui_internal_append_known_module_tool module tool)
    if(NOT ${tool} IN_LIST BOBUI_KNOWN_MODULE_${module}_TOOLS)
        list(APPEND BOBUI_KNOWN_MODULE_${module}_TOOLS "${tool}")
        set(BOBUI_KNOWN_MODULE_${module}_TOOLS "${BOBUI_KNOWN_MODULE_${module}_TOOLS}"
            CACHE INTERNAL "Known BobUI module ${module} tools" FORCE)
    endif()
endmacro()
