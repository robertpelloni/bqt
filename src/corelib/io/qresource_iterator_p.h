// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRESOURCE_ITERATOR_P_H
#define QRESOURCE_ITERATOR_P_H

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

#include "qabstractfileengine_p.h"
#include "qdir.h"

BOBUI_BEGIN_NAMESPACE

class QResourceFileEngineIteratorPrivate;
class QResourceFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    QResourceFileEngineIterator(const QString &path, QDir::Filters filters,
                                const QStringList &filterNames);
    QResourceFileEngineIterator(const QString &path, QDirListing::IteratorFlags filters,
                                const QStringList &filterNames);
    ~QResourceFileEngineIterator();

    bool advance() override;

    QString currentFileName() const override;
    QFileInfo currentFileInfo() const override;

private:
    mutable QStringList entries;
    mutable int index;
};

BOBUI_END_NAMESPACE

#endif
