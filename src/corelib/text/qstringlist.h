// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUICore/qlist.h>

#ifndef QSTRINGLIST_H
#define QSTRINGLIST_H

#include <BobUICore/qalgorithms.h>
#include <BobUICore/qcontainertools_impl.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringmatcher.h>

BOBUI_BEGIN_NAMESPACE

class QLatin1StringMatcher;
class QRegularExpression;

#if !defined(BOBUI_NO_JAVA_STYLE_ITERATORS)
using QStringListIterator = QListIterator<QString>;
using QMutableStringListIterator = QMutableListIterator<QString>;
#endif


namespace BobUIPrivate {
    void Q_CORE_EXPORT QStringList_sort(QStringList *that, BobUI::CaseSensitivity cs);
    qsizetype Q_CORE_EXPORT QStringList_removeDuplicates(QStringList *that);
    QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
    QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
    Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
    QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
                                               BobUI::CaseSensitivity cs);
    Q_CORE_EXPORT QStringList QStringList_filter(const QStringList &that, QLatin1StringView needle,
                                                 BobUI::CaseSensitivity cs);
    Q_CORE_EXPORT QStringList QStringList_filter(const QStringList &that,
                                                 const QStringMatcher &matcher);
    Q_CORE_EXPORT QStringList QStringList_filter(const QStringList &that,
                                                 const QLatin1StringMatcher &matcher);

    bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, BobUI::CaseSensitivity cs);
    bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, BobUI::CaseSensitivity cs);
    void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
                                      BobUI::CaseSensitivity cs);

    qsizetype Q_CORE_EXPORT QStringList_indexOf(const QStringList &that, QStringView str,
                                                qsizetype from, BobUI::CaseSensitivity cs);
    qsizetype Q_CORE_EXPORT QStringList_indexOf(const QStringList &that, QLatin1StringView str,
                                                qsizetype from, BobUI::CaseSensitivity cs);

    Q_CORE_EXPORT qsizetype QStringList_lastIndexOf(const QStringList &that, QStringView str,
                                                    qsizetype from, BobUI::CaseSensitivity cs);
    Q_CORE_EXPORT qsizetype QStringList_lastIndexOf(const QStringList &that, QLatin1StringView str,
                                                    qsizetype from, BobUI::CaseSensitivity cs);

#if BOBUI_CONFIG(regularexpression)
    void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
    QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, const QRegularExpression &re);
    qsizetype Q_CORE_EXPORT QStringList_indexOf(const QStringList *that, const QRegularExpression &re, qsizetype from);
    qsizetype Q_CORE_EXPORT QStringList_lastIndexOf(const QStringList *that, const QRegularExpression &re, qsizetype from);
#endif // BOBUI_CONFIG(regularexpression)
}

