# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Macros and functions for building BobUI submodules

# The macro sets all the necessary pre-conditions and setup consistent environment for building
# the BobUI repository. It has to be called right after the find_package(BobUI6 COMPONENTS BuildInternals)
# call. Otherwise we cannot make sure that all the required policies will be applied to the BobUI
# components that are involved in build procedure.
macro(bobui_internal_project_setup)
    # Check for the minimum CMake version.
    bobui_internal_require_suitable_cmake_version()
    bobui_internal_upgrade_cmake_policies()
    # Make sure BOBUI_INTERNAL_BUILD_STANDALONE_PARTS is defined as early as possible
    bobui_internal_setup_standalone_parts()
endmacro()

macro(bobui_build_internals_set_up_private_api)
    # TODO: this call needs to be removed once all repositories got the bobuibase update
    bobui_internal_project_setup()

    # BobUI specific setup common for all modules:
    include(BobUISetup)

    # Optionally include a repo specific Setup module.
    include(${PROJECT_NAME}Setup OPTIONAL)
    include(BobUIRepoSetup OPTIONAL)

    # Find Apple frameworks if needed.
    bobui_find_apple_system_frameworks()

    # Decide whether tools will be built.
    bobui_check_if_tools_will_be_built()
endmacro()

# add toplevel targets for each subdirectory, e.g. bobuibase_src
function(bobui_build_internals_add_toplevel_targets bobui_repo_targets_name)
    set(bobui_repo_target_all "")
    get_directory_property(directories DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" SUBDIRECTORIES)
    foreach(directory IN LISTS directories)
        set(bobui_repo_targets "")
        get_filename_component(bobui_repo_target_basename ${directory} NAME)
        _bobui_internal_collect_buildsystem_targets(bobui_repo_targets "${directory}" EXCLUDE UTILITY)
        if (bobui_repo_targets)
            set(bobui_repo_target_name "${bobui_repo_targets_name}_${bobui_repo_target_basename}")
            message(DEBUG "${bobui_repo_target_name} depends on ${bobui_repo_targets}")
            add_custom_target("${bobui_repo_target_name}"
                COMMENT "Building everything in ${bobui_repo_targets_name}/${bobui_repo_target_basename}")
            add_dependencies("${bobui_repo_target_name}" ${bobui_repo_targets})
            list(APPEND bobui_repo_target_all "${bobui_repo_target_name}")

            # Create special dependency target for External Project examples excluding targets
            # marked as skipped.
            if(bobui_repo_target_basename STREQUAL "src")
                set(bobui_repo_target_name
                    "${bobui_repo_targets_name}_${bobui_repo_target_basename}_for_examples")
                add_custom_target("${bobui_repo_target_name}")

                set(unskipped_targets "")
                foreach(target IN LISTS bobui_repo_targets)
                    if(TARGET "${target}")
                        bobui_internal_is_target_skipped_for_examples("${target}" is_skipped)
                        if(NOT is_skipped)
                            list(APPEND unskipped_targets "${target}")
                        endif()
                    endif()
                endforeach()
                if(unskipped_targets)
                    add_dependencies("${bobui_repo_target_name}" ${unskipped_targets})
                endif()
            endif()
        endif()

    endforeach()
    if (bobui_repo_target_all)
        # Note bobui_repo_targets_name is different from bobui_repo_target_name that is used above.
        add_custom_target("${bobui_repo_targets_name}"
                            COMMENT "Building everything in ${bobui_repo_targets_name}")
        add_dependencies("${bobui_repo_targets_name}" ${bobui_repo_target_all})
        message(DEBUG "${bobui_repo_targets_name} depends on ${bobui_repo_target_all}")
    endif()
endfunction()

macro(bobui_enable_cmake_languages)
    set(__bobui_required_language_list C CXX)
    set(__bobui_platform_required_language_list )

    if(APPLE)
        list(APPEND __bobui_platform_required_language_list OBJC OBJCXX)
    endif()

    foreach(__bobui_lang ${__bobui_required_language_list})
        enable_language(${__bobui_lang})
    endforeach()

    foreach(__bobui_lang ${__bobui_platform_required_language_list})
        enable_language(${__bobui_lang})
    endforeach()

    # The bobuibase call is handled in bobuibase/CMakeLists.txt.
    # This call is used for projects other than bobuibase, including for other project's standalone
    # tests/examples.
    # Because the function uses BOBUI_FEATURE_foo values, it's important that find_package(BobUI6Core) is
    # called before this function. but that's usually the case for BobUI repos.
    if(NOT PROJECT_NAME STREQUAL "BobUIBase")
        bobui_internal_set_up_config_optimizations_like_in_qmake()
    endif()
endmacro()

# Minimum setup required to have any CMakeList.txt build as as a standalone
# project after importing BuildInternals
macro(bobui_prepare_standalone_project)
    bobui_set_up_build_internals_paths()
    bobui_build_internals_set_up_private_api()
    bobui_enable_cmake_languages()
endmacro()

# Define a repo target set, and store accompanying information.
#
# A repo target set is a subset of targets in a BobUI module repository. To build a repo target set,
# set BOBUI_BUILD_SINGLE_REPO_TARGET_SET to the name of the repo target set.
#
# This function is to be called in the top-level project file of a repository,
# before bobui_internal_prepare_single_repo_target_set_build()
#
# This function stores information in variables of the parent scope.
#
# Positional Arguments:
#   name - The name of this repo target set.
#
# Named Arguments:
#   DEPENDS - List of BobUI6 COMPONENTS that are build dependencies of this repo target set.
function(bobui_internal_define_repo_target_set name)
    set(oneValueArgs DEPENDS)
    set(prefix BOBUI_REPO_TARGET_SET_)
    cmake_parse_arguments(${prefix}${name} "" ${oneValueArgs} "" ${ARGN})
    foreach(arg IN LISTS oneValueArgs)
        set(${prefix}${name}_${arg} ${${prefix}${name}_${arg}} PARENT_SCOPE)
    endforeach()
    set(BOBUI_REPO_KNOWN_TARGET_SETS "${BOBUI_REPO_KNOWN_TARGET_SETS};${name}" PARENT_SCOPE)
endfunction()

# Setup a single repo target set build if BOBUI_BUILD_SINGLE_REPO_TARGET_SET is defined.
#
# This macro must be called in the top-level project file of the repository after all repo target
# sets have been defined.
macro(bobui_internal_prepare_single_repo_target_set_build)
    if(DEFINED BOBUI_BUILD_SINGLE_REPO_TARGET_SET)
        if(NOT BOBUI_BUILD_SINGLE_REPO_TARGET_SET IN_LIST BOBUI_REPO_KNOWN_TARGET_SETS)
            message(FATAL_ERROR
                "Repo target set '${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}' is undefined.")
        endif()
        message(STATUS
            "Preparing single repo target set build of ${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}")
        if (NOT "${BOBUI_REPO_TARGET_SET_${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}_DEPENDS}" STREQUAL "")
            find_package(${INSTALL_CMAKE_NAMESPACE} ${PROJECT_VERSION} CONFIG REQUIRED
                COMPONENTS ${BOBUI_REPO_TARGET_SET_${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}_DEPENDS})
        endif()
    endif()
endmacro()

# There are three necessary copies of this macro in
#  bobuibase/cmake/BobUIBaseHelpers.cmake
#  bobuibase/cmake/BobUIBaseTopLevelHelpers.cmake
#  bobuibase/cmake/BobUIBuildRepoHelpers.cmake
macro(bobui_internal_setup_standalone_parts)
    # A generic marker for any kind of standalone builds, either tests or examples.
    if(NOT DEFINED BOBUI_INTERNAL_BUILD_STANDALONE_PARTS
            AND (BOBUI_BUILD_STANDALONE_TESTS OR BOBUI_BUILD_STANDALONE_EXAMPLES))
        set(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS TRUE CACHE INTERNAL
            "Whether standalone tests or examples are being built")
    endif()
endmacro()

macro(bobui_internal_setup_global_doc_targets)
    # Add global docs targets that will work both for per-repo builds, and super builds.
    if(NOT TARGET docs AND BOBUI_BUILD_DOCS)
        add_custom_target(docs)
        add_custom_target(prepare_docs)
        add_custom_target(generate_docs)
        add_custom_target(html_docs)
        add_custom_target(qch_docs)
        add_custom_target(install_html_docs)
        add_custom_target(install_qch_docs)
        add_custom_target(install_docs)
        add_dependencies(html_docs generate_docs)
        add_dependencies(docs html_docs qch_docs)
        add_dependencies(install_docs install_html_docs install_qch_docs)
    endif()

    if(BOBUI_BUILD_DOCS)
        set(bobui_docs_target_name docs_${project_name_lower})
        set(bobui_docs_prepare_target_name prepare_docs_${project_name_lower})
        set(bobui_docs_generate_target_name generate_docs_${project_name_lower})
        set(bobui_docs_html_target_name html_docs_${project_name_lower})
        set(bobui_docs_qch_target_name qch_docs_${project_name_lower})
        set(bobui_docs_install_html_target_name install_html_docs_${project_name_lower})
        set(bobui_docs_install_qch_target_name install_qch_docs_${project_name_lower})
        set(bobui_docs_install_target_name install_docs_${project_name_lower})

        add_custom_target(${bobui_docs_target_name})
        add_custom_target(${bobui_docs_prepare_target_name})
        add_custom_target(${bobui_docs_generate_target_name})
        add_custom_target(${bobui_docs_qch_target_name})
        add_custom_target(${bobui_docs_html_target_name})
        add_custom_target(${bobui_docs_install_html_target_name})
        add_custom_target(${bobui_docs_install_qch_target_name})
        add_custom_target(${bobui_docs_install_target_name})

        add_dependencies(${bobui_docs_generate_target_name} ${bobui_docs_prepare_target_name})
        add_dependencies(${bobui_docs_html_target_name} ${bobui_docs_generate_target_name})
        add_dependencies(${bobui_docs_target_name}
            ${bobui_docs_html_target_name} ${bobui_docs_qch_target_name})
        add_dependencies(${bobui_docs_install_target_name}
            ${bobui_docs_install_html_target_name} ${bobui_docs_install_qch_target_name})

        # Make top-level prepare_docs target depend on the repository-level
        # prepare_docs_<repo> target.
        add_dependencies(prepare_docs ${bobui_docs_prepare_target_name})

        # Make top-level install_*_docs targets depend on the repository-level
        # install_*_docs targets.
        add_dependencies(install_html_docs ${bobui_docs_install_html_target_name})
        add_dependencies(install_qch_docs ${bobui_docs_install_qch_target_name})
    endif()
endmacro()

macro(bobui_build_repo_begin)
    bobui_internal_setup_standalone_parts()

    set(BOBUI_INTERNAL_REPO_POST_PROCESS_CALLED FALSE)
    list(APPEND CMAKE_MESSAGE_CONTEXT "${PROJECT_NAME}")

    bobui_build_internals_set_up_private_api()

    # Prevent installation in non-prefix builds.
    # We need to associate targets with export names, and that is only possible to do with the
    # install(TARGETS) command. But in a non-prefix build, we don't want to install anything.
    # To make sure that developers don't accidentally run make install, add bail out code to
    # cmake_install.cmake.
    if(NOT BOBUI_WILL_INSTALL)
        # In a top-level build, print a message only in bobuibase, which is the first repository.
        if(NOT BOBUI_SUPERBUILD OR (PROJECT_NAME STREQUAL "BobUIBase"))
            install(CODE [[message(FATAL_ERROR
                    "BobUI was configured as non-prefix build. "
                    "Installation is not supported for this arrangement.")]])
        endif()

        install(CODE [[return()]])
    endif()

    bobui_enable_cmake_languages()

    bobui_internal_generate_binary_strip_wrapper()
    bobui_internal_create_bobui_configure_redo_script()

    if(NOT TARGET sync_headers)
        add_custom_target(sync_headers)
    endif()

    # The special target that we use to sync 3rd-party headers before the gn run when building
    # bobuiwebengine in top-level builds.
    if(NOT TARGET thirdparty_sync_headers)
        add_custom_target(thirdparty_sync_headers)
    endif()

    # Add global bobui_plugins, qpa_plugins and qpa_default_plugins convenience custom targets.
    # Internal executables will add a dependency on the qpa_default_plugins target,
    # so that building and running a test ensures it won't fail at runtime due to a missing qpa
    # plugin.
    if(NOT TARGET bobui_plugins)
        add_custom_target(bobui_plugins)
        add_custom_target(qpa_plugins)
        add_custom_target(qpa_default_plugins)
    endif()

    string(TOLOWER ${PROJECT_NAME} project_name_lower)

    # Target to build all plugins that are part of the current repo.
    set(bobui_repo_plugins "bobui_plugins_${project_name_lower}")
    if(NOT TARGET ${bobui_repo_plugins})
        add_custom_target(${bobui_repo_plugins})
    endif()

    # Target to build all plugins that are part of the current repo and the current repo's
    # dependencies plugins. Used for external project example dependencies.
    set(bobui_repo_plugins_recursive "${bobui_repo_plugins}_recursive")
    if(NOT TARGET ${bobui_repo_plugins_recursive})
        add_custom_target(${bobui_repo_plugins_recursive})
        add_dependencies(${bobui_repo_plugins_recursive} "${bobui_repo_plugins}")
    endif()

    bobui_internal_read_repo_dependencies(bobui_repo_deps "${PROJECT_SOURCE_DIR}")
    if(bobui_repo_deps)
        set_property(GLOBAL PROPERTY _bobui_repo_deps_${project_name_lower} ${bobui_repo_deps})
        foreach(bobui_repo_dep IN LISTS bobui_repo_deps)
            if(TARGET bobui_plugins_${bobui_repo_dep})
                message(DEBUG
                    "${bobui_repo_plugins_recursive} depends on bobui_plugins_${bobui_repo_dep}")
                add_dependencies(${bobui_repo_plugins_recursive} "bobui_plugins_${bobui_repo_dep}")
            endif()
        endforeach()
    endif()

    set(bobui_repo_targets_name ${project_name_lower})

    bobui_internal_setup_global_doc_targets()

    # Add host_tools meta target, so that developrs can easily build only tools and their
    # dependencies when working in bobuibase.
    if(NOT TARGET host_tools)
        add_custom_target(host_tools)
        add_custom_target(bootstrap_tools)
        add_custom_target(doc_tools)

        # TODO: Investigate complexity of installing tools for shared builds.
        # Currently installing host tools without libraries only really makes sense for static
        # builds. Tracking dependencies for shared builds is more involved.
        if(NOT BUILD_SHARED_LIBS)
            add_custom_target(install_tools
                COMMAND ${CMAKE_COMMAND}
                    --install ${CMAKE_BINARY_DIR} --component host_tools
            )
            add_custom_target(install_tools_stripped
                COMMAND ${CMAKE_COMMAND}
                    --install ${CMAKE_BINARY_DIR} --component host_tools --strip
            )

            add_custom_target(install_doc_tools
                COMMAND ${CMAKE_COMMAND}
                    --install ${CMAKE_BINARY_DIR} --component doc_tools
            )
            add_custom_target(install_doc_tools_stripped
                COMMAND ${CMAKE_COMMAND}
                    --install ${CMAKE_BINARY_DIR} --component doc_tools --strip
            )

            if(NOT BOBUI_INTERNAL_NO_INSTALL_TOOLS_BUILD_DEPS)
                add_dependencies(install_tools host_tools)
                add_dependencies(install_tools_stripped host_tools)
                add_dependencies(install_doc_tools doc_tools)
                add_dependencies(install_doc_tools_stripped doc_tools)
            endif()
        endif()
    endif()

    # Add benchmark meta target. It's collection of all benchmarks added/registered by
    # 'bobui_internal_add_benchmark' helper.
    if(NOT TARGET benchmark)
        add_custom_target(benchmark)
    endif()

    if(BOBUI_INTERNAL_SYNCED_MODULES)
        set_property(GLOBAL PROPERTY _bobui_synced_modules ${BOBUI_INTERNAL_SYNCED_MODULES})
    endif()

    _bobui_internal_sbom_auto_begin_bobui_repo_project()
    bobui_internal_set_unity_build()
endmacro()

# Runs delayed actions on some of the BobUI targets.
# Can be called either explicitly or as part of bobui_build_repo_end().
macro(bobui_build_repo_post_process)
    if(NOT BOBUI_INTERNAL_REPO_POST_PROCESS_CALLED)
        set(BOBUI_INTERNAL_REPO_POST_PROCESS_CALLED TRUE)
        if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
            include(BobUIPostProcess)
        endif()
    endif()
endmacro()

macro(bobui_build_repo_end)
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        bobui_build_repo_post_process()

        # Install the repo-specific cmake find modules.
        bobui_path_join(__bobui_repo_install_dir ${BOBUI_CONFIG_INSTALL_DIR} ${INSTALL_CMAKE_NAMESPACE})
        bobui_path_join(__bobui_repo_build_dir ${BOBUI_CONFIG_BUILD_DIR} ${INSTALL_CMAKE_NAMESPACE})

        if(NOT PROJECT_NAME STREQUAL "BobUIBase")
            if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
                    AND NOT BOBUI_NO_INSTALL_CMAKE_DIR_FIND_SCRIPTS)
                bobui_copy_or_install(DIRECTORY cmake/
                    DESTINATION "${__bobui_repo_install_dir}"
                    FILES_MATCHING PATTERN "Find*.cmake"
                )
                if(BOBUI_SUPERBUILD AND BOBUI_WILL_INSTALL)
                    file(COPY cmake/
                         DESTINATION "${__bobui_repo_build_dir}"
                         FILES_MATCHING PATTERN "Find*.cmake"
                    )
                endif()
            endif()
        endif()

        if(NOT BOBUI_SUPERBUILD)
            bobui_print_feature_summary()
        endif()
    endif()

    bobui_build_internals_add_toplevel_targets(${bobui_repo_targets_name})

    bobui_internal_show_extra_ide_sources()

    if(NOT BOBUI_SUPERBUILD)
        bobui_print_build_instructions()
    endif()

    get_property(synced_modules GLOBAL PROPERTY _bobui_synced_modules)
    if(synced_modules)
        set(BOBUI_INTERNAL_SYNCED_MODULES ${synced_modules} CACHE INTERNAL
            "List of the synced modules. Prevents running syncbobui.cpp after the first configuring.")
    endif()

    if(NOT BOBUI_SUPERBUILD)
        bobui_internal_save_previously_visited_packages()
    endif()

    if(BOBUI_INTERNAL_FRESH_REQUESTED)
        set(BOBUI_INTERNAL_FRESH_REQUESTED "FALSE" CACHE INTERNAL "")
    endif()

    _bobui_internal_sbom_auto_end_bobui_repo_project()

    if(NOT BOBUI_SUPERBUILD)
        bobui_internal_bobui_configure_end()
        if(BOBUI_WILL_INSTALL AND BOBUI_INSTALL_CONFIG_INFO_FILES)
            string(TOLOWER "${PROJECT_NAME}" repo_name)
            bobui_install(
                FILES
                    "${CMAKE_BINARY_DIR}/config.opt"
                RENAME
                    "config_${repo_name}.opt"
                DESTINATION ${INSTALL_DATADIR}
            )
            bobui_install(
                FILES
                    "${CMAKE_BINARY_DIR}/config.summary"
                RENAME
                    "config_${repo_name}.summary"
                DESTINATION ${INSTALL_DATADIR}
            )
        endif()
    endif()

    list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endmacro()

