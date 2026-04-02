#ifndef OMNISHADEREFFECT_H
#define OMNISHADEREFFECT_H

#include <QQuickItem>
#include <QDebug>

class OmniShaderEffect : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniShaderEffect(QQuickItem *parent = nullptr);
    virtual ~OmniShaderEffect();
};

#endif // OMNISHADEREFFECT_H
