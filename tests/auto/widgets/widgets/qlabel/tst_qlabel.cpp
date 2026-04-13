// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BOBUIimer>

#include "qlabel.h"
#include <qapplication.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmovie.h>
#include <qpicture.h>
#include <qmessagebox.h>
#include <qsizepolicy.h>
#include <qfontmetrics.h>
#include <qmath.h>
#include <private/qlabel_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

class Widget : public QWidget
{
    Q_OBJECT
public:
    QList<QEvent::Type> events;

protected:
    bool event(QEvent *ev) override {
        events.append(ev->type());
        return QWidget::event(ev);
    }

};

class tst_QLabel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void getSetCheck();
    void setText_data();
    void setText();
    void setTextFormat();
#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
    void setBuddy();
#endif
    void setNum();
    void clear();
    void wordWrap_data();
    void wordWrap();
    void eventPropagation_data();
    void eventPropagation();
    void focusPolicy();

    void task190318_sizes();

    void sizeHint();

    void task226479_movieResize();
    void emptyPixmap();

    void unicodeText_data();
    void unicodeText();

#if BOBUI_CONFIG(shortcut)
    void mnemonic_data();
    void mnemonic();
#endif
    void selection();

#ifndef BOBUI_NO_CONTEXTMENU
    void taskBOBUIBUG_7902_contextMenuCrash();
    void contextMenu_data();
    void contextMenu();
#endif

    void taskBOBUIBUG_48157_dprPixmap();
    void taskBOBUIBUG_48157_dprMovie();

    void resourceProvider();
    void mouseEventPropagation_data();
    void mouseEventPropagation();

private:
    QLabel *testWidget;
    QPointer<Widget> test_box;
    QPointer<QLabel> test_label;
};

// Testing get/set functions
void tst_QLabel::getSetCheck()
{
    QLabel obj1;
    obj1.setWordWrap(false);
    QVERIFY(!obj1.wordWrap());
    obj1.setWordWrap(true);
    QVERIFY(obj1.wordWrap());

#if BOBUI_CONFIG(shortcut)
    QWidget *var2 = new QWidget();
    obj1.setBuddy(var2);
    QCOMPARE(var2, obj1.buddy());
    obj1.setBuddy((QWidget *)0);
    QCOMPARE((QWidget *)0, obj1.buddy());
    delete var2;
#endif // BOBUI_CONFIG(shortcut)

    QMovie *var3 = new QMovie;
    obj1.setMovie(var3);
    QCOMPARE(var3, obj1.movie());
    obj1.setMovie((QMovie *)0);
    QCOMPARE((QMovie *)0, obj1.movie());
    delete var3;
}

void tst_QLabel::initTestCase()
{
    // Create the test class
    testWidget = new QLabel(0);
    testWidget->resize( 200, 200 );
    testWidget->show();
}

void tst_QLabel::cleanupTestCase()
{
    delete testWidget;
    testWidget = 0;
    delete test_box;
}

void tst_QLabel::init()
{
    testWidget->setTextFormat( BobUI::AutoText );
# if BOBUI_CONFIG(shortcut)
    testWidget->setBuddy( 0 );
#endif
    testWidget->setIndent( 0 );
    testWidget->setAlignment( BobUI::AlignLeft | BobUI::AlignVCenter );
    testWidget->setScaledContents( false );
}

void tst_QLabel::cleanup()
{
    if (BOBUIest::currentTestFunction() == QLatin1String("setBuddy")) {
        testWidget->show();

        delete test_box; // this should delete tst_labl and test_edit as well.
    }
}

