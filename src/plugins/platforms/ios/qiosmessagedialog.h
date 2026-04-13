// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSMESSAGEDIALOG_H
#define QIOSMESSAGEDIALOG_H

#include <BobUICore/qeventloop.h>
#include <qpa/qplatformdialoghelper.h>

Q_FORWARD_DECLARE_OBJC_CLASS(UIAlertController);
Q_FORWARD_DECLARE_OBJC_CLASS(UIAlertAction);

BOBUI_BEGIN_NAMESPACE

class QIOSMessageDialog : public QPlatformMessageDialogHelper
{
public:
    QIOSMessageDialog();
    ~QIOSMessageDialog();

    void exec() override;
    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

private:
    QEventLoop m_eventLoop;
    UIAlertController *m_alertController;
    QString messageTextPlain();
    UIAlertAction *createAction(StandardButton button);
    UIAlertAction *createAction(const QMessageDialogOptions::CustomButton &customButton);
};

BOBUI_END_NAMESPACE

#endif // QIOSMESSAGEDIALOG_H

