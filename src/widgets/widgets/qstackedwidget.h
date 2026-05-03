// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTACKEDWIDGET_H
#define QSTACKEDWIDGET_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>

BOBUI_REQUIRE_CONFIG(stackedwidget);

BOBUI_BEGIN_NAMESPACE

class QStackedWidgetPrivate;

class Q_WIDGETS_EXPORT QStackedWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)
public:
    explicit QStackedWidget(QWidget *parent = nullptr);
    ~QStackedWidget();

    int addWidget(QWidget *w);
    int insertWidget(int index, QWidget *w);
    void removeWidget(QWidget *w);

    QWidget *currentWidget() const;
    int currentIndex() const;

    int indexOf(const QWidget *) const;
    QWidget *widget(int) const;
    int count() const;

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);

Q_SIGNALS:
    void currentChanged(int);
    void widgetRemoved(int index);
    void widgetAdded(int index);

protected:
    bool event(QEvent *e) override;

private:
    Q_DISABLE_COPY(QStackedWidget)
    Q_DECLARE_PRIVATE(QStackedWidget)
};

BOBUI_END_NAMESPACE

#endif // QSTACKEDWIDGET_H
