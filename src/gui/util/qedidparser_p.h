// Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// Copyright (C) 2021 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QEDIDPARSER_P_H
#define QEDIDPARSER_P_H

#include <BobUICore/QMap>
#include <BobUICore/QPointF>
#include <BobUICore/QSize>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qsize.h>
#include <BobUICore/qstring.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QEdidParser
{
public:
    bool parse(const QByteArray &blob);

    QString identifier;
    QString manufacturer;
    QString model;
    QString serialNumber;
    QSizeF physicalSize;
    qreal gamma;
    QPointF redChromaticity;
    QPointF greenChromaticity;
    QPointF blueChromaticity;
    QPointF whiteChromaticity;
    QList<QList<uint16_t>> tables;
    bool sRgb;
    bool useTables;

private:
    QString parseEdidString(const quint8 *data);
};

BOBUI_END_NAMESPACE

#endif // QEDIDPARSER_P_H
