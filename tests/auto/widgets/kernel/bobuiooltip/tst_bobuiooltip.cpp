// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BOBUIimer>

#include <qfont.h>
#include <qfontmetrics.h>
#include <bobuiooltip.h>
#include <qwhatsthis.h>
#include <qscreen.h>

#include <BobUIWidgets/private/qapplication_p.h>

class tst_BOBUIoolTip : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void keyEvent_data();
    void keyEvent();
    void whatsThis();
    void setPalette();
    void bobuibug64550_stylesheet();
    void dontCrashOutsideScreenGeometry();
    void marginSetWithStyleSheet();
};

void tst_BOBUIoolTip::init()
{
    QVERIFY(!BOBUIoolTip::isVisible());
}

void tst_BOBUIoolTip::cleanup()
{
    BOBUIRY_VERIFY(QApplication::topLevelWidgets().isEmpty());
    qApp->setStyleSheet(QString());
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr) : QWidget(parent) {}

    void showDelayedToolTip(int msecs)
    {
        BOBUIimer::singleShot(msecs, this, SLOT(showToolTip()));
    }

    static inline QString toolTipText() { return QStringLiteral("tool tip text"); }

private slots:
    void showToolTip()
    {
        BOBUIoolTip::showText(mapToGlobal(QPoint(0, 0)), Widget::toolTipText(), this);
    }
};

Q_DECLARE_METATYPE(BobUI::Key)

void tst_BOBUIoolTip::keyEvent_data()
{
    BOBUIest::addColumn<BobUI::Key>("key");
    BOBUIest::addColumn<bool>("visible");

    BOBUIest::newRow("non-modifier") << BobUI::Key_A <<
#if defined(Q_OS_MACOS)
        // macOS natively hides tooltips on non-modifier key events,
        // so BOBUIipLabel::eventFilter does the same. Match that here.
        false;
#else
        true;
#endif
    BOBUIest::newRow("Shift") << BobUI::Key_Shift << true;
    BOBUIest::newRow("Control") << BobUI::Key_Control << true;
    BOBUIest::newRow("Alt") << BobUI::Key_Alt << true;
    BOBUIest::newRow("Meta") << BobUI::Key_Meta << true;
}

void tst_BOBUIoolTip::keyEvent()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(BobUI::Key, key);
    QFETCH(bool, visible);

    Widget widget;
    widget.move(QGuiApplication::primaryScreen()->availableGeometry().topLeft() + QPoint(50, 50));
    // Ensure cursor is not over tooltip, which causes it to hide
#ifndef BOBUI_NO_CURSOR
    QCursor::setPos(widget.geometry().topRight() + QPoint(-50, 50));
#endif
    widget.setWindowTitle(QLatin1String(BOBUIest::currentTestFunction())
                          + QLatin1Char(' ') + QLatin1String(BOBUIest::currentDataTag()));
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));

    widget.showDelayedToolTip(100);
    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());

    BOBUIest::keyPress(&widget, key);

    // Important: the following delay must be larger than the duration of the timer potentially
    // initiated by the key press (currently 300 msecs), but smaller than the minimum
    // auto-close timeout (currently 10000 msecs)
    BOBUIest::qWait(1500);

    QCOMPARE(BOBUIoolTip::isVisible(), visible);
    if (visible)
        BOBUIoolTip::hideText();
}

static QWidget *findWhatsThat()
{
    const auto widgets = QApplication::topLevelWidgets();
    for (QWidget *widget : widgets) {
        if (widget->inherits("QWhatsThat"))
            return widget;
    }
    return nullptr;
}

void tst_BOBUIoolTip::whatsThis()
{
    qApp->setStyleSheet( "QWidget { font-size: 72px; }" );
    QWhatsThis::showText(QPoint(0, 0), "This is text");

    QWidget *whatsthis = nullptr;
    BOBUIRY_VERIFY( (whatsthis = findWhatsThat()) );
    QVERIFY(whatsthis->isVisible());
    const int whatsThisHeight = whatsthis->height();
    qApp->setStyleSheet(QString());
    QWhatsThis::hideText();
    QVERIFY2(whatsThisHeight > 100, QByteArray::number(whatsThisHeight)); // Test BOBUIBUG-2416
}

