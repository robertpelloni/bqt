// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include "bobuiextdocumentwriter.h"

#include <BobUICore/qfile.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qfileinfo.h>
#include <BobUICore/bobuiextstream.h>
#include <BobUICore/qdebug.h>
#include "bobuiextdocument.h"
#include "bobuiextdocumentfragment.h"

#include "bobuiextdocumentfragment_p.h"
#include "bobuiextodfwriter_p.h"
#if BOBUI_CONFIG(textmarkdownwriter)
#include "bobuiextmarkdownwriter_p.h"
#endif

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocumentWriterPrivate
{
public:
    BOBUIextDocumentWriterPrivate(BOBUIextDocumentWriter* qq);

    // device
    QByteArray format;
    QIODevice *device;
    bool deleteDevice;

    BOBUIextDocumentWriter *q;
};

/*!
    \since 4.5
    \class BOBUIextDocumentWriter

    \brief The BOBUIextDocumentWriter class provides a format-independent interface for writing a BOBUIextDocument to files or other devices.
    \inmodule BobUIGui

    \ingroup richtext-processing

    To write a document, construct a BOBUIextDocumentWriter object with either a
    file name or a device object, and specify the document format to be
    written. You can construct a writer and set the format using setFormat()
    later.

    Call write() to write the document to the device. If the document is
    successfully written, this function returns \c true. However, if an error
    occurs when writing the document, it will return false.

    Call supportedDocumentFormats() for a list of formats that
    BOBUIextDocumentWriter can write.

    Since the capabilities of the supported output formats vary considerably,
    the writer simply outputs the appropriate subset of objects for each format.
    This typically includes the formatted text and images contained in a
    document.
*/

/*!
    \internal
*/
BOBUIextDocumentWriterPrivate::BOBUIextDocumentWriterPrivate(BOBUIextDocumentWriter *qq)
    : device(nullptr),
    deleteDevice(false),
    q(qq)
{
}

/*!
    Constructs an empty BOBUIextDocumentWriter object. Before writing, you must
    call setFormat() to set a document format, then setDevice() or
    setFileName().
*/
BOBUIextDocumentWriter::BOBUIextDocumentWriter()
    : d(new BOBUIextDocumentWriterPrivate(this))
{
}

/*!
    Constructs a BOBUIextDocumentWriter object to write to the given \a device
    in the document format specified by \a format.
*/
BOBUIextDocumentWriter::BOBUIextDocumentWriter(QIODevice *device, const QByteArray &format)
    : d(new BOBUIextDocumentWriterPrivate(this))
{
    d->device = device;
    d->format = format;
}

/*!
    Constructs an BOBUIextDocumentWriter object that will write to a file with
    the name \a fileName, using the document format specified by \a format.
    If \a format is not provided, BOBUIextDocumentWriter will detect the document
    format by inspecting the extension of \a fileName.
*/
BOBUIextDocumentWriter::BOBUIextDocumentWriter(const QString &fileName, const QByteArray &format)
    : BOBUIextDocumentWriter(new QFile(fileName), format)
{
    d->deleteDevice = true;
}

/*!
    Destroys the BOBUIextDocumentWriter object.
*/
BOBUIextDocumentWriter::~BOBUIextDocumentWriter()
{
    if (d->deleteDevice)
        delete d->device;
    delete d;
}

/*!
    Sets the format used to write documents to the \a format specified.
    \a format is a case insensitive text string. For example:

    \snippet code/src_gui_text_bobuiextdocumentwriter.cpp 0

    You can call supportedDocumentFormats() for the full list of formats
    BOBUIextDocumentWriter supports.

    \sa format()
*/
void BOBUIextDocumentWriter::setFormat (const QByteArray &format)
{
    d->format = format;
}

/*!
    Returns the format used for writing documents.

    \sa setFormat()
*/
QByteArray BOBUIextDocumentWriter::format () const
{
    return d->format;
}

/*!
    Sets the writer's device to the \a device specified. If a device has
    already been set, the old device is removed but otherwise left
    unchanged.

    If the device is not already open, BOBUIextDocumentWriter will attempt to
    open the device in \l {QIODeviceBase::}{WriteOnly} mode by calling open().

    \note This will not work for certain devices, such as QProcess,
    BOBUIcpSocket and QUdpSocket, where some configuration is required before
    the device can be opened.

    \sa device(), setFileName()
*/
void BOBUIextDocumentWriter::setDevice (QIODevice *device)
{
    if (d->device && d->deleteDevice)
        delete d->device;

    d->device = device;
    d->deleteDevice = false;
}

