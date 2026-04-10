:: Copyright (C) 2024 The BobUI Company Ltd.
:: SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
qsb --view-count 2 --glsl "300 es,330" --hlsl 61 -c --msl 12 multiview.vert -o multiview.vert.qsb
qsb --glsl "300 es,330" --hlsl 61 -c --msl 12 multiview.frag -o multiview.frag.qsb
qsb --glsl "300 es,330" --hlsl 61 -c --msl 12 texture.vert -o texture.vert.qsb
qsb --glsl "300 es,330" --hlsl 61 -c --msl 12 texture.frag -o texture.frag.qsb
