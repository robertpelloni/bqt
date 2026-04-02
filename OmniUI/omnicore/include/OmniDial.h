#ifndef OMNIDIAL_H
#define OMNIDIAL_H

#include <QQuickItem>
#include <QDebug>

class OmniDial : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniDial(QQuickItem *parent = nullptr);
    virtual ~OmniDial();
};

#endif // OMNIDIAL_H
