include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6qml_args BobUIQml)
find_package(BobUI6Qml ${_bobui_find_bobui6qml_args})

if(BobUI6Qml_FOUND)
    set(BobUIQml_FOUND TRUE)
    set(BobUIQml_VERSION "${BobUI6Qml_VERSION}")
    set(BobUIQml_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Qml" "BobUI6::Qml")
else()
    set(BobUIQml_FOUND FALSE)
endif()
