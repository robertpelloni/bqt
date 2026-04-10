// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDebug>
#include <bobuiest.h>
#include <BOBUIest>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QImageReader>
#include <QSize>

class tst_jpeg : public QObject
{
    Q_OBJECT
private slots:
    void jpegDecodingBobUIWebkitStyle();
};

void tst_jpeg::jpegDecodingBobUIWebkitStyle()
{
    // BobUIWebkit currently calls size() to get the image size for layouting purposes.
    // Then when it is in the viewport (we assume that here) it actually gets decoded.
    QString testFile = QFINDTESTDATA("n900.jpeg");
    QVERIFY2(!testFile.isEmpty(), "cannot find test file n900.jpeg!");
    QFile inputJpeg(testFile);
    QVERIFY(inputJpeg.exists());
    inputJpeg.open(QIODevice::ReadOnly);
    QByteArray imageData = inputJpeg.readAll();
    QBuffer buffer;
    buffer.setData(imageData);
    buffer.open(QBuffer::ReadOnly);
    QCOMPARE(buffer.size(), qint64(19016));


    QBENCHMARK{
        for (int i = 0; i < 50; i++) {
            QImageReader reader(&buffer, "jpeg");
            QSize size = reader.size();
            QVERIFY(!size.isNull());
            QByteArray format = reader.format();
            QVERIFY(!format.isEmpty());
            QImage img = reader.read();
            QVERIFY(!img.isNull());
            buffer.reset();
        }
    }
}

BOBUIEST_MAIN(tst_jpeg)

#include "jpeg.moc"
