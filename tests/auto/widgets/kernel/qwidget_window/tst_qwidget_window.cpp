// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BobUIGui/BobUIGui>
#include <BobUICore/BOBUIextStream>
#include <BobUICore/QStringList>
#include <BobUICore/QMimeData>
#include <BobUICore/QPoint>
#include <qeventloop.h>
#include <qlist.h>

#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <bobuiabwidget.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <bobuioolbar.h>
#include <qsignalspy.h>
#include <private/qwindow_p.h>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatformdrag.h>
#include <private/qhighdpiscaling_p.h>

#include <BobUITest/private/bobuiesthelpers_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

using namespace BOBUIestPrivate;
using namespace BobUI::StringLiterals;

// Compare a window position that may go through scaling in the platform plugin with fuzz.
static inline bool qFuzzyCompareWindowPosition(const QPoint &p1, const QPoint p2, int fuzz)
{
    return (p1 - p2).manhattanLength() <= fuzz;
}

static QString msgPointMismatch(const QPoint &p1, const QPoint p2)
{
    QString result;
    QDebug(&result) << p1 << "!=" << p2 << ", manhattanLength=" << (p1 - p2).manhattanLength();
    return result;
}

class tst_QWidget_window : public QObject
{
    Q_OBJECT

public:
    tst_QWidget_window();

public slots:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

private slots:
    void tst_min_max_size();
    void tst_min_max_size_data();
    void tst_move_show();
    void tst_show_move();
    void tst_show_move_hide_show();

    void tst_resize_show();
    void tst_show_resize();
    void tst_show_resize_hide_show();

    void close();

    void tst_windowFilePathAndwindowTitle_data();
    void tst_windowFilePathAndwindowTitle();
    void tst_windowFilePath_data();
    void tst_windowFilePath();

    void tst_showWithoutActivating();
    void tst_paintEventOnSecondShow();
    void tst_exposeObscuredMapped_BOBUIBUG39220();
    void tst_paintEventOnResize_BOBUIBUG50796();

#if BOBUI_CONFIG(draganddrop)
    void tst_dnd();
    void tst_dnd_events();
    void tst_dnd_propagation();
    void tst_dnd_destroyOnDrop();
#endif

    void tst_bobuibug35600();
    void tst_updateWinId_BOBUIBUG40681();
    void tst_recreateWindow_BOBUIBUG40817();

    void tst_resize_count();
    void tst_move_count();

    void tst_showhide_count();

    void tst_eventfilter_on_toplevel();

    void BOBUIBUG_50561_QCocoaBackingStore_paintDevice_crash();

    void setWindowState_data();
    void setWindowState();

    void nativeShow();

    void BOBUIBUG_56277_resize_on_showEvent();

    void mouseMoveWithPopup_data();
    void mouseMoveWithPopup();

    void showHideWindowHandle_data();
    void showHideWindowHandle();

    void resetFocusObjectOnDestruction();

    void cleanupOnDestruction();

private:
    QSize m_testWidgetSize;
    const int m_fuzz;
};

tst_QWidget_window::tst_QWidget_window() :
     m_fuzz(int(QHighDpiScaling::factor(QGuiApplication::primaryScreen())))
{
    const int screenWidth =  QGuiApplication::primaryScreen()->geometry().width();
    const int width = qMax(200, 100 * ((screenWidth + 500) / 1000));
    m_testWidgetSize = QSize(width, width);
}

void tst_QWidget_window::initTestCase()
{
}

void tst_QWidget_window::init()
{
    BOBUIest::failOnWarning(QRegularExpression(".*No such slot.*"));
}

void tst_QWidget_window::cleanupTestCase()
{
}

void tst_QWidget_window::cleanup()
{
    QVERIFY(QApplication::topLevelWidgets().isEmpty());
}

/* Test if the maximum/minimum size constraints
 * are propagated from the widget to the QWidgetWindow
 * independently of whether they were set before or after
 * window creation (BOBUIBUG-26745). */

void tst_QWidget_window::tst_min_max_size_data()
{
    BOBUIest::addColumn<bool>("setMinMaxSizeBeforeShow");
    BOBUIest::newRow("Set min/max size after show") << false;
    BOBUIest::newRow("Set min/max size before show") << true;
}

void tst_QWidget_window::tst_min_max_size()
{
    QFETCH(bool, setMinMaxSizeBeforeShow);
    const QSize minSize(300, 400);
    const QSize maxSize(1000, 500);
    QWidget w1;
    setFrameless(&w1);
    (new QVBoxLayout(&w1))->addWidget(new QPushButton("Test"));
    if (setMinMaxSizeBeforeShow) {
        w1.setMinimumSize(minSize);
        w1.setMaximumSize(maxSize);
    }
    w1.show();
    if (!setMinMaxSizeBeforeShow) {
        w1.setMinimumSize(minSize);
        w1.setMaximumSize(maxSize);
    }
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w1));
    QCOMPARE(w1.windowHandle()->minimumSize(),minSize);
    QCOMPARE(w1.windowHandle()->maximumSize(), maxSize);
}

void tst_QWidget_window::tst_move_show()
{
    QWidget w;
    const QPoint pos(100, 100);
    w.move(pos);
    w.show();
    QVERIFY2(qFuzzyCompareWindowPosition(w.pos(), pos, m_fuzz),
             qPrintable(msgPointMismatch(w.pos(), pos)));
}

void tst_QWidget_window::tst_show_move()
{
    QWidget w;
    w.show();
    const QPoint pos(100, 100);
    w.move(pos);
    QVERIFY2(qFuzzyCompareWindowPosition(w.pos(), pos, m_fuzz),
             qPrintable(msgPointMismatch(w.pos(), pos)));
}

void tst_QWidget_window::tst_show_move_hide_show()
{
    QWidget w;
    w.show();
    const QPoint pos(100, 100);
    w.move(pos);
    w.hide();
    w.show();
    QVERIFY2(qFuzzyCompareWindowPosition(w.pos(), pos, m_fuzz),
             qPrintable(msgPointMismatch(w.pos(), pos)));
}

void tst_QWidget_window::tst_resize_show()
{
    QWidget w;
    w.resize(m_testWidgetSize);
    w.show();
    QCOMPARE(w.size(), m_testWidgetSize);
}

void tst_QWidget_window::tst_show_resize()
{
    QWidget w;
    w.show();
    w.resize(m_testWidgetSize);
    QCOMPARE(w.size(), m_testWidgetSize);
}

void tst_QWidget_window::tst_show_resize_hide_show()
{
    QWidget w;
    w.show();
    w.resize(m_testWidgetSize);
    w.hide();
    w.show();
    QCOMPARE(w.size(), m_testWidgetSize);
}

void tst_QWidget_window::close()
{
    // Verfy that closing a QWidgetWindow deletes its platform window,
    // as expected of a QWindow subclass. This must be done also
    // if QWidget API is used to close. The QCloseEvent must not be
    // spontaneous if the close is triggered by a BobUI API that the application
    // would call in response to an event, and spontaneous if it is directly
    // caused by user interaction, such as clicking the (x) in the titlebar.
    // We can simulate this only by generating a WindowSystemEvent.
    // Children of the window should get a hide event (never spontaneous when
    // caused by closing the window).

    struct Widget : public QWidget
    {
        using QWidget::QWidget;
        int spontClose = -1;
        int spontHide = -1;
    protected:
        void hideEvent(QHideEvent *e) override
        { spontHide = e->spontaneous() ? 1 : 0; }
        void closeEvent(QCloseEvent *e) override
        { spontClose = e->spontaneous() ? 1 : 0; }
    };

    // QWindow::close()
    {
        Widget w;
        Widget child(&w);
        w.winId();
        QVERIFY(w.windowHandle());
        QVERIFY(w.windowHandle()->handle());
        w.windowHandle()->close();
        QCOMPARE(w.spontClose, 0);
        QCOMPARE(child.spontHide, -1); // was never shown
        QVERIFY(w.windowHandle());
        QVERIFY(!w.windowHandle()->handle());
    }

    // QWidget::close()
    {
        Widget w;
        Widget child(&w);
        w.show();
        QVERIFY(w.windowHandle());
        QVERIFY(w.windowHandle()->handle());
        w.close();
        QCOMPARE(w.spontClose, 0);
        QCOMPARE(child.spontHide, 0);
        QVERIFY(w.windowHandle());
        QVERIFY(!w.windowHandle()->handle());
    }

    // User-initiated close
    {
        Widget w;
        Widget child(&w);
        w.show();
        QWindowSystemInterface::handleCloseEvent(w.windowHandle());
        QApplication::processEvents();
        QCOMPARE(w.spontClose, 1);
        QCOMPARE(child.spontHide, 0);
    }
}

