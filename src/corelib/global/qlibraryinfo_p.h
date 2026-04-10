// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLIBRARYINFO_P_H
#define QLIBRARYINFO_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of a number of BobUI sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "BobUICore/qlibraryinfo.h"
#include "BobUICore/private/qglobal_p.h"

#if BOBUI_CONFIG(settings)
#    include "BobUICore/qsettings.h"
#endif
#include "BobUICore/qstring.h"

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QLibraryInfoPrivate final
{
public:
#if BOBUI_CONFIG(settings)
    static QSettings *configuration();
    static void reload();
    static void setBobUIconfManualPath(const QString *bobuiconfManualPath);
#endif

    struct LocationInfo
    {
        QString key;
        QString defaultValue;
        QString fallbackKey;
    };

    static LocationInfo locationInfo(QLibraryInfo::LibraryPath loc);

    enum UsageMode {
        RegularUsage,
        UsedFromBobUIBinDir
    };

    static QString path(QLibraryInfo::LibraryPath p, UsageMode usageMode = RegularUsage);
    static QList<QString> paths(QLibraryInfo::LibraryPath p, UsageMode usageMode = RegularUsage);
};

BOBUI_END_NAMESPACE

#endif // QLIBRARYINFO_P_H
