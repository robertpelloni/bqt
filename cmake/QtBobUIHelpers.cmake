# Copyright (C) 2026 The BobUI Project
# SPDX-License-Identifier: BSD-3-Clause

function(qt_internal_install_bobui_compat_package package_name config_source_file)
    qt_path_join(package_build_dir ${QT_CONFIG_BUILD_DIR} ${package_name})
    qt_path_join(package_install_dir ${QT_CONFIG_INSTALL_DIR} ${package_name})

    set(package_source_files
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUICompatibilityHelpers.cmake"
        "${config_source_file}"
    )

    qt_copy_or_install(FILES ${package_source_files} DESTINATION "${package_install_dir}")

    write_basic_package_version_file(
        "${package_build_dir}/${package_name}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    qt_copy_or_install(FILES
        "${package_build_dir}/${package_name}ConfigVersion.cmake"
        DESTINATION "${package_install_dir}"
    )

    if(QT_WILL_INSTALL)
        foreach(package_source_file_path ${package_source_files})
            file(COPY "${package_source_file_path}" DESTINATION "${package_build_dir}")
        endforeach()
    endif()
endfunction()