// Set buddy doesn't make much sense on OS X
#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
void tst_QLabel::setBuddy()
{
    testWidget->hide();

    test_box = new Widget;
    test_label= new QLabel( test_box );
    test_label->setText( "&Test with a buddy" );
    QWidget *test_edit = new QLineEdit( test_box );
    QWidget *test_edit2 = new QLineEdit( test_box );
    QVBoxLayout *layout = new QVBoxLayout(test_box);
    layout->addWidget(test_label);
    layout->addWidget(test_edit);
    layout->addWidget(test_edit2);
    test_box->show();
    QApplicationPrivate::setActiveWindow(test_box);
    QVERIFY(test_box->isActiveWindow());

    test_label->setBuddy( test_edit );
    test_label->setFocus();
    QVERIFY( !test_edit->hasFocus() );
    BOBUIest::keyClick( test_box, 't', BobUI::AltModifier );
    QVERIFY( test_edit->hasFocus() );

    // Setting a new buddy should disconnect the old one's destroyed() signal
    test_label->setBuddy(test_edit2);
    delete test_edit;
    QCOMPARE(test_label->buddy(), test_edit2);

    // And deleting our own buddy should disconnect and not crash
    delete test_edit2;
    BOBUIest::keyClick(test_box, 't', BobUI::AltModifier );

    delete test_box;
}
#endif // BOBUI_CONFIG(shortcut) && !Q_OS_DARWIN

void tst_QLabel::setText_data()
{
    BOBUIest::addColumn<QString>("txt");
    BOBUIest::addColumn<QString>("font");

    QString prefix = "";
#ifdef Q_OS_WIN32
    prefix = "win32_";
#endif

    BOBUIest::newRow( QString(prefix + "data0").toLatin1() ) << QString("This is a single line") << QString("Helvetica");
    BOBUIest::newRow( QString(prefix + "data1").toLatin1() ) << QString("This is the first line\nThis is the second line") << QString("Courier");
    BOBUIest::newRow( QString(prefix + "data2").toLatin1() ) << QString("This is the first line\nThis is the second line\nThis is the third line") << QString("Helvetica");
    BOBUIest::newRow( QString(prefix + "data3").toLatin1() ) << QString("This is <b>bold</b> richtext") << QString("Courier");
    BOBUIest::newRow( QString(prefix + "data4").toLatin1() ) << QString("I Have a &shortcut") << QString("Helvetica");
}

void tst_QLabel::setText()
{
    QFETCH( QString, txt );
    QFETCH( QString, font );
    QFont f( font, 8 );
    testWidget->setFont( f );
    testWidget->setText( txt );
    QCOMPARE( testWidget->text(), txt );
}

void tst_QLabel::setTextFormat()
{
    // lets' start with the simple stuff...
    testWidget->setTextFormat( BobUI::PlainText );
    QVERIFY( testWidget->textFormat() == BobUI::PlainText );

    testWidget->setTextFormat( BobUI::RichText );
    QVERIFY( testWidget->textFormat() == BobUI::RichText );

    testWidget->setTextFormat( BobUI::AutoText );
    QVERIFY( testWidget->textFormat() == BobUI::AutoText );
}

void tst_QLabel::setNum()
{
    testWidget->setText( "This is a text" );
    testWidget->setNum( 12 );
    QCOMPARE( testWidget->text(), QString("12") );
    testWidget->setNum( 12.345 );
    QCOMPARE( testWidget->text(), QString("12.345") );
}

void tst_QLabel::clear()
{
    const QString TEXT = "blah blah";
    testWidget->setText(TEXT);
    QCOMPARE(testWidget->text(), TEXT);
    testWidget->clear();
    QVERIFY(testWidget->text().isEmpty());
}

void tst_QLabel::wordWrap_data()
{
    BOBUIest::addColumn<QString>("text");

    BOBUIest::newRow("Plain text") << "Plain text1";
    BOBUIest::newRow("Rich text") << "<b>Rich text</b>";
    BOBUIest::newRow("Long text")
                  << "This is a very long text to check that QLabel "
                     "does not wrap, even if the text would require wrapping to be fully displayed";
}

