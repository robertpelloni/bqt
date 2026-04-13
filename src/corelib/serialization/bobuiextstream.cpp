// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

//#define BOBUIEXTSTREAM_DEBUG

/*!
    \class BOBUIextStream
    \inmodule BobUICore

    \brief The BOBUIextStream class provides a convenient interface for
    reading and writing text.

    \ingroup io
    \ingroup string-processing
    \ingroup bobuiserialization
    \reentrant

    BOBUIextStream can operate on a QIODevice, a QByteArray or a
    QString. Using BOBUIextStream's streaming operators, you can
    conveniently read and write words, lines and numbers. For
    generating text, BOBUIextStream supports formatting options for field
    padding and alignment, and formatting of numbers. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 0

    It's also common to use BOBUIextStream to read console input and write
    console output. BOBUIextStream is locale aware, and will automatically decode
    standard input using the correct encoding. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 1

    Besides using BOBUIextStream's constructors, you can also set the
    device or string BOBUIextStream operates on by calling setDevice() or
    setString(). You can seek to a position by calling seek(), and
    atEnd() will return true when there is no data left to be read. If
    you call flush(), BOBUIextStream will empty all data from its write
    buffer into the device and call flush() on the device.

    Internally, BOBUIextStream uses a Unicode based buffer, and
    QStringConverter is used by BOBUIextStream to automatically support
    different encodings. By default, UTF-8
    is used for reading and writing, but you can also set the encoding by
    calling setEncoding(). Automatic Unicode detection is also
    supported. When this feature is enabled (the default behavior),
    BOBUIextStream will detect the UTF-8, UTF-16 or the UTF-32 BOM (Byte Order Mark) and
    switch to the appropriate UTF encoding when reading. BOBUIextStream
    does not write a BOM by default, but you can enable this by calling
    setGenerateByteOrderMark(true). When BOBUIextStream operates on a QString
    directly, the encoding is disabled.

    There are three general ways to use BOBUIextStream when reading text
    files:

    \list

    \li Chunk by chunk, by calling readLine() or readAll().

    \li Word by word. BOBUIextStream supports streaming into \l {QString}s,
    \l {QByteArray}s and char* buffers. Words are delimited by space, and
    leading white space is automatically skipped.

    \li Character by character, by streaming into QChar or char types.
    This method is often used for convenient input handling when
    parsing files, independent of character encoding and end-of-line
    semantics. To skip white space, call skipWhiteSpace().

    \endlist

    Since the text stream uses a buffer, you should not read from
    the stream using the implementation of a superclass. For instance,
    if you have a QFile and read from it directly using
    QFile::readLine() instead of using the stream, the text stream's
    internal position will be out of sync with the file's position.

    By default, when reading numbers from a stream of text,
    BOBUIextStream will automatically detect the number's base
    representation. For example, if the number starts with "0x", it is
    assumed to be in hexadecimal form. If it starts with the digits
    1-9, it is assumed to be in decimal form, and so on. You can set
    the integer base, thereby disabling the automatic detection, by
    calling setIntegerBase(). Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 2

    BOBUIextStream supports many formatting options for generating text.
    You can set the field width and pad character by calling
    setFieldWidth() and setPadChar(). Use setFieldAlignment() to set
    the alignment within each field. For real numbers, call
    setRealNumberNotation() and setRealNumberPrecision() to set the
    notation (SmartNotation, ScientificNotation, FixedNotation) and precision in
    digits of the generated number. Some extra number formatting
    options are also available through setNumberFlags().

    \target BOBUIextStream manipulators

    Like \c <iostream> in the standard C++ library, BOBUIextStream also
    defines several global manipulator functions:

    \table
    \header \li Manipulator        \li Description
    \row    \li BobUI::bin             \li Same as setIntegerBase(2).
    \row    \li BobUI::oct             \li Same as setIntegerBase(8).
    \row    \li BobUI::dec             \li Same as setIntegerBase(10).
    \row    \li BobUI::hex             \li Same as setIntegerBase(16).
    \row    \li BobUI::showbase        \li Same as setNumberFlags(numberFlags() | ShowBase).
    \row    \li BobUI::forcesign       \li Same as setNumberFlags(numberFlags() | ForceSign).
    \row    \li BobUI::forcepoint      \li Same as setNumberFlags(numberFlags() | ForcePoint).
    \row    \li BobUI::noshowbase      \li Same as setNumberFlags(numberFlags() & ~ShowBase).
    \row    \li BobUI::noforcesign     \li Same as setNumberFlags(numberFlags() & ~ForceSign).
    \row    \li BobUI::noforcepoint    \li Same as setNumberFlags(numberFlags() & ~ForcePoint).
    \row    \li BobUI::uppercasebase   \li Same as setNumberFlags(numberFlags() | UppercaseBase).
    \row    \li BobUI::uppercasedigits \li Same as setNumberFlags(numberFlags() | UppercaseDigits).
    \row    \li BobUI::lowercasebase   \li Same as setNumberFlags(numberFlags() & ~UppercaseBase).
    \row    \li BobUI::lowercasedigits \li Same as setNumberFlags(numberFlags() & ~UppercaseDigits).
    \row    \li BobUI::fixed           \li Same as setRealNumberNotation(FixedNotation).
    \row    \li BobUI::scientific      \li Same as setRealNumberNotation(ScientificNotation).
    \row    \li BobUI::left            \li Same as setFieldAlignment(AlignLeft).
    \row    \li BobUI::right           \li Same as setFieldAlignment(AlignRight).
    \row    \li BobUI::center          \li Same as setFieldAlignment(AlignCenter).
    \row    \li BobUI::endl            \li Same as operator<<('\\n') and flush().
    \row    \li BobUI::flush           \li Same as flush().
    \row    \li BobUI::reset           \li Same as reset().
    \row    \li BobUI::ws              \li Same as skipWhiteSpace().
    \row    \li BobUI::bom             \li Same as setGenerateByteOrderMark(true).
    \endtable

    In addition, BobUI provides three global manipulators that take a
    parameter: qSetFieldWidth(), qSetPadChar(), and
    qSetRealNumberPrecision().

    \sa QDataStream, QIODevice, QFile, QBuffer, BOBUIcpSocket
*/

/*! \enum BOBUIextStream::RealNumberNotation

    This enum specifies which notations to use for expressing \c
    float and \c double as strings.

    \value ScientificNotation Scientific notation (\c{printf()}'s \c %e flag).
    \value FixedNotation Fixed-point notation (\c{printf()}'s \c %f flag).
    \value SmartNotation Scientific or fixed-point notation, depending on which makes most sense (\c{printf()}'s \c %g flag).

    \sa setRealNumberNotation()
*/

/*! \enum BOBUIextStream::FieldAlignment

    This enum specifies how to align text in fields when the field is
    wider than the text that occupies it.

    \value AlignLeft  Pad on the right side of fields.
    \value AlignRight  Pad on the left side of fields.
    \value AlignCenter  Pad on both sides of field.
    \value AlignAccountingStyle  Same as AlignRight, except that the
                                 sign of a number is flush left.

    \sa setFieldAlignment()
*/

/*! \enum BOBUIextStream::NumberFlag

    This enum specifies various flags that can be set to affect the
    output of integers, \c{float}s, and \c{double}s.

    \value ShowBase  Show the base as a prefix if the base
                     is 16 ("0x"), 8 ("0"), or 2 ("0b").
    \value ForcePoint  Always put the decimal separator in numbers, even if
                       there are no decimals.
    \value ForceSign  Always put the sign in numbers, even for positive numbers.
    \value UppercaseBase  Use uppercase versions of base prefixes ("0X", "0B").
    \value UppercaseDigits  Use uppercase letters for expressing
                            digits 10 to 35 instead of lowercase.

    \sa setNumberFlags()
*/

/*! \enum BOBUIextStream::Status

    This enum describes the current status of the text stream.

    \value Ok               The text stream is operating normally.
    \value ReadPastEnd      The text stream has read past the end of the
                            data in the underlying device.
    \value ReadCorruptData  The text stream has read corrupt data.
    \value WriteFailed      The text stream cannot write to the underlying device.

    \sa status()
*/

#include "bobuiextstream.h"
#include "private/bobuiextstream_p.h"
#include "qbuffer.h"
#include "qfile.h"
#include "qnumeric.h"
#include "qvarlengtharray.h"
#include <private/qdebug_p.h>
#include <private/qnumeric_p.h>
#include <private/bobuiools_p.h>

#include <locale.h>
#include "private/qlocale_p.h"
#include "private/qstringconverter_p.h"

#include <stdlib.h>
#include <limits.h>
#include <new>

// A precondition macro
#define Q_VOID
#define CHECK_VALID_STREAM(x) do { \
    if (!d->string && !d->device) { \
        qWarning("BOBUIextStream: No device"); \
        return x; \
    } } while (0)

// Base implementations of operator>> for ints and reals
#define IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(type) do { \
    Q_D(BOBUIextStream); \
    CHECK_VALID_STREAM(*this); \
    qulonglong tmp; \
    switch (d->getNumber(&tmp)) { \
    case BOBUIextStreamPrivate::npsOk: \
        i = (type)tmp; \
        break; \
    case BOBUIextStreamPrivate::npsMissingDigit: \
    case BOBUIextStreamPrivate::npsInvalidPrefix: \
        i = (type)0; \
        setStatus(atEnd() ? BOBUIextStream::ReadPastEnd : BOBUIextStream::ReadCorruptData); \
        break; \
    } \
    return *this; } while (0)

#define IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR(type) do { \
    Q_D(BOBUIextStream); \
    CHECK_VALID_STREAM(*this); \
    double tmp; \
    if (d->getReal(&tmp)) { \
        f = (type)tmp; \
    } else { \
        f = (type)0; \
        setStatus(atEnd() ? BOBUIextStream::ReadPastEnd : BOBUIextStream::ReadCorruptData); \
    } \
    return *this; } while (0)

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;
using namespace BobUIMiscUtils;

static const qsizetype BOBUIEXTSTREAM_BUFFERSIZE = 16384;

//-------------------------------------------------------------------

/*!
    \internal
*/
BOBUIextStreamPrivate::BOBUIextStreamPrivate(BOBUIextStream *q_ptr)
    : readConverterSavedStateOffset(0),
      locale(QLocale::c())
{
    this->q_ptr = q_ptr;
    reset();
}

