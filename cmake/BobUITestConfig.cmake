include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6test_args BobUITest)
find_package(BobUI6Test ${_bobui_find_bobui6test_args})

if(BobUI6Test_FOUND)
    set(BobUITest_FOUND TRUE)
    set(BobUITest_VERSION "${BobUI6Test_VERSION}")
    set(BobUITest_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Test" "BobUI6::Test")
else()
    set(BobUITest_FOUND FALSE)
endif()
