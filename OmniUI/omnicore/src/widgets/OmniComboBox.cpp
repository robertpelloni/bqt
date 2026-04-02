#include "OmniComboBox.h"

OmniComboBox::OmniComboBox(QWidget *parent) : QComboBox(parent) {
    setObjectName("OmniComboBox");
    // qDebug() << "Initialized OmniComboBox";
}

OmniComboBox::~OmniComboBox() = default;
