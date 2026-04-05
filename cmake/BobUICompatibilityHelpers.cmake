# BobUI compatibility helpers
#
# These helpers provide a small additive compatibility surface for BobUI-facing
# package discovery without rewriting the canonical Qt-derived build internals.
#
# The goal is to let future BobUI package/export surfaces forward to the tracked
# Qt package layout in small verified slices.

function(bobui_compute_qt_package_name out_var bobui_package)
    if("${bobui_package}" MATCHES "^BobUI([0-9]+)(.*)$")
        set(_bobui_major "${CMAKE_MATCH_1}")
        set(_bobui_suffix "${CMAKE_MATCH_2}")
        set(_bobui_result "Qt${_bobui_major}${_bobui_suffix}")
    elseif("${bobui_package}" MATCHES "^BobUI(.*)$")
        set(_bobui_suffix "${CMAKE_MATCH_1}")
        if(_bobui_suffix STREQUAL "")
            set(_bobui_result "Qt6")
        else()
            set(_bobui_result "Qt${_bobui_suffix}")
        endif()
    else()
        set(_bobui_result "${bobui_package}")
    endif()

    set(${out_var} "${_bobui_result}" PARENT_SCOPE)
endfunction()

function(bobui_collect_forward_find_package_args out_var package_prefix)
    set(_bobui_args CONFIG)

    if(DEFINED ${package_prefix}_FIND_VERSION AND NOT "${${package_prefix}_FIND_VERSION}" STREQUAL "")
        list(APPEND _bobui_args "${${package_prefix}_FIND_VERSION}")
    endif()

    if(DEFINED ${package_prefix}_FIND_VERSION_EXACT AND ${${package_prefix}_FIND_VERSION_EXACT})
        list(APPEND _bobui_args EXACT)
    endif()

    if(DEFINED ${package_prefix}_FIND_QUIETLY AND ${${package_prefix}_FIND_QUIETLY})
        list(APPEND _bobui_args QUIET)
    endif()

    if(DEFINED ${package_prefix}_FIND_REQUIRED AND ${${package_prefix}_FIND_REQUIRED})
        list(APPEND _bobui_args REQUIRED)
    endif()

    if(DEFINED ${package_prefix}_FIND_COMPONENTS AND NOT "${${package_prefix}_FIND_COMPONENTS}" STREQUAL "")
        list(APPEND _bobui_args COMPONENTS ${${package_prefix}_FIND_COMPONENTS})
    endif()

    set(${out_var} "${_bobui_args}" PARENT_SCOPE)
endfunction()

function(_bobui_create_target_forwarder compat_target real_target)
    if(TARGET "${real_target}" AND NOT TARGET "${compat_target}")
        add_library("${compat_target}" INTERFACE IMPORTED)
        set_property(TARGET "${compat_target}" PROPERTY INTERFACE_LINK_LIBRARIES "${real_target}")
    endif()
endfunction()

function(bobui_create_component_compat_targets compat_namespace primary_qt_namespace fallback_qt_namespace)
    foreach(component IN LISTS ARGN)
        if("${component}" STREQUAL "")
            continue()
        endif()

        set(_bobui_real_target "${primary_qt_namespace}::${component}")
        if(NOT TARGET "${_bobui_real_target}" AND NOT "${fallback_qt_namespace}" STREQUAL "")
            set(_bobui_real_target "${fallback_qt_namespace}::${component}")
        endif()

        _bobui_create_target_forwarder("${compat_namespace}::${component}" "${_bobui_real_target}")
    endforeach()
endfunction()
