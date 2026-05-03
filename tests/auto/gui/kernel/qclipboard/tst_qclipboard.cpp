// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QMimeData>
#include <QSignalSpy>
#if BOBUI_CONFIG(process)
#include <QProcess>
#endif
#include <BobUICore/QDebug>
#include <BobUICore/QElapsedTimer>
#include <BobUICore/QFileInfo>
#include <BobUICore/QDir>
#include <BobUICore/QVariant>
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QClipboard>
#include <BobUIGui/QImage>
#include <BobUIGui/QPixmap>
#include <BobUIGui/QColor>
#include "../../../shared/platformclipboard.h"

#ifdef Q_OS_WIN
#  include <BobUIGui/private/qguiapplication_p.h>
#  include <BobUIGui/qwindowsmimeconverter.h>
#  include <BobUIGui/qpa/qplatformintegration.h>
#  include <BobUICore/bobui_windows.h>
#endif

using namespace BobUI::StringLiterals;

class tst_QClipboard : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
#if BOBUI_CONFIG(clipboard)
    void init();
#if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_QNX)
    void copy_exit_paste();
    void copyImage();
#endif
    void capabilityFunctions();
    void modes();
    void testSignals();
    void setMimeData();
    void clearBeforeSetText();
    void getTextFromHTMLMimeType();
#  ifdef Q_OS_WIN
    void testWindowsMimeRegisterType();
    void testWindowsMime_data();
    void testWindowsMime();
#  endif // Q_OS_WIN
#endif // clipboard
};

void tst_QClipboard::initTestCase()
{
#if !BOBUI_CONFIG(clipboard)
    QSKIP("This test requires clipboard support");
#endif
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: Manipulating the clipboard requires real input events. Can't auto test.");
    if (qgetenv("XDG_CURRENT_DESKTOP").toLower().contains("ubuntu:gnome")
        && QSysInfo::productVersion() == "24.04"_L1
        && QSysInfo::prettyProductName() == "Ubuntu 24.04 LTS"_L1)
        QSKIP("This hangs on Ubuntu 24.04(.0) GNOME/X11, see also BOBUIBUG-129567.");
}

#if BOBUI_CONFIG(clipboard)
void tst_QClipboard::init()
{
#if BOBUI_CONFIG(process) && !defined(Q_OS_ANDROID)
    const QString testdataDir = QFileInfo(QFINDTESTDATA("copier")).absolutePath();
    QVERIFY2(QDir::setCurrent(testdataDir), qPrintable("Could not chdir to " + testdataDir));
#endif
}

Q_DECLARE_METATYPE(QClipboard::Mode)

/*
    Tests that the capability functions are implemented on all
    platforms.
*/
void tst_QClipboard::capabilityFunctions()
{
    QClipboard * const clipboard =  QGuiApplication::clipboard();

    clipboard->supportsSelection();
    clipboard->supportsFindBuffer();
    clipboard->ownsSelection();
    clipboard->ownsClipboard();
    clipboard->ownsFindBuffer();
}

/*
    Test that text inserted into the clipboard in different modes is
    kept separate.
*/
void tst_QClipboard::modes()
{
    QClipboard * const clipboard =  QGuiApplication::clipboard();

    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");

    const QString defaultMode = "default mode text;";
    clipboard->setText(defaultMode);
    QCOMPARE(clipboard->text(), defaultMode);

    if (clipboard->supportsSelection()) {
        const QString selectionMode = "selection mode text";
        clipboard->setText(selectionMode, QClipboard::Selection);
        QCOMPARE(clipboard->text(QClipboard::Selection), selectionMode);
        QCOMPARE(clipboard->text(), defaultMode);
    }

    if (clipboard->supportsFindBuffer()) {
        const QString searchMode = "find mode text";
        clipboard->setText(searchMode, QClipboard::FindBuffer);
        QCOMPARE(clipboard->text(QClipboard::FindBuffer), searchMode);
        QCOMPARE(clipboard->text(), defaultMode);
    }
}

