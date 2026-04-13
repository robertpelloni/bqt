// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qapplication.h>
#include <qlineedit.h>
#include <qmenu.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qevent.h>
#include <qlineedit.h>
#include <QBoxLayout>
#include <QSysInfo>

#include <qpa/qplatformintegration.h>
#include <private/qguiapplication_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

BOBUI_FORWARD_DECLARE_CLASS(QWidget)

class FocusLineEdit : public QLineEdit
{
public:
    FocusLineEdit(QWidget *parent = nullptr, const char *name = nullptr ) : QLineEdit(name, parent) {}
    int focusInEventReason;
    int focusOutEventReason;
    bool focusInEventRecieved;
    bool focusInEventGotFocus;
    bool focusOutEventRecieved;
    bool focusOutEventLostFocus;
protected:
    virtual void keyPressEvent( QKeyEvent *e ) override
    {
//        qDebug( QString("keyPressEvent: %1").arg(e->key()) );
        QLineEdit::keyPressEvent( e );
    }
    void focusInEvent( QFocusEvent* e ) override
    {
        QLineEdit::focusInEvent( e );
        focusInEventReason = e->reason();
        focusInEventGotFocus = e->gotFocus();
        focusInEventRecieved = true;
    }
    void focusOutEvent( QFocusEvent* e ) override
    {
        QLineEdit::focusOutEvent( e );
        focusOutEventReason = e->reason();
        focusOutEventLostFocus = !e->gotFocus();
        focusOutEventRecieved = true;
    }
};

class tst_QFocusEvent : public QObject
{
    Q_OBJECT

public:
    void initWidget();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
    void checkReason_Tab();
    void checkReason_ShiftTab();
    void checkReason_BackTab();
    void checkReason_Popup();
    void checkReason_focusWidget();
#if BOBUI_CONFIG(shortcut)
    void checkReason_Shortcut();
#endif
    void checkReason_ActiveWindow();

private:
    QWidget* testFocusWidget = nullptr;
    FocusLineEdit* childFocusWidgetOne;
    FocusLineEdit* childFocusWidgetTwo;
};

void tst_QFocusEvent::initTestCase()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported on this platform.");

    testFocusWidget = new QWidget( nullptr );
    childFocusWidgetOne = new FocusLineEdit( testFocusWidget );
    childFocusWidgetOne->setGeometry( 10, 10, 180, 20 );
    childFocusWidgetTwo = new FocusLineEdit( testFocusWidget );
    childFocusWidgetTwo->setGeometry( 10, 50, 180, 20 );

    //qApp->setMainWidget( testFocusWidget ); BobUI4
    testFocusWidget->resize( 200,100 );
    testFocusWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testFocusWidget));
// Applications don't get focus when launched from the command line on Mac.
#ifdef Q_OS_MAC
    testFocusWidget->raise();
#endif
}

void tst_QFocusEvent::cleanupTestCase()
{
    delete testFocusWidget;
}

void tst_QFocusEvent::cleanup()
{
    childFocusWidgetTwo->setGeometry( 10, 50, 180, 20 );
}

void tst_QFocusEvent::initWidget()
{
    // On X11 we have to ensure the event was processed before doing any checking, on Windows
    // this is processed straight away.
    QApplicationPrivate::setActiveWindow(testFocusWidget);
    childFocusWidgetOne->setFocus(); // The first lineedit should have focus
    QVERIFY(BOBUIest::qWaitForWindowActive(testFocusWidget));
    BOBUIRY_VERIFY(childFocusWidgetOne->hasFocus());

    childFocusWidgetOne->focusInEventRecieved = false;
    childFocusWidgetOne->focusInEventGotFocus = false;
    childFocusWidgetOne->focusInEventReason = -1;
    childFocusWidgetOne->focusOutEventRecieved = false;
    childFocusWidgetOne->focusOutEventLostFocus = false;
    childFocusWidgetOne->focusOutEventReason = -1;
    childFocusWidgetTwo->focusInEventRecieved = false;
    childFocusWidgetTwo->focusInEventGotFocus = false;
    childFocusWidgetTwo->focusInEventReason = -1;
    childFocusWidgetTwo->focusOutEventRecieved = false;
    childFocusWidgetTwo->focusOutEventLostFocus = false;
    childFocusWidgetTwo->focusOutEventReason = -1;
}

void tst_QFocusEvent::checkReason_Tab()
{
    initWidget();

    // Now test the tab key
    BOBUIest::keyClick( childFocusWidgetOne, BobUI::Key_Tab );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int) BobUI::TabFocusReason );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int) BobUI::TabFocusReason );
}

void tst_QFocusEvent::checkReason_ShiftTab()
{
    initWidget();

    // Now test the shift + tab key
    BOBUIest::keyClick( childFocusWidgetOne, BobUI::Key_Tab, BobUI::ShiftModifier );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)BobUI::BacktabFocusReason );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int)BobUI::BacktabFocusReason );

}

/*!
    In this test we verify that the BobUI::KeyBacktab key is handled in a qfocusevent
*/
void tst_QFocusEvent::checkReason_BackTab()
{
#ifdef Q_OS_WIN32 // key is not supported on Windows
    QSKIP( "Backtab is not supported on Windows");
#else
    initWidget();
    QVERIFY( childFocusWidgetOne->hasFocus() );

    // Now test the backtab key
    BOBUIest::keyClick( childFocusWidgetOne, BobUI::Key_Backtab );

    BOBUIRY_VERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, int(BobUI::BacktabFocusReason) );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, int(BobUI::BacktabFocusReason) );
