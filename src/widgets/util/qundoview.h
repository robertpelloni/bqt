// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUNDOVIEW_H
#define QUNDOVIEW_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qlistview.h>
#include <BobUICore/qstring.h>

BOBUI_REQUIRE_CONFIG(undoview);

BOBUI_BEGIN_NAMESPACE

class QUndoViewPrivate;
class QUndoStack;
class QUndoGroup;
class QIcon;


class Q_WIDGETS_EXPORT QUndoView : public QListView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QUndoView)
    Q_PROPERTY(QString emptyLabel READ emptyLabel WRITE setEmptyLabel)
    Q_PROPERTY(QIcon cleanIcon READ cleanIcon WRITE setCleanIcon)

public:
    explicit QUndoView(QWidget *parent = nullptr);
    explicit QUndoView(QUndoStack *stack, QWidget *parent = nullptr);
#if BOBUI_CONFIG(undogroup)
    explicit QUndoView(QUndoGroup *group, QWidget *parent = nullptr);
#endif
    ~QUndoView();

    QUndoStack *stack() const;
#if BOBUI_CONFIG(undogroup)
    QUndoGroup *group() const;
#endif

    void setEmptyLabel(const QString &label);
    QString emptyLabel() const;

    void setCleanIcon(const QIcon &icon);
    QIcon cleanIcon() const;

public Q_SLOTS:
    void setStack(QUndoStack *stack);
#if BOBUI_CONFIG(undogroup)
    void setGroup(QUndoGroup *group);
#endif

private:
    Q_DISABLE_COPY(QUndoView)
};

BOBUI_END_NAMESPACE

#endif // QUNDOVIEW_H
