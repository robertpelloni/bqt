#include "OmniHttpClient.h"

OmniHttpClient::OmniHttpClient(QObject *parent) : QObject(parent) {
    setObjectName("OmniHttpClient");
    // qDebug() << "Initialized OmniHttpClient";
}

OmniHttpClient::~OmniHttpClient() = default;
