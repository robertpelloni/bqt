// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>
#include <BobUITest/bobuiesteventloop.h>

#include <BobUIWidgets/qprogressdialog.h>

#include <BobUINetwork/qnetworkaccessmanager.h>
#include <BobUINetwork/qnetworkrequest.h>
#include <BobUINetwork/qnetworkreply.h>

#include <BobUICore/qdebug.h>

#include "../../network-settings.h"


class tst_QNetworkAccessManager_And_QProgressDialog : public QObject
{
    Q_OBJECT
public:
    tst_QNetworkAccessManager_And_QProgressDialog();
private slots:
    void initTestCase();
    void downloadCheck();
    void downloadCheck_data();
};

class DownloadCheckWidget : public QWidget
{
    Q_OBJECT
public:
    DownloadCheckWidget(QWidget *parent = nullptr) :
        QWidget(parent), lateReadyRead(true), zeroCopy(false),
        progressDlg(this), netmanager(this)
    {
        progressDlg.setRange(1, 100);
        QMetaObject::invokeMethod(this, "go", BobUI::QueuedConnection);
    }
    bool lateReadyRead;
    bool zeroCopy;
public slots:
    void go()
    {
        QNetworkRequest request(QUrl("http://" + BobUINetworkSettings::httpServerName() + "/bobuiest/bigfile"));
        if (zeroCopy)
            request.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, 10*1024*1024);

        QNetworkReply *reply = netmanager.get(
                QNetworkRequest(
                QUrl("http://" + BobUINetworkSettings::httpServerName() + "/bobuiest/bigfile")
                ));
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(dataReadProgress(qint64,qint64)));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(dataReadyRead()));
        connect(reply, SIGNAL(finished()), this, SLOT(finishedFromReply()));

        progressDlg.exec();
    }
    void dataReadProgress(qint64 done, qint64 total)
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        Q_UNUSED(reply);
        progressDlg.setMaximum(total);
        progressDlg.setValue(done);
    }
    void dataReadyRead()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        Q_UNUSED(reply);
        lateReadyRead = true;
    }
    void finishedFromReply()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        lateReadyRead = false;
        reply->deleteLater();
        BOBUIestEventLoop::instance().exitLoop();
    }

private:
    QProgressDialog progressDlg;
    QNetworkAccessManager netmanager;
};


tst_QNetworkAccessManager_And_QProgressDialog::tst_QNetworkAccessManager_And_QProgressDialog()
{
}

void tst_QNetworkAccessManager_And_QProgressDialog::initTestCase()
{
#ifdef BOBUI_TEST_SERVER
    if (!BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpServerName(), 80))
        QSKIP("No network test server available");
#else
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
#endif
}

void tst_QNetworkAccessManager_And_QProgressDialog::downloadCheck_data()
{
    BOBUIest::addColumn<bool>("useZeroCopy");
    BOBUIest::newRow("with-zeroCopy") << true;
    BOBUIest::newRow("without-zeroCopy") << false;
}

void tst_QNetworkAccessManager_And_QProgressDialog::downloadCheck()
{
    QFETCH(bool, useZeroCopy);

    DownloadCheckWidget widget;
    widget.zeroCopy = useZeroCopy;
    widget.show();
    // run and exit on finished()
    BOBUIestEventLoop::instance().enterLoop(10);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    // run some more to catch the late readyRead() (or: to not catch it)
    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(BOBUIestEventLoop::instance().timeout());
    // the following fails when a readyRead() was received after finished()
    QVERIFY(!widget.lateReadyRead);
}



BOBUIEST_MAIN(tst_QNetworkAccessManager_And_QProgressDialog)
#include "tst_qnetworkaccessmanager_and_qprogressdialog.moc"
