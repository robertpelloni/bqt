#include "OmniCalendar.h"

OmniCalendar::OmniCalendar(QWidget *parent) : QCalendarWidget(parent) {
    setObjectName("OmniCalendar");
    // qDebug() << "Initialized OmniCalendar";
}

OmniCalendar::~OmniCalendar() = default;
