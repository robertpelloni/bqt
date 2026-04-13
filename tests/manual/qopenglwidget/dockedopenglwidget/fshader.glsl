// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
uniform sampler2D texture;
varying vec2 v_texcoord;

void main()
{
    gl_FragColor = texture2D(texture, v_texcoord);
}

