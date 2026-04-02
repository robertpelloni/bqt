#ifndef OMNIHTTPCLIENT_H
#define OMNIHTTPCLIENT_H

#include <QObject>
#include <QDebug>

class OmniHttpClient : public QObject {
    Q_OBJECT
public:
    explicit OmniHttpClient(QQuickItem *parent = nullptr);
    virtual ~OmniHttpClient();
};

#endif // OMNIHTTPCLIENT_H
