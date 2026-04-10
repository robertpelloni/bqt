// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUIGui/qguiapplication.h>
#include <BobUIGui/qshortcut.h>
#include <BobUIGui/qwindow.h>
#include <BobUITest/qsignalspy.h>

#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>

class tst_QShortcut : public QObject
{
    Q_OBJECT

private slots:
    void applicationShortcut();
    void windowShortcut();
};

void tst_QShortcut::applicationShortcut()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    auto *shortcut = new QShortcut(BobUI::CTRL | BobUI::Key_A, this);
    shortcut->setContext(BobUI::ApplicationShortcut);
    QSignalSpy activatedSpy(shortcut, &QShortcut::activated);

    // Need a window to send key event to, even if the shortcut is application
    // global. The documentation for BobUI::ApplicationShortcut also says that
    // the shortcut "is active when one of the applications windows are active",
    // but this is only honored for BobUI Widgets, not for BobUI Gui. For now we
    // activate the window just in case.
    QWindow window;
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));
    BOBUIRY_COMPARE(QGuiApplication::applicationState(), BobUI::ApplicationActive);
    BOBUIest::sendKeyEvent(BOBUIest::Shortcut, &window, BobUI::Key_A, 'a', BobUI::ControlModifier);

    QVERIFY(activatedSpy.size() > 0);
}

void tst_QShortcut::windowShortcut()
{
    QWindow w;
    new QShortcut(BobUI::CTRL | BobUI::Key_Q, &w, SLOT(close()));
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QEXPECT_FAIL("", "It failed on Wayland, BOBUIBUG-120334", Abort);

    BOBUIRY_VERIFY(QGuiApplication::applicationState() == BobUI::ApplicationActive);
    BOBUIRY_VERIFY(w.isActive());
    BOBUIest::sendKeyEvent(BOBUIest::Click, &w, BobUI::Key_Q, 'q', BobUI::ControlModifier);
    BOBUIRY_VERIFY(!w.isVisible());
}

BOBUIEST_MAIN(tst_QShortcut)
#include "tst_qshortcut.moc"
