// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSFILEENGINEFACTORY_H
#define QIOSFILEENGINEFACTORY_H

#include <BobUICore/qstandardpaths.h>
#include <BobUICore/private/qabstractfileengine_p.h>
#include "qiosfileengineassetslibrary.h"

BOBUI_BEGIN_NAMESPACE

class QIOSFileEngineFactory : public QAbstractFileEngineHandler
{
    Q_DISABLE_COPY_MOVE(QIOSFileEngineFactory)
public:
    QIOSFileEngineFactory() = default;

    std::unique_ptr<QAbstractFileEngine> create(const QString &fileName) const
    {
        Q_CONSTINIT static QLatin1StringView assetsScheme("assets-library:");

#ifndef Q_OS_TVOS
        if (fileName.toLower().startsWith(assetsScheme))
            return std::make_unique<QIOSFileEngineAssetsLibrary>(fileName);
#else
        Q_UNUSED(fileName);
#endif

        return {};
    }
};

BOBUI_END_NAMESPACE

#endif // QIOSFILEENGINEFACTORY_H
