// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLEHELPER_P_H
#define QACCESSIBLEHELPER_P_H

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

#include <BobUICore/QString>
#include <BobUIGui/qaccessible.h>
#include <BobUIGui/bobuiguiglobal.h>

class BOBUIextCursor;

BOBUI_BEGIN_NAMESPACE

Q_GUI_EXPORT QString bobui_accStripAmp(const QString &text);

Q_GUI_EXPORT QString bobui_accTextBeforeOffsetHelper(const QAccessibleTextInterface &textInterface,
                                                  const BOBUIextCursor &textCursor, int offset,
                                                  QAccessible::TextBoundaryType boundaryType,
                                                  int *startOffset, int *endOffset);

Q_GUI_EXPORT QString bobui_accTextAfterOffsetHelper(const QAccessibleTextInterface &textInterface,
                                                 const BOBUIextCursor &textCursor, int offset,
                                                 QAccessible::TextBoundaryType boundaryType,
                                                 int *startOffset, int *endOffset);

Q_GUI_EXPORT QString bobui_accTextAtOffsetHelper(const QAccessibleTextInterface &textInterface,
                                              const BOBUIextCursor &textCursor, int offset,
                                              QAccessible::TextBoundaryType boundaryType,
                                              int *startOffset, int *endOffset);

BOBUI_END_NAMESPACE

#endif // QACCESSIBLEHELPER_P_H