class PaintTestWidget : public QWidget
{
public:
    int paintEventCount;

    explicit PaintTestWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , paintEventCount(0)
    {}

    void paintEvent(QPaintEvent *event) override
    {
        ++paintEventCount;
        QWidget::paintEvent(event);
    }
};

void tst_QWidget_window::tst_windowFilePathAndwindowTitle_data()
{
    BOBUIest::addColumn<bool>("setWindowTitleBefore");
    BOBUIest::addColumn<bool>("setWindowTitleAfter");
    BOBUIest::addColumn<QString>("filePath");
    BOBUIest::addColumn<QString>("applicationName");
    BOBUIest::addColumn<QString>("indyWindowTitle");
    BOBUIest::addColumn<QString>("finalTitleBefore");
    BOBUIest::addColumn<QString>("finalTitleAfter");

    QString validPath = QApplication::applicationFilePath();
    QString fileNameOnly = QFileInfo(validPath).fileName() + QLatin1String("[*]");
    QString windowTitle = QLatin1String("Here is a Window Title");
    QString defaultPlatString = fileNameOnly;

    BOBUIest::newRow("never Set Title nor AppName") << false << false << validPath << QString() << windowTitle << defaultPlatString << defaultPlatString;
    BOBUIest::newRow("set title after only, but no AppName") << false << true << validPath << QString() << windowTitle << defaultPlatString << windowTitle;
    BOBUIest::newRow("set title before only, not AppName") << true << false << validPath << QString() << windowTitle << windowTitle << windowTitle;
    BOBUIest::newRow("always set title, not appName") << true << true << validPath << QString() << windowTitle << windowTitle << windowTitle;

    QString appName = QLatin1String("Killer App"); // BobUI4 used to make it part of windowTitle(), BobUI5 doesn't anymore, the QPA plugin takes care of it.
    QString platString = fileNameOnly;

    BOBUIest::newRow("never Set Title, yes AppName") << false << false << validPath << appName << windowTitle << platString << platString;
    BOBUIest::newRow("set title after only, yes AppName") << false << true << validPath << appName << windowTitle << platString << windowTitle;
    BOBUIest::newRow("set title before only, yes AppName") << true << false << validPath << appName << windowTitle << windowTitle << windowTitle;
    BOBUIest::newRow("always set title, yes appName") << true << true << validPath << appName << windowTitle << windowTitle << windowTitle;
}

void tst_QWidget_window::tst_windowFilePathAndwindowTitle()
{
    QFETCH(bool, setWindowTitleBefore);
    QFETCH(bool, setWindowTitleAfter);
    QFETCH(QString, filePath);
    QFETCH(QString, applicationName);
    QFETCH(QString, indyWindowTitle);
    QFETCH(QString, finalTitleBefore);
    QFETCH(QString, finalTitleAfter);


    QWidget widget;
    QCOMPARE(widget.windowFilePath(), QString());

    if (!applicationName.isEmpty())
        qApp->setApplicationName(applicationName);
    else
        qApp->setApplicationName(QString());

    if (setWindowTitleBefore) {
        widget.setWindowTitle(indyWindowTitle);
    }
    widget.setWindowFilePath(filePath);
    QCOMPARE(widget.windowTitle(), finalTitleBefore);
    QCOMPARE(widget.windowFilePath(), filePath);

    if (setWindowTitleAfter) {
        widget.setWindowTitle(indyWindowTitle);
    }
    QCOMPARE(widget.windowTitle(), finalTitleAfter);
    QCOMPARE(widget.windowFilePath(), filePath);
}

void tst_QWidget_window::tst_windowFilePath_data()
{
    BOBUIest::addColumn<QString>("filePath");
    BOBUIest::addColumn<QString>("result");
    BOBUIest::addColumn<bool>("again");
    BOBUIest::addColumn<QString>("filePath2");
    BOBUIest::addColumn<QString>("result2");

    QString validPath = QApplication::applicationFilePath();
    QString invalidPath = QLatin1String("::**Never a Real Path**::");

    BOBUIest::newRow("never Set Path") << QString() << QString() << false << QString() << QString();
    BOBUIest::newRow("never EVER Set Path") << QString() << QString() << true << QString() << QString();
    BOBUIest::newRow("Valid Path") << validPath << validPath << false << QString() << QString();
    BOBUIest::newRow("invalid Path") << invalidPath << invalidPath << false << QString() << QString();
    BOBUIest::newRow("Valid Path then empty") << validPath << validPath << true << QString() << QString();
    BOBUIest::newRow("invalid Path then empty") << invalidPath << invalidPath << true << QString() << QString();
    BOBUIest::newRow("invalid Path then valid") << invalidPath << invalidPath << true << validPath << validPath;
    BOBUIest::newRow("valid Path then invalid") << validPath << validPath << true << invalidPath << invalidPath;
}

void tst_QWidget_window::tst_windowFilePath()
{
    QFETCH(QString, filePath);
    QFETCH(QString, result);
    QFETCH(bool, again);
    QFETCH(QString, filePath2);
    QFETCH(QString, result2);

    QWidget widget;
    QCOMPARE(widget.windowFilePath(), QString());
    widget.setWindowFilePath(filePath);
    QCOMPARE(widget.windowFilePath(), result);
    if (again) {
        widget.setWindowFilePath(filePath2);
        QCOMPARE(widget.windowFilePath(), result2);
    }
}

void tst_QWidget_window::tst_showWithoutActivating()
{
    QString platformName = QGuiApplication::platformName().toLower();
    if (platformName != QStringLiteral("xcb")
        && platformName != QStringLiteral("windows")
        && platformName != QStringLiteral("cocoa")
        && platformName != QStringLiteral("ios")
        && platformName != QStringLiteral("tvos")
        && platformName != QStringLiteral("watchos"))
        QSKIP("BobUI::WA_ShowWithoutActivating is currently supported only on xcb, " \
              "windows, and macos/ios/tvos/watchos platforms.");

    QWidget w1;
    w1.setAttribute(BobUI::WA_ShowWithoutActivating);
    w1.show();
    QVERIFY(!BOBUIest::qWaitForWindowActive(&w1));

    QWidget w2;
    w2.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w2));

    QWidget w3;
    w3.setAttribute(BobUI::WA_ShowWithoutActivating);
    w3.show();
    QVERIFY(!BOBUIest::qWaitForWindowActive(&w3));

    w3.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w3));
}

void tst_QWidget_window::tst_paintEventOnSecondShow()
{
    PaintTestWidget w;
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    w.hide();

    w.paintEventCount = 0;
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    QApplication::processEvents();
    BOBUIRY_VERIFY(w.paintEventCount > 0);
}

void tst_QWidget_window::tst_exposeObscuredMapped_BOBUIBUG39220()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    const auto integration = QGuiApplicationPrivate::platformIntegration();
    if (!integration->hasCapability(QPlatformIntegration::MultipleWindows)
        || !integration->hasCapability(QPlatformIntegration::NonFullScreenWindows)) {
        QSKIP("The platform does not have the required capabilities");
    }
    // BOBUIBUG-39220: Fully obscured parent widgets may not receive expose
    // events (as is the case for frameless, obscured parents on Windows).
    // Ensure BobUI::WA_Mapped is set so updating works.
    const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    const QSize size = availableGeometry.size() / 6;
    QWidget topLevel;
    setFrameless(&topLevel);
    topLevel.resize(size);
    const QPoint sizeP(size.width(), size.height());
    topLevel.move(availableGeometry.center() - sizeP / 2);
    QWidget *child = new QWidget(&topLevel);
    child->resize(size);
    child->move(0, 0);
    QVERIFY(child->winId());
    topLevel.show();
    BOBUIRY_VERIFY(child->testAttribute(BobUI::WA_Mapped));
    QVERIFY(topLevel.testAttribute(BobUI::WA_Mapped));
}