static QWidget *findToolTip()
{
    const QWidgetList &topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (widget->windowType() == BobUI::ToolTip && widget->objectName() == QLatin1String("bobuiooltip_label"))
            return widget;
    }
    return nullptr;
}

void tst_BOBUIoolTip::setPalette()
{
    //the previous test may still have a tooltip pending for deletion
    QVERIFY(!BOBUIoolTip::isVisible());

    BOBUIoolTip::showText(QPoint(), "tool tip text", 0);

    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());

    QWidget *toolTip = findToolTip();
    QVERIFY(toolTip);
    BOBUIRY_VERIFY(toolTip->isVisible());

    const QPalette oldPalette = toolTip->palette();
    QPalette newPalette = oldPalette;
    newPalette.setColor(QPalette::ToolTipBase, BobUI::red);
    newPalette.setColor(QPalette::ToolTipText, BobUI::blue);
    BOBUIoolTip::setPalette(newPalette);
    QCOMPARE(toolTip->palette(), newPalette);
    BOBUIoolTip::hideText();
}

static QByteArray msgSizeTooSmall(const QSize &actual, const QSize &expected)
{
    return QByteArray::number(actual.width()) + 'x'
        + QByteArray::number(actual.height()) + " < "
        + QByteArray::number(expected.width())  + 'x'
        + QByteArray::number(expected.height());
}

// BOBUIBUG-4550: When setting a style sheet specifying a font size on the tooltip's
// parent widget (as opposed to setting on QApplication), the tooltip should
// resize accordingly. This is an issue on Windows since the ToolTip widget is
// not directly parented on the widget itself.
// Set a large font size and verify that the tool tip is big enough.
void tst_BOBUIoolTip::bobuibug64550_stylesheet()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    Widget widget;
    widget.setStyleSheet(QStringLiteral("* { font-size: 48pt; }\n"));
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));

    widget.showDelayedToolTip(100);
    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());
    QWidget *toolTip = findToolTip();
    QVERIFY(toolTip);
    BOBUIRY_VERIFY(toolTip->isVisible());

    const QRect boundingRect = QFontMetrics(widget.font()).boundingRect(Widget::toolTipText());
    const QSize toolTipSize = toolTip->size();
    QVERIFY2(toolTipSize.width() >= boundingRect.width()
             && toolTipSize.height() >= boundingRect.height(),
             msgSizeTooSmall(toolTipSize, boundingRect.size()).constData());
}

void tst_BOBUIoolTip::dontCrashOutsideScreenGeometry() {
    BOBUIoolTip::showText(QPoint(-10000, -10000), "tip outside monitor", nullptr);
    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());
    BOBUIoolTip::hideText();
}

void tst_BOBUIoolTip::marginSetWithStyleSheet()
{
    const char *toolTipText = "Test Tool Tip";

    qApp->setStyleSheet("BOBUIoolTip {font-size: 8px; margin: 5px;}");
    BOBUIoolTip::showText(QGuiApplication::primaryScreen()->availableGeometry().topLeft(), toolTipText);
    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());
    QWidget *toolTip = findToolTip();
    QVERIFY(toolTip);
    BOBUIRY_VERIFY(toolTip->isVisible());
    int toolTipHeight = toolTip->size().height();
    qApp->setStyleSheet(QString());
    BOBUIoolTip::hideText();

    qApp->setStyleSheet("BOBUIoolTip {font-size: 8px; margin: 10px;}");
    BOBUIoolTip::showText(QGuiApplication::primaryScreen()->availableGeometry().topLeft(), toolTipText);
    BOBUIRY_VERIFY(BOBUIoolTip::isVisible());
    toolTip = findToolTip();
    QVERIFY(toolTip);
    BOBUIRY_VERIFY(toolTip->isVisible());
    QCOMPARE_LE(toolTip->size().height(), toolTipHeight + 10);
    qApp->setStyleSheet(QString());
    BOBUIoolTip::hideText();
}

BOBUIEST_MAIN(tst_BOBUIoolTip)
#include "tst_bobuiooltip.moc"
