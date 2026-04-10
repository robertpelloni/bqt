// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAFILEDIALOGHELPER_H
#define QCOCOAFILEDIALOGHELPER_H

#include <QObject>
#include <qpa/qplatformdialoghelper.h>
#include <BobUICore/private/qcore_mac_p.h>

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QNSOpenSavePanelDelegate, NSObject<NSOpenSavePanelDelegate>)

BOBUI_BEGIN_NAMESPACE

class QEventLoop;
class QFileDialog;
class QFileDialogPrivate;

class QCocoaFileDialogHelper : public QPlatformFileDialogHelper
{
public:
    QCocoaFileDialogHelper();
    virtual ~QCocoaFileDialogHelper();

    void exec() override;

    bool defaultNameFilterDisables() const override;

    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality, QWindow *parent) override;
    void hide() override;
    void setDirectory(const QUrl &directory) override;
    QUrl directory() const override;
    void selectFile(const QUrl &filename) override;
    QList<QUrl> selectedFiles() const override;
    void setFilter() override;
    void selectNameFilter(const QString &filter) override;
    QString selectedNameFilter() const override;

public: // for QNSOpenSavePanelDelegate
    void panelClosed(NSInteger result);
    void panelDirectoryDidChange(NSString *path);

private:
    void createNSOpenSavePanelDelegate();

    QNSOpenSavePanelDelegate *m_delegate = nil;
    QUrl m_directory;
    QEventLoop *m_eventLoop = nullptr;
};

BOBUI_END_NAMESPACE

#endif // QCOCOAFILEDIALOGHELPER_H
