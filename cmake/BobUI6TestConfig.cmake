include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6test_args BobUI6Test)
find_package(Qt6Test ${_bobui_find_qt6test_args})

if(Qt6Test_FOUND)
    set(BobUI6Test_FOUND TRUE)
    set(_bobui_module_version "${Qt6Test_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Test_VERSION "${_bobui_module_version}")
    set(BobUI6Test_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Test" "Qt6::Test")
    _bobui_create_target_forwarder("BobUI::Test" "Qt::Test")
    if(NOT TARGET "BobUI::Test")
        _bobui_create_target_forwarder("BobUI::Test" "Qt6::Test")
    endif()
else()
    set(BobUI6Test_FOUND FALSE)
endif()
