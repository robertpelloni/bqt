include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6concurrent_args BobUI6Concurrent)
find_package(Qt6Concurrent ${_bobui_find_qt6concurrent_args})

if(Qt6Concurrent_FOUND)
    set(BobUI6Concurrent_FOUND TRUE)
    set(_bobui_module_version "${Qt6Concurrent_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Concurrent_VERSION "${_bobui_module_version}")
    set(BobUI6Concurrent_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Concurrent" "Qt6::Concurrent")
    _bobui_create_target_forwarder("BobUI::Concurrent" "Qt::Concurrent")
    if(NOT TARGET "BobUI::Concurrent")
        _bobui_create_target_forwarder("BobUI::Concurrent" "Qt6::Concurrent")
    endif()
else()
    set(BobUI6Concurrent_FOUND FALSE)
endif()
