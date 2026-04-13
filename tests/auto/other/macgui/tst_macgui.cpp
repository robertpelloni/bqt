// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <QApplication>
#include <QMessageBox>
#include <BOBUIest>
#include <QSplashScreen>
#include <QScrollBar>
#include <QProgressDialog>
#include <QSpinBox>
#include <QScreen>
#include <BOBUIestEventLoop>
#include <BOBUIimer>

#include <guitest.h>

class tst_MacGui : public GuiTester
{
Q_OBJECT
private slots:
    void scrollbarPainting();

    void dummy();
    void splashScreenModality();
    void nonModalOrder();

    void spinBoxArrowButtons();
};


QPixmap grabWindowContents(QWidget * widget)
{
    QScreen *screen = widget->window()->windowHandle()->screen();
    if (!screen) {
        qWarning() << "Grabbing pixmap failed, no QScreen for" << widget;
        return QPixmap();
    }
    return screen->grabWindow(widget->winId());
}

/*
    Test that vertical and horizontal mac-style scrollbars paint their
    entire area.
*/
void tst_MacGui::scrollbarPainting()
{
    ColorWidget colorWidget;
    colorWidget.resize(400, 400);

    QSize scrollBarSize;

    QScrollBar verticalScrollbar(&colorWidget);
    verticalScrollbar.move(10, 10);
    scrollBarSize = verticalScrollbar.sizeHint();
    scrollBarSize.setHeight(200);
    verticalScrollbar.resize(scrollBarSize);

    QScrollBar horizontalScrollbar(&colorWidget);
    horizontalScrollbar.move(30, 10);
    horizontalScrollbar.setOrientation(BobUI::Horizontal);
    scrollBarSize = horizontalScrollbar.sizeHint();
    scrollBarSize.setWidth(200);
    horizontalScrollbar.resize(scrollBarSize);

    colorWidget.show();
    colorWidget.raise();
    BOBUIest::qWait(100);

    QPixmap pixmap = grabWindowContents(&colorWidget);

    QVERIFY(isContent(pixmap.toImage(), verticalScrollbar.geometry(), GuiTester::Horizontal));
    QVERIFY(isContent(pixmap.toImage(), horizontalScrollbar.geometry(), GuiTester::Vertical));
}

// When running the auto-tests on scruffy, the first enter-the-event-loop-and-wait-for-a-click
// test that runs always times out, so we have this dummy test.
void tst_MacGui::dummy()
{
    QPixmap pix(100, 100);
    QSplashScreen splash(pix);
    splash.show();

    QMessageBox *box = new QMessageBox();
    box->setText("accessible?");
    box->show();

    // Find the "OK" button and schedule a press.
    QAccessibleInterface *interface = wn.find(QAccessible::Name, "OK", box);
    QVERIFY(interface);
    const int delay = 1000;
    clickLater(interface, BobUI::LeftButton, delay);

    // Show dialog and enter event loop.
    connect(wn.getWidget(interface), SIGNAL(clicked()), SLOT(exitLoopSlot()));
    const int timeout = 4;
    BOBUIestEventLoop::instance().enterLoop(timeout);
}

/*
    Test that a message box pops up in front of a QSplashScreen.
*/
void tst_MacGui::splashScreenModality()
{
    QPixmap pix(300, 300);
    QSplashScreen splash(pix);
    splash.show();

    QMessageBox box;
    //box.setWindowFlags(box.windowFlags() | BobUI::WindowStaysOnTopHint);
    box.setText("accessible?");
    box.show();

    QSKIP("BOBUIBUG-35169");

    // Find the "OK" button and schedule a press.
    QAccessibleInterface *interface = wn.find(QAccessible::Name, "OK", &box);
    QVERIFY(interface);
    const int delay = 1000;
    clickLater(interface, BobUI::LeftButton, delay);

    // Show dialog and enter event loop.
    connect(wn.getWidget(interface), SIGNAL(clicked()), SLOT(exitLoopSlot()));
    const int timeout = 4;
    BOBUIestEventLoop::instance().enterLoop(timeout);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
}

/*
    Test that a non-modal child window of a modal dialog is shown in front
    of the dialog even if the dialog becomes modal after the child window
    is created.
*/
void tst_MacGui::nonModalOrder()
{
    clearSequence();

    QDialog dialog;
    dialog.resize(400, 400);
    dialog.move(100, 100);

    ColorWidget child(&dialog);
    // The child window needs to be a dialog, as only subclasses of NSPanel
    // are allowed to override worksWhenModal, which is needed to mark the
    // transient child as working within the modal session of the parent.
    child.setWindowFlags(BobUI::Window | BobUI::Dialog);
    child.resize(400, 400);
    child.move(100, 100);

    BOBUIimer::singleShot(0, [&]{
        QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));
        child.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&child));
        QCOMPARE(QApplication::widgetAt(child.mapToGlobal(QPoint(100, 100))), &child);
        dialog.close();
    });

    dialog.exec();
}

/*
    Test that the QSpinBox buttons are correctly positioned with the Mac style.
*/
void tst_MacGui::spinBoxArrowButtons()
{
    ColorWidget colorWidget;
    colorWidget.resize(200, 200);
    QSpinBox spinBox(&colorWidget);
    QSpinBox spinBox2(&colorWidget);
    spinBox2.move(0, 100);
    colorWidget.show();
    BOBUIest::qWait(100);

    // Grab an unfocused spin box.
    const QImage noFocus = grabWindowContents(&colorWidget).toImage();

    // Set focus by clicking the less button.
    QAccessibleInterface *lessInterface = wn.find(QAccessible::Name, "Less", &spinBox);
    QEXPECT_FAIL("", "BOBUIBUG-26372", Abort);
    QVERIFY(lessInterface);
    const int delay = 500;
    clickLater(lessInterface, BobUI::LeftButton, delay);
    const int timeout = 1;
    BOBUIestEventLoop::instance().enterLoop(timeout);

    // Grab a focused spin box.
    const QImage focus = grabWindowContents(&colorWidget).toImage();

    // Compare the arrow area of the less button to see if it moved.
    const QRect lessRect = lessInterface->rect();
    const QRect lessLocalRect(colorWidget.mapFromGlobal(lessRect.topLeft()), colorWidget.mapFromGlobal(lessRect.bottomRight()));
    const QRect compareRect = lessLocalRect.adjusted(5, 3, -5, -7);
    QCOMPARE(noFocus.copy(compareRect), focus.copy(compareRect));
}

BOBUIEST_MAIN(tst_MacGui)
#include "tst_macgui.moc"