void tst_QLabel::wordWrap()
{
    QFETCH(QString, text);

    QLabel label;
    label.setText(text);
    QVERIFY(!label.wordWrap());
    QVERIFY(!label.sizePolicy().hasHeightForWidth());

    const QSize unWrappedSizeHint = label.sizeHint();

    label.setWordWrap(true);
    QVERIFY(label.sizePolicy().hasHeightForWidth());

    if (text.size() > 1 && text.contains(" ")) {
        const int wrappedHeight = label.heightForWidth(unWrappedSizeHint.width() / 2);
        QVERIFY(wrappedHeight > unWrappedSizeHint.height());
    }

}

void tst_QLabel::eventPropagation_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("textInteractionFlags");
    BOBUIest::addColumn<int>("focusPolicy");
    BOBUIest::addColumn<bool>("propagation");

    BOBUIest::newRow("plain text1") << QString("plain text") << int(BobUI::LinksAccessibleByMouse) << int(BobUI::NoFocus) << true;
    BOBUIest::newRow("plain text2") << QString("plain text") << (int)BobUI::TextSelectableByKeyboard << (int)BobUI::ClickFocus << true;
    BOBUIest::newRow("plain text3") << QString("plain text") << (int)BobUI::TextSelectableByMouse << (int)BobUI::ClickFocus << false;
    BOBUIest::newRow("plain text4") << QString("plain text") << (int)BobUI::NoTextInteraction << (int)BobUI::NoFocus << true;
    BOBUIest::newRow("rich text1") << QString("<b>rich text</b>") << (int)BobUI::LinksAccessibleByMouse << (int)BobUI::NoFocus << true;
    BOBUIest::newRow("rich text2") << QString("<b>rich text</b>") << (int)BobUI::TextSelectableByKeyboard << (int)BobUI::ClickFocus << true;
    BOBUIest::newRow("rich text3") << QString("<b>rich text</b>") << (int)BobUI::TextSelectableByMouse << (int)BobUI::ClickFocus << false;
    BOBUIest::newRow("rich text4") << QString("<b>rich text</b>") << (int)BobUI::NoTextInteraction << (int)BobUI::NoFocus << true;
    BOBUIest::newRow("rich text5") << QString("<b>rich text</b>") << (int)BobUI::LinksAccessibleByKeyboard << (int)BobUI::StrongFocus << true;

    if (!test_box)
        test_box = new Widget;
    if (!test_label)
        test_label = new QLabel(test_box);
}

void tst_QLabel::eventPropagation()
{
    QFETCH(QString, text);
    QFETCH(int, textInteractionFlags);
    QFETCH(int, focusPolicy);
    QFETCH(bool, propagation);

    // plain text (accepts mouse event _only_ when label selectable by mouse)
    test_label->setText(text);
    test_box->events.clear();
    test_label->setTextInteractionFlags(BobUI::TextInteractionFlags(textInteractionFlags));
    QCOMPARE(int(test_label->focusPolicy()), focusPolicy);
    BOBUIest::mousePress(test_label, BobUI::LeftButton);
    QVERIFY(test_box->events.contains(QEvent::MouseButtonPress) == propagation); // should have propagated!
}

void tst_QLabel::focusPolicy()
{
    delete test_label;
    test_label = new QLabel;
    QCOMPARE(test_label->focusPolicy(), BobUI::NoFocus); // default
    test_label->setFocusPolicy(BobUI::StrongFocus);
    test_label->setText("Whatever"); // setting text should not change the focus policy
    QCOMPARE(test_label->focusPolicy(), BobUI::StrongFocus);
    test_label->setTextInteractionFlags(BobUI::TextSelectableByKeyboard); // this should
    QCOMPARE(test_label->focusPolicy(), BobUI::ClickFocus);
    test_label->setFocusPolicy(BobUI::StrongFocus);
    test_label->setText("Whatever"); // setting text should not change the focus policy
    QCOMPARE(test_label->focusPolicy(), BobUI::StrongFocus);
    test_label->setTextInteractionFlags(BobUI::NoTextInteraction);
    QCOMPARE(test_label->focusPolicy(), BobUI::NoFocus);
    test_label->setFocusPolicy(BobUI::StrongFocus);
    test_label->setTextInteractionFlags(BobUI::NoTextInteraction);
    QCOMPARE(test_label->focusPolicy(), BobUI::StrongFocus); // is not touched since value didn't change
    delete test_label;
}

