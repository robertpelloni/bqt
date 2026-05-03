include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6multimedia_args BobUIMultimedia)
find_package(BobUI6Multimedia ${_bobui_find_bobui6multimedia_args})

if(BobUI6Multimedia_FOUND)
    set(BobUIMultimedia_FOUND TRUE)
    set(BobUIMultimedia_VERSION "${BobUI6Multimedia_VERSION}")
    set(BobUIMultimedia_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Multimedia" "BobUI6::Multimedia")
else()
    set(BobUIMultimedia_FOUND FALSE)
endif()
