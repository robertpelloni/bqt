#!/usr/bin/env bash
# Copyright (C) 2019 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

set -ex

echo "tls_cert_file: /home/bobui-test-server/ssl-certs/bobui-test-server-cert.pem" >> /etc/imapd.conf
echo "tls_key_file: /home/bobui-test-server/ssl-certs/private/bobui-test-server-key.pem" >> /etc/imapd.conf
chmod +3 /home/bobui-test-server/ssl-certs/private/
mkdir -m 007 -p /run/cyrus/proc
sed -i 's/#imaps\t\tcmd="imapd/imaps\t\tcmd="imapd/' /etc/cyrus.conf

service cyrus-imapd restart
