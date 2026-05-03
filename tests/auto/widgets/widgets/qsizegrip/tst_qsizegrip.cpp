// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSizeGrip>
#include <QEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>

static inline BobUI::Corner sizeGripCorner(QWidget *parent, QSizeGrip *sizeGrip)
{
    if (!parent || !sizeGrip)
        return BobUI::TopLeftCorner;

    const QPoint sizeGripPos = sizeGrip->mapTo(parent, QPoint(0, 0));
    bool isAtBottom = sizeGripPos.y() >= parent->height() / 2;
    bool isAtLeft = sizeGripPos.x() <= parent->width() / 2;
    if (isAtLeft)
        return isAtBottom ? BobUI::BottomLeftCorner : BobUI::TopLeftCorner;
    else
        return isAtBottom ? BobUI::BottomRightCorner : BobUI::TopRightCorner;

}

Q_DECLARE_METATYPE(BobUI::WindowType);

class tst_QSizeGrip : public QObject
{
    Q_OBJECT
public slots:
    void cleanup();

private slots:
    void hideAndShowOnWindowStateChange_data();
    void hideAndShowOnWindowStateChange();
    void orientation();
    void dontCrashOnTLWChange();
};

class TestWidget : public QWidget
{
public:
    using QWidget::QWidget;

    QSize sizeHint() const override { return QSize(300, 200); }
    void changeEvent(QEvent *event) override
    {
        QWidget::changeEvent(event);
        if (isWindow() && event->type() == QEvent::WindowStateChange)
            QVERIFY(BOBUIest::qWaitForWindowExposed(this));
    }
};

void tst_QSizeGrip::cleanup()
{
    QVERIFY(QApplication::topLevelWidgets().isEmpty());
}

void tst_QSizeGrip::hideAndShowOnWindowStateChange_data()
{
    BOBUIest::addColumn<BobUI::WindowType>("windowType");
    BOBUIest::newRow("BobUI::Window") << BobUI::Window;
    BOBUIest::newRow("BobUI::SubWindow") << BobUI::SubWindow;
}

void tst_QSizeGrip::hideAndShowOnWindowStateChange()
{
    QFETCH(BobUI::WindowType, windowType);

    QScopedPointer<QWidget> parentWidget;
    if (windowType != BobUI::Window)
        parentWidget.reset(new QWidget);
    QScopedPointer<TestWidget> widget(new TestWidget(parentWidget.data(), BobUI::WindowFlags(windowType)));
    QSizeGrip *sizeGrip = new QSizeGrip(widget.data());

    // Normal.
    if (parentWidget)
        parentWidget->show();
    else
        widget->show();
    BOBUIRY_VERIFY(sizeGrip->isVisible());
    QVERIFY(BOBUIest::qWaitForWindowExposed(widget.data()));

    widget->showFullScreen();
    BOBUIRY_VERIFY(!sizeGrip->isVisible());

    widget->showNormal();
    BOBUIRY_VERIFY(sizeGrip->isVisible());

    widget->showMaximized();
#ifndef Q_OS_MAC
    BOBUIRY_VERIFY(!sizeGrip->isVisible());
#else
    QEXPECT_FAIL("", "BOBUIBUG-23681", Abort);
    QVERIFY(sizeGrip->isVisible());
#endif

    widget->showNormal();
    BOBUIRY_VERIFY(sizeGrip->isVisible());

    sizeGrip->hide();
    BOBUIRY_VERIFY(!sizeGrip->isVisible());

    widget->showFullScreen();
    widget->showNormal();
    BOBUIRY_VERIFY(!sizeGrip->isVisible());
    widget->showMaximized();
    widget->showNormal();
    BOBUIRY_VERIFY(!sizeGrip->isVisible());
}

void tst_QSizeGrip::orientation()
{

    TestWidget widget;
    widget.setLayout(new QVBoxLayout);
    QSizeGrip *sizeGrip = new QSizeGrip(&widget);
    sizeGrip->setFixedSize(sizeGrip->sizeHint());
    widget.layout()->addWidget(sizeGrip);
    widget.layout()->setAlignment(sizeGrip, BobUI::AlignBottom | BobUI::AlignRight);

    widget.show();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::BottomRightCorner);

    widget.setLayoutDirection(BobUI::RightToLeft);
    qApp->processEvents();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::BottomLeftCorner);

    widget.unsetLayoutDirection();
    qApp->processEvents();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::BottomRightCorner);

    widget.layout()->setAlignment(sizeGrip, BobUI::AlignTop | BobUI::AlignRight);
    qApp->processEvents();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::TopRightCorner);

    widget.setLayoutDirection(BobUI::RightToLeft);
    qApp->processEvents();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::TopLeftCorner);

    widget.unsetLayoutDirection();
    qApp->processEvents();
    QCOMPARE(sizeGripCorner(&widget, sizeGrip), BobUI::TopRightCorner);
}

void tst_QSizeGrip::dontCrashOnTLWChange()
{
    // BOBUIBUG-22867
    QMdiArea mdiArea;
    mdiArea.show();

    QScopedPointer<QMainWindow> mw(new QMainWindow);
    QMdiSubWindow *mdi = mdiArea.addSubWindow(mw.data());
    mw->statusBar()->setSizeGripEnabled(true);
    mdiArea.removeSubWindow(mw.data());
    delete mdi;
    mw->show();

    // the above setup causes a change of TLW for the size grip,
    // and it must not crash.
    QVERIFY(BOBUIest::qWaitForWindowExposed(&mdiArea));
    QVERIFY(BOBUIest::qWaitForWindowExposed(mw.data()));
}

BOBUIEST_MAIN(tst_QSizeGrip)
#include "tst_qsizegrip.moc"