/*!
    \internal
*/
BOBUIextStreamPrivate::~BOBUIextStreamPrivate()
{
    disconnectFromDevice();
    if (deleteDevice) {
#ifndef BOBUI_NO_QOBJECT
        device->blockSignals(true);
#endif
        delete device;
    }
}

void BOBUIextStreamPrivate::Params::reset()
{
    realNumberPrecision = 6;
    integerBase = 0;
    fieldWidth = 0;
    padChar = u' ';
    fieldAlignment = BOBUIextStream::AlignRight;
    realNumberNotation = BOBUIextStream::SmartNotation;
    numberFlags = { };
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::reset()
{
    params.reset();

    device = nullptr;
    deleteDevice = false;
    string = nullptr;
    stringOffset = 0;
    stringOpenMode = BOBUIextStream::NotOpen;

    readBufferOffset = 0;
    readBufferStartDevicePos = 0;
    lastTokenSize = 0;

    hasWrittenData = false;
    generateBOM = false;
    encoding = QStringConverter::Utf8;
    toUtf16 = QStringDecoder(encoding);
    fromUtf16 = QStringEncoder(encoding);
    autoDetectUnicode = true;
}

void BOBUIextStreamPrivate::setupDevice(QIODevice *device)
{
    disconnectFromDevice();

#ifndef BOBUI_NO_QOBJECT
    if (device) {
        // Explicitly set a direct connection (though it would have been so
        // anyway) so that BOBUIextStream can be used from multiple threads when the
        // application code is handling synchronization (see also BOBUIBUG-12055).
        aboutToCloseConnection = QObject::connect(
                device, &QIODevice::aboutToClose, device, [this] { flushWriteBuffer(); },
                BobUI::DirectConnection);
    }
#else
    Q_UNUSED(device);
#endif
}

void BOBUIextStreamPrivate::disconnectFromDevice()
{
#ifndef BOBUI_NO_QOBJECT
    QObject::disconnect(aboutToCloseConnection);
    aboutToCloseConnection = {};
#endif
}

/*!
    \internal
*/
bool BOBUIextStreamPrivate::fillReadBuffer(qint64 maxBytes)
{
    // no buffer next to the QString itself; this function should only
    // be called internally, for devices.
    Q_ASSERT(!string);
    Q_ASSERT(device);

    // handle text translation and bypass the Text flag in the device.
    bool textModeEnabled = device->isTextModeEnabled();
    if (textModeEnabled)
        device->setTextModeEnabled(false);

    // read raw data into a temporary buffer
    char buf[BOBUIEXTSTREAM_BUFFERSIZE];
    qint64 bytesRead = 0;
#if defined(Q_OS_WIN)
    // On Windows, there is no non-blocking stdin - so we fall back to reading
    // lines instead. If there is no QOBJECT, we read lines for all sequential
    // devices; otherwise, we read lines only for stdin.
    QFile *file = 0;
    Q_UNUSED(file);
    if (device->isSequential()
#if !defined(BOBUI_NO_QOBJECT)
        && (file = qobject_cast<QFile *>(device)) && file->handle() == 0
#endif
        ) {
        if (maxBytes != -1)
            bytesRead = device->readLine(buf, qMin<qint64>(sizeof(buf), maxBytes));
        else
            bytesRead = device->readLine(buf, sizeof(buf));
    } else
#endif
    {
        if (maxBytes != -1)
            bytesRead = device->read(buf, qMin<qint64>(sizeof(buf), maxBytes));
        else
            bytesRead = device->read(buf, sizeof(buf));
    }

    // reset the Text flag.
    if (textModeEnabled)
        device->setTextModeEnabled(true);

    if (bytesRead <= 0)
        return false;

#ifndef BOBUI_BOOTSTRAPPED
    if (autoDetectUnicode) {
        autoDetectUnicode = false;

        auto e = QStringConverter::encodingForData(QByteArrayView(buf, bytesRead));
        // QStringConverter::Locale implies unknown, so keep the current encoding
        if (e) {
            encoding = *e;
            toUtf16 = QStringDecoder(encoding);
            fromUtf16 = QStringEncoder(encoding);
        }
    }
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::fillReadBuffer(), using %s encoding", QStringConverter::nameForEncoding(encoding));
#endif
#endif

#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::fillReadBuffer(), device->read(\"%s\", %d) == %d",
           BobUIDebugUtils::toPrintable(buf, bytesRead, 32).constData(),
           int(sizeof(buf)), int(bytesRead));
#endif

    qsizetype oldReadBufferSize = readBuffer.size();
    readBuffer += toUtf16(QByteArrayView(buf, bytesRead));

    // remove all '\r\n' in the string.
    if (readBuffer.size() > oldReadBufferSize && textModeEnabled) {
        QChar CR = u'\r';
        QChar *writePtr = readBuffer.data() + oldReadBufferSize;
        QChar *readPtr = readBuffer.data() + oldReadBufferSize;
        QChar *endPtr = readBuffer.data() + readBuffer.size();

        qsizetype n = oldReadBufferSize;
        if (readPtr < endPtr) {
            // Cut-off to avoid unnecessary self-copying.
            while (*readPtr++ != CR) {
                ++n;
                if (++writePtr == endPtr)
                    break;
            }
        }
        while (readPtr < endPtr) {
            QChar ch = *readPtr++;
            if (ch != CR) {
                *writePtr++ = ch;
            } else {
                if (n < readBufferOffset)
                    --readBufferOffset;
                --bytesRead;
            }
            ++n;
        }
        readBuffer.resize(writePtr - readBuffer.data());
    }

#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::fillReadBuffer() read %d bytes from device. readBuffer = [%s]",
           int(bytesRead),
           BobUIDebugUtils::toPrintable(readBuffer.toLatin1(), readBuffer.size(),
                                     readBuffer.size()).constData());
#endif
    return true;
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::resetReadBuffer()
{
    readBuffer.clear();
    readBufferOffset = 0;
    readBufferStartDevicePos = (device ? device->pos() : 0);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::flushWriteBuffer()
{
    // no buffer next to the QString itself; this function should only
    // be called internally, for devices.
    if (string || !device)
        return;

    // Stream went bye-bye already. Appending further data may succeed again,
    // but would create a corrupted stream anyway.
    if (status != BOBUIextStream::Ok)
        return;

    if (writeBuffer.isEmpty())
        return;

#if defined (Q_OS_WIN)
    // handle text translation and bypass the Text flag in the device.
    bool textModeEnabled = device->isTextModeEnabled();
    if (textModeEnabled) {
        device->setTextModeEnabled(false);
        writeBuffer.replace(u'\n', "\r\n"_L1);
    }
#endif

    QByteArray data = fromUtf16(writeBuffer);
    writeBuffer.clear();
    hasWrittenData = true;

    // write raw data to the device
    qint64 bytesWritten = device->write(data);
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::flushWriteBuffer(), device->write(\"%s\") == %d",
           BobUIDebugUtils::toPrintable(data.constData(), data.size(), 32).constData(),
           int(bytesWritten));
#endif

#if defined (Q_OS_WIN)
    // reset the text flag
    if (textModeEnabled)
        device->setTextModeEnabled(true);
#endif

    if (bytesWritten <= 0) {
        status = BOBUIextStream::WriteFailed;
        return;
    }

    // flush the file
#ifndef BOBUI_NO_QOBJECT
    QFileDevice *file = qobject_cast<QFileDevice *>(device);
    bool flushed = !file || file->flush();
#else
    bool flushed = true;
#endif

#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::flushWriteBuffer() wrote %d bytes", int(bytesWritten));
#endif
    if (!flushed || bytesWritten != qint64(data.size()))
        status = BOBUIextStream::WriteFailed;
}

QString BOBUIextStreamPrivate::read(qsizetype maxlen)
{
    QString ret;
    if (string) {
        lastTokenSize = qMin(maxlen, string->size() - stringOffset);
        ret = string->mid(stringOffset, lastTokenSize);
    } else {
        while (readBuffer.size() - readBufferOffset < maxlen && fillReadBuffer()) {}
        lastTokenSize = qMin(maxlen, readBuffer.size() - readBufferOffset);
        ret = readBuffer.mid(readBufferOffset, lastTokenSize);
    }
    consumeLastToken();

#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::read() maxlen = %d, token length = %d",
           int(maxlen), int(ret.length()));
#endif
    return ret;
}

/*!
    \internal

    Scans no more than \a maxlen QChars in the current buffer for the
    first \a delimiter. Stores a pointer to the start offset of the
    token in \a ptr, and the length in QChars in \a length.
*/
bool BOBUIextStreamPrivate::scan(const QChar **ptr, qsizetype *length, qsizetype maxlen,
                              TokenDelimiter delimiter)
{
    qsizetype totalSize = 0;
    qsizetype delimSize = 0;
    bool consumeDelimiter = false;
    bool foundToken = false;
    qsizetype startOffset = device ? readBufferOffset : stringOffset;
    QChar lastChar;

    do {
        qsizetype endOffset;
        const QChar *chPtr;
        if (device) {
            chPtr = readBuffer.constData();
            endOffset = readBuffer.size();
        } else {
            chPtr = string->constData();
            endOffset = string->size();
        }
        chPtr += startOffset;

        for (; !foundToken && startOffset < endOffset && (!maxlen || totalSize < maxlen); ++startOffset) {
            const QChar ch = *chPtr++;
            ++totalSize;

            switch (delimiter) {
            case Space:
                if (ch.isSpace()) {
                    foundToken = true;
                    delimSize = 1;
                }
                break;
            case NotSpace:
                if (!ch.isSpace()) {
                    foundToken = true;
                    delimSize = 1;
                }
                break;
            case EndOfLine:
                if (ch == u'\n') {
                    foundToken = true;
                    delimSize = (lastChar == u'\r') ? 2 : 1;
                    consumeDelimiter = true;
                }
                lastChar = ch;
                break;
            }
        }
    } while (!foundToken
             && (!maxlen || totalSize < maxlen)
             && device && fillReadBuffer());

    if (totalSize == 0) {
#if defined (BOBUIEXTSTREAM_DEBUG)
        qDebug("BOBUIextStreamPrivate::scan() reached the end of input.");
#endif
        return false;
    }

    // if we find a '\r' at the end of the data when reading lines,
    // don't make it part of the line.
    if (delimiter == EndOfLine && totalSize > 0 && !foundToken) {
        if (((string && stringOffset + totalSize == string->size()) || (device && device->atEnd()))
            && lastChar == u'\r') {
            consumeDelimiter = true;
            ++delimSize;
        }
    }

    // set the read offset and length of the token
    if (length)
        *length = totalSize - delimSize;
    if (ptr)
        *ptr = readPtr();

    // update last token size. the callee will call consumeLastToken() when
    // done.
    lastTokenSize = totalSize;
    if (!consumeDelimiter)
        lastTokenSize -= delimSize;

#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::scan(%p, %p, %d, %x) token length = %d, delimiter = %d",
           ptr, length, int(maxlen), uint(delimiter), int(totalSize - delimSize), int(delimSize));
