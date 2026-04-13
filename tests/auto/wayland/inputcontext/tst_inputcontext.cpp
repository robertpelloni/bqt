// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mockcompositor.h"
#include "textinput.h"
#include "bobuitextinput.h"

#include <BobUICore/QString>
#include <BobUICore/QByteArray>

#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIGui/qpa/qplatforminputcontext.h>
#include <BobUIGui/qpa/qplatformintegration.h>
#include <BobUIGui/qpa/qplatforminputcontextfactory_p.h>

#include <BobUITest/BobUITest>

using namespace MockCompositor;

class tst_inputcontext : public QObject, private DefaultCompositor
{
    Q_OBJECT
private slots:
    void initTestCase();
    void selectingInputContext_data();
    void selectingInputContext();
    void selectingTextInputProtocol_data();
    void selectingTextInputProtocol();
    void inputContextReconfigurationWhenTogglingTextInputExtension();

private:
    QByteArray inputContextName() const;

    template<typename arg_type>
    void ensurePresentOnCompositor()
    {
        exec([&] {
            QList<arg_type *> extensions = getAll<arg_type>();
            if (extensions.size() > 1)
                QFAIL("Requested type is a singleton, hence there should not be more then one object returned");
            if (extensions.size() == 0)
                add<arg_type>();
        });
    }

    template<typename arg_type>
    void ensureNotPresentOnCompositor()
    {
        exec([&] {
            QList<arg_type *> extensions = getAll<arg_type>();
            if (extensions.size() > 1)
                QFAIL("Requested type is a singleton, hence there should not be more then one object returned");
            if (extensions.size() == 1)
                remove(extensions.first());
        });
    }

    QByteArray mComposeModule = QByteArray("QComposeInputContext"); // default input context
    QByteArray mIbusModule    = QByteArray("QIBusPlatformInputContext");
    QByteArray mTextInputModule = QByteArray("BobUIWaylandClient::QWaylandInputContext");
    QByteArray mBobUITextInputModule = QByteArray("BobUIWaylandClient::QWaylandInputMethodContext");
};

void tst_inputcontext::initTestCase()
{
    // Verify that plugins are present and valid
    QPlatformInputContext *context = QPlatformInputContextFactory::create(QStringLiteral("compose"));
    QVERIFY(context && context->isValid());

    context = QPlatformInputContextFactory::create(QStringLiteral("ibus"));
    // The ibus plugin depends on properly configured system services, if plugin is not valid
    // verify that wayland qpa plugin properly fallbacks to default input context.
    if (!context || !context->isValid())
        mIbusModule = mComposeModule;
}

QByteArray tst_inputcontext::inputContextName() const
{
    QPlatformIntegration *platformIntegration = QGuiApplicationPrivate::platformIntegration();
    if (platformIntegration->inputContext())
        return platformIntegration->inputContext()->metaObject()->className();

    return QByteArray("");
}

void tst_inputcontext::selectingInputContext_data()
{
    BOBUIest::addColumn<QByteArray>("requestedModule");
    BOBUIest::addColumn<QByteArray>("expectedModule");

    // Test compositor without Text Input extension
    BOBUIest::newRow("ibus")    << QByteArray("ibus")    << mIbusModule;
    BOBUIest::newRow("compose") << QByteArray("compose") << mComposeModule;
    BOBUIest::newRow("empty")   << QByteArray("")        << mComposeModule;
    BOBUIest::newRow("null")    << QByteArray()          << mComposeModule;
    BOBUIest::newRow("fake")    << QByteArray("fake")    << mComposeModule;

    // Test compositor with Text Input extension
    BOBUIest::newRow("ibus:text-input")    << QByteArray("ibus")    << mIbusModule;
    BOBUIest::newRow("compose:text-input") << QByteArray("compose") << mComposeModule;
    BOBUIest::newRow("empty:text-input")   << QByteArray("")        << mTextInputModule;
    BOBUIest::newRow("null:text-input")    << QByteArray()          << mTextInputModule;
    BOBUIest::newRow("wayland:text-input") << QByteArray("wayland") << mTextInputModule;
    BOBUIest::newRow("fake:text-input")    << QByteArray("fake")    << mComposeModule;
}

void tst_inputcontext::selectingInputContext()
{
    QFETCH(QByteArray, requestedModule);
    QFETCH(QByteArray, expectedModule);

    if (requestedModule.isNull())
        qunsetenv("BOBUI_IM_MODULE");
    else
        qputenv("BOBUI_IM_MODULE", requestedModule);

    const bool withTextInputAtCompositorSide = QByteArray(BOBUIest::currentDataTag()).endsWith(":text-input");

    if (withTextInputAtCompositorSide)
        ensurePresentOnCompositor<TextInputManager>();
    else
        ensureNotPresentOnCompositor<TextInputManager>();

    int argc = 0;
    QGuiApplication app(argc, nullptr); // loads the platform plugin

    QCOMPARE(inputContextName(), expectedModule);
}

