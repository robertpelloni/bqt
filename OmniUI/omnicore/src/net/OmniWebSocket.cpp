#include "OmniWebSocket.h"

OmniWebSocket::OmniWebSocket(QObject *parent) : QObject(parent) {
    setObjectName("OmniWebSocket");
    // qDebug() << "Initialized OmniWebSocket";
}

OmniWebSocket::~OmniWebSocket() = default;
