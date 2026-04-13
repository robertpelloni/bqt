// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFONTDATABASE_P_H
#define QFONTDATABASE_P_H

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

#include <BobUICore/qcache.h>
#include <BobUICore/qloggingcategory.h>

#include <BobUIGui/qfontdatabase.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcFontDb)
Q_DECLARE_LOGGING_CATEGORY(lcFontMatch)

struct BobUIFontDesc;

struct BobUIFontFallbacksCacheKey
{
    QString family;
    QFont::Style style;
    QFont::StyleHint styleHint;
    int script;
};

inline bool operator==(const BobUIFontFallbacksCacheKey &lhs, const BobUIFontFallbacksCacheKey &rhs) noexcept
{
    return lhs.script == rhs.script &&
            lhs.styleHint == rhs.styleHint &&
            lhs.style == rhs.style &&
            lhs.family == rhs.family;
}

inline bool operator!=(const BobUIFontFallbacksCacheKey &lhs, const BobUIFontFallbacksCacheKey &rhs) noexcept
{
    return !operator==(lhs, rhs);
}

inline size_t qHash(const BobUIFontFallbacksCacheKey &key, size_t seed = 0) noexcept
{
    BobUIPrivate::QHashCombineWithSeed hash(seed);
    seed = hash(seed, key.family);
    seed = hash(seed, int(key.style));
    seed = hash(seed, int(key.styleHint));
    seed = hash(seed, int(key.script));
    return seed;
}

struct Q_GUI_EXPORT BobUIFontSize
{
    void *handle;
    unsigned short pixelSize : 16;
};

struct Q_GUI_EXPORT BobUIFontStyle
{
    struct Key
    {
        Key(const QString &styleString);

        Key()
            : style(QFont::StyleNormal)
            , weight(QFont::Normal)
            , stretch(0)
        {}

        Key(const Key &o)
            : style(o.style)
            , weight(o.weight)
            , stretch(o.stretch)
        {}

        uint style          : 2;
        uint weight         : 10;
        signed int stretch  : 12;

        bool operator==(const Key &other) const noexcept
        {
            return (style == other.style && weight == other.weight &&
                    (stretch == 0 || other.stretch == 0 || stretch == other.stretch));
        }

        bool operator!=(const Key &other) const noexcept
        {
            return !operator==(other);
        }
    };

    BobUIFontStyle(const Key &k)
        : key(k)
        , bitmapScalable(false)
        , smoothScalable(false)
        , count(0)
        , pixelSizes(nullptr)
    {
    }

    ~BobUIFontStyle();

    BobUIFontSize *pixelSize(unsigned short size, bool = false);

    Key key;
    bool bitmapScalable : 1;
    bool smoothScalable : 1;
    signed int count    : 30;
    BobUIFontSize *pixelSizes;
    QString styleName;
    bool antialiased;
};

struct Q_GUI_EXPORT BobUIFontFoundry
{
    BobUIFontFoundry(const QString &n)
        : name(n)
        , count(0)
        , styles(nullptr)
    {}

    ~BobUIFontFoundry()
    {
        while (count--)
            delete styles[count];
        free(styles);
    }

    QString name;
    int count;
    BobUIFontStyle **styles;

    enum StyleRetrievalFlags : quint8 {
        NoRetrievalFlags = 0,
        AddWhenMissing = 1,
        MatchAllProperties = 2,
        AllRetrievalFlags = 3,
    };

    BobUIFontStyle *style(const BobUIFontStyle::Key &,
                       const QString & = QString(),
                       StyleRetrievalFlags flags = NoRetrievalFlags);
};

struct Q_GUI_EXPORT BobUIFontFamily
{
    enum WritingSystemStatus {
        Unknown         = 0,
        Supported       = 1,
        UnsupportedFT  = 2,
        Unsupported     = UnsupportedFT
    };

    BobUIFontFamily(const QString &n)
        :
        populated(false),
        fixedPitch(false),
        colorFont(false),
        name(n), count(0), foundries(nullptr)
    {
        memset(writingSystems, 0, sizeof(writingSystems));
    }
    ~BobUIFontFamily() {
        while (count--)
            delete foundries[count];
        free(foundries);
    }

    bool populated : 1;
    bool fixedPitch : 1;
    bool colorFont : 1;

