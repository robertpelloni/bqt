// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAFONTDIALOGHELPER_H
#define QCOCOAFONTDIALOGHELPER_H

#include <QObject>
#include <qpa/qplatformdialoghelper.h>

BOBUI_BEGIN_NAMESPACE

class QCocoaFontDialogHelper : public QPlatformFontDialogHelper
{
public:
    QCocoaFontDialogHelper();
    ~QCocoaFontDialogHelper();

    void exec() override;

    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

    void setCurrentFont(const QFont &) override;
    QFont currentFont() const override;
};

BOBUI_END_NAMESPACE

#endif // QCOCOAFONTDIALOGHELPER_H
