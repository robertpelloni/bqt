#include "OmniHttpClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>

OmniHttpClient::OmniHttpClient(QObject *parent)
    : QObject(parent)
{
    setObjectName("OmniHttpClient");
    connect(&m_manager, &QNetworkAccessManager::finished, this, &OmniHttpClient::onReplyFinished);
}

OmniHttpClient::~OmniHttpClient() = default;

void OmniHttpClient::get(const QString& urlStr, const QVariantMap& headers) {
    QUrl url(urlStr);
    QNetworkRequest request(url);

    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
    }

    qDebug() << "OmniHttpClient: GET" << urlStr;
    m_manager.get(request);
}

void OmniHttpClient::post(const QString& urlStr, const QString& payload, const QVariantMap& headers) {
    QUrl url(urlStr);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // Default
    
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
    }

    qDebug() << "OmniHttpClient: POST" << urlStr;
    m_manager.post(request, payload.toUtf8());
}

void OmniHttpClient::onReplyFinished(QNetworkReply* reply) {
    reply->deleteLater();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError) {
        QString responseBody = QString::fromUtf8(reply->readAll());
        emit responseReceived(statusCode, responseBody);
    } else {
        QString errorString = reply->errorString();
        qWarning() << "OmniHttpClient Error:" << errorString;
        emit errorOccurred(errorString);
    }
}
