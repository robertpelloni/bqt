include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6printsupport_args BobUIPrintSupport)
find_package(BobUI6PrintSupport ${_bobui_find_bobui6printsupport_args})

if(BobUI6PrintSupport_FOUND)
    set(BobUIPrintSupport_FOUND TRUE)
    set(BobUIPrintSupport_VERSION "${BobUI6PrintSupport_VERSION}")
    set(BobUIPrintSupport_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::PrintSupport" "BobUI6::PrintSupport")
else()
    set(BobUIPrintSupport_FOUND FALSE)
endif()
