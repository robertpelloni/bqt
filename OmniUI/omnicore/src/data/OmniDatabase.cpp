#include "OmniDatabase.h"

OmniDatabase::OmniDatabase(QObject *parent) : QObject(parent) {
    setObjectName("OmniDatabase");
    // qDebug() << "Initialized OmniDatabase";
}

OmniDatabase::~OmniDatabase() = default;
