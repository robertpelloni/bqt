set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")
include("${_bobui_repo_root}/cmake/BobUISupportedComponents.cmake")

set(_bobui_report_path
    "${_bobui_repo_root}/docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md")

if(NOT EXISTS "${_bobui_report_path}")
    message(FATAL_ERROR "Expected supported component status report to exist: ${_bobui_report_path}")
endif()

file(READ "${_bobui_report_path}" _bobui_report_content)

foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    if(NOT _bobui_report_content MATCHES "\| ${_component} \|")
        message(FATAL_ERROR "Supported component report is missing table row for ${_component}")
    endif()
    if(NOT _bobui_report_content MATCHES "BobUI6${_component}")
        message(FATAL_ERROR "Supported component report is missing BobUI6${_component}")
    endif()
    if(NOT _bobui_report_content MATCHES "BobUI${_component}")
        message(FATAL_ERROR "Supported component report is missing BobUI${_component}")
    endif()
    if(NOT _bobui_report_content MATCHES "BobUI6::${_component}")
        message(FATAL_ERROR "Supported component report is missing BobUI6::${_component}")
    endif()
    if(NOT _bobui_report_content MATCHES "BobUI::${_component}")
        message(FATAL_ERROR "Supported component report is missing BobUI::${_component}")
    endif()
endforeach()

message(STATUS "BobUI supported component report consistency checks passed")
