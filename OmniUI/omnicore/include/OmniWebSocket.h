#ifndef OMNIWEBSOCKET_H
#define OMNIWEBSOCKET_H

#include <QObject>
#include <QDebug>

class OmniWebSocket : public QObject {
    Q_OBJECT
public:
    explicit OmniWebSocket(QWidget *parent = nullptr);
    virtual ~OmniWebSocket();
};

#endif // OMNIWEBSOCKET_H