#endif
    return true;
}

/*!
    \internal
*/
inline const QChar *BOBUIextStreamPrivate::readPtr() const
{
    Q_ASSERT(readBufferOffset <= readBuffer.size());
    if (string)
        return string->constData() + stringOffset;
    return readBuffer.constData() + readBufferOffset;
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::consumeLastToken()
{
    if (lastTokenSize)
        consume(lastTokenSize);
    lastTokenSize = 0;
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::consume(qsizetype size)
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStreamPrivate::consume(%d)", int(size));
#endif
    if (string) {
        stringOffset += size;
        if (stringOffset > string->size())
            stringOffset = string->size();
    } else {
        readBufferOffset += size;
        if (readBufferOffset >= readBuffer.size()) {
            readBufferOffset = 0;
            readBuffer.clear();
            saveConverterState(device->pos());
        } else if (readBufferOffset > BOBUIEXTSTREAM_BUFFERSIZE) {
            readBuffer = readBuffer.remove(0,readBufferOffset);
            readConverterSavedStateOffset += readBufferOffset;
            readBufferOffset = 0;
        }
    }
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::saveConverterState(qint64 newPos)
{
    // ### Hack, FIXME
    memcpy((void *)&savedToUtf16, (void *)&toUtf16, sizeof(QStringDecoder));
    readBufferStartDevicePos = newPos;
    readConverterSavedStateOffset = 0;
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::restoreToSavedConverterState()
{
    if (savedToUtf16.isValid())
        memcpy((void *)&toUtf16, (void *)&savedToUtf16, sizeof(QStringDecoder));
    else
        toUtf16.resetState();
    savedToUtf16 = QStringDecoder();
}

/*!
    \internal
*/
template <typename Appendable>
void BOBUIextStreamPrivate::writeImpl(Appendable s)
{
    if (string) {
        // ### What about seek()??
        string->append(s);
    } else {
        writeBuffer.append(s);
        if (writeBuffer.size() > BOBUIEXTSTREAM_BUFFERSIZE)
            flushWriteBuffer();
    }
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::write(QStringView s)
{
    writeImpl(s);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::write(QChar ch)
{
    writeImpl(ch);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::write(QLatin1StringView data)
{
    writeImpl(data);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::writePadding(qsizetype len)
{
    if (string) {
        // ### What about seek()??
        string->resize(string->size() + len, params.padChar);
    } else {
        writeBuffer.resize(writeBuffer.size() + len, params.padChar);
        if (writeBuffer.size() > BOBUIEXTSTREAM_BUFFERSIZE)
            flushWriteBuffer();
    }
}

/*!
    \internal
*/
inline bool BOBUIextStreamPrivate::getChar(QChar *ch)
{
    if ((string && stringOffset == string->size())
        || (device && readBuffer.isEmpty() && !fillReadBuffer())) {
        if (ch)
            *ch = QChar();
        return false;
    }
    if (ch)
        *ch = *readPtr();
    consume(1);
    return true;
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::ungetChar(QChar ch)
{
    if (string) {
        if (stringOffset == 0)
            string->prepend(ch);
        else
            (*string)[--stringOffset] = ch;
        return;
    }

    if (readBufferOffset == 0) {
        readBuffer.prepend(ch);
        return;
    }

    readBuffer[--readBufferOffset] = ch;
}

/*!
    \internal
*/
inline void BOBUIextStreamPrivate::putChar(QChar ch)
{
    if (params.fieldWidth > 0)
        putString(QStringView{&ch, 1});
    else
        write(ch);
}


/*!
    \internal
*/
BOBUIextStreamPrivate::PaddingResult BOBUIextStreamPrivate::padding(qsizetype len) const
{
    Q_ASSERT(params.fieldWidth > len); // calling padding() when no padding is needed is an error

    qsizetype left = 0, right = 0;

    const qsizetype padSize = params.fieldWidth - len;

    switch (params.fieldAlignment) {
    case BOBUIextStream::AlignLeft:
        right = padSize;
        break;
    case BOBUIextStream::AlignRight:
    case BOBUIextStream::AlignAccountingStyle:
        left  = padSize;
        break;
    case BOBUIextStream::AlignCenter:
        left  = padSize/2;
        right = padSize - padSize/2;
        break;
    }
    return { left, right };
}

namespace {
template <typename StringView>
auto parseSign(StringView data, const QLocale &loc)
{
    struct R {
        StringView sign, rest;
        explicit operator bool() const noexcept { return !sign.isEmpty(); }
    };
    // This assumes that the size in UTF-16 (return value of QLocale functions)
    // and StringView is the same; in particular, it doesn't work for UTF-8!
    if (const QString sign = loc.negativeSign(); data.startsWith(sign))
        return R{data.first(sign.size()), data.sliced(sign.size())};
    if (const QString sign = loc.positiveSign(); data.startsWith(sign))
        return R{data.first(sign.size()), data.sliced(sign.size())};
    return R{nullptr, data};
}
} // unnamed namespace

/*!
    \internal
*/
template <typename StringView>
void BOBUIextStreamPrivate::putStringImpl(StringView data, PutStringMode mode)
{
    const bool number = mode == PutStringMode::Number;
    if (Q_UNLIKELY(params.fieldWidth > data.size())) {

        // handle padding:

        const PaddingResult pad = padding(data.size());

        if (params.fieldAlignment == BOBUIextStream::AlignAccountingStyle && number) {
            if (const auto r = parseSign(data, locale)) {
                // write the sign before the padding, then skip it later
                write(r.sign);
                data = r.rest;
            }
        }

        writePadding(pad.left);
        write(data);
        writePadding(pad.right);
    } else {
        write(data);
    }
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::putString(QLatin1StringView data, PutStringMode mode)
{
    putStringImpl(data, mode);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::putString(QStringView data, PutStringMode mode)
{
    putStringImpl(data, mode);
}

/*!
    \internal
*/
void BOBUIextStreamPrivate::putString(QUtf8StringView data, PutStringMode mode)
{
    putString(data.toString(), mode);
}

/*!
    Constructs a BOBUIextStream. Before you can use it for reading or
    writing, you must assign a device or a string.

    \sa setDevice(), setString()
*/
BOBUIextStream::BOBUIextStream()
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream()");
#endif
    Q_D(BOBUIextStream);
    d->status = Ok;
}

/*!
    Constructs a BOBUIextStream that operates on \a device.
*/
BOBUIextStream::BOBUIextStream(QIODevice *device)
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream(QIODevice *device == *%p)",
           device);
#endif
    Q_D(BOBUIextStream);
    d->device = device;
    d->setupDevice(device);
    d->status = Ok;
}

/*!
    Constructs a BOBUIextStream that operates on \a string, using \a
    openMode to define the open mode.
*/
BOBUIextStream::BOBUIextStream(QString *string, OpenMode openMode)
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream(QString *string == *%p, openMode = %d)",
           string, int(openMode.toInt()));
#endif
    Q_D(BOBUIextStream);
    d->string = string;
    d->stringOpenMode = openMode;
    d->status = Ok;
}

#ifndef BOBUI_BOOTSTRAPPED
/*!
    Constructs a BOBUIextStream that operates on \a array, using \a
    openMode to define the open mode. Internally, the array is wrapped
    by a QBuffer.
*/
BOBUIextStream::BOBUIextStream(QByteArray *array, OpenMode openMode)
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream(QByteArray *array == *%p, openMode = %d)",
           array, int(openMode.toInt()));
#endif
    Q_D(BOBUIextStream);
    d->device = new QBuffer(array);
    d->device->open(openMode);
    d->deleteDevice = true;
    d->setupDevice(d->device);
    d->status = Ok;
}

/*!
    Constructs a BOBUIextStream that operates on \a array, using \a
    openMode to define the open mode. The array is accessed as
    read-only, regardless of the values in \a openMode.

    This constructor is convenient for working on constant
    strings. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 3
*/
BOBUIextStream::BOBUIextStream(const QByteArray &array, OpenMode openMode)
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream(const QByteArray &array == *(%p), openMode = %d)",
           &array, int(openMode.toInt()));
#endif
    QBuffer *buffer = new QBuffer;
    buffer->setData(array);
    buffer->open(openMode);

    Q_D(BOBUIextStream);
    d->device = buffer;
    d->deleteDevice = true;
    d->setupDevice(d->device);
    d->status = Ok;
}
#endif

/*!
    Constructs a BOBUIextStream that operates on \a fileHandle, using \a
    openMode to define the open mode. Internally, a QFile is created
    to handle the FILE pointer.

    This constructor is useful for working directly with the common
    FILE based input and output streams: stdin, stdout and stderr. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 4
*/

BOBUIextStream::BOBUIextStream(FILE *fileHandle, OpenMode openMode)
    : d_ptr(new BOBUIextStreamPrivate(this))
{
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::BOBUIextStream(FILE *fileHandle = %p, openMode = %d)",
           fileHandle, int(openMode.toInt()));
#endif
    QFile *file = new QFile;
    // Discarding the return value of open; even if it failed
    // (and the file is not open), BOBUIextStream still reports `Ok`
    // for closed QIODevices, so there's nothing really to do here.
    (void)file->open(fileHandle, openMode);

    Q_D(BOBUIextStream);
    d->device = file;
    d->deleteDevice = true;
    d->setupDevice(d->device);
    d->status = Ok;
}

/*!
    Destroys the BOBUIextStream.

    If the stream operates on a device, flush() will be called
    implicitly. Otherwise, the device is unaffected.
*/
BOBUIextStream::~BOBUIextStream()
{
    Q_D(BOBUIextStream);
#if defined (BOBUIEXTSTREAM_DEBUG)
    qDebug("BOBUIextStream::~BOBUIextStream()");
#endif
    if (!d->writeBuffer.isEmpty())
        d->flushWriteBuffer();
}

/*!
    Resets BOBUIextStream's formatting options, bringing it back to its
    original constructed state. The device, string and any buffered
    data is left untouched.
*/
void BOBUIextStream::reset()
{
    Q_D(BOBUIextStream);

    d->params.reset();
}

