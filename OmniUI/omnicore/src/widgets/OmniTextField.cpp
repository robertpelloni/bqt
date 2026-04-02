#include "OmniTextField.h"

OmniTextField::OmniTextField(QWidget *parent) : QLineEdit(parent) {
    setObjectName("OmniTextField");
    // qDebug() << "Initialized OmniTextField";
}

OmniTextField::~OmniTextField() = default;
