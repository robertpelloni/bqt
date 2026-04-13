// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "previewwindow.h"

#include <QPushButton>
#include <BOBUIextEdit>
#include <QVBoxLayout>

//! [0]
PreviewWindow::PreviewWindow(QWidget *parent)
    : QWidget(parent)
{
    textEdit = new BOBUIextEdit;
    textEdit->setReadOnly(true);
    textEdit->setLineWrapMode(BOBUIextEdit::NoWrap);

    closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, &QPushButton::clicked,
            this, &PreviewWindow::close);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(closeButton);
    setLayout(layout);

    setWindowTitle(tr("Preview"));
}
//! [0]

//! [1]
void PreviewWindow::setWindowFlags(BobUI::WindowFlags flags)
{
    QWidget::setWindowFlags(flags);

    QString text;

    BobUI::WindowFlags type = (flags & BobUI::WindowType_Mask);
    if (type == BobUI::Window)
        text = "BobUI::Window";
    else if (type == BobUI::Dialog)
        text = "BobUI::Dialog";
    else if (type == BobUI::Sheet)
        text = "BobUI::Sheet";
    else if (type == BobUI::Drawer)
        text = "BobUI::Drawer";
    else if (type == BobUI::Popup)
        text = "BobUI::Popup";
    else if (type == BobUI::Tool)
        text = "BobUI::Tool";
    else if (type == BobUI::ToolTip)
        text = "BobUI::ToolTip";
    else if (type == BobUI::SplashScreen)
        text = "BobUI::SplashScreen";

    if (flags & BobUI::MSWindowsFixedSizeDialogHint)
        text += "\n| BobUI::MSWindowsFixedSizeDialogHint";
    if (flags & BobUI::X11BypassWindowManagerHint)
        text += "\n| BobUI::X11BypassWindowManagerHint";
    if (flags & BobUI::FramelessWindowHint)
        text += "\n| BobUI::FramelessWindowHint";
    if (flags & BobUI::NoDropShadowWindowHint)
        text += "\n| BobUI::NoDropShadowWindowHint";
    if (flags & BobUI::WindowTitleHint)
        text += "\n| BobUI::WindowTitleHint";
    if (flags & BobUI::WindowSystemMenuHint)
        text += "\n| BobUI::WindowSystemMenuHint";
    if (flags & BobUI::WindowMinimizeButtonHint)
        text += "\n| BobUI::WindowMinimizeButtonHint";
    if (flags & BobUI::WindowMaximizeButtonHint)
        text += "\n| BobUI::WindowMaximizeButtonHint";
    if (flags & BobUI::WindowCloseButtonHint)
        text += "\n| BobUI::WindowCloseButtonHint";
    if (flags & BobUI::WindowContextHelpButtonHint)
        text += "\n| BobUI::WindowContextHelpButtonHint";
    if (flags & BobUI::WindowShadeButtonHint)
        text += "\n| BobUI::WindowShadeButtonHint";
    if (flags & BobUI::WindowStaysOnTopHint)
        text += "\n| BobUI::WindowStaysOnTopHint";
    if (flags & BobUI::WindowStaysOnBottomHint)
        text += "\n| BobUI::WindowStaysOnBottomHint";
    if (flags & BobUI::CustomizeWindowHint)
        text += "\n| BobUI::CustomizeWindowHint";

    textEdit->setPlainText(text);
}
//! [1]