/*!
    Flushes any buffered data waiting to be written to the device.

    If BOBUIextStream operates on a string, this function does nothing.
*/
void BOBUIextStream::flush()
{
    Q_D(BOBUIextStream);
    d->flushWriteBuffer();
}

/*!
    Seeks to the position \a pos in the device. Returns \c true on
    success; otherwise returns \c false.
*/
bool BOBUIextStream::seek(qint64 pos)
{
    Q_D(BOBUIextStream);
    d->lastTokenSize = 0;

    if (d->device) {
        // Empty the write buffer
        d->flushWriteBuffer();
        if (!d->device->seek(pos))
            return false;
        d->resetReadBuffer();

        d->toUtf16.resetState();
        d->fromUtf16.resetState();
        return true;
    }

    // string
    if (d->string && pos <= d->string->size()) {
        d->stringOffset = pos;
        return true;
    }
    return false;
}

/*!
    \since 4.2

    Returns the device position corresponding to the current position of the
    stream, or -1 if an error occurs (e.g., if there is no device or string,
    or if there's a device error).

    Because BOBUIextStream is buffered, this function may have to
    seek the device to reconstruct a valid device position. This
    operation can be expensive, so you may want to avoid calling this
    function in a tight loop.

    \sa seek()
*/
qint64 BOBUIextStream::pos() const
{
    Q_D(const BOBUIextStream);
    if (d->device) {
        // Cutoff
        if (d->readBuffer.isEmpty())
            return d->device->pos();
        if (d->device->isSequential())
            return 0;

        // Seek the device
        if (!d->device->seek(d->readBufferStartDevicePos))
            return qint64(-1);

        // Reset the read buffer
        BOBUIextStreamPrivate *thatd = const_cast<BOBUIextStreamPrivate *>(d);
        thatd->readBuffer.clear();

        thatd->restoreToSavedConverterState();
        if (d->readBufferStartDevicePos == 0)
            thatd->autoDetectUnicode = true;

        // Rewind the device to get to the current position Ensure that
        // readBufferOffset is unaffected by fillReadBuffer()
        qsizetype oldReadBufferOffset = d->readBufferOffset + d->readConverterSavedStateOffset;
        while (d->readBuffer.size() < oldReadBufferOffset) {
            if (!thatd->fillReadBuffer(1))
                return qint64(-1);
        }
        thatd->readBufferOffset = oldReadBufferOffset;
        thatd->readConverterSavedStateOffset = 0;

        // Return the device position.
        return d->device->pos();
    }

    if (d->string)
        return d->stringOffset;

    qWarning("BOBUIextStream::pos: no device");
    return qint64(-1);
}

/*!
    Reads and discards whitespace from the stream until either a
    non-space character is detected, or until atEnd() returns
    true. This function is useful when reading a stream character by
    character.

    Whitespace characters are all characters for which
    QChar::isSpace() returns \c true.

    \sa operator>>()
*/
void BOBUIextStream::skipWhiteSpace()
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(Q_VOID);
    d->scan(nullptr, nullptr, 0, BOBUIextStreamPrivate::NotSpace);
    d->consumeLastToken();
}

/*!
    Sets the current device to \a device. If a device has already been
    assigned, BOBUIextStream will call flush() before the old device is
    replaced.

    \note This function resets locale to the default locale ('C')
    and encoding to the default encoding, UTF-8.

    \sa device(), setString()
*/
void BOBUIextStream::setDevice(QIODevice *device)
{
    Q_D(BOBUIextStream);
    flush();
    if (d->deleteDevice) {
        d->disconnectFromDevice();
        delete d->device;
        d->deleteDevice = false;
    }

    d->reset();
    d->status = Ok;
    d->device = device;
    d->resetReadBuffer();
    d->setupDevice(d->device);
}

/*!
    Returns the current device associated with the BOBUIextStream,
    or \nullptr if no device has been assigned.

    \sa setDevice(), string()
*/
QIODevice *BOBUIextStream::device() const
{
    Q_D(const BOBUIextStream);
    return d->device;
}

/*!
    Sets the current string to \a string, using the given \a
    openMode. If a device has already been assigned, BOBUIextStream will
    call flush() before replacing it.

    \sa string(), setDevice()
*/
void BOBUIextStream::setString(QString *string, OpenMode openMode)
{
    Q_D(BOBUIextStream);
    flush();
    if (d->deleteDevice) {
#ifndef BOBUI_NO_QOBJECT
        d->setupDevice(d->device);
        d->device->blockSignals(true);
#endif
        delete d->device;
        d->deleteDevice = false;
    }

    d->reset();
    d->status = Ok;
    d->string = string;
    d->stringOpenMode = openMode;
}

/*!
    Returns the current string assigned to the BOBUIextStream, or
    \nullptr if no string has been assigned.

    \sa setString(), device()
*/
QString *BOBUIextStream::string() const
{
    Q_D(const BOBUIextStream);
    return d->string;
}

/*!
    Sets the field alignment to \a mode. When used together with
    setFieldWidth(), this function allows you to generate formatted
    output with text aligned to the left, to the right or center
    aligned.

    \sa fieldAlignment(), setFieldWidth()
*/
void BOBUIextStream::setFieldAlignment(FieldAlignment mode)
{
    Q_D(BOBUIextStream);
    d->params.fieldAlignment = mode;
}

/*!
    Returns the current field alignment.

    \sa setFieldAlignment(), fieldWidth()
*/
BOBUIextStream::FieldAlignment BOBUIextStream::fieldAlignment() const
{
    Q_D(const BOBUIextStream);
    return d->params.fieldAlignment;
}

/*!
    Sets the pad character to \a ch. The default value is the ASCII
    space character (' '), or QChar(0x20). This character is used to
    fill in the space in fields when generating text.

    Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 5

    The string \c s contains:

    \snippet code/src_corelib_io_bobuiextstream.cpp 6

    \sa padChar(), setFieldWidth()
*/
void BOBUIextStream::setPadChar(QChar ch)
{
    Q_D(BOBUIextStream);
    d->params.padChar = ch;
}

/*!
    Returns the current pad character.

    \sa setPadChar(), setFieldWidth()
*/
QChar BOBUIextStream::padChar() const
{
    Q_D(const BOBUIextStream);
    return d->params.padChar;
}

/*!
    Sets the current field width to \a width. If \a width is 0 (the
    default), the field width is equal to the length of the generated
    text.

    \note The field width applies to every element appended to this
    stream after this function has been called (e.g., it also pads
    endl). This behavior is different from similar classes in the STL,
    where the field width only applies to the next element.

    \sa fieldWidth(), setPadChar()
*/
void BOBUIextStream::setFieldWidth(int width)
{
    Q_D(BOBUIextStream);
    d->params.fieldWidth = width;
}

/*!
    Returns the current field width.

    \sa setFieldWidth()
*/
int BOBUIextStream::fieldWidth() const
{
    Q_D(const BOBUIextStream);
    return d->params.fieldWidth;
}

/*!
    Sets the current number flags to \a flags. \a flags is a set of
    flags from the NumberFlag enum, and describes options for
    formatting generated code (e.g., whether or not to always write
    the base or sign of a number).

    \sa numberFlags(), setIntegerBase(), setRealNumberNotation()
*/
void BOBUIextStream::setNumberFlags(NumberFlags flags)
{
    Q_D(BOBUIextStream);
    d->params.numberFlags = flags;
}

/*!
    Returns the current number flags.

    \sa setNumberFlags(), integerBase(), realNumberNotation()
*/
BOBUIextStream::NumberFlags BOBUIextStream::numberFlags() const
{
    Q_D(const BOBUIextStream);
    return d->params.numberFlags;
}

/*!
    Sets the base of integers to \a base, both for reading and for
    generating numbers. \a base can be either 2 (binary), 8 (octal),
    10 (decimal) or 16 (hexadecimal). If \a base is 0, BOBUIextStream
    will attempt to detect the base by inspecting the data on the
    stream. When generating numbers, BOBUIextStream assumes base is 10
    unless the base has been set explicitly.

    \sa integerBase(), QString::number(), setNumberFlags()
*/
void BOBUIextStream::setIntegerBase(int base)
{
    Q_D(BOBUIextStream);
    d->params.integerBase = base;
}

/*!
    Returns the current base of integers. 0 means that the base is
    detected when reading, or 10 (decimal) when generating numbers.

    \sa setIntegerBase(), QString::number(), numberFlags()
*/
int BOBUIextStream::integerBase() const
{
    Q_D(const BOBUIextStream);
    return d->params.integerBase;
}

/*!
    Sets the real number notation to \a notation (SmartNotation,
    FixedNotation, ScientificNotation). When reading and generating
    numbers, BOBUIextStream uses this value to detect the formatting of
    real numbers.

    \sa realNumberNotation(), setRealNumberPrecision(), setNumberFlags(), setIntegerBase()
*/
void BOBUIextStream::setRealNumberNotation(RealNumberNotation notation)
{
    Q_D(BOBUIextStream);
    d->params.realNumberNotation = notation;
}

/*!
    Returns the current real number notation.

    \sa setRealNumberNotation(), realNumberPrecision(), numberFlags(), integerBase()
*/
BOBUIextStream::RealNumberNotation BOBUIextStream::realNumberNotation() const
{
    Q_D(const BOBUIextStream);
    return d->params.realNumberNotation;
}

/*!
    Sets the precision of real numbers to \a precision. This value
    describes the number of fraction digits BOBUIextStream should
    write when generating real numbers (FixedNotation, ScientificNotation), or
    the maximum number of significant digits (SmartNotation).

    The precision cannot be a negative value. The default value is 6.

    \sa realNumberPrecision(), setRealNumberNotation()
*/
void BOBUIextStream::setRealNumberPrecision(int precision)
{
    Q_D(BOBUIextStream);
    if (precision < 0) {
        qWarning("BOBUIextStream::setRealNumberPrecision: Invalid precision (%d)", precision);
        d->params.realNumberPrecision = 6;
        return;
    }
    d->params.realNumberPrecision = precision;
}

/*!
    Returns the current real number precision, or the number of fraction
    digits BOBUIextStream will write when generating real numbers
    (FixedNotation, ScientificNotation), or the maximum number of significant
    digits (SmartNotation).

    \sa setRealNumberNotation(), realNumberNotation(), numberFlags(), integerBase()
*/
int BOBUIextStream::realNumberPrecision() const
{
    Q_D(const BOBUIextStream);
    return d->params.realNumberPrecision;
}

