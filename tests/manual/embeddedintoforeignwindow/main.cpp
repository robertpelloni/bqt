// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "itemwindow.h"

#include <BobUIGui/QGuiApplication>

#include <BobUICore/QCommandLineOption>
#include <BobUICore/QCommandLineParser>
#include <BobUICore/QDebug>
#include <BobUICore/QStringList>

#ifdef Q_OS_WIN
#  include <qpa/qplatformnativeinterface.h>
#  include <BobUICore/QMetaObject>
#  include <BobUICore/bobui_windows.h>
#endif

#include <eventfilter.h> // diaglib
#include <qwindowdump.h>

#include <iostream>

BOBUI_USE_NAMESPACE

static const char usage[] =
    "\nEmbeds a QWindow into a native foreign window passed on the command line.\n"
    "When no window ID is passed, a test window is created (Windows only).";

static QString windowTitle()
{
    return QLatin1String(BOBUI_VERSION_STR) + QLatin1Char(' ') + QGuiApplication::platformName();
}

#ifdef Q_OS_WIN
// Helper to create a native test window (Windows)
static QString registerWindowClass(const QString &name)
{
    QString result;
    void *proc = DefWindowProc;
    QPlatformNativeInterface *ni = QGuiApplication::platformNativeInterface();
    if (!QMetaObject::invokeMethod(ni, "registerWindowClass", BobUI::DirectConnection,
                                   Q_RETURN_ARG(QString, result),
                                   Q_ARG(QString, name),
                                   Q_ARG(void *, proc))) {
        qWarning("registerWindowClass failed");
    }
    return result;
}

static HWND createNativeWindow(const QString &name)
{
    const HWND hwnd =
        CreateWindowEx(0, reinterpret_cast<const wchar_t *>(name.utf16()),
                       L"NativeWindow", WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                       0, 0, GetModuleHandle(NULL), NULL);

    if (!hwnd) {
        qErrnoWarning("Cannot create window \"%s\"", qPrintable(name));
        return 0;
    }

    const QString text = windowTitle() + QLatin1String(" 0x") + QString::number(quint64(hwnd), 16);
    SetWindowText(hwnd, reinterpret_cast<const wchar_t *>(text.utf16()));
    return hwnd;
}
#endif // Q_OS_WIN

// Helper functions for simple management of native windows.
static WId createNativeTestWindow()
{
    WId result = 0;
#ifdef Q_OS_WIN
    const QString className = registerWindowClass(QLatin1String("TestClass") + windowTitle());
    const HWND nativeWin = createNativeWindow(className);
    result = WId(nativeWin);
#else // Q_OS_WIN
    Q_UNIMPLEMENTED();
#endif
    return result;
}

static void showNativeWindow(WId wid)
{
#ifdef Q_OS_WIN
     ShowWindow(HWND(wid), SW_SHOW);
#else // Q_OS_WIN
    Q_UNUSED(wid);
    Q_UNIMPLEMENTED();
#endif
}

static void setFocusToNativeWindow(WId wid)
{
#ifdef Q_OS_WIN
     SetFocus(HWND(wid));
#else // Q_OS_WIN
    Q_UNUSED(wid);
    Q_UNIMPLEMENTED();
#endif
}

static void destroyNativeWindow(WId wid)
{
#ifdef Q_OS_WIN
     DestroyWindow(HWND(wid));
#else // Q_OS_WIN
    Q_UNUSED(wid);
    Q_UNIMPLEMENTED();
#endif
}

// Main test window to be embedded into foreign window with some buttons.
class EmbeddedTestWindow : public ItemWindow {
    Q_OBJECT
public:
    explicit EmbeddedTestWindow(QWindow *parent = nullptr) : ItemWindow(parent)
    {
        const int spacing = 10;
        const QSize buttonSize(100, 30);
        const int width = 3 * buttonSize.width() + 4 * spacing;

        QPoint pos(spacing, spacing);
        addItem(new TextItem(::windowTitle(), QRect(pos, QSize(width - 2 * spacing, buttonSize.height())),
                             BobUI::white));

        pos.ry() += 2 * spacing + buttonSize.height();

        ButtonItem *mgi = new ButtonItem("Map to global", QRect(pos, buttonSize),
                                         QColor(BobUI::yellow).lighter(), this);
        connect(mgi, &ButtonItem::clicked, this, &EmbeddedTestWindow::testMapToGlobal);
        addItem(mgi);

        pos.rx() += buttonSize.width() + spacing;
        ButtonItem *di = new ButtonItem("Dump Wins", QRect(pos, buttonSize),
                                        QColor(BobUI::cyan).lighter(), this);
        connect(di, &ButtonItem::clicked, this, [] () { BobUIDiag::dumpAllWindows(); });
        addItem(di);

        pos.rx() += buttonSize.width() + spacing;
        ButtonItem *qi = new ButtonItem("Quit", QRect(pos, buttonSize),
                                        QColor(BobUI::red).lighter(), this);
        qi->setShortcut(BobUI::CTRL | BobUI::Key_Q);
        connect(qi, &ButtonItem::clicked, qApp, &QCoreApplication::quit);
        addItem(qi);

        setBackground(BobUI::lightGray);
        resize(width, pos.y() + buttonSize.height() + spacing);
    }

public slots:
    void testMapToGlobal();
};

