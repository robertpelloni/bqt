#include "OmniSlider.h"

OmniSlider::OmniSlider(QWidget *parent) : QSlider(parent) {
    setObjectName("OmniSlider");
    // qDebug() << "Initialized OmniSlider";
}

OmniSlider::~OmniSlider() = default;
