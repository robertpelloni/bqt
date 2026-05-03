// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIGui/qpa/qplatformintegration.h>

class tst_Keyboard : public QObject
{
    Q_OBJECT

private slots:
    void keyPressAndRelease();
};

class KeyWindow : public QWindow
{
public:
    void keyPressEvent(QKeyEvent *event) override
    {
        QSharedPointer<QKeyEvent> copiedEvent(new QKeyEvent(event->type(), event->key(),
            event->modifiers(), event->text(), event->isAutoRepeat(), event->count()));
        mEventOrder.append(copiedEvent);
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        QSharedPointer<QKeyEvent> copiedEvent(new QKeyEvent(event->type(), event->key(),
            event->modifiers(), event->text(), event->isAutoRepeat(), event->count()));
        mEventOrder.append(copiedEvent);
    }

    QList<QSharedPointer<QKeyEvent>> mEventOrder;
};

void tst_Keyboard::keyPressAndRelease()
{
    KeyWindow window;
    window.show();
    window.setGeometry(100, 100, 200, 200);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation)) {
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             "qWaitForWindowActive was called on a platform that doesn't support window "
                             "activation. This means there is an error in the test and it should either "
                             "check for the WindowActivation platform capability before calling "
                             "qWaitForWindowActivate, use qWaitForWindowExposed instead, or skip the test. "
                             "Falling back to qWaitForWindowExposed.");
    }
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    BOBUIest::keyPress(&window, BobUI::Key_A);
    BOBUIest::keyRelease(&window, BobUI::Key_A);
    QCOMPARE(window.mEventOrder.size(), 2);

    const auto pressEvent = window.mEventOrder.at(0);
    QCOMPARE(pressEvent->type(), QEvent::KeyPress);
    QCOMPARE(pressEvent->key(), BobUI::Key_A);
    QCOMPARE(pressEvent->modifiers(), BobUI::NoModifier);
    QCOMPARE(pressEvent->text(), "a");
    QCOMPARE(pressEvent->isAutoRepeat(), false);

    const auto releaseEvent = window.mEventOrder.at(1);
    QCOMPARE(releaseEvent->type(), QEvent::KeyRelease);
    QCOMPARE(releaseEvent->key(), BobUI::Key_A);
    QCOMPARE(releaseEvent->modifiers(), BobUI::NoModifier);
    QCOMPARE(releaseEvent->text(), "a");
    QCOMPARE(releaseEvent->isAutoRepeat(), false);
}

BOBUIEST_MAIN(tst_Keyboard)
#include "tst_keyboard.moc"
