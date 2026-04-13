// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <BOBUIest>

#include "qnativeevents.h"
#include "nativeeventlist.h"
#include "expectedeventlist.h"

BOBUI_USE_NAMESPACE

// Unicode code points for the glyphs associated with these keys
// Defined by Carbon headers but not anywhere in Cocoa
static const int kControlUnicode = 0x2303;
static const int kCommandUnicode = 0x2318;

class tst_MacNativeEvents : public QObject
{
Q_OBJECT
private slots:
    void testMouseMoveLocation();
    void testPushButtonPressRelease();
    void testMouseLeftDoubleClick();
    void stressTestMouseLeftDoubleClick();
    void testMouseDragInside();
    void testMouseDragOutside();
    void testMouseDragToNonClientArea();
    void testDragWindow();
    void testMouseEnter();
    void testChildDialogInFrontOfModalParent();
//    void testChildWindowInFrontOfParentWindow();
//    void testChildToolWindowInFrontOfChildNormalWindow();
    void testChildWindowInFrontOfStaysOnTopParentWindow();
    void testKeyPressOnToplevel();
    void testModifierShift();
    void testModifierAlt();
    void testModifierCtrl();
    void testModifierCtrlWithDontSwapCtrlAndMeta();
};

void tst_MacNativeEvents::testMouseMoveLocation()
{
    QWidget w;
    w.setMouseTracking(true);
    w.show();
    QPoint p = w.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseMoveEvent(p, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(p), p, BobUI::NoButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testPushButtonPressRelease()
{
    // Check that a native mouse press and release generates the
    // same qevents on a pushbutton:
    QPushButton w("click me");
    w.show();
    QPoint p = w.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testMouseLeftDoubleClick()
{
    // Check that a native double click makes
    // the test widget receive a press-release-click-release:
    QWidget w;
    w.show();
    QPoint p = w.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 0, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 2, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonDblClick, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::stressTestMouseLeftDoubleClick()
{
    // Check that multiple, fast, double clicks makes
    // the test widget receive correct click events
    QWidget w;
    w.show();
    QPoint p = w.geometry().center();

    NativeEventList native;
    ExpectedEventList expected(&w);

    for (int i=0; i<10; ++i){
        native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 1, BobUI::NoModifier));
        native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 0, BobUI::NoModifier));
        native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 2, BobUI::NoModifier));
        native.append(new QNativeMouseButtonEvent(p, BobUI::LeftButton, 0, BobUI::NoModifier));

        expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
        expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));
        expected.append(new QMouseEvent(QEvent::MouseButtonDblClick, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
        expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p), p, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));
    }

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testMouseDragInside()
{
    // Check that a mouse drag inside a widget
    // will cause press-move-release events to be delivered
    QWidget w;
    w.show();
    QPoint p1 = w.geometry().center();
    QPoint p2 = p1 - QPoint(10, 0);
    QPoint p3 = p1 - QPoint(20, 0);
    QPoint p4 = p1 - QPoint(30, 0);

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(p1, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(p2, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(p3, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(p4, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(p1), p1, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(p2), p2, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(p3), p3, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(p4), p4, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testMouseDragOutside()
{
    // Check that if we drag the mouse from inside the
    // widget, and release it outside, we still get mouse move
    // and release events when the mouse is outside the widget.
    QWidget w;
    w.show();
    QPoint inside1 = w.geometry().center();
    QPoint inside2 = inside1 - QPoint(10, 0);
    QPoint outside1 = w.geometry().topLeft() - QPoint(50, 0);
    QPoint outside2 = outside1 - QPoint(10, 0);

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(inside1, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(inside2, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(outside1, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(outside2, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(inside1), inside1, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(inside2), inside2, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(outside1), outside1, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(outside2), outside2, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testMouseDragToNonClientArea()
{
    // Check that if we drag the mouse from inside the
    // widget, and release it on the title bar, we still get mouse move
    // and release events when the mouse is on the title bar
    QWidget w;
    w.show();
    QPoint inside1 = w.geometry().center();
    QPoint inside2 = inside1 - QPoint(10, 0);
    QPoint titlebar1 = w.geometry().topLeft() - QPoint(-100, 10);
    QPoint titlebar2 = titlebar1 - QPoint(10, 0);

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(inside1, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(inside2, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(titlebar1, BobUI::LeftButton, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(titlebar2, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::MouseButtonPress, w.mapFromGlobal(inside1), inside1, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(inside2), inside2, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(titlebar1), titlebar1, BobUI::NoButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::MouseButtonRelease, w.mapFromGlobal(titlebar2), titlebar2, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testDragWindow()
{
    // Check that if we drag the mouse from inside the
    // widgets title bar, we get a move event on the window
    QWidget w;
    w.show();
    QPoint titlebar = w.geometry().topLeft() - QPoint(-100, 10);
    QPoint moveTo = titlebar + QPoint(100, 0);

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(titlebar, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseDragEvent(moveTo, BobUI::LeftButton, BobUI::NoModifier));
    native.append(500, new QNativeMouseButtonEvent(moveTo, BobUI::LeftButton, 0, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QMouseEvent(QEvent::NonClientAreaMouseButtonPress, w.mapFromGlobal(titlebar), titlebar, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier));
    expected.append(new QMouseEvent(QEvent::NonClientAreaMouseButtonRelease, w.mapFromGlobal(titlebar), moveTo, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testMouseEnter()
{
    // When a mouse enters a widget, both a mouse enter events and a
    // mouse move event should be sent. Let's test this:
    QWidget w;
    w.setMouseTracking(true);
    w.show();
    QPoint outside = w.geometry().topLeft() - QPoint(50, 0);
    QPoint inside = w.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseMoveEvent(outside, BobUI::NoModifier));
    native.append(new QNativeMouseMoveEvent(inside, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QEvent(QEvent::Enter));
    expected.append(new QMouseEvent(QEvent::MouseMove, w.mapFromGlobal(inside), inside, BobUI::NoButton, BobUI::NoButton, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testChildDialogInFrontOfModalParent()
{
    QSKIP("Modal dialog causes later tests to fail, see BOBUIBUG-58474");

    // Test that a child dialog of a modal parent dialog is
    // in front of the parent, and active:
    QDialog parent;
    parent.setWindowModality(BobUI::ApplicationModal);
    QDialog child(&parent);
    QPushButton button("close", &child);
    connect(&button, SIGNAL(clicked()), &child, SLOT(close()));
    parent.show();
    child.show();
    QPoint inside = button.mapToGlobal(button.geometry().center());

    // Post a click on the button to close the child dialog:
    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(inside, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(inside, BobUI::LeftButton, 0, BobUI::NoModifier));

    native.play();
    BOBUIest::qWait(100);
    QVERIFY(!child.isVisible());
}

#if 0
// This test is disabled as of BobUI-4.7.4 because we cannot do it
// unless we use the Cocoa sub window API. But using that opens up
// a world of side effects that we cannot live with. So we rather
// not support child-on-top-of-parent instead.
void tst_MacNativeEvents::testChildWindowInFrontOfParentWindow()
{
    // Test that a child window always stacks in front of its parent window.
    // Do this by first click on the parent, then on the child window button.
    QWidget parent;
    QPushButton child("a button", &parent);
    child.setWindowFlags(BobUI::Window);
    connect(&child, SIGNAL(clicked()), &child, SLOT(close()));
    parent.show();
    child.show();

    QPoint parent_p = parent.geometry().bottomLeft() + QPoint(20, -20);
    QPoint child_p = child.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(parent_p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(parent_p, BobUI::LeftButton, 0, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(child_p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(child_p, BobUI::LeftButton, 0, BobUI::NoModifier));

    native.play();
    BOBUIest::qWait(100);
    QVERIFY(!child.isVisible());
}
#endif

/* This test can be enabled once setStackingOrder has been fixed in qwidget_mac.mm
void tst_MacNativeEvents::testChildToolWindowInFrontOfChildNormalWindow()
{
    // Test that a child tool window always stacks in front of normal sibling windows.
    // Do this by first click on the sibling, then on the tool window button.
    QWidget parent;
    QWidget normalChild(&parent, BobUI::Window);
    QPushButton toolChild("a button", &parent);
    toolChild.setWindowFlags(BobUI::Tool);
    connect(&toolChild, SIGNAL(clicked()), &toolChild, SLOT(close()));
    parent.show();
    normalChild.show();
    toolChild.show();

    QPoint normalChild_p = normalChild.geometry().bottomLeft() + QPoint(20, -20);
    QPoint toolChild_p = toolChild.geometry().center();

    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(normalChild_p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(normalChild_p, BobUI::LeftButton, 0, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(toolChild_p, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(toolChild_p, BobUI::LeftButton, 0, BobUI::NoModifier));

    native.play();
    BOBUIest::qWait(100);
    QVERIFY(!toolChild.isVisible());
}
*/
void tst_MacNativeEvents::testChildWindowInFrontOfStaysOnTopParentWindow()
{
    // Test that a child window stacks on top of a stays-on-top parent.
    QWidget parent(0, BobUI::WindowStaysOnTopHint);
    QPushButton button("close", &parent);
    button.setWindowFlags(BobUI::Window);
    connect(&button, SIGNAL(clicked()), &button, SLOT(close()));
    parent.show();
    button.show();
    QPoint inside = button.geometry().center();

    // Post a click on the button to close the child dialog:
    NativeEventList native;
    native.append(new QNativeMouseButtonEvent(inside, BobUI::LeftButton, 1, BobUI::NoModifier));
    native.append(new QNativeMouseButtonEvent(inside, BobUI::LeftButton, 0, BobUI::NoModifier));

    native.play();
    BOBUIest::qWait(100);
    QVERIFY(!button.isVisible());
}

void tst_MacNativeEvents::testKeyPressOnToplevel()
{
    // Check that we receive keyevents for
    // toplevel widgets. For leagacy reasons, and according to BobUI on
    // other platforms (carbon port + linux), we should get these events
    // even when the focus policy is set to BobUI::NoFocus when there is no
    // other focus widget on screen:
    QWidget w;
    w.show();

    NativeEventList native;
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, true, BobUI::NoModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, false, BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::NoModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_A, BobUI::NoModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testModifierShift()
{
    QWidget w;
    w.show();

    NativeEventList native;
    native.append(new QNativeModifierEvent(BobUI::ShiftModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, true, BobUI::ShiftModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, false, BobUI::ShiftModifier));
    native.append(new QNativeModifierEvent(BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_Shift, BobUI::NoModifier));
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::ShiftModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_A, BobUI::ShiftModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_Shift, BobUI::ShiftModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testModifierAlt()
{
    QWidget w;
    w.show();

    NativeEventList native;
    native.append(new QNativeModifierEvent(BobUI::AltModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, true, BobUI::AltModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, false, BobUI::AltModifier));
    native.append(new QNativeModifierEvent(BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_Alt, BobUI::NoModifier));
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::AltModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_A, BobUI::AltModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_Alt, BobUI::AltModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testModifierCtrl()
{
    // On Mac, we switch the Command and Control modifier by default, so that Command
    // means Meta, and Control means Command. Lets check that this works:
    QWidget w;
    w.show();

    QCOMPARE(ushort(kControlUnicode), QKeySequence(BobUI::Key_Meta).toString(QKeySequence::NativeText).at(0).unicode());
    QCOMPARE(ushort(kCommandUnicode), QKeySequence(BobUI::Key_Control).toString(QKeySequence::NativeText).at(0).unicode());

    NativeEventList native;
    native.append(new QNativeModifierEvent(BobUI::ControlModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, true, BobUI::ControlModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, false, BobUI::ControlModifier));
    native.append(new QNativeModifierEvent(BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_Meta, BobUI::NoModifier));
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::MetaModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_A, BobUI::MetaModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_Meta, BobUI::MetaModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
}

void tst_MacNativeEvents::testModifierCtrlWithDontSwapCtrlAndMeta()
{
    // On Mac, we switch the Command and Control modifier by default, so that Command
    // means Meta, and Control means Command. Lets check that the flag to swith off
    // this behaviour works. While working on this test I realized that we actually
    // don't (and never have) respected this flag for raw key events. Only for
    // menus, through QKeySequence. I don't want to change this behaviour now, at
    // least not until someone complains. So I choose to let the test just stop
    // any unintended regressions instead. If we decide to resepect the flag at one
    // point, fix the test.
    QCoreApplication::setAttribute(BobUI::AA_MacDontSwapCtrlAndMeta);
    QWidget w;
    w.show();

    QCOMPARE(ushort(kCommandUnicode), QKeySequence(BobUI::Key_Meta).toString(QKeySequence::NativeText).at(0).unicode());
    QCOMPARE(ushort(kControlUnicode), QKeySequence(BobUI::Key_Control).toString(QKeySequence::NativeText).at(0).unicode());

    NativeEventList native;
    native.append(new QNativeModifierEvent(BobUI::ControlModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, true, BobUI::ControlModifier));
    native.append(new QNativeKeyEvent(QNativeKeyEvent::Key_A, false, BobUI::ControlModifier));
    native.append(new QNativeModifierEvent(BobUI::NoModifier));

    ExpectedEventList expected(&w);
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_Control, BobUI::NoModifier));
    expected.append(new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::ControlModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_A, BobUI::ControlModifier));
    expected.append(new QKeyEvent(QEvent::KeyRelease, BobUI::Key_Control, BobUI::ControlModifier));

    native.play();
    QVERIFY2(expected.waitForAllEvents(), "the test did not receive all expected events!");
    QCoreApplication::setAttribute(BobUI::AA_MacDontSwapCtrlAndMeta, false);
}

BOBUIEST_MAIN(tst_MacNativeEvents)
#include "tst_macnativeevents.moc"
