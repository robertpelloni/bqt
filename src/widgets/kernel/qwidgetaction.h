// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIDGETACTION_H
#define QWIDGETACTION_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/qaction.h>

BOBUI_REQUIRE_CONFIG(action);

BOBUI_BEGIN_NAMESPACE

class QWidgetActionPrivate;

class Q_WIDGETS_EXPORT QWidgetAction : public QAction
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWidgetAction)

public:
    explicit QWidgetAction(QObject *parent);
    virtual ~QWidgetAction();

    void setDefaultWidget(QWidget *w);
    QWidget *defaultWidget() const;

    QWidget *requestWidget(QWidget *parent);
    void releaseWidget(QWidget *widget);

protected:
    virtual bool event(QEvent *) override;
    virtual bool eventFilter(QObject *, QEvent *) override;
    virtual QWidget *createWidget(QWidget *parent);
    virtual void deleteWidget(QWidget *widget);
    QList<QWidget *> createdWidgets() const;

private:
    Q_DISABLE_COPY(QWidgetAction)
    friend class BOBUIoolBar;
};

BOBUI_END_NAMESPACE

#endif // QWIDGETACTION_H
