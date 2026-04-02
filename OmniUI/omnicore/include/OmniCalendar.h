#ifndef OMNICALENDAR_H
#define OMNICALENDAR_H

#include <QQuickItem>
#include <QDebug>

class OmniCalendar : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniCalendar(QQuickItem *parent = nullptr);
    virtual ~OmniCalendar();
};

#endif // OMNICALENDAR_H
