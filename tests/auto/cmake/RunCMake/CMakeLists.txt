# Add RunCMake tests using `add_RunCMake_test()`

bobui_internal_add_RunCMake_test(BobUIFlagHandlingHelpers)

bobui_internal_add_RunCMake_test(find_package)

set(extra_run_cmake_args "")
if(TARGET BobUI::Gui)
    list(APPEND extra_run_cmake_args "-DHAS_GUI=TRUE")
endif()
if(TARGET BobUI::DBus)
    list(APPEND extra_run_cmake_args "-DHAS_DBUS=TRUE")
endif()
if(TARGET BobUI::Widgets)
    list(APPEND extra_run_cmake_args "-DHAS_WIDGETS=TRUE")
endif()
if(TARGET BobUI::OpenGL)
    list(APPEND extra_run_cmake_args "-DHAS_OPENGL=TRUE")
endif()
bobui_internal_add_RunCMake_test(BobUI6DirConfiguration ${extra_run_cmake_args})

if(APPLE AND TARGET BobUI::Gui)
    bobui_internal_add_RunCMake_test(AppleFrameworkDeduplication)
endif()

if(TARGET BobUI6::Platform)
    get_target_property(sbom_spdx_id BobUI6::Platform _bobui_sbom_spdx_id)
    # If the id is present and non-empty, that means we built BobUI with sbom support.
    if(sbom_spdx_id)
        bobui_internal_add_RunCMake_test(Sbom ${extra_run_cmake_args})
    endif()
endif()

list(APPEND extra_run_cmake_args "-DBOBUI_WILL_INSTALL=${BOBUI_WILL_INSTALL}")
list(APPEND extra_run_cmake_args "-DBOBUI_REPO_MODULE_VERSION=${BOBUI_REPO_MODULE_VERSION}")
bobui_internal_add_RunCMake_test(StandaloneToolsPackage ${extra_run_cmake_args})
