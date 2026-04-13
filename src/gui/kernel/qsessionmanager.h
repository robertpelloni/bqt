// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSESSIONMANAGER_H
#define QSESSIONMANAGER_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringlist.h>

#ifndef BOBUI_NO_SESSIONMANAGER

BOBUI_BEGIN_NAMESPACE


class QGuiApplication;

class QSessionManagerPrivate;

class Q_GUI_EXPORT  QSessionManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSessionManager)
    QSessionManager(QGuiApplication *app, QString &id, QString &key);
    ~QSessionManager();
public:
    QString sessionId() const;
    QString sessionKey() const;

    bool allowsInteraction();
    bool allowsErrorInteraction();
    void release();

    void cancel();

    enum RestartHint {
        RestartIfRunning,
        RestartAnyway,
        RestartImmediately,
        RestartNever
    };
    void setRestartHint(RestartHint);
    RestartHint restartHint() const;

    void setRestartCommand(const QStringList&);
    QStringList restartCommand() const;
    void setDiscardCommand(const QStringList&);
    QStringList discardCommand() const;

    void setManagerProperty(const QString& name, const QString& value);
    void setManagerProperty(const QString& name, const QStringList& value);

    bool isPhase2() const;
    void requestPhase2();

private:
    friend class QGuiApplication;
    friend class QGuiApplicationPrivate;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_SESSIONMANAGER

#endif // QSESSIONMANAGER_H
