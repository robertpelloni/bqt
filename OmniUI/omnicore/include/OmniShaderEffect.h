#ifndef OMNISHADEREFFECT_H
#define OMNISHADEREFFECT_H

#include <QWidget>
#include <QDebug>

class OmniShaderEffect : public QWidget {
    Q_OBJECT
public:
    explicit OmniShaderEffect(QWidget *parent = nullptr);
    virtual ~OmniShaderEffect();
};

#endif // OMNISHADEREFFECT_H
