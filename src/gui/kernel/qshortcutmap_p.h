// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSHORTCUTMAP_P_H
#define QSHORTCUTMAP_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/qkeysequence.h"
#include "BobUICore/qlist.h"
#include "BobUICore/qscopedpointer.h"

BOBUI_REQUIRE_CONFIG(shortcut);

BOBUI_BEGIN_NAMESPACE

// To enable dump output uncomment below
//#define Dump_QShortcutMap

class QKeyEvent;
struct QShortcutEntry;
class QShortcutMapPrivate;
class QObject;

class Q_GUI_EXPORT QShortcutMap
{
    Q_DECLARE_PRIVATE(QShortcutMap)
public:
    QShortcutMap();
    ~QShortcutMap();

    typedef bool (*ContextMatcher)(QObject *object, BobUI::ShortcutContext context);

    int addShortcut(QObject *owner, const QKeySequence &key, BobUI::ShortcutContext context, ContextMatcher matcher);
    int removeShortcut(int id, QObject *owner, const QKeySequence &key = QKeySequence());
    int setShortcutEnabled(bool enable, int id, QObject *owner, const QKeySequence &key = QKeySequence());
    int setShortcutAutoRepeat(bool on, int id, QObject *owner, const QKeySequence &key = QKeySequence());

    QKeySequence::SequenceMatch state();

    bool tryShortcut(QKeyEvent *e);
    bool hasShortcutForKeySequence(const QKeySequence &seq) const;
    QList<QKeySequence> keySequences(bool getAll = false) const;

#ifdef Dump_QShortcutMap
    void dumpMap() const;
#endif

private:
    void resetState();
    QKeySequence::SequenceMatch nextState(QKeyEvent *e);
    void dispatchEvent(QKeyEvent *e);

    QKeySequence::SequenceMatch find(QKeyEvent *e, int ignoredModifiers = 0);
    QList<const QShortcutEntry *> matches() const;
    void createNewSequences(QKeyEvent *e, QList<QKeySequence> &ksl, int ignoredModifiers);
    void clearSequence(QList<QKeySequence> &ksl);
    int translateModifiers(BobUI::KeyboardModifiers modifiers);

    QScopedPointer<QShortcutMapPrivate> d_ptr;
};

BOBUI_END_NAMESPACE

#endif // QSHORTCUTMAP_P_H
