// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSCOLORDIALOG_H
#define QIOSCOLORDIALOG_H

#include <BobUICore/qeventloop.h>
#include <qpa/qplatformdialoghelper.h>

Q_FORWARD_DECLARE_OBJC_CLASS(QIOSColorDialogController);

BOBUI_BEGIN_NAMESPACE

class QIOSColorDialog : public QPlatformColorDialogHelper
{
public:
    QIOSColorDialog();
    ~QIOSColorDialog();

    void exec() override;
    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

    void setCurrentColor(const QColor&) override;
    QColor currentColor() const override;

    void updateColor(const QColor&);

private:
    QEventLoop m_eventLoop;
    QIOSColorDialogController *m_viewController;
    QColor m_currentColor;
};

BOBUI_END_NAMESPACE

#endif // QIOSCOLORDIALOG_H