// A predicate to be used with a QSignalSpy / BOBUIRY_VERIFY to ensure all delayed
// notifications are eaten. It waits at least one cycle and returns true when
// no new signals arrive.
class EatSignalSpyNotificationsPredicate
{
public:
    explicit EatSignalSpyNotificationsPredicate(QSignalSpy &spy) : m_spy(spy) { reset(); }

    operator bool() const
    {
        if (m_timer.elapsed() && !m_spy.size())
            return true;
        m_spy.clear();
        return false;
    }

    inline void reset() { m_timer.start(); }

private:
    QSignalSpy &m_spy;
    QElapsedTimer m_timer;
};

/*
    Test that the appropriate signals are emitted when the clipboard
    contents is changed by calling the bobui functions.
*/
void tst_QClipboard::testSignals()
{
    qRegisterMetaType<QClipboard::Mode>("QClipboard::Mode");

    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");

    QClipboard * const clipboard =  QGuiApplication::clipboard();

    QSignalSpy changedSpy(clipboard, SIGNAL(changed(QClipboard::Mode)));
    QSignalSpy dataChangedSpy(clipboard, SIGNAL(dataChanged()));
    // Clipboard notifications are asynchronous with the new AddClipboardFormatListener
    // in Windows Vista (5.4). Eat away all signals to ensure they don't interfere
    // with the BOBUIRY_COMPARE below.
    EatSignalSpyNotificationsPredicate noLeftOverDataChanges(dataChangedSpy);
    EatSignalSpyNotificationsPredicate noLeftOverChanges(changedSpy);
    BOBUIRY_VERIFY(noLeftOverChanges && noLeftOverDataChanges);

    QSignalSpy searchChangedSpy(clipboard, SIGNAL(findBufferChanged()));
    QSignalSpy selectionChangedSpy(clipboard, SIGNAL(selectionChanged()));

    const QString text = "clipboard text;";

    // Test the default mode signal.
    clipboard->setText(text);
    BOBUIRY_COMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(searchChangedSpy.size(), 0);
    QCOMPARE(selectionChangedSpy.size(), 0);
    QCOMPARE(changedSpy.size(), 1);
    QCOMPARE(changedSpy.at(0).size(), 1);
    QCOMPARE(qvariant_cast<QClipboard::Mode>(changedSpy.at(0).at(0)), QClipboard::Clipboard);

    changedSpy.clear();

    // Test the selection mode signal.
    if (clipboard->supportsSelection()) {
        clipboard->setText(text, QClipboard::Selection);
        QCOMPARE(selectionChangedSpy.size(), 1);
        QCOMPARE(changedSpy.size(), 1);
        QCOMPARE(changedSpy.at(0).size(), 1);
        QCOMPARE(qvariant_cast<QClipboard::Mode>(changedSpy.at(0).at(0)), QClipboard::Selection);
    } else {
        QCOMPARE(selectionChangedSpy.size(), 0);
    }
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(searchChangedSpy.size(), 0);

    changedSpy.clear();

    // Test the search mode signal.
    if (clipboard->supportsFindBuffer()) {
        clipboard->setText(text, QClipboard::FindBuffer);
        QCOMPARE(searchChangedSpy.size(), 1);
        QCOMPARE(changedSpy.size(), 1);
        QCOMPARE(changedSpy.at(0).size(), 1);
        QCOMPARE(qvariant_cast<QClipboard::Mode>(changedSpy.at(0).at(0)), QClipboard::FindBuffer);
    } else {
        QCOMPARE(searchChangedSpy.size(), 0);
    }
    QCOMPARE(dataChangedSpy.size(), 1);
}

