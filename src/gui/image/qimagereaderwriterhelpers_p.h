// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIMAGEREADERWRITERHELPERS_P_H
#define QIMAGEREADERWRITERHELPERS_P_H

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <qsharedpointer.h>
#include "qimageiohandler.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

BOBUI_BEGIN_NAMESPACE

class QFactoryLoader;

namespace QImageReaderWriterHelpers {

enum _bobui_BuiltInFormatType {
#ifndef BOBUI_NO_IMAGEFORMAT_PNG
    _bobui_PngFormat,
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_BMP
    _bobui_BmpFormat,
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_PPM
    _bobui_PpmFormat,
    _bobui_PgmFormat,
    _bobui_PbmFormat,
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_XBM
    _bobui_XbmFormat,
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_XPM
    _bobui_XpmFormat,
#endif
    _bobui_NumFormats,
    _bobui_NoFormat = -1
};

#if !defined(BOBUI_NO_IMAGEFORMAT_PPM)
# define MAX_MT_SIZE 20
#elif !defined(BOBUI_NO_IMAGEFORMAT_XBM) || !defined(BOBUI_NO_IMAGEFORMAT_XPM)
#  define MAX_MT_SIZE 10
#else
#  define MAX_MT_SIZE 4
#endif

struct _bobui_BuiltInFormatStruct
{
    char extension[4];
    char mimeType[MAX_MT_SIZE];
};

#undef MAX_MT_SIZE

static const _bobui_BuiltInFormatStruct _bobui_BuiltInFormats[] = {
#ifndef BOBUI_NO_IMAGEFORMAT_PNG
    {"png", "png"},
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_BMP
    {"bmp", "bmp"},
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_PPM
    {"ppm", "x-portable-pixmap"},
    {"pgm", "x-portable-graymap"},
    {"pbm", "x-portable-bitmap"},
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_XBM
    {"xbm", "x-xbitmap"},
#endif
#ifndef BOBUI_NO_IMAGEFORMAT_XPM
    {"xpm", "x-xpixmap"},
#endif
};
static_assert(_bobui_NumFormats == sizeof _bobui_BuiltInFormats / sizeof *_bobui_BuiltInFormats);

#ifndef BOBUI_NO_IMAGEFORMATPLUGIN
QSharedPointer<QFactoryLoader> pluginLoader();
#endif

enum Capability {
    CanRead,
    CanWrite
};
QList<QByteArray> supportedImageFormats(Capability cap);
QList<QByteArray> supportedMimeTypes(Capability cap);
QList<QByteArray> imageFormatsForMimeType(QByteArrayView mimeType, Capability cap);

}

BOBUI_END_NAMESPACE

#endif // QIMAGEREADERWRITERHELPERS_P_H
