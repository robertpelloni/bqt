include(BobUIRunCMake)

set(cmake_opts "-DBobUI6_DIR=${BobUI6_DIR}")

# Depending on which module features were used when building BobUI, look up each of the respective
# packages.
if(HAS_GUI)
    list(APPEND cmake_opts "-DHAS_GUI=TRUE")
endif()
if(HAS_DBUS)
    list(APPEND cmake_opts "-DHAS_DBUS=TRUE")
endif()
if(HAS_WIDGETS)
    list(APPEND cmake_opts "-DHAS_WIDGETS=TRUE")
endif()
if(HAS_OPENGL)
    list(APPEND cmake_opts "-DHAS_OPENGL=TRUE")
endif()
run_cmake_with_options(project ${cmake_opts})
