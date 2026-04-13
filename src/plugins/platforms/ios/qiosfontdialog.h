// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSFONTDIALOG_H
#define QIOSFONTDIALOG_H

#include <BobUICore/qeventloop.h>
#include <qpa/qplatformdialoghelper.h>

@interface QIOSFontDialogController : UIFontPickerViewController <UIFontPickerViewControllerDelegate,
                                                                    UIAdaptivePresentationControllerDelegate>
@end

BOBUI_BEGIN_NAMESPACE

class QIOSFontDialog : public QPlatformFontDialogHelper
{
public:
    QIOSFontDialog();
    ~QIOSFontDialog();

    void exec() override;

    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

    void setCurrentFont(const QFont &) override;
    QFont currentFont() const override;

    void updateCurrentFont(const QFont &);

private:
    QEventLoop m_eventLoop;
    QIOSFontDialogController *m_viewController;
    QFont m_currentFont;

};

BOBUI_END_NAMESPACE

#endif // QIOSFONTDIALOG_H
