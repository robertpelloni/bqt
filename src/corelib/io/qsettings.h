// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSETTINGS_H
#define QSETTINGS_H

#include <BobUICore/qobject.h>
#include <BobUICore/qvariant.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qscopedpointer.h>

BOBUI_REQUIRE_CONFIG(settings);

#include <ctype.h>

BOBUI_BEGIN_NAMESPACE

#ifdef Status // we seem to pick up a macro Status --> int somewhere
#undef Status
#endif

class QIODevice;
class QSettingsPrivate;

#ifndef BOBUI_NO_QOBJECT
class Q_CORE_EXPORT QSettings : public QObject
#else
class Q_CORE_EXPORT QSettings
#endif
{
#ifndef BOBUI_NO_QOBJECT
    Q_OBJECT
#else
    std::unique_ptr<QSettingsPrivate> d_ptr;
#endif
    Q_DECLARE_PRIVATE(QSettings)

public:
    enum Status {
        NoError = 0,
        AccessError,
        FormatError
    };
#ifndef BOBUI_NO_QOBJECT
    Q_ENUM(Status)
#endif

    enum Format {
        NativeFormat = 0,
        IniFormat = 1,

#if defined(Q_OS_WIN) || defined(Q_QDOC)
        Registry32Format = 2,
        Registry64Format = 3,
#endif

#if defined(Q_OS_WASM) || defined(Q_QDOC)
        WebLocalStorageFormat = 4,
        WebIndexedDBFormat = 5,
#endif

        InvalidFormat = 16,
        CustomFormat1,
        CustomFormat2,
        CustomFormat3,
        CustomFormat4,
        CustomFormat5,
        CustomFormat6,
        CustomFormat7,
        CustomFormat8,
        CustomFormat9,
        CustomFormat10,
        CustomFormat11,
        CustomFormat12,
        CustomFormat13,
        CustomFormat14,
        CustomFormat15,
        CustomFormat16
    };
#ifndef BOBUI_NO_QOBJECT
    Q_ENUM(Format)
#endif

    enum Scope {
        UserScope,
        SystemScope
    };
#ifndef BOBUI_NO_QOBJECT
    Q_ENUM(Scope)
#endif

#ifndef BOBUI_NO_QOBJECT
    explicit QSettings(const QString &organization,
                       const QString &application = QString(), QObject *parent = nullptr);
    QSettings(Scope scope, const QString &organization,
              const QString &application = QString(), QObject *parent = nullptr);
    QSettings(Format format, Scope scope, const QString &organization,
              const QString &application = QString(), QObject *parent = nullptr);
    QSettings(const QString &fileName, Format format, QObject *parent = nullptr);
    explicit QSettings(QObject *parent = nullptr);
    explicit QSettings(Scope scope, QObject *parent = nullptr);
#else
    explicit QSettings(const QString &organization,
                       const QString &application = QString());
    QSettings(Scope scope, const QString &organization,
              const QString &application = QString());
    QSettings(Format format, Scope scope, const QString &organization,
              const QString &application = QString());
    QSettings(const QString &fileName, Format format);
    explicit QSettings(Scope scope = UserScope);
#endif
    ~QSettings();

    void clear();
    void sync();
    Status status() const;
    bool isAtomicSyncRequired() const;
    void setAtomicSyncRequired(bool enable);

#if BOBUI_CORE_REMOVED_SINCE(6, 4)
    void beginGroup(const QString &prefix);
#endif
    void beginGroup(QAnyStringView prefix);
    void endGroup();
    QString group() const;

#if BOBUI_CORE_REMOVED_SINCE(6, 4)
    int beginReadArray(const QString &prefix);
    void beginWriteArray(const QString &prefix, int size = -1);
#endif
    int beginReadArray(QAnyStringView prefix);
    void beginWriteArray(QAnyStringView prefix, int size = -1);
    void endArray();
    void setArrayIndex(int i);

    QStringList allKeys() const;
    QStringList childKeys() const;
    QStringList childGroups() const;
    bool isWritable() const;

#if BOBUI_CORE_REMOVED_SINCE(6, 4)
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue) const;
    QVariant value(const QString &key) const;
#endif
    void setValue(QAnyStringView key, const QVariant &value);
    QVariant value(QAnyStringView key, const QVariant &defaultValue) const;
    QVariant value(QAnyStringView key) const;

#if BOBUI_CORE_REMOVED_SINCE(6, 4)
    void remove(const QString &key);
    bool contains(const QString &key) const;
#endif
    void remove(QAnyStringView key);
    bool contains(QAnyStringView key) const;

    void setFallbacksEnabled(bool b);
    bool fallbacksEnabled() const;

    QString fileName() const;
    Format format() const;
    Scope scope() const;
    QString organizationName() const;
    QString applicationName() const;

    static void setDefaultFormat(Format format);
    static Format defaultFormat();
    static void setPath(Format format, Scope scope, const QString &path);

    typedef QMap<QString, QVariant> SettingsMap;
    typedef bool (*ReadFunc)(QIODevice &device, SettingsMap &map);
    typedef bool (*WriteFunc)(QIODevice &device, const SettingsMap &map);

    static Format registerFormat(const QString &extension, ReadFunc readFunc, WriteFunc writeFunc,
                                 BobUI::CaseSensitivity caseSensitivity = BobUI::CaseSensitive);

protected:
#ifndef BOBUI_NO_QOBJECT
    bool event(QEvent *event) override;
#endif

private:
    Q_DISABLE_COPY(QSettings)
};

BOBUI_END_NAMESPACE

#endif // QSETTINGS_H
