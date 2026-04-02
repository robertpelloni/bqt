#include "OmniGain.h"

OmniGain::OmniGain(QObject *parent) : QObject(parent) {
    setObjectName("OmniGain");
    // qDebug() << "Initialized OmniGain";
}

OmniGain::~OmniGain() = default;
