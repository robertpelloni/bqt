#ifndef OMNICALENDAR_H
#define OMNICALENDAR_H

#include <QCalendarWidget>
#include <QDebug>

class OmniCalendar : public QCalendarWidget {
    Q_OBJECT
public:
    explicit OmniCalendar(QWidget *parent = nullptr);
    virtual ~OmniCalendar();
};

#endif // OMNICALENDAR_H
