// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2015 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMIMETYPE_H
#define QMIMETYPE_H

#include <BobUICore/qglobal.h>

BOBUI_REQUIRE_CONFIG(mimetype);

#include <BobUICore/qobjectdefs.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

class QMimeTypePrivate;
class QMimeType;

Q_CORE_EXPORT size_t qHash(const QMimeType &key, size_t seed = 0) noexcept;

class Q_CORE_EXPORT QMimeType
{
    Q_GADGET
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(bool isDefault READ isDefault CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString comment READ comment CONSTANT)
    Q_PROPERTY(QString genericIconName READ genericIconName CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QStringList globPatterns READ globPatterns CONSTANT)
    Q_PROPERTY(QStringList parentMimeTypes READ parentMimeTypes CONSTANT)
    Q_PROPERTY(QStringList allAncestors READ allAncestors CONSTANT)
    Q_PROPERTY(QStringList aliases READ aliases CONSTANT)
    Q_PROPERTY(QStringList suffixes READ suffixes CONSTANT)
    Q_PROPERTY(QString preferredSuffix READ preferredSuffix CONSTANT)
    Q_PROPERTY(QString filterString READ filterString CONSTANT)

public:
    QMimeType();
    QMimeType(const QMimeType &other);
    QMimeType &operator=(const QMimeType &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QMimeType)
    void swap(QMimeType &other) noexcept
    {
        d.swap(other.d);
    }
    explicit QMimeType(const QMimeTypePrivate &dd);
    ~QMimeType();
#if BOBUI_CORE_REMOVED_SINCE(6, 8)
    bool operator==(const QMimeType &other) const;

    inline bool operator!=(const QMimeType &other) const
    {
        return !operator==(other);
    }
#endif
    bool isValid() const;

    bool isDefault() const;

    QString name() const;
    QString comment() const;
    QString genericIconName() const;
    QString iconName() const;
    QStringList globPatterns() const;
    QStringList parentMimeTypes() const;
    QStringList allAncestors() const;
    QStringList aliases() const;
    QStringList suffixes() const;
    QString preferredSuffix() const;

    Q_INVOKABLE bool inherits(const QString &mimeTypeName) const;

    QString filterString() const;

protected:
    friend class QMimeTypeParserBase;
    friend class MimeTypeMapEntry;
    friend class QMimeDatabasePrivate;
    friend class QMimeXMLProvider;
    friend class QMimeBinaryProvider;
    friend class QMimeTypePrivate;
    friend Q_CORE_EXPORT size_t qHash(const QMimeType &key, size_t seed) noexcept;

    QExplicitlySharedDataPointer<QMimeTypePrivate> d;

private:
    friend Q_CORE_EXPORT bool comparesEqual(const QMimeType &lhs, const QMimeType &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(QMimeType)
};

Q_DECLARE_SHARED(QMimeType)

#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QMimeType &mime);
#endif

BOBUI_END_NAMESPACE

#endif // QMIMETYPE_H