void tst_QWidget_window::tst_paintEventOnResize_BOBUIBUG50796()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    const QRect availableGeo = QGuiApplication::primaryScreen()->availableGeometry();

    QWidget root;
    root.setGeometry(availableGeo.width()/2 - 100, availableGeo.height()/2 - 100,
                     200, 200);

    PaintTestWidget *native = new PaintTestWidget(&root);
    native->winId(); // We're testing native widgets
    native->setGeometry(10, 10, 50, 50);

    root.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
    QVERIFY(BOBUIest::qWaitForWindowActive(&root));
    QVERIFY(native->isVisible());

    // The top level widget can receive new margins after show
    // if the size is big enough to end up overlapping safe areas.
    int safeMarginsResizeCount = 0;
    connect(native->windowHandle(), &QWindow::safeAreaMarginsChanged, this, [&]() {
        ++safeMarginsResizeCount;
    });

    native->paintEventCount = 0;
    native->resize(native->width() + 10, native->height() + 10);
    BOBUIest::qWait(50); // Wait for paint events
    BOBUIRY_COMPARE(native->paintEventCount, 1 + safeMarginsResizeCount);
}

#if BOBUI_CONFIG(draganddrop)

/* DnD test for QWidgetWindow (handleDrag*Event() functions).
 * Simulates a drop onto a QWidgetWindow of a top level widget
 * that has 3 child widgets in a vertical layout with a frame. Only the lower 2
 * child widgets accepts drops (BOBUIBUG-22987), the bottom child has another child
 * that does not accept drops.
 * Sends a series of DnD events to the QWidgetWindow,
 * entering the top level at the top frame and move
 * down in steps of 5 pixels, drop onto the bottom widget.
 * The test compares the sequences of events received by the widgets in readable format.
 * It also checks whether the address of the mimedata received is the same as the
 * sending one, that is, no conversion/serialization of text mime data occurs in the
 * process. */

static const char *expectedLogC[] = {
    "Event at 11,1 ignored",
    "Event at 11,21 ignored",
    "Event at 11,41 ignored",
    "Event at 11,61 ignored",
    "Event at 11,81 ignored",
    "Event at 11,101 ignored",
    "acceptingDropsWidget1::dragEnterEvent at 1,11 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "acceptingDropsWidget1::dragMoveEvent at 1,11 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,121 accepted",
    "acceptingDropsWidget1::dragMoveEvent at 1,31 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,141 accepted",
    "acceptingDropsWidget1::dragMoveEvent at 1,51 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,161 accepted",
    "acceptingDropsWidget1::dragMoveEvent at 1,71 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,181 accepted",
    "acceptingDropsWidget1::dragLeaveEvent QDragLeaveEvent",
    "Event at 11,201 ignored",
    "acceptingDropsWidget2::dragEnterEvent at 1,11 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "acceptingDropsWidget2::dragMoveEvent at 1,11 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,221 accepted",
    "acceptingDropsWidget2::dragMoveEvent at 1,31 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,241 accepted",
    "acceptingDropsWidget2::dropEvent at 1,51 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,261 accepted",
    "acceptingDropsWidget3::dragEnterEvent at 1,21 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 11,281 accepted",
    "acceptingDropsWidget3::dragLeaveEvent QDragLeaveEvent",
    "Event at 11,301 ignored",
    "acceptingDropsWidget1::dragEnterEvent at 10,10 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 0,0 accepted",
    "acceptingDropsWidget1::dragMoveEvent at 11,11 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 1,1 accepted",
    "acceptingDropsWidget1::dropEvent at 12,12 action=1 MIME_DATA_ADDRESS 'testmimetext'",
    "Event at 2,2 accepted"
};

// A widget that logs the DnD events it receives into a QStringList.
class DnDEventLoggerWidget : public QWidget
{
public:
    DnDEventLoggerWidget(QStringList *log, QWidget *w = nullptr, bool ignoreDragMove = false)
        : QWidget(w), m_log(log), m_ignoreDragMove(ignoreDragMove)
    {}
protected:
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dropEvent(QDropEvent *) override;

private:
    void formatDropEvent(const char *function, const QDropEvent *e, BOBUIextStream &str) const;
    QStringList *m_log;
    bool m_ignoreDragMove;
};

void DnDEventLoggerWidget::formatDropEvent(const char *function, const QDropEvent *e, BOBUIextStream &str) const
{
    str << objectName() << "::" << function  << " at " << e->position().toPoint().x() << ',' << e->position().toPoint().y()
        << " action=" << e->dropAction()
        << ' ' << quintptr(e->mimeData()) << " '" << e->mimeData()->text() << '\'';
}

void DnDEventLoggerWidget::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
    QString message;
    BOBUIextStream str(&message);
    formatDropEvent("dragEnterEvent", e, str);
    m_log->push_back(message);
}

void DnDEventLoggerWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (m_ignoreDragMove)
        return;
    e->accept();
    QString message;
    BOBUIextStream str(&message);
    formatDropEvent("dragMoveEvent", e, str);
    m_log->push_back(message);
}

void DnDEventLoggerWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
    m_log->push_back(objectName() + QLatin1String("::") + QLatin1String("dragLeaveEvent") + QLatin1String(" QDragLeaveEvent"));
}

void DnDEventLoggerWidget::dropEvent(QDropEvent *e)
{
    e->accept();
    QString message;
    BOBUIextStream str(&message);
    formatDropEvent("dropEvent", e, str);
    m_log->push_back(message);
}

static QString msgEventAccepted(const QDropEvent &e)
{
    QString message;
    BOBUIextStream str(&message);
    str << "Event at " << e.position().toPoint().x() << ',' << e.position().toPoint().y() << ' ' << (e.isAccepted() ? "accepted" : "ignored");
    return message;
}

