#!/usr/bin/env bash
# Copyright (C) 2018 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

# export variables
export USER=bobui-test-server
export PASS=password
export CONFIG=service/testdata
export TESTDATA=service/testdata

# add users
useradd -m -s /bin/bash $USER; echo "$USER:$PASS" | chpasswd

# install configurations and test data
su $USER -c "cp $CONFIG/system/passwords ~/"

./startup.sh "$@"
