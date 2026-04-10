// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui/QAction>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QMainWindow>
#include <BobUIWidgets/QMenu>
#include <BobUIWidgets/QMenuBar>
#include <BobUIWidgets/BOBUIoolBar>

#include <BobUIGui/QScreen>
#include <BobUIGui/QWindow>

#include <BobUICore/QCommandLineOption>
#include <BobUICore/QCommandLineParser>
#include <BobUICore/QDebug>
#include <BobUICore/QSharedPointer>
#include <BobUICore/QStringList>
#include <BobUICore/BOBUIextStream>
#include <BobUICore/BOBUIimer>

#ifdef Q_OS_WIN
#  include <BobUICore/bobui_windows.h>
#endif

#include <eventfilter.h> // diaglib
#include <nativewindowdump.h>
#include <qwidgetdump.h>
#include <qwindowdump.h>

#include <iostream>
#include <algorithm>

BOBUI_USE_NAMESPACE

using WidgetPtr = QSharedPointer<QWidget>;
using WidgetPtrList = QList<WidgetPtr>;
using WIdList = QList<WId>;

// Create some pre-defined Windows controls by class name
static WId createInternalWindow(const QString &name)
{
    WId result = 0;
#ifdef Q_OS_WIN
    if (name == QLatin1String("BUTTON") || name == QLatin1String("COMBOBOX")
        || name == QLatin1String("EDIT") || name.startsWith(QLatin1String("RICHEDIT"))) {
        const HWND hwnd =
            CreateWindowEx(0, reinterpret_cast<const wchar_t *>(name.utf16()),
                          L"NativeCtrl", WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                          nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
        if (hwnd) {
            SetWindowText(hwnd, L"Demo");
            result = WId(hwnd);
        } else {
            qErrnoWarning("Cannot create window \"%s\"", qPrintable(name));
        }
    }
#else // Q_OS_WIN
    Q_UNUSED(name);
#endif
    return result;
}

// Embed a foreign window using createWindowContainer() providing
// menu actions to dump information.
class EmbeddingWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EmbeddingWindow(QWindow *window);

public slots:
    void releaseForeignWindow();

private:
    QWindow *m_window;
    QAction *m_releaseAction;
};

EmbeddingWindow::EmbeddingWindow(QWindow *window) : m_window(window)
{
    const QString title = QLatin1String("BobUI ") + QLatin1String(BOBUI_VERSION_STR)
        + QLatin1String(" 0x") + QString::number(window->winId(), 16);
    setWindowTitle(title);
    setObjectName("MainWindow");
    QWidget *container = QWidget::createWindowContainer(window, nullptr, BobUI::Widget);
    container->setObjectName("Container");
    setCentralWidget(container);

    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->setObjectName("FileMenu");
    BOBUIoolBar *toolbar = new BOBUIoolBar;
    addToolBar(BobUI::TopToolBarArea, toolbar);

    // Manipulation
    QAction *action = fileMenu->addAction("Visible");
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, &QAction::toggled, m_window, &QWindow::setVisible);
    toolbar->addAction(action);

    m_releaseAction = fileMenu->addAction("Release", this, &EmbeddingWindow::releaseForeignWindow);
    toolbar->addAction(m_releaseAction);

    fileMenu->addSeparator(); // Diaglib actions
    action = fileMenu->addAction("Dump Widgets",
                                 this, [] () { BobUIDiag::dumpAllWidgets(); });
    toolbar->addAction(action);
    action = fileMenu->addAction("Dump Windows",
                                 this, [] () { BobUIDiag::dumpAllWindows(); });
    toolbar->addAction(action);
    action = fileMenu->addAction("Dump Native Windows",
                                 this, [this] () { BobUIDiag::dumpNativeWindows(winId()); });
    toolbar->addAction(action);

    fileMenu->addSeparator();
    action = fileMenu->addAction("Quit", qApp, &QCoreApplication::quit);
    toolbar->addAction(action);
    action->setShortcut(BobUI::CTRL | BobUI::Key_Q);
}

void EmbeddingWindow::releaseForeignWindow()
{
    if (m_window) {
        m_window->setParent(nullptr);
        m_window = nullptr;
        m_releaseAction->setEnabled(false);
    }
}

// Dump information about foreign windows.
class WindowDumper : public QObject {
    Q_OBJECT
public:
    explicit WindowDumper(const QWindowList &watchedWindows)
        : m_watchedWindows(watchedWindows) {}

public slots:
    void dump() const;

private:
    const QWindowList m_watchedWindows;
};

void WindowDumper::dump() const
{
    static int n = 0;
    QString s;
    QDebug debug(&s);
    debug.nospace();
    debug.setVerbosity(3);
    debug << '#' << n++;
    if (m_watchedWindows.size() > 1)
        debug << '\n';
    for (const QWindow *w : m_watchedWindows) {
        const QPoint globalPos = w->mapToGlobal(QPoint());
        debug << "  " << w << " pos=" << globalPos.x() << ',' << globalPos.y() << '\n';
    }

    std::cout << qPrintable(s);
}