void tst_QWidget_window::tst_dnd()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStringList log;
    DnDEventLoggerWidget dndTestWidget(&log);

    dndTestWidget.setObjectName(QLatin1String("dndTestWidget"));
    dndTestWidget.setWindowTitle(dndTestWidget.objectName());
    dndTestWidget.resize(200, 300);

    QWidget *dropsRefusingWidget1 = new DnDEventLoggerWidget(&log, &dndTestWidget);
    dropsRefusingWidget1->setObjectName(QLatin1String("dropsRefusingWidget1"));
    dropsRefusingWidget1->resize(180, 80);
    dropsRefusingWidget1->move(10, 10);

    QWidget *dropsAcceptingWidget1 = new DnDEventLoggerWidget(&log, &dndTestWidget);
    dropsAcceptingWidget1->setAcceptDrops(true);
    dropsAcceptingWidget1->setObjectName(QLatin1String("acceptingDropsWidget1"));
    dropsAcceptingWidget1->resize(180, 80);
    dropsAcceptingWidget1->move(10, 110);

    // Create a native widget on top of dropsAcceptingWidget1 to check BOBUIBUG-27336
    QWidget *nativeWidget = new QWidget(dropsAcceptingWidget1);
    nativeWidget->resize(160, 60);
    nativeWidget->move(10, 10);
    nativeWidget->winId();

    QWidget *dropsAcceptingWidget2 = new DnDEventLoggerWidget(&log, &dndTestWidget);
    dropsAcceptingWidget2->setAcceptDrops(true);
    dropsAcceptingWidget2->setObjectName(QLatin1String("acceptingDropsWidget2"));
    dropsAcceptingWidget2->resize(180, 80);
    dropsAcceptingWidget2->move(10, 210);

    QWidget *dropsRefusingWidget2 = new DnDEventLoggerWidget(&log, dropsAcceptingWidget2);
    dropsRefusingWidget2->setObjectName(QLatin1String("dropsRefusingDropsWidget2"));
    dropsRefusingWidget2->resize(160, 60);
    dropsRefusingWidget2->move(10, 10);

    QWidget *dropsAcceptingWidget3 = new DnDEventLoggerWidget(&log, &dndTestWidget, true);
    dropsAcceptingWidget3->setAcceptDrops(true);
    dropsAcceptingWidget3->setObjectName(QLatin1String("acceptingDropsWidget3"));
    dropsAcceptingWidget3->resize(180, 40);
    dropsAcceptingWidget3->move(10, 260);

    dndTestWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dndTestWidget));
    QVERIFY(BOBUIest::qWaitForWindowActive(&dndTestWidget));

    QMimeData mimeData;
    mimeData.setText(QLatin1String("testmimetext"));

    // Simulate DnD events on the QWidgetWindow.
    QPoint position = QPoint(11, 1);
    QDragEnterEvent e(position, BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
    QWindow *window = dndTestWidget.windowHandle();
    qApp->sendEvent(window, &e);
    log.push_back(msgEventAccepted(e));
    while (true) {
        position.ry() += 20;
        if (position.y() >= 250 && position.y() < 270) {
            QDropEvent e(position, BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
            qApp->sendEvent(window, &e);
            log.push_back(msgEventAccepted(e));
        } else {
            QDragMoveEvent e(position, BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
            qApp->sendEvent(window, &e);
            log.push_back(msgEventAccepted(e));
        }
        if (position.y() > 290)
            break;
    }

    window = nativeWidget->windowHandle();
    QDragEnterEvent enterEvent(QPoint(0, 0), BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
    qApp->sendEvent(window, &enterEvent);
    log.push_back(msgEventAccepted(enterEvent));

    QDragMoveEvent moveEvent(QPoint(1, 1), BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
    qApp->sendEvent(window, &moveEvent);
    log.push_back(msgEventAccepted(moveEvent));

    QDropEvent dropEvent(QPoint(2, 2), BobUI::CopyAction, &mimeData, BobUI::LeftButton, BobUI::NoModifier);
    qApp->sendEvent(window, &dropEvent);
    log.push_back(msgEventAccepted(dropEvent));

    // Compare logs.
    QStringList expectedLog;
    const int expectedLogSize = int(sizeof(expectedLogC) / sizeof(expectedLogC[0]));
    const QString mimeDataAddress = QString::number(quintptr(&mimeData));
    const QString mimeDataAddressPlaceHolder = QLatin1String("MIME_DATA_ADDRESS");
    for (int i= 0; i < expectedLogSize; ++i)
        expectedLog.push_back(QString::fromLatin1(expectedLogC[i]).replace(mimeDataAddressPlaceHolder, mimeDataAddress));

    if (log.size() != expectedLog.size()) {
        for (int i = 0; i < log.size() && i < expectedLog.size(); ++i)
            QCOMPARE(log.at(i), expectedLog.at(i));
        const int iMin = std::min(log.size(), expectedLog.size());
        for (int i = iMin; i < log.size(); ++i)
            qDebug() << "log[" << i << "]:" << log.at(i);
        for (int i = iMin; i < expectedLog.size(); ++i)
            qDebug() << "exp[" << i << "]:" << log.at(i);
    }
    QCOMPARE(log, expectedLog);
}

class DnDEventRecorder : public QWidget
{
    Q_OBJECT
public:
    QString _dndEvents;
    DnDEventRecorder() { setAcceptDrops(true); }

protected:
    void mousePressEvent(QMouseEvent *) override
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", "some data");
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec();
    }

    void dragEnterEvent(QDragEnterEvent *e) override
    {
        e->accept();
        _dndEvents.append(QStringLiteral("DragEnter "));
    }
    void dragMoveEvent(QDragMoveEvent *e) override
    {
        e->accept();
        _dndEvents.append(QStringLiteral("DragMove "));
        emit dragMoveReceived();
    }
    void dragLeaveEvent(QDragLeaveEvent *e) override
    {
        e->accept();
        _dndEvents.append(QStringLiteral("DragLeave "));
    }
    void dropEvent(QDropEvent *e) override
    {
        e->accept();
        _dndEvents.append(QStringLiteral("DropEvent "));
    }

signals:
    void dragMoveReceived();
};

void tst_QWidget_window::tst_dnd_events()
{
    // Note: This test is somewhat a hack as testing DnD with bobuiestlib is not
    // supported at the moment. The test verifies that we get an expected event
    // sequence on dnd operation that does not move a mouse. This logic is implemented
    // in QGuiApplication, so we have to go via QWindowSystemInterface API (BOBUIest::mouse*).
    const auto platformName = QGuiApplication::platformName().toLower();
    // The test is known to work with XCB and platforms that use the default dnd
    // implementation QSimpleDrag (e.g. qnx). Running on XCB should be sufficient to
    // catch regressions at cross platform code: QGuiApplication::processDrag/Leave().
    if (platformName != "xcb")
        return;
    if (qgetenv("XDG_CURRENT_DESKTOP").toLower().contains("ubuntu") && QSysInfo::productVersion() == "24.04"_L1)
        QSKIP("This hangs on Ubuntu 24.04 X11, see also BOBUIBUG-129567.");

    const QString expectedDndEvents = "DragEnter DragMove DropEvent DragEnter DragMove "
                                      "DropEvent DragEnter DragMove DropEvent ";
    DnDEventRecorder dndWidget;
    dndWidget.setGeometry(100, 100, 200, 200);
    dndWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dndWidget));
    QVERIFY(BOBUIest::qWaitForWindowActive(&dndWidget));

    // ### FIXME - BOBUIBUG-35117 ???
    auto targetCenter = QPoint(dndWidget.width(), dndWidget.height()) / 2;
    auto targetCenterGlobal = dndWidget.mapToGlobal(targetCenter);
    QCursor::setPos(targetCenterGlobal);
    QVERIFY(BOBUIest::qWaitFor([&]() { return QCursor::pos() == targetCenterGlobal; }));
    QCoreApplication::processEvents(); // clear mouse events generated from cursor

    auto window = dndWidget.window()->windowHandle();

    // Some dnd implementation rely on running internal event loops, so we have to use
    // the following queued signal hack to simulate mouse clicks in the widget.
    auto connection = QObject::connect(&dndWidget, &DnDEventRecorder::dragMoveReceived, this,
                                        [=]() {
        BOBUIest::mouseRelease(window, BobUI::LeftButton);
    }, BobUI::QueuedConnection);

    BOBUIest::mousePress(window, BobUI::LeftButton);
    BOBUIest::mousePress(window, BobUI::LeftButton);
    BOBUIest::mousePress(window, BobUI::LeftButton);

    QCOMPARE(dndWidget._dndEvents, expectedDndEvents);

    dndWidget._dndEvents.clear();
    dndWidget.disconnect(connection);
    int step = 0;
    QObject::connect(&dndWidget, &DnDEventRecorder::dragMoveReceived, this, [window, &step]() {
        switch (step++) {
        case 0:
            BOBUIest::keyPress(window, BobUI::Key_Shift, BobUI::ShiftModifier);
            break;
        case 1:
            BOBUIest::keyRelease(window, BobUI::Key_Shift, BobUI::NoModifier);
            break;
        default:
            BOBUIest::mouseRelease(window, BobUI::LeftButton);
            break;
        }
    }, BobUI::QueuedConnection);

    BOBUIest::mousePress(window, BobUI::LeftButton);
    const QString expectedDndWithModsEvents = "DragEnter DragMove DragMove DragMove DropEvent ";
    QCOMPARE(dndWidget._dndEvents, expectedDndWithModsEvents);
}

class DropTarget : public QWidget
{
public:
    explicit DropTarget()
    {
        setAcceptDrops(true);

        const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        auto width = availableGeometry.width() / 6;
        auto height = availableGeometry.height() / 4;

        setGeometry(availableGeometry.x() + 200, availableGeometry.y() + 200, width, height);

        QLabel *label = new QLabel(QStringLiteral("Test"), this);
        label->setGeometry(40, 40, 60, 60);
        label->setAcceptDrops(true);
    }

    void dragEnterEvent(QDragEnterEvent *event) override
    {
        event->accept();
        mDndEvents.append("enter ");
    }

    void dragMoveEvent(QDragMoveEvent *event) override
    {
        event->acceptProposedAction();
    }

    void dragLeaveEvent(QDragLeaveEvent *) override
    {
        mDndEvents.append("leave ");
    }

    void dropEvent(QDropEvent *event) override
    {
        event->accept();
        mDndEvents.append("drop ");
    }

    QString mDndEvents;
};

void tst_QWidget_window::tst_dnd_propagation()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMimeData mimeData;
    mimeData.setText(QLatin1String("testmimetext"));

    DropTarget target;
    target.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&target));

    BobUI::DropActions supportedActions = BobUI::DropAction::CopyAction;
    QWindow *window = target.windowHandle();

    auto posInsideDropTarget = QHighDpi::toNativePixels(QPoint(20, 20), window->screen());
    auto posInsideLabel      = QHighDpi::toNativePixels(QPoint(60, 60), window->screen());

    // Enter DropTarget.
    QWindowSystemInterface::handleDrag(window, &mimeData, posInsideDropTarget, supportedActions, {}, {});
    // Enter QLabel. This will propagate because default QLabel does
    // not accept the drop event in dragEnterEvent().
    QWindowSystemInterface::handleDrag(window, &mimeData, posInsideLabel, supportedActions, {}, {});
    // Drop on QLabel. DropTarget will get dropEvent(), because it accepted the event.
    QWindowSystemInterface::handleDrop(window, &mimeData, posInsideLabel, supportedActions, {}, {});

    QGuiApplication::processEvents();

    QCOMPARE(target.mDndEvents, "enter leave enter drop ");
}