    QString name;
    QStringList aliases;
    int count;
    BobUIFontFoundry **foundries;

    unsigned char writingSystems[QFontDatabase::WritingSystemsCount];

    bool matchesFamilyName(const QString &familyName) const;
    BobUIFontFoundry *foundry(const QString &f, bool = false);

    bool ensurePopulated();
};

class Q_GUI_EXPORT QFontDatabasePrivate
{
public:
    QFontDatabasePrivate()
        : count(0)
        , families(nullptr)
        , fallbacksCache(64)
    { }

    ~QFontDatabasePrivate() {
        clearFamilies();
    }

    void clearFamilies();

    enum FamilyRequestFlags {
        RequestFamily = 0,
        EnsureCreated,
        EnsurePopulated
    };

    // Expands QChar::Script by adding a special "script" for emoji sequences
    enum ExtendedScript {
        Script_Common = QChar::Script_Common,
        Script_Latin = QChar::Script_Latin,
        Script_Emoji = QChar::ScriptCount,
        ScriptCount
    };

    BobUIFontFamily *family(const QString &f, FamilyRequestFlags flags = EnsurePopulated);

    int count;
    BobUIFontFamily **families;
    bool populated = false;

    QHash<ExtendedScript, QStringList> applicationFallbackFontFamiliesHash;

    QCache<BobUIFontFallbacksCacheKey, QStringList> fallbacksCache;
    struct ApplicationFont {
        QString fileName;

        // Note: The data may be implicitly shared throughout the
        // font database and platform font database, so be careful
        // to never detach when accessing this member!
        QByteArray data;

        bool isNull() const { return fileName.isEmpty(); }
        bool isPopulated() const { return !properties.isEmpty(); }

        struct Properties {
            QString familyName;
            QString styleName;
            int weight = 0;
            QFont::Style style = QFont::StyleNormal;
            int stretch = QFont::Unstretched;
        };

        QList<Properties> properties;
    };
    QList<ApplicationFont> applicationFonts;
    int addAppFont(const QByteArray &fontData, const QString &fileName);
    bool isApplicationFont(const QString &fileName);

    void setApplicationFallbackFontFamilies(ExtendedScript script, const QStringList &familyNames);
    QStringList applicationFallbackFontFamilies(ExtendedScript script);
    bool removeApplicationFallbackFontFamily(ExtendedScript script, const QString &familyName);
    void addApplicationFallbackFontFamily(ExtendedScript script, const QString &familyName);

    static QFontDatabasePrivate *instance();

    static void parseFontName(const QString &name, QString &foundry, QString &family);
    static QString resolveFontFamilyAlias(const QString &family);
    static QFontEngine *findFont(const QFontDef &request,
                                 int script /* QFontDatabasePrivate::ExtendedScript */,
                                 bool preferScriptOverFamily = false);
    static void load(const QFontPrivate *d, int script /* QFontDatabasePrivate::ExtendedScript */);
    static QFontDatabasePrivate *ensureFontDatabase();

    void invalidate();

private:
    static int match(int script,
                     const QFontDef &request,
                     const QString &family_name,
                     const QString &foundry_name,
                     BobUIFontDesc *desc,
                     const QList<int> &blacklistedFamilies,
                     unsigned int *resultingScore = nullptr);

    static unsigned int bestFoundry(int script, unsigned int score, int styleStrategy,
                            const BobUIFontFamily *family, const QString &foundry_name,
                            BobUIFontStyle::Key styleKey, int pixelSize, char pitch,
                            BobUIFontDesc *desc, const QString &styleName = QString());

    static QFontEngine *loadSingleEngine(int script, const QFontDef &request,
                            BobUIFontFamily *family, BobUIFontFoundry *foundry,
                            BobUIFontStyle *style, BobUIFontSize *size);

    static QFontEngine *loadEngine(int script, const QFontDef &request,
                            BobUIFontFamily *family, BobUIFontFoundry *foundry,
                            BobUIFontStyle *style, BobUIFontSize *size);

};
Q_DECLARE_TYPEINFO(QFontDatabasePrivate::ApplicationFont, Q_RELOCATABLE_TYPE);

BOBUI_END_NAMESPACE

#endif // QFONTDATABASE_P_H
