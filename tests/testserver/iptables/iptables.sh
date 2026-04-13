#!/usr/bin/env bash
# Copyright (C) 2019 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

iptables -A INPUT -p tcp --destination-port 1357 -j DROP
