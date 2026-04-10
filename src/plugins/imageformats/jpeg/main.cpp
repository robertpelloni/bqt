// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qimageiohandler.h>
#include <qstringlist.h>

#ifdef BOBUI_NO_IMAGEFORMAT_JPEG
#undef BOBUI_NO_IMAGEFORMAT_JPEG
#endif
#include <qjpeghandler_p.h>

BOBUI_BEGIN_NAMESPACE

class QJpegPlugin : public QImageIOPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QImageIOHandlerFactoryInterface" FILE "jpeg.json")
public:
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const override;
};

QImageIOPlugin::Capabilities QJpegPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "jpeg" || format == "jpg" || format == "jfif")
        return Capabilities(CanRead | CanWrite);
    if (!format.isEmpty())
        return { };
    if (!device->isOpen())
        return { };

    Capabilities cap;
    if (device->isReadable() && QJpegHandler::canRead(device))
        cap |= CanRead;
    if (device->isWritable())
        cap |= CanWrite;
    return cap;
}

QImageIOHandler *QJpegPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new QJpegHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

BOBUI_END_NAMESPACE

#include "main.moc"
