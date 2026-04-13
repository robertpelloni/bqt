// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qminimalbackingstore.h"
#include "qminimalintegration.h"
#include "qscreen.h"
#include <BobUICore/qdebug.h>
#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QMinimalBackingStore::QMinimalBackingStore(QWindow *window)
    : QPlatformBackingStore(window)
    , mDebug(QMinimalIntegration::instance()->options() & QMinimalIntegration::DebugBackingStore)
{
    if (mDebug)
        qDebug() << "QMinimalBackingStore::QMinimalBackingStore:" << (quintptr)this;
}

QMinimalBackingStore::~QMinimalBackingStore()
{
}

QPaintDevice *QMinimalBackingStore::paintDevice()
{
    if (mDebug)
        qDebug("QMinimalBackingStore::paintDevice");

    return &mImage;
}

void QMinimalBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
    Q_UNUSED(window);
    Q_UNUSED(region);
    Q_UNUSED(offset);

    if (mDebug) {
        static int c = 0;
        QString filename = QString("output%1.png").arg(c++, 4, 10, QChar(u'0'));
        qDebug() << "QMinimalBackingStore::flush() saving contents to" << filename.toLocal8Bit().constData();
        mImage.save(filename);
    }
}

void QMinimalBackingStore::resize(const QSize &size, const QRegion &)
{
    QImage::Format format = QGuiApplication::primaryScreen()->handle()->format();
    if (mImage.size() != size)
        mImage = QImage(size, format);
}

BOBUI_END_NAMESPACE
