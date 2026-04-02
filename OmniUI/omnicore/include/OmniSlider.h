#ifndef OMNISLIDER_H
#define OMNISLIDER_H

#include <QSlider>
#include <QDebug>

class OmniSlider : public QSlider {
    Q_OBJECT
public:
    explicit OmniSlider(QWidget *parent = nullptr);
    virtual ~OmniSlider();
};

#endif // OMNISLIDER_H
