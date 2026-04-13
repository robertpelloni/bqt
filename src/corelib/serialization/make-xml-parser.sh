#!/bin/sh
# Copyright (C) 2016 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
# BobUI-Security score:insignificant reason:build-tool-containing-no-compiled-source

me=$(dirname $0)
mkdir -p $me/out
(cd $me/out && ../../../../util/qlalr/qlalr --bobui --no-debug --no-lines ../qxmlstream.g)

for f in $me/out/*.h; do
    n=$(basename $f)
    cp $f $n
done

git diff .

