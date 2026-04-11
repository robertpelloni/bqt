include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6dbus_args BobUIDBus)
find_package(BobUI6DBus ${_bobui_find_bobui6dbus_args})

if(BobUI6DBus_FOUND)
    set(BobUIDBus_FOUND TRUE)
    set(BobUIDBus_VERSION "${BobUI6DBus_VERSION}")
    set(BobUIDBus_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::DBus" "BobUI6::DBus")
else()
    set(BobUIDBus_FOUND FALSE)
endif()
