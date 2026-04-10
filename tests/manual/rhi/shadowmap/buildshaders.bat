:: Copyright (C) 2024 The BobUI Company Ltd.
:: SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
qsb --glsl "120,300 es" --hlsl 50 --msl 12 -c shadowmap.vert -o shadowmap.vert.qsb
qsb --glsl "120,300 es" --hlsl 50 --msl 12 -c shadowmap.frag -o shadowmap.frag.qsb
qsb --glsl "120,300 es" --hlsl 50 --msl 12 -c main.vert -o main.vert.qsb
qsb --glsl "120,300 es" --hlsl 50 --msl 12 -c main.frag -o main.frag.qsb
