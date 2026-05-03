// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QUrl>

#include <memory>

BOBUI_BEGIN_NAMESPACE
class QFile;
class QLabel;
class QLineEdit;
class QPushButton;
class QSslError;
class QAuthenticator;
class QNetworkReply;
class QCheckBox;
#if BOBUI_CONFIG(networkproxy)
class QNetworkProxy;
#endif

BOBUI_END_NAMESPACE

class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent = nullptr);

public slots:
   void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};

class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HttpWindow(QWidget *parent = nullptr);
    ~HttpWindow();

    void startRequest(const QUrl &requestedUrl);

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void enableDownloadButton();
    void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator);
#if BOBUI_CONFIG(ssl)
    void sslErrors(const QList<QSslError> &errors);
#endif
#if BOBUI_CONFIG(networkproxy)
    void slotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
#endif

private:
    std::unique_ptr<QFile> openFileForWrite(const QString &fileName);

    QLabel *statusLabel;
    QLineEdit *urlLineEdit;
    QPushButton *downloadButton;
    QCheckBox *launchCheckBox;
    QLineEdit *defaultFileLineEdit;
    QLineEdit *downloadDirectoryLineEdit;

    QUrl url;
    QNetworkAccessManager qnam;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;
    std::unique_ptr<QFile> file;
    bool httpRequestAborted = false;
};

#endif