/*!
    Returns the status of the text stream.

    \sa BOBUIextStream::Status, setStatus(), resetStatus()
*/

BOBUIextStream::Status BOBUIextStream::status() const
{
    Q_D(const BOBUIextStream);
    return d->status;
}

/*!
    \since 4.1

    Resets the status of the text stream.

    \sa BOBUIextStream::Status, status(), setStatus()
*/
void BOBUIextStream::resetStatus()
{
    Q_D(BOBUIextStream);
    d->status = Ok;
}

/*!
    \since 4.1

    Sets the status of the text stream to the \a status given.

    Subsequent calls to setStatus() are ignored until resetStatus()
    is called.

    \sa Status, status(), resetStatus()
*/
void BOBUIextStream::setStatus(Status status)
{
    Q_D(BOBUIextStream);
    if (d->status == Ok)
        d->status = status;
}

/*!
    Returns \c true if there is no more data to be read from the
    BOBUIextStream; otherwise returns \c false. This is similar to, but not
    the same as calling QIODevice::atEnd(), as BOBUIextStream also takes
    into account its internal Unicode buffer.
*/
bool BOBUIextStream::atEnd() const
{
    Q_D(const BOBUIextStream);
    CHECK_VALID_STREAM(true);

    if (d->string)
        return d->string->size() == d->stringOffset;
    return d->readBuffer.isEmpty() && d->device->atEnd();
}

/*!
    Reads the entire content of the stream, and returns it as a
    QString. Avoid this function when working on large files, as it
    will consume a significant amount of memory.

    Calling \l {BOBUIextStream::readLine()}{readLine()} is better if you do not know how much data is
    available.

    \sa readLine()
*/
QString BOBUIextStream::readAll()
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(QString());

    return d->read(std::numeric_limits<qsizetype>::max());
}

/*!
    Reads one line of text from the stream, and returns it as a
    QString. The maximum allowed line length is set to \a maxlen. If
    the stream contains lines longer than this, then the lines will be
    split after \a maxlen characters and returned in parts.

    If \a maxlen is 0, the lines can be of any length.

    The returned line has no trailing end-of-line characters ("\\n"
    or "\\r\\n"), so calling QString::trimmed() can be unnecessary.

    If the stream has read to the end of the file, \l {BOBUIextStream::readLine()}{readLine()}
    will return a null QString. For strings, or for devices that support it,
    you can explicitly test for the end of the stream using atEnd().

    \sa readAll(), QIODevice::readLine()
*/
QString BOBUIextStream::readLine(qint64 maxlen)
{
    QString line;

    readLineInto(&line, maxlen);
    return line;
}

/*!
    \since 5.5

    Reads one line of text from the stream into \a line.
    If \a line is \nullptr, the read line is not stored.

    The maximum allowed line length is set to \a maxlen. If
    the stream contains lines longer than this, then the lines will be
    split after \a maxlen characters and returned in parts.

    If \a maxlen is 0, the lines can be of any length.

    The resulting line has no trailing end-of-line characters ("\\n"
    or "\\r\\n"), so calling QString::trimmed() can be unnecessary.

    If \a line has sufficient capacity for the data that is about to be
    read, this function may not need to allocate new memory. Because of
    this, it can be faster than readLine().

    Returns \c false if the stream has read to the end of the file or
    an error has occurred; otherwise returns \c true. The contents in
    \a line before the call are discarded in any case.

    \sa readAll(), QIODevice::readLine(), QIODevice::readLineInto()
*/
bool BOBUIextStream::readLineInto(QString *line, qint64 maxlen)
{
    Q_D(BOBUIextStream);
    // keep in sync with CHECK_VALID_STREAM
    if (!d->string && !d->device) {
        qWarning("BOBUIextStream: No device");
        if (line && !line->isNull())
            line->resize(0);
        return false;
    }

    const QChar *readPtr;
    qsizetype length;
    if (!d->scan(&readPtr, &length, qsizetype(maxlen), BOBUIextStreamPrivate::EndOfLine)) {
        if (line && !line->isNull())
            line->resize(0);
        return false;
    }

    if (Q_LIKELY(line))
        line->setUnicode(readPtr, length);
    d->consumeLastToken();
    return true;
}

/*!
    \since 4.1

    Reads at most \a maxlen characters from the stream, and returns the data
    read as a QString.

    \sa readAll(), readLine(), QIODevice::read()
*/
QString BOBUIextStream::read(qint64 maxlen)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(QString());

    if (maxlen <= 0)
        return QString::fromLatin1("");     // empty, not null

    return d->read(q26::saturate_cast<qsizetype>(maxlen));
}

/*!
    \internal
*/
BOBUIextStreamPrivate::NumberParsingStatus BOBUIextStreamPrivate::getNumber(qulonglong *ret)
{
    scan(nullptr, nullptr, 0, NotSpace);
    consumeLastToken();

    // detect integer encoding
    int base = params.integerBase;
    if (base == 0) {
        QChar ch;
        if (!getChar(&ch))
            return npsInvalidPrefix;
        if (ch == u'0') {
            QChar ch2;
            if (!getChar(&ch2)) {
                // Result is the number 0
                *ret = 0;
                return npsOk;
            }
            ch2 = ch2.toLower();

            if (ch2 == u'x') {
                base = 16;
            } else if (ch2 == u'b') {
                base = 2;
            } else if (ch2.isDigit() && ch2.digitValue() >= 0 && ch2.digitValue() <= 7) {
                base = 8;
            } else {
                base = 10;
            }
            ungetChar(ch2);
        } else if (ch == locale.negativeSign() || ch == locale.positiveSign() || ch.isDigit()) {
            base = 10;
        } else {
            ungetChar(ch);
            return npsInvalidPrefix;
        }
        ungetChar(ch);
        // State of the stream is now the same as on entry
        // (cursor is at prefix),
        // and local variable 'base' has been set appropriately.
    }

    qulonglong val=0;
    switch (base) {
    case 2: {
        QChar pf1, pf2, dig;
        // Parse prefix '0b'
        if (!getChar(&pf1) || pf1 != u'0')
            return npsInvalidPrefix;
        if (!getChar(&pf2) || pf2.toLower() != u'b')
            return npsInvalidPrefix;
        // Parse digits
        qsizetype ndigits = 0;
        while (getChar(&dig)) {
            char16_t n = dig.toLower().unicode();
            if (n == u'0' || n == u'1') {
                val <<= 1;
                val += n - u'0';
            } else {
                ungetChar(dig);
                break;
            }
            ndigits++;
        }
        if (ndigits == 0) {
            // Unwind the prefix and abort
            ungetChar(pf2);
            ungetChar(pf1);
            return npsMissingDigit;
        }
        break;
    }
    case 8: {
        QChar pf, dig;
        // Parse prefix u'0'
        if (!getChar(&pf) || pf != u'0')
            return npsInvalidPrefix;
        // Parse digits
        qsizetype ndigits = 0;
        while (getChar(&dig)) {
            char16_t n = dig.toLower().unicode();
            if (isOctalDigit(n)) {
                val *= 8;
                val += n - u'0';
            } else {
                ungetChar(dig);
                break;
            }
            ndigits++;
        }
        if (ndigits == 0) {
            // Unwind the prefix and abort
            ungetChar(pf);
            return npsMissingDigit;
        }
        break;
    }
    case 10: {
        // Parse sign (or first digit)
        QChar sign;
        qsizetype ndigits = 0;
        if (!getChar(&sign))
            return npsMissingDigit;
        if (sign != locale.negativeSign() && sign != locale.positiveSign()) {
            if (!sign.isDigit()) {
                ungetChar(sign);
                return npsMissingDigit;
            }
            val += sign.digitValue();
            ndigits++;
        }
        // Parse digits
        QChar ch;
        while (getChar(&ch)) {
            if (ch.isDigit()) {
                val *= 10;
                val += ch.digitValue();
            } else if (locale != QLocale::c() && ch == locale.groupSeparator()) {
                continue;
            } else {
                ungetChar(ch);
                break;
            }
            ndigits++;
        }
        if (ndigits == 0)
            return npsMissingDigit;
        if (sign == locale.negativeSign()) {
            qlonglong ival = qlonglong(val);
            if (ival > 0)
                ival = -ival;
            val = qulonglong(ival);
        }
        break;
    }
    case 16: {
        QChar pf1, pf2, dig;
        // Parse prefix ' 0x'
        if (!getChar(&pf1) || pf1 != u'0')
            return npsInvalidPrefix;
        if (!getChar(&pf2) || pf2.toLower() != u'x')
            return npsInvalidPrefix;
        // Parse digits
        qsizetype ndigits = 0;
        while (getChar(&dig)) {
            const int h = fromHex(dig.unicode());
            if (h != -1) {
                val <<= 4;
                val += h;
            } else {
                ungetChar(dig);
                break;
            }
            ndigits++;
        }
        if (ndigits == 0) {
            return npsMissingDigit;
        }
        break;
    }
    default:
        // Unsupported integerBase
        return npsInvalidPrefix;
    }

    if (ret)
        *ret = val;
    return npsOk;
}

