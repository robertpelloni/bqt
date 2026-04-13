// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <qstatusbar.h>
#include <QElapsedTimer>
#include <QLabel>
#include <QMainWindow>
#include <QSizeGrip>

class tst_QStatusBar: public QObject
{
    Q_OBJECT

protected slots:
    void messageChanged(const QString&);

private slots:
    void init();
    void cleanup();

    void tempMessage();
    void insertWidget();
    void insertPermanentWidget();
    void removeWidget();
    void setSizeGripEnabled();
    void task194017_hiddenWidget();
    void BOBUIBUG4334_hiddenOnMaximizedWindow();
    void BOBUIBUG25492_msgtimeout();
    void messageChangedSignal();

private:
    QStatusBar *testWidget;
    QString currentMessage;
};

void tst_QStatusBar::init()
{
    testWidget = new QStatusBar;
    connect(testWidget, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    QWidget *item1 = new QWidget(testWidget);
    testWidget->addWidget(item1);
    // currentMessage needs to be null as the code relies on this
    currentMessage = QString();
}

void tst_QStatusBar::cleanup()
{
    delete testWidget;
}

void tst_QStatusBar::messageChanged(const QString &m)
{
    currentMessage = m;
}

void tst_QStatusBar::tempMessage()
{
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());

    testWidget->showMessage("Ready", 500);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    BOBUIRY_VERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());

    testWidget->showMessage("Ready again", 500);
    QCOMPARE(testWidget->currentMessage(), QString("Ready again"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    testWidget->clearMessage();
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());
}

void tst_QStatusBar::insertWidget()
{
    QStatusBar sb;
    sb.addPermanentWidget(new QLabel("foo"));
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertWidget: Index out of range (-1), appending widget");
    QCOMPARE(sb.insertWidget(-1, new QLabel("foo")), 0);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertWidget: Index out of range (2), appending widget");
    QCOMPARE(sb.insertWidget(2, new QLabel("foo")), 1);
    QCOMPARE(sb.insertWidget(0, new QLabel("foo")), 0);
    QCOMPARE(sb.insertWidget(3, new QLabel("foo")), 3);
}

void tst_QStatusBar::insertPermanentWidget()
{
    QStatusBar sb;
    sb.addWidget(new QLabel("foo"));
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (-1), appending widget");
    QCOMPARE(sb.insertPermanentWidget(-1, new QLabel("foo")), 1);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (0), appending widget");
    QCOMPARE(sb.insertPermanentWidget(0, new QLabel("foo")), 2);
    QCOMPARE(sb.insertPermanentWidget(2, new QLabel("foo")), 2);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (5), appending widget");
    QCOMPARE(sb.insertPermanentWidget(5, new QLabel("foo")), 4);
    QCOMPARE(sb.insertWidget(1, new QLabel("foo")), 1);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (1), appending widget");
    QCOMPARE(sb.insertPermanentWidget(1, new QLabel("foo")), 6);
}

void tst_QStatusBar::removeWidget()
{
    QStatusBar sb;
    std::vector<std::unique_ptr<QLabel>> widgets;
    std::vector<bool> states;
    for (int i = 0; i < 10; ++i) {
        const QString text = i > 5 ? QString("p_%1").arg(i) : QString::number(i);
        widgets.push_back(std::make_unique<QLabel>(text));
        states.push_back(true);
    }

    for (auto &&widget : widgets) {
        if (widget->text().startsWith("p_"))
            sb.addPermanentWidget(widget.get());
        else
            sb.addWidget(widget.get());
    }
    sb.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&sb));

    auto checkStates = [&]{
        for (size_t index = 0; index < std::size(widgets); ++index) {
            if (widgets.at(index)->isVisible() != states.at(index)) {
                qCritical("Mismatch for widget at index %zu\n"
                          "\tActual  : %s\n"
                          "\tExpected: %s",
                          index, widgets.at(index)->isVisible() ? "true" : "false",
                          states.at(index) ? "true" : "false");
                return false;
            }
        }
        return true;
    };

    QVERIFY(checkStates());
    // remove every widget except the first to trigger unstable reference
    for (size_t i = 2; i < std::size(widgets); ++i) {
        sb.removeWidget(widgets[i].get());
        states[i] = false;
        QVERIFY2(checkStates(), qPrintable(QString("Failure at index %1").arg(i)));
    }
}