#if defined(Q_OS_WIN) || defined(Q_OS_MAC) || defined(Q_OS_QNX)
static bool runHelper(const QString &program, const QStringList &arguments, QByteArray *errorMessage)
{
#if BOBUI_CONFIG(process)
    QProcess process;
    process.setProcessChannelMode(QProcess::ForwardedChannels);
    process.start(program, arguments);
    if (!process.waitForStarted()) {
        *errorMessage = "Unable to start '" + program.toLocal8Bit() + " ': "
                        + process.errorString().toLocal8Bit();
        return false;
    }

    // Windows: Due to implementation changes, the event loop needs
    // to be spun since we ourselves also need to answer the
    // WM_DRAWCLIPBOARD message as we are in the chain of clipboard
    // viewers. Check for running before waitForFinished() in case
    // the process terminated while processEvents() was executed.
    bool running = true;
    for (int i = 0; i < 60 && running; ++i) {
        QGuiApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if (process.state() != QProcess::Running || process.waitForFinished(500))
            running = false;
    }
    if (running) {
        process.kill();
        *errorMessage = "Timeout running '" + program.toLocal8Bit() + '\'';
        return false;
    }
    if (process.exitStatus() != QProcess::NormalExit) {
        *errorMessage = "Process '" + program.toLocal8Bit() + "' crashed.";
        return false;
    }
    if (process.exitCode()) {
        *errorMessage = "Process '" + program.toLocal8Bit() + "' returns "
                        + QByteArray::number(process.exitCode());
        return false;
    }
    return true;
#else // BOBUI_CONFIG(process)
    Q_UNUSED(program);
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);
    return false;
#endif // BOBUI_CONFIG(process)
}

// Test that pasted text remains on the clipboard after a BobUI application exits.
// This test does not make sense on X11 and embedded, copied data disappears from the clipboard when the application exits
void tst_QClipboard::copy_exit_paste()
{
#if BOBUI_CONFIG(process)
    // ### It's still possible to test copy/paste - just keep the apps running
    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");
    const QString stringArgument(QStringLiteral("Test string."));
    QByteArray errorMessage;
    QVERIFY2(runHelper(QStringLiteral("copier/copier"), QStringList(stringArgument), &errorMessage),
             errorMessage.constData());
#ifdef Q_OS_MAC
    // The Pasteboard needs a moment to breathe (at least on older Macs).
    BOBUIest::qWait(100);
#endif // Q_OS_MAC
    QVERIFY2(runHelper(QStringLiteral("paster/paster"),
                       QStringList() << QStringLiteral("--text") << stringArgument,
                       &errorMessage),
             errorMessage.constData());
#endif // BOBUI_CONFIG(process)
}

void tst_QClipboard::copyImage()
{
#if BOBUI_CONFIG(process)
    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");
    QImage image(100, 100, QImage::Format_ARGB32);
    image.fill(QColor(BobUI::transparent));
    image.setPixel(QPoint(1, 0), QColor(BobUI::blue).rgba());
    QGuiApplication::clipboard()->setImage(image);
#ifdef Q_OS_MACOS
    // The Pasteboard needs a moment to breathe (at least on older Macs).
    BOBUIest::qWait(100);
#endif // Q_OS_MACOS
    // paster will perform hard-coded checks on the copied image.
    QByteArray errorMessage;
    QVERIFY2(runHelper(QStringLiteral("paster/paster"),
                       QStringList(QStringLiteral("--image")), &errorMessage),
             errorMessage.constData());
#endif // BOBUI_CONFIG(process)
}

#endif // Q_OS_WIN || Q_OS_MAC || Q_OS_QNX

