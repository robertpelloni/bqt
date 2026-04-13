// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2019 Mail.ru Group.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:trivial-impl-only

#ifndef QSTRINGMATCHER_H
#define QSTRINGMATCHER_H

#include <BobUICore/qstring.h>
#include <BobUICore/qstringview.h>

BOBUI_BEGIN_NAMESPACE


class QStringMatcherPrivate;

class Q_CORE_EXPORT QStringMatcher
{
    void updateSkipTable();
public:
    QStringMatcher() = default;
    explicit QStringMatcher(const QString &pattern,
                   BobUI::CaseSensitivity cs = BobUI::CaseSensitive);
    QStringMatcher(const QChar *uc, qsizetype len,
                   BobUI::CaseSensitivity cs = BobUI::CaseSensitive)
        : QStringMatcher(QStringView(uc, len), cs)
    {}
    QStringMatcher(QStringView pattern,
                   BobUI::CaseSensitivity cs = BobUI::CaseSensitive);
    QStringMatcher(const QStringMatcher &other);
    ~QStringMatcher();

    QStringMatcher &operator=(const QStringMatcher &other);

    void setPattern(const QString &pattern);
    void setCaseSensitivity(BobUI::CaseSensitivity cs);

    qsizetype indexIn(const QString &str, qsizetype from = 0) const
    { return indexIn(QStringView(str), from); }
    qsizetype indexIn(const QChar *str, qsizetype length, qsizetype from = 0) const
    { return indexIn(QStringView(str, length), from); }
    qsizetype indexIn(QStringView str, qsizetype from = 0) const;
    QString pattern() const;
    QStringView patternView() const noexcept
    { return q_sv; }

    inline BobUI::CaseSensitivity caseSensitivity() const { return q_cs; }

private:
    QStringMatcherPrivate *d_ptr = nullptr;
    BobUI::CaseSensitivity q_cs = BobUI::CaseSensitive;
    QString q_pattern;
    QStringView q_sv;
    uchar q_skiptable[256] = {};
};

BOBUI_END_NAMESPACE

#endif // QSTRINGMATCHER_H
