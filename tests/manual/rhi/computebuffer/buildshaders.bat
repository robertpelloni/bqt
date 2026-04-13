:: Copyright (C) 2024 The BobUI Company Ltd.
:: SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
qsb --glsl "310 es,430" --hlsl 50 --msl 12 buffer.comp -o buffer.comp.qsb
qsb --glsl "310 es,430" --hlsl 50 --msl 12 main.vert -o main.vert.qsb
qsb --glsl "310 es,430" --hlsl 50 --msl 12 main.frag -o main.frag.qsb
