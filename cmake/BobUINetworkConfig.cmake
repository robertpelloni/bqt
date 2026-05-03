include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6network_args BobUINetwork)
find_package(BobUI6Network ${_bobui_find_bobui6network_args})

if(BobUI6Network_FOUND)
    set(BobUINetwork_FOUND TRUE)
    set(BobUINetwork_VERSION "${BobUI6Network_VERSION}")
    set(BobUINetwork_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Network" "BobUI6::Network")
else()
    set(BobUINetwork_FOUND FALSE)
endif()
