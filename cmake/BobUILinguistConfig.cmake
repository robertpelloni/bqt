include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6linguist_args BobUILinguist)
find_package(BobUI6Linguist ${_bobui_find_bobui6linguist_args})

if(BobUI6Linguist_FOUND)
    set(BobUILinguist_FOUND TRUE)
    set(BobUILinguist_VERSION "${BobUI6Linguist_VERSION}")
    set(BobUILinguist_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Linguist" "BobUI6::Linguist")
else()
    set(BobUILinguist_FOUND FALSE)
endif()
