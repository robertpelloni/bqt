set(_bobui_smoke_root "${CMAKE_CURRENT_LIST_DIR}/_bobui_package_forwarding_smoke")
set(_bobui_prefix "${_bobui_smoke_root}/prefix")
set(_bobui_cmake_root "${_bobui_prefix}/lib/cmake")
set(_bobui_source_dir "${_bobui_smoke_root}/src")
set(_bobui_build_dir "${_bobui_smoke_root}/build")

file(REMOVE_RECURSE "${_bobui_smoke_root}")
file(MAKE_DIRECTORY
    "${_bobui_cmake_root}/Qt6"
    "${_bobui_cmake_root}/Qt6Core"
    "${_bobui_cmake_root}/Qt6Gui"
    "${_bobui_cmake_root}/Qt6Network"
    "${_bobui_cmake_root}/Qt6Widgets"
    "${_bobui_cmake_root}/BobUI6"
    "${_bobui_cmake_root}/BobUI"
    "${_bobui_cmake_root}/BobUI6Core"
    "${_bobui_cmake_root}/BobUICore"
    "${_bobui_cmake_root}/BobUI6Gui"
    "${_bobui_cmake_root}/BobUIGui"
    "${_bobui_cmake_root}/BobUI6Network"
    "${_bobui_cmake_root}/BobUINetwork"
    "${_bobui_cmake_root}/BobUI6Widgets"
    "${_bobui_cmake_root}/BobUIWidgets"
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
set(Qt6Core_VERSION "6.12.0")
if(NOT TARGET Qt6::Core)
    add_library(Qt6::Core INTERFACE IMPORTED)
endif()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Core/Qt6CoreConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Gui/Qt6GuiConfig.cmake" [=[
set(Qt6Gui_FOUND TRUE)
set(Qt6Gui_VERSION "6.12.0")
if(NOT TARGET Qt6::Gui)
    add_library(Qt6::Gui INTERFACE IMPORTED)
endif()
if(NOT TARGET Qt::Gui)
    add_library(Qt::Gui INTERFACE IMPORTED)
    set_property(TARGET Qt::Gui PROPERTY INTERFACE_LINK_LIBRARIES "Qt6::Gui")
endif()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Gui/Qt6GuiConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Network/Qt6NetworkConfig.cmake" [=[
set(Qt6Network_FOUND TRUE)
set(Qt6Network_VERSION "6.12.0")
if(NOT TARGET Qt6::Network)
    add_library(Qt6::Network INTERFACE IMPORTED)
endif()
if(NOT TARGET Qt::Network)
    add_library(Qt::Network INTERFACE IMPORTED)
    set_property(TARGET Qt::Network PROPERTY INTERFACE_LINK_LIBRARIES "Qt6::Network")
endif()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Network/Qt6NetworkConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Widgets/Qt6WidgetsConfig.cmake" [=[
set(Qt6Widgets_FOUND TRUE)
set(Qt6Widgets_VERSION "6.12.0")
if(NOT TARGET Qt6::Widgets)
    add_library(Qt6::Widgets INTERFACE IMPORTED)
endif()
if(NOT TARGET Qt::Widgets)
    add_library(Qt::Widgets INTERFACE IMPORTED)
    set_property(TARGET Qt::Widgets PROPERTY INTERFACE_LINK_LIBRARIES "Qt6::Widgets")
endif()
]=])
file(WRITE "${_bobui_cmake_root}/Qt6Widgets/Qt6WidgetsConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])

foreach(_bobui_package_dir BobUI6 BobUI BobUI6Core BobUICore BobUI6Gui BobUIGui BobUI6Network BobUINetwork BobUI6Widgets BobUIWidgets)
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake"
        DESTINATION "${_bobui_cmake_root}/${_bobui_package_dir}")
endforeach()
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6Config.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUIConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6CoreConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6Core")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUICoreConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUICore")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6GuiConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6Gui")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUIGuiConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUIGui")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6NetworkConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6Network")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUINetworkConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUINetwork")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUI6WidgetsConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUI6Widgets")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../BobUIWidgetsConfig.cmake"
    DESTINATION "${_bobui_cmake_root}/BobUIWidgets")

foreach(_bobui_package_name BobUI6 BobUI BobUI6Core BobUICore BobUI6Gui BobUIGui BobUI6Network BobUINetwork BobUI6Widgets BobUIWidgets)
    file(WRITE "${_bobui_cmake_root}/${_bobui_package_name}/${_bobui_package_name}ConfigVersion.cmake" [=[
set(PACKAGE_VERSION "6.12.0")
set(PACKAGE_VERSION_COMPATIBLE TRUE)
]=])
endforeach()

file(WRITE "${_bobui_source_dir}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.16)
project(BobUIPackageForwardingSmoke LANGUAGES NONE)

find_package(BobUI6 6.12 REQUIRED CONFIG COMPONENTS Core Widgets)
find_package(BobUI REQUIRED CONFIG COMPONENTS Core Widgets)
find_package(BobUI6Core 6.12 REQUIRED CONFIG)
find_package(BobUICore REQUIRED CONFIG)
find_package(BobUI6Gui 6.12 REQUIRED CONFIG)
find_package(BobUIGui REQUIRED CONFIG)
find_package(BobUI6Network 6.12 REQUIRED CONFIG)
find_package(BobUINetwork REQUIRED CONFIG)
find_package(BobUI6Widgets 6.12 REQUIRED CONFIG)
find_package(BobUIWidgets REQUIRED CONFIG)

foreach(target_name
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Network
    BobUI6::Widgets
    BobUI::Core
    BobUI::Gui
    BobUI::Network
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

file(REMOVE_RECURSE "${_bobui_smoke_root}")
message(STATUS "BobUI package forwarding smoke test passed")
