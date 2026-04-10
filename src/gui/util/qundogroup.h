// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUNDOGROUP_H
#define QUNDOGROUP_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qstring.h>

BOBUI_REQUIRE_CONFIG(undogroup);

BOBUI_BEGIN_NAMESPACE

class QUndoGroupPrivate;
class QUndoStack;
class QAction;

class Q_GUI_EXPORT QUndoGroup : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QUndoGroup)

public:
    explicit QUndoGroup(QObject *parent = nullptr);
    ~QUndoGroup();

    void addStack(QUndoStack *stack);
    void removeStack(QUndoStack *stack);
    QList<QUndoStack*> stacks() const;
    QUndoStack *activeStack() const;

#ifndef BOBUI_NO_ACTION
    QAction *createUndoAction(QObject *parent, const QString &prefix = QString()) const;
    QAction *createRedoAction(QObject *parent, const QString &prefix = QString()) const;
#endif // BOBUI_NO_ACTION

    bool canUndo() const;
    bool canRedo() const;
    QString undoText() const;
    QString redoText() const;
    bool isClean() const;

public Q_SLOTS:
    void undo();
    void redo();
    void setActiveStack(QUndoStack *stack);

Q_SIGNALS:
    void activeStackChanged(QUndoStack *stack);
    void indexChanged(int idx);
    void cleanChanged(bool clean);
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void undoTextChanged(const QString &undoText);
    void redoTextChanged(const QString &redoText);

private:
    Q_DISABLE_COPY(QUndoGroup)
};

BOBUI_END_NAMESPACE

#endif // QUNDOGROUP_H
