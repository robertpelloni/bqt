// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qimageiohandler.h>
#include <qdebug.h>

#ifdef BOBUI_NO_IMAGEFORMAT_ICO
#undef BOBUI_NO_IMAGEFORMAT_ICO
#endif
#include "qicohandler.h"

BOBUI_BEGIN_NAMESPACE

class QICOPlugin : public QImageIOPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QImageIOHandlerFactoryInterface" FILE "ico.json")
public:
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const override;
};

QImageIOPlugin::Capabilities QICOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "ico" || format == "cur")
        return Capabilities(CanRead | CanWrite);
    if (!format.isEmpty())
        return { };
    if (!device->isOpen())
        return { };

    Capabilities cap;
    if (device->isReadable() && BobUIIcoHandler::canRead(device))
        cap |= CanRead;
    if (device->isWritable())
        cap |= CanWrite;
    return cap;
}

QImageIOHandler *QICOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new BobUIIcoHandler(device);

    handler->setFormat(format);
    return handler;
}

BOBUI_END_NAMESPACE

#include "main.moc"
