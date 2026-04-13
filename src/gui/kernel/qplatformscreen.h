// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMSCREEN_H
#define QPLATFORMSCREEN_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qvariant.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qobject.h>

#include <BobUIGui/qcolorspace.h>
#include <BobUIGui/qcursor.h>
#include <BobUIGui/qimage.h>
#include <BobUIGui/qwindowdefs.h>
#include <qpa/qplatformpixmap.h>

BOBUI_BEGIN_NAMESPACE


class QPlatformBackingStore;
class QPlatformScreenPrivate;
class QPlatformWindow;
class QPlatformCursor;
class QScreen;
class QSurfaceFormat;

typedef std::pair<qreal, qreal> QDpi;


class Q_GUI_EXPORT QPlatformScreen
{
    Q_GADGET
    Q_DECLARE_PRIVATE(QPlatformScreen)

public:
    Q_DISABLE_COPY_MOVE(QPlatformScreen)

    enum SubpixelAntialiasingType { // copied from qfontengine_p.h since we can't include private headers
        Subpixel_None,
        Subpixel_RGB,
        Subpixel_BGR,
        Subpixel_VRGB,
        Subpixel_VBGR
    };

    enum PowerState {
        PowerStateOn,
        PowerStateStandby,
        PowerStateSuspend,
        PowerStateOff
    };

    struct Mode {
        QSize size;
        qreal refreshRate;
    };

    QPlatformScreen();
    virtual ~QPlatformScreen();

    virtual bool isPlaceholder() const { return false; }

    virtual QPixmap grabWindow(WId window, int x, int y, int width, int height) const;

    virtual QRect geometry() const = 0;
    virtual QRect availableGeometry() const {return geometry();}

    virtual int depth() const = 0;
    virtual QImage::Format format() const = 0;
    virtual QColorSpace colorSpace() const { return QColorSpace::SRgb; }

    virtual QSizeF physicalSize() const;
    virtual QDpi logicalDpi() const;
    virtual QDpi logicalBaseDpi() const;
    virtual qreal devicePixelRatio() const;

    virtual qreal refreshRate() const;

    virtual BobUI::ScreenOrientation nativeOrientation() const;
    virtual BobUI::ScreenOrientation orientation() const;

    virtual QWindow *topLevelAt(const QPoint &point) const;
    QWindowList windows() const;

    virtual QList<QPlatformScreen *> virtualSiblings() const;
    const QPlatformScreen *screenForPosition(const QPoint &point) const;

    QScreen *screen() const;

    //jl: should this function be in QPlatformIntegration
    //jl: maybe screenForWindow is a better name?
    static QPlatformScreen *platformScreenForWindow(const QWindow *window);

    virtual QString name() const { return QString(); }

    virtual QString manufacturer() const;
    virtual QString model() const;
    virtual QString serialNumber() const;

    virtual QPlatformCursor *cursor() const;
    virtual SubpixelAntialiasingType subpixelAntialiasingTypeHint() const;

    virtual PowerState powerState() const;
    virtual void setPowerState(PowerState state);

    virtual QList<Mode> modes() const;

    virtual int currentMode() const;
    virtual int preferredMode() const;

    static int angleBetween(BobUI::ScreenOrientation a, BobUI::ScreenOrientation b);
    static BOBUIransform transformBetween(BobUI::ScreenOrientation a, BobUI::ScreenOrientation b, const QRect &target);
    static QRect mapBetween(BobUI::ScreenOrientation a, BobUI::ScreenOrientation b, const QRect &rect);

    static QDpi overrideDpi(const QDpi &in);

protected:
    void resizeMaximizedWindows();

    QScopedPointer<QPlatformScreenPrivate> d_ptr;

private:
    friend class QScreen;
};

// BobUI doesn't currently support running with no platform screen
// QPA plugins can use this class to create a fake screen
class Q_GUI_EXPORT QPlatformPlaceholderScreen : public QPlatformScreen {
public:
    // virtualSibling can be passed in to make the placeholder a sibling with other screens during
    // the transitioning phase when the real screen is about to be removed, or the first real screen
    // is about to be added. This is useful because BobUI will currently recreate (but now show!)
    // windows when they are moved from one virtual desktop to another, so if the last monitor is
    // unplugged, then plugged in again, windows will be hidden unless the placeholder belongs to
    // the same virtual desktop as the other screens.
    QPlatformPlaceholderScreen(bool virtualSibling = true) : m_virtualSibling(virtualSibling) {}
    bool isPlaceholder() const override { return true; }
    QRect geometry() const override { return QRect(); }
    QRect availableGeometry() const override { return QRect(); }
    int depth() const override { return 32; }
    QImage::Format format() const override { return QImage::Format::Format_RGB32; }
    QList<QPlatformScreen *> virtualSiblings() const override;
private:
    bool m_virtualSibling = true;
};

BOBUI_END_NAMESPACE

#endif // QPLATFORMSCREEN_H
