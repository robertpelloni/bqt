// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [1]
const auto certs = QSslCertificate::fromPath("C:/ssl/certificate.*.pem",
                                             QSsl::Pem, QSslCertificate::Wildcard);
for (const QSslCertificate &cert : certs) {
    qDebug() << cert.issuerInfo(QSslCertificate::Organization);
}
//! [1]
