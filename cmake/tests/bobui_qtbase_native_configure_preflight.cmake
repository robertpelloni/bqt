set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")
set(_bobui_build_dir "${CMAKE_CURRENT_LIST_DIR}/_bobui_qtbase_native_configure_smoke")

function(_bobui_find_first_program out_var)
    set(_bobui_program "")
    foreach(_candidate IN LISTS ARGN)
        find_program(_bobui_candidate_path NAMES "${_candidate}")
        if(_bobui_candidate_path)
            set(_bobui_program "${_bobui_candidate_path}")
            break()
        endif()
    endforeach()
    set(${out_var} "${_bobui_program}" PARENT_SCOPE)
endfunction()

_bobui_find_first_program(_bobui_c_compiler cl gcc clang cc)
_bobui_find_first_program(_bobui_cxx_compiler cl g++ clang++ c++)

if(DEFINED BOBUI_SKIP_NATIVE_CONFIGURE AND BOBUI_SKIP_NATIVE_CONFIGURE)
    message(STATUS
        "Skipping qtbase-native configure smoke: BOBUI_SKIP_NATIVE_CONFIGURE is enabled for this run.")
    file(REMOVE_RECURSE "${_bobui_build_dir}")
elseif("${_bobui_c_compiler}" STREQUAL "" OR "${_bobui_cxx_compiler}" STREQUAL "")
    message(STATUS
        "Skipping qtbase-native configure smoke: no usable C/C++ compiler was found in PATH. "
        "This environment cannot currently complete a native top-level CMake configure.")
    file(REMOVE_RECURSE "${_bobui_build_dir}")
else()
    file(REMOVE_RECURSE "${_bobui_build_dir}")
    execute_process(
        COMMAND "${CMAKE_COMMAND}"
            -S "${_bobui_repo_root}"
            -B "${_bobui_build_dir}"
            -DQT_BUILD_TESTS=OFF
            -DQT_BUILD_EXAMPLES=OFF
            -DBUILD_SHARED_LIBS=OFF
        RESULT_VARIABLE _bobui_configure_result
        OUTPUT_VARIABLE _bobui_configure_stdout
        ERROR_VARIABLE _bobui_configure_stderr
    )

    if(NOT _bobui_configure_result EQUAL 0)
        message(FATAL_ERROR
            "qtbase-native configure smoke failed.\n"
            "stdout:\n${_bobui_configure_stdout}\n"
            "stderr:\n${_bobui_configure_stderr}")
    endif()

    file(REMOVE_RECURSE "${_bobui_build_dir}")
    message(STATUS "qtbase-native configure smoke passed")
endif()