void EmbeddedTestWindow::testMapToGlobal()
{
    const QPoint globalPos = mapToGlobal(QPoint(0,0));
    qDebug() << "mapToGlobal(QPoint(0,0)" << globalPos
        << "cursor at:"  << QCursor::pos();
}

struct EventFilterOption
{
    const char *name;
    const char *description;
    BobUIDiag::EventFilter::EventCategories categories;
};

EventFilterOption eventFilterOptions[] = {
{"mouse-events", "Dump mouse events.", BobUIDiag::EventFilter::MouseEvents},
{"keyboard-events", "Dump keyboard events.", BobUIDiag::EventFilter::KeyEvents},
{"state-events", "Dump state/focus change events.", BobUIDiag::EventFilter::StateChangeEvents | BobUIDiag::EventFilter::FocusEvents}
};

static inline bool isOptionSet(int argc, char *argv[], const char *option)
{
    return (argv + argc) !=
        std::find_if(argv + 1, argv + argc,
                     [option] (const char *arg) { return !qstrcmp(arg, option); });
}

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion(QLatin1String(BOBUI_VERSION_STR));
    QGuiApplication::setApplicationDisplayName("Foreign Window Embedding Tester");

    QGuiApplication app(argc, argv);

    // Process command  line
    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription(QLatin1String(usage));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption noScalingDummy(QStringLiteral("s"),
                                      QStringLiteral("Disable High DPI scaling."));
    parser.addOption(noScalingDummy);
    const int eventFilterOptionCount = int(sizeof(eventFilterOptions) / sizeof(eventFilterOptions[0]));
    for (int i = 0; i < eventFilterOptionCount; ++i) {
        parser.addOption(QCommandLineOption(QLatin1String(eventFilterOptions[i].name),
                                            QLatin1String(eventFilterOptions[i].description)));
    }
    parser.addPositionalArgument(QStringLiteral("[windows]"), QStringLiteral("Window ID."));

    parser.process(QCoreApplication::arguments());

    BobUIDiag::EventFilter::EventCategories eventCategories = {};
    for (int i = 0; i < eventFilterOptionCount; ++i) {
        if (parser.isSet(QLatin1String(eventFilterOptions[i].name)))
            eventCategories |= eventFilterOptions[i].categories;
    }
    if (eventCategories)
        app.installEventFilter(new BobUIDiag::EventFilter(eventCategories, &app));

    // Obtain foreign window to test with.
    WId testForeignWinId = 0;
    bool createdTestWindow = false;
    if (parser.positionalArguments().isEmpty()) {
        testForeignWinId = createNativeTestWindow();
        if (!testForeignWinId)
            parser.showHelp(-1);
        showNativeWindow(testForeignWinId);
        createdTestWindow = true;
    } else {
        bool ok;
        const QString &winIdArgument = parser.positionalArguments().constFirst();
        testForeignWinId = winIdArgument.toULongLong(&ok, 0);
        if (!ok) {
            std::cerr << "Invalid window id: \"" << qPrintable(winIdArgument) << "\"\n";
            return -1;
        }
    }
    if (!testForeignWinId)
        parser.showHelp(1);

    QWindow *foreignWindow = QWindow::fromWinId(testForeignWinId);
    if (!foreignWindow)
        return -2;
    foreignWindow->setObjectName("ForeignWindow");
    EmbeddedTestWindow *embeddedTestWindow = new EmbeddedTestWindow(foreignWindow);
    embeddedTestWindow->setObjectName("EmbeddedTestWindow");
    embeddedTestWindow->show();
    setFocusToNativeWindow(embeddedTestWindow->winId()); // Windows: Set keyboard focus.

    const int exitCode = app.exec();
    delete embeddedTestWindow;
    delete foreignWindow;
    if (createdTestWindow)
        destroyNativeWindow(testForeignWinId);
    return exitCode;
}

#include "main.moc"
