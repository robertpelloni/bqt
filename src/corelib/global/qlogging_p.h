// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLOGGING_P_H
#define QLOGGING_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include "qlogging.h"
#include <BobUICore/qloggingcategory.h>

#if !defined(BOBUI_BOOTSTRAPPED) && BOBUI_CONFIG(regularexpression)
#  if __has_include(<cxxabi.h>) && BOBUI_CONFIG(backtrace)
#    include <optional>
#    include "qvarlengtharray.h"
#    define QLOGGING_USE_EXECINFO_BACKTRACE
#    define QLOGGING_HAVE_BACKTRACE
#  elif BOBUI_CONFIG(cxx23_stacktrace)
#    include <optional>
#    include <stacktrace>
#    define QLOGGING_USE_STD_BACKTRACE
#    define QLOGGING_HAVE_BACKTRACE
#  endif
#endif // BOBUI_BOOTSTRAPPED

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {

Q_CORE_EXPORT bool shouldLogToStderr();

}

class QInternalMessageLogContext : public QMessageLogContext
{
public:
    static constexpr int DefaultBacktraceDepth = 32;

#if defined(QLOGGING_USE_EXECINFO_BACKTRACE)
    using BacktraceStorage = QVarLengthArray<void *, DefaultBacktraceDepth>;
#elif defined(QLOGGING_USE_STD_BACKTRACE)
    using BacktraceStorage = std::stacktrace;
#else
    using BacktraceStorage = bool; // dummy
#endif

    std::optional<BacktraceStorage> backtrace;

    Q_ALWAYS_INLINE QInternalMessageLogContext(const QMessageLogContext &logContext)
    {
        int backtraceFrames = initFrom(logContext);
        if (backtraceFrames)
            populateBacktrace(backtraceFrames);
    }
    QInternalMessageLogContext(const QMessageLogContext &logContext,
                               const QLoggingCategory &categoryOverride)
        : QInternalMessageLogContext(logContext)
    {
        category = categoryOverride.categoryName();
    }

    int initFrom(const QMessageLogContext &logContext);
    void populateBacktrace(int frameCount);
};

BOBUI_END_NAMESPACE

#endif // QLOGGING_P_H
