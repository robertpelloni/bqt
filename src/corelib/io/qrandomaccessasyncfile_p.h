// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRANDOMACCESSASYNCFILE_P_H
#define QRANDOMACCESSASYNCFILE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qiooperation_p.h"

#include <BobUICore/qobject.h>
#include <BobUICore/qspan.h>

BOBUI_BEGIN_NAMESPACE

class QRandomAccessAsyncFilePrivate;
class Q_CORE_EXPORT QRandomAccessAsyncFile : public QObject
{
    Q_OBJECT
public:
    explicit QRandomAccessAsyncFile(QObject *parent = nullptr);
    ~QRandomAccessAsyncFile() override;

    // sync APIs
    void close();
    qint64 size() const;

    [[nodiscard]] QIOOperation *open(const QString &filePath, QIODeviceBase::OpenMode mode);
    [[nodiscard]] QIOOperation *flush();

    // owning APIs: we are responsible for storing the data
    [[nodiscard]] QIOReadOperation *read(qint64 offset, qint64 maxSize);
    [[nodiscard]] QIOWriteOperation *write(qint64 offset, const QByteArray &data);
    [[nodiscard]] QIOWriteOperation *write(qint64 offset, QByteArray &&data);

    // non-owning APIs: the user has to control the lifetime of buffers
    [[nodiscard]] QIOVectoredReadOperation *
    readInto(qint64 offset, QSpan<std::byte> buffer);
    [[nodiscard]] QIOVectoredWriteOperation *
    writeFrom(qint64 offset, QSpan<const std::byte> buffer);

    // vectored IO APIs, also non-owning
    [[nodiscard]] QIOVectoredReadOperation *
    readInto(qint64 offset, QSpan<const QSpan<std::byte>> buffers);
    [[nodiscard]] QIOVectoredWriteOperation *
    writeFrom(qint64 offset, QSpan<const QSpan<const std::byte>> buffers);

Q_SIGNALS:

private:
    Q_DECLARE_PRIVATE(QRandomAccessAsyncFile)
    Q_DISABLE_COPY_MOVE(QRandomAccessAsyncFile)
};

BOBUI_END_NAMESPACE

#endif // QRANDOMACCESSASYNCFILE_P_H
