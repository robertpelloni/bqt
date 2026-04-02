#ifndef OMNIPLUGINMANAGER_H
#define OMNIPLUGINMANAGER_H

#include <QObject>
#include <QDebug>

class OmniPluginManager : public QObject {
    Q_OBJECT
public:
    explicit OmniPluginManager(QQuickItem *parent = nullptr);
    virtual ~OmniPluginManager();
};

#endif // OMNIPLUGINMANAGER_H
