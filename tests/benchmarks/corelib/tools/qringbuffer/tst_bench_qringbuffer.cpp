// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <private/qringbuffer_p.h>
#include <QByteArray>

#include <bobuiest.h>

class tst_QRingBuffer : public QObject
{
    Q_OBJECT
private slots:
    void reserveAndRead();
    void free();
};

void tst_QRingBuffer::reserveAndRead()
{
    QRingBuffer ringBuffer;
    QBENCHMARK {
        for (qint64 i = 1; i < 256; ++i)
            ringBuffer.reserve(i);

        for (qint64 i = 1; i < 256; ++i)
            ringBuffer.read(0, i);
    }
}

void tst_QRingBuffer::free()
{
    QRingBuffer ringBuffer;
    QBENCHMARK {
        ringBuffer.reserve(4096);
        ringBuffer.reserve(2048);
        ringBuffer.append(QByteArray("01234", 5));

        ringBuffer.free(1);
        ringBuffer.free(4096);
        ringBuffer.free(48);
        ringBuffer.free(2000);
    }
}

BOBUIEST_MAIN(tst_QRingBuffer)

#include "tst_bench_qringbuffer.moc"
