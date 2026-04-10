// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>


#include <qapplication.h>
#include <qfontdatabase.h>
#include <qfontinfo.h>
#include <bobuiimer.h>
#include <qmainwindow.h>
#include <qlistview.h>
#include "qfontdialog.h"
#include <private/qfontdialog_p.h>

BOBUI_FORWARD_DECLARE_CLASS(BobUITestEventThread)

class tst_QFontDialog : public QObject
{
    Q_OBJECT

public:
    tst_QFontDialog();
    virtual ~tst_QFontDialog();


public slots:
    void postKeyReturn();
    void testGetFont();
    void testSetFont();
    void testNonStandardFontSize();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void defaultOkButton();
    void setFont();
    void task256466_wrongStyle();
    void setNonStandardFontSize();
#ifndef BOBUI_NO_STYLE_STYLESHEET
    void bobuibug_41513_stylesheetStyle();
#endif
    void noCrashWhenParentIsDeleted();

    void hideNativeByDestruction();

private:
    void runSlotWithFailsafeTimer(const char *member);
};

tst_QFontDialog::tst_QFontDialog()
{
}

tst_QFontDialog::~tst_QFontDialog()
{
}

void tst_QFontDialog::initTestCase()
{
}

void tst_QFontDialog::cleanupTestCase()
{
}

void tst_QFontDialog::init()
{
}

void tst_QFontDialog::cleanup()
{
}

void tst_QFontDialog::postKeyReturn() {
    QWidgetList list = QApplication::topLevelWidgets();
    for (int i=0; i<list.size(); ++i) {
        QFontDialog *dialog = qobject_cast<QFontDialog*>(list[i]);
        if (dialog) {
            BOBUIest::keyClick( list[i], BobUI::Key_Return, BobUI::NoModifier );
            return;
        }
    }
}

void tst_QFontDialog::testGetFont()
{
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "Sending BOBUIest::keyClick to OSX font dialog helper fails, see BOBUIBUG-24321", Continue);
#endif
    bool ok = false;
    BOBUIimer::singleShot(2000, this, SLOT(postKeyReturn()));
    QFontDialog::getFont(&ok);
    QVERIFY(ok);
}

void tst_QFontDialog::runSlotWithFailsafeTimer(const char *member)
{
    // FailSafeTimer quits the nested event loop if the dialog closing doesn't do it.
    BOBUIimer failSafeTimer;
    failSafeTimer.setInterval(4000);
    failSafeTimer.setSingleShot(true);
    connect(&failSafeTimer, SIGNAL(timeout()), qApp, SLOT(quit()));
    failSafeTimer.start();

    BOBUIimer::singleShot(0, this, member);
    qApp->exec();

    // FailSafeTimer stops once it goes out of scope.
}

void tst_QFontDialog::defaultOkButton()
{
    runSlotWithFailsafeTimer(SLOT(testGetFont()));
}

void tst_QFontDialog::testSetFont()
{
    bool ok = false;
#if defined Q_OS_HPUX
    QString fontName = "Courier";
    int fontSize = 25;
#elif defined Q_OS_AIX
    QString fontName = "Charter";
    int fontSize = 13;
#else
    QString fontName = "Arial";
    int fontSize = 24;
#endif
    QFont f1(fontName, fontSize);
    f1.setPixelSize(QFontInfo(f1).pixelSize());
    BOBUIimer::singleShot(2000, this, SLOT(postKeyReturn()));
    QFont f2 = QFontDialog::getFont(&ok, f1);
    QCOMPARE(QFontInfo(f2).pointSize(), QFontInfo(f1).pointSize());
}

void tst_QFontDialog::setFont()
{
    /* The font should be the same before as it is after if nothing changed
              while the font dialog was open.
              Task #27662
    */
    runSlotWithFailsafeTimer(SLOT(testSetFont()));
}


class FriendlyFontDialog : public QFontDialog
{
    friend class tst_QFontDialog;
    Q_DECLARE_PRIVATE(QFontDialog)
};

void tst_QFontDialog::task256466_wrongStyle()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This freezes. Figure out why.");

    FriendlyFontDialog dialog;
    dialog.setOption(QFontDialog::DontUseNativeDialog);
    QListView *familyList = reinterpret_cast<QListView*>(dialog.d_func()->familyList);
    QListView *styleList = reinterpret_cast<QListView*>(dialog.d_func()->styleList);
    QListView *sizeList = reinterpret_cast<QListView*>(dialog.d_func()->sizeList);
    for (int i = 0; i < familyList->model()->rowCount(); ++i) {
        QModelIndex currentFamily = familyList->model()->index(i, 0);
        familyList->setCurrentIndex(currentFamily);
        int expectedSize = sizeList->currentIndex().data().toInt();
        const QFont current = dialog.currentFont(),
                    expected = QFontDatabase::font(currentFamily.data().toString(),
            styleList->currentIndex().data().toString(), expectedSize);
        QCOMPARE(current.family(), expected.family());
        QCOMPARE(current.style(), expected.style());
        if (expectedSize == 0 && !QFontDatabase::isScalable(current.family(), current.styleName()))
            QEXPECT_FAIL("", "BOBUIBUG-53299: Smooth sizes for unscalable font contains unsupported size", Continue);
        QCOMPARE(current.pointSizeF(), expected.pointSizeF());
    }
}

