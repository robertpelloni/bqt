# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# BOBUIBUG-87669
if(TARGET BobUI::Network AND NOT ANDROID)
    add_subdirectory(qimagereader)
endif()
if(BOBUI_FEATURE_ico)
    add_subdirectory(qicoimageformat)
endif()
add_subdirectory(qpixmap)
add_subdirectory(qimage)
add_subdirectory(qimageiohandler)
add_subdirectory(qimagewriter)
if(BOBUI_FEATURE_movie)
    add_subdirectory(qmovie)
endif()
if(BOBUI_FEATURE_picture)
    add_subdirectory(qpicture)
endif()
add_subdirectory(qiconhighdpi)
if(BOBUI_FEATURE_private_tests)
    add_subdirectory(qpixmapcache)
endif()

add_subdirectory(qicon)
