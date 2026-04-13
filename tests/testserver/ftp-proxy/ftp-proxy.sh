#!/usr/bin/env bash
# Copyright (C) 2018 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

# package ftp-proxy

# install configurations and test data
sed -i -e 's/# AllowMagicUser\tno/AllowMagicUser\tyes/' \
    -e 's/# ForkLimit\t\t40/ForkLimit\t\t2000/' \
    /etc/proxy-suite/ftp-proxy.conf

# enable service with installed configurations
ftp-proxy -d
