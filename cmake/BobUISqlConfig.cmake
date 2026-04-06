include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6sql_args BobUISql)
find_package(BobUI6Sql ${_bobui_find_bobui6sql_args})

if(BobUI6Sql_FOUND)
    set(BobUISql_FOUND TRUE)
    set(BobUISql_VERSION "${BobUI6Sql_VERSION}")
    set(BobUISql_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Sql" "BobUI6::Sql")
else()
    set(BobUISql_FOUND FALSE)
endif()
