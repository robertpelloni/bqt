#ifndef OMNIPARTICLES_H
#define OMNIPARTICLES_H

#include <QQuickItem>
#include <QDebug>

class OmniParticles : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniParticles(QQuickItem *parent = nullptr);
    virtual ~OmniParticles();
};

#endif // OMNIPARTICLES_H
