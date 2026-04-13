// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGLYPHRUN_P_H
#define QGLYPHRUN_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qshareddata.h>
#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "qglyphrun.h"
#include "qrawfont.h"

#include <qfont.h>

#if !defined(BOBUI_NO_RAWFONT)

BOBUI_BEGIN_NAMESPACE

class QGlyphRunPrivate: public QSharedData
{
public:
    QGlyphRunPrivate()
        : glyphIndexData(glyphIndexes.constData())
        , glyphIndexDataSize(0)
        , glyphPositionData(glyphPositions.constData())
        , glyphPositionDataSize(0)
        , textRangeStart(-1)
        , textRangeEnd(-1)
    {
    }

    QGlyphRunPrivate(const QGlyphRunPrivate &other)
      : QSharedData(other)
      , glyphIndexes(other.glyphIndexes)
      , glyphPositions(other.glyphPositions)
      , stringIndexes(other.stringIndexes)
      , rawFont(other.rawFont)
      , boundingRect(other.boundingRect)
      , sourceString(other.sourceString)
      , flags(other.flags)
      , glyphIndexData(other.glyphIndexData)
      , glyphIndexDataSize(other.glyphIndexDataSize)
      , glyphPositionData(other.glyphPositionData)
      , glyphPositionDataSize(other.glyphPositionDataSize)
      , textRangeStart(other.textRangeStart)
      , textRangeEnd(other.textRangeEnd)
    {
    }

    QList<quint32> glyphIndexes;
    QList<QPointF> glyphPositions;
    QList<qsizetype> stringIndexes;
    QRawFont rawFont;
    QRectF boundingRect;
    QString sourceString;

    QGlyphRun::GlyphRunFlags flags;

    const quint32 *glyphIndexData;
    int glyphIndexDataSize;

    const QPointF *glyphPositionData;
    int glyphPositionDataSize;

    int textRangeStart;
    int textRangeEnd;

    static QGlyphRunPrivate *get(const QGlyphRun &glyphRun)
    {
        return glyphRun.d.data();
    }
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_RAWFONT

#endif // QGLYPHRUN_P_H
