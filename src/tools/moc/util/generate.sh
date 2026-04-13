#!/bin/sh
# Copyright (C) 2016 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

qmake
make
cat licenseheader.cpp.in > ../keywords.cpp
cat licenseheader.cpp.in > ../ppkeywords.cpp
./generate_keywords >> ../keywords.cpp
./generate_keywords preprocessor >> ../ppkeywords.cpp
