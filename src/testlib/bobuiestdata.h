// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTDATA_H
#define BOBUIESTDATA_H

#include <BobUITest/bobuitestglobal.h>

#include <BobUICore/qmetatype.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIestTable;
class BOBUIestDataPrivate;

class Q_TESTLIB_EXPORT BOBUIestData
{
public:
    ~BOBUIestData();

    void append(int type, const void *data);
    void *data(int index) const;
    const char *dataTag() const;
    BOBUIestTable *parent() const;
    int dataCount() const;

private:
    friend class BOBUIestTable;
    BOBUIestData(const char *tag, BOBUIestTable *parent);

    Q_DISABLE_COPY(BOBUIestData)

    BOBUIestDataPrivate *d;
};

template<typename T>
BOBUIestData &operator<<(BOBUIestData &data, const T &value)
{
    data.append(qMetaTypeId<T>(), &value);
    return data;
}

inline BOBUIestData &operator<<(BOBUIestData &data, const char * value)
{
    QString str = QString::fromUtf8(value);
    data.append(QMetaType::QString, &str);
    return data;
}

#ifdef __cpp_char8_t
Q_WEAK_OVERLOAD
inline BOBUIestData &operator<<(BOBUIestData &data, const char8_t *value)
{
    return data << reinterpret_cast<const char *>(value);
}
#endif

#ifdef BOBUI_USE_QSTRINGBUILDER
template<typename A, typename B>
inline BOBUIestData &operator<<(BOBUIestData &data, const QStringBuilder<A, B> &value)
{
    return data << typename QConcatenable<QStringBuilder<A, B> >::ConvertTo(value);
}
#endif

BOBUI_END_NAMESPACE

#endif
