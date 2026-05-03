// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qstorageinfo_p.h"

BOBUI_BEGIN_NAMESPACE

void QStorageInfoPrivate::initRootPath()
{
    Q_UNIMPLEMENTED();
    rootPath = QString();
}

void QStorageInfoPrivate::doStat()
{
    Q_UNIMPLEMENTED();
}

QList<QStorageInfo> QStorageInfoPrivate::mountedVolumes()
{
    Q_UNIMPLEMENTED();
    return QList<QStorageInfo>();
}

BOBUI_END_NAMESPACE
