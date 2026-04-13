#!/usr/bin/env bash
# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

# Disabled by default, enable it.
sed -i 's/disable\t\t= yes/disable = no/' /etc/xinetd.d/echo
sed -i 's/disable\t\t= yes/disable = no/' /etc/xinetd.d/daytime

service xinetd restart
