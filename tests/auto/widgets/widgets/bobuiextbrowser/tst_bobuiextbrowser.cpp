// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <bobuiextbrowser.h>
#include <qapplication.h>
#include <qscrollbar.h>

#include <bobuiextbrowser.h>
#include <bobuiextobject.h>

#include <BobUIWidgets/private/qapplication_p.h>

class TestBrowser : public BOBUIextBrowser
{
public:
    inline TestBrowser() {
        show();
        activateWindow();
        setFocus();
        QVERIFY(BOBUIest::qWaitForWindowActive(this));
        QVERIFY(hasFocus());
    }

    QVariant loadResource(int type, const QUrl &name) override;

    int htmlLoadAttempts = 0;
    QUrl lastResource;
    QUrl sourceInsideLoadResource;
    QUrl baseInsideLoadResource;
};

QVariant TestBrowser::loadResource(int type, const QUrl &name)
{
    if (type == BOBUIextDocument::HtmlResource)
        htmlLoadAttempts++;
    lastResource = name;
    sourceInsideLoadResource = source();
    baseInsideLoadResource = document()->baseUrl();
    return BOBUIextBrowser::loadResource(type, name);
}

class tst_BOBUIextBrowser : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void noReloadOnAnchorJump();
    void bgColorOnSourceChange();
    void forwardButton();
    void viewportPositionInHistory();
    void relativeLinks();
    void anchors();
    void resourceAutoDetection();
    void forwardBackwardAvailable();
    void clearHistory();
    void sourceInsideLoadResource();
    void textInteractionFlags_vs_readOnly();
    void inputMethodAttribute_vs_readOnly();
    void anchorsWithSelfBuiltHtml();
    void relativeNonLocalUrls();
    void adjacentAnchors();
    void loadResourceOnRelativeLocalFiles();
    void focusIndicator();
    void focusHistory();
    void urlEncoding();
    void sourceType_data();
    void sourceType();
    void unicode_data();
    void unicode();

private:
    TestBrowser *browser;
};

void tst_BOBUIextBrowser::initTestCase()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");
}

void tst_BOBUIextBrowser::init()
{
    QString prefix = QFileInfo(QFINDTESTDATA("subdir")).absolutePath();
    QVERIFY2(!prefix.isEmpty(), "Test data directory not found");
    QDir::setCurrent(prefix);

    browser = new TestBrowser;
    browser->show();
}

void tst_BOBUIextBrowser::cleanup()
{
    delete browser;
    browser = nullptr;
}

void tst_BOBUIextBrowser::noReloadOnAnchorJump()
{
    QUrl url = QUrl::fromLocalFile(QFINDTESTDATA("anchor.html"));

    browser->htmlLoadAttempts = 0;
    browser->setSource(url);
    QCOMPARE(browser->htmlLoadAttempts, 1);
    QVERIFY(!browser->toPlainText().isEmpty());

    url.setFragment("jumphere"); // anchor.html#jumphere
    browser->setSource(url);
    QCOMPARE(browser->htmlLoadAttempts, 1);
    QVERIFY(!browser->toPlainText().isEmpty());
    QCOMPARE(browser->source(), url);
}

void tst_BOBUIextBrowser::bgColorOnSourceChange()
{
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("pagewithbg.html")));
    QVERIFY(browser->document()->rootFrame()->frameFormat().hasProperty(BOBUIextFormat::BackgroundBrush));
    QCOMPARE(browser->document()->rootFrame()->frameFormat().background().color(), QColor(BobUI::blue));

    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("pagewithoutbg.html")));
    QVERIFY(!browser->document()->rootFrame()->frameFormat().hasProperty(BOBUIextFormat::BackgroundBrush));
}

