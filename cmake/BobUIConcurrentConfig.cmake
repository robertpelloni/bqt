include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6concurrent_args BobUIConcurrent)
find_package(BobUI6Concurrent ${_bobui_find_bobui6concurrent_args})

if(BobUI6Concurrent_FOUND)
    set(BobUIConcurrent_FOUND TRUE)
    set(BobUIConcurrent_VERSION "${BobUI6Concurrent_VERSION}")
    set(BobUIConcurrent_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Concurrent" "BobUI6::Concurrent")
else()
    set(BobUIConcurrent_FOUND FALSE)
endif()
