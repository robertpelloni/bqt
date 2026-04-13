// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRADIOBUTTON_H
#define QRADIOBUTTON_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractbutton.h>

BOBUI_REQUIRE_CONFIG(radiobutton);

BOBUI_BEGIN_NAMESPACE


class QRadioButtonPrivate;
class QStyleOptionButton;

class Q_WIDGETS_EXPORT QRadioButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit QRadioButton(QWidget *parent = nullptr);
    explicit QRadioButton(const QString &text, QWidget *parent = nullptr);
    ~QRadioButton();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    bool event(QEvent *e) override;
    bool hitButton(const QPoint &) const override;
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    virtual void initStyleOption(QStyleOptionButton *button) const;


private:
    Q_DECLARE_PRIVATE(QRadioButton)
    Q_DISABLE_COPY(QRadioButton)
    friend class QAccessibleButton;
};

BOBUI_END_NAMESPACE

#endif // QRADIOBUTTON_H
