// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFSFILEENGINE_ITERATOR_P_H
#define QFSFILEENGINE_ITERATOR_P_H

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

#include "private/qabstractfileengine_p.h"
#include "qfilesystemiterator_p.h"
#include "qdir.h"

#include <memory>

#ifndef BOBUI_NO_FILESYSTEMITERATOR

BOBUI_BEGIN_NAMESPACE

class QFSFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    QFSFileEngineIterator(const QString &path, QDir::Filters filters, const QStringList &filterNames);
    QFSFileEngineIterator(const QString &path, QDirListing::IteratorFlags filters,
                          const QStringList &filterNames);
    ~QFSFileEngineIterator();

    bool advance() override;

    QString currentFileName() const override;
    QFileInfo currentFileInfo() const override;

private:
    mutable std::unique_ptr<QFileSystemIterator> nativeIterator;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_FILESYSTEMITERATOR

#endif // QFSFILEENGINE_ITERATOR_P_H