#ifdef Q_QDOC
class QStringList : public QList<QString>
#else
template <> struct QListSpecialMethods<QString> : QListSpecialMethodsBase<QString>
#endif
{
#ifdef Q_QDOC
public:
    using QList<QString>::QList;
    QStringList(const QString &str);
    QStringList(const QList<QString> &other);
    QStringList(QList<QString> &&other);

    QStringList &operator=(const QList<QString> &other);
    QStringList &operator=(QList<QString> &&other);
    QStringList operator+(const QStringList &other) const;
    QStringList &operator<<(const QString &str);
    QStringList &operator<<(const QStringList &other);
    QStringList &operator<<(const QList<QString> &other);
private:
#endif

public:
    inline void sort(BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
    { BobUIPrivate::QStringList_sort(self(), cs); }
    inline qsizetype removeDuplicates()
    { return BobUIPrivate::QStringList_removeDuplicates(self()); }

    inline QString join(QStringView sep) const
    { return BobUIPrivate::QStringList_join(self(), sep); }
    inline QString join(QLatin1StringView sep) const
    { return BobUIPrivate::QStringList_join(*self(), sep); }
    inline QString join(QChar sep) const
    { return BobUIPrivate::QStringList_join(self(), &sep, 1); }

    QStringList filter(const QStringMatcher &matcher) const
    { return BobUIPrivate::QStringList_filter(*self(), matcher); }
    QStringList filter(const QLatin1StringMatcher &matcher) const
    { return BobUIPrivate::QStringList_filter(*self(), matcher); }

    QStringList filter(QLatin1StringView needle, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const
    { return BobUIPrivate::QStringList_filter(*self(), needle, cs); }
    inline QStringList filter(QStringView str, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const
    { return BobUIPrivate::QStringList_filter(self(), str, cs); }
    inline QStringList &replaceInStrings(QStringView before, QStringView after, BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
    {
        BobUIPrivate::QStringList_replaceInStrings(self(), before, after, cs);
        return *self();
    }

    inline QString join(const QString &sep) const
    { return BobUIPrivate::QStringList_join(self(), sep.constData(), sep.size()); }
    inline QStringList filter(const QString &str, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const
    { return BobUIPrivate::QStringList_filter(self(), str, cs); }
    inline QStringList &replaceInStrings(const QString &before, const QString &after, BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
    {
        BobUIPrivate::QStringList_replaceInStrings(self(), before, after, cs);
        return *self();
    }
    inline QStringList &replaceInStrings(const QString &before, QStringView after, BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
    {
        BobUIPrivate::QStringList_replaceInStrings(self(), before, after, cs);
        return *self();
    }
    inline QStringList &replaceInStrings(QStringView before, const QString &after, BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
    {
        BobUIPrivate::QStringList_replaceInStrings(self(), before, after, cs);
        return *self();
    }
    using QListSpecialMethodsBase<QString>::contains;
    using QListSpecialMethodsBase<QString>::indexOf;
    using QListSpecialMethodsBase<QString>::lastIndexOf;

    inline bool contains(QLatin1StringView str, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_contains(self(), str, cs); }
    inline bool contains(QStringView str, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_contains(self(), str, cs); }

    inline bool contains(const QString &str, BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_contains(self(), str, cs); }

    qsizetype indexOf(const QString &str, qsizetype from = 0,
                      BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return indexOf(QStringView(str), from, cs); }
    qsizetype indexOf(QStringView needle, qsizetype from = 0,
                      BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_indexOf(*self(), needle, from, cs); }
    qsizetype indexOf(QLatin1StringView needle, qsizetype from = 0,
                      BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_indexOf(*self(), needle, from, cs); }

    qsizetype lastIndexOf(const QString &str, qsizetype from = -1,
                          BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return lastIndexOf(QStringView(str), from, cs); }
    qsizetype lastIndexOf(QStringView str, qsizetype from = -1,
                          BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_lastIndexOf(*self(), str, from, cs); }
    qsizetype lastIndexOf(QLatin1StringView needle, qsizetype from = -1,
                          BobUI::CaseSensitivity cs = BobUI::CaseSensitive) const noexcept
    { return BobUIPrivate::QStringList_lastIndexOf(*self(), needle, from, cs); }

#if BOBUI_CONFIG(regularexpression)
    inline QStringList filter(const QRegularExpression &re) const
    { return BobUIPrivate::QStringList_filter(self(), re); }
    inline QStringList &replaceInStrings(const QRegularExpression &re, const QString &after)
    {
        BobUIPrivate::QStringList_replaceInStrings(self(), re, after);
        return *self();
    }
    inline qsizetype indexOf(const QRegularExpression &re, qsizetype from = 0) const
    { return BobUIPrivate::QStringList_indexOf(self(), re, from); }
    inline qsizetype lastIndexOf(const QRegularExpression &re, qsizetype from = -1) const
    { return BobUIPrivate::QStringList_lastIndexOf(self(), re, from); }
#endif // BOBUI_CONFIG(regularexpression)
};

BOBUI_END_NAMESPACE

#endif // QSTRINGLIST_H
