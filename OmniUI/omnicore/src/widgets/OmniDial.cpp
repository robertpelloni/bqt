#include "OmniDial.h"

OmniDial::OmniDial(QWidget *parent) : QDial(parent) {
    setObjectName("OmniDial");
    // qDebug() << "Initialized OmniDial";
}

OmniDial::~OmniDial() = default;
