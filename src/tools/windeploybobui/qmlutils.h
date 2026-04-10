// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef QMLUTILS_H
#define QMLUTILS_H

#include "utils.h"

#include <QStringList>

BOBUI_BEGIN_NAMESPACE

QString findQmlDirectory(Platform platform, const QString &startDirectoryName);

struct QmlImportScanResult {
    struct Module {
        QString installPath(const QString &root) const;

        QString name;
        QString className;
        QString sourcePath;
        QString relativePath;
    };

    void append(const QmlImportScanResult &other);

    bool ok = false;
    QList<Module> modules;
    QStringList plugins;
};

bool operator==(const QmlImportScanResult::Module &m1, const QmlImportScanResult::Module &m2);

QmlImportScanResult runQmlImportScanner(const QString &directory, const QStringList &qmlImportPaths,
                                        bool usesWidgets, int platform, DebugMatchMode debugMatchMode,
                                        QString *errorMessage, int timeout = 30000);

BOBUI_END_NAMESPACE

#endif // QMLUTILS_H