/*!
    Returns the device currently assigned, or \nullptr if no device
    has been assigned.
*/
QIODevice *BOBUIextDocumentWriter::device () const
{
    return d->device;
}

/*!
    Sets the name of the file to be written to \a fileName. Internally,
    BOBUIextDocumentWriter will create a QFile and open it in \l
    {QIODeviceBase::}{WriteOnly} mode, and use this file when writing the
    document.

    \sa fileName(), setDevice()
*/
void BOBUIextDocumentWriter::setFileName (const QString &fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

/*!
    If the currently assigned device is a QFile, or if setFileName()
    has been called, this function returns the name of the file
    to be written to. In all other cases, it returns an empty string.

    \sa setFileName(), setDevice()
*/
QString BOBUIextDocumentWriter::fileName () const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

/*!
    Writes the given \a document to the assigned device or file and
    returns \c true if successful; otherwise returns \c false.
*/
bool BOBUIextDocumentWriter::write(const BOBUIextDocument *document)
{
    if (!d->device)
        return false;

    QByteArray suffix;
    if (d->format.isEmpty()) {
        // if there's no format, see if device is a file, and if so, find
        // the file suffix
        if (QFile *file = qobject_cast<QFile *>(d->device))
            suffix = QFileInfo(file->fileName()).suffix().toLower().toLatin1();
    }

    QByteArray format = !d->format.isEmpty() ? d->format.toLower() : suffix;

#ifndef BOBUI_NO_TEXTODFWRITER
    if (format == "odf" || format == "opendocumentformat" || format == "odt") {
        BOBUIextOdfWriter writer(*document, d->device);
        return writer.writeAll();
    }
#endif // BOBUI_NO_TEXTODFWRITER

#if BOBUI_CONFIG(textmarkdownwriter)
    if (format == "md" || format == "mkd" || format == "markdown") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning("BOBUIextDocumentWriter::write: the device can not be opened for writing");
            return false;
        }
        BOBUIextStream s(d->device);
        BOBUIextMarkdownWriter writer(s, BOBUIextDocument::MarkdownDialectGitHub);
        return writer.writeAll(document);
    }
#endif // textmarkdownwriter

#ifndef BOBUI_NO_TEXTHTMLPARSER
    if (format == "html" || format == "htm") {
        if (!d->device->isWritable() && ! d->device->open(QIODevice::WriteOnly)) {
            qWarning("BOBUIextDocumentWriter::write: the device cannot be opened for writing");
            return false;
        }
        d->device->write(document->toHtml().toUtf8());
        d->device->close();
        return true;
    }
#endif
    if (format == "txt" || format == "plaintext") {
        if (!d->device->isWritable() && ! d->device->open(QIODevice::WriteOnly)) {
            qWarning("BOBUIextDocumentWriter::write: the device cannot be opened for writing");
            return false;
        }
        d->device->write(document->toPlainText().toUtf8());
        d->device->close();
        return true;
    }

    return false;
}

/*!
    Writes the document fragment specified by \a fragment to the assigned device
    or file and returns \c true if successful; otherwise returns \c false.
*/
bool BOBUIextDocumentWriter::write(const BOBUIextDocumentFragment &fragment)
{
    if (fragment.d == nullptr)
        return false; // invalid fragment.
    BOBUIextDocument *doc = fragment.d->doc;
    if (doc)
        return write(doc);
    return false;
}

/*!
    Returns the list of document formats supported by BOBUIextDocumentWriter.

    By default, BobUI can write the following formats:

    \table
    \header \li Format    \li Description
    \row    \li plaintext \li Plain text
    \row    \li HTML      \li HyperText Markup Language
    \row    \li markdown  \li Markdown (CommonMark or GitHub dialects)
    \row    \li ODF       \li OpenDocument Format
    \endtable

    \sa setFormat()
*/
QList<QByteArray> BOBUIextDocumentWriter::supportedDocumentFormats()
{
    QList<QByteArray> answer;
    answer << "plaintext";

#ifndef BOBUI_NO_TEXTHTMLPARSER
    answer << "HTML";
#endif
#ifndef BOBUI_NO_TEXTODFWRITER
    answer << "ODF";
#endif // BOBUI_NO_TEXTODFWRITER
#if BOBUI_CONFIG(textmarkdownwriter)
    answer << "markdown";
#endif

    std::sort(answer.begin(), answer.end());
    return answer;
}

BOBUI_END_NAMESPACE
