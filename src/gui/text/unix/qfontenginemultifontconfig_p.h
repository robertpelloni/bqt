// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFONTENGINEMULTIFONTCONFIG_H
#define QFONTENGINEMULTIFONTCONFIG_H

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

#include <BobUIGui/private/qfontengine_p.h>
#include <fontconfig/fontconfig.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QFontEngineMultiFontConfig : public QFontEngineMulti
{
public:
    explicit QFontEngineMultiFontConfig(QFontEngine *fe, int script);

    ~QFontEngineMultiFontConfig();

    bool shouldLoadFontEngineForCharacter(int at, uint ucs4) const override;
private:
    FcPattern* getMatchPatternForFallback(int at) const;

    mutable QList<FcPattern *> cachedMatchPatterns;
};

BOBUI_END_NAMESPACE

#endif // QFONTENGINEMULTIFONTCONFIG_H
