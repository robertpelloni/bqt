// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOACOLORDIALOGHELPER_H
#define QCOCOACOLORDIALOGHELPER_H

#include <QObject>
#include <qpa/qplatformdialoghelper.h>

BOBUI_BEGIN_NAMESPACE

class QCocoaColorDialogHelper : public QPlatformColorDialogHelper
{
public:
    QCocoaColorDialogHelper();
    ~QCocoaColorDialogHelper();

    void exec() override;
    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

    void setCurrentColor(const QColor&) override;
    QColor currentColor() const override;
};

BOBUI_END_NAMESPACE

#endif // QCOCOACOLORDIALOGHELPER_H
