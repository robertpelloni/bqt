// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
QSslConfiguration config = sslSocket.sslConfiguration();
config.setProtocol(QSsl::TlsV1_2);
sslSocket.setSslConfiguration(config);
//! [0]
