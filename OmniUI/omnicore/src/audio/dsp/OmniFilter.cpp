#include "OmniFilter.h"

OmniFilter::OmniFilter(QObject *parent) : QObject(parent) {
    setObjectName("OmniFilter");
    // qDebug() << "Initialized OmniFilter";
}

OmniFilter::~OmniFilter() = default;
