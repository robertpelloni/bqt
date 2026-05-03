// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef BOBUIEXTSTREAM_H
#define BOBUIEXTSTREAM_H

#include <BobUICore/qiodevicebase.h>
#include <BobUICore/qchar.h>
#include <BobUICore/qstringconverter_base.h>

#include <memory>

#include <stdio.h>

#if 0
// the macros around the class name throw off syncbobui:
#pragma bobui_class(BOBUIextStream)
#endif

#ifdef Status
#error bobuiextstream.h must be included before any header file that defines Status
#endif

BOBUI_BEGIN_NAMESPACE

class QIODevice;
class QLocale;
class QString;

#if !BOBUI_DEPRECATED_SINCE(6, 9)
# define BOBUI_NO_INHERITABLE_TEXT_STREAM
#endif

#ifdef BOBUI_NO_INHERITABLE_TEXT_STREAM
#  define BOBUI_TEXT_STREAM_FINAL final
#else
#  define BOBUI_TEXT_STREAM_FINAL
#endif

class BOBUIextStreamPrivate;
class Q_CORE_EXPORT BOBUIextStream BOBUI_TEXT_STREAM_FINAL : public QIODeviceBase
{
    Q_DECLARE_PRIVATE(BOBUIextStream)

public:
    enum RealNumberNotation {
        SmartNotation,
        FixedNotation,
        ScientificNotation
    };
    enum FieldAlignment {
        AlignLeft,
        AlignRight,
        AlignCenter,
        AlignAccountingStyle
    };
    enum Status {
        Ok,
        ReadPastEnd,
        ReadCorruptData,
        WriteFailed
    };
    enum NumberFlag {
        ShowBase = 0x1,
        ForcePoint = 0x2,
        ForceSign = 0x4,
        UppercaseBase = 0x8,
        UppercaseDigits = 0x10
    };
    Q_DECLARE_FLAGS(NumberFlags, NumberFlag)

    BOBUIextStream();
    explicit BOBUIextStream(QIODevice *device);
    explicit BOBUIextStream(FILE *fileHandle, OpenMode openMode = ReadWrite);
    explicit BOBUIextStream(QString *string, OpenMode openMode = ReadWrite);
    explicit BOBUIextStream(QByteArray *array, OpenMode openMode = ReadWrite);
    explicit BOBUIextStream(const QByteArray &array, OpenMode openMode = ReadOnly);
    BOBUI6_ONLY(virtual)
    ~BOBUIextStream();

    void setEncoding(QStringConverter::Encoding encoding);
    QStringConverter::Encoding encoding() const;
    void setAutoDetectUnicode(bool enabled);
    bool autoDetectUnicode() const;
    void setGenerateByteOrderMark(bool generate);
    bool generateByteOrderMark() const;

    void setLocale(const QLocale &locale);
    QLocale locale() const;

    void setDevice(QIODevice *device);
    QIODevice *device() const;

    void setString(QString *string, OpenMode openMode = ReadWrite);
    QString *string() const;

    Status status() const;
    void setStatus(Status status);
    void resetStatus();

    bool atEnd() const;
    void reset();
    void flush();
    bool seek(qint64 pos);
    qint64 pos() const;

    void skipWhiteSpace();

    QString readLine(qint64 maxlen = 0);
    bool readLineInto(QString *line, qint64 maxlen = 0);
    QString readAll();
    QString read(qint64 maxlen);

    void setFieldAlignment(FieldAlignment alignment);
    FieldAlignment fieldAlignment() const;

    void setPadChar(QChar ch);
    QChar padChar() const;

    void setFieldWidth(int width);
    int fieldWidth() const;

    void setNumberFlags(NumberFlags flags);
    NumberFlags numberFlags() const;

    void setIntegerBase(int base);
    int integerBase() const;

    void setRealNumberNotation(RealNumberNotation notation);
    RealNumberNotation realNumberNotation() const;

    void setRealNumberPrecision(int precision);
    int realNumberPrecision() const;

    BOBUIextStream &operator>>(QChar &ch);
    BOBUIextStream &operator>>(char &ch);
    BOBUIextStream &operator>>(char16_t &ch)
    { QChar c; *this >> c; ch = c.unicode(); return *this; }
    BOBUIextStream &operator>>(signed short &i);
    BOBUIextStream &operator>>(unsigned short &i);
    BOBUIextStream &operator>>(signed int &i);
    BOBUIextStream &operator>>(unsigned int &i);
    BOBUIextStream &operator>>(signed long &i);
    BOBUIextStream &operator>>(unsigned long &i);
    BOBUIextStream &operator>>(qlonglong &i);
    BOBUIextStream &operator>>(qulonglong &i);
    BOBUIextStream &operator>>(float &f);
    BOBUIextStream &operator>>(double &f);
    BOBUIextStream &operator>>(QString &s);
    BOBUIextStream &operator>>(QByteArray &array);
    BOBUIextStream &operator>>(char *c);

