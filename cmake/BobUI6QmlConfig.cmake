include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6qml_args BobUI6Qml)
find_package(Qt6Qml ${_bobui_find_qt6qml_args})

if(Qt6Qml_FOUND)
    set(BobUI6Qml_FOUND TRUE)
    set(_bobui_module_version "${Qt6Qml_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Qml_VERSION "${_bobui_module_version}")
    set(BobUI6Qml_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Qml" "Qt6::Qml")
    _bobui_create_target_forwarder("BobUI::Qml" "Qt::Qml")
    if(NOT TARGET "BobUI::Qml")
        _bobui_create_target_forwarder("BobUI::Qml" "Qt6::Qml")
    endif()
else()
    set(BobUI6Qml_FOUND FALSE)
endif()
