# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(torrent LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Network Widgets)

bobui_standard_project_setup()

bobui_add_executable(torrent
    addtorrentdialog.cpp addtorrentdialog.h
    addtorrentform.ui
    bencodeparser.cpp bencodeparser.h
    connectionmanager.cpp connectionmanager.h
    filemanager.cpp filemanager.h
    main.cpp
    mainwindow.cpp mainwindow.h
    metainfo.cpp metainfo.h
    peerwireclient.cpp peerwireclient.h
    ratecontroller.cpp ratecontroller.h
    torrentclient.cpp torrentclient.h
    torrentserver.cpp torrentserver.h
    trackerclient.cpp trackerclient.h
)

set_target_properties(torrent PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(torrent PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Network
    BobUI6::Widgets
)

# Resources:
set(icons_resource_files
    "icons/1downarrow.svg"
    "icons/1uparrow.svg"
    "icons/bottom.svg"
    "icons/exit.svg"
    "icons/peertopeer.svg"
    "icons/player_pause.svg"
    "icons/player_play.svg"
    "icons/player_stop.svg"
    "icons/about.svg"
)

bobui_add_resources(torrent "icons"
    PREFIX
        "/"
    FILES
        ${icons_resource_files}
)

install(TARGETS torrent
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET torrent
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