class ReparentSelfOnDropWidget : public QWidget
{
public:
    ReparentSelfOnDropWidget(QWidget *newFutureParent)
        : m_newFutureParent(newFutureParent)
    {
        setAcceptDrops(true);

        const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        auto width = availableGeometry.width() / 6;
        auto height = availableGeometry.height() / 4;

        setGeometry(availableGeometry.x() + 200, availableGeometry.y() + 200, width, height);

        QLabel *label = new QLabel(QStringLiteral("Test"), this);
        label->setGeometry(40, 40, 60, 60);
        label->setAcceptDrops(true);
    }

    void dragEnterEvent(QDragEnterEvent *event) override
    {
        event->accept();
    }

    void dragMoveEvent(QDragMoveEvent *event) override
    {
        event->acceptProposedAction();
    }

    void dropEvent(QDropEvent *event) override
    {
        event->accept();
        // Turn 'this' from a top-level widget to a child widget.
        // This destroys the QWidgetWindow since the widget is no longer top-level.
        setParent(m_newFutureParent);
    }

private:
    QWidget *m_newFutureParent;
};

void tst_QWidget_window::tst_dnd_destroyOnDrop()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMimeData mimeData;
    mimeData.setText(QLatin1String("testmimetext"));

    QWidget newParent;
    newParent.resize(400, 400);
    newParent.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&newParent));

    ReparentSelfOnDropWidget *target = new ReparentSelfOnDropWidget(&newParent);
    target->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(target));

    BobUI::DropActions supportedActions = BobUI::DropAction::CopyAction;
    QWindow *window = target->windowHandle();

    auto posInsideDropTarget = QHighDpi::toNativePixels(QPoint(20, 20), window->screen());
    auto posInsideLabel      = QHighDpi::toNativePixels(QPoint(60, 60), window->screen());

    QWindowSystemInterface::handleDrag(window, &mimeData, posInsideDropTarget, supportedActions, {}, {});
    QWindowSystemInterface::handleDrag(window, &mimeData, posInsideLabel, supportedActions, {}, {});
    QWindowSystemInterface::handleDrop(window, &mimeData, posInsideLabel, supportedActions, {}, {});

    QGuiApplication::processEvents();
}

#endif

void tst_QWidget_window::tst_bobuibug35600()
{
    QWidget w;
    w.show();

    QWidget *wA = new QWidget;
    QHBoxLayout *layoutA = new QHBoxLayout;

    QWidget *wB = new QWidget;
    layoutA->addWidget(wB);

    QWidget *wC = new QWidget;
    layoutA->addWidget(wC);

    wA->setLayout(layoutA);

    QWidget *wD = new QWidget;
    wD->setAttribute(BobUI::WA_NativeWindow);
    wD->setParent(wB);

    QWidget *wE = new QWidget(wC, BobUI::Tool | BobUI::FramelessWindowHint | BobUI::WindowTransparentForInput);
    wE->show();

    wA->setParent(&w);

    // BOBUIBUG-35600: program may crash here or on exit
}

void tst_QWidget_window::tst_updateWinId_BOBUIBUG40681()
{
    QWidget w;
    QVBoxLayout *vl = new QVBoxLayout(&w);
    QLabel *lbl = new QLabel("HELLO1");
    lbl->setAttribute(BobUI::WA_NativeWindow);
    lbl->setObjectName("label1");
    vl->addWidget(lbl);
    w.setMinimumWidth(m_testWidgetSize.width());

    w.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    QCOMPARE(lbl->winId(), lbl->windowHandle()->winId());

     // simulate screen change and notification
    QWindow *win = w.windowHandle();
    w.windowHandle()->destroy();
    lbl->windowHandle()->destroy();
    w.windowHandle()->create();
    lbl->windowHandle()->create();
    QWindowPrivate *p = bobui_window_private(win);
    p->emitScreenChangedRecursion(win->screen());

    QCOMPARE(lbl->winId(), lbl->windowHandle()->winId());
}

void tst_QWidget_window::tst_recreateWindow_BOBUIBUG40817()
{
    BOBUIabWidget tab;
    tab.setMinimumWidth(m_testWidgetSize.width());

    QWidget *w = new QWidget;
    tab.addTab(w, "Tab1");
    QVBoxLayout *vl = new QVBoxLayout(w);
    QLabel *lbl = new QLabel("HELLO1");
    lbl->setObjectName("label1");
    vl->addWidget(lbl);
    w = new QWidget;
    tab.addTab(w, "Tab2");
    vl = new QVBoxLayout(w);
    lbl = new QLabel("HELLO2");
    lbl->setAttribute(BobUI::WA_NativeWindow);
    lbl->setObjectName("label2");
    vl->addWidget(lbl);

    tab.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&tab));

    QWindow *win = tab.windowHandle();
    win->destroy();
    QWindowPrivate *p = bobui_window_private(win);
    p->create(true);
    win->show();

    tab.setCurrentIndex(1);
}

class ResizeWidget : public QWidget
{
Q_OBJECT
public:
    ResizeWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , resizeCount(0)
    { }

    int resizeCount;

protected:
    void resizeEvent(QResizeEvent *) override
    {
        resizeCount++;
    }
};

void tst_QWidget_window::tst_resize_count()
{
    {
        ResizeWidget resize;
        resize.setWindowFlags(BobUI::X11BypassWindowManagerHint);
        resize.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&resize));
        QCOMPARE(resize.resizeCount, 1);
        resize.resizeCount = 0;
        QSize size = resize.size();
        size.rwidth() += 10;
        resize.resize(size);
        QGuiApplication::sync();
        BOBUIRY_COMPARE(resize.resizeCount, 1);

        resize.resizeCount = 0;

        ResizeWidget child(&resize);
        child.resize(m_testWidgetSize);
        child.winId();
        child.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&child));
        QGuiApplication::sync();
        BOBUIRY_COMPARE(child.resizeCount, 1);
        child.resizeCount = 0;
        size = child.size();
        size.rwidth() += 10;
        child.resize(size);
        QGuiApplication::sync();
        QCOMPARE(resize.resizeCount, 0);
        QCOMPARE(child.resizeCount, 1);
    }
    {
        ResizeWidget parent;
        parent.setWindowFlag(BobUI::X11BypassWindowManagerHint);
        ResizeWidget child(&parent);
        child.resize(m_testWidgetSize);
        child.winId();
        parent.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&parent));
        parent.resizeCount = 0;
        QGuiApplication::sync();
        BOBUIRY_COMPARE(child.resizeCount, 1);
        child.resizeCount = 0;
        QSize size = child.size();
        size.rwidth() += 10;
        child.resize(size);
        QGuiApplication::sync();
        QCOMPARE(parent.resizeCount, 0);
        QCOMPARE(child.resizeCount, 1);
    }

}

