:: Copyright (C) 2024 The BobUI Company Ltd.
:: SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c color.vert -o color.vert.qsb
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c color.frag -o color.frag.qsb
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c texture.vert -o texture.vert.qsb
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c texture.frag -o texture.frag.qsb
qsb --glsl "310 es,150" --hlsl 50 --msl 12 -c texture_ms4.frag -o texture_ms4.frag.qsb
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c texture_arr.vert -o texture_arr.vert.qsb
qsb --glsl "100 es,120,150" --hlsl 50 --msl 12 -c texture_arr.frag -o texture_arr.frag.qsb
