// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qimageiohandler.h>
#include <qstringlist.h>

#ifdef BOBUI_NO_IMAGEFORMAT_GIF
#undef BOBUI_NO_IMAGEFORMAT_GIF
#endif
#include <qgifhandler_p.h>

BOBUI_BEGIN_NAMESPACE

class QGifPlugin : public QImageIOPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QImageIOHandlerFactoryInterface" FILE "gif.json")
public:
    QGifPlugin();
    ~QGifPlugin();

    Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const override;
};

QGifPlugin::QGifPlugin()
{
}

QGifPlugin::~QGifPlugin()
{
}

QImageIOPlugin::Capabilities QGifPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "gif" || (device && device->isReadable() && QGifHandler::canRead(device)))
        return Capabilities(CanRead);
    return { };
}

QImageIOHandler *QGifPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new QGifHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

BOBUI_END_NAMESPACE

#include "main.moc"