/*!
    \internal
    (hihi)
*/
bool BOBUIextStreamPrivate::getReal(double *f)
{
    // We use a table-driven FSM to parse floating point numbers
    // strtod() cannot be used directly since we may be reading from a
    // QIODevice.
    enum ParserState {
        Init = 0,
        Sign = 1,
        Mantissa = 2,
        Dot = 3,
        Abscissa = 4,
        ExpMark = 5,
        ExpSign = 6,
        Exponent = 7,
        Nan1 = 8,
        Nan2 = 9,
        Inf1 = 10,
        Inf2 = 11,
        NanInf = 12,
        Done = 13
    };
    enum InputToken {
        None = 0,
        InputSign = 1,
        InputDigit = 2,
        InputDot = 3,
        InputExp = 4,
        InputI = 5,
        InputN = 6,
        InputF = 7,
        InputA = 8,
        InputT = 9
    };

    static const uchar table[13][10] = {
        // None InputSign InputDigit InputDot InputExp InputI    InputN    InputF    InputA    InputT
        { 0,    Sign,     Mantissa,  Dot,     0,       Inf1,     Nan1,     0,        0,        0      }, // 0  Init
        { 0,    0,        Mantissa,  Dot,     0,       Inf1,     Nan1,     0,        0,        0      }, // 1  Sign
        { Done, Done,     Mantissa,  Dot,     ExpMark, 0,        0,        0,        0,        0      }, // 2  Mantissa
        { 0,    0,        Abscissa,  0,       0,       0,        0,        0,        0,        0      }, // 3  Dot
        { Done, Done,     Abscissa,  Done,    ExpMark, 0,        0,        0,        0,        0      }, // 4  Abscissa
        { 0,    ExpSign,  Exponent,  0,       0,       0,        0,        0,        0,        0      }, // 5  ExpMark
        { 0,    0,        Exponent,  0,       0,       0,        0,        0,        0,        0      }, // 6  ExpSign
        { Done, Done,     Exponent,  Done,    Done,    0,        0,        0,        0,        0      }, // 7  Exponent
        { 0,    0,        0,         0,       0,       0,        0,        0,        Nan2,     0      }, // 8  Nan1
        { 0,    0,        0,         0,       0,       0,        NanInf,   0,        0,        0      }, // 9  Nan2
        { 0,    0,        0,         0,       0,       0,        Inf2,     0,        0,        0      }, // 10 Inf1
        { 0,    0,        0,         0,       0,       0,        0,        NanInf,   0,        0      }, // 11 Inf2
        { Done, 0,        0,         0,       0,       0,        0,        0,        0,        0      }, // 11 NanInf
    };

    ParserState state = Init;
    InputToken input = None;

    scan(nullptr, nullptr, 0, NotSpace);
    consumeLastToken();

    const qsizetype BufferSize = 128;
    char buf[BufferSize];
    qsizetype i = 0;

    QChar c;
    while (getChar(&c)) {
        switch (c.unicode()) {
        case u'0': case u'1': case u'2': case u'3': case u'4':
        case u'5': case u'6': case u'7': case u'8': case u'9':
            input = InputDigit;
            break;
        case u'i': case u'I':
            input = InputI;
            break;
        case u'n': case u'N':
            input = InputN;
            break;
        case u'f': case u'F':
            input = InputF;
            break;
        case u'a': case u'A':
            input = InputA;
            break;
        case u't': case u'T':
            input = InputT;
            break;
        default: {
            QChar lc = c.toLower();
            if (lc == locale.decimalPoint().toLower())
                input = InputDot;
            else if (lc == locale.exponential().toLower())
                input = InputExp;
            else if (lc == locale.negativeSign().toLower()
                     || lc == locale.positiveSign().toLower())
                input = InputSign;
            else if (locale != QLocale::c() // backward-compatibility
                     && lc == locale.groupSeparator().toLower())
                input = InputDigit; // well, it isn't a digit, but no one cares.
            else
                input = None;
        }
            break;
        }

        state = ParserState(table[state][input]);

        if  (state == Init || state == Done || i > (BufferSize - 5)) {
            ungetChar(c);
            if (i > (BufferSize - 5)) { // ignore rest of digits
                while (getChar(&c)) {
                    if (!c.isDigit()) {
                        ungetChar(c);
                        break;
                    }
                }
            }
            break;
        }

        buf[i++] = c.toLatin1();
    }

    if (i == 0)
        return false;
    if (!f)
        return true;
    buf[i] = '\0';

    // backward-compatibility. Old implementation supported +nan/-nan
    // for some reason. QLocale only checks for lower-case
    // nan/+inf/-inf, so here we also check for uppercase and mixed
    // case versions.
    if (!qstricmp(buf, "nan") || !qstricmp(buf, "+nan") || !qstricmp(buf, "-nan")) {
        *f = bobui_qnan();
        return true;
    } else if (!qstricmp(buf, "+inf") || !qstricmp(buf, "inf")) {
        *f = bobui_inf();
        return true;
    } else if (!qstricmp(buf, "-inf")) {
        *f = -bobui_inf();
        return true;
    }
    bool ok;
    *f = locale.toDouble(QString::fromLatin1(buf), &ok);
    return ok;
}

/*!
    Reads a character from the stream and stores it in \a c. Returns a
    reference to the BOBUIextStream, so several operators can be
    nested. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 7

    Whitespace is \e not skipped.
*/

BOBUIextStream &BOBUIextStream::operator>>(QChar &c)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->scan(nullptr, nullptr, 0, BOBUIextStreamPrivate::NotSpace);
    if (!d->getChar(&c))
        setStatus(ReadPastEnd);
    return *this;
}

/*!
    \overload

    Reads a character from the stream and stores it in \a c. The
    character from the stream is converted to ISO-8859-1 before it is
    stored.

    \sa QChar::toLatin1()
*/
BOBUIextStream &BOBUIextStream::operator>>(char &c)
{
    QChar ch;
    *this >> ch;
    c = ch.toLatin1();
    return *this;
}

/*!
    \fn BOBUIextStream &BOBUIextStream::operator>>(char16_t &c)
    \overload
    \since 6.4

    Reads a character from the stream and stores it in \a c.
*/

/*!
    Reads an integer from the stream and stores it in \a i, then
    returns a reference to the BOBUIextStream. The number is cast to
    the correct type before it is stored. If no number was detected on
    the stream, \a i is set to 0.

    By default, BOBUIextStream will attempt to detect the base of the
    number using the following rules:

    \table
    \header \li Prefix                \li Base
    \row    \li "0b" or "0B"          \li 2 (binary)
    \row    \li "0" followed by "0-7" \li 8 (octal)
    \row    \li "0" otherwise         \li 10 (decimal)
    \row    \li "0x" or "0X"          \li 16 (hexadecimal)
    \row    \li "1" to "9"            \li 10 (decimal)
    \endtable

    By calling setIntegerBase(), you can specify the integer base
    explicitly. This will disable the auto-detection, and speed up
    BOBUIextStream slightly.

    Leading whitespace is skipped.
*/
BOBUIextStream &BOBUIextStream::operator>>(signed short &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(signed short);
}

/*!
    \overload

    Stores the integer in the unsigned short \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(unsigned short &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(unsigned short);
}

/*!
    \overload

    Stores the integer in the signed int \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(signed int &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(signed int);
}

/*!
    \overload

    Stores the integer in the unsigned int \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(unsigned int &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(unsigned int);
}

/*!
    \overload

    Stores the integer in the signed long \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(signed long &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(signed long);
}

/*!
    \overload

    Stores the integer in the unsigned long \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(unsigned long &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(unsigned long);
}

/*!
    \overload

    Stores the integer in the qlonglong \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(qlonglong &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(qlonglong);
}

/*!
    \overload

    Stores the integer in the qulonglong \a i.
*/
BOBUIextStream &BOBUIextStream::operator>>(qulonglong &i)
{
    IMPLEMENT_STREAM_RIGHT_INT_OPERATOR(qulonglong);
}

/*!
    Reads a real number from the stream and stores it in \a f, then
    returns a reference to the BOBUIextStream. The number is cast to
    the correct type. If no real number is detect on the stream, \a f
    is set to 0.0.

    As a special exception, BOBUIextStream allows the strings "nan" and "inf" to
    represent NAN and INF floats or doubles.

    Leading whitespace is skipped.
*/
BOBUIextStream &BOBUIextStream::operator>>(float &f)
{
    IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR(float);
}

/*!
    \overload

    Stores the real number in the double \a f.
*/
BOBUIextStream &BOBUIextStream::operator>>(double &f)
{
    IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR(double);
}

/*!
    Reads a word from the stream and stores it in \a str, then returns
    a reference to the stream. Words are separated by whitespace
    (i.e., all characters for which QChar::isSpace() returns \c true).

    Leading whitespace is skipped.
*/
BOBUIextStream &BOBUIextStream::operator>>(QString &str)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);

    str.clear();
    d->scan(nullptr, nullptr, 0, BOBUIextStreamPrivate::NotSpace);
    d->consumeLastToken();

    const QChar *ptr;
    qsizetype length;
    if (!d->scan(&ptr, &length, 0, BOBUIextStreamPrivate::Space)) {
        setStatus(ReadPastEnd);
        return *this;
    }

    str = QString(ptr, length);
    d->consumeLastToken();
    return *this;
}

/*!
    \overload

    Converts the word to UTF-8, then stores it in \a array.

    \sa QString::toLatin1()
*/
BOBUIextStream &BOBUIextStream::operator>>(QByteArray &array)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);

    d->scan(nullptr, nullptr, 0, BOBUIextStreamPrivate::NotSpace);
    d->consumeLastToken();

    const QChar *ptr;
    qsizetype length;
    if (!d->scan(&ptr, &length, 0, BOBUIextStreamPrivate::Space)) {
        setStatus(ReadPastEnd);
        array.clear();
        return *this;
    }

    array = QStringView(ptr, length).toUtf8();

    d->consumeLastToken();
    return *this;
}

/*!
    \overload

    Converts the word to UTF-8 and stores it in \a c, terminated by a '\\0'
    character. If no word is available, only the '\\0' character is stored.

    Warning: Although convenient, this operator is dangerous and must
    be used with care. BOBUIextStream assumes that \a c points to a
    buffer with enough space to hold the word. If the buffer is too
    small, your application may crash. For a word consisting of \c{n} QChars,
    the buffer needs to be at least \c{3*n+1} characters long.

    If possible, use the QByteArray operator instead.
*/
BOBUIextStream &BOBUIextStream::operator>>(char *c)
{
    Q_D(BOBUIextStream);
    *c = 0;
    CHECK_VALID_STREAM(*this);
    d->scan(nullptr, nullptr, 0, BOBUIextStreamPrivate::NotSpace);
    d->consumeLastToken();

    const QChar *ptr;
    qsizetype length;
    if (!d->scan(&ptr, &length, 0, BOBUIextStreamPrivate::Space)) {
        setStatus(ReadPastEnd);
        return *this;
    }

    QStringEncoder encoder(QStringConverter::Utf8);
    char *e = encoder.appendToBuffer(c, QStringView(ptr, length));
    *e = '\0';
    d->consumeLastToken();
    return *this;
}

/*!
    \internal
 */
