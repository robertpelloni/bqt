set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")
include("${_bobui_repo_root}/cmake/BobUISupportedComponents.cmake")

if(NOT BOBUI_SUPPORTED_COMPONENTS)
    message(FATAL_ERROR "Expected BOBUI_SUPPORTED_COMPONENTS to be non-empty")
endif()

set(_bobui_forwarding_smoke "${CMAKE_CURRENT_LIST_DIR}/bobui_package_forwarding_smoke.cmake")
set(_bobui_publication_smoke "${CMAKE_CURRENT_LIST_DIR}/bobui_export_publication_configure_smoke.cmake")

file(READ "${_bobui_forwarding_smoke}" _bobui_forwarding_content)
file(READ "${_bobui_publication_smoke}" _bobui_publication_content)

# Verify that smoke tests use the manifest loop
if(NOT _bobui_forwarding_content MATCHES "foreach.*IN LISTS BOBUI_SUPPORTED_COMPONENTS")
    message(FATAL_ERROR "Forwarding smoke test does not appear to use BOBUI_SUPPORTED_COMPONENTS manifest loop")
endif()
if(NOT _bobui_publication_content MATCHES "foreach.*IN LISTS BOBUI_SUPPORTED_COMPONENTS")
    message(FATAL_ERROR "Publication smoke test does not appear to use BOBUI_SUPPORTED_COMPONENTS manifest loop")
endif()

foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    set(_bobui_v6_config "${_bobui_repo_root}/cmake/BobUI6${_component}Config.cmake")
    set(_bobui_config "${_bobui_repo_root}/cmake/BobUI${_component}Config.cmake")

    if(NOT EXISTS "${_bobui_v6_config}")
        message(FATAL_ERROR "Missing supported BobUI6 component config: ${_bobui_v6_config}")
    endif()
    if(NOT EXISTS "${_bobui_config}")
        message(FATAL_ERROR "Missing supported BobUI component config: ${_bobui_config}")
    endif()
endforeach()

message(STATUS "BobUI supported component manifest consistency checks passed")
