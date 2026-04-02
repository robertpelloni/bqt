#include "OmniCheckBox.h"

OmniCheckBox::OmniCheckBox(QWidget *parent) : QCheckBox(parent) {
    setObjectName("OmniCheckBox");
    // qDebug() << "Initialized OmniCheckBox";
}

OmniCheckBox::~OmniCheckBox() = default;