/*!
    This test verifies that windows get a balanced number of show
    and hide events, no matter how the window was closed.
*/
void tst_QWidget_window::tst_showhide_count()
{
    class EventSpy : public QObject
    {
    public:
        EventSpy()
        {
            QApplication::instance()->installEventFilter(this);
        }

        int takeCount(QWidget *widget, QEvent::Type type) {
            const auto entry = Entry(widget, type);
            int count = counter[entry];
            counter[entry] = 0;
            return count;
        }
    protected:
        bool eventFilter(QObject *receiver, QEvent *event) override
        {
            if (QWidget *widget = qobject_cast<QWidget*>(receiver)) {
                const auto entry = Entry(widget, event->type());
                ++counter[entry];
                return false;
            }
            return QObject::eventFilter(receiver, event);
        }
    private:
        using Entry = std::pair<QWidget*, QEvent::Type>;
        QHash<Entry, int> counter;
    };

    EventSpy spy;

    QWidget w1;
    w1.setGeometry(100, 100, 200, 200);

    w1.show();
    QCOMPARE(spy.takeCount(&w1, QEvent::Show), 1);
    w1.hide();
    QCOMPARE(spy.takeCount(&w1, QEvent::Hide), 1);
    w1.close();
    QCOMPARE(spy.takeCount(&w1, QEvent::Close), 1);
    w1.show();
    QCOMPARE(spy.takeCount(&w1, QEvent::Show), 1);
    w1.close();
    QCOMPARE(spy.takeCount(&w1, QEvent::Hide), 1);
    QCOMPARE(spy.takeCount(&w1, QEvent::Close), 1);

    w1.show();
    QWidget *popup = new QWidget(&w1, BobUI::Popup);
    popup->setGeometry(120, 120, 30, 30);
    popup->show();
    popup->close();
    QCOMPARE(spy.takeCount(popup, QEvent::Show), 1);
    QCOMPARE(spy.takeCount(popup, QEvent::Hide), 1);
    QCOMPARE(spy.takeCount(popup, QEvent::Close), 1);

    popup->show();

    // clicking outside the popup should close the popup
    BOBUIest::mousePress(popup->window(), BobUI::LeftButton, {}, QPoint(-10, -10));

    QCOMPARE(spy.takeCount(popup, QEvent::Show), 1);
    QCOMPARE(spy.takeCount(popup, QEvent::Hide), 1);
    QCOMPARE(spy.takeCount(popup, QEvent::Close), 1);
}


class MoveWidget : public QWidget
{
Q_OBJECT
public:
    MoveWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , moveCount(0)
    { }

    void moveEvent(QMoveEvent *) override
    {
        moveCount++;
    }

    int moveCount;
};

void tst_QWidget_window::tst_move_count()
{
    MoveWidget move;
    move.move(500,500);
    move.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&move));
    BOBUIRY_VERIFY(move.moveCount >= 1);
    move.moveCount = 0;

    move.move(220,250);
    BOBUIRY_VERIFY(move.moveCount >= 1);
}

class EventFilter : public QObject
{
public:
    int eventCount;

    EventFilter()
        : QObject(),
          eventCount(0)
    {
    }

    static QEvent::Type filterEventType()
    {
        static int type = QEvent::registerEventType();
        return static_cast<QEvent::Type>(type);
    }

protected:
    bool eventFilter(QObject *o, QEvent *e) override
    {
        if (e->type() == filterEventType())
            ++eventCount;

        return QObject::eventFilter(o, e);
    }
};

void tst_QWidget_window::tst_eventfilter_on_toplevel()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget w;
    EventFilter filter;
    w.installEventFilter(&filter);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));
    QVERIFY(w.isWindow());
    QCOMPARE(filter.eventCount, 0);

    // send an event not handled in a special way by QWidgetWindow::event,
    // and check that it's received by the event filter
    QCoreApplication::postEvent(w.windowHandle(), new QEvent(EventFilter::filterEventType()));
    BOBUIRY_COMPARE(filter.eventCount, 1);
}

class ApplicationStateSaver
{
public:
    ApplicationStateSaver()
    {
        QApplication::setAttribute(BobUI::AA_NativeWindows, true);
        QApplication::setQuitOnLastWindowClosed(false);
    }

    ~ApplicationStateSaver()
    {
        QApplication::setAttribute(BobUI::AA_NativeWindows, false);
        QApplication::setQuitOnLastWindowClosed(true);
    }
};

void tst_QWidget_window::BOBUIBUG_50561_QCocoaBackingStore_paintDevice_crash()
{
    // Keep application state clean if testcase fails
    ApplicationStateSaver as;

    QMainWindow w;
    w.setMinimumWidth(m_testWidgetSize.width());
    w.addToolBar(new BOBUIoolBar(&w));
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    // Simulate window system close
    QCloseEvent *e = new QCloseEvent;
    e->accept();
    qApp->postEvent(w.windowHandle(), e);
    qApp->processEvents();

    // Show again
    w.show();
    qApp->processEvents();

    // No crash, all good.
    // Wrap up and leave
    w.close();
}

void tst_QWidget_window::setWindowState_data()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QString platformName = QGuiApplication::platformName().toLower();

    BOBUIest::addColumn<BobUI::WindowStates>("state");
    BOBUIest::newRow("nostate") << BobUI::WindowStates();
    BOBUIest::newRow("maximized") << BobUI::WindowStates(BobUI::WindowMaximized);
    BOBUIest::newRow("minimized") << BobUI::WindowStates(BobUI::WindowMinimized);
    BOBUIest::newRow("fullscreen") << BobUI::WindowStates(BobUI::WindowFullScreen);

    if (platformName != "xcb" && platformName != "windows" && !platformName.startsWith("wayland")
        && platformName != "offscreen")
        return; // Combination of states is not preserved on all platforms.
    if (platformName == "xcb" && qgetenv("XDG_CURRENT_DESKTOP") != "KDE"
        && qgetenv("XDG_CURRENT_DESKTOP") != "Unity")
        return; // Not all window managers support state combinations.

    BOBUIest::newRow("BobUI::WindowMaximized|BobUI::WindowMinimized")
        << (BobUI::WindowMaximized | BobUI::WindowMinimized);
    BOBUIest::newRow("BobUI::WindowFullScreen|BobUI::WindowMinimized")
        << (BobUI::WindowFullScreen | BobUI::WindowMinimized);
    BOBUIest::newRow("BobUI::WindowMaximized|BobUI::WindowFullScreen")
        << (BobUI::WindowMaximized | BobUI::WindowFullScreen);
    BOBUIest::newRow("BobUI::WindowMaximized|BobUI::WindowFullScreen|BobUI::WindowMinimized")
        << (BobUI::WindowMaximized | BobUI::WindowFullScreen | BobUI::WindowMinimized);
}

void tst_QWidget_window::setWindowState()
{
    QFETCH(BobUI::WindowStates, state);

    // This tests make sure that the states are preserved when the window is shown.

    QWidget w;
    w.setWindowState(state);
    QCOMPARE(w.windowState(), state);
    w.show();
    QCOMPARE(w.windowState(), state);
    QCOMPARE(w.windowHandle()->windowStates(), state);
    if (!(state & BobUI::WindowMinimized))
        QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    BOBUIRY_COMPARE(w.windowState(), state);
    QCOMPARE(w.windowHandle()->windowStates(), state);

    // Minimizing keeps other states
    w.showMinimized();
    QCOMPARE(w.windowState(), state | BobUI::WindowMinimized);
    BOBUIest::qWait(200);
    QCOMPARE(w.windowState(), state | BobUI::WindowMinimized);
    QCOMPARE(w.windowHandle()->windowStates(), state | BobUI::WindowMinimized);
}

void tst_QWidget_window::nativeShow()
{
    // Verify that a native widget can be shown using the QWindow::setVisible() API
    QWidget w;
    w.winId();
    w.windowHandle()->setVisible(true);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    QVERIFY(w.isVisible());

    // ... and that we can hide it
    w.windowHandle()->setVisible(false);
    BOBUIRY_VERIFY(!w.isVisible());
}

