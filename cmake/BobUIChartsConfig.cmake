include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6charts_args BobUICharts)
find_package(BobUI6Charts ${_bobui_find_bobui6charts_args})

if(BobUI6Charts_FOUND)
    set(BobUICharts_FOUND TRUE)
    set(BobUICharts_VERSION "${BobUI6Charts_VERSION}")
    set(BobUICharts_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Charts" "BobUI6::Charts")
else()
    set(BobUICharts_FOUND FALSE)
endif()
