#!/usr/bin/env bash
# Copyright (C) 2018 The BobUI Company Ltd
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex
$@
env BOBUI_WAYLAND_SHELL_INTEGRATION=wl-shell $@
env BOBUI_WAYLAND_SHELL_INTEGRATION=ivi-shell $@
