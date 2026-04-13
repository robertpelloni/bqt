# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qpainterpath)
add_subdirectory(qpainterpathstroker)
add_subdirectory(qcolor)
add_subdirectory(qbrush)
add_subdirectory(qregion)
add_subdirectory(qpagelayout)
add_subdirectory(qpageranges)
add_subdirectory(qpagesize)
add_subdirectory(qpainter)
add_subdirectory(qpainterstateguard)
if (BOBUI_FEATURE_pdf)
    add_subdirectory(qpdfwriter)
endif()
add_subdirectory(qpen)
add_subdirectory(qpaintengine)
add_subdirectory(bobuiransform)
add_subdirectory(qpolygon)

# BOBUIBUG-87669
if(NOT ANDROID)
    add_subdirectory(qcolorspace)
    add_subdirectory(qcolortransform)
endif()
if(BOBUI_FEATURE_private_tests)
    add_subdirectory(qpathclipper)
endif()