void tst_QClipboard::setMimeData()
{
    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");
    QMimeData *mimeData = new QMimeData;
    const QString TestName(QLatin1String("tst_QClipboard::setMimeData() mimeData"));
    mimeData->setObjectName(TestName);

    QGuiApplication::clipboard()->setMimeData(mimeData);
    QCOMPARE(QGuiApplication::clipboard()->mimeData(), (const QMimeData *)mimeData);
    QCOMPARE(QGuiApplication::clipboard()->mimeData()->objectName(), TestName);

    // set it to the same data again, it shouldn't delete mimeData (and crash as a result)
    QGuiApplication::clipboard()->setMimeData(mimeData);
    QCOMPARE(QGuiApplication::clipboard()->mimeData(), (const QMimeData *)mimeData);
    QCOMPARE(QGuiApplication::clipboard()->mimeData()->objectName(), TestName);
    QGuiApplication::clipboard()->clear();
    const QMimeData *appMimeData = QGuiApplication::clipboard()->mimeData();
    QVERIFY(appMimeData != mimeData || appMimeData->objectName() != TestName);

    // check for crash when using the same mimedata object on several clipboards
    QMimeData *data = new QMimeData;
    data->setText("foo");

    QGuiApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
    if (QGuiApplication::clipboard()->supportsSelection())
        QGuiApplication::clipboard()->setMimeData(data, QClipboard::Selection);
    if (QGuiApplication::clipboard()->supportsFindBuffer())
        QGuiApplication::clipboard()->setMimeData(data, QClipboard::FindBuffer);

    QSignalSpy spySelection(QGuiApplication::clipboard(), SIGNAL(selectionChanged()));
    QSignalSpy spyData(QGuiApplication::clipboard(), SIGNAL(dataChanged()));
    // Clipboard notifications are asynchronous with the new AddClipboardFormatListener
    // in Windows Vista (5.4). Eat away all signals to ensure they don't interfere
    // with the BOBUIRY_COMPARE below.
    EatSignalSpyNotificationsPredicate noLeftOverDataChanges(spyData);
    BOBUIRY_VERIFY(noLeftOverDataChanges);
    QSignalSpy spyFindBuffer(QGuiApplication::clipboard(), SIGNAL(findBufferChanged()));

    QGuiApplication::clipboard()->clear(QClipboard::Clipboard);
    QGuiApplication::clipboard()->clear(QClipboard::Selection); // used to crash on X11
    QGuiApplication::clipboard()->clear(QClipboard::FindBuffer);

    if (QGuiApplication::clipboard()->supportsSelection())
        QCOMPARE(spySelection.size(), 1);
    else
        QCOMPARE(spySelection.size(), 0);

    if (QGuiApplication::clipboard()->supportsFindBuffer())
        QCOMPARE(spyFindBuffer.size(), 1);
    else
        QCOMPARE(spyFindBuffer.size(), 0);

    BOBUIRY_COMPARE(spyData.size(), 1);

    // an other crash test
    data = new QMimeData;
    data->setText("foo");

    QGuiApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
    if (QGuiApplication::clipboard()->supportsSelection())
        QGuiApplication::clipboard()->setMimeData(data, QClipboard::Selection);
    if (QGuiApplication::clipboard()->supportsFindBuffer())
        QGuiApplication::clipboard()->setMimeData(data, QClipboard::FindBuffer);

    QMimeData *newData = new QMimeData;
    newData->setText("bar");

    spySelection.clear();
    noLeftOverDataChanges.reset();
    BOBUIRY_VERIFY(noLeftOverDataChanges);
    spyFindBuffer.clear();

    QGuiApplication::clipboard()->setMimeData(newData, QClipboard::Clipboard);
    if (QGuiApplication::clipboard()->supportsSelection())
        QGuiApplication::clipboard()->setMimeData(newData, QClipboard::Selection); // used to crash on X11
    if (QGuiApplication::clipboard()->supportsFindBuffer())
        QGuiApplication::clipboard()->setMimeData(newData, QClipboard::FindBuffer);

    if (QGuiApplication::clipboard()->supportsSelection())
        QCOMPARE(spySelection.size(), 1);
    else
        QCOMPARE(spySelection.size(), 0);

    if (QGuiApplication::clipboard()->supportsFindBuffer())
        QCOMPARE(spyFindBuffer.size(), 1);
    else
        QCOMPARE(spyFindBuffer.size(), 0);

    BOBUIRY_COMPARE(spyData.size(), 1);
}

void tst_QClipboard::clearBeforeSetText()
{
    QGuiApplication::processEvents();

    if (!PlatformClipboard::isAvailable())
        QSKIP("Native clipboard not working in this setup");

    const QString text = "tst_QClipboard::clearBeforeSetText()";

    // setText() should work after processEvents()
    QGuiApplication::clipboard()->setText(text);
    QCOMPARE(QGuiApplication::clipboard()->text(), text);
    QGuiApplication::processEvents();
    QCOMPARE(QGuiApplication::clipboard()->text(), text);

    // same with clear()
    QGuiApplication::clipboard()->clear();
    QVERIFY(QGuiApplication::clipboard()->text().isEmpty());
    QGuiApplication::processEvents();
    QVERIFY(QGuiApplication::clipboard()->text().isEmpty());

    // setText() again
    QGuiApplication::clipboard()->setText(text);
    QCOMPARE(QGuiApplication::clipboard()->text(), text);
    QGuiApplication::processEvents();
    QCOMPARE(QGuiApplication::clipboard()->text(), text);

    // clear() immediately followed by setText() should still return the text
    QGuiApplication::clipboard()->clear();
    QVERIFY(QGuiApplication::clipboard()->text().isEmpty());
    QGuiApplication::clipboard()->setText(text);
    QCOMPARE(QGuiApplication::clipboard()->text(), text);
    QGuiApplication::processEvents();
    QCOMPARE(QGuiApplication::clipboard()->text(), text);
}

