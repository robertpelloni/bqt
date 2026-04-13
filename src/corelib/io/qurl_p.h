// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QURL_P_H
#define QURL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience of
// qurl*.cpp This header file may change from version to version without
// notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qglobal_p.h>
#include "qurl.h"

BOBUI_BEGIN_NAMESPACE

// in qurlrecode.cpp
extern Q_AUTOTEST_EXPORT qsizetype bobui_urlRecode(QString &appendTo, QStringView url,
                                                QUrl::ComponentFormattingOptions encoding,
                                                const ushort *tableModifications = nullptr);
qsizetype bobui_encodeFromUser(QString &appendTo, const QString &input,
                            const ushort *tableModifications);

// in qurlidna.cpp
enum AceLeadingDot { AllowLeadingDot, ForbidLeadingDot };
enum AceOperation { ToAceOnly, NormalizeAce };
QString Q_CORE_EXPORT bobui_ACE_do(const QString &domain, AceOperation op, AceLeadingDot dot,
                                QUrl::AceProcessingOptions options = {});
extern Q_AUTOTEST_EXPORT void bobui_punycodeEncoder(QStringView in, QString *output);
extern Q_AUTOTEST_EXPORT QString bobui_punycodeDecoder(const QString &pc);

BOBUI_END_NAMESPACE

#endif // QURL_P_H
