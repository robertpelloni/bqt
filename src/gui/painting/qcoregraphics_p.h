// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOREGRAPHICS_P_H
#define QCOREGRAPHICS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qcore_mac_p.h>

#include <BobUICore/qsize.h>

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qicon.h>
#include <BobUIGui/qpalette.h>
#include <BobUIGui/qregion.h>

#include <CoreGraphics/CoreGraphics.h>

#if defined(Q_OS_MACOS)
# if defined(__OBJC__)
#  include <AppKit/NSImage.h>
# else
Q_FORWARD_DECLARE_OBJC_CLASS(NSImage);
# endif
#endif

Q_FORWARD_DECLARE_OBJC_CLASS(UIImage);
Q_FORWARD_DECLARE_OBJC_CLASS(NSColor);

struct vImage_CGImageFormat;

BOBUI_BEGIN_NAMESPACE

Q_GUI_EXPORT std::optional<vImage_CGImageFormat> bobui_mac_cgImageFormatForImage(const QImage &image);

#ifdef BOBUI_PLATFORM_UIKIT
Q_GUI_EXPORT QImage bobui_mac_toQImage(const UIImage *image, QSizeF size);
#endif

#ifdef Q_OS_MACOS
Q_GUI_EXPORT QPixmap bobui_mac_toQPixmap(const NSImage *image, const QSizeF &size);

#if defined(__OBJC__)
BOBUI_END_NAMESPACE

// @compatibility_alias doesn't work with categories or their methods
#define imageFromQImage BOBUI_MANGLE_NAMESPACE(imageFromQImage)
#define imageFromQIcon BOBUI_MANGLE_NAMESPACE(imageFromQIcon)

@interface NSImage (BobUIExtras)
+ (instancetype)imageFromQImage:(const BOBUI_PREPEND_NAMESPACE(QImage) &)image;
+ (instancetype)imageFromQIcon:(const BOBUI_PREPEND_NAMESPACE(QIcon) &)icon;
+ (instancetype)imageFromQIcon:(const BOBUI_PREPEND_NAMESPACE(QIcon) &)icon
                      withSize:(const BOBUI_PREPEND_NAMESPACE(QSize) &)size;
+ (instancetype)imageFromQIcon:(const BOBUI_PREPEND_NAMESPACE(QIcon) &)icon
                      withSize:(const BOBUI_PREPEND_NAMESPACE(QSize) &)size
                      withMode:(BOBUI_PREPEND_NAMESPACE(QIcon)::Mode)mode
                     withState:(BOBUI_PREPEND_NAMESPACE(QIcon)::State)state;
+ (instancetype)internalImageFromQIcon:(const BOBUI_PREPEND_NAMESPACE(QIcon) &)icon;
@end
BOBUI_BEGIN_NAMESPACE
#endif // __OBJC__

#endif
Q_GUI_EXPORT CGImageRef bobui_mac_toCGImage(const QImage &qImage);
Q_GUI_EXPORT QImage bobui_mac_toQImage(CGImageRef image);

Q_GUI_EXPORT QImage bobui_mac_padToSquareImage(const QImage &image);

Q_GUI_EXPORT void bobui_mac_drawCGImage(CGContextRef inContext, const CGRect *inBounds, CGImageRef inImage);

Q_GUI_EXPORT void bobui_mac_clip_cg(CGContextRef hd, const QRegion &rgn, CGAffineTransform *orig_xform);

#ifdef Q_OS_MACOS
Q_GUI_EXPORT QColor bobui_mac_toQColor(const NSColor *color);
Q_GUI_EXPORT QBrush bobui_mac_toQBrush(const NSColor *color, QPalette::ColorGroup colorGroup = QPalette::Normal);
#endif
Q_GUI_EXPORT QColor bobui_mac_toQColor(CGColorRef color);
Q_GUI_EXPORT QBrush bobui_mac_toQBrush(CGColorRef color);

class Q_GUI_EXPORT QMacCGContext
{
public:
    QMacCGContext() = default;
    QMacCGContext(QPaintDevice *pdev);
    QMacCGContext(QPainter *p);

    operator CGContextRef() { return context; }

private:
    void initialize(QPaintDevice *paintDevice);
    void initialize(const QImage *, QPainter *painter = nullptr);
    QCFType<CGContextRef> context;
};

BOBUI_END_NAMESPACE

#endif // QCOREGRAPHICS_P_H
