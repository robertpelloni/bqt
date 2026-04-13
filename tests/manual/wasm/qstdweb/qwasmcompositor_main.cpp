// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QEvent>
#include <BobUICore/QObject>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/qguiapplication.h>
#include <BobUIGui/qoffscreensurface.h>
#include <BobUIGui/qpa/qwindowsysteminterface.h>
#include <BobUIGui/rhi/qrhi.h>

#include <bobuiwasmtestlib.h>

#include <emscripten.h>
#include <emscripten/val.h>

#include <functional>
#include <memory>
#include <vector>

namespace tst_qwasmcompositor_internal {
namespace {
class Window : public QWindow
{
    Q_OBJECT
public:
    Window();
    ~Window() override { qDebug() << "dtor Window"; }

    void keyPressEvent(QKeyEvent *) final;

signals:
    void exposed();
    void keyEventReceived();
    void initFailed();

protected:
private:
    void init();

    void exposeEvent(QExposeEvent *) override;
    bool m_exposedOnce = false;

    std::unique_ptr<QOffscreenSurface> m_fallbackSurface;
    std::unique_ptr<QRhi> m_rhi;
};

Window::Window()
{
    setSurfaceType(OpenGLSurface);
}

void Window::exposeEvent(QExposeEvent *)
{
    if (isExposed() && !m_exposedOnce) {
        m_exposedOnce = true;
        init();
        emit exposed();
    }
}

void Window::keyPressEvent(QKeyEvent *)
{
    emit keyEventReceived();
}

void Window::init()
{
    QRhi::Flags rhiFlags = QRhi::EnableDebugMarkers;

    m_fallbackSurface.reset(QRhiGles2InitParams::newFallbackSurface());
    QRhiGles2InitParams params;
    params.fallbackSurface = m_fallbackSurface.get();
    params.window = this;

    // Double init of RHI causes the OpenGL context to be destroyed, which causes a bug with input.
    m_rhi.reset(QRhi::create(QRhi::OpenGLES2, &params, rhiFlags));
    m_rhi.reset(QRhi::create(QRhi::OpenGLES2, &params, rhiFlags));

    if (!m_rhi)
        emit initFailed();
}

} // namespace
} // namespace tst_qwasmcompositor_internal

using namespace emscripten;

class QWasmCompositorTest : public QObject
{
    Q_OBJECT

public:
    QWasmCompositorTest() : m_window(val::global("window")), m_testSupport(val::object())
    {
        m_window.set("testSupport", m_testSupport);
        m_testSupport.set("bobuiSetContainerElements",
                          emscripten::val::module_property("bobuiSetContainerElements"));
    }

    ~QWasmCompositorTest() noexcept
    {
        qDebug() << this << "In dtor";
        for (auto it = m_cleanup.rbegin(); it != m_cleanup.rend(); ++it)
            (*it)();
        m_window.set("testSupport", emscripten::val::undefined());
    }

private:
    void init()
    {
        EM_ASM({
            testSupport.screenElement = document.createElement("div");
            testSupport.screenElement.id = "test-canvas-qwasmcompositor";
            document.body.appendChild(testSupport.screenElement);
        });
        m_cleanup.emplace_back([]() mutable {
            EM_ASM({
                testSupport.bobuiSetContainerElements([]);
                testSupport.screenElement.parentElement.removeChild(testSupport.screenElement);
            });
        });

        EM_ASM({ testSupport.bobuiSetContainerElements([testSupport.screenElement]); });
    }

    template<class T>
    T *Own(T *plainPtr)
    {
        m_cleanup.emplace_back([plainPtr]() mutable { delete plainPtr; });
        return plainPtr;
    }

    val m_window;
    val m_testSupport;

    std::vector<std::function<void()>> m_cleanup;

private slots:
    void testReceivingKeyboardEventsAfterOpenGLContextReset();
};

void QWasmCompositorTest::testReceivingKeyboardEventsAfterOpenGLContextReset()
{
    init();

    using Window = tst_qwasmcompositor_internal::Window;
    Window *window = Own(new Window);
    window->show();
    window->requestActivate();

    QWindowSystemInterface::flushWindowSystemEvents();

    QObject::connect(window, &Window::keyEventReceived, []() { QWASMSUCCESS(); });
    QObject::connect(window, &Window::initFailed,
                     []() { QWASMFAIL("Cannot initialize test window"); });
    QObject::connect(window, &Window::exposed, []() {
        EM_ASM({
            testSupport.screenElement.shadowRoot.querySelector('.bobui-window')
                    .dispatchEvent(new KeyboardEvent('keydown', { key : 'a' }));
        });
    });
}

int main(int argc, char **argv)
{
    auto testObject = std::make_shared<QWasmCompositorTest>();
    BobUIWasmTest::initTestCase<QGuiApplication>(argc, argv, testObject);
    return 0;
}

#include "qwasmcompositor_main.moc"