void tst_QStatusBar::setSizeGripEnabled()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMainWindow mainWindow;
    QPointer<QStatusBar> statusBar = mainWindow.statusBar();
    QVERIFY(statusBar);
    mainWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&mainWindow));

    BOBUIRY_VERIFY(statusBar->isVisible());
    QPointer<QSizeGrip> sizeGrip = statusBar->findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(true);
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());

    statusBar->hide();
    QVERIFY(!sizeGrip->isVisible());
    statusBar->show();
    QVERIFY(sizeGrip->isVisible());

    sizeGrip->setVisible(false);
    QVERIFY(!sizeGrip->isVisible());
    statusBar->hide();
    statusBar->show();
    QVERIFY(!sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(false);
    QVERIFY(!sizeGrip);

    qApp->processEvents();
#ifndef Q_OS_MAC // Work around Lion fullscreen issues on CI system - BOBUIQAINFRA-506
    mainWindow.showFullScreen();
#endif
    QVERIFY(BOBUIest::qWaitForWindowExposed(&mainWindow));
    qApp->processEvents();

    mainWindow.setStatusBar(new QStatusBar(&mainWindow));
    //we now call deleteLater on the previous statusbar
    QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
    QVERIFY(!statusBar);
    statusBar = mainWindow.statusBar();
    QVERIFY(statusBar);

    sizeGrip = statusBar->findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(!sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(true);
    QVERIFY(!sizeGrip->isVisible());

    qApp->processEvents();
#ifndef Q_OS_MAC
    mainWindow.showNormal();
#endif
    qApp->processEvents();
    BOBUIRY_VERIFY(sizeGrip->isVisible());
}

void tst_QStatusBar::task194017_hiddenWidget()
{
    QStatusBar sb;

    QWidget *label= new QLabel("bar",&sb);
    sb.addWidget(label);
    sb.show();
    QVERIFY(label->isVisible());
    sb.showMessage("messssage");
    QVERIFY(!label->isVisible());
    sb.hide();
    QVERIFY(!label->isVisible());
    sb.show();
    QVERIFY(!label->isVisible());
    sb.clearMessage();
    QVERIFY(label->isVisible());
    label->hide();
    QVERIFY(!label->isVisible());
    sb.showMessage("messssage");
    QVERIFY(!label->isVisible());
    sb.clearMessage();
    QVERIFY(!label->isVisible());
    sb.hide();
    QVERIFY(!label->isVisible());
    sb.show();
    QVERIFY(!label->isVisible());
}

void tst_QStatusBar::BOBUIBUG4334_hiddenOnMaximizedWindow()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMainWindow main;
    QStatusBar statusbar;
    statusbar.setSizeGripEnabled(true);
    main.setStatusBar(&statusbar);
    main.showMaximized();
    QVERIFY(BOBUIest::qWaitForWindowActive(&main));
#ifndef Q_OS_MAC
    BOBUIRY_VERIFY(!statusbar.findChild<QSizeGrip*>()->isVisible());
#endif
    main.showNormal();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&main));
    QVERIFY(statusbar.findChild<QSizeGrip*>()->isVisible());
    main.showFullScreen();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&main));
    QVERIFY(!statusbar.findChild<QSizeGrip*>()->isVisible());
}

void tst_QStatusBar::BOBUIBUG25492_msgtimeout()
{
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());
    testWidget->show();

    // Set display message forever first
    testWidget->showMessage("Ready", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    // Set display message for 2 seconds
    QElapsedTimer t;
    t.start();
    testWidget->showMessage("Ready 2000", 2000);
    QCOMPARE(testWidget->currentMessage(), QString("Ready 2000"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    // Message disappears after 2 seconds
    BOBUIRY_VERIFY(testWidget->currentMessage().isNull());
    qint64 ts = t.elapsed();

    // XXX: ideally ts should be 2000, but sometimes it appears to go away early, probably due to timer granularity.
    QVERIFY2(ts >= 1800, qPrintable("Timer was " + QString::number(ts)));
    if (ts < 2000)
        qWarning("BOBUIBUG25492_msgtimeout: message vanished early, should be >= 2000, was %lld", ts);
    QVERIFY(currentMessage.isNull());

    // Set display message for 2 seconds first
    testWidget->showMessage("Ready 25492", 2000);
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    // Set display message forever again
    testWidget->showMessage("Ready 25492", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    BOBUIest::qWait(3000);

    // Message displays forever
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);
}

void tst_QStatusBar::messageChangedSignal()
{
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());
    testWidget->show();

    QSignalSpy spy(testWidget, SIGNAL(messageChanged(QString)));
    testWidget->showMessage("Ready", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), currentMessage);
    testWidget->clearMessage();
    QCOMPARE(testWidget->currentMessage(), QString());
    QCOMPARE(testWidget->currentMessage(), currentMessage);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), currentMessage);
    testWidget->showMessage("Ready", 0);
    testWidget->showMessage("Ready", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), currentMessage);
}

BOBUIEST_MAIN(tst_QStatusBar)
#include "tst_qstatusbar.moc"
