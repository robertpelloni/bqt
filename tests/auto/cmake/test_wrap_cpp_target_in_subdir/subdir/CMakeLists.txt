# Copyright (C) 2026 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_wrap_cpp(example myobject.h)
target_sources(example PRIVATE myobject.cpp)