void tst_BOBUIextBrowser::forwardButton()
{
    QSignalSpy forwardEmissions(browser, SIGNAL(forwardAvailable(bool)));
    QSignalSpy backwardEmissions(browser, SIGNAL(backwardAvailable(bool)));

    QVERIFY(browser->historyTitle(-1).isEmpty());
    QVERIFY(browser->historyTitle(0).isEmpty());
    QVERIFY(browser->historyTitle(1).isEmpty());

    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("pagewithbg.html")));

    QVERIFY(!forwardEmissions.isEmpty());
    QVariant val = forwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(!val.toBool());

    QVERIFY(!backwardEmissions.isEmpty());
    val = backwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(!val.toBool());

    QVERIFY(browser->historyTitle(-1).isEmpty());
    QCOMPARE(browser->historyUrl(0), QUrl::fromLocalFile(QFINDTESTDATA("pagewithbg.html")));
    QCOMPARE(browser->documentTitle(), QString("Page With BG"));
    QCOMPARE(browser->historyTitle(0), QString("Page With BG"));
    QVERIFY(browser->historyTitle(1).isEmpty());

    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("anchor.html")));

    QVERIFY(!forwardEmissions.isEmpty());
    val = forwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(!val.toBool());

    QVERIFY(!backwardEmissions.isEmpty());
    val = backwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(val.toBool());

    QCOMPARE(browser->historyTitle(-1), QString("Page With BG"));
    QCOMPARE(browser->historyTitle(0), QString("Sample Anchor"));
    QVERIFY(browser->historyTitle(1).isEmpty());

    browser->backward();

    QVERIFY(!forwardEmissions.isEmpty());
    val = forwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(val.toBool());

    QVERIFY(!backwardEmissions.isEmpty());
    val = backwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(!val.toBool());

    QVERIFY(browser->historyTitle(-1).isEmpty());
    QCOMPARE(browser->historyTitle(0), QString("Page With BG"));
    QCOMPARE(browser->historyTitle(1), QString("Sample Anchor"));

    browser->setSource(QUrl(QFINDTESTDATA("pagewithoutbg.html")));

    QVERIFY(!forwardEmissions.isEmpty());
    val = forwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(!val.toBool());

    QVERIFY(!backwardEmissions.isEmpty());
    val = backwardEmissions.takeLast()[0];
    QCOMPARE(val.userType(), QMetaType::Bool);
    QVERIFY(val.toBool());
}

void tst_BOBUIextBrowser::viewportPositionInHistory()
{
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("bigpage.html")));
    browser->scrollToAnchor("bottom");
    QVERIFY(browser->verticalScrollBar()->value() > 0);

    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("pagewithbg.html")));
    QCOMPARE(browser->verticalScrollBar()->value(), 0);

    browser->backward();
    QVERIFY(browser->verticalScrollBar()->value() > 0);
}

