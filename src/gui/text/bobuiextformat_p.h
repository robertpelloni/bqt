// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTFORMAT_P_H
#define BOBUIEXTFORMAT_P_H

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
#include "BobUIGui/bobuiextformat.h"
#include "BobUICore/qlist.h"
#include <BobUICore/qhash.h> // QMultiHash

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT BOBUIextFormatCollection
{
public:
    BOBUIextFormatCollection() {}
    ~BOBUIextFormatCollection();

    void clear();

    inline BOBUIextFormat objectFormat(int objectIndex) const
    { return format(objectFormatIndex(objectIndex)); }
    inline void setObjectFormat(int objectIndex, const BOBUIextFormat &format)
    { setObjectFormatIndex(objectIndex, indexForFormat(format)); }

    int objectFormatIndex(int objectIndex) const;
    void setObjectFormatIndex(int objectIndex, int formatIndex);

    int createObjectIndex(const BOBUIextFormat &f);

    int indexForFormat(const BOBUIextFormat &f);
    bool hasFormatCached(const BOBUIextFormat &format) const;

    BOBUIextFormat format(int idx) const;
    inline BOBUIextBlockFormat blockFormat(int index) const
    { return format(index).toBlockFormat(); }
    inline BOBUIextCharFormat charFormat(int index) const
    { return format(index).toCharFormat(); }
    inline BOBUIextListFormat listFormat(int index) const
    { return format(index).toListFormat(); }
    inline BOBUIextTableFormat tableFormat(int index) const
    { return format(index).toTableFormat(); }
    inline BOBUIextImageFormat imageFormat(int index) const
    { return format(index).toImageFormat(); }

    inline int numFormats() const { return formats.size(); }

    typedef QList<BOBUIextFormat> FormatVector;

    FormatVector formats;
    QList<qint32> objFormats;
    QMultiHash<size_t,int> hashes;

    inline QFont defaultFont() const { return defaultFnt; }
    void setDefaultFont(const QFont &f);

    inline void setSuperScriptBaseline(qreal baseline) { defaultFormat.setSuperScriptBaseline(baseline); }
    inline void setSubScriptBaseline(qreal baseline) { defaultFormat.setSubScriptBaseline(baseline); }
    inline void setBaselineOffset(qreal baseline) { defaultFormat.setBaselineOffset(baseline); }

    inline BOBUIextCharFormat defaultTextFormat() const { return defaultFormat; }

private:
    QFont defaultFnt;
    BOBUIextCharFormat defaultFormat;

    Q_DISABLE_COPY_MOVE(BOBUIextFormatCollection)
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTFORMAT_P_H
