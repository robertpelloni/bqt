# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(mochelpers)

# BOBUIBUG-88538
if(NOT ANDROID AND NOT IOS)
    add_subdirectory(qmakelib)
    if(BOBUI_FEATURE_qmake)
        add_subdirectory(qmake)
    endif()
    add_subdirectory(moc)
    add_subdirectory(rcc)
    add_subdirectory(bobui_cmake_create)
endif()
# BOBUIBUG-88538
if(TARGET BobUI::Widgets AND NOT ANDROID AND NOT IOS)
    add_subdirectory(uic)
endif()
if(run_dbus_tests)
    add_subdirectory(qdbuscpp2xml)
    add_subdirectory(qdbusxml2cpp)
endif()
if(TARGET BobUI::Gui AND BOBUI_FEATURE_process AND NOT CMAKE_CROSSCOMPILING)
    # testapp (windeploybobui) and source_basicapp (macdeploybobui) require BobUIGui.
    if(BOBUI_FEATURE_macdeploybobui)
        add_subdirectory(macdeploybobui)
    endif()
    if(BOBUI_FEATURE_windeploybobui AND BUILD_SHARED_LIBS)
        # windeploybobui does not work with static BobUI builds. See BOBUIBUG-69427.
        add_subdirectory(windeploybobui)
    endif()
endif()
