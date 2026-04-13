# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(drawtexture)
add_subdirectory(qcolor)
add_subdirectory(qregion)
add_subdirectory(bobuiransform)
add_subdirectory(lancebench)
if(TARGET BobUI::Widgets)
    add_subdirectory(qpainter)
    add_subdirectory(bobuibench)
endif()
