// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSCREEN_H
#define QWAYLANDSCREEN_H

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

#include <qpa/qplatformscreen.h>
#include <BobUIGui/qscreen_platform.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwayland-xdg-output-unstable-v1.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;
class QWaylandCursor;

class Q_WAYLANDCLIENT_EXPORT QWaylandXdgOutputManagerV1 : public BobUIWayland::zxdg_output_manager_v1 {
public:
    QWaylandXdgOutputManagerV1(QWaylandDisplay *display, uint id, uint version);
    ~QWaylandXdgOutputManagerV1();
};

class Q_WAYLANDCLIENT_EXPORT QWaylandScreen : public QPlatformScreen,
                                              BobUIWayland::wl_output,
                                              BobUIWayland::zxdg_output_v1,
                                              public QNativeInterface::QWaylandScreen
{
public:
    QWaylandScreen(QWaylandDisplay *waylandDisplay, int version, uint32_t id);
    ~QWaylandScreen() override;

    void maybeInitialize();

    void initXdgOutput(QWaylandXdgOutputManagerV1 *xdgOutputManager);

    QWaylandDisplay *display() const;

    QString manufacturer() const override;
    QString model() const override;

    QRect geometry() const override;
    int depth() const override;
    QImage::Format format() const override;

    QSizeF physicalSize() const override;

    QDpi logicalDpi() const override;
    QList<QPlatformScreen *> virtualSiblings() const override;

    BobUI::ScreenOrientation orientation() const override;
    int scale() const;
    qreal devicePixelRatio() const override;
    qreal refreshRate() const override;

    QString name() const override { return mOutputName; }

#if BOBUI_CONFIG(cursor)
    QPlatformCursor *cursor() const override;
#endif

    SubpixelAntialiasingType subpixelAntialiasingTypeHint() const override;

    uint32_t outputId() const { return m_outputId; }
    ::wl_output *output() const override
    {
        return const_cast<::wl_output *>(BobUIWayland::wl_output::object());
    }

    static QWaylandScreen *waylandScreenFromWindow(QWindow *window);
    static QWaylandScreen *fromWlOutput(::wl_output *output);

    BobUI::ScreenOrientation toScreenOrientation(int wlTransform,
                                              BobUI::ScreenOrientation fallback) const;

protected:
    enum Event : uint {
        XdgOutputDoneEvent = 0x1,
        OutputDoneEvent = 0x2,
        XdgOutputNameEvent = 0x4,
        OutputNameEvent = 0x8,
    };
    uint requiredEvents() const;

    void output_mode(uint32_t flags, int width, int height, int refresh) override;
    void output_geometry(int32_t x, int32_t y,
                         int32_t width, int32_t height,
                         int subpixel,
                         const QString &make,
                         const QString &model,
                         int32_t transform) override;
    void output_scale(int32_t factor) override;
    void output_done() override;
    void output_name(const QString &name) override;
    void updateOutputProperties();

    // XdgOutput
    void zxdg_output_v1_logical_position(int32_t x, int32_t y) override;
    void zxdg_output_v1_logical_size(int32_t width, int32_t height) override;
    void zxdg_output_v1_done() override;
    void zxdg_output_v1_name(const QString &name) override;
    void updateXdgOutputProperties();

    int m_outputId;
    QWaylandDisplay *mWaylandDisplay = nullptr;
    QString mManufacturer;
    QString mModel;
    QRect mGeometry;
    QRect mXdgGeometry;
    int mScale = 1;
    int mDepth = 32;
    int mRefreshRate = 60000;
    int mSubpixel = -1;
    int mTransform = -1;
    QImage::Format mFormat = QImage::Format_ARGB32_Premultiplied;
    QSize mPhysicalSize;
    QString mOutputName;
    BobUI::ScreenOrientation m_orientation = BobUI::PrimaryOrientation;
    uint mProcessedEvents = 0;
    bool mInitialized = false;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDSCREEN_H
