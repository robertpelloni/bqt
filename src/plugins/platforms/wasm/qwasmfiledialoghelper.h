// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMFILEDIALOGHELPER_H
#define QWASMFILEDIALOGHELPER_H

#include <BobUICore/QObject>
#include <BobUICore/QUrl>
#include <BobUICore/QEventLoop>
#include <BobUIGui/qpa/qplatformdialoghelper.h>
#include <BobUIGui/private/qwasmlocalfileaccess_p.h>

BOBUI_BEGIN_NAMESPACE

class QWasmFileDialogHelper : public QPlatformFileDialogHelper
{
    Q_OBJECT
public:
    QWasmFileDialogHelper();
    ~QWasmFileDialogHelper();
public:
    virtual void exec() override;
    virtual bool show(BobUI::WindowFlags windowFlags,
                          BobUI::WindowModality windowModality,
                          QWindow *parent) override;
    virtual void hide() override;
    virtual bool defaultNameFilterDisables() const override;
    virtual void setDirectory(const QUrl &directory) override;
    virtual QUrl directory() const override;
    virtual void selectFile(const QUrl &filename) override;
    virtual QList<QUrl> selectedFiles() const override;
    virtual void setFilter() override;
    virtual void selectNameFilter(const QString &filter) override;
    virtual QString selectedNameFilter() const override;
    static QStringList cleanFilterList(const QString &filter);
signals:
    void fileDone(const QUrl &);
private:
    void showFileDialog();
    void onOpenDialogClosed(bool accepted, std::vector<qstdweb::File> files);
    void onSaveDialogClosed(bool accepted, qstdweb::FileSystemFileHandle file);

    QList<QUrl> m_selectedFiles;
    QEventLoop *m_eventLoop;
};

BOBUI_END_NAMESPACE

#endif // QWASMFILEDIALOGHELPER_H