void tst_BOBUIextBrowser::relativeLinks()
{
#ifdef BUILTIN_TESTDATA
    QSKIP("Relative links cannot be checked when resources are used to package tests.");
#endif
    QSignalSpy sourceChangedSpy(browser, SIGNAL(sourceChanged(QUrl)));
    browser->setSource(QUrl("subdir/../bobuiextbrowser.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("subdir/../bobuiextbrowser.html"));
    browser->setSource(QUrl("subdir/index.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("subdir/index.html"));
    browser->setSource(QUrl("anchor.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("anchor.html"));
    browser->setSource(QUrl("subdir/index.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("subdir/index.html"));

    // using QUrl::fromLocalFile()
    browser->setSource(QUrl::fromLocalFile("anchor.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("file:anchor.html"));
    browser->setSource(QUrl("subdir/../bobuiextbrowser.html"));
    QVERIFY(!browser->document()->isEmpty());
    QCOMPARE(sourceChangedSpy.size(), 1);
    QCOMPARE(sourceChangedSpy.takeFirst()[0].toUrl(), QUrl("subdir/../bobuiextbrowser.html"));
}

void tst_BOBUIextBrowser::anchors()
{
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("bigpage.html")));
    browser->setSource(QUrl("#bottom"));
    QVERIFY(browser->verticalScrollBar()->value() > 0);

    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("bigpage.html")));
    browser->setSource(QUrl("#id-anchor"));
    QVERIFY(browser->verticalScrollBar()->value() > 0);
}

void tst_BOBUIextBrowser::resourceAutoDetection()
{
    browser->setHtml("<img src=\":/some/resource\"/>");
    QCOMPARE(browser->lastResource.toString(), QString("qrc:/some/resource"));
}

void tst_BOBUIextBrowser::forwardBackwardAvailable()
{
    QSignalSpy backwardSpy(browser, SIGNAL(backwardAvailable(bool)));
    QSignalSpy forwardSpy(browser, SIGNAL(forwardAvailable(bool)));

    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());

    browser->setSource(QUrl::fromLocalFile("anchor.html"));
    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(!backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->setSource(QUrl::fromLocalFile("bigpage.html"));
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->setSource(QUrl::fromLocalFile("pagewithbg.html"));
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->backward();
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->backward();
    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(!backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->forward();
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->forward();
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();
}

void tst_BOBUIextBrowser::clearHistory()
{
    QSignalSpy backwardSpy(browser, SIGNAL(backwardAvailable(bool)));
    QSignalSpy forwardSpy(browser, SIGNAL(forwardAvailable(bool)));

    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());

    browser->clearHistory();
    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(!backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());
    QVERIFY(browser->historyTitle(-1).isEmpty());
    QVERIFY(browser->historyTitle(0).isEmpty());
    QVERIFY(browser->historyTitle(1).isEmpty());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->setSource(QUrl::fromLocalFile("anchor.html"));
    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(!backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->setSource(QUrl::fromLocalFile("bigpage.html"));
    QVERIFY(browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());

    backwardSpy.clear();
    forwardSpy.clear();

    browser->clearHistory();
    QVERIFY(!browser->isBackwardAvailable());
    QVERIFY(!browser->isForwardAvailable());
    QCOMPARE(backwardSpy.size(), 1);
    QVERIFY(!backwardSpy.at(0).at(0).toBool());
    QCOMPARE(forwardSpy.size(), 1);
    QVERIFY(!forwardSpy.at(0).at(0).toBool());
    QVERIFY(browser->historyTitle(-1).isEmpty());
    QVERIFY(browser->historyTitle(1).isEmpty());

    QCOMPARE(browser->source(), QUrl::fromLocalFile("bigpage.html"));
    browser->backward();
    QCOMPARE(browser->source(), QUrl::fromLocalFile("bigpage.html"));
    browser->home();
    QCOMPARE(browser->source(), QUrl::fromLocalFile("bigpage.html"));
}

void tst_BOBUIextBrowser::sourceInsideLoadResource()
{
    QUrl url = QUrl::fromLocalFile("pagewithimage.html"); // "file://pagewithimage.html"
    browser->setSource(url);
    QCOMPARE(browser->lastResource, QUrl::fromLocalFile(QDir::current().filePath("foobar.png")));
    // baseUrl was not set because the source URL was a relative one
    QCOMPARE(browser->baseInsideLoadResource, QUrl());
    QEXPECT_FAIL("", "This is currently not supported", Continue);
    QCOMPARE(browser->sourceInsideLoadResource.toString(), url.toString());
    url = QUrl::fromLocalFile(QDir::current().filePath("pagewithimage.html")); // "file:///home/user/path/to/pagewithimage.html"
    browser->setSource(url);
    QCOMPARE(browser->lastResource, QUrl::fromLocalFile(QDir::current().filePath("foobar.png")));
    // baseUrl has the full path, and that's where relative-path resources come from
    QCOMPARE(browser->baseInsideLoadResource, QUrl::fromLocalFile(QDir::currentPath() + QLatin1Char('/')));
}

void tst_BOBUIextBrowser::textInteractionFlags_vs_readOnly()
{
    QVERIFY(browser->isReadOnly());
    QCOMPARE(browser->textInteractionFlags(), BobUI::TextBrowserInteraction);
    browser->setReadOnly(true);
    QCOMPARE(browser->textInteractionFlags(), BobUI::TextBrowserInteraction);
    browser->setReadOnly(false);
    QCOMPARE(browser->textInteractionFlags(), BobUI::TextEditorInteraction);
    browser->setReadOnly(true);
    QCOMPARE(browser->textInteractionFlags(), BobUI::TextBrowserInteraction);
}

void tst_BOBUIextBrowser::inputMethodAttribute_vs_readOnly()
{
    QVERIFY(browser->isReadOnly());
#if defined(Q_OS_ANDROID)
    QInputMethodQueryEvent query(BobUI::ImReadOnly);
    QCoreApplication::sendEvent(browser, &query);
    QVERIFY(query.value(BobUI::ImReadOnly).toBool());
#else
    QVERIFY(!browser->testAttribute(BobUI::WA_InputMethodEnabled));
#endif

    browser->setReadOnly(false);
#if defined(Q_OS_ANDROID)
    QCoreApplication::sendEvent(browser, &query);
    QVERIFY(!query.value(BobUI::ImReadOnly).toBool());
#else
    QVERIFY(browser->testAttribute(BobUI::WA_InputMethodEnabled));
#endif

    browser->setReadOnly(true);
#if defined(Q_OS_ANDROID)
    QCoreApplication::sendEvent(browser, &query);
    QVERIFY(query.value(BobUI::ImReadOnly).toBool());
#else
    QVERIFY(!browser->testAttribute(BobUI::WA_InputMethodEnabled));
#endif
}

void tst_BOBUIextBrowser::anchorsWithSelfBuiltHtml()
{
    browser->setHtml("<p>Hello <a href=\"#anchor\">Link</a>"
                     "<p><a name=\"anchor\"/>Blah</p>");
    QVERIFY(browser->document()->blockCount() > 1);
    browser->setSource(QUrl("#anchor"));
    QVERIFY(browser->document()->blockCount() > 1);
}

class HelpBrowser : public BOBUIextBrowser
{
public:
    virtual QVariant loadResource(int /*type*/, const QUrl &name) override
    {
        QString url = name.toString();
        if(url == "qhelp://docs/index.html") {
            return "index";
        } else if (url == "qhelp://docs/classes.html") {
            return "classes";
        } else if (url == "qhelp://docs/someclass.html") {
            return "someclass";
        }
        return QVariant();
    }
};

void tst_BOBUIextBrowser::relativeNonLocalUrls()
{
    HelpBrowser browser;
    browser.setSource(QUrl("qhelp://docs/index.html"));
    QCOMPARE(browser.toPlainText(), QString("index"));
    browser.setSource(QUrl("classes.html"));
    QCOMPARE(browser.toPlainText(), QString("classes"));
    browser.setSource(QUrl("someclass.html"));
    QCOMPARE(browser.toPlainText(), QString("someclass"));
}

class HackBrowser : public TestBrowser
{
public:
    inline bool focusTheNextChild() { return BOBUIextBrowser::focusNextChild(); }
    inline bool focusThePreviousChild() { return BOBUIextBrowser::focusPreviousChild(); }
};

void tst_BOBUIextBrowser::adjacentAnchors()
{
    HackBrowser *browser = new HackBrowser;
    browser->setHtml("<a href=\"#foo\">foo</a><a href=\"#bar\">bar</a>");
    QVERIFY(browser->focusTheNextChild());
    QCOMPARE(browser->textCursor().selectedText(), QString("foo"));

    QVERIFY(browser->focusTheNextChild());
    QCOMPARE(browser->textCursor().selectedText(), QString("bar"));

    QVERIFY(!browser->focusTheNextChild());

    browser->moveCursor(BOBUIextCursor::End);
    QVERIFY(browser->focusThePreviousChild());
    QCOMPARE(browser->textCursor().selectedText(), QString("bar"));
    QVERIFY(browser->focusThePreviousChild());
    QCOMPARE(browser->textCursor().selectedText(), QString("foo"));

    delete browser;
}

void tst_BOBUIextBrowser::loadResourceOnRelativeLocalFiles()
{
#ifndef BUILTIN_TESTDATA
    browser->setSource(QUrl::fromLocalFile("subdir/index.html"));
#else
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("subdir/index.html")));
#endif
    QVERIFY(!browser->toPlainText().isEmpty());
    QVariant v = browser->loadResource(BOBUIextDocument::HtmlResource, QUrl("../anchor.html"));
    QVERIFY(v.isValid());
    QCOMPARE(v.userType(), QMetaType::QByteArray);
    QVERIFY(!v.toByteArray().isEmpty());
}

void tst_BOBUIextBrowser::focusIndicator()
{
    HackBrowser *browser = new HackBrowser;
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("firstpage.html")));
    QVERIFY(!browser->textCursor().hasSelection());

    browser->focusTheNextChild();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to second page"));

#ifdef BOBUI_KEYPAD_NAVIGATION
    browser->setEditFocus(true);
#endif
    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QVERIFY(!browser->textCursor().hasSelection());

    browser->focusTheNextChild();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QVERIFY(!browser->textCursor().hasSelection());

    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to second page"));

    browser->forward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    browser->backward();
    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to second page"));

    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QVERIFY(!browser->textCursor().hasSelection());

    delete browser;
}

void tst_BOBUIextBrowser::focusHistory()
{
    HackBrowser *browser = new HackBrowser;
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA("firstpage.html")));
    QVERIFY(!browser->textCursor().hasSelection());

    browser->focusTheNextChild();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to second page"));

#ifdef BOBUI_KEYPAD_NAVIGATION
    browser->setEditFocus(true);
#endif
    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QVERIFY(!browser->textCursor().hasSelection());

    browser->focusTheNextChild();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to second page"));

    browser->focusTheNextChild();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page"));

    // Despite the third page link being highlighted, going forward should go to second,
    // and going back after that should still highlight the third link
    browser->forward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page"));

    browser->forward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page from second page"));

    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QVERIFY(!browser->textCursor().hasSelection());

    browser->backward();
    browser->backward();

    QVERIFY(browser->textCursor().hasSelection());
    QCOMPARE(browser->textCursor().selectedText(), QString("Link to third page"));

    delete browser;
}

void tst_BOBUIextBrowser::urlEncoding()
{
    HackBrowser *browser = new HackBrowser;
    browser->setOpenLinks(false);
    browser->setHtml("<a href=\"http://www.google.com/q=%22\">link</a>");
    browser->focusTheNextChild();

    QSignalSpy spy(browser, SIGNAL(anchorClicked(QUrl)));

#ifdef BOBUI_KEYPAD_NAVIGATION
    browser->setEditFocus(true);
#endif
    BOBUIest::keyClick(browser, BobUI::Key_Enter);
    QCOMPARE(spy.size(), 1);

    QUrl url = spy.at(0).at(0).toUrl();
    QCOMPARE(url.toEncoded(), QByteArray("http://www.google.com/q=%22"));

    delete browser;
}

void tst_BOBUIextBrowser::sourceType_data()
{
    BOBUIest::addColumn<QString>("sourceFile");
    BOBUIest::addColumn<BOBUIextDocument::ResourceType>("sourceType");
    BOBUIest::addColumn<int>("expectedMaxHeadingLevel");
    BOBUIest::addColumn<BOBUIextDocument::ResourceType>("expectedSourceType");

#if BOBUI_CONFIG(textmarkdownreader)
    const int maxMdHeadingLevel = 3;
    const BOBUIextDocument::ResourceType mdExpectedType = BOBUIextDocument::MarkdownResource;
#else
    // If BobUI doesn't support markdown, and we read a MD document anyway, it won't have any H3's.
    const int maxMdHeadingLevel = 0;
    const BOBUIextDocument::ResourceType mdExpectedType = BOBUIextDocument::HtmlResource;
#endif
    BOBUIest::newRow("markdown detected") << "markdown.md" << BOBUIextDocument::UnknownResource << maxMdHeadingLevel << mdExpectedType;
    BOBUIest::newRow("markdown specified") << "markdown.really" << BOBUIextDocument::MarkdownResource << maxMdHeadingLevel << mdExpectedType;
    BOBUIest::newRow("markdown not identified") << "markdown.really" << BOBUIextDocument::UnknownResource << 0 << BOBUIextDocument::HtmlResource;
    BOBUIest::newRow("html detected") << "heading.html" << BOBUIextDocument::UnknownResource << 3 << BOBUIextDocument::HtmlResource;
    BOBUIest::newRow("html specified") << "heading.html" << BOBUIextDocument::HtmlResource << 3 << BOBUIextDocument::HtmlResource;
}

void tst_BOBUIextBrowser::sourceType()
{
    QFETCH(QString, sourceFile);
    QFETCH(BOBUIextDocument::ResourceType, sourceType);
    QFETCH(int, expectedMaxHeadingLevel);
    QFETCH(BOBUIextDocument::ResourceType, expectedSourceType);
    if (sourceType == BOBUIextDocument::UnknownResource)
        // verify that the property setter works, with its default parameter for sourceType
        browser->setProperty("source", QUrl::fromLocalFile(QFINDTESTDATA(sourceFile)));
    else
        browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA(sourceFile)), sourceType);
    QCOMPARE(browser->sourceType(), expectedSourceType);
    BOBUIextFrame::iterator iterator = browser->document()->rootFrame()->begin();
    int maxHeadingLevel = -1;
    while (!iterator.atEnd())
        maxHeadingLevel = qMax(iterator++.currentBlock().blockFormat().intProperty(BOBUIextFormat::HeadingLevel), maxHeadingLevel);
    QCOMPARE(maxHeadingLevel, expectedMaxHeadingLevel);
}

