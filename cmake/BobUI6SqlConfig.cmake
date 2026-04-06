include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6sql_args BobUI6Sql)
find_package(Qt6Sql ${_bobui_find_qt6sql_args})

if(Qt6Sql_FOUND)
    set(BobUI6Sql_FOUND TRUE)
    set(_bobui_module_version "${Qt6Sql_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Sql_VERSION "${_bobui_module_version}")
    set(BobUI6Sql_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Sql" "Qt6::Sql")
    _bobui_create_target_forwarder("BobUI::Sql" "Qt::Sql")
    if(NOT TARGET "BobUI::Sql")
        _bobui_create_target_forwarder("BobUI::Sql" "Qt6::Sql")
    endif()
else()
    set(BobUI6Sql_FOUND FALSE)
endif()
