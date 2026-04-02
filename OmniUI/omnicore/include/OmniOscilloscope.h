#ifndef OMNIOSCILLOSCOPE_H
#define OMNIOSCILLOSCOPE_H

#include <QWidget>
#include <QDebug>

class OmniOscilloscope : public QWidget {
    Q_OBJECT
public:
    explicit OmniOscilloscope(QWidget *parent = nullptr);
    virtual ~OmniOscilloscope();
};

#endif // OMNIOSCILLOSCOPE_H
