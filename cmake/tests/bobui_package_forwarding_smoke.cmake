set(_bobui_repo_root "${CMAKE_CURRENT_LIST_DIR}/../..")
include("${_bobui_repo_root}/cmake/BobUISupportedComponents.cmake")

set(_bobui_smoke_root "${CMAKE_CURRENT_LIST_DIR}/_bobui_package_forwarding_smoke")
set(_bobui_prefix "${_bobui_smoke_root}/prefix")
set(_bobui_cmake_root "${_bobui_prefix}/lib/cmake")
set(_bobui_source_dir "${_bobui_smoke_root}/src")
set(_bobui_build_dir "${_bobui_smoke_root}/build")

file(REMOVE_RECURSE "${_bobui_smoke_root}")
file(MAKE_DIRECTORY "${_bobui_cmake_root}/Qt6" "${_bobui_source_dir}")

# Create fake Qt6 packages
file(WRITE "${_bobui_cmake_root}/Qt6/Qt6Config.cmake" [=[
set(Qt6_FOUND TRUE)
set(Qt6_VERSION "6.12.0")
set(Qt6_VERSION_MAJOR "6")
set(Qt6_VERSION_MINOR "12")
set(Qt6_VERSION_PATCH "0")
foreach(_qt6_component IN LISTS Qt6_FIND_COMPONENTS)
    find_package(Qt6${_qt6_component} REQUIRED CONFIG
        PATHS "${CMAKE_CURRENT_LIST_DIR}/.." NO_DEFAULT_PATH)
endforeach()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6/Qt6ConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])

foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    set(_qt_pkg "Qt6${_component}")
    file(MAKE_DIRECTORY "${_bobui_cmake_root}/${_qt_pkg}")
    file(WRITE "${_bobui_cmake_root}/${_qt_pkg}/${_qt_pkg}Config.cmake" "
set(${_qt_pkg}_FOUND TRUE)
set(${_qt_pkg}_VERSION \"6.12.0\")
if(NOT TARGET Qt6::${_component})
    add_library(Qt6::${_component} INTERFACE IMPORTED)
endif()
if(NOT TARGET Qt::${_component})
    add_library(Qt::${_component} INTERFACE IMPORTED)
    set_property(TARGET Qt::${_component} PROPERTY INTERFACE_LINK_LIBRARIES \"Qt6::${_component}\")
endif()
")
    file(WRITE "${_bobui_cmake_root}/${_qt_pkg}/${_qt_pkg}ConfigVersion.cmake" "
set(PACKAGE_VERSION \"6.12.0\")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
")
endforeach()

# Create BobUI shim packages in the fake prefix
foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    foreach(_prefix BobUI6 BobUI)
        set(_bobui_pkg "${_prefix}${_component}")
        file(MAKE_DIRECTORY "${_bobui_cmake_root}/${_bobui_pkg}")
        file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake"
             DESTINATION "${_bobui_cmake_root}/${_bobui_pkg}")
        file(COPY "${CMAKE_CURRENT_LIST_DIR}/../${_bobui_pkg}Config.cmake"
             DESTINATION "${_bobui_cmake_root}/${_bobui_pkg}")
        file(WRITE "${_bobui_cmake_root}/${_bobui_pkg}/${_bobui_pkg}ConfigVersion.cmake" "
set(PACKAGE_VERSION \"6.12.0\")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
")
    endforeach()
endforeach()

# Special handling for umbrella packages
foreach(_pkg BobUI6 BobUI)
    file(MAKE_DIRECTORY "${_bobui_cmake_root}/${_pkg}")
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake"
         DESTINATION "${_bobui_cmake_root}/${_pkg}")
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/../${_pkg}Config.cmake"
         DESTINATION "${_bobui_cmake_root}/${_pkg}")
    file(WRITE "${_bobui_cmake_root}/${_pkg}/${_pkg}ConfigVersion.cmake" "
set(PACKAGE_VERSION \"6.12.0\")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
")
endforeach()

# Create test project
set(_cmakelists "cmake_minimum_required(VERSION 3.16)\nproject(BobUIPackageForwardingSmoke LANGUAGES NONE)\n\n")
string(REPLACE ";" " " _components_str "${BOBUI_SUPPORTED_COMPONENTS}")
string(APPEND _cmakelists "find_package(BobUI6 6.12 REQUIRED CONFIG COMPONENTS ${_components_str})\n")
string(APPEND _cmakelists "find_package(BobUI REQUIRED CONFIG COMPONENTS ${_components_str})\n")

foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    string(APPEND _cmakelists "find_package(BobUI6${_component} 6.12 REQUIRED CONFIG)\n")
    string(APPEND _cmakelists "find_package(BobUI${_component} REQUIRED CONFIG)\n")
endforeach()

string(APPEND _cmakelists "\nforeach(target_name\n")
foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    string(APPEND _cmakelists "    BobUI6::${_component}\n")
    string(APPEND _cmakelists "    BobUI::${_component}\n")
endforeach()
string(APPEND _cmakelists ")\n    if(NOT TARGET \${target_name})\n        message(FATAL_ERROR \"Expected compatibility target \${target_name} to exist\")\n    endif()\nendforeach()\n")

file(WRITE "${_bobui_source_dir}/CMakeLists.txt" "${_cmakelists}")

execute_process(
    COMMAND "${CMAKE_COMMAND}"
        -S "${_bobui_source_dir}"
        -B "${_bobui_build_dir}"
        -DCMAKE_PREFIX_PATH=${_bobui_prefix}
    RESULT_VARIABLE _bobui_configure_result
    OUTPUT_VARIABLE _bobui_configure_stdout
    ERROR_VARIABLE _bobui_configure_stderr
)

if(NOT _bobui_configure_result EQUAL 0)
    message(FATAL_ERROR
        "BobUI package forwarding smoke test failed.\n"
        "stdout:\n${_bobui_configure_stdout}\n"
        "stderr:\n${_bobui_configure_stderr}")
endif()

file(REMOVE_RECURSE "${_bobui_smoke_root}")
message(STATUS "BobUI package forwarding smoke test passed")
