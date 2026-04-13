// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mockcompositor.h"
#include <BobUIGui/QRasterWindow>
#if BOBUI_CONFIG(opengl)
#include <BobUIOpenGL/QOpenGLWindow>
#endif

using namespace MockCompositor;

class tst_surface : public QObject, private DefaultCompositor
{
    Q_OBJECT
public:
    explicit tst_surface();
private slots:
    void cleanup() { BOBUIRY_VERIFY2(isClean(), qPrintable(dirtyMessage())); }
    void createDestroySurface();
    void waitForFrameCallbackRaster();
#if BOBUI_CONFIG(opengl)
    void waitForFrameCallbackGl();
#endif
    void negotiateShmFormat();

    // Subsurfaces
    void createSubsurface();
    void createSubsurfaceForHiddenParent();
    void changeToSubsurface();
};

tst_surface::tst_surface()
{
    m_config.autoFrameCallback = false;
}

void tst_surface::createDestroySurface()
{
    QWindow window;
    window.show();

    QCOMPOSITOR_TRY_VERIFY(surface());

    window.destroy();
    QCOMPOSITOR_TRY_VERIFY(!surface());
}

void tst_surface::waitForFrameCallbackRaster()
{
    class TestWindow : public QRasterWindow {
    public:
        explicit TestWindow() { resize(40, 40); }
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);
            update();
        }
    };
    TestWindow window;
    window.show();
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel());
    QSignalSpy bufferSpy(exec([&] { return xdgSurface()->m_surface; }), &Surface::bufferCommitted);
    exec([&] { xdgToplevel()->sendCompleteConfigure(); });

    // We should get the first buffer without waiting for a frame callback
    BOBUIRY_COMPARE(bufferSpy.size(), 1);
    bufferSpy.removeFirst();

    // Make sure we follow frame callbacks for some frames
    for (int i = 0; i < 5; ++i) {
        xdgPingAndWaitForPong(); // Make sure things have happened on the client
        exec([&] {
            QVERIFY(bufferSpy.empty()); // Make sure no extra buffers have arrived
            QVERIFY(!xdgToplevel()->surface()->m_waitingFrameCallbacks.empty());
            xdgToplevel()->surface()->sendFrameCallbacks();
        });
        BOBUIRY_COMPARE(bufferSpy.size(), 1);
        bufferSpy.removeFirst();
    }
}

#if BOBUI_CONFIG(opengl)
void tst_surface::waitForFrameCallbackGl()
{
    class TestWindow : public QOpenGLWindow {
    public:
        explicit TestWindow()
        {
            resize(40, 40);
            connect(this, &QOpenGLWindow::frameSwapped,
                    this, QOverload<>::of(&QPaintDeviceWindow::update));
            update();
        }
        void paintGL() override
        {
            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    };
    TestWindow window;
    window.show();
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel());
    QSignalSpy bufferSpy(exec([&] { return xdgSurface()->m_surface; }), &Surface::bufferCommitted);
    exec([&] { xdgToplevel()->sendCompleteConfigure(); });

    // We should get the first buffer without waiting for a frame callback
    BOBUIRY_COMPARE(bufferSpy.size(), 1);
    bufferSpy.removeFirst();

    // Make sure we follow frame callbacks for some frames
    for (int i = 0; i < 5; ++i) {
        xdgPingAndWaitForPong(); // Make sure things have happened on the client
        if (!qEnvironmentVariableIntValue("BOBUI_WAYLAND_DISABLE_WINDOWDECORATION") && i == 0) {
            QCOMPARE(bufferSpy.size(), 1);
            bufferSpy.removeFirst();
        }
        exec([&] {
            QVERIFY(bufferSpy.empty()); // Make sure no extra buffers have arrived
            QVERIFY(!xdgToplevel()->surface()->m_waitingFrameCallbacks.empty());
            xdgToplevel()->surface()->sendFrameCallbacks();
        });
        BOBUIRY_COMPARE(bufferSpy.size(), 1);
        bufferSpy.removeFirst();
    }
}
#endif // BOBUI_CONFIG(opengl)

