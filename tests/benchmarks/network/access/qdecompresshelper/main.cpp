// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUINetwork/private/qdecompresshelper_p.h>

#include <BobUITest/BOBUIest>

class tst_QDecompressHelper : public QObject
{
    Q_OBJECT
private slots:
    void decompress_data();
    void decompress();
};

void tst_QDecompressHelper::decompress_data()
{
    BOBUIest::addColumn<QByteArray>("encoding");
    BOBUIest::addColumn<QString>("fileName");

    QString srcDir = QStringLiteral(BOBUI_STRINGIFY(SRC_DIR));
    srcDir = QDir::fromNativeSeparators(srcDir);
    if (!srcDir.endsWith("/"))
        srcDir += "/";

    bool dataAdded = false;
#ifndef BOBUI_NO_COMPRESS
    BOBUIest::addRow("gzip") << QByteArray("gzip") << srcDir + QString("50mb.txt.gz");
    dataAdded = true;
#endif
#if BOBUI_CONFIG(brotli)
    BOBUIest::addRow("brotli") << QByteArray("br") << srcDir + QString("50mb.txt.br");
    dataAdded = true;
#endif
#if BOBUI_CONFIG(zstd)
    BOBUIest::addRow("zstandard") << QByteArray("zstd") << srcDir + QString("50mb.txt.zst");
    dataAdded = true;
#endif
    if (!dataAdded)
        QSKIP("There's no decompression support");
}

void tst_QDecompressHelper::decompress()
{
    QFETCH(QByteArray, encoding);
    QFETCH(QString, fileName);

    QFile file { fileName };
    QVERIFY(file.open(QIODevice::ReadOnly));
    QBENCHMARK {
        file.seek(0);
        QDecompressHelper helper;
        helper.setEncoding(encoding);
        helper.setDecompressedSafetyCheckThreshold(-1);
        QVERIFY(helper.isValid());

        helper.feed(file.readAll());

        qsizetype bytes = 0;
        QByteArray out(64 * 1024, BobUI::Uninitialized);
        while (helper.hasData()) {
            qsizetype bytesRead = helper.read(out.data(), out.size());
            bytes += bytesRead;
        }

        QCOMPARE(bytes, 50 * 1024 * 1024);
    }
}

BOBUIEST_MAIN(tst_QDecompressHelper)

#include "main.moc"