void BOBUIextStreamPrivate::putNumber(qulonglong number, bool negative)
{
    unsigned flags = 0;
    const BOBUIextStream::NumberFlags numberFlags = params.numberFlags;
    if (numberFlags & BOBUIextStream::ShowBase)
        flags |= QLocaleData::ShowBase;
    // ForceSign is irrelevant when we'll be including a sign in any case:
    if ((numberFlags & BOBUIextStream::ForceSign) && !negative)
        flags |= QLocaleData::AlwaysShowSign;
    if (numberFlags & BOBUIextStream::UppercaseBase)
        flags |= QLocaleData::UppercaseBase;
    if (numberFlags & BOBUIextStream::UppercaseDigits)
        flags |= QLocaleData::CapitalEorX;

    // Group digits. For backward compatibility, we skip this for the C locale.
    if (locale != QLocale::c() && !locale.numberOptions().testFlag(QLocale::OmitGroupSeparator))
        flags |= QLocaleData::GroupDigits;

    const QLocaleData *dd = locale.d->m_data;
    int base = params.integerBase ? params.integerBase : 10;
    QString result = dd->unsLongLongToString(number, -1, base, -1, flags);
    if (negative) {
        result.prepend(locale.negativeSign());
    } else if (number == 0 && base == 8 && params.numberFlags & BOBUIextStream::ShowBase
               && result == "0"_L1) {
        // Workaround for backward compatibility - in octal form with ShowBase
        // flag set, zero should get its 0 prefix before its 0 value, but
        // QLocalePrivate only adds the prefix if the number doesn't start with
        // a zero.
        result.prepend(u'0');
    }
    putString(result, PutStringMode::Number);
}

/*!
    Writes the character \a c to the stream, then returns a reference
    to the BOBUIextStream.

    \sa setFieldWidth()
*/
BOBUIextStream &BOBUIextStream::operator<<(QChar c)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putChar(c);
    return *this;
}

/*!
    \overload

    Converts \a c from ASCII to a QChar, then writes it to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(char c)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putChar(QChar::fromLatin1(c));
    return *this;
}

/*!
    \fn BOBUIextStream &BOBUIextStream::operator<<(char16_t c)
    \overload
    \since 6.3.1

    Writes the Unicode character \a c to the stream, then returns a
    reference to the BOBUIextStream.
*/

/*!
    Writes the integer number \a i to the stream, then returns a
    reference to the BOBUIextStream. By default, the number is stored in
    decimal form, but you can also set the base by calling
    setIntegerBase().

    \sa setFieldWidth(), setNumberFlags()
*/
BOBUIextStream &BOBUIextStream::operator<<(signed short i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber(BobUIPrivate::qUnsignedAbs(i), i < 0);
    return *this;
}

/*!
    \overload

    Writes the unsigned short \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(unsigned short i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber((qulonglong)i, false);
    return *this;
}

/*!
    \overload

    Writes the signed int \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(signed int i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber(BobUIPrivate::qUnsignedAbs(i), i < 0);
    return *this;
}

/*!
    \overload

    Writes the unsigned int \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(unsigned int i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber((qulonglong)i, false);
    return *this;
}

/*!
    \overload

    Writes the signed long \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(signed long i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber(BobUIPrivate::qUnsignedAbs(i), i < 0);
    return *this;
}

/*!
    \overload

    Writes the unsigned long \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(unsigned long i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber((qulonglong)i, false);
    return *this;
}

/*!
    \overload

    Writes the qlonglong \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(qlonglong i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber(BobUIPrivate::qUnsignedAbs(i), i < 0);
    return *this;
}

/*!
    \overload

    Writes the qulonglong \a i to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(qulonglong i)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putNumber(i, false);
    return *this;
}

/*!
    Writes the real number \a f to the stream, then returns a
    reference to the BOBUIextStream. By default, BOBUIextStream stores it
    using SmartNotation, with up to 6 digits of precision. You can
    change the textual representation BOBUIextStream will use for real
    numbers by calling setRealNumberNotation(),
    setRealNumberPrecision() and setNumberFlags().

    \sa setFieldWidth(), setRealNumberNotation(),
    setRealNumberPrecision(), setNumberFlags()
*/
BOBUIextStream &BOBUIextStream::operator<<(float f)
{
    return *this << double(f);
}

/*!
    \overload

    Writes the double \a f to the stream.
*/
BOBUIextStream &BOBUIextStream::operator<<(double f)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);

    QLocaleData::DoubleForm form = QLocaleData::DFDecimal;
    switch (realNumberNotation()) {
    case FixedNotation:
        form = QLocaleData::DFDecimal;
        break;
    case ScientificNotation:
        form = QLocaleData::DFExponent;
        break;
    case SmartNotation:
        form = QLocaleData::DFSignificantDigits;
        break;
    }

    uint flags = 0;
    const QLocale::NumberOptions numberOptions = locale().numberOptions();
    if (numberFlags() & ShowBase)
        flags |= QLocaleData::ShowBase;
    if (numberFlags() & ForceSign)
        flags |= QLocaleData::AlwaysShowSign;
    if (numberFlags() & UppercaseBase)
        flags |= QLocaleData::UppercaseBase;
    if (numberFlags() & UppercaseDigits)
        flags |= QLocaleData::CapitalEorX;
    if (numberFlags() & ForcePoint) {
        flags |= QLocaleData::ForcePoint;

        // Only for backwards compatibility
        flags |= QLocaleData::AddTrailingZeroes | QLocaleData::ShowBase;
    }
    if (locale() != QLocale::c() && !(numberOptions & QLocale::OmitGroupSeparator))
        flags |= QLocaleData::GroupDigits;
    if (!(numberOptions & QLocale::OmitLeadingZeroInExponent))
        flags |= QLocaleData::ZeroPadExponent;
    if (numberOptions & QLocale::IncludeTrailingZeroesAfterDot)
        flags |= QLocaleData::AddTrailingZeroes;

    const QLocaleData *dd = d->locale.d->m_data;
    QString num = dd->doubleToString(f, d->params.realNumberPrecision, form, -1, flags);
    d->putString(num, BOBUIextStreamPrivate::PutStringMode::Number);
    return *this;
}

/*!
    Writes the string \a string to the stream, and returns a reference
    to the BOBUIextStream. The string is first encoded using the assigned
    encoding (the default is UTF-8) before it is written to the stream.

    \sa setFieldWidth(), setEncoding()
*/
BOBUIextStream &BOBUIextStream::operator<<(const QString &string)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putString(string);
    return *this;
}

/*!
    \overload

    Writes \a string to the stream, and returns a reference to the
    BOBUIextStream.
    \since 5.12
*/
BOBUIextStream &BOBUIextStream::operator<<(QStringView string)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putString(string);
    return *this;
}

/*!
    \overload

    Writes \a string to the stream, and returns a reference to the
    BOBUIextStream.
*/
BOBUIextStream &BOBUIextStream::operator<<(QLatin1StringView string)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putString(string);
    return *this;
}

/*!
    \overload

    Writes \a array to the stream. The contents of \a array are
    converted with QString::fromUtf8().
*/
BOBUIextStream &BOBUIextStream::operator<<(const QByteArray &array)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putString(QUtf8StringView{array});
    return *this;
}

/*!
    \overload

    Writes the constant string pointed to by \a string to the stream. \a
    string is assumed to be in UTF-8 encoding. This operator
    is convenient when working with constant string data. Example:

    \snippet code/src_corelib_io_bobuiextstream.cpp 8

    Warning: BOBUIextStream assumes that \a string points to a string of
    text, terminated by a '\\0' character. If there is no terminating
    '\\0' character, your application may crash.
*/
BOBUIextStream &BOBUIextStream::operator<<(const char *string)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    d->putString(QUtf8StringView(string));
    return *this;
}

/*!
    \overload

    Writes \a ptr to the stream as a hexadecimal number with a base.
*/

BOBUIextStream &BOBUIextStream::operator<<(const void *ptr)
{
    Q_D(BOBUIextStream);
    CHECK_VALID_STREAM(*this);
    const int oldBase = d->params.integerBase;
    const NumberFlags oldFlags = d->params.numberFlags;
    d->params.integerBase = 16;
    d->params.numberFlags |= ShowBase;
    d->putNumber(reinterpret_cast<quintptr>(ptr), false);
    d->params.integerBase = oldBase;
    d->params.numberFlags = oldFlags;
    return *this;
}

/*!
    \fn BOBUIextStream::operator bool() const
    \since 6.10

    Returns whether this stream has no errors (status() returns \l{Ok}).
*/

namespace BobUI {

/*!
    Calls BOBUIextStream::setIntegerBase(2) on \a stream and returns \a
    stream.

    \since 5.14

    \sa oct(), dec(), hex(), {BOBUIextStream manipulators}
*/
BOBUIextStream &bin(BOBUIextStream &stream)
{
    stream.setIntegerBase(2);
    return stream;
}

/*!
    Calls BOBUIextStream::setIntegerBase(8) on \a stream and returns \a
    stream.

    \since 5.14

    \sa bin(), dec(), hex(), {BOBUIextStream manipulators}
*/
BOBUIextStream &oct(BOBUIextStream &stream)
{
    stream.setIntegerBase(8);
    return stream;
}

/*!
    Calls BOBUIextStream::setIntegerBase(10) on \a stream and returns \a
    stream.

    \since 5.14

    \sa bin(), oct(), hex(), {BOBUIextStream manipulators}
*/
BOBUIextStream &dec(BOBUIextStream &stream)
{
    stream.setIntegerBase(10);
    return stream;
}

/*!
    Calls BOBUIextStream::setIntegerBase(16) on \a stream and returns \a
    stream.

    \since 5.14

    \note The hex modifier can only be used for writing to streams.
    \sa bin(), oct(), dec(), {BOBUIextStream manipulators}
*/
BOBUIextStream &hex(BOBUIextStream &stream)
{
    stream.setIntegerBase(16);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() |
    BOBUIextStream::ShowBase) on \a stream and returns \a stream.

    \since 5.14

    \sa noshowbase(), forcesign(), forcepoint(), {BOBUIextStream manipulators}
*/
BOBUIextStream &showbase(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() | BOBUIextStream::ShowBase);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() |
    BOBUIextStream::ForceSign) on \a stream and returns \a stream.

    \since 5.14

    \sa noforcesign(), forcepoint(), showbase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &forcesign(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() | BOBUIextStream::ForceSign);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() |
    BOBUIextStream::ForcePoint) on \a stream and returns \a stream.

    \since 5.14

    \sa noforcepoint(), forcesign(), showbase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &forcepoint(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() | BOBUIextStream::ForcePoint);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() &
    ~BOBUIextStream::ShowBase) on \a stream and returns \a stream.

    \since 5.14

    \sa showbase(), noforcesign(), noforcepoint(), {BOBUIextStream manipulators}
