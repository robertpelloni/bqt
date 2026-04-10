# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(bobuiconcurrentfilter)
add_subdirectory(bobuiconcurrentiteratekernel)
add_subdirectory(bobuiconcurrentfiltermapgenerated)
add_subdirectory(bobuiconcurrentmap)
add_subdirectory(bobuiconcurrentmedian)
if(NOT INTEGRITY)
    add_subdirectory(bobuiconcurrentrun)
    add_subdirectory(bobuiconcurrenttask)
endif()
add_subdirectory(bobuiconcurrentthreadengine)