void tst_QLabel::task190318_sizes()
{
    QLabel label(" ");
    QSize ms(500,600);
    label.setMinimumSize(ms);
    QCOMPARE(label.minimumSize(), ms);
    QCOMPARE(label.sizeHint(), ms);
    QCOMPARE(label.minimumSizeHint(), ms);
}

void tst_QLabel::sizeHint()
{
    QLabel label(QLatin1String("Test"));
    label.setIndent(0);
    label.setMargin(0);
    label.setContentsMargins(0, 0, 0, 0);
    label.setAlignment(BobUI::AlignVCenter);
    int h = label.sizeHint().height();

    QLabel l1(QLatin1String("Test"));
    l1.setIndent(0);
    l1.setMargin(0);
    l1.setContentsMargins(0, 0, 0, 0);
    l1.setAlignment(BobUI::AlignVCenter);
    l1.setTextInteractionFlags(BobUI::TextSelectableByMouse);   // will now use bobuiextcontrol
    int h1 = l1.sizeHint().height();

    QFontMetricsF fontMetrics(QApplication::font());
    qreal leading = fontMetrics.leading();
    qreal ascent = fontMetrics.ascent();
    qreal descent = fontMetrics.descent();

    bool leadingOverflow = qCeil(ascent + descent) < qCeil(ascent + descent + leading);
    if (leadingOverflow)
        QEXPECT_FAIL("", "See BOBUIBUG-82954", Continue);
    QCOMPARE(h1, h);
}

void tst_QLabel::task226479_movieResize()
{
    class Label : public QLabel {
        protected:
            void paintEvent(QPaintEvent *e) override
            {
                paintedRegion += e->region();
                QLabel::paintEvent(e);
            }

        public:
            QRegion paintedRegion;
    };

    Label label;
    label.resize(350,350);
    label.show();
    QMovie *movie = new QMovie( &label );
    label.setMovie(movie);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&label));
    movie->setFileName(QFINDTESTDATA("red.png"));
    movie->start();
    BOBUIest::qWait(50);
    movie->stop();
    label.paintedRegion = QRegion();
    movie->setFileName(QFINDTESTDATA("green.png"));
    movie->start();

    BOBUIRY_COMPARE(label.paintedRegion , QRegion(label.rect()) );
}

void tst_QLabel::emptyPixmap()
{
    //task 197919
    QLabel label1, label2, label3, label4;
    label2.setPixmap(QPixmap("/tmp/idonotexist"));
    QMovie movie;
    label3.setMovie(&movie);
    label4.setPicture(QPicture());
    QCOMPARE(label1.sizeHint(), label2.sizeHint());
    QCOMPARE(label1.sizeHint(), label3.sizeHint());
    QCOMPARE(label1.sizeHint(), label4.sizeHint());
}

