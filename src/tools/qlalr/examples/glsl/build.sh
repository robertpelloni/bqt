#!/bin/sh
# Copyright (C) 2016 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

${FLEX-flex} -oglsl-lex.incl glsl-lex.l
${QLALR-qlalr} glsl.g

qmake
make
