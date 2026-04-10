// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSFONTDATABASEBASE_P_H
#define QWINDOWSFONTDATABASEBASE_P_H

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

#include <qpa/qplatformfontdatabase.h>
#include <BobUIGui/private/bobuigui-config_p.h>
#include <BobUICore/QSharedPointer>
#include <BobUICore/QLoggingCategory>
#include <BobUICore/bobui_windows.h>

#if BOBUI_CONFIG(directwrite)
    struct IDWriteFactory;
    struct IDWriteGdiInterop;
    struct IDWriteFontFace;
#endif

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(directwrite)
    class QCustomFontFileLoader;
#endif

class QWindowsFontEngineData
{
    Q_DISABLE_COPY_MOVE(QWindowsFontEngineData)
public:
    QWindowsFontEngineData();
    ~QWindowsFontEngineData();

    uint pow_gamma[256];

    bool clearTypeEnabled = false;
    qreal fontSmoothingGamma;
    HDC hdc = 0;
#if BOBUI_CONFIG(directwrite)
    IDWriteFactory *directWriteFactory = nullptr;
    IDWriteGdiInterop *directWriteGdiInterop = nullptr;
#endif
};

class Q_GUI_EXPORT QWindowsFontDatabaseBase : public QPlatformFontDatabase
{
public:
    QWindowsFontDatabaseBase();
    ~QWindowsFontDatabaseBase() override;

    QFontEngine *fontEngine(const QFontDef &fontDef, void *handle) override;
    QFontEngine *fontEngine(const QByteArray &fontData, qreal pixelSize, QFont::HintingPreference hintingPreference) override;

    void invalidate() override;

    static int defaultVerticalDPI();

    static QSharedPointer<QWindowsFontEngineData> data();
#if BOBUI_CONFIG(directwrite)
    static void createDirectWriteFactory(IDWriteFactory **factory);
#endif
    static QFont systemDefaultFont();
    static HFONT systemFont();
    static LOGFONT fontDefToLOGFONT(const QFontDef &fontDef, const QString &faceName);
    static QFont LOGFONT_to_QFont(const LOGFONT& lf, int verticalDPI = 0);

    static QString familyForStyleHint(QFont::StyleHint styleHint);
    static QStringList extraTryFontsForFamily(const QString &family);
    static QStringList familiesForScript(QFontDatabasePrivate::ExtendedScript script);

    class FontTable{};
    class EmbeddedFont
    {
    public:
        EmbeddedFont(const QByteArray &fontData) : m_fontData(fontData) {}

        QString changeFamilyName(const QString &newFamilyName);
        QByteArray data() const { return m_fontData; }
        void updateFromOS2Table(QFontEngine *fontEngine);
        FontTable *tableDirectoryEntry(const QByteArray &tagName);
        QString familyName(FontTable *nameTableDirectory = nullptr);

    private:
        QByteArray m_fontData;
    };

    QFontDef sanitizeRequest(QFontDef request) const;

protected:

#if BOBUI_CONFIG(directwrite)
    QList<IDWriteFontFace *> createDirectWriteFaces(const QByteArray &fontData,
                                                    const QString &filename,
                                                    bool queryVariations = true) const;
    IDWriteFontFace *createDirectWriteFace(const QByteArray &fontData);
#endif

private:
    static bool init(QSharedPointer<QWindowsFontEngineData> data);

#if BOBUI_CONFIG(directwrite)
    mutable std::unique_ptr<QCustomFontFileLoader> m_fontFileLoader;
#endif
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSFONTDATABASEBASE_P_H
