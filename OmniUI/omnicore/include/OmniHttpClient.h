#ifndef OMNIHTTPCLIENT_H
#define OMNIHTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QVariantMap>

class OmniHttpClient : public QObject {
    Q_OBJECT

public:
    explicit OmniHttpClient(QObject *parent = nullptr);
    virtual ~OmniHttpClient();

    // Async requests exposed to QML
    Q_INVOKABLE void get(const QString& url, const QVariantMap& headers = QVariantMap());
    Q_INVOKABLE void post(const QString& url, const QString& payload, const QVariantMap& headers = QVariantMap());

signals:
    // Emitted when a request completes
    void responseReceived(int statusCode, const QString& responseBody);
    void errorOccurred(const QString& errorString);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
};

#endif // OMNIHTTPCLIENT_H
