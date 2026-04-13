// Copyright (C) 2021 David Redondo <bobui@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QSignalSpy>
#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandintegration_p.h>
#include <BobUIWaylandClient/qwaylandclientextension.h>
#include <qwayland-server-test.h>
#include <qwayland-test.h>
#include "mockcompositor.h"
#include "coreprotocol.h"

using namespace MockCompositor;

class TestExtension
    : public QWaylandClientExtensionTemplate<TestExtension, &BobUIWayland::test_interface::release>,
      public BobUIWayland::test_interface
{
public:
    TestExtension() : QWaylandClientExtensionTemplate(1){};
    void initialize() { QWaylandClientExtension::initialize(); }
};

class TestGlobal : public Global, public BobUIWaylandServer::test_interface
{
    Q_OBJECT
public:
    explicit TestGlobal(CoreCompositor *compositor)
        : BobUIWaylandServer::test_interface(compositor->m_display, 1)
    {
    }
};

class tst_clientextension : public QObject, private CoreCompositor
{
    Q_OBJECT
private:
    BobUIWaylandClient::QWaylandDisplay *display()
    {
        return static_cast<BobUIWaylandClient::QWaylandIntegration *>(
                       QGuiApplicationPrivate::platformIntegration())
                ->display();
    }
private slots:
    void cleanup()
    {
        display()->flushRequests();
        dispatch();
        exec([this] { removeAll<TestGlobal>(); });
        BOBUIRY_COMPARE(display()->globals().size(), 0);
    }
    void createWithoutGlobal();
    void createWithGlobalAutomatic();
    void createWithGlobalManual();
    void globalBecomesAvailable();
    void globalRemoved();
};

void tst_clientextension::createWithoutGlobal()
{
    TestExtension extension;
    QSignalSpy spy(&extension, &QWaylandClientExtension::activeChanged);
    QVERIFY(spy.isValid());
    QVERIFY(!extension.isActive());
    QCOMPARE(spy.size(), 0);
    extension.initialize();
    QVERIFY(!extension.isActive());
    QCOMPARE(spy.size(), 0);
}

void tst_clientextension::createWithGlobalAutomatic()
{
    exec([this] { add<TestGlobal>(); });
    BOBUIRY_COMPARE(display()->globals().size(), 1);
    TestExtension extension;
    QSignalSpy spy(&extension, &QWaylandClientExtension::activeChanged);
    QVERIFY(spy.isValid());
    BOBUIRY_VERIFY(extension.isActive());
    QCOMPARE(spy.size(), 1);
}

void tst_clientextension::createWithGlobalManual()
{
    exec([this] { add<TestGlobal>(); });
    BOBUIRY_COMPARE(display()->globals().size(), 1);
    // Wait for the display to have the global
    TestExtension extension;
    QSignalSpy spy(&extension, &QWaylandClientExtension::activeChanged);
    QVERIFY(spy.isValid());
    extension.initialize();
    QVERIFY(extension.isActive());
    QCOMPARE(spy.size(), 1);
}

void tst_clientextension::globalBecomesAvailable()
{
    TestExtension extension;
    QSignalSpy spy(&extension, &QWaylandClientExtension::activeChanged);
    QVERIFY(spy.isValid());
    exec([this] { add<TestGlobal>(); });
    BOBUIRY_VERIFY(extension.isActive());
    QCOMPARE(spy.size(), 1);
}

void tst_clientextension::globalRemoved()
{
    exec([this] { add<TestGlobal>(); });
    TestExtension extension;
    BOBUIRY_VERIFY(extension.isActive());
    QSignalSpy spy(&extension, &QWaylandClientExtension::activeChanged);
    QVERIFY(spy.isValid());
    QCOMPOSITOR_TRY_COMPARE(get<TestGlobal>()->resourceMap().size(), 1);

    exec([this] { removeAll<TestGlobal>(); });
    BOBUIRY_VERIFY(!extension.isActive());
    QCOMPARE(spy.size(), 1);
}

int main(int argc, char **argv)
{
    BOBUIemporaryDir tmpRuntimeDir;
    qputenv("XDG_RUNTIME_DIR", tmpRuntimeDir.path().toLocal8Bit());
    qputenv("BOBUI_QPA_PLATFORM", "wayland");
    qputenv("BOBUI_WAYLAND_DONT_CHECK_SHELL_INTEGRATION", "1");

    tst_clientextension tc;
    QGuiApplication app(argc, argv);
    BOBUIEST_SET_MAIN_SOURCE_PATH
    return BOBUIest::qExec(&tc, argc, argv);
}

#include "tst_clientextension.moc"
