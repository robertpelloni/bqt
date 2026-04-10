// Copyright (C) 2022 Laszlo Papp <lpapp@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUICore/qglobal.h>

#if defined(Q_OS_WIN)

#include <windows.h>
#include <QByteArray>
#include <QAbstractNativeEventFilter>

//! [0]
class MyMSGEventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *) override
    {
        if (eventType == "windows_generic_MSG") {
            MSG *msg = static_cast<MSG *>(message);
            // ...
        } else if (eventType == "windows_dispatcher_MSG") {
            MSG *msg = static_cast<MSG *>(message);
            // ...
        }
        return false;
    }
};
//! [0]
#endif
