include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6quick_args BobUIQuick)
find_package(BobUI6Quick ${_bobui_find_bobui6quick_args})

if(BobUI6Quick_FOUND)
    set(BobUIQuick_FOUND TRUE)
    set(BobUIQuick_VERSION "${BobUI6Quick_VERSION}")
    set(BobUIQuick_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Quick" "BobUI6::Quick")
else()
    set(BobUIQuick_FOUND FALSE)
endif()
