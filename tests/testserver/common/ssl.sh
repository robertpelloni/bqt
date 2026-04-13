#!/usr/bin/env bash
# Copyright (C) 2018 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

# package ssl

# install ssl_certs and test data
su $USER -c "mkdir -p -m 700 ~/ssl-certs/private"
su $USER -c \
    "cp $CONFIG/ssl/${test_cert:-bobui-test-server-cert.pem} ~/ssl-certs/bobui-test-server-cert.pem"
su $USER -c "cp $CONFIG/ssl/private/bobui-test-server-key.pem ~/ssl-certs/private/"
