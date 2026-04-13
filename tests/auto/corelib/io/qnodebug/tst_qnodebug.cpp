// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// This test is for "release" mode, with -DBOBUI_NO_DEBUG -DBOBUI_NO_DEBUG_OUTPUT
#ifndef BOBUI_NO_DEBUG
#define BOBUI_NO_DEBUG
#endif
#ifndef BOBUI_NO_DEBUG_OUTPUT
#define BOBUI_NO_DEBUG_OUTPUT
#endif

#include <BobUICore/BobUICore>
#include <BobUICore/BobUIDebug>
#include <BobUICore/QLoggingCategory>
#include <BOBUIest>

class tst_QNoDebug: public QObject
{
    Q_OBJECT
private slots:
    void noDebugOutput() const;
    void streaming() const;
};

void tst_QNoDebug::noDebugOutput() const
{
    QLoggingCategory cat("custom");
    // should do nothing
    qDebug() << "foo";
    qCDebug(cat) << "foo";
    qCDebug(cat, "foo");

    // qWarning still works, though
    BOBUIest::ignoreMessage(BobUIWarningMsg, "bar");
    BOBUIest::ignoreMessage(BobUIWarningMsg, "custom-bar");
    qWarning() << "bar";
    qCWarning(cat) << "custom-bar";
}

void tst_QNoDebug::streaming() const
{
    QDateTime dt(QDate(1,2,3),BOBUIime(4,5,6));
    const QByteArray debugString = dt.toString(u"yyyy-MM-dd HH:mm:ss.zzz t").toLocal8Bit();
    const QByteArray message = "QDateTime(" + debugString + " BobUI::LocalTime)";
    BOBUIest::ignoreMessage(BobUIWarningMsg, message.constData());
    qWarning() << dt;
}

BOBUIEST_MAIN(tst_QNoDebug);
#include "tst_qnodebug.moc"
