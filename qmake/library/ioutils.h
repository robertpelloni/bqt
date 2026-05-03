// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef IOUTILS_H
#define IOUTILS_H

#include "qmake_global.h"

#include <qstring.h>

BOBUI_BEGIN_NAMESPACE

namespace QMakeInternal {

/*!
  This class provides replacement functionality for QFileInfo, QFile & QDir,
  as these are abysmally slow.
*/
class QMAKE_EXPORT IoUtils {
public:
    enum FileType {
        FileNotFound = 0,
        FileIsRegular = 1,
        FileIsDir = 2
    };

    static QString binaryAbsLocation(const QString &argv0);
    static FileType fileType(const QString &fileName);
    static bool exists(const QString &fileName) { return fileType(fileName) != FileNotFound; }
    static bool isRelativePath(const QString &fileName);
    static bool isAbsolutePath(const QString &fileName) { return !isRelativePath(fileName); }
    static QStringView pathName(const QString &fileName); // Requires normalized path
    static QStringView fileName(const QString &fileName); // Requires normalized path
    static QString resolvePath(const QString &baseDir, const QString &fileName);
    static QString shellQuoteUnix(const QString &arg);
    static QString shellQuoteWin(const QString &arg);
    static QString shellQuote(const QString &arg)
#ifdef Q_OS_UNIX
        { return shellQuoteUnix(arg); }
#else
        { return shellQuoteWin(arg); }
#endif
#if defined(PROEVALUATOR_FULL)
    static bool touchFile(const QString &targetFileName, const QString &referenceFileName, QString *errorString);
# if defined(BOBUI_BUILD_QMAKE) && defined(Q_OS_UNIX)
    static bool readLinkTarget(const QString &symlinkPath, QString *target);
# endif
#endif
};

} // namespace ProFileEvaluatorInternal

BOBUI_END_NAMESPACE

#endif // IOUTILS_H