class ResizedOnShowEventWidget : public QWidget
{
public:
    void showEvent(QShowEvent *) override
    {
        const auto *primaryScreen = QApplication::primaryScreen();
        auto newSize = primaryScreen->availableGeometry().size() / 4;
        if (newSize == geometry().size())
            newSize -= QSize(10, 10);
        resize(newSize);
    }
};

void tst_QWidget_window::BOBUIBUG_56277_resize_on_showEvent()
{
    const auto platformName = QGuiApplication::platformName().toLower();
    if (platformName != "cocoa" && platformName != "windows")
        QSKIP("This can only be consistently tested on desktop platforms with well-known behavior.");

    ResizedOnShowEventWidget w;
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    const auto *screen = w.windowHandle()->screen();
    const auto geometry = w.geometry();
    const int frameHeight = geometry.top() - w.frameGeometry().top();
    const int topmostY = screen->availableGeometry().top() + frameHeight;
    QVERIFY(geometry.top() > topmostY || geometry.left() > screen->availableGeometry().left());
}

void tst_QWidget_window::mouseMoveWithPopup_data()
{
    BOBUIest::addColumn<BobUI::WindowType>("windowType");

    BOBUIest::addRow("Dialog") << BobUI::Dialog;
    BOBUIest::addRow("Popup") << BobUI::Popup;
}

void tst_QWidget_window::mouseMoveWithPopup()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: Skip this test, see also BOBUIBUG-107154");

    QFETCH(BobUI::WindowType, windowType);

    class Window : public QWidget
    {
    public:
        Window(QWidget *parent = nullptr, BobUI::WindowFlags flags = {})
        : QWidget(parent, flags|BobUI::CustomizeWindowHint|BobUI::FramelessWindowHint)
        {}

        QSize sizeHint() const override
        {
            if (parent())
                return QSize(150, 100);
            return QSize(250, 250);
        }

        Window *popup = nullptr;
        BobUI::WindowType type = BobUI::Popup;
        int mousePressCount = 0;
        int mouseMoveCount = 0;
        int mouseReleaseCount = 0;
        void resetCounters()
        {
            mousePressCount = 0;
            mouseMoveCount = 0;
            mouseReleaseCount = 0;
        }
    protected:
        void mousePressEvent(QMouseEvent *event) override
        {
            ++mousePressCount;

            if (event->button() == BobUI::RightButton) {
                if (!popup)
                    popup = new Window(this, type);
                popup->move(event->globalPosition().toPoint());
                popup->show();
                if (!BOBUIest::qWaitForWindowExposed(popup)) {
                    delete popup;
                    popup = nullptr;
                    QSKIP("Failed to expose popup window!");
                }
            } else {
                QWidget::mousePressEvent(event);
            }
        }
        void mouseReleaseEvent(QMouseEvent *event) override
        {
            ++mouseReleaseCount;
            QWidget::mouseReleaseEvent(event);
        }
        void mouseMoveEvent(QMouseEvent *event) override
        {
            ++mouseMoveCount;
            QWidget::mouseMoveEvent(event);
        }
    };
    Window topLevel;
    topLevel.setObjectName("topLevel");
    topLevel.type = windowType;
    topLevel.show();
    if (!BOBUIest::qWaitForWindowExposed(&topLevel))
        QSKIP("Failed to expose window!");

    QCOMPARE(QApplication::activePopupWidget(), nullptr);
    BOBUIRY_COMPARE(QApplication::activeWindow(), &topLevel);

    QPoint mousePos = topLevel.geometry().center();
    QWindow *window = nullptr;
    BobUI::MouseButtons buttons = {};
    auto mouseAction = [&](BobUI::MouseButton button, QPoint offset = {}) -> QEvent::Type
    {
        QEvent::Type type;
        if (offset != QPoint()) {
            type = QEvent::MouseMove;
        } else if (buttons & button) {
            type = QEvent::MouseButtonRelease;
            buttons &= ~button;
        } else {
            Q_ASSERT(button != BobUI::NoButton);
            type = QEvent::MouseButtonPress;
            buttons |= button;
            window = QApplication::activeWindow()->windowHandle();
        }

        mousePos += offset;

        if (!window)
            return QEvent::None;

        bool result = QWindowSystemInterface::handleMouseEvent(window, window->mapFromGlobal(mousePos),
                                                               mousePos, buttons, button, type);
        QCoreApplication::processEvents();
        if (type == QEvent::MouseButtonRelease && buttons == BobUI::NoButton)
            window = nullptr;

        if (!result)
            return QEvent::None;
        return type;
    };

    QCOMPARE(mouseAction(BobUI::RightButton), QEvent::MouseButtonPress);
    QCOMPARE(topLevel.mousePressCount, 1);
    QVERIFY(topLevel.popup);
    QCOMPARE(topLevel.popup->mousePressCount, 0);
    topLevel.popup->setObjectName(windowType == BobUI::Popup ? "Popup" : "Dialog");
    QCOMPARE(QApplication::activePopupWidget(), windowType == BobUI::Popup ? topLevel.popup : nullptr);
    // if popup, then popup gets the mouse move even though it didn't get any press
    QCOMPARE(mouseAction(BobUI::NoButton, QPoint(10, 10)), QEvent::MouseMove);
    QCOMPARE(topLevel.mouseMoveCount, windowType == BobUI::Popup ? 0 : 1);
    QCOMPARE(topLevel.popup->mouseMoveCount, windowType == BobUI::Popup ? 1 : 0);
    // if popup, then popup gets the release even though it didn't get any press
    QCOMPARE(mouseAction(BobUI::RightButton), QEvent::MouseButtonRelease);
    QCOMPARE(topLevel.mouseReleaseCount, windowType == BobUI::Popup ? 0 : 1);
    QCOMPARE(topLevel.popup->mouseReleaseCount, windowType == BobUI::Popup ? 1 : 0);

    Q_ASSERT(buttons == BobUI::NoButton);
    topLevel.resetCounters();
    topLevel.popup->resetCounters();

    BOBUIRY_VERIFY(QApplication::activeWindow() == topLevel.popup
                || QApplication::activePopupWidget() == topLevel.popup);

    // nested popup, same procedure
    QCOMPARE(mouseAction(BobUI::RightButton), QEvent::MouseButtonPress);
    QVERIFY(topLevel.popup);
    QCOMPARE(topLevel.popup->mousePressCount, 1);
    QVERIFY(topLevel.popup->popup);
    topLevel.popup->popup->setObjectName("NestedPopup");
    QCOMPARE(QApplication::activePopupWidget(), topLevel.popup->popup);
    QCOMPARE(topLevel.popup->popup->mousePressCount, 0);

    // nested popup is always a popup and grabs the mouse, so first popup gets nothing
    QCOMPARE(mouseAction(BobUI::NoButton, QPoint(10, 10)), QEvent::MouseMove);
    QCOMPARE(topLevel.popup->mouseMoveCount, 0);
    QCOMPARE(topLevel.popup->popup->mouseMoveCount, 1);

    // nested popup gets the release, as before
    QCOMPARE(mouseAction(BobUI::RightButton), QEvent::MouseButtonRelease);
    QCOMPARE(topLevel.popup->mouseReleaseCount, 0);
    QCOMPARE(topLevel.popup->popup->mouseReleaseCount, 1);

    Q_ASSERT(buttons == BobUI::NoButton);

    // move mouse back into first popup
    mouseAction({}, QPoint(-15, -15));
    QVERIFY(!topLevel.popup->popup->geometry().contains(mousePos));
    QVERIFY(topLevel.popup->geometry().contains(mousePos));

    topLevel.popup->resetCounters();
    topLevel.popup->popup->resetCounters();

    // closing the nested popup by clicking into the first popup/dialog; the nested popup gets the press
    QCOMPARE(mouseAction(BobUI::LeftButton), QEvent::MouseButtonPress);
    QCOMPARE(topLevel.popup->popup->mousePressCount, 1);
    QVERIFY(!topLevel.popup->popup->isVisible());
    QCOMPARE(QApplication::activePopupWidget(), windowType == BobUI::Popup ? topLevel.popup : nullptr);
    QCOMPARE(QApplication::activeWindow(), windowType == BobUI::Popup ? &topLevel : topLevel.popup);

    // the move event following a press that closed the active popup should NOT be delivered to the first popup
    QCOMPARE(mouseAction({}, QPoint(-10, -10)), QEvent::MouseMove);
    // dialogs might or might not get the event - platform specific behavior in BobUI 5
    if (topLevel.popup->mouseMoveCount != 0)
        QEXPECT_FAIL("Dialog", "Platform specific behavior", Continue);
    QCOMPARE(topLevel.popup->mouseMoveCount, 0);
    QCOMPARE(topLevel.popup->popup->mouseMoveCount, 0);

    // but the release event will still be delivered to the first popup - dialogs might not get it
    QCOMPARE(mouseAction(BobUI::LeftButton), QEvent::MouseButtonRelease);
    if (topLevel.popup->mouseReleaseCount != 1)
        QEXPECT_FAIL("Dialog", "Platform specific behavior", Continue);
    QCOMPARE(topLevel.popup->mouseReleaseCount, 1);
}

