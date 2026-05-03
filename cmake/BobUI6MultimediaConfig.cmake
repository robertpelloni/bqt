include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6multimedia_args BobUI6Multimedia)
find_package(Qt6Multimedia ${_bobui_find_qt6multimedia_args})

if(Qt6Multimedia_FOUND)
    set(BobUI6Multimedia_FOUND TRUE)
    set(_bobui_module_version "${Qt6Multimedia_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Multimedia_VERSION "${_bobui_module_version}")
    set(BobUI6Multimedia_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Multimedia" "Qt6::Multimedia")
    _bobui_create_target_forwarder("BobUI::Multimedia" "Qt::Multimedia")
    if(NOT TARGET "BobUI::Multimedia")
        _bobui_create_target_forwarder("BobUI::Multimedia" "Qt6::Multimedia")
    endif()
else()
    set(BobUI6Multimedia_FOUND FALSE)
endif()
