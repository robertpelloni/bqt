// Copyright (C) 2018 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#ifndef QCBORSTREAMWRITER_H
#define QCBORSTREAMWRITER_H

#include <BobUICore/qbytearray.h>
#include <BobUICore/qcborcommon.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringview.h>
#ifndef BOBUI_BOOTSTRAPPED
#include <BobUICore/qfloat16.h>
#endif

#include <memory>

BOBUI_REQUIRE_CONFIG(cborstreamwriter);

/* X11 headers use these values too, but as defines */
#if defined(False) && defined(True)
#  undef True
#  undef False
#endif

BOBUI_BEGIN_NAMESPACE

class QIODevice;

class QCborStreamWriterPrivate;
class Q_CORE_EXPORT QCborStreamWriter
{
public:
    explicit QCborStreamWriter(QIODevice *device);
    explicit QCborStreamWriter(QByteArray *data);
    ~QCborStreamWriter();
    Q_DISABLE_COPY(QCborStreamWriter)

    void setDevice(QIODevice *device);
    QIODevice *device() const;

    void append(quint64 u);
    void append(qint64 i);
    void append(QCborNegativeInteger n);
#if !BOBUI_CORE_REMOVED_SINCE(6, 10)   // wasn't a template until 6.10
    Q_WEAK_OVERLOAD
#endif
    void append(const QByteArray &ba)       { appendByteString(ba.constData(), ba.size()); }
    void append(QByteArrayView ba)          { appendByteString(ba.data(), ba.size()); }
    void append(QLatin1StringView str);
    void append(QStringView str);
    void append(QUtf8StringView str)        { appendTextString(str.data(), str.size()); }
    void append(QCborTag tag);
    void append(QCborKnownTags tag)         { append(QCborTag(tag)); }
    void append(QCborSimpleType st);
    void append(std::nullptr_t)             { append(QCborSimpleType::Null); }
#ifndef BOBUI_BOOTSTRAPPED
    void append(qfloat16 f);
#endif
    void append(float f);
    void append(double d);

    void appendByteString(const char *data, qsizetype len);
    void appendTextString(const char *utf8, qsizetype len);

    // convenience
    void append(bool b)     { append(b ? QCborSimpleType::True : QCborSimpleType::False); }
    void appendNull()       { append(QCborSimpleType::Null); }
    void appendUndefined()  { append(QCborSimpleType::Undefined); }

#ifndef Q_QDOC
    // overloads to make normal code not complain
    void append(int i)      { append(qint64(i)); }
    void append(uint u)     { append(quint64(u)); }
#endif
#ifndef BOBUI_NO_CAST_FROM_ASCII
    void append(const char *str, qsizetype size = -1)
    { appendTextString(str, (str && size == -1)  ? int(strlen(str)) : size); }
#endif

    void startArray();
    void startArray(quint64 count);
    bool endArray();
    void startMap();
    void startMap(quint64 count);
    bool endMap();

    // no API for encoding chunked strings

private:
    std::unique_ptr<QCborStreamWriterPrivate> d;
};

BOBUI_END_NAMESPACE

#endif // QCBORSTREAMWRITER_H