    BOBUIextStream &operator<<(QChar ch);
    BOBUIextStream &operator<<(char ch);
    BOBUIextStream &operator<<(char16_t ch) { return *this << QChar(ch); }
    BOBUIextStream &operator<<(signed short i);
    BOBUIextStream &operator<<(unsigned short i);
    BOBUIextStream &operator<<(signed int i);
    BOBUIextStream &operator<<(unsigned int i);
    BOBUIextStream &operator<<(signed long i);
    BOBUIextStream &operator<<(unsigned long i);
    BOBUIextStream &operator<<(qlonglong i);
    BOBUIextStream &operator<<(qulonglong i);
    BOBUIextStream &operator<<(float f);
    BOBUIextStream &operator<<(double f);
    BOBUIextStream &operator<<(const QString &s);
    BOBUIextStream &operator<<(QStringView s);
    BOBUIextStream &operator<<(QLatin1StringView s);
    BOBUIextStream &operator<<(const QByteArray &array);
    BOBUIextStream &operator<<(const char *c);
    BOBUIextStream &operator<<(const void *ptr);

    explicit operator bool() const noexcept { return status() == Ok; }

private:
    Q_DISABLE_COPY(BOBUIextStream)
    friend class QDebugStateSaverPrivate;
    friend class QDebug;

    std::unique_ptr<BOBUIextStreamPrivate> d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextStream::NumberFlags)

/*****************************************************************************
  BOBUIextStream manipulators
 *****************************************************************************/

typedef BOBUIextStream & (*BOBUIextStreamFunction)(BOBUIextStream &);// manipulator function
typedef void (BOBUIextStream::*BOBUISMFI)(int); // manipulator w/int argument
typedef void (BOBUIextStream::*BOBUISMFC)(QChar); // manipulator w/QChar argument


class Q_CORE_EXPORT BOBUIextStreamManipulator
{
public:
    constexpr BOBUIextStreamManipulator(BOBUISMFI m, int a) noexcept : mf(m), mc(nullptr), arg(a), ch() {}
    constexpr BOBUIextStreamManipulator(BOBUISMFC m, QChar c) noexcept : mf(nullptr), mc(m), arg(-1), ch(c) {}
    void exec(BOBUIextStream &s) { if (mf) { (s.*mf)(arg); } else { (s.*mc)(ch); } }

private:
    BOBUISMFI mf;                                        // BOBUIextStream member function
    BOBUISMFC mc;                                        // BOBUIextStream member function
    int arg;                                          // member function argument
    QChar ch;
};

inline BOBUIextStream &operator>>(BOBUIextStream &s, BOBUIextStreamFunction f)
{ return (*f)(s); }

inline BOBUIextStream &operator<<(BOBUIextStream &s, BOBUIextStreamFunction f)
{ return (*f)(s); }

inline BOBUIextStream &operator<<(BOBUIextStream &s, BOBUIextStreamManipulator m)
{ m.exec(s); return s; }

namespace BobUI {
Q_CORE_EXPORT BOBUIextStream &bin(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &oct(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &dec(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &hex(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &showbase(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &forcesign(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &forcepoint(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &noshowbase(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &noforcesign(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &noforcepoint(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &uppercasebase(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &uppercasedigits(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &lowercasebase(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &lowercasedigits(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &fixed(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &scientific(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &left(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &right(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &center(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &endl(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &flush(BOBUIextStream &s);
Q_CORE_EXPORT BOBUIextStream &reset(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &bom(BOBUIextStream &s);

Q_CORE_EXPORT BOBUIextStream &ws(BOBUIextStream &s);

} // namespace BobUI

inline BOBUIextStreamManipulator qSetFieldWidth(int width)
{
    BOBUISMFI func = &BOBUIextStream::setFieldWidth;
    return BOBUIextStreamManipulator(func,width);
}

inline BOBUIextStreamManipulator qSetPadChar(QChar ch)
{
    BOBUISMFC func = &BOBUIextStream::setPadChar;
    return BOBUIextStreamManipulator(func, ch);
}

inline BOBUIextStreamManipulator qSetRealNumberPrecision(int precision)
{
    BOBUISMFI func = &BOBUIextStream::setRealNumberPrecision;
    return BOBUIextStreamManipulator(func, precision);
}

BOBUI_END_NAMESPACE

#endif // BOBUIEXTSTREAM_H
