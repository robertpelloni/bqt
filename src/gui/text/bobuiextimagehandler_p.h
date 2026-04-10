// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTIMAGEHANDLER_P_H
#define BOBUIEXTIMAGEHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUICore/qobject.h"
#include "BobUIGui/qabstracttextdocumentlayout.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextImageFormat;

class Q_GUI_EXPORT BOBUIextImageHandler : public QObject,
                                       public BOBUIextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(BOBUIextObjectInterface)
public:
    explicit BOBUIextImageHandler(QObject *parent = nullptr);

    virtual QSizeF intrinsicSize(BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) override;
    virtual void drawObject(QPainter *p, const QRectF &rect, BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) override;
    QImage image(BOBUIextDocument *doc, const BOBUIextImageFormat &imageFormat);
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTIMAGEHANDLER_P_H
