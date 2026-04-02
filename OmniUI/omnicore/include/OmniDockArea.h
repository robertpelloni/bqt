#ifndef OMNIDOCKAREA_H
#define OMNIDOCKAREA_H

#include <QWidget>
#include <QDebug>

class OmniDockArea : public QWidget {
    Q_OBJECT
public:
    explicit OmniDockArea(QWidget *parent = nullptr);
    virtual ~OmniDockArea();
};

#endif // OMNIDOCKAREA_H
