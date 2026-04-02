#ifndef OMNIDOCKAREA_H
#define OMNIDOCKAREA_H

#include <QQuickItem>
#include <QDebug>

class OmniDockArea : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniDockArea(QQuickItem *parent = nullptr);
    virtual ~OmniDockArea();
};

#endif // OMNIDOCKAREA_H