/**
    Test for BOBUIBUG-4848 - unicode data corrupting QLabel display
*/
void tst_QLabel::unicodeText_data()
{
    BOBUIest::addColumn<QString>("text");

    /*
    The "glass" phrase in Thai was the initial report for bug BOBUIBUG-4848, was
    originally found on http://www.columbia.edu/kermit/utf8.html.

    The phrase is from an internet tradition regarding a striking phrase
    that is translated into many different languages.  The utf8 strings
    below were generated by using http://translate.google.com.

    The glass phrase in Thai contains the ้ว character which manifests bug
    BOBUIBUG-4848

    The last long phrase is an excerpt from Churchills "on the beaches"
    speech, also translated using http://translate.google.com.
    */

    BOBUIest::newRow("english") << QString::fromUtf8("I can eat glass and it doesn't hurt me.");
    BOBUIest::newRow("thai") << QString::fromUtf8("ฉันจะกินแก้วและไม่เจ็บฉัน");
    BOBUIest::newRow("chinese") << QString::fromUtf8("我可以吃玻璃，并没有伤害我。");
    BOBUIest::newRow("arabic") << QString::fromUtf8("أستطيع أكل الزجاج ، وأنه لا يؤذيني.");
    BOBUIest::newRow("russian") << QString::fromUtf8("Я могу есть стекло, и не больно.");
    BOBUIest::newRow("korean") << QString::fromUtf8("유리를 먹을 수있는, 그리고 그게 날 다치게하지 않습니다.");
    BOBUIest::newRow("greek") << QString::fromUtf8("Μπορώ να φάτε γυαλί και δεν μου κάνει κακό.");
    BOBUIest::newRow("german") << QString::fromUtf8("Ich kann Glas essen und es macht mich nicht heiß.");
    BOBUIest::newRow("thai_long") << QString::fromUtf8("เราจะต่อสู้ในทะเลและมหาสมุทร. เราจะต่อสู้ด้วยความมั่นใจเติบโตและความเจริญเติบโตในอากาศเราจะปกป้องเกาะของเราค่าใช้จ่ายใดๆอาจ."
                                                    "เราจะต่อสู้บนชายหาดเราจะต่อสู้ในบริเวณเชื่อมโยงไปถึงเราจะต่อสู้ในช่องและในถนนที่เราจะต่อสู้ในภูเขานั้นเราจะไม่ยอม.");
}

void tst_QLabel::unicodeText()
{
    QFETCH(QString, text);
    QFrame frame;
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label = new QLabel(text, &frame);
    layout->addWidget(label);
    layout->setContentsMargins(8, 8, 8, 8);
    frame.setLayout(layout);
    frame.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&frame));
    QVERIFY(frame.isVisible());  // was successfully sized and shown
    testWidget->show();
}

#if BOBUI_CONFIG(shortcut)

void tst_QLabel::mnemonic_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QString>("expectedDocText");
    BOBUIest::addColumn<QString>("expectedShortcutCursor");

    BOBUIest::newRow("1") << QString("Normal") << QString("Normal") << QString();
    BOBUIest::newRow("2") << QString("&Simple") << QString("Simple") << QString("S");
    BOBUIest::newRow("3") << QString("Also &simple") << QString("Also simple") << QString("s");
    BOBUIest::newRow("4") << QString("&&With &Double &&amp;") << QString("&With Double &amp;") << QString("D");
    BOBUIest::newRow("5") << QString("Hep&&Hop") << QString("Hep&Hop") << QString("");
    BOBUIest::newRow("6") << QString("Hep&&&Hop") << QString("Hep&Hop") << QString("H");
}


