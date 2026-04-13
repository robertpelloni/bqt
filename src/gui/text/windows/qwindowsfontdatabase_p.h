// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSFONTDATABASE_H
#define QWINDOWSFONTDATABASE_H

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

#include "qwindowsfontdatabasebase_p.h"

#include <qpa/qplatformfontdatabase.h>
#include <BobUICore/QSharedPointer>
#include <BobUICore/QLoggingCategory>
#include <BobUICore/qhashfunctions.h>
#include <BobUICore/qmutex.h>
#include <BobUICore/bobui_windows.h>

BOBUI_BEGIN_NAMESPACE

class QDebug;

class Q_GUI_EXPORT QWindowsFontDatabase : public QWindowsFontDatabaseBase
{
    Q_DISABLE_COPY_MOVE(QWindowsFontDatabase)
public:
    enum FontOptions {
        // Relevant bits from QWindowsIntegration::Options
        DontUseDirectWriteFonts = 0x40,
        DontUseColorFonts = 0x80
    };

    QWindowsFontDatabase();
    ~QWindowsFontDatabase() override;

    void ensureFamilyPopulated(const QString &familyName);

    void populateFontDatabase() override;
    void invalidate() override;
    void removeApplicationFonts();
    bool supportsColrv0Fonts() const override;

    void populateFamily(const QString &familyName) override;
    bool populateFamilyAliases(const QString &missingFamily) override;
    QFontEngine *fontEngine(const QFontDef &fontDef, void *handle) override;
    QFontEngine *fontEngine(const QByteArray &fontData, qreal pixelSize, QFont::HintingPreference hintingPreference) override;
    QStringList fallbacksForFamily(const QString &family, QFont::Style style, QFont::StyleHint styleHint, QFontDatabasePrivate::ExtendedScript script) const override;
    QStringList addApplicationFont(const QByteArray &fontData, const QString &fileName, QFontDatabasePrivate::ApplicationFont *applicationFont = nullptr) override;
    void releaseHandle(void *handle) override;
    QString fontDir() const override;

    QFont defaultFont() const  override { return systemDefaultFont(); }
    bool fontsAlwaysScalable() const override;
    void derefUniqueFont(const QString &uniqueFont);
    void refUniqueFont(const QString &uniqueFont);
    bool isPrivateFontFamily(const QString &family) const override;

    static QFontEngine *createEngine(const QFontDef &request, const QString &faceName,
                                     int dpi,
                                     const QSharedPointer<QWindowsFontEngineData> &data);

    static qreal fontSmoothingGamma();

    static void setFontOptions(unsigned options);
    static unsigned fontOptions();

#ifndef BOBUI_NO_DEBUG_STREAM
    static void debugFormat(QDebug &d, const LOGFONT &lf);
#endif // !BOBUI_NO_DEBUG_STREAM

    struct FontHandle {
        FontHandle(const QString &name) : faceName(name) {}
#if BOBUI_CONFIG(directwrite)
        FontHandle(IDWriteFontFace *face, const QString &name);
#endif // !BOBUI_NO_DIRECTWRITE
        ~FontHandle();

#if BOBUI_CONFIG(directwrite)
        IDWriteFontFace *fontFace = nullptr;
#endif // !BOBUI_NO_DIRECTWRITE
        QString faceName;
    };

private:
    void addDefaultEUDCFont();

    struct WinApplicationFont {
        HANDLE handle;
        QString fileName;
    };

    QList<WinApplicationFont> m_applicationFonts;

    struct UniqueFontData {
        HANDLE handle;
        int refCount;
    };

    QMutex m_uniqueFontDataMutex; // protects m_uniqueFontData
    QMap<QString, UniqueFontData> m_uniqueFontData;

    static unsigned m_fontOptions;
    QStringList m_eudcFonts;
    bool m_hasPopulatedAliases = false;
};

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QFontDef &def);
#endif

inline quint16 bobui_getUShort(const unsigned char *p)
{
    quint16 val;
    val = *p++ << 8;
    val |= *p;

    return val;
}

struct QFontNames
{
    QString name;   // e.g. "DejaVu Sans Condensed"
    QString style;  // e.g. "Italic"
    QString preferredName;  // e.g. "DejaVu Sans"
    QString preferredStyle; // e.g. "Condensed Italic"
};

struct QFontValues
{
    quint16 weight = 0;
    bool isItalic = false;
    bool isOverstruck = false;
    bool isUnderlined = false;
};

bool bobui_localizedName(const QString &name);
QString bobui_getEnglishName(const QString &familyName, bool includeStyle = false);
QFontNames bobui_getCanonicalFontNames(const LOGFONT &lf);

struct FontAndStyle {
    QString font;
    QString style;

    friend inline bool operator==(const FontAndStyle &lhs, const FontAndStyle &rhs) noexcept
    { return lhs.font == rhs.font && lhs.style == rhs.style; }
    friend inline bool operator!=(const FontAndStyle &lhs, const FontAndStyle &rhs) noexcept
    { return !operator==(lhs, rhs); }
};
inline size_t qHash(const FontAndStyle &key, size_t seed) noexcept
{
    return qHashMulti(seed, key.font, key.style);
}

BOBUI_END_NAMESPACE

#endif // QWINDOWSFONTDATABASE_H
