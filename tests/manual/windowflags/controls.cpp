// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#undef BOBUI_NO_FOREACH // this file contains unported legacy Q_FOREACH uses

#include "controls.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDebug>

HintControl::HintControl(QWidget *parent)
    : QGroupBox(tr("Hints"), parent)
    , msWindowsFixedSizeDialogCheckBox(new QCheckBox(tr("MS Windows fixed size dialog")))
    , x11BypassWindowManagerCheckBox(new QCheckBox(tr("X11 bypass window manager")))
    , framelessWindowCheckBox(new QCheckBox(tr("Frameless window")))
    , windowTitleCheckBox(new QCheckBox(tr("Window title")))
    , windowSystemMenuCheckBox(new QCheckBox(tr("Window system menu")))
    , windowMinimizeButtonCheckBox(new QCheckBox(tr("Window minimize button")))
    , windowMaximizeButtonCheckBox(new QCheckBox(tr("Window maximize button")))
    , windowFullscreenButtonCheckBox(new QCheckBox(tr("Window fullscreen button")))
    , windowCloseButtonCheckBox(new QCheckBox(tr("Window close button")))
    , windowContextHelpButtonCheckBox(new QCheckBox(tr("Window context help button")))
    , windowShadeButtonCheckBox(new QCheckBox(tr("Window shade button")))
    , windowStaysOnTopCheckBox(new QCheckBox(tr("Window stays on top")))
    , windowStaysOnBottomCheckBox(new QCheckBox(tr("Window stays on bottom")))
    , customizeWindowGroup(new QGroupBox(tr("Customize window title bar controls")))
    , transparentForInputCheckBox(new QCheckBox(tr("Transparent for input")))
    , noDropShadowCheckBox(new QCheckBox(tr("No drop shadow")))
    , expandedClientAreaCheckBox(new QCheckBox(tr("Expanded client area")))
    , noTitleBarBackgroundCheckBox(new QCheckBox(tr("No titlebar background")))
{
    connect(msWindowsFixedSizeDialogCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(x11BypassWindowManagerCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(framelessWindowCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowTitleCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowSystemMenuCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowMinimizeButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowMaximizeButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowFullscreenButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowCloseButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowContextHelpButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowShadeButtonCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowStaysOnTopCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(windowStaysOnBottomCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(customizeWindowGroup, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(transparentForInputCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(noDropShadowCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(expandedClientAreaCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));
    connect(noTitleBarBackgroundCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxChanged()));

    auto *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);

    auto *basicHintsLayout = new QVBoxLayout;
    basicHintsLayout->setSpacing(0);
    basicHintsLayout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);

    basicHintsLayout->addWidget(framelessWindowCheckBox);
    basicHintsLayout->addWidget(noDropShadowCheckBox);
    basicHintsLayout->addWidget(windowStaysOnTopCheckBox);
    basicHintsLayout->addWidget(windowStaysOnBottomCheckBox);
    basicHintsLayout->addWidget(transparentForInputCheckBox);
    basicHintsLayout->addWidget(msWindowsFixedSizeDialogCheckBox);
    basicHintsLayout->addWidget(x11BypassWindowManagerCheckBox);
    basicHintsLayout->addWidget(expandedClientAreaCheckBox);
    basicHintsLayout->addWidget(noTitleBarBackgroundCheckBox);
    layout->addLayout(basicHintsLayout);

    customizeWindowGroup->setCheckable(true);
    customizeWindowGroup->setChecked(false);
    auto *customizeWindowLayout = new QVBoxLayout(customizeWindowGroup);
    customizeWindowLayout->setSpacing(0);
    customizeWindowLayout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);

    customizeWindowLayout->addWidget(windowTitleCheckBox);
    customizeWindowLayout->addWidget(windowSystemMenuCheckBox);
    customizeWindowLayout->addWidget(windowMinimizeButtonCheckBox);
    customizeWindowLayout->addWidget(windowShadeButtonCheckBox);
    customizeWindowLayout->addWidget(windowMaximizeButtonCheckBox);
    customizeWindowLayout->addWidget(windowFullscreenButtonCheckBox);
    customizeWindowLayout->addWidget(windowCloseButtonCheckBox);
    customizeWindowLayout->addWidget(windowContextHelpButtonCheckBox);

    layout->addWidget(customizeWindowGroup);
}

BobUI::WindowFlags HintControl::hints() const
{
    BobUI::WindowFlags flags;
    if (msWindowsFixedSizeDialogCheckBox->isChecked())
        flags |= BobUI::MSWindowsFixedSizeDialogHint;
    if (x11BypassWindowManagerCheckBox->isChecked())
        flags |= BobUI::X11BypassWindowManagerHint;
    if (framelessWindowCheckBox->isChecked())
        flags |= BobUI::FramelessWindowHint;
    if (windowTitleCheckBox->isChecked())
        flags |= BobUI::WindowTitleHint;
    if (windowSystemMenuCheckBox->isChecked())
        flags |= BobUI::WindowSystemMenuHint;
    if (windowMinimizeButtonCheckBox->isChecked())
        flags |= BobUI::WindowMinimizeButtonHint;
    if (windowMaximizeButtonCheckBox->isChecked())
        flags |= BobUI::WindowMaximizeButtonHint;
    if (windowFullscreenButtonCheckBox->isChecked())
        flags |= BobUI::WindowFullscreenButtonHint;
    if (windowCloseButtonCheckBox->isChecked())
        flags |= BobUI::WindowCloseButtonHint;
    if (windowContextHelpButtonCheckBox->isChecked())
        flags |= BobUI::WindowContextHelpButtonHint;
    if (windowShadeButtonCheckBox->isChecked())
        flags |= BobUI::WindowShadeButtonHint;
    if (windowStaysOnTopCheckBox->isChecked())
        flags |= BobUI::WindowStaysOnTopHint;
    if (windowStaysOnBottomCheckBox->isChecked())
        flags |= BobUI::WindowStaysOnBottomHint;
    if (customizeWindowGroup->isChecked())
        flags |= BobUI::CustomizeWindowHint;
    if (transparentForInputCheckBox->isChecked())
        flags |= BobUI::WindowTransparentForInput;
    if (noDropShadowCheckBox->isChecked())
        flags |= BobUI::NoDropShadowWindowHint;
    if (expandedClientAreaCheckBox->isChecked())
        flags |= BobUI::ExpandedClientAreaHint;
    if (noTitleBarBackgroundCheckBox->isChecked())
        flags |= BobUI::NoTitleBarBackgroundHint;
    return flags;
}

void HintControl::setHints(BobUI::WindowFlags flags)
{
    msWindowsFixedSizeDialogCheckBox->setChecked(flags & BobUI::MSWindowsFixedSizeDialogHint);
    x11BypassWindowManagerCheckBox->setChecked(flags & BobUI::X11BypassWindowManagerHint);
    framelessWindowCheckBox->setChecked(flags & BobUI::FramelessWindowHint);
    windowTitleCheckBox->setChecked(flags & BobUI::WindowTitleHint);
    windowSystemMenuCheckBox->setChecked(flags & BobUI::WindowSystemMenuHint);
    windowMinimizeButtonCheckBox->setChecked(flags & BobUI::WindowMinimizeButtonHint);
    windowMaximizeButtonCheckBox->setChecked(flags & BobUI::WindowMaximizeButtonHint);
    windowFullscreenButtonCheckBox->setChecked(flags & BobUI::WindowFullscreenButtonHint);
    windowCloseButtonCheckBox->setChecked(flags & BobUI::WindowCloseButtonHint);
    windowContextHelpButtonCheckBox->setChecked(flags & BobUI::WindowContextHelpButtonHint);
    windowShadeButtonCheckBox->setChecked(flags & BobUI::WindowShadeButtonHint);
    windowStaysOnTopCheckBox->setChecked(flags & BobUI::WindowStaysOnTopHint);
    windowStaysOnBottomCheckBox->setChecked(flags & BobUI::WindowStaysOnBottomHint);
    customizeWindowGroup->setChecked(flags & BobUI::CustomizeWindowHint);
    transparentForInputCheckBox->setChecked(flags & BobUI::WindowTransparentForInput);
    noDropShadowCheckBox->setChecked(flags & BobUI::NoDropShadowWindowHint);
    expandedClientAreaCheckBox->setChecked(flags & BobUI::ExpandedClientAreaHint);
    noTitleBarBackgroundCheckBox->setChecked(flags & BobUI::NoTitleBarBackgroundHint);
}

void HintControl::slotCheckBoxChanged()
{
    emit changed(hints());
}

WindowStateControl::WindowStateControl(QWidget *parent)
    : QWidget(parent)
    , group(new QButtonGroup)
    , restoreButton(new QCheckBox(tr("Normal")))
    , minimizeButton(new QCheckBox(tr("Minimized")))
    , maximizeButton(new QCheckBox(tr("Maximized")))
    , fullscreenButton(new QCheckBox(tr("Fullscreen")))
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    group->setExclusive(false);
    layout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);
    group->addButton(restoreButton, BobUI::WindowNoState);
    restoreButton->setEnabled(false);
    layout->addWidget(restoreButton);
    group->addButton(minimizeButton, BobUI::WindowMinimized);
    layout->addWidget(minimizeButton);
    group->addButton(maximizeButton, BobUI::WindowMaximized);
    layout->addWidget(maximizeButton);
    group->addButton(fullscreenButton, BobUI::WindowFullScreen);
    layout->addWidget(fullscreenButton);
    connect(group, &QButtonGroup::idReleased, this, &WindowStateControl::stateChanged);
}

