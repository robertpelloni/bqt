// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qapplication.h>
#include <qfontinfo.h>


#include <qpushbutton.h>
#include <qscrollbar.h>
#include <bobuiimer.h>

#include <qdialog.h>

class TstWidget;
class TstDialog;
BOBUI_FORWARD_DECLARE_CLASS(QPushButton)

class TestButton : public QPushButton
{
public:
    TestButton(const QString &title, QWidget *parent = nullptr)
    : QPushButton(title, parent)
    {}
protected:
    bool hitButton(const QPoint &pos) const override
    {
        return rect().contains(pos);
    }
};

class tst_qmouseevent_modal : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void mousePressRelease();

private:
    TstWidget *w;
};

class TstWidget : public QWidget
{
    Q_OBJECT
public:
    TstWidget();
public slots:
    void buttonPressed();
public:
    TestButton *pb;
    TstDialog *d;
};


class TstDialog : public QDialog
{
    Q_OBJECT
public:
    TstDialog( QWidget *mouseWidget, QWidget *parent, const char *name );
    int count() { return c; }
protected:
    void showEvent(QShowEvent *) override;
public slots:
    void releaseMouse();
    void closeDialog();
private:
    QWidget *m;
    int c;
};

void tst_qmouseevent_modal::initTestCase()
{
    w = new TstWidget;
    w->show();
}

void tst_qmouseevent_modal::cleanupTestCase()
{
    delete w;
    w = 0;
}

/*
  Test for task 22500
*/
void tst_qmouseevent_modal::mousePressRelease()
{

    QVERIFY( !w->d->isVisible() );
    QVERIFY( w->d->count() == 0 );

    BOBUIest::mousePress( w->pb, BobUI::LeftButton );

    BOBUIRY_VERIFY( !w->d->isVisible() );
    QVERIFY( w->d->count() == 1 );
    QVERIFY( !w->pb->isDown() );

    BOBUIest::mousePress( w->pb, BobUI::LeftButton );

    BOBUIRY_VERIFY( !w->d->isVisible() );
    QVERIFY( w->d->count() == 2 );
    QVERIFY( !w->pb->isDown() );

    // With the current QWS mouse handling, the 3rd press would fail...

    BOBUIest::mousePress( w->pb, BobUI::LeftButton );

    BOBUIRY_VERIFY( !w->d->isVisible() );
    QVERIFY( w->d->count() == 3 );
    QVERIFY( !w->pb->isDown() );

    BOBUIest::mousePress( w->pb, BobUI::LeftButton );

    BOBUIRY_VERIFY( !w->d->isVisible() );
    QVERIFY( w->d->count() == 4 );
    QVERIFY( !w->pb->isDown() );
}


TstWidget::TstWidget()
{
    pb = new TestButton( "Press me", this );
    pb->setObjectName("testbutton");
    QSize s = pb->sizeHint();
    pb->setGeometry( 5, 5, s.width(), s.height() );

    connect( pb, SIGNAL(pressed()), this, SLOT(buttonPressed()) );

//    QScrollBar *sb = new QScrollBar( BobUI::Horizontal,  this );

//    sb->setGeometry( 5, pb->geometry().bottom() + 5, 100, sb->sizeHint().height() );

    d = new TstDialog( pb, this , 0 );
}

void TstWidget::buttonPressed()
{
    d->exec();
}

TstDialog::TstDialog( QWidget *mouseWidget, QWidget *parent, const char *name )
    :QDialog( parent )
{
    setObjectName(name);
    setModal(true);
    m = mouseWidget;
    c = 0;
}

void TstDialog::showEvent ( QShowEvent * )
{
    BOBUIimer::singleShot(1, this, SLOT(releaseMouse()));
    BOBUIimer::singleShot(100, this, SLOT(closeDialog()));
}

void TstDialog::releaseMouse()
{
    BOBUIest::mouseRelease(m, BobUI::LeftButton);
}

void TstDialog::closeDialog()
{
    if ( isVisible() ) {
        c++;
        accept();
    }
}

BOBUIEST_MAIN(tst_qmouseevent_modal)
#include "tst_qmouseevent_modal.moc"
