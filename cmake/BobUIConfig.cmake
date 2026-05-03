include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6_args BobUI)
find_package(BobUI6 ${_bobui_find_bobui6_args})

if(BobUI6_FOUND)
    set(BobUI_FOUND TRUE)
    set(BobUI_VERSION "${BobUI6_VERSION}")
    set(BobUI_VERSION_MAJOR "${BobUI6_VERSION_MAJOR}")
    set(BobUI_VERSION_MINOR "${BobUI6_VERSION_MINOR}")
    set(BobUI_VERSION_PATCH "${BobUI6_VERSION_PATCH}")
    set(BobUI_DIR "${CMAKE_CURRENT_LIST_DIR}")

    if(DEFINED BobUI_FIND_COMPONENTS AND NOT "${BobUI_FIND_COMPONENTS}" STREQUAL "")
        bobui_create_component_compat_targets("BobUI" "Qt" "Qt6" ${BobUI_FIND_COMPONENTS})
    endif()
else()
    set(BobUI_FOUND FALSE)
endif()