void tst_inputcontext::selectingTextInputProtocol_data()
{
    BOBUIest::addColumn<bool>("requestBobUITextInput");
    BOBUIest::addColumn<bool>("requestTextInput");
    BOBUIest::addColumn<QByteArray>("clientProtocol");
    BOBUIest::addColumn<QByteArray>("expectedModule");

    BOBUIest::newRow("1-1") << true << true << QByteArray() << mBobUITextInputModule;
    BOBUIest::newRow("1-2") << true << false << QByteArray() << mBobUITextInputModule;
    BOBUIest::newRow("1-3") << false << true << QByteArray() << mTextInputModule;
    BOBUIest::newRow("1-4") << false << false << QByteArray() << mComposeModule;

    BOBUIest::newRow("2-1") << true << true << QByteArray("zwp_text_input_v2") << mTextInputModule;
    BOBUIest::newRow("2-2") << true << false << QByteArray("zwp_text_input_v2") << mComposeModule;
    BOBUIest::newRow("2-3") << false << true << QByteArray("zwp_text_input_v2") << mTextInputModule;
    BOBUIest::newRow("2-4") << false << false << QByteArray("zwp_text_input_v2") << mComposeModule;

    BOBUIest::newRow("3-1") << true << true << QByteArray("bobui_text_input_method_v1") << mBobUITextInputModule;
    BOBUIest::newRow("3-2") << true << false << QByteArray("bobui_text_input_method_v1") << mBobUITextInputModule;
    BOBUIest::newRow("3-3") << false << true << QByteArray("bobui_text_input_method_v1") << mComposeModule;
    BOBUIest::newRow("3-4") << false << false << QByteArray("bobui_text_input_method_v1") << mComposeModule;

    BOBUIest::newRow("4-1") << true << true << QByteArray("bobui_text_input_method_v1;zwp_text_input_v2") << mBobUITextInputModule;
    BOBUIest::newRow("4-2") << true << false << QByteArray("bobui_text_input_method_v1;zwp_text_input_v2") << mBobUITextInputModule;
    BOBUIest::newRow("4-3") << false << true << QByteArray("bobui_text_input_method_v1;zwp_text_input_v2") << mTextInputModule;
    BOBUIest::newRow("4-4") << false << false << QByteArray("bobui_text_input_method_v1;zwp_text_input_v2") << mComposeModule;

    BOBUIest::newRow("5-1") << true << true << QByteArray("zwp_text_input_v2;bobui_text_input_method_v1") << mTextInputModule;
    BOBUIest::newRow("5-2") << true << false << QByteArray("zwp_text_input_v2;bobui_text_input_method_v1") << mBobUITextInputModule;
    BOBUIest::newRow("5-3") << false << true << QByteArray("zwp_text_input_v2;bobui_text_input_method_v1") << mTextInputModule;
    BOBUIest::newRow("5-4") << false << false << QByteArray("zwp_text_input_v2;bobui_text_input_method_v1") << mComposeModule;
}

void tst_inputcontext::selectingTextInputProtocol()
{
    QFETCH(bool, requestBobUITextInput);
    QFETCH(bool, requestTextInput);
    QFETCH(QByteArray, clientProtocol);
    QFETCH(QByteArray, expectedModule);

    exec([] {
        qputenv("BOBUI_IM_MODULE", "bobuivirtualkeyboard");
    });

    qunsetenv("BOBUI_IM_MODULE");

    if (clientProtocol.isNull())
        qunsetenv("BOBUI_WAYLAND_TEXT_INPUT_PROTOCOL");
    else
        qputenv("BOBUI_WAYLAND_TEXT_INPUT_PROTOCOL", clientProtocol);

    if (requestTextInput)
        ensurePresentOnCompositor<TextInputManager>();
    else
        ensureNotPresentOnCompositor<TextInputManager>();

    if (requestBobUITextInput)
        ensurePresentOnCompositor<BobUITextInputManager>();
    else
        ensureNotPresentOnCompositor<BobUITextInputManager>();

    int argc = 0;
    QGuiApplication app(argc, nullptr); // loads the platform plugin

    QCOMPARE(inputContextName(), expectedModule);
}

void tst_inputcontext::inputContextReconfigurationWhenTogglingTextInputExtension()
{
    qunsetenv("BOBUI_IM_MODULE");

    ensurePresentOnCompositor<TextInputManager>();
    int argc = 0;
    QGuiApplication app(argc, nullptr); // loads the platform plugin
    QCOMPARE(inputContextName(), mTextInputModule);

    // remove text input extension after the platform plugin has been loaded
    ensureNotPresentOnCompositor<TextInputManager>();
    // BOBUIRY_* because we need to spin the event loop for wayland QPA plugin
    // to handle registry_global_remove()
    BOBUIRY_COMPARE(inputContextName(), mComposeModule);

    // add text input extension after the platform plugin has been loaded
    ensurePresentOnCompositor<TextInputManager>();
    // BOBUIRY_* because we need to spin the event loop for wayland QPA plugin
    // to handle registry_global()
    BOBUIRY_COMPARE(inputContextName(), mTextInputModule);
}

int main(int argc, char *argv[])
{
    BOBUIemporaryDir tmpRuntimeDir;
    qputenv("XDG_RUNTIME_DIR", tmpRuntimeDir.path().toLocal8Bit());
    qputenv("BOBUI_QPA_PLATFORM", "wayland");

    tst_inputcontext tc;
    BOBUIEST_SET_MAIN_SOURCE_PATH
    return BOBUIest::qExec(&tc, argc, argv);
}

#include "tst_inputcontext.moc"
