include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6_args BobUI6)
find_package(Qt6 ${_bobui_find_qt6_args})

if(Qt6_FOUND)
    set(BobUI6_FOUND TRUE)
    set(BobUI6_VERSION "${Qt6_VERSION}")
    set(BobUI6_VERSION_MAJOR "${Qt6_VERSION_MAJOR}")
    set(BobUI6_VERSION_MINOR "${Qt6_VERSION_MINOR}")
    set(BobUI6_VERSION_PATCH "${Qt6_VERSION_PATCH}")
    set(BobUI6_DIR "${CMAKE_CURRENT_LIST_DIR}")

    if(DEFINED BobUI6_FIND_COMPONENTS AND NOT "${BobUI6_FIND_COMPONENTS}" STREQUAL "")
        bobui_create_component_compat_targets("BobUI6" "Qt6" "Qt" ${BobUI6_FIND_COMPONENTS})
        bobui_create_component_compat_targets("BobUI" "Qt" "Qt6" ${BobUI6_FIND_COMPONENTS})
    endif()
else()
    set(BobUI6_FOUND FALSE)
endif()