void tst_surface::negotiateShmFormat()
{
    QSKIP("TODO: I'm not sure why we're choosing xrgb over argb in this case...");
    QRasterWindow window;
    window.setFlag(BobUI::FramelessWindowHint); // decorations force alpha
    QSurfaceFormat format;
    format.setAlphaBufferSize(0);
    window.setFormat(format);
    window.resize(64, 48);
    window.show();
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel());
    QSignalSpy bufferSpy(exec([&] { return xdgSurface()->m_surface; }), &Surface::bufferCommitted);
    const uint serial = exec([&] { return xdgToplevel()->sendCompleteConfigure(); });
    QCOMPOSITOR_TRY_COMPARE(xdgSurface()->m_committedConfigureSerial, serial);
    exec([&] {
        Buffer *buffer = xdgToplevel()->surface()->m_committed.buffer;
        QVERIFY(buffer);
        auto *shmBuffer = ShmBuffer::fromBuffer(buffer);
        QVERIFY(shmBuffer);
        qDebug() << "shmBuffer->m_format" << shmBuffer->m_format;
        QCOMPARE(shmBuffer->m_format, Shm::format_xrgb8888);
    });
}

void tst_surface::createSubsurface()
{
    m_config.autoFrameCallback = true;
    auto autoFrameCallback = qScopeGuard([&] { m_config.autoFrameCallback = false; });

    QRasterWindow window;
    window.setObjectName("main");
    window.resize(200, 200);

    QRasterWindow subWindow;
    subWindow.setObjectName("subwindow");
    subWindow.setParent(&window);
    subWindow.resize(64, 64);

    window.show();
    subWindow.show();

    QCOMPOSITOR_TRY_VERIFY(subSurface());
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel());
    exec([&] { xdgToplevel()->sendCompleteConfigure(); });
    QCOMPOSITOR_TRY_VERIFY(xdgSurface()->m_committedConfigureSerial);

    const Surface *mainSurface = exec([&] {return surface(0);});
    const Surface *childSurface = exec([&] {return surface(1);});
    QSignalSpy  mainSurfaceCommitSpy(mainSurface, &Surface::commit);
    QSignalSpy childSurfaceCommitSpy(childSurface, &Surface::commit);

    // Move subsurface. The parent should redraw and commit
    subWindow.setGeometry(100, 100, 64, 64);
    // the toplevel should commit to indicate the subsurface moved
    QCOMPOSITOR_TRY_COMPARE(mainSurfaceCommitSpy.size(), 1);
    mainSurfaceCommitSpy.clear();
    childSurfaceCommitSpy.clear();

    // Move and resize the subSurface. The parent should redraw and commit
    // The child should also redraw
    subWindow.setGeometry(50, 50, 80, 80);
    QCOMPOSITOR_TRY_COMPARE(mainSurfaceCommitSpy.size(), 1);
    QCOMPOSITOR_TRY_COMPARE(childSurfaceCommitSpy.size(), 1);

}

// Used to cause a crash in libwayland (BOBUIBUG-79674)
void tst_surface::createSubsurfaceForHiddenParent()
{
    QRasterWindow window;
    window.resize(64, 64);
    window.show();
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel());
    exec([&] { xdgToplevel()->sendCompleteConfigure(); });
    QCOMPOSITOR_TRY_VERIFY(xdgSurface()->m_committedConfigureSerial);

    window.hide();

    QRasterWindow subWindow;
    subWindow.setParent(&window);
    subWindow.resize(64, 64);
    subWindow.show();

    // Make sure the client doesn't quit before it has a chance to crash
    xdgPingAndWaitForPong();

    // Make sure the subsurface was actually created
    const Subsurface *subsurface = exec([&] {return subSurface(0);});
    QVERIFY(subsurface);
}

void tst_surface::changeToSubsurface()
{
    QRasterWindow window1;
    window1.resize(64, 64);
    window1.show();

    QRasterWindow window2;
    window2.resize(64, 64);
    window2.show();

    window2.setParent(&window1);
    QCOMPOSITOR_TRY_VERIFY(subSurface());

    window2.setParent(nullptr);
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel(1));

    window2.hide();
    window2.setParent(&window1);
    window2.show();
    QCOMPOSITOR_TRY_VERIFY(subSurface());

    window2.hide();
    window2.setParent(nullptr);
    window2.show();
    QCOMPOSITOR_TRY_VERIFY(xdgToplevel(1));
}

QCOMPOSITOR_TEST_MAIN(tst_surface)
#include "tst_surface.moc"
