#ifndef OMNIGAIN_H
#define OMNIGAIN_H

#include <QObject>
#include <QDebug>

class OmniGain : public QObject {
    Q_OBJECT
public:
    explicit OmniGain(QWidget *parent = nullptr);
    virtual ~OmniGain();
};

#endif // OMNIGAIN_H
