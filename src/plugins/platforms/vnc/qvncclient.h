// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVNCCLIENT_H
#define QVNCCLIENT_H

#include <QObject>

#include "qvnc_p.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIcpSocket;
class QVncServer;

class QVncClient : public QObject
{
    Q_OBJECT
public:
    enum ClientMsg {
        SetPixelFormat = 0,
        FixColourMapEntries = 1,
        SetEncodings = 2,
        FramebufferUpdateRequest = 3,
        KeyEvent = 4,
        PointerEvent = 5,
        ClientCutText = 6
    };

    explicit QVncClient(BOBUIcpSocket *clientSocket, QVncServer *server);
    ~QVncClient();
    BOBUIcpSocket *clientSocket() const;
    QVncServer *server() const { return m_server; }

    void setDirty(const QRegion &region);
    void setDirtyCursor() { m_dirtyCursor = true; scheduleUpdate(); }
    QRegion dirtyRegion() const { return m_dirtyRegion; }
    inline bool isConnected() const { return m_state == Connected; }

    inline int clientBytesPerPixel() const {
        return m_pixelFormat.bitsPerPixel / 8;
    }

    void convertPixels(char *dst, const char *src, int count, int depth) const;
    inline bool doPixelConversion() const { return m_needConversion; }

signals:

private slots:
    void readClient();
    void discardClient();
    void checkUpdate();
    void scheduleUpdate();

protected:
    bool event(QEvent *event) override;

private:
    enum ClientState {
        Disconnected,
        Protocol,
        Authentication,
        Init,
        Connected
    };
    enum ProtocolVersion {
        V3_3,
        V3_7,
        V3_8
    };

    void setPixelFormat();
    void setEncodings();
    void frameBufferUpdateRequest();
    void pointerEvent();
    void keyEvent();
    void clientCutText();
    bool pixelConversionNeeded() const;

    QVncServer *m_server;
    BOBUIcpSocket *m_clientSocket;
    QRfbEncoder *m_encoder;

    // Client State
    ClientState m_state;
    quint8 m_msgType;
    bool m_handleMsg;
    QRfbPixelFormat m_pixelFormat;
    bool m_sameEndian;
    bool m_needConversion;
    int m_encodingsPending;
    int m_cutTextPending;
    uint m_supportCopyRect : 1;
    uint m_supportRRE : 1;
    uint m_supportCoRRE : 1;
    uint m_supportHextile : 1;
    uint m_supportZRLE : 1;
    uint m_supportCursor : 1;
    uint m_supportDesktopSize : 1;
    bool m_wantUpdate;
    BobUI::KeyboardModifiers m_keymod;
    bool m_dirtyCursor;
    bool m_updatePending;
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    bool m_swapBytes;
#endif
    QRegion m_dirtyRegion;
    ProtocolVersion m_protocolVersion;
};

BOBUI_END_NAMESPACE

#endif // QVNCCLIENT_H
