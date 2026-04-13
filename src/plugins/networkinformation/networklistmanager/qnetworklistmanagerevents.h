// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKLISTMANAGEREVENTS_H
#define QNETWORKLISTMANAGEREVENTS_H

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUINetwork/qnetworkinformation.h>

#include <BobUICore/qstring.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qmutex.h>

#include <objbase.h>
#include <ocidl.h>
#include <netlistmgr.h>
#include <BobUICore/private/qcomptr_p.h>
#include <wrl/wrappers/corewrappers.h>

#if BOBUI_CONFIG(cpp_winrt)
#include <BobUICore/private/bobui_winrtbase_p.h>
#endif

using namespace Microsoft::WRL;

BOBUI_BEGIN_NAMESPACE
Q_DECLARE_LOGGING_CATEGORY(lcNetInfoNLM)

class QNetworkListManagerEvents : public QObject, public INetworkListManagerEvents
{
    Q_OBJECT
public:
    QNetworkListManagerEvents();
    virtual ~QNetworkListManagerEvents();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;

    ULONG STDMETHODCALLTYPE AddRef() override { return ++ref; }
    ULONG STDMETHODCALLTYPE Release() override
    {
        if (--ref == 0) {
            delete this;
            return 0;
        }
        return ref;
    }

    HRESULT STDMETHODCALLTYPE ConnectivityChanged(NLM_CONNECTIVITY newConnectivity) override;

    [[nodiscard]] bool start();
    void stop();

    [[nodiscard]] bool checkBehindCaptivePortal();

signals:
    void connectivityChanged(NLM_CONNECTIVITY);
    void transportMediumChanged(QNetworkInformation::TransportMedium);
    void isMeteredChanged(bool);

private:
    ComPtr<INetworkListManager> networkListManager = nullptr;
    ComPtr<IConnectionPoint> connectionPoint = nullptr;

#if BOBUI_CONFIG(cpp_winrt)
    void emitWinRTUpdates();

    winrt::event_token token;
    QMutex winrtLock;
#endif

    QAtomicInteger<ULONG> ref = 0;
    DWORD cookie = 0;
};

BOBUI_END_NAMESPACE

#endif // QNETWORKLISTMANAGEREVENTS_H