function(bobui_internal_show_extra_ide_sources)
    if(CMAKE_VERSION VERSION_LESS 3.20)
        set(ide_sources_default OFF)
    else()
        set(ide_sources_default ON)
    endif()

    option(BOBUI_SHOW_EXTRA_IDE_SOURCES "Generate CMake targets exposing non-source files to IDEs" ${ide_sources_default})
    if(CMAKE_VERSION VERSION_LESS 3.20 AND BOBUI_SHOW_EXTRA_IDE_SOURCES)
        message(WARNING "BOBUI_SHOW_EXTRA_IDE_SOURCES requires cmake-3.20")
        return()
    endif()

    if(NOT BOBUI_SHOW_EXTRA_IDE_SOURCES)
        return()
    endif()

    # coin
    set(coin_target_name ${bobui_repo_targets_name}_coin_files)
    file(GLOB_RECURSE coin_files LIST_DIRECTORIES false FOLLOW_SYMLINKS coin/*)
    if(coin_files)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/coin" FILES ${coin_files})
        add_custom_target(${coin_target_name} SOURCES ${coin_files})
    endif()

    # config.test
    set(config_tests_target_name ${bobui_repo_targets_name}_config_tests)
    file(GLOB_RECURSE config_tests_file LIST_DIRECTORIES false FOLLOW_SYMLINKS config.tests/*)
    if(config_tests_file)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/config.tests" FILES ${config_tests_file})
        add_custom_target(${config_tests_target_name} SOURCES ${config_tests_file})
    endif()

    # cmake
    set(cmake_target_name ${bobui_repo_targets_name}_cmake_files)
    file(GLOB_RECURSE cmake_files LIST_DIRECTORIES false FOLLOW_SYMLINKS
        cmake/*
        configure.cmake
        bobui_cmdline.cmake
        .cmake.conf
        *.cmake
        *.cmake.in)
    foreach(cmake_file IN LISTS cmake_files)
        if(NOT ((cmake_file IN_LIST coin_files) OR (file IN_LIST config_tests_files)))
            list(APPEND cmake_target_files ${cmake_file})
        endif()
    endforeach()

    if(cmake_target_files)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${cmake_target_files})
        add_custom_target(${cmake_target_name} SOURCES ${cmake_target_files})
    endif()

    # licenses
    set(licenses_target_name ${bobui_repo_targets_name}_licenses)
    file(GLOB licenses_files LIST_DIRECTORIES false LICENSES/*)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/licenseRule.json")
        list(APPEND licenses_files "${CMAKE_CURRENT_SOURCE_DIR}/licenseRule.json")
    endif()
    if(licenses_files)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${licenses_files})
        add_custom_target(${licenses_target_name} SOURCES ${licenses_files})
    endif()

    # changelogs
    set(changelogs_target_name ${bobui_repo_targets_name}_changelogs)
    file(GLOB change_logs_files LIST_DIRECTORIES false dist/changes-*)
    if(change_logs_files)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/dist" FILES ${change_logs_files})
        add_custom_target(${changelogs_target_name} SOURCES ${change_logs_files})
    endif()

    # extra files
    set(target_name ${bobui_repo_targets_name}_extra_files)
    add_custom_target(${target_name})

    set(recursive_glob_patterns
        REUSE.toml
        ${BOBUI_BUILD_EXTRA_IDE_FILE_RECURSIVE_PATTERNS}
    )
    set(simple_glob_patterns
        .gitattributes
        .gitignore
        .tag
        config_help.txt
        ${BOBUI_BUILD_EXTRA_IDE_FILE_PATTERNS}
    )

    if(recursive_glob_patterns)
        file(GLOB_RECURSE files LIST_DIRECTORIES false FOLLOW_SYMLINKS ${recursive_glob_patterns})
        if(files)
            source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${files})
            target_sources(${target_name} PRIVATE ${files})
        endif()
    endif()

    file(GLOB files LIST_DIRECTORIES false ${simple_glob_patterns})
    if(files)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${files})
        target_sources(${target_name} PRIVATE ${files})
    endif()
endfunction()


# Function called either at the end of per-repo configuration, or at the end of configuration of
# a super build.
# At the moment it is called before examples are configured in a per-repo build. We might want
# to change that at some point if needed.
function(bobui_internal_bobui_configure_end)
    # If BobUI is configued via the configure script, remove the marker variable, so that any future
    # reconfigurations that are done by calling cmake directly don't trigger configure specific
    # logic.
    if(BOBUI_INTERNAL_CALLED_FROM_CONFIGURE)
        unset(BOBUI_INTERNAL_CALLED_FROM_CONFIGURE CACHE)
    endif()
endfunction()

macro(bobui_build_repo)
    bobui_build_repo_begin(${ARGN})

    bobui_build_repo_impl_find_package_tests()
    bobui_build_repo_impl_src()
    bobui_build_repo_impl_tools()

    bobui_build_repo_post_process()
    bobui_build_repo_impl_tests()

    bobui_build_repo_end()

    bobui_build_repo_impl_examples()
endmacro()

macro(bobui_build_repo_impl_find_package_tests)
    # If testing is enabled, try to find the bobuibase Test package.
    # Do this before adding src, because there might be test related conditions
    # in source.
    if(BOBUI_BUILD_TESTS AND NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        # When looking for the Test package, do it using the BobUI6 package version, in case if
        # PROJECT_VERSION is following a different versioning scheme.
        if(BobUI6_VERSION)
            set(_bobui_build_repo_impl_find_package_tests_version "${BobUI6_VERSION}")
        else()
            set(_bobui_build_repo_impl_find_package_tests_version "${PROJECT_VERSION}")
        endif()

        find_package(BobUI6
            "${_bobui_build_repo_impl_find_package_tests_version}"
            CONFIG REQUIRED COMPONENTS Test)
        unset(_bobui_build_repo_impl_find_package_tests_version)
    endif()
endmacro()

macro(bobui_build_repo_impl_src)
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt")
            add_subdirectory(src)
        endif()
    endif()
    if(BOBUI_FEATURE_lttng AND NOT TARGET LTTng::UST)
        bobui_find_package(LTTngUST PROVIDED_TARGETS LTTng::UST
                        MODULE_NAME global QMAKE_LIB lttng-ust)
    endif()
endmacro()

macro(bobui_build_repo_impl_tools)
    if(NOT BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/CMakeLists.txt")
            add_subdirectory(tools)
        endif()
    endif()
endmacro()

macro(bobui_build_repo_impl_tests)
    if((BOBUI_BUILD_TESTS OR BOBUI_BUILD_STANDALONE_TESTS)
            AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        if(BOBUI_BUILD_STANDALONE_EXAMPLES)
            message(FATAL_ERROR
                "Can't build both standalone tests and standalone examples at once.")
        endif()
        string(TOLOWER "${PROJECT_NAME}" __bobui_repo_project_name_lowercase)
        option(BOBUI_BUILD_TESTS_PROJECT_${__bobui_repo_project_name_lowercase}
            "Configure tests for project ${__bobui_repo_project_name_lowercase}" TRUE)

        if (BOBUI_BUILD_TESTS_PROJECT_${__bobui_repo_project_name_lowercase})
            add_subdirectory(tests)
            if(NOT BOBUI_BUILD_TESTS_BY_DEFAULT)
                set_property(DIRECTORY tests PROPERTY EXCLUDE_FROM_ALL TRUE)
            endif()
        endif()
        unset(__bobui_repo_project_name_lowercase)
    endif()
endmacro()

macro(bobui_build_repo_impl_examples)
    if((BOBUI_BUILD_EXAMPLES OR BOBUI_BUILD_STANDALONE_EXAMPLES)
            AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
        if(BOBUI_BUILD_STANDALONE_TESTS)
            message(FATAL_ERROR
                "Can't build both standalone tests and standalone examples at once.")
        endif()

        message(STATUS "Configuring examples.")

        string(TOLOWER "${PROJECT_NAME}" __bobui_repo_project_name_lowercase)
        option(BOBUI_BUILD_EXAMPLES_PROJECT_${__bobui_repo_project_name_lowercase}
            "Configure examples for project ${__bobui_repo_project_name_lowercase}" TRUE)
        if(BOBUI_BUILD_EXAMPLES_PROJECT_${__bobui_repo_project_name_lowercase})

            # Set this before any examples subdirectories are added, to warn about examples that are
            # added via add_subdirectory() calls instead of bobui_internal_add_example().
            if(BOBUI_FEATURE_developer_build
                    AND NOT BOBUI_NO_WARN_ABOUT_EXAMPLE_ADD_SUBDIRECTORY_WARNING)
                set(BOBUI_WARN_ABOUT_EXAMPLE_ADD_SUBDIRECTORY TRUE)
            endif()

            add_subdirectory(examples)
        endif()
        unset(__bobui_repo_project_name_lowercase)
    endif()
endmacro()

macro(bobui_set_up_standalone_tests_build)
    # Remove this macro once all usages of it have been removed.
    # Standalone tests are not handled via the main repo project and bobui_build_tests.
endmacro()

function(bobui_get_standalone_parts_config_files_path out_var)
    # TODO: Rename this to StandaloneParts in some future BobUI version, if it confuses people too
    # much. Currently not renamed, not to break distro installation scripts that might exclude
    # the files.
    set(dir_name "StandaloneTests")

    set(path_suffix "${INSTALL_CMAKEDIR}/${INSTALL_CMAKE_NAMESPACE}BuildInternals/${dir_name}")

    # Each repo's standalone parts might be configured with a unique CMAKE_STAGING_PREFIX,
    # different from any previous one, and it might not coincide with where the BuildInternals
    # config file is.
    if(BOBUI_WILL_INSTALL AND CMAKE_STAGING_PREFIX)
        bobui_path_join(path "${CMAKE_STAGING_PREFIX}" "${path_suffix}")
    else()
        bobui_path_join(path "${BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}" "${path_suffix}")
    endif()

    set("${out_var}" "${path}" PARENT_SCOPE)
endfunction()

function(bobui_internal_get_standalone_parts_config_file_name out_var)
    # When doing a "single repo target set" build (like in bobuiscxqml) ensure we use a unique tests
    # config file for each repo target set. Using the PROJECT_NAME only is not enough because
    # the same file will be overridden with different content on each repo set install.
    set(tests_config_file_name "${PROJECT_NAME}")

    if(BOBUI_BUILD_SINGLE_REPO_TARGET_SET)
        string(APPEND tests_config_file_name "RepoSet${BOBUI_BUILD_SINGLE_REPO_TARGET_SET}")
    endif()

    # TODO: Rename this to StandalonePartsConfig.cmake in some future BobUI version, if it confuses
    # people too much. Currently not renamed, not to break distro installation scripts that might
    # exclude # the files.
    string(APPEND tests_config_file_name "TestsConfig.cmake")

    set(${out_var} "${tests_config_file_name}" PARENT_SCOPE)
endfunction()

macro(bobui_internal_find_standalone_test_config_file)
    if(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        # Of course we always need the test module as well.
        # When looking for the Test package, do it using the BobUI6 package version, in case if
        # PROJECT_VERSION is following a different versioning scheme.
        if(BobUI6_VERSION)
            set(_bobui_build_tests_package_version "${BobUI6_VERSION}")
        else()
            set(_bobui_build_tests_package_version "${PROJECT_VERSION}")
        endif()
        find_package(BobUI6 "${_bobui_build_tests_package_version}" CONFIG REQUIRED COMPONENTS Test)
        unset(_bobui_build_tests_package_version)
    endif()
endmacro()

# Used inside the standalone parts config file to find all requested BobUI module packages.
# standalone_parts_args_var_name should be the var name in the outer scope that contains
# all the arguments for this function.
macro(bobui_internal_find_standalone_parts_bobui_packages standalone_parts_args_var_name)
    set(__standalone_parts_opt_args "")
    set(__standalone_parts_single_args "")
    set(__standalone_parts_multi_args
        BOBUI_MODULE_PACKAGES
        BOBUI_TOOL_PACKAGES
    )
    cmake_parse_arguments(__standalone_parts
        "${__standalone_parts_opt_args}"
        "${__standalone_parts_single_args}"
        "${__standalone_parts_multi_args}"
        ${${standalone_parts_args_var_name}})

    # Packages looked up in standalone tests Config files should use the same version as
    # the one recorded on the Platform target.
    bobui_internal_get_package_version_of_target(Platform __standalone_parts_main_bobui_package_version)

    if(__standalone_parts_BOBUI_TOOL_PACKAGES)
        # Set up BOBUI_HOST_PATH as an extra root path to look for the Tools packages when
        # cross-compiling.
        if(NOT "${BOBUI_HOST_PATH}" STREQUAL "")
             set(__standalone_parts_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
             set(__standalone_parts_CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH})
             list(PREPEND CMAKE_PREFIX_PATH "${BOBUI_HOST_PATH_CMAKE_DIR}")
             list(PREPEND CMAKE_FIND_ROOT_PATH "${BOBUI_HOST_PATH}")
        endif()

        foreach(__standalone_parts_package_name IN LISTS __standalone_parts_BOBUI_TOOL_PACKAGES)
            find_package(${BOBUI_CMAKE_EXPORT_NAMESPACE}${__standalone_parts_package_name}
                "${__standalone_parts_main_bobui_package_version}"
                PATHS
                        # These come from BobUI6Config.cmake
                        ${_bobui_additional_packages_prefix_path}
                        ${_bobui_additional_packages_prefix_path_env}
            )
        endforeach()

        if(NOT "${BOBUI_HOST_PATH}" STREQUAL "")
             set(CMAKE_PREFIX_PATH ${__standalone_parts_CMAKE_PREFIX_PATH})
             set(CMAKE_FIND_ROOT_PATH ${__standalone_parts_CMAKE_FIND_ROOT_PATH})
        endif()
    endif()

    if(__standalone_parts_BOBUI_MODULE_PACKAGES)
        foreach(__standalone_parts_package_name IN LISTS __standalone_parts_BOBUI_MODULE_PACKAGES)
            find_package(${BOBUI_CMAKE_EXPORT_NAMESPACE}
                "${__standalone_parts_main_bobui_package_version}"
                COMPONENTS "${__standalone_parts_package_name}")
        endforeach()
    endif()

    unset(__standalone_parts_opt_args)
    unset(__standalone_parts_single_args)
    unset(__standalone_parts_multi_args)
    unset(__standalone_parts_BOBUI_MODULE_PACKAGES)
    unset(__standalone_parts_BOBUI_TOOL_PACKAGES)
    unset(__standalone_parts_main_bobui_package_version)
    unset(__standalone_parts_package_name)
    unset(__standalone_parts_CMAKE_PREFIX_PATH)
    unset(__standalone_parts_CMAKE_FIND_ROOT_PATH)
endmacro()

# Used by standalone tests and standalone non-ExternalProject examples to find all installed bobui
# packages.
macro(bobui_internal_find_standalone_parts_config_files)
    if(BOBUI_INTERNAL_BUILD_STANDALONE_PARTS)
        # Find location of TestsConfig.cmake. These contain the modules that need to be
        # find_package'd when building tests or examples.
        bobui_get_standalone_parts_config_files_path(_bobui_build_parts_install_prefix)

        bobui_internal_get_standalone_parts_config_file_name(_bobui_parts_config_file_name)
        set(_bobui_standalone_parts_config_file_path
            "${_bobui_build_parts_install_prefix}/${_bobui_parts_config_file_name}")
        include("${_bobui_standalone_parts_config_file_path}"
            OPTIONAL
            RESULT_VARIABLE _bobui_standalone_parts_included)
        if(NOT _bobui_standalone_parts_included)
            message(DEBUG
                "Standalone parts config file not included because it does not exist: "
                "${_bobui_standalone_parts_config_file_path}"
            )
        else()
            message(DEBUG
                "Standalone parts config file included successfully: "
                "${_bobui_standalone_parts_config_file_path}"
            )
        endif()

        unset(_bobui_standalone_parts_config_file_path)
        unset(_bobui_standalone_parts_included)
        unset(_bobui_parts_config_file_name)
    endif()
endmacro()

macro(bobui_build_tests)
    # Indicates that we are configuring tests now
    set(BOBUI_INTERNAL_CONFIGURING_TESTS TRUE)

    # Set this as a directory scoped variable, so we can easily check the variable in child
    # directories, to prevent certain code from running, like sbom file checks for all targets
    # created in tests subdir.
    if(NOT BOBUI_BUILD_TESTS_BY_DEFAULT)
        set(BOBUI_INTERNAL_TEST_TARGETS_EXCLUDE_FROM_ALL TRUE)
    endif()

    # Tests are not unity-ready.
    set(CMAKE_UNITY_BUILD OFF)

    bobui_internal_sbom_disable_sbom_for_tests_subdir()

    # Prepending to BOBUI_BUILD_CMAKE_PREFIX_PATH helps find components of BobUI6, because those
    # find_package calls use NO_DEFAULT_PATH, and thus CMAKE_PREFIX_PATH is ignored.
    list(PREPEND CMAKE_FIND_ROOT_PATH "${BOBUI_BUILD_DIR}")
    list(PREPEND BOBUI_BUILD_CMAKE_PREFIX_PATH "${BOBUI_BUILD_DIR}/${INSTALL_CMAKEDIR}")

    bobui_internal_find_standalone_parts_config_files()
    bobui_internal_find_standalone_test_config_file()

    if(BOBUI_BUILD_STANDALONE_TESTS)
        # Set language standards after finding Core, because that's when the relevant
        # feature variables are available, and the call in BobUISetup is too early when building
        # standalone tests, because Core was not find_package()'d yet.
        bobui_set_language_standards()

        # Set up fake standalone parts install prefix, so we don't pollute the BobUI install
        # prefix with tests.
        bobui_internal_set_up_fake_standalone_parts_install_prefix()
    else()
        if(ANDROID)
            # When building in-tree tests we need to specify the BOBUI_ANDROID_ABIS list. Since we
            # build BobUI for the single ABI, build tests for this ABI only.
            set(BOBUI_ANDROID_ABIS "${CMAKE_ANDROID_ARCH_ABI}")
        endif()
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/auto/CMakeLists.txt")
        add_subdirectory(auto)
    endif()
    if(NOT BOBUI_BUILD_MINIMAL_STATIC_TESTS AND NOT BOBUI_BUILD_MINIMAL_ANDROID_MULTI_ABI_TESTS)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/baseline/CMakeLists.txt")
            add_subdirectory(baseline)
        endif()
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/CMakeLists.txt" AND BOBUI_BUILD_BENCHMARKS)
            add_subdirectory(benchmarks)
        endif()
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/manual/CMakeLists.txt" AND BOBUI_BUILD_MANUAL_TESTS)
            add_subdirectory(manual)
            # Adding this logic to all tests impacts the configure time ~3sec in addition. We still
            # might want this in the future for other test types since currently we have a moderate
            # subset of tests that require manual initialization of autotools.
            _bobui_internal_collect_buildsystem_targets(targets
                "${CMAKE_CURRENT_SOURCE_DIR}/manual" EXCLUDE UTILITY ALIAS)
            foreach(target ${targets})
                bobui_autogen_tools(${target} ENABLE_AUTOGEN_TOOLS "moc" "rcc")
                if(TARGET BobUI::Widgets)
                    bobui_autogen_tools(${target} ENABLE_AUTOGEN_TOOLS "uic")
                endif()
            endforeach()
        endif()
    endif()

    bobui_internal_add_default_tests()

    if(NOT BOBUI_SUPERBUILD)
        # In a super build, we don't want to finalize the batch blacklist at the end of each repo,
        # but rather once at the end of the top-level configuration.
        bobui_internal_finalize_test_batch_blacklist()
    endif()

    set(CMAKE_UNITY_BUILD ${BOBUI_UNITY_BUILD})
    unset(BOBUI_INTERNAL_CONFIGURING_TESTS)
endmacro()

function(bobui_compute_relative_path_from_cmake_config_dir_to_prefix)
    # Compute the reverse relative path from the CMake config dir to the install prefix.
    # This is used in BobUIBuildInternalsExtras to create a relocatable relative install prefix path.
    # This path is used for finding syncbobui and other things, regardless of initial install prefix
    # (e.g installed BobUI was archived and unpacked to a different path on a different machine).
    #
    # This is meant to be called only once when configuring bobuibase.
    #
    # Similar code exists in BobUI6CoreConfigExtras.cmake.in and src/corelib/CMakeLists.txt which
    # might not be needed anymore.
    if(CMAKE_STAGING_PREFIX)
        set(__bobui_prefix "${CMAKE_STAGING_PREFIX}")
    else()
        set(__bobui_prefix "${CMAKE_INSTALL_PREFIX}")
    endif()

    if(BOBUI_WILL_INSTALL)
        get_filename_component(clean_config_prefix
                               "${__bobui_prefix}/${BOBUI_CONFIG_INSTALL_DIR}" ABSOLUTE)
    else()
        get_filename_component(clean_config_prefix "${BOBUI_CONFIG_BUILD_DIR}" ABSOLUTE)
    endif()
    file(RELATIVE_PATH
         bobui_path_from_cmake_config_dir_to_prefix
         "${clean_config_prefix}" "${__bobui_prefix}")
     set(bobui_path_from_cmake_config_dir_to_prefix "${bobui_path_from_cmake_config_dir_to_prefix}"
         PARENT_SCOPE)
endfunction()

function(bobui_get_relocatable_install_prefix out_var)
    # We need to compute it only once while building bobuibase. Afterwards it's loaded from
    # BobUIBuildInternalsExtras.cmake.
    if(BOBUI_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX)
        return()
    endif()
    # The BobUIBuildInternalsExtras value is dynamically computed, whereas the initial bobuibase
    # configuration uses an absolute path.
    set(${out_var} "${CMAKE_INSTALL_PREFIX}" PARENT_SCOPE)
endfunction()

function(bobui_internal_get_fake_standalone_install_prefix out_var)
    set(new_install_prefix "${CMAKE_BINARY_DIR}/fake_prefix")
    set(${out_var} "${new_install_prefix}" PARENT_SCOPE)
endfunction()

function(bobui_internal_set_up_fake_standalone_parts_install_prefix)
    # Set a fake local (non-cache) CMAKE_INSTALL_PREFIX.
    # Needed for standalone tests, we don't want to accidentally install a test into the BobUI prefix.
    # Allow opt-out, if a user knows what they're doing.
    if(BOBUI_NO_FAKE_STANDALONE_TESTS_INSTALL_PREFIX)
        return()
    endif()
    bobui_internal_get_fake_standalone_install_prefix(new_install_prefix)

    # It's IMPORTANT that this is not a cache variable. Otherwise
    # bobui_get_standalone_parts_config_files_path() will not work on re-configuration.
    message(STATUS
            "Setting local standalone test install prefix (non-cached) to '${new_install_prefix}'.")
    set(CMAKE_INSTALL_PREFIX "${new_install_prefix}" PARENT_SCOPE)

    # We also need to clear the staging prefix if it's set, otherwise CMake will modify any computed
    # rpaths containing the staging prefix to point to the new fake prefix, which is not what we
    # want. This replacement is done in cmComputeLinkInformation::GetRPath().
    #
    # By clearing the staging prefix for the standalone tests, any detected link time
    # rpaths will be embedded as-is, which will point to the place where BobUI was installed (aka
    # the staging prefix).
    if(DEFINED CMAKE_STAGING_PREFIX)
        message(STATUS "Clearing local standalone test staging prefix (non-cached).")
        set(CMAKE_STAGING_PREFIX "" PARENT_SCOPE)
    endif()
endfunction()

# Meant to be called when configuring examples as part of the main build tree (unless standalone
# examples are being built), as well as for CMake tests (tests that call CMake to try and build
# CMake applications).
macro(bobui_internal_set_up_build_dir_package_paths)
    list(PREPEND CMAKE_PREFIX_PATH "${BOBUI_BUILD_DIR}/${INSTALL_CMAKEDIR}")

    # Make sure the CMake config files do not recreate the already-existing targets.
    if(NOT BOBUI_BUILD_STANDALONE_EXAMPLES)
        set(BOBUI_NO_CREATE_TARGETS TRUE)
    endif()
endmacro()

function(bobui_internal_static_link_order_test)
    # The CMake versions greater than 3.21 take care about the resource object files order in a
    # linker line, it's expected that all object files are located at the beginning of the linker
    # line.
    # No need to run the test.
    if(CMAKE_VERSION VERSION_LESS 3.21)
        __bobui_internal_check_link_order_matters(link_order_matters)
        if(link_order_matters)
            set(summary_message "no")
        else()
            set(summary_message "yes")
        endif()
    else()
        set(summary_message "yes")
    endif()
    bobui_configure_add_summary_entry(TYPE "message"
        ARGS "Linker can resolve circular dependencies"
        MESSAGE "${summary_message}"
    )
endfunction()

function(bobui_internal_check_cmp0099_available)
    # Don't care about CMP0099 in CMake versions greater than or equal to 3.21
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
        return()
    endif()

    __bobui_internal_check_cmp0099_available(result)
    if(result)
        set(summary_message "yes")
    else()
        set(summary_message "no")
    endif()
    bobui_configure_add_summary_entry(TYPE "message"
        ARGS "CMake policy CMP0099 is supported"
        MESSAGE "${summary_message}"
    )
endfunction()

function(bobui_internal_run_common_config_tests)
    bobui_configure_add_summary_section(NAME "Common build options")
    bobui_internal_static_link_order_test()
    bobui_internal_check_cmp0099_available()
    bobui_configure_end_summary_section()
endfunction()

# It is used in BobUIWebEngine to replace the REALPATH with ABSOLUTE path, which is
# useful for building BobUI in Homebrew.
function(bobui_internal_get_filename_path_mode out_var)
    set(mode REALPATH)
    if(APPLE AND BOBUI_ALLOW_SYMLINK_IN_PATHS)
        set(mode ABSOLUTE)
    endif()
    set(${out_var} ${mode} PARENT_SCOPE)
endfunction()

macro(bobui_internal_setup_platform_support_variables)
    # Define some constants to check for certain platforms, etc.
    # Needs to be loaded before bobui_repo_build() to handle require() clauses before even starting a
    # repo build.
    include(BobUIPlatformSupport)
endmacro()

function(bobui_build_internals_set_up_system_prefixes)
    if(APPLE AND NOT FEATURE_pkg_config)
        # Remove /usr/local and other paths like that which CMake considers as system prefixes on
        # darwin platforms. CMake considers them as system prefixes, but in qmake / BobUI land we only
        # consider the SDK path as a system prefix.
        # 3rd party libraries in these locations should not be picked up when building BobUI,
        # unless opted-in via the pkg-config feature, which in turn will disable this behavior.
        #
        # Note that we can't remove /usr as a system prefix path, because many programs won't be
        # found then (e.g. perl).
        set(BOBUI_CMAKE_SYSTEM_PREFIX_PATH_BACKUP "${CMAKE_SYSTEM_PREFIX_PATH}" PARENT_SCOPE)
        set(BOBUI_CMAKE_SYSTEM_FRAMEWORK_PATH_BACKUP "${CMAKE_SYSTEM_FRAMEWORK_PATH}" PARENT_SCOPE)

        list(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH
            "/usr/local" # Homebrew
            "/opt/homebrew" # Apple Silicon Homebrew
            "/usr/X11R6"
            "/usr/pkg"
            "/opt"
            "/sw" # Fink
            "/opt/local" # MacPorts
        )
        if(_CMAKE_INSTALL_DIR)
            list(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH "${_CMAKE_INSTALL_DIR}")
        endif()
        list(REMOVE_ITEM CMAKE_SYSTEM_FRAMEWORK_PATH "~/Library/Frameworks")
        set(CMAKE_SYSTEM_PREFIX_PATH "${CMAKE_SYSTEM_PREFIX_PATH}" PARENT_SCOPE)
        set(CMAKE_SYSTEM_FRAMEWORK_PATH "${CMAKE_SYSTEM_FRAMEWORK_PATH}" PARENT_SCOPE)

        # Also tell bobui_find_package() not to use PATH when looking for packages.
        # We can't simply set CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH to OFF because that will break
        # find_program(), and for instance ccache won't be found.
        # That's why we set a different variable which is used by bobui_find_package.
        set(BOBUI_NO_USE_FIND_PACKAGE_SYSTEM_ENVIRONMENT_PATH "ON" PARENT_SCOPE)
    endif()
endfunction()

function(bobui_build_internals_disable_pkg_config_if_needed)
    # pkg-config should not be used by default on Darwin and Windows platforms (and QNX), as defined
    # in the bobuibase/configure.json. Unfortunately by the time the feature is evaluated there are
    # already a few find_package() calls that try to use the FindPkgConfig module.
    # Thus, we have to duplicate the condition logic here and disable pkg-config for those platforms
    # by default.
    # We also need to check if the pkg-config executable exists, to mirror the condition test in
    # configure.json. We do that by trying to find the executable ourselves, and not delegating to
    # the FindPkgConfig module because that has more unwanted side-effects.
    #
    # Note that on macOS, if the pkg-config feature is enabled by the user explicitly, we will also
    # tell CMake to consider paths like /usr/local (Homebrew) as system paths when looking for
    # packages.
    # We have to do that because disabling these paths but keeping pkg-config
    # enabled won't enable finding all system libraries via pkg-config alone, many libraries can
    # only be found via FooConfig.cmake files which means /usr/local should be in the system prefix
    # path.

    set(pkg_config_enabled ON)
    bobui_build_internals_find_pkg_config_executable()

    if(APPLE OR WIN32 OR QNX OR ANDROID OR WASM OR (NOT PKG_CONFIG_EXECUTABLE))
        set(pkg_config_enabled OFF)
    endif()

    # If user explicitly specified a value for the feature, honor it, even if it might break
    # the build.
    if(DEFINED FEATURE_pkg_config)
        if(FEATURE_pkg_config)
            set(pkg_config_enabled ON)
        else()
            set(pkg_config_enabled OFF)
        endif()
    endif()

    set(FEATURE_pkg_config "${pkg_config_enabled}" CACHE STRING "Using pkg-config")
    if(NOT pkg_config_enabled)
        bobui_build_internals_disable_pkg_config()
    else()
        unset(PKG_CONFIG_EXECUTABLE CACHE)
    endif()
endfunction()

# This is a copy of the first few lines in FindPkgConfig.cmake.
function(bobui_build_internals_find_pkg_config_executable)
    # find pkg-config, use PKG_CONFIG if set
    if((NOT PKG_CONFIG_EXECUTABLE) AND (NOT "$ENV{PKG_CONFIG}" STREQUAL ""))
      set(PKG_CONFIG_EXECUTABLE "$ENV{PKG_CONFIG}" CACHE FILEPATH "pkg-config executable")
    endif()
    find_program(PKG_CONFIG_EXECUTABLE NAMES pkg-config DOC "pkg-config executable")
    mark_as_advanced(PKG_CONFIG_EXECUTABLE)
endfunction()

function(bobui_build_internals_disable_pkg_config)
    # Disable pkg-config by setting an empty executable path. There's no documented way to
    # mark the package as not found, but we can force all pkg_check_modules calls to do nothing
    # by setting the variable to an empty value.
    set(PKG_CONFIG_EXECUTABLE "" CACHE STRING "Disabled pkg-config usage." FORCE)
endfunction()

macro(bobui_build_internals_find_pkg_config)
    # Find package config once before any system prefix modifications.
    find_package(PkgConfig QUIET)
endmacro()


macro(bobui_internal_setup_pkg_config_and_system_prefixes)
    if(NOT BOBUI_BUILD_INTERNALS_SKIP_PKG_CONFIG_ADJUSTMENT)
        bobui_build_internals_disable_pkg_config_if_needed()
    endif()

    if(NOT BOBUI_BUILD_INTERNALS_SKIP_FIND_PKG_CONFIG)
        bobui_build_internals_find_pkg_config()
    endif()

    if(NOT BOBUI_BUILD_INTERNALS_SKIP_SYSTEM_PREFIX_ADJUSTMENT)
        bobui_build_internals_set_up_system_prefixes()
    endif()
endmacro()

macro(bobui_internal_setup_standalone_test_when_called_as_a_find_package_component)
    if ("STANDALONE_TEST" IN_LIST BobUI6BuildInternals_FIND_COMPONENTS)
        include(${CMAKE_CURRENT_LIST_DIR}/BobUIStandaloneTestTemplateProject/Main.cmake)
        if (NOT PROJECT_VERSION_MAJOR)
            get_property(_bobui_major_version TARGET ${BOBUI_CMAKE_EXPORT_NAMESPACE}::Core PROPERTY INTERFACE_BOBUI_MAJOR_VERSION)
            set(PROJECT_VERSION ${BobUI${_bobui_major_version}Core_VERSION})

            string(REPLACE "." ";" _bobui_core_version_list ${PROJECT_VERSION})
            list(GET _bobui_core_version_list 0 PROJECT_VERSION_MAJOR)
            list(GET _bobui_core_version_list 1 PROJECT_VERSION_MINOR)
            list(GET _bobui_core_version_list 2 PROJECT_VERSION_PATCH)
        endif()
    endif()
endmacro()

macro(bobui_internal_setup_build_internals)
    bobui_internal_set_bobui_repo_dependencies()
    bobui_internal_setup_platform_support_variables()
    bobui_internal_setup_pkg_config_and_system_prefixes()
    bobui_internal_setup_standalone_test_when_called_as_a_find_package_component()
endmacro()

# Recursively reads the dependencies section from dependencies.yaml in ${repo_dir} and returns the
# list of dependencies, including transitive ones, in out_var.
#
# The returned dependencies are topologically sorted.
#
# Example output for bobuideclarative:
# bobuibase;bobuiimageformats;bobuilanguageserver;bobuishadertools;bobuisvg
#
function(bobui_internal_read_repo_dependencies out_var repo_dir)
    set(seen ${ARGN})
    set(dependencies "")
    set(in_dependencies_section FALSE)
    set(dependencies_file "${repo_dir}/dependencies.yaml")
    if(EXISTS "${dependencies_file}")
        file(STRINGS "${dependencies_file}" lines)
        foreach(line IN LISTS lines)
            if(line MATCHES "^([^ ]+):")
                if(CMAKE_MATCH_1 STREQUAL "dependencies")
                    set(in_dependencies_section TRUE)
                else()
                    set(in_dependencies_section FALSE)
                endif()
            elseif(in_dependencies_section AND line MATCHES "^  (.+):$")
                set(dependency "${CMAKE_MATCH_1}")
                set(dependency_repo_dir "${repo_dir}/${dependency}")
                string(REGEX MATCH "[^/]+$" dependency "${dependency}")
                if(NOT dependency IN_LIST seen)
                    bobui_internal_read_repo_dependencies(subdeps "${dependency_repo_dir}"
                        ${seen} ${dependency})
                    if(dependency MATCHES "^tbobuic-(.+)")
                        set(dependency "${CMAKE_MATCH_1}")
                    endif()
                    list(APPEND dependencies ${subdeps} ${dependency})
                endif()
            endif()
        endforeach()
        list(REMOVE_DUPLICATES dependencies)
    endif()
    set(${out_var} "${dependencies}" PARENT_SCOPE)
endfunction()

macro(bobui_internal_set_bobui_repo_dependencies)
    # The top-level check needs to happen because it's possible
    # to configure a top-level build with a few repos and then configure another repo
    # using bobui-configure-module in a separate build dir, where BOBUI_SUPERBUILD will not
    # be set anymore.
    if(DEFINED BOBUI_REPO_MODULE_VERSION AND NOT DEFINED BOBUI_REPO_DEPENDENCIES AND NOT BOBUI_SUPERBUILD)
        bobui_internal_read_repo_dependencies(BOBUI_REPO_DEPENDENCIES "${PROJECT_SOURCE_DIR}")
    endif()
endmacro()
