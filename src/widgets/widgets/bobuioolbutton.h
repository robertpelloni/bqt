// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIOOLBUTTON_H
#define BOBUIOOLBUTTON_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>

#include <BobUIWidgets/qabstractbutton.h>

BOBUI_REQUIRE_CONFIG(toolbutton);

BOBUI_BEGIN_NAMESPACE

class BOBUIoolButtonPrivate;
class QMenu;
class QStyleOptionToolButton;

class Q_WIDGETS_EXPORT BOBUIoolButton : public QAbstractButton
{
    Q_OBJECT
#if BOBUI_CONFIG(menu)
    Q_PROPERTY(ToolButtonPopupMode popupMode READ popupMode WRITE setPopupMode)
#endif
    Q_PROPERTY(BobUI::ToolButtonStyle toolButtonStyle READ toolButtonStyle WRITE setToolButtonStyle)
    Q_PROPERTY(bool autoRaise READ autoRaise WRITE setAutoRaise)
    Q_PROPERTY(BobUI::ArrowType arrowType READ arrowType WRITE setArrowType)

public:
    enum ToolButtonPopupMode {
        DelayedPopup,
        MenuButtonPopup,
        InstantPopup
    };
    Q_ENUM(ToolButtonPopupMode)

    explicit BOBUIoolButton(QWidget *parent = nullptr);
    ~BOBUIoolButton();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    BobUI::ToolButtonStyle toolButtonStyle() const;

    BobUI::ArrowType arrowType() const;
    void setArrowType(BobUI::ArrowType type);

#if BOBUI_CONFIG(menu)
    void setMenu(QMenu* menu);
    QMenu* menu() const;

    void setPopupMode(ToolButtonPopupMode mode);
    ToolButtonPopupMode popupMode() const;
#endif

    QAction *defaultAction() const;

    void setAutoRaise(bool enable);
    bool autoRaise() const;

public Q_SLOTS:
#if BOBUI_CONFIG(menu)
    void showMenu();
#endif
    void setToolButtonStyle(BobUI::ToolButtonStyle style);
    void setDefaultAction(QAction *);

Q_SIGNALS:
    void triggered(QAction *);

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void actionEvent(QActionEvent *) override;

    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void timerEvent(BOBUIimerEvent *) override;
    void changeEvent(QEvent *) override;

    bool hitButton(const QPoint &pos) const override;
    void checkStateSet() override;
    void nextCheckState() override;
    virtual void initStyleOption(QStyleOptionToolButton *option) const;

private:
    Q_DISABLE_COPY(BOBUIoolButton)
    Q_DECLARE_PRIVATE(BOBUIoolButton)
};

BOBUI_END_NAMESPACE

#endif // BOBUIOOLBUTTON_H