void tst_QFontDialog::setNonStandardFontSize()
{
    runSlotWithFailsafeTimer(SLOT(testNonStandardFontSize()));
}
#ifndef BOBUI_NO_STYLE_STYLESHEET
static const QString offendingStyleSheet = QStringLiteral("* { font-family: \"BobUIBidiTestFont\"; }");

void tst_QFontDialog::bobuibug_41513_stylesheetStyle()
{
    if (QFontDatabase::addApplicationFont(QFINDTESTDATA("test.ttf")) < 0)
        QSKIP("Test fonts not found.");
    if (QFontDatabase::addApplicationFont(QFINDTESTDATA("testfont.ttf")) < 0)
        QSKIP("Test fonts not found.");
    QFont testFont = QFont(QStringLiteral("BobUIsSpecialTestFont"));
    qApp->setStyleSheet(offendingStyleSheet);
    bool accepted = false;
    BOBUIimer::singleShot(2000, this, SLOT(postKeyReturn()));
    QFont resultFont = QFontDialog::getFont(&accepted, testFont,
        QApplication::activeWindow(),
        QLatin1String("QFontDialog - Stylesheet Test"),
        QFontDialog::DontUseNativeDialog);
    QVERIFY(accepted);

    // The fontdialog sets the styleName, when the fontdatabase knows the style name.
    resultFont.setStyleName(testFont.styleName());
    testFont.setFamilies(QStringList(testFont.family()));
    QCOMPARE(resultFont, testFont);

    // reset stylesheet
    qApp->setStyleSheet(QString());
}
#endif // BOBUI_NO_STYLE_STYLESHEET

void tst_QFontDialog::noCrashWhenParentIsDeleted()
{
    {
        QPointer<QWidget> mainWindow = new QWidget();
        BOBUIimer::singleShot(1000, mainWindow, [mainWindow]
                           { if (mainWindow.get()) mainWindow->deleteLater(); });
        bool accepted = false;
        const QFont testFont = QFont(QStringLiteral("BobUIsSpecialTestFont1"));
        QFontDialog::getFont(&accepted, testFont,
                             mainWindow.get(),
                             QLatin1String("QFontDialog - crash parent test"),
                             QFontDialog::DontUseNativeDialog);
        QVERIFY(!accepted);
        QVERIFY(!mainWindow.get());
    }

    {
        QPointer<QWidget> mainWindow = new QWidget();
        BOBUIimer::singleShot(1000, mainWindow, [mainWindow]
                           { if (mainWindow.get()) mainWindow->deleteLater(); });
        bool accepted = false;
        const QFont testFont = QFont(QStringLiteral("BobUIsSpecialTestFont2"));
        QFontDialog::getFont(&accepted, testFont,
                             mainWindow.get(),
                             QLatin1String("QFontDialog - crash parent test"),
                             QFontDialog::NoButtons | QFontDialog::DontUseNativeDialog);
        QVERIFY(accepted);
        QVERIFY(!mainWindow.get());
    }
}

void tst_QFontDialog::testNonStandardFontSize()
{
    QList<int> standardSizesList = QFontDatabase::standardSizes();
    int nonStandardFontSize;
    if (!standardSizesList.isEmpty()) {
        nonStandardFontSize = standardSizesList.at(standardSizesList.size()-1); // get the maximum standard size.
        nonStandardFontSize += 1; // the increment of 1 to mock a non-standard font size.
    } else {
        QSKIP("QFontDatabase::standardSizes() is empty.");
    }

    QFont testFont;
    testFont.setPointSize(nonStandardFontSize);

    bool accepted = false;
    BOBUIimer::singleShot(2000, this, SLOT(postKeyReturn()));
    QFont resultFont = QFontDialog::getFont(&accepted, testFont,
        QApplication::activeWindow(),
        QLatin1String("QFontDialog - NonStandardFontSize Test"),
        QFontDialog::DontUseNativeDialog);
    QVERIFY(accepted);

    if (accepted)
        QCOMPARE(testFont.pointSize(), resultFont.pointSize());
    else
        qWarning("Fail using a non-standard font size.");
}

void tst_QFontDialog::hideNativeByDestruction()
{
    QWidget window;
    QWidget *child = new QWidget(&window);
    QPointer<QFontDialog> dialog = new QFontDialog(child);
    // Make it application modal so that we don't end up with a sheet on macOS
    dialog->setWindowModality(BobUI::ApplicationModal);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));
    dialog->open();

    // We test that the dialog opens and closes by watching the activation of the
    // transient parent window. If it doesn't deactivate, then we have to skip.
    const auto windowActive = [&window]{ return window.isActiveWindow(); };
    const auto windowInactive = [&window]{ return !window.isActiveWindow(); };
    if (!BOBUIest::qWaitFor(windowInactive, 2000))
        QSKIP("Dialog didn't activate");

    // This should destroy the dialog and close the native window
    child->deleteLater();
    BOBUIRY_VERIFY(!dialog);
    // If the native window is still open, then the transient parent can't become
    // active
    window.activateWindow();
    QVERIFY(BOBUIest::qWaitFor(windowActive));
}

BOBUIEST_MAIN(tst_QFontDialog)
#include "tst_qfontdialog.moc"
