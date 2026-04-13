// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAMESSAGEDIALOG_H
#define QCOCOAMESSAGEDIALOG_H

#include <qpa/qplatformdialoghelper.h>

Q_FORWARD_DECLARE_OBJC_CLASS(NSAlert);
typedef long NSInteger;
typedef NSInteger NSModalResponse;

BOBUI_BEGIN_NAMESPACE

class QEventLoop;

class QCocoaMessageDialog : public QPlatformMessageDialogHelper
{
public:
    QCocoaMessageDialog() = default;
    ~QCocoaMessageDialog();

    void exec() override;
    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

private:
    BobUI::WindowModality modality() const;
    NSAlert *m_alert = nullptr;
    QEventLoop *m_eventLoop = nullptr;
    NSModalResponse runModal() const;
    void processResponse(NSModalResponse response);
};

BOBUI_END_NAMESPACE

#endif // QCOCOAMESSAGEDIALOG_H

