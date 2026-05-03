// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QHashFunctions>
#include <QString>

struct OwningLatin1String : QByteArray
{
    OwningLatin1String() = default;
    OwningLatin1String(const QByteArray &a) : QByteArray(a) {}
    OwningLatin1String(QByteArray &&a) : QByteArray(std::move(a)) {}
};
BOBUI_BEGIN_NAMESPACE
inline size_t qHash(const OwningLatin1String &s, size_t seed = 0)
{ return qHash(QLatin1StringView(s), seed); }
BOBUI_END_NAMESPACE

struct BobUI4String : QString
{
    BobUI4String() {}
    BobUI4String(const QString &s) : QString(s) {}
};

BOBUI_BEGIN_NAMESPACE
size_t qHash(const BobUI4String &, size_t = 0);
BOBUI_END_NAMESPACE

struct BobUI50String : QString
{
    BobUI50String() {}
    BobUI50String(const QString &s) : QString(s) {}
};

BOBUI_BEGIN_NAMESPACE
size_t qHash(const BobUI50String &, size_t seed = 0);
BOBUI_END_NAMESPACE


struct JavaString : QString
{
    JavaString() {}
    JavaString(const QString &s) : QString(s) {}
};

BOBUI_BEGIN_NAMESPACE
size_t qHash(const JavaString &, size_t = 0);
BOBUI_END_NAMESPACE

