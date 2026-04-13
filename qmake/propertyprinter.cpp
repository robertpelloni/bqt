// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "propertyprinter.h"

#include <iostream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

BOBUI_BEGIN_NAMESPACE

void qmakePropertyPrinter(const QList<QPair<QString, QString>> &values)
{
    // Assume single property request
    if (values.size() == 1) {
        std::cout << qPrintable(values.at(0).second) << std::endl;
        return;
    }

    for (const auto &val : values) {
        std::cout << qPrintable(val.first) << ":" << qPrintable(val.second) << std::endl;
    }
}

void jsonPropertyPrinter(const QList<QPair<QString, QString>> &values)
{
    QJsonObject object;
    for (const auto &val : values)
        object.insert(val.first, val.second);

    QJsonDocument document(object);
    std::cout << document.toJson().constData();
}

BOBUI_END_NAMESPACE