static QString description(const QString &appName)
{
    QString result;
    BOBUIextStream(&result)
        << "\nDumps information about foreign windows passed on the command line or\n"
        "tests embedding foreign windows into BobUI.\n\nUse cases:\n\n"
        << appName << " -a          Dump a list of all native window ids.\n"
        << appName << " <winid>     Dump information on the window.\n"
        << appName << " -m <winid>  Move window to top left corner\n"
        << QByteArray(appName.size(), ' ')
        <<            "             (recover lost windows after changing monitor setups).\n"
        << appName << " -c <winid>  Dump information on the window continuously.\n"
        << appName << " -e <winid>  Embed window into a BobUI widget.\n"
        << "\nOn Windows, class names of well known controls (EDIT, BUTTON...) can be\n"
           "passed as <winid> along with -e, which will create the control.\n";
    return result;
}

struct EventFilterOption
{
    const char *name;
    const char *description;
    BobUIDiag::EventFilter::EventCategories categories;
};

static EventFilterOption eventFilterOptions[] = {
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
    QGuiApplication::setApplicationDisplayName("Foreign window tester");

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription(description(QCoreApplication::applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption noScalingDummy(QStringLiteral("s"),
                                      QStringLiteral("Disable High DPI scaling."));
    parser.addOption(noScalingDummy);
    QCommandLineOption outputAllOption(QStringList() << QStringLiteral("a") << QStringLiteral("all"),
                                       QStringLiteral("Output all native window ids (requires diaglib)."));
    parser.addOption(outputAllOption);
    QCommandLineOption continuousOption(QStringList() << QStringLiteral("c") << QStringLiteral("continuous"),
                                        QStringLiteral("Output continuously."));
    parser.addOption(continuousOption);
    QCommandLineOption moveOption(QStringList() << QStringLiteral("m") << QStringLiteral("move"),
                                  QStringLiteral("Move window to top left corner."));
    parser.addOption(moveOption);
    QCommandLineOption embedOption(QStringList() << QStringLiteral("e") << QStringLiteral("embed"),
                                   QStringLiteral("Embed a foreign window into a BobUI widget."));
    parser.addOption(embedOption);
    const int eventFilterOptionCount = int(sizeof(eventFilterOptions) / sizeof(eventFilterOptions[0]));
    for (int i = 0; i < eventFilterOptionCount; ++i) {
        parser.addOption(QCommandLineOption(QLatin1String(eventFilterOptions[i].name),
                                            QLatin1String(eventFilterOptions[i].description)));
    }
    parser.addPositionalArgument(QStringLiteral("[windows]"), QStringLiteral("Window IDs."));

    parser.process(QCoreApplication::arguments());

    if (parser.isSet(outputAllOption)) {
        BobUIDiag::dumpNativeWindows();
        return 0;
    }

    QWindowList windows;
    for (const QString &argument : parser.positionalArguments()) {
        bool ok = true;
        WId wid = createInternalWindow(argument);
        if (!wid)
            wid = argument.toULongLong(&ok, 0);
        if (!wid || !ok) {
            std::cerr << "Invalid window id: \"" << qPrintable(argument) << "\"\n";
            return -1;
        }
        QWindow *foreignWindow = QWindow::fromWinId(wid);
        if (!foreignWindow)
            return -1;
        foreignWindow->setObjectName("ForeignWindow" + QString::number(wid, 16));
        windows.append(foreignWindow);
        if (parser.isSet(moveOption))
            foreignWindow->setFramePosition(QGuiApplication::primaryScreen()->availableGeometry().topLeft());
    }

    if (windows.isEmpty())
        parser.showHelp(0);

    int exitCode = 0;

    if (parser.isSet(embedOption)) {
        BobUIDiag::EventFilter::EventCategories eventCategories;
        for (int i = 0; i < eventFilterOptionCount; ++i) {
            if (parser.isSet(QLatin1String(eventFilterOptions[i].name)))
                eventCategories |= eventFilterOptions[i].categories;
        }
        if (eventCategories)
            app.installEventFilter(new BobUIDiag::EventFilter(eventCategories, &app));

        const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        QPoint pos = availableGeometry.topLeft() + QPoint(availableGeometry.width(), availableGeometry.height()) / 3;

        WidgetPtrList mainWindows;
        for (QWindow *window : std::as_const(windows)) {
            WidgetPtr mainWindow(new EmbeddingWindow(window));
            mainWindow->move(pos);
            mainWindow->resize(availableGeometry.size() / 4);
            mainWindow->show();
            pos += QPoint(40, 40);
            mainWindows.append(mainWindow);
        }
        exitCode = app.exec();

    } else if (parser.isSet(continuousOption)) {
        WindowDumper dumper(windows);
        dumper.dump();
        BOBUIimer *timer = new BOBUIimer(&dumper);
        QObject::connect(timer, &BOBUIimer::timeout, &dumper, &WindowDumper::dump);
        timer->start(1000);
        exitCode = app.exec();
    } else {
        WindowDumper(windows).dump();
    }

    return exitCode;
}

#include "main.moc"