struct ShowHideEntry {
    QEvent::Type action;
    BobUI::WindowType target;
    using List = QList<ShowHideEntry>;
};

void tst_QWidget_window::showHideWindowHandle_data()
{
    BOBUIest::addColumn<ShowHideEntry::List>("entries");

    BOBUIest::addRow("show/hide widget") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Hide, BobUI::Widget }
    };
    BOBUIest::addRow("show/hide window") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Window }, { QEvent::Hide, BobUI::Window }
    };
    BOBUIest::addRow("show widget, hide window") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Hide, BobUI::Window }
    };
    BOBUIest::addRow("show window, hide widget") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Window }, { QEvent::Hide, BobUI::Widget }
    };
    BOBUIest::addRow("show/hide widget, then show window, hide widget") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Hide, BobUI::Widget },
        { QEvent::Show, BobUI::Window }, { QEvent::Hide, BobUI::Widget }
    };
    BOBUIest::addRow("show widget, close widget, show widget") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Close, BobUI::Widget }, { QEvent::Show, BobUI::Widget }
    };
    BOBUIest::addRow("show widget, close widget, show window") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Close, BobUI::Widget }, { QEvent::Show, BobUI::Window }
    };
    BOBUIest::addRow("show widget, close window, show widget") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Close, BobUI::Window }, { QEvent::Show, BobUI::Widget }
    };
    BOBUIest::addRow("show widget, close window, show window") << ShowHideEntry::List{
        { QEvent::Show, BobUI::Widget }, { QEvent::Close, BobUI::Window }, { QEvent::Show, BobUI::Window }
    };
}

void tst_QWidget_window::showHideWindowHandle()
{
    QWidget parent;
    parent.setObjectName("Parent");
    QCOMPARE(parent.isVisible(), false);
    QCOMPARE(parent.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
    QCOMPARE(parent.testAttribute(BobUI::WA_WState_Visible), false);
    QCOMPARE(parent.testAttribute(BobUI::WA_WState_Hidden), true);

    QWidget child;
    child.setObjectName("Child");
    QCOMPARE(child.isVisible(), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_Visible), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_Hidden), true);

    child.setParent(&parent);
    QCOMPARE(child.isVisible(), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_Visible), false);
    QCOMPARE(child.testAttribute(BobUI::WA_WState_Hidden), false);

    QFETCH(QList<ShowHideEntry>, entries);
    for (const auto entry : entries) {

        if (entry.action == QEvent::Show) {
            if (entry.target == BobUI::Window && !parent.windowHandle()) {
                parent.setAttribute(BobUI::WA_NativeWindow);
                QVERIFY(parent.windowHandle());

                QCOMPARE(parent.isVisible(), false);
                QCOMPARE(parent.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
                QCOMPARE(parent.testAttribute(BobUI::WA_WState_Visible), false);
                QCOMPARE(parent.testAttribute(BobUI::WA_WState_Hidden), true);
            }

            bool wasExplicitShowHide = parent.testAttribute(BobUI::WA_WState_ExplicitShowHide);

            if (entry.target == BobUI::Widget)
                parent.show();
            else
                parent.windowHandle()->show();

            QVERIFY(BOBUIest::qWaitForWindowActive(&parent));

            QCOMPARE(parent.isVisible(), true);
            QVERIFY(parent.windowHandle());
            QCOMPARE(parent.windowHandle()->isVisible(), true);

            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Visible), true);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Hidden), false);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_ExplicitShowHide),
                entry.target == BobUI::Widget || wasExplicitShowHide);

            QCOMPARE(child.isVisible(), true);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Visible), true);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Hidden), false);

        } else if (entry.action == QEvent::Hide) {

            bool wasExplicitShowHide = parent.testAttribute(BobUI::WA_WState_ExplicitShowHide);

            if (entry.target == BobUI::Widget)
                parent.hide();
            else
                parent.windowHandle()->hide();

            QCOMPARE(parent.isVisible(), false);
            QVERIFY(parent.windowHandle());
            QCOMPARE(parent.windowHandle()->isVisible(), false);

            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Visible), false);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Hidden), true);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_ExplicitShowHide),
                entry.target == BobUI::Widget || wasExplicitShowHide);

            QCOMPARE(child.isVisible(), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Visible), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Hidden), false);

        } else if (entry.action == QEvent::Close) {

            bool wasExplicitShowHide = parent.testAttribute(BobUI::WA_WState_ExplicitShowHide);

            if (entry.target == BobUI::Widget)
                parent.close();
            else
                parent.windowHandle()->close();

            QCOMPARE(parent.isVisible(), false);
            QVERIFY(parent.windowHandle());
            QCOMPARE(parent.windowHandle()->isVisible(), false);

            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Visible), false);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_Hidden), true);
            QCOMPARE(parent.testAttribute(BobUI::WA_WState_ExplicitShowHide),
                entry.target == BobUI::Widget || wasExplicitShowHide);

            QCOMPARE(child.isVisible(), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_ExplicitShowHide), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Visible), false);
            QCOMPARE(child.testAttribute(BobUI::WA_WState_Hidden), false);
        }
    }
}

void tst_QWidget_window::resetFocusObjectOnDestruction()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    QSignalSpy focusObjectChangedSpy(qApp, &QGuiApplication::focusObjectChanged);

    // single top level widget that has focus
    std::unique_ptr<QWidget> widget(new QWidget);
    widget->setObjectName("Widget 1");
    widget->setFocus();
    widget->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(widget.get()));

    int activeCount = focusObjectChangedSpy.size();
    widget.reset();
    QVERIFY(focusObjectChangedSpy.size() > activeCount);
    QCOMPARE(focusObjectChangedSpy.last().last().value<QObject*>(), nullptr);
    focusObjectChangedSpy.clear();

    // top level widget with focused child
    widget.reset(new QWidget);
    widget->setObjectName("Widget 2");
    QWidget *child = new QWidget(widget.get());
    child->setObjectName("Child widget");
    child->setFocus();
    widget->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(widget.get()));

    activeCount = focusObjectChangedSpy.size();
    widget.reset();
    // we might get more than one signal emission
    QVERIFY(focusObjectChangedSpy.size() > activeCount);
    QCOMPARE(focusObjectChangedSpy.last().last().value<QObject*>(), nullptr);
}

class CreateDestroyWidget : public QWidget
{
public:
    using QWidget::create;
    using QWidget::destroy;
};

void tst_QWidget_window::cleanupOnDestruction()
{
    CreateDestroyWidget widget;
    QWidget child(&widget);

    QWidget grandChild(&child);
    // Ensure there's not a 1:1 native window hierarhcy that we could
    // recurse during QWidget::destroy(), triggering the issue that
    // we were failing to clean up when not destroyed via QWidget.
    grandChild.setAttribute(BobUI::WA_DontCreateNativeAncestors);
    grandChild.winId();

    widget.destroy();
    widget.create();

    widget.show();
}

BOBUIEST_MAIN(tst_QWidget_window)
#include "tst_qwidget_window.moc"
