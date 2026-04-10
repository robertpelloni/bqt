// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMIMETYPE_P_H
#define QMIMETYPE_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qshareddata.h>

BOBUI_REQUIRE_CONFIG(mimetype);

#include <BobUICore/qhash.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QMimeTypePrivate : public QSharedData
{
public:
    typedef QHash<QString, QString> LocaleHash;

    QMimeTypePrivate() { }
    explicit QMimeTypePrivate(const QString &name) : name(name) { }

    QString name;
};

BOBUI_END_NAMESPACE

#endif // QMIMETYPE_P_H
