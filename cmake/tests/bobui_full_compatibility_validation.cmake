set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")

set(_bobui_cmake_checks
    bobui_supported_component_manifest_consistency.cmake
    bobui_supported_component_report_consistency.cmake
    bobui_qtbase_native_configure_preflight.cmake
    bobui_compatibility_helpers_mapping.cmake
    bobui_package_forwarding_smoke.cmake
    bobui_export_publication_configure_smoke.cmake
)

foreach(_bobui_check IN LISTS _bobui_cmake_checks)
    set(_bobui_check_path "${CMAKE_CURRENT_LIST_DIR}/${_bobui_check}")
    if(NOT EXISTS "${_bobui_check_path}")
        message(FATAL_ERROR "Missing BobUI compatibility check: ${_bobui_check_path}")
    endif()

    execute_process(
        COMMAND "${CMAKE_COMMAND}" -P "${_bobui_check_path}"
        RESULT_VARIABLE _bobui_check_result
        OUTPUT_VARIABLE _bobui_check_stdout
        ERROR_VARIABLE _bobui_check_stderr
    )

    message(STATUS "BobUI compatibility check: ${_bobui_check}")
    if(NOT "${_bobui_check_stdout}" STREQUAL "")
        string(REGEX REPLACE "\n$" "" _bobui_check_stdout_trimmed "${_bobui_check_stdout}")
        message(STATUS "${_bobui_check_stdout_trimmed}")
    endif()
    if(NOT "${_bobui_check_stderr}" STREQUAL "")
        string(REGEX REPLACE "\n$" "" _bobui_check_stderr_trimmed "${_bobui_check_stderr}")
        message(STATUS "${_bobui_check_stderr_trimmed}")
    endif()

    if(NOT _bobui_check_result EQUAL 0)
        message(FATAL_ERROR "BobUI compatibility check failed: ${_bobui_check}")
    endif()
endforeach()

message(STATUS "BobUI full compatibility validation passed")