void tst_QLabel::mnemonic()
{
    // this test that the mnemonics appears correctly when the label has a text control.

    QFETCH(QString, text);
    QFETCH(QString, expectedDocText);
    QFETCH(QString, expectedShortcutCursor);

    QWidget w;
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *lab = new QLabel(text);
    //lab->setText("plop &plop");
    QLineEdit *lineedit = new QLineEdit;
    lab->setBuddy(lineedit);
    lab->setTextInteractionFlags(BobUI::TextSelectableByMouse);

    hbox->addWidget(lab);
    hbox->addWidget(lineedit);
    hbox->addWidget(new QLineEdit);
    w.setLayout(hbox);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    QLabelPrivate *d = static_cast<QLabelPrivate *>(QObjectPrivate::get(lab));
    QVERIFY(d->control);
    QCOMPARE(d->control->document()->toPlainText(), expectedDocText);
    QCOMPARE(d->shortcutCursor.selectedText(), expectedShortcutCursor);
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QLabel::selection()
{
    QLabel label;
    label.setText("Hello world");

    label.setTextInteractionFlags(BobUI::TextSelectableByMouse);

    QVERIFY(!label.hasSelectedText());
    QCOMPARE(label.selectedText(), QString());
    QCOMPARE(label.selectionStart(), -1);

    label.setSelection(0, 4);
    QVERIFY(label.hasSelectedText());
    QCOMPARE(label.selectedText(), QString::fromLatin1("Hell"));
    QCOMPARE(label.selectionStart(), 0);

    label.setSelection(6, 5);
    QVERIFY(label.hasSelectedText());
    QCOMPARE(label.selectedText(), QString::fromLatin1("world"));
    QCOMPARE(label.selectionStart(), 6);
}

#ifndef BOBUI_NO_CONTEXTMENU
void tst_QLabel::taskBOBUIBUG_7902_contextMenuCrash()
{
    QLabel *w = new QLabel("Test or crash?");
    w->setTextInteractionFlags(BobUI::TextSelectableByMouse);
    w->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(w));

    BOBUIimer ti;
    w->connect(&ti, SIGNAL(timeout()), w, SLOT(deleteLater()));
    ti.start(300);

    QContextMenuEvent *cme = new QContextMenuEvent(QContextMenuEvent::Mouse, w->rect().center(),
                                                   w->mapToGlobal(w->rect().center()));
    qApp->postEvent(w, cme);

    BOBUIest::qWait(350);
    // No crash, it's allright.
}

void tst_QLabel::contextMenu_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<BobUI::TextInteractionFlag>("interactionFlags");
    BOBUIest::addColumn<bool>("showsContextMenu");

    BOBUIest::addRow("Read-only")  << "Plain Text"
                                << BobUI::NoTextInteraction
                                << false;
    BOBUIest::addRow("Selectable") << "Plain Text"
                                << BobUI::TextEditorInteraction
                                << true;
    BOBUIest::addRow("Link")       << "<a href=\"nowhere\">Rich text with link</a>"
                                << BobUI::TextBrowserInteraction
                                << true;
    BOBUIest::addRow("Rich text")  << "<b>Rich text without link</b>"
                                << BobUI::TextBrowserInteraction
                                << true;
}

void tst_QLabel::contextMenu()
{
    QFETCH(QString, text);
    QFETCH(BobUI::TextInteractionFlag, interactionFlags);
    QFETCH(bool, showsContextMenu);

    QLabel label(text);
    label.setTextInteractionFlags(interactionFlags);
    label.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&label));

    const QPoint menuPosition = label.rect().center();
    QContextMenuEvent cme(QContextMenuEvent::Mouse, menuPosition, label.mapToGlobal(menuPosition));
    QApplication::sendEvent(&label, &cme);
    QCOMPARE(cme.isAccepted(), showsContextMenu);
}
#endif

void tst_QLabel::taskBOBUIBUG_48157_dprPixmap()
{
    QLabel label;
    QPixmap pixmap;
    pixmap.load(QFINDTESTDATA(QStringLiteral("red@2x.png")));
    QCOMPARE(pixmap.devicePixelRatio(), 2.0);
    label.setPixmap(pixmap);
    QCOMPARE(label.sizeHint(), pixmap.deviceIndependentSize().toSize());
}

void tst_QLabel::taskBOBUIBUG_48157_dprMovie()
{
    QLabel label;
    QMovie movie;
    movie.setFileName(QFINDTESTDATA(QStringLiteral("red@2x.png")));
    movie.start();
    QCOMPARE(movie.currentPixmap().devicePixelRatio(), 2.0);
    label.setMovie(&movie);
    QCOMPARE(label.sizeHint(), movie.currentPixmap().deviceIndependentSize().toSize());
}

