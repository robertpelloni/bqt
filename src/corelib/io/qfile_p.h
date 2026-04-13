// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILE_P_H
#define QFILE_P_H

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

#include "qfile.h"
#include "private/qfiledevice_p.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIemporaryFile;

class QFilePrivate : public QFileDevicePrivate
{
    Q_DECLARE_PUBLIC(QFile)
    friend class BOBUIemporaryFile;

protected:
    QFilePrivate();
    ~QFilePrivate();

    bool openExternalFile(QIODevice::OpenMode flags, int fd, QFile::FileHandleFlags handleFlags);
    bool openExternalFile(QIODevice::OpenMode flags, FILE *fh, QFile::FileHandleFlags handleFlags);

    QAbstractFileEngine *engine() const override;
    bool copy(const QString &newName);

    QString fileName;

#ifndef BOBUI_NO_DEBUG_STREAM
    void writeToDebugStream(QDebug &) const override;
#endif
};

BOBUI_END_NAMESPACE

#endif // QFILE_P_H
