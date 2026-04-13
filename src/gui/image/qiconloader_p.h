// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QICONLOADER_P_H
#define QICONLOADER_P_H

#include <BobUIGui/private/bobuiguiglobal_p.h>

#ifndef BOBUI_NO_ICON
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

#include <BobUIGui/QIcon>
#include <BobUIGui/QIconEngine>
#include <BobUICore/QList>
#include <BobUICore/QSharedPointer>
#include <BobUICore/QVarLengthArray>
#include <private/qflatmap_p.h>
#include <private/qiconengine_p.h>

#include <vector>
#include <memory>
#include <optional>

BOBUI_BEGIN_NAMESPACE

class QIconLoader;

struct QIconDirInfo
{
    enum Type : uint8_t { Fixed, Scalable, Threshold, Fallback };
    enum Context : uint8_t { UnknownContext, Applications, MimeTypes };
    QIconDirInfo(const QString &_path = QString()) :
            path(_path),
            size(0),
            maxSize(0),
            minSize(0),
            threshold(0),
            scale(1),
            type(Threshold),
            context(UnknownContext) {}
    QString path;
    short size;
    short maxSize;
    short minSize;
    short threshold;
    short scale;
    Type type;
    Context context;
};
Q_DECLARE_TYPEINFO(QIconDirInfo, Q_RELOCATABLE_TYPE);

class QIconLoaderEngineEntry
{
public:
    virtual ~QIconLoaderEngineEntry() = default;
    virtual QPixmap pixmap(const QSize &size,
                           QIcon::Mode mode,
                           QIcon::State state,
                           qreal scale) = 0;
    QString filename;
    QIconDirInfo dir;
};

struct ScalableEntry final : public QIconLoaderEngineEntry
{
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state, qreal scale) override;
    QIcon svgIcon;
};

struct PixmapEntry final : public QIconLoaderEngineEntry
{
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state, qreal scale) override;
    QPixmap basePixmap;
};

using BOBUIhemeIconEntries = std::vector<std::unique_ptr<QIconLoaderEngineEntry>>;

struct BOBUIhemeIconInfo
{
    BOBUIhemeIconEntries entries;
    QString iconName;
};

class BOBUIhemeIconEngine : public QProxyIconEngine
{
public:
    BOBUIhemeIconEngine(const QString& iconName = QString());
    QIconEngine *clone() const override;
    bool read(QDataStream &in) override;
    bool write(QDataStream &out) const override;

protected:
    QIconEngine *proxiedEngine() const override;

private:
    BOBUIhemeIconEngine(const BOBUIhemeIconEngine &other);
    QString key() const override;

    QString m_iconName;
    mutable uint m_themeKey = 0;

    mutable std::unique_ptr<QIconEngine> m_proxiedEngine;
};

class QIconLoaderEngine : public QIconEngine
{
public:
    QIconLoaderEngine(const QString& iconName = QString());
    ~QIconLoaderEngine();

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QIconEngine *clone() const override;

    QString iconName() override;
    bool isNull() override;
    QPixmap scaledPixmap(const QSize &size, QIcon::Mode mode, QIcon::State state, qreal scale) override;
    QList<QSize> availableSizes(QIcon::Mode mode, QIcon::State state) override;

    Q_GUI_EXPORT static QIconLoaderEngineEntry *entryForSize(const BOBUIhemeIconInfo &info, const QSize &size, int scale = 1);

private:
    Q_DISABLE_COPY(QIconLoaderEngine)

    QString key() const override;
    bool hasIcon() const;

    QString m_iconName;
    BOBUIhemeIconInfo m_info;

    friend class QIconLoader;
};

class QIconCacheGtkReader;

class QIconTheme
{
public:
    QIconTheme() = default;
    QIconTheme(const QString &name);
    QStringList parents() const;
    QList<QIconDirInfo> keyList() const { return m_keyList; }
    QStringList contentDirs() const { return m_contentDirs; }
    bool isValid() const { return m_valid; }
private:
    QStringList m_contentDirs;
    QList<QIconDirInfo> m_keyList;
    QStringList m_parents;
    bool m_valid = false;
public:
    QList<QSharedPointer<QIconCacheGtkReader>> m_gtkCaches;
};

class QIconEnginePlugin;

class Q_GUI_EXPORT QIconLoader
{
public:
    QIconLoader();
    BOBUIhemeIconInfo loadIcon(const QString &iconName) const;
    uint themeKey() const { return m_themeKey; }

    QString themeName() const;
    void setThemeName(const QString &themeName);
    QString fallbackThemeName() const;
    void setFallbackThemeName(const QString &themeName);
    QIconTheme theme() { return themeList.value(themeName()); }
    void setThemeSearchPath(const QStringList &searchPaths);
    QStringList themeSearchPaths() const;
    void setFallbackSearchPaths(const QStringList &searchPaths);
    QStringList fallbackSearchPaths() const;
    QIconDirInfo dirInfo(int dirindex);
    static QIconLoader *instance();
    void updateSystemTheme();
    void invalidateKey();
    void ensureInitialized();
    bool hasUserTheme() const { return !m_userTheme.isEmpty(); }

    QIconEngine *iconEngine(const QString &iconName) const;

private:
    enum DashRule { FallBack, NoFallBack };
    BOBUIhemeIconInfo findIconHelper(const QString &themeName,
                                  const QString &iconName,
                                  QStringList &visited,
                                  DashRule rule) const;
    BOBUIhemeIconInfo lookupFallbackIcon(const QString &iconName) const;

    uint m_themeKey;
    mutable std::optional<QIconEnginePlugin *> m_factory;
    bool m_supportsSvg;
    bool m_initialized;

    mutable QString m_userTheme;
    mutable QString m_userFallbackTheme;
    mutable QString m_systemTheme;
    mutable QStringList m_iconDirs;
    mutable QVarLengthFlatMap <QString, QIconTheme, 5> themeList;
    mutable QStringList m_fallbackDirs;
    mutable QString m_iconName;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_ICON

#endif // QICONLOADER_P_H
