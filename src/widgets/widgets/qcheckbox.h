// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCHECKBOX_H
#define QCHECKBOX_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractbutton.h>

BOBUI_REQUIRE_CONFIG(checkbox);

BOBUI_BEGIN_NAMESPACE


class QCheckBoxPrivate;
class QStyleOptionButton;

class Q_WIDGETS_EXPORT QCheckBox : public QAbstractButton
{
    Q_OBJECT

    Q_PROPERTY(bool tristate READ isTristate WRITE setTristate)

public:
    explicit QCheckBox(QWidget *parent = nullptr);
    explicit QCheckBox(const QString &text, QWidget *parent = nullptr);
    ~QCheckBox();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setTristate(bool y = true);
    bool isTristate() const;

    BobUI::CheckState checkState() const;
    void setCheckState(BobUI::CheckState state);

Q_SIGNALS:
#if BOBUI_DEPRECATED_SINCE(6, 9)
    BOBUI_MOC_COMPAT BOBUI_DEPRECATED_VERSION_X_6_9("Use checkStateChanged() instead")
    void stateChanged(int);
#endif
    void checkStateChanged(BobUI::CheckState);

protected:
    bool event(QEvent *e) override;
    bool hitButton(const QPoint &pos) const override;
    void checkStateSet() override;
    void nextCheckState() override;
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    virtual void initStyleOption(QStyleOptionButton *option) const;


private:
    Q_DECLARE_PRIVATE(QCheckBox)
    Q_DISABLE_COPY(QCheckBox)
    friend class QAccessibleButton;
};

BOBUI_END_NAMESPACE

#endif // QCHECKBOX_H
