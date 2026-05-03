include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6dbus_args BobUI6DBus)
find_package(Qt6DBus ${_bobui_find_qt6dbus_args})

if(Qt6DBus_FOUND)
    set(BobUI6DBus_FOUND TRUE)
    set(_bobui_module_version "${Qt6DBus_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6DBus_VERSION "${_bobui_module_version}")
    set(BobUI6DBus_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::DBus" "Qt6::DBus")
    _bobui_create_target_forwarder("BobUI::DBus" "Qt::DBus")
    if(NOT TARGET "BobUI::DBus")
        _bobui_create_target_forwarder("BobUI::DBus" "Qt6::DBus")
    endif()
else()
    set(BobUI6DBus_FOUND FALSE)
endif()