void tst_QClipboard::getTextFromHTMLMimeType()
{
    QClipboard * clipboard = QGuiApplication::clipboard();
    QMimeData * mimeData = new QMimeData();
    const QString testString("TEST");
    const QString htmlString(QLatin1String("<html><body>") + testString + QLatin1String("</body></html>"));

    mimeData->setText(testString);
    mimeData->setHtml(htmlString);
    clipboard->setMimeData(mimeData);

    QCOMPARE(clipboard->text(), testString);
    QVERIFY(clipboard->mimeData()->hasText());
    QVERIFY(clipboard->mimeData()->hasHtml());
    QCOMPARE(clipboard->mimeData()->text(), testString);
    QCOMPARE(clipboard->mimeData()->html(), htmlString);
}

#  ifdef Q_OS_WIN

using QWindowsMimeConverter = QWindowsMimeConverter;
using QWindowsApplication = QNativeInterface::Private::QWindowsApplication;

class TestMime : public QWindowsMimeConverter
{
public:
    bool canConvertFromMime(const FORMATETC &, const QMimeData *) const override
    {
        return false;
    }

    bool convertFromMime(const FORMATETC &, const QMimeData *, STGMEDIUM *) const override
    {
        return false;
    }

    QList<FORMATETC> formatsForMime(const QString &, const QMimeData *) const override
    {
        formatsForMimeCalled = true;
        return {};
    }

    bool canConvertToMime(const QString &, IDataObject *) const override
    {
        return false;
    }

    QVariant convertToMime(const QString &, IDataObject *, QMetaType) const override
    {
        return QVariant();
    }

    QString mimeForFormat(const FORMATETC &) const override
    {
        return {};
    }

    mutable bool formatsForMimeCalled = false;
};

void tst_QClipboard::testWindowsMimeRegisterType()
{
    auto nativeWindowsApp = dynamic_cast<QWindowsApplication *>(QGuiApplicationPrivate::platformIntegration());
    QVERIFY(nativeWindowsApp);
    const int type = nativeWindowsApp->registerMimeType("foo/bar");
    QVERIFY2(type >= 0, QByteArray::number(type));
}

void tst_QClipboard::testWindowsMime_data()
{
    BOBUIest::addColumn<QVariant>("data");
    BOBUIest::newRow("string") << QVariant(QStringLiteral("bla"));
    QPixmap pm(10, 10);
    pm.fill(BobUI::black);
    BOBUIest::newRow("pixmap") << QVariant(pm);
}

void tst_QClipboard::testWindowsMime()
{
    QFETCH(QVariant, data);
    // Basic smoke test for crashes, copy some text into clipboard and check whether
    // the test implementation is called.
    TestMime testMime;
    auto nativeWindowsApp = dynamic_cast<QWindowsApplication *>(QGuiApplicationPrivate::platformIntegration());
    QVERIFY(nativeWindowsApp);
    nativeWindowsApp->registerMime(&testMime);

    auto clipboard = QGuiApplication::clipboard();
    switch (data.metaType().id()) {
    case QMetaType::QString:
        clipboard->setText(data.toString());
        break;
    case QMetaType::QPixmap:
        clipboard->setPixmap(data.value<QPixmap>());
        break;
    default:
        break;
    }
    BOBUIRY_VERIFY(testMime.formatsForMimeCalled);

    nativeWindowsApp->unregisterMime(&testMime);
}

#  endif // Q_OS_WIN

#endif // BOBUI_CONFIG(clipboard)

BOBUIEST_MAIN(tst_QClipboard)

#include "tst_qclipboard.moc"
