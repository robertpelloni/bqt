set(_bobui_smoke_root "${CMAKE_CURRENT_LIST_DIR}/_bobui_package_forwarding_smoke")
set(_bobui_prefix "${_bobui_smoke_root}/prefix")
set(_bobui_cmake_root "${_bobui_prefix}/lib/cmake")
set(_bobui_source_dir "${_bobui_smoke_root}/src")
set(_bobui_build_dir "${_bobui_smoke_root}/build")

file(REMOVE_RECURSE "${_bobui_smoke_root}")
file(MAKE_DIRECTORY
    "${_bobui_cmake_root}/Qt6"
    "${_bobui_cmake_root}/Qt6Core"
    "${_bobui_cmake_root}/Qt6Widgets"
    "${_bobui_cmake_root}/BobUI6"
    "${_bobui_cmake_root}/BobUI"
    "${_bobui_source_dir}"
)

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
if(PACKAGE_FIND_VERSION)
    if(PACKAGE_FIND_VERSION VERSION_GREATER PACKAGE_VERSION)
        set(PACKAGE_VERSION_COMPATIBLE FALSE)
    else()
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
        if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
            set(PACKAGE_VERSION_EXACT TRUE)
        endif()
    endif()
else()
    set(PACKAGE_VERSION_COMPATIBLE TRUE)
endif()
]=])

file(WRITE "${_bobui_cmake_root}/Qt6Core/Qt6CoreConfig.cmake" [=[
set(Qt6Core_FOUND TRUE)
if(NOT TARGET Qt6::Core)
    add_library(Qt6::Core INTERFACE IMPORTED)
endif()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Widgets/Qt6WidgetsConfig.cmake" [=[
set(Qt6Widgets_FOUND TRUE)
if(NOT TARGET Qt6::Widgets)
    add_library(Qt6::Widgets INTERFACE IMPORTED)
endif()
]=])

file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6Config.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUIConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI")

file(WRITE "${_bobui_cmake_root}/BobUI6/BobUI6ConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])
file(WRITE "${_bobui_cmake_root}/BobUI/BobUIConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])

file(WRITE "${_bobui_source_dir}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.16)
project(BobUIPackageForwardingSmoke LANGUAGES NONE)

find_package(BobUI6 6.12 REQUIRED CONFIG COMPONENTS Core Widgets)
find_package(BobUI REQUIRED CONFIG COMPONENTS Core Widgets)

foreach(target_name
    BobUI6::Core
    BobUI6::Widgets
    BobUI::Core
    BobUI::Widgets
)
    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "Expected compatibility target ${target_name} to exist")
    endif()
endforeach()
]=])

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

message(STATUS "BobUI package forwarding smoke test passed")