*/
BOBUIextStream &noshowbase(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() &= ~BOBUIextStream::ShowBase);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() &
    ~BOBUIextStream::ForceSign) on \a stream and returns \a stream.

    \since 5.14

    \sa forcesign(), noforcepoint(), noshowbase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &noforcesign(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() &= ~BOBUIextStream::ForceSign);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() &
    ~BOBUIextStream::ForcePoint) on \a stream and returns \a stream.

    \since 5.14

    \sa forcepoint(), noforcesign(), noshowbase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &noforcepoint(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() &= ~BOBUIextStream::ForcePoint);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() |
    BOBUIextStream::UppercaseBase) on \a stream and returns \a stream.

    \since 5.14

    \sa lowercasebase(), uppercasedigits(), {BOBUIextStream manipulators}
*/
BOBUIextStream &uppercasebase(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() | BOBUIextStream::UppercaseBase);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() |
    BOBUIextStream::UppercaseDigits) on \a stream and returns \a stream.

    \since 5.14

    \sa lowercasedigits(), uppercasebase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &uppercasedigits(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() | BOBUIextStream::UppercaseDigits);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() &
    ~BOBUIextStream::UppercaseBase) on \a stream and returns \a stream.

    \since 5.14

    \sa uppercasebase(), lowercasedigits(), {BOBUIextStream manipulators}
*/
BOBUIextStream &lowercasebase(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() & ~BOBUIextStream::UppercaseBase);
    return stream;
}

/*!
    Calls BOBUIextStream::setNumberFlags(BOBUIextStream::numberFlags() &
    ~BOBUIextStream::UppercaseDigits) on \a stream and returns \a stream.

    \since 5.14

    \sa uppercasedigits(), lowercasebase(), {BOBUIextStream manipulators}
*/
BOBUIextStream &lowercasedigits(BOBUIextStream &stream)
{
    stream.setNumberFlags(stream.numberFlags() & ~BOBUIextStream::UppercaseDigits);
    return stream;
}

/*!
    Calls BOBUIextStream::setRealNumberNotation(BOBUIextStream::FixedNotation)
    on \a stream and returns \a stream.

    \since 5.14

    \sa scientific(), {BOBUIextStream manipulators}
*/
BOBUIextStream &fixed(BOBUIextStream &stream)
{
    stream.setRealNumberNotation(BOBUIextStream::FixedNotation);
    return stream;
}

/*!
    Calls BOBUIextStream::setRealNumberNotation(BOBUIextStream::ScientificNotation)
    on \a stream and returns \a stream.

    \since 5.14

    \sa fixed(), {BOBUIextStream manipulators}
*/
BOBUIextStream &scientific(BOBUIextStream &stream)
{
    stream.setRealNumberNotation(BOBUIextStream::ScientificNotation);
    return stream;
}

/*!
    Calls BOBUIextStream::setFieldAlignment(BOBUIextStream::AlignLeft)
    on \a stream and returns \a stream.

    \since 5.14

    \sa right(), center(), {BOBUIextStream manipulators}
*/
BOBUIextStream &left(BOBUIextStream &stream)
{
    stream.setFieldAlignment(BOBUIextStream::AlignLeft);
    return stream;
}

/*!
    Calls BOBUIextStream::setFieldAlignment(BOBUIextStream::AlignRight)
    on \a stream and returns \a stream.

    \since 5.14

    \sa left(), center(), {BOBUIextStream manipulators}
*/
BOBUIextStream &right(BOBUIextStream &stream)
{
    stream.setFieldAlignment(BOBUIextStream::AlignRight);
    return stream;
}

/*!
    Calls BOBUIextStream::setFieldAlignment(BOBUIextStream::AlignCenter)
    on \a stream and returns \a stream.

    \since 5.14

    \sa left(), right(), {BOBUIextStream manipulators}
*/
BOBUIextStream &center(BOBUIextStream &stream)
{
    stream.setFieldAlignment(BOBUIextStream::AlignCenter);
    return stream;
}

/*!
    Writes '\\n' to the \a stream and flushes the stream.

    Equivalent to

    \snippet code/src_corelib_io_bobuiextstream.cpp 9

    Note: On Windows, all '\\n' characters are written as '\\r\\n' if
    BOBUIextStream's device or string is opened using the \l QIODeviceBase::Text flag.

    \since 5.14

    \sa flush(), reset(), {BOBUIextStream manipulators}
*/
BOBUIextStream &endl(BOBUIextStream &stream)
{
    return stream << '\n'_L1 << BobUI::flush;
}

/*!
    Calls BOBUIextStream::flush() on \a stream and returns \a stream.

    \since 5.14

    \sa endl(), reset(), {BOBUIextStream manipulators}
*/
BOBUIextStream &flush(BOBUIextStream &stream)
{
    stream.flush();
    return stream;
}

/*!
    Calls BOBUIextStream::reset() on \a stream and returns \a stream.

    \since 5.14

    \sa flush(), {BOBUIextStream manipulators}
*/
BOBUIextStream &reset(BOBUIextStream &stream)
{
    stream.reset();
    return stream;
}

/*!
    Calls \l {BOBUIextStream::}{skipWhiteSpace()} on \a stream and returns \a stream.

    \since 5.14

    \sa {BOBUIextStream manipulators}
*/
BOBUIextStream &ws(BOBUIextStream &stream)
{
    stream.skipWhiteSpace();
    return stream;
}

} // namespace BobUI

/*!
    \fn BOBUIextStreamManipulator qSetFieldWidth(int width)
    \relates BOBUIextStream

    Equivalent to BOBUIextStream::setFieldWidth(\a width).
*/

/*!
    \fn BOBUIextStreamManipulator qSetPadChar(QChar ch)
    \relates BOBUIextStream

    Equivalent to BOBUIextStream::setPadChar(\a ch).
*/

/*!
    \fn BOBUIextStreamManipulator qSetRealNumberPrecision(int precision)
    \relates BOBUIextStream

    Equivalent to BOBUIextStream::setRealNumberPrecision(\a precision).
*/


namespace BobUI {
/*!
    Toggles insertion of the Byte Order Mark on \a stream when BOBUIextStream is
    used with a UTF encoding.

    \since 5.14

    \sa BOBUIextStream::setGenerateByteOrderMark(), {BOBUIextStream manipulators}
*/
BOBUIextStream &bom(BOBUIextStream &stream)
{
    stream.setGenerateByteOrderMark(true);
    return stream;
}

} // namespace BobUI


/*!
    \since 6.0
    Sets the encoding for this stream to \a encoding. The encoding is used for
    decoding any data that is read from the assigned device, and for
    encoding any data that is written. By default,
    QStringConverter::Utf8 is used, and automatic unicode
    detection is enabled.

    If BOBUIextStream operates on a string, this function does nothing.

    \warning If you call this function while the text stream is reading
    from an open sequential socket, the internal buffer may still contain
    text decoded using the old encoding.

    \sa encoding(), setAutoDetectUnicode(), setLocale()
*/
void BOBUIextStream::setEncoding(QStringConverter::Encoding encoding)
{
    Q_D(BOBUIextStream);
    if (d->encoding == encoding)
        return;

    qint64 seekPos = -1;
    if (!d->readBuffer.isEmpty()) {
        if (!d->device->isSequential()) {
            seekPos = pos();
        }
    }

    d->encoding = encoding;
    d->toUtf16 = QStringDecoder(d->encoding);
    bool generateBOM = !d->hasWrittenData && d->generateBOM;
    d->fromUtf16 = QStringEncoder(d->encoding,
                                  generateBOM ? QStringEncoder::Flag::WriteBom : QStringEncoder::Flag::Default);

    if (seekPos >=0 && !d->readBuffer.isEmpty())
        seek(seekPos);
}

/*!
    Returns the encoding that is current assigned to the stream.

    \sa setEncoding(), setAutoDetectUnicode(), locale()
*/
QStringConverter::Encoding BOBUIextStream::encoding() const
{
    Q_D(const BOBUIextStream);
    return d->encoding;
}

/*!
    If \a enabled is true, BOBUIextStream will attempt to detect Unicode encoding
    by peeking into the stream data to see if it can find the UTF-8, UTF-16, or
    UTF-32 Byte Order Mark (BOM). If this mark is found, BOBUIextStream will
    replace the current encoding with the UTF encoding.

    This function can be used together with setEncoding(). It is common
    to set the encoding to UTF-8, and then enable UTF-16 detection.

    \sa autoDetectUnicode(), setEncoding()
*/
void BOBUIextStream::setAutoDetectUnicode(bool enabled)
{
    Q_D(BOBUIextStream);
    d->autoDetectUnicode = enabled;
}

/*!
    Returns \c true if automatic Unicode detection is enabled, otherwise
    returns \c false. Automatic Unicode detection is enabled by default.

    \sa setAutoDetectUnicode(), setEncoding()
*/
bool BOBUIextStream::autoDetectUnicode() const
{
    Q_D(const BOBUIextStream);
    return d->autoDetectUnicode;
}

/*!
    If \a generate is true and a UTF encoding is used, BOBUIextStream will insert
    the BOM (Byte Order Mark) before any data has been written to the
    device. If \a generate is false, no BOM will be inserted. This function
    must be called before any data is written. Otherwise, it does nothing.

    \sa generateByteOrderMark(), {BobUI::}{bom()}
*/
void BOBUIextStream::setGenerateByteOrderMark(bool generate)
{
    Q_D(BOBUIextStream);
    if (d->hasWrittenData || d->generateBOM == generate)
        return;

    d->generateBOM = generate;
    d->fromUtf16 = QStringEncoder(d->encoding, generate ? QStringConverter::Flag::WriteBom : QStringConverter::Flag::Default);
}

/*!
    Returns \c true if BOBUIextStream is set to generate the UTF BOM (Byte Order
    Mark) when using a UTF encoding; otherwise returns \c false. UTF BOM generation is
    set to false by default.

    \sa setGenerateByteOrderMark()
*/
bool BOBUIextStream::generateByteOrderMark() const
{
    Q_D(const BOBUIextStream);
    return d->generateBOM;
}

/*!
    \since 4.5

    Sets the locale for this stream to \a locale. The specified locale is
    used for conversions between numbers and their string representations.

    The default locale is C and it is a special case - the thousands
    group separator is not used for backward compatibility reasons.

    \sa locale()
*/
void BOBUIextStream::setLocale(const QLocale &locale)
{
    Q_D(BOBUIextStream);
    d->locale = locale;
}

/*!
    \since 4.5

    Returns the locale for this stream. The default locale is C.

    \sa setLocale()
*/
QLocale BOBUIextStream::locale() const
{
    Q_D(const BOBUIextStream);
    return d->locale;
}

BOBUI_END_NAMESPACE
