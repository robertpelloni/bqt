set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")
set(_bobui_report_path
    "${_bobui_repo_root}/docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md")
set(_bobui_generated_report_path
    "${CMAKE_CURRENT_LIST_DIR}/_bobui_supported_component_status_generated.md")
set(_bobui_generator_script
    "${_bobui_repo_root}/cmake/bobui_generate_supported_component_report.cmake")

if(NOT EXISTS "${_bobui_report_path}")
    message(FATAL_ERROR "Expected supported component status report to exist: ${_bobui_report_path}")
endif()
if(NOT EXISTS "${_bobui_generator_script}")
    message(FATAL_ERROR "Expected report generator script to exist: ${_bobui_generator_script}")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -DOUTPUT_PATH=${_bobui_generated_report_path} -P "${_bobui_generator_script}"
    RESULT_VARIABLE _bobui_generate_result
    OUTPUT_VARIABLE _bobui_generate_stdout
    ERROR_VARIABLE _bobui_generate_stderr
)

if(NOT _bobui_generate_result EQUAL 0)
    message(FATAL_ERROR
        "Failed to generate supported component report.\n"
        "stdout:\n${_bobui_generate_stdout}\n"
        "stderr:\n${_bobui_generate_stderr}")
endif()

file(READ "${_bobui_report_path}" _bobui_report_content)
file(READ "${_bobui_generated_report_path}" _bobui_generated_report_content)

if(NOT _bobui_report_content STREQUAL _bobui_generated_report_content)
    message(FATAL_ERROR
        "Checked-in BobUI supported component report does not match generated output. "
        "Regenerate it with cmake -P cmake/bobui_generate_supported_component_report.cmake")
endif()

file(REMOVE "${_bobui_generated_report_path}")
message(STATUS "BobUI supported component report consistency checks passed")
