# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#
# Copy/Install doc configuration files to the build/install directory
#
bobui_path_join(doc_install_dir ${BOBUI_INSTALL_DIR} ${INSTALL_DOCDIR})
foreach(dir global config)
    bobui_copy_or_install(DIRECTORY ${dir} DESTINATION ${doc_install_dir})
    if(BOBUI_SUPERBUILD OR "${PROJECT_NAME}" STREQUAL "BobUIBase")
        bobui_path_join(destination ${BobUIBase_BINARY_DIR} ${INSTALL_DOCDIR})
        file(COPY ${dir} DESTINATION ${destination})

        if(NOT BOBUI_NO_SET_BOBUIBASE_DOC_FILE_DEPS_COPYING)
            # Make sure touched doc files cause a reconfigure, so they get re-copied.
            # TODO: Consider making this a build time file copy dependency.
            # It is more complicated, because all documentation generation custom targets would have
            # to depend on the file outputs of add_custom_command(copy), and it's not clear how the
            # dependencies should be set up in that case (coarsness, use install vs build dirs,
            # etc).
            file(GLOB_RECURSE files LIST_DIRECTORIES FALSE "${dir}/*")
            _bobui_internal_append_cmake_configure_depends(${files})
        endif()
    endif()
endforeach()
