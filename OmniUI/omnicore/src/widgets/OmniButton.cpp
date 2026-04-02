#include "OmniButton.h"

OmniButton::OmniButton(QWidget *parent) : QPushButton(parent) {
    setObjectName("OmniButton");
    // qDebug() << "Initialized OmniButton";
}

OmniButton::~OmniButton() = default;
