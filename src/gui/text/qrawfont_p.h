// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRAWFONTPRIVATE_P_H
#define QRAWFONTPRIVATE_P_H

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
#include "qrawfont.h"

#include "qfontengine_p.h"
#include <BobUICore/bobuihread.h>
#include <BobUICore/bobuihreadstorage.h>

#if !defined(BOBUI_NO_RAWFONT)

BOBUI_BEGIN_NAMESPACE

namespace { class CustomFontFileLoader; }
class Q_GUI_EXPORT QRawFontPrivate
{
public:
    QRawFontPrivate()
        : fontEngine(nullptr)
        , hintingPreference(QFont::PreferDefaultHinting)
        , thread(nullptr)
    {}

    QRawFontPrivate(const QRawFontPrivate &other)
        : fontEngine(other.fontEngine)
        , hintingPreference(other.hintingPreference)
        , thread(other.thread)
    {
#ifndef BOBUI_NO_DEBUG
        Q_ASSERT(fontEngine == nullptr || thread == BOBUIhread::currentThread());
#endif
        if (fontEngine != nullptr)
            fontEngine->ref.ref();
    }

    ~QRawFontPrivate()
    {
#ifndef BOBUI_NO_DEBUG
        Q_ASSERT(ref.loadRelaxed() == 0);
#endif
        cleanUp();
    }

    inline void cleanUp()
    {
        setFontEngine(nullptr);
        hintingPreference = QFont::PreferDefaultHinting;
    }

    inline bool isValid() const
    {
#ifndef BOBUI_NO_DEBUG
        Q_ASSERT(fontEngine == nullptr || thread == BOBUIhread::currentThread());
#endif
        return fontEngine != nullptr;
    }

    inline void setFontEngine(QFontEngine *engine)
    {
#ifndef BOBUI_NO_DEBUG
        Q_ASSERT(fontEngine == nullptr || thread == BOBUIhread::currentThread());
#endif
        if (fontEngine == engine)
            return;

        if (fontEngine != nullptr) {
            if (!fontEngine->ref.deref())
                delete fontEngine;
#ifndef BOBUI_NO_DEBUG
            thread = nullptr;
#endif
        }

        fontEngine = engine;

        if (fontEngine != nullptr) {
            fontEngine->ref.ref();
#ifndef BOBUI_NO_DEBUG
            thread = BOBUIhread::currentThread();
            Q_ASSERT(thread);
#endif
        }
    }

    void loadFromData(const QByteArray &fontData,
                              qreal pixelSize,
                              QFont::HintingPreference hintingPreference);

    static QRawFontPrivate *get(const QRawFont &font) { return font.d.data(); }

    QFontEngine *fontEngine;
    QFont::HintingPreference hintingPreference;
    QAtomicInt ref;

private:
    BOBUIhread *thread;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_RAWFONT

#endif // QRAWFONTPRIVATE_P_H