BobUI::WindowStates WindowStateControl::state() const
{
    BobUI::WindowStates states;
    foreach (QAbstractButton *button, group->buttons()) {
        if (button->isChecked())
            states |= BobUI::WindowState(group->id(button));
    }
    return states;
}

void WindowStateControl::setState(BobUI::WindowStates s)
{
    group->blockSignals(true);
    foreach (QAbstractButton *button, group->buttons())
        button->setChecked(s & BobUI::WindowState(group->id(button)));

    if (!(s & (BobUI::WindowMaximized | BobUI::WindowFullScreen)))
        restoreButton->setChecked(true);

    group->blockSignals(false);
}

WindowStatesControl::WindowStatesControl(QWidget *parent)
    : QGroupBox(tr("States"), parent)
    , visibleCheckBox(new QCheckBox(tr("Visible")))
    , activeCheckBox(new QCheckBox(tr("Active")))
    , stateControl(new WindowStateControl)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);
    connect(visibleCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
    layout->addWidget(visibleCheckBox);
    connect(activeCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
    layout->addWidget(activeCheckBox);
    layout->addWidget(stateControl);
    connect(stateControl, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
}

BobUI::WindowStates WindowStatesControl::states() const
{
    BobUI::WindowStates s = stateControl->state();
    if (activeValue())
        s |= BobUI::WindowActive;
    return s;
}

void WindowStatesControl::setStates(BobUI::WindowStates s)
{
    stateControl->setState(s);
    setActiveValue(s & BobUI::WindowActive);
}

bool WindowStatesControl::visibleValue() const
{
    return visibleCheckBox && visibleCheckBox->isChecked();
}

void WindowStatesControl::setVisibleValue(bool v)
{
    if (visibleCheckBox) {
        visibleCheckBox->blockSignals(true);
        visibleCheckBox->setChecked(v);
        visibleCheckBox->blockSignals(false);
    }
}

bool WindowStatesControl::activeValue() const
{
    return activeCheckBox && activeCheckBox->isChecked();
}

void WindowStatesControl::setActiveValue(bool v)
{
    if (activeCheckBox) {
        activeCheckBox->blockSignals(true);
        activeCheckBox->setChecked(v);
        activeCheckBox->blockSignals(false);
    }
}

TypeControl::TypeControl(QWidget *parent)
    : QGroupBox(tr("Type"), parent)
    , group(new QButtonGroup)
    , windowRadioButton(new QRadioButton(tr("Window")))
    , dialogRadioButton(new QRadioButton(tr("Dialog")))
    , sheetRadioButton(new QRadioButton(tr("Sheet")))
    , drawerRadioButton(new QRadioButton(tr("Drawer")))
    , popupRadioButton(new QRadioButton(tr("Popup")))
    , toolRadioButton(new QRadioButton(tr("Tool")))
    , toolTipRadioButton(new QRadioButton(tr("Tooltip")))
    , splashScreenRadioButton(new QRadioButton(tr("Splash screen")))
{
    group->setExclusive(true);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(ControlLayoutMargin, ControlLayoutMargin,
                               ControlLayoutMargin, ControlLayoutMargin);
    group->addButton(windowRadioButton, BobUI::Window);
    layout->addWidget(windowRadioButton, 0, 0);
    group->addButton(dialogRadioButton, BobUI::Dialog);
    layout->addWidget(dialogRadioButton, 1, 0);
    group->addButton(sheetRadioButton, BobUI::Sheet);
    layout->addWidget(sheetRadioButton, 2, 0);
    group->addButton(drawerRadioButton, BobUI::Drawer);
    layout->addWidget(drawerRadioButton, 3, 0);
    group->addButton(popupRadioButton, BobUI::Popup);
    layout->addWidget(popupRadioButton, 0, 1);
    group->addButton(toolRadioButton, BobUI::Tool);
    layout->addWidget(toolRadioButton, 1, 1);
    group->addButton(toolTipRadioButton, BobUI::ToolTip);
    layout->addWidget(toolTipRadioButton, 2, 1);
    group->addButton(splashScreenRadioButton, BobUI::SplashScreen);
    layout->addWidget(splashScreenRadioButton, 3, 1);
    connect(group, &QButtonGroup::idReleased, this, &TypeControl::slotChanged);
}

BobUI::WindowFlags TypeControl::type() const
{
    return BobUI::WindowFlags(group->checkedId());
}

void TypeControl::setType(BobUI::WindowFlags s)
{
    if (QAbstractButton *b = group->button(s & BobUI::WindowType_Mask))
        b->setChecked(true);
}

void TypeControl::slotChanged()
{
    emit changed(type());
}