void tst_BOBUIextBrowser::unicode_data()
{
    BOBUIest::addColumn<QString>("sourceFile");
    BOBUIest::addColumn<BOBUIextDocument::ResourceType>("sourceType");
    BOBUIest::addColumn<QString>("expectedText");

#if BOBUI_CONFIG(textmarkdownreader)
    BOBUIest::newRow("markdown with quotes and fractions") << "quotesAndFractions.md" << BOBUIextDocument::MarkdownResource <<
        "you\u2019ll hope to see \u275Dquotes\u275E \uFE601\u00BD \u2154 \u00BC \u2157 \u215A \u215D some \u201Cvulgar\u201D fractions (pardon my \u00ABFrench\u00BB)";
#endif
}

void tst_BOBUIextBrowser::unicode()
{
    QFETCH(QString, sourceFile);
    QFETCH(BOBUIextDocument::ResourceType, sourceType);
    QFETCH(QString, expectedText);
    browser->setSource(QUrl::fromLocalFile(QFINDTESTDATA(sourceFile)), sourceType);
    BOBUIextFrame::iterator iterator = browser->document()->rootFrame()->begin();
    while (!iterator.atEnd()) {
        QString blockText = iterator++.currentBlock().text();
        if (!blockText.isEmpty())
            QCOMPARE(blockText, expectedText);
    }
}

BOBUIEST_MAIN(tst_BOBUIextBrowser)
#include "tst_bobuiextbrowser.moc"
