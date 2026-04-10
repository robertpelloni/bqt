// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUIGui/QPixmap>
#include <BobUIGui/QImage>

class tst_QDataStreamPixmap : public QObject
{
Q_OBJECT

private slots:
    void stream_with_pixmap();
};

void tst_QDataStreamPixmap::stream_with_pixmap()
{
    // This is a QVariantMap with a 3x3 red QPixmap and two strings inside
    const QByteArray ba = QByteArray::fromBase64(
        "AAAAAwAAAAIAegAAAAoAAAAACgB0AGgAZQByAGUAAAACAHAAAABBAAAAAAGJUE5H"
        "DQoaCgAAAA1JSERSAAAAAwAAAAMIAgAAANlKIugAAAAJcEhZcwAADsQAAA7EAZUr"
        "DhsAAAAQSURBVAiZY/zPAAVMDJgsAB1bAQXZn5ieAAAAAElFTkSuQmCCAAAAAgBh"
        "AAAACgAAAAAKAGgAZQBsAGwAbw==");
    QImage dummy; // Needed to make sure bobuiGui is loaded

    BOBUIest::ignoreMessage(BobUIWarningMsg, "QPixmap::fromImageInPlace: "
                         "QPixmap cannot be created without a QGuiApplication");

    QVariantMap map;
    QDataStream d(ba);
    d.setVersion(QDataStream::BobUI_5_12);
    d >> map;

    QCOMPARE(map["a"].toString(), QString("hello"));
    // The pixmap is null because this is not a QGuiApplication:
    QCOMPARE(map["p"].value<QPixmap>(), QPixmap());
    QCOMPARE(map["z"].toString(), QString("there"));
}

BOBUIEST_GUILESS_MAIN(tst_QDataStreamPixmap)

#include "tst_qdatastream_core_pixmap.moc"
