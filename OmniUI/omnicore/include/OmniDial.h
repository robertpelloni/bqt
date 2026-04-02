#ifndef OMNIDIAL_H
#define OMNIDIAL_H

#include <QDial>
#include <QDebug>

class OmniDial : public QDial {
    Q_OBJECT
public:
    explicit OmniDial(QWidget *parent = nullptr);
    virtual ~OmniDial();
};

#endif // OMNIDIAL_H
