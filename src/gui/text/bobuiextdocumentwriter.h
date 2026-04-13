// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef BOBUIEXTDOCUMENTWRITER_H
#define BOBUIEXTDOCUMENTWRITER_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextDocumentWriterPrivate;
class QIODevice;
class QByteArray;
class BOBUIextDocument;
class BOBUIextDocumentFragment;

class Q_GUI_EXPORT BOBUIextDocumentWriter
{
public:
    BOBUIextDocumentWriter();
    BOBUIextDocumentWriter(QIODevice *device, const QByteArray &format);
    explicit BOBUIextDocumentWriter(const QString &fileName, const QByteArray &format = QByteArray());
    ~BOBUIextDocumentWriter();

    void setFormat (const QByteArray &format);
    QByteArray format () const;

    void setDevice (QIODevice *device);
    QIODevice *device () const;
    void setFileName (const QString &fileName);
    QString fileName () const;

    bool write(const BOBUIextDocument *document);
    bool write(const BOBUIextDocumentFragment &fragment);

    static QList<QByteArray> supportedDocumentFormats();

private:
    Q_DISABLE_COPY(BOBUIextDocumentWriter)
    BOBUIextDocumentWriterPrivate *d;
};

BOBUI_END_NAMESPACE

#endif
