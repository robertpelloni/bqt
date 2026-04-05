set(_bobui_publication_root "${CMAKE_CURRENT_LIST_DIR}/_bobui_export_publication_smoke")
set(_bobui_source_dir "${_bobui_publication_root}/src")
set(_bobui_build_dir "${_bobui_publication_root}/build")

file(REMOVE_RECURSE "${_bobui_publication_root}")
file(MAKE_DIRECTORY "${_bobui_source_dir}/cmake")

foreach(_bobui_file
    QtBobUIHelpers.cmake
    BobUICompatibilityHelpers.cmake
    BobUI6Config.cmake
    BobUIConfig.cmake
    BobUI6CoreConfig.cmake
    BobUICoreConfig.cmake
    BobUI6WidgetsConfig.cmake
    BobUIWidgetsConfig.cmake
)
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/../${_bobui_file}"
        DESTINATION "${_bobui_source_dir}/cmake")
endforeach()

file(WRITE "${_bobui_source_dir}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.16)
project(BobUIExportPublicationSmoke VERSION 6.12.0 LANGUAGES NONE)

include(CMakePackageConfigHelpers)

function(qt_path_join out_var)
    set(_qt_path_join_result "")
    foreach(_qt_path_join_part IN LISTS ARGN)
        if("${_qt_path_join_part}" STREQUAL "")
            continue()
        endif()
        if("${_qt_path_join_result}" STREQUAL "")
            set(_qt_path_join_result "${_qt_path_join_part}")
        elseif(_qt_path_join_result MATCHES "/$")
            set(_qt_path_join_result "${_qt_path_join_result}${_qt_path_join_part}")
        else()
            set(_qt_path_join_result "${_qt_path_join_result}/${_qt_path_join_part}")
        endif()
    endforeach()
    set(${out_var} "${_qt_path_join_result}" PARENT_SCOPE)
endfunction()

function(qt_copy_or_install)
    set(options FILES PROGRAMS DIRECTORY)
    cmake_parse_arguments(arg "${options}" "DESTINATION" "" ${ARGN})

    if(NOT arg_DESTINATION)
        message(FATAL_ERROR "qt_copy_or_install smoke stub requires DESTINATION")
    endif()

    file(MAKE_DIRECTORY "${arg_DESTINATION}")

    set(_bobui_copy_inputs ${ARGN})
    list(REMOVE_ITEM _bobui_copy_inputs FILES PROGRAMS DIRECTORY DESTINATION "${arg_DESTINATION}")
    foreach(_bobui_copy_input IN LISTS _bobui_copy_inputs)
        if(EXISTS "${_bobui_copy_input}")
            file(COPY "${_bobui_copy_input}" DESTINATION "${arg_DESTINATION}")
        endif()
    endforeach()
endfunction()

set(QT_CONFIG_BUILD_DIR "${CMAKE_BINARY_DIR}/prefix/lib/cmake")
set(QT_CONFIG_INSTALL_DIR "${CMAKE_BINARY_DIR}/install/lib/cmake")
set(QT_WILL_INSTALL ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtBobUIHelpers.cmake")

qt_internal_install_bobui_compat_package(
    "BobUI6"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUI6Config.cmake"
)
qt_internal_install_bobui_compat_package(
    "BobUI"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIConfig.cmake"
)
qt_internal_install_bobui_compat_package(
    "BobUI6Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUI6CoreConfig.cmake"
)
qt_internal_install_bobui_compat_package(
    "BobUICore"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUICoreConfig.cmake"
)
qt_internal_install_bobui_compat_package(
    "BobUI6Widgets"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUI6WidgetsConfig.cmake"
)
qt_internal_install_bobui_compat_package(
    "BobUIWidgets"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIWidgetsConfig.cmake"
)

foreach(_bobui_expected_file
    "${QT_CONFIG_BUILD_DIR}/BobUI6/BobUI6Config.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6/BobUI6ConfigVersion.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI/BobUIConfig.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI/BobUIConfigVersion.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Core/BobUI6CoreConfig.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Core/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Core/BobUI6CoreConfigVersion.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUICore/BobUICoreConfig.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUICore/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUICore/BobUICoreConfigVersion.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Widgets/BobUI6WidgetsConfig.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Widgets/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUI6Widgets/BobUI6WidgetsConfigVersion.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUIWidgets/BobUIWidgetsConfig.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUIWidgets/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_BUILD_DIR}/BobUIWidgets/BobUIWidgetsConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6/BobUI6Config.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6/BobUI6ConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI/BobUIConfig.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI/BobUIConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Core/BobUI6CoreConfig.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Core/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Core/BobUI6CoreConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUICore/BobUICoreConfig.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUICore/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUICore/BobUICoreConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Widgets/BobUI6WidgetsConfig.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Widgets/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUI6Widgets/BobUI6WidgetsConfigVersion.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUIWidgets/BobUIWidgetsConfig.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUIWidgets/BobUICompatibilityHelpers.cmake"
    "${QT_CONFIG_INSTALL_DIR}/BobUIWidgets/BobUIWidgetsConfigVersion.cmake"
)
    if(NOT EXISTS "${_bobui_expected_file}")
        message(FATAL_ERROR "Expected BobUI publication file to exist: ${_bobui_expected_file}")
    endif()
endforeach()

file(READ "${QT_CONFIG_BUILD_DIR}/BobUI6/BobUI6ConfigVersion.cmake" _bobui_version_file)
if(NOT _bobui_version_file MATCHES "6\\.12\\.0")
    message(FATAL_ERROR "Expected BobUI6ConfigVersion.cmake to contain version 6.12.0")
endif()
]=])

execute_process(
    COMMAND "${CMAKE_COMMAND}" -S "${_bobui_source_dir}" -B "${_bobui_build_dir}"
    RESULT_VARIABLE _bobui_configure_result
    OUTPUT_VARIABLE _bobui_configure_stdout
    ERROR_VARIABLE _bobui_configure_stderr
)

if(NOT _bobui_configure_result EQUAL 0)
    message(FATAL_ERROR
        "BobUI export publication configure smoke test failed.\n"
        "stdout:\n${_bobui_configure_stdout}\n"
        "stderr:\n${_bobui_configure_stderr}")
endif()

file(REMOVE_RECURSE "${_bobui_publication_root}")
message(STATUS "BobUI export publication configure smoke test passed")