void tst_QLabel::resourceProvider()
{
    QLabel label;
    int providerCalled = 0;
    QUrl providerUrl;
    label.setResourceProvider([&](const QUrl &url){
        providerUrl = url;
        ++providerCalled;
        return QVariant();
    });

    const QUrl url("test://img");
    label.setText(QStringLiteral("<img src='%1'/>").arg(url.toString()));
    label.show();
    QCOMPARE(providerUrl, url);
    QVERIFY(providerCalled > 0);
}

// Test if mouse events are correctly propagated to the parent widget,
// even if a label contains rich text (BOBUIBUG-110055)
void tst_QLabel::mouseEventPropagation_data()
{
    BOBUIest::addColumn<const QString>("text");
    BOBUIest::addColumn<const BobUI::TextInteractionFlag>("interaction");
    BOBUIest::addColumn<const QList<BobUI::MouseButton>>("buttons");
    BOBUIest::addColumn<const bool>("expectPropagation");


    BOBUIest::newRow("RichText")
            << QString("<b>This is a rich text propagating mouse events</b>")
            << BobUI::LinksAccessibleByMouse
            << QList<BobUI::MouseButton>{BobUI::LeftButton, BobUI::RightButton, BobUI::MiddleButton}
            << true;
    BOBUIest::newRow("PlainText")
            << QString("This is a plain text propagating mouse events")
            << BobUI::LinksAccessibleByMouse
            << QList<BobUI::MouseButton>{BobUI::LeftButton, BobUI::RightButton, BobUI::MiddleButton}
            << true;
    BOBUIest::newRow("PlainTextConsume")
            << QString("This is a plain text consuming mouse events")
            << BobUI::TextSelectableByMouse
            << QList<BobUI::MouseButton>{BobUI::LeftButton}
            << false;
    BOBUIest::newRow("RichTextConsume")
            << QString("<b>This is a rich text consuming mouse events</b>")
            << BobUI::TextSelectableByMouse
            << QList<BobUI::MouseButton>{BobUI::LeftButton}
            << false;
    BOBUIest::newRow("PlainTextNoInteraction")
            << QString("This is a text not interacting with mouse")
            << BobUI::NoTextInteraction
            << QList<BobUI::MouseButton>{BobUI::LeftButton, BobUI::RightButton, BobUI::MiddleButton}
            << true;
    BOBUIest::newRow("RichTextNoInteraction")
            << QString("<b>This is a rich text not interacting with mouse</b>")
            << BobUI::NoTextInteraction
            << QList<BobUI::MouseButton>{BobUI::LeftButton, BobUI::RightButton, BobUI::MiddleButton}
            << true;
}

void tst_QLabel::mouseEventPropagation()
{
    class MouseEventWidget : public QWidget
    {
    public:
        uint pressed() const { return m_pressed; }
        uint released() const { return m_released; }

    private:
        uint m_pressed = 0;
        uint m_released = 0;
        void mousePressEvent(QMouseEvent *event) override
        {
            ++m_pressed;
            return QWidget::mousePressEvent(event);
        }

        void mouseReleaseEvent(QMouseEvent *event) override
        {
            ++m_released;
            return QWidget::mouseReleaseEvent(event);
        }
    };

    QFETCH(const QString, text);
    QFETCH(const BobUI::TextInteractionFlag, interaction);
    QFETCH(const QList<BobUI::MouseButton>, buttons);
    QFETCH(const bool, expectPropagation);

    MouseEventWidget widget;
    auto *layout = new QVBoxLayout(&widget);
    auto *label = new QLabel(text);
    label->setTextInteractionFlags(interaction);

    layout->addWidget(label);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    const QPoint labelCenter = label->rect().center();
    for (BobUI::MouseButton mouseButton : buttons)
        BOBUIest::mouseClick(label, mouseButton, BobUI::KeyboardModifiers(), labelCenter);

    const uint count = expectPropagation ? buttons.count() : 0;
    BOBUIRY_COMPARE(widget.pressed(), count);
    BOBUIRY_COMPARE(widget.released(), count);
}

BOBUIEST_MAIN(tst_QLabel)
#include "tst_qlabel.moc"
