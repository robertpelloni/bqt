// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTATUSBAR_H
#define QSTATUSBAR_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(statusbar);

BOBUI_BEGIN_NAMESPACE

class QStatusBarPrivate;

class Q_WIDGETS_EXPORT QStatusBar: public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled)

public:
    explicit QStatusBar(QWidget *parent = nullptr);
    virtual ~QStatusBar();

    void addWidget(QWidget *widget, int stretch = 0);
    int insertWidget(int index, QWidget *widget, int stretch = 0);
    void addPermanentWidget(QWidget *widget, int stretch = 0);
    int insertPermanentWidget(int index, QWidget *widget, int stretch = 0);
    void removeWidget(QWidget *widget);

    void setSizeGripEnabled(bool);
    bool isSizeGripEnabled() const;

    QString currentMessage() const;

public Q_SLOTS:
    void showMessage(const QString &text, int timeout = 0);
    void clearMessage();


Q_SIGNALS:
    void messageChanged(const QString &text);

protected:
    void showEvent(QShowEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    void reformat();
    void hideOrShow();
    bool event(QEvent *) override;

private:
    Q_DISABLE_COPY(QStatusBar)
    Q_DECLARE_PRIVATE(QStatusBar)
};

BOBUI_END_NAMESPACE

#endif // QSTATUSBAR_H
