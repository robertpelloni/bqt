#ifndef OMNIPARTICLES_H
#define OMNIPARTICLES_H

#include <QWidget>
#include <QDebug>

class OmniParticles : public QWidget {
    Q_OBJECT
public:
    explicit OmniParticles(QWidget *parent = nullptr);
    virtual ~OmniParticles();
};

#endif // OMNIPARTICLES_H