#endif
}

void tst_QFocusEvent::checkReason_Popup()
{
    initWidget();

    // Now test the popup reason
    QMenu* popupMenu = new QMenu( testFocusWidget );
    popupMenu->addMenu( "Test" );
    popupMenu->popup( QPoint(0,0) );

    BOBUIRY_VERIFY(childFocusWidgetOne->focusOutEventLostFocus);

    BOBUIRY_VERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusInEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, int(BobUI::PopupFocusReason));

    popupMenu->hide();

    QVERIFY(childFocusWidgetOne->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusInEventGotFocus);

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusInEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );
}

#ifdef Q_OS_MAC
BOBUI_BEGIN_NAMESPACE
    extern void bobui_set_sequence_auto_mnemonic(bool);
BOBUI_END_NAMESPACE
#endif

#if BOBUI_CONFIG(shortcut)

void tst_QFocusEvent::checkReason_Shortcut()
{
    initWidget();
#ifdef Q_OS_MAC
    bobui_set_sequence_auto_mnemonic(true);
#endif
    QLabel* label = new QLabel( "&Test", testFocusWidget );
    label->setBuddy( childFocusWidgetTwo );
    label->setGeometry( 10, 50, 90, 20 );
    childFocusWidgetTwo->setGeometry( 105, 50, 95, 20 );
    label->show();

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( !childFocusWidgetTwo->hasFocus() );

    BOBUIest::keyClick( label, BobUI::Key_T, BobUI::AltModifier );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)BobUI::ShortcutFocusReason );
    QVERIFY( childFocusWidgetTwo->focusInEventRecieved );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int)BobUI::ShortcutFocusReason );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );

    label->hide();
    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QVERIFY( !childFocusWidgetOne->hasFocus() );
#ifdef Q_OS_MAC
    bobui_set_sequence_auto_mnemonic(false);
#endif
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QFocusEvent::checkReason_focusWidget()
{
    // This test checks that a widget doesn't loose
    // its focuswidget just because the focuswidget loses focus.
    QWidget window1;
    QWidget frame1;
    QWidget frame2;
    QLineEdit edit1;
    QLineEdit edit2;

    QVBoxLayout outerLayout;
    outerLayout.addWidget(&frame1);
    outerLayout.addWidget(&frame2);
    window1.setLayout(&outerLayout);

    QVBoxLayout leftLayout;
    QVBoxLayout rightLayout;
    leftLayout.addWidget(&edit1);
    rightLayout.addWidget(&edit2);
    frame1.setLayout(&leftLayout);
    frame2.setLayout(&rightLayout);
    window1.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window1));

    edit1.setFocus();
    BOBUIRY_VERIFY(edit1.hasFocus());
    edit2.setFocus();

    QVERIFY(frame1.focusWidget() != nullptr);
    QVERIFY(frame2.focusWidget() != nullptr);
}

void tst_QFocusEvent::checkReason_ActiveWindow()
{
    initWidget();

    QDialog* d = new QDialog( testFocusWidget );
    d->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(d));
    QVERIFY(BOBUIest::qWaitForWindowActive(d));

    BOBUIRY_VERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);

#if defined(Q_OS_WIN)
    if (QSysInfo::kernelVersion() == "10.0.15063") {
        // Activate window of testFocusWidget, focus in that window goes to childFocusWidgetOne
        qWarning("Windows 10 Creators Update (10.0.15063) requires explicit activateWindow()");
        testFocusWidget->activateWindow();
    }
#endif

    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)BobUI::ActiveWindowFocusReason);
    QVERIFY( !childFocusWidgetOne->hasFocus() );

    d->hide();

    if (!QGuiApplication::platformName().compare(QLatin1String("offscreen"), BobUI::CaseInsensitive)
            || !QGuiApplication::platformName().compare(QLatin1String("minimal"), BobUI::CaseInsensitive)
            || !QGuiApplication::platformName().compare(QLatin1String("cocoa"), BobUI::CaseInsensitive)) {
        // Activate window of testFocusWidget, focus in that window goes to childFocusWidgetOne
        qWarning("Platforms offscreen, minimal and macOS require explicit activateWindow()");
        testFocusWidget->activateWindow();
    }

    BOBUIRY_VERIFY(childFocusWidgetOne->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusInEventGotFocus);

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( childFocusWidgetOne->focusInEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusInEventReason, (int)BobUI::ActiveWindowFocusReason);

    const bool windowActivationReasonFail =
        QGuiApplication::platformName().toLower() == "minimal";

    struct Window : public QWindow
    {
        BobUI::FocusReason lastReason = BobUI::NoFocusReason;
    protected:
        void focusInEvent(QFocusEvent *event) override
        {
            lastReason = event->reason();
        }
        void focusOutEvent(QFocusEvent *event) override
        {
            lastReason = event->reason();
        }
    };

    Window window;
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    if (windowActivationReasonFail)
        QEXPECT_FAIL("", "Platform doesn't set window activation reason for QWindow", Continue);
    QCOMPARE(window.lastReason, BobUI::ActiveWindowFocusReason);
    window.lastReason = BobUI::NoFocusReason;

    Window window2;
    window2.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window2));

    if (windowActivationReasonFail)
        QEXPECT_FAIL("", "Platform doesn't set window activation reason for QWindow", Continue);
    QCOMPARE(window.lastReason, BobUI::ActiveWindowFocusReason);
}


BOBUIEST_MAIN(tst_QFocusEvent)
#include "tst_qfocusevent.moc"
