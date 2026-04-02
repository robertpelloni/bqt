#include "OmniProgressBar.h"

OmniProgressBar::OmniProgressBar(QWidget *parent) : QProgressBar(parent) {
    setObjectName("OmniProgressBar");
    // qDebug() << "Initialized OmniProgressBar";
}

OmniProgressBar::~OmniProgressBar() = default;
