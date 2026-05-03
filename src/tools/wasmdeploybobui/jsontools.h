// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef JSONTOOLS_H
#define JSONTOOLS_H

#include <QFileInfo>
#include <QSet>

#include "common.h"

#include <optional>

namespace JsonTools {
bool savePreloadFile(QSet<PreloadEntry> preload, QString destFile);
std::optional<QSet<PreloadEntry>> getPreloadsFromQmlImportScannerOutput(QString output);
}; // namespace JsonTools

#endif
