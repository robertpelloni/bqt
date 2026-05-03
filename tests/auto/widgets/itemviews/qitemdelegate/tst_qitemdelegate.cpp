// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "../../../shared/highdpi.h"

#include <BOBUIest>
#include <BOBUIimeZone>
#include <QDateTime>
#include <BOBUIimer>
#include <BOBUIestEventLoop>
#include <QSignalSpy>

#include <qabstractitemview.h>
#include <qstandarditemmodel.h>
#include <qapplication.h>
#include <qdatetimeedit.h>
#include <qspinbox.h>
#include <qlistview.h>
#include <bobuiableview.h>
#include <bobuireeview.h>
#include <qheaderview.h>
#include <qitemeditorfactory.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <bobuiablewidget.h>
#include <bobuireewidget.h>

#include <QItemDelegate>
#include <QComboBox>
#include <QAbstractItemDelegate>
#include <BOBUIextEdit>
#include <QPlainTextEdit>
#include <QDialog>

#include <qscreen.h>

#include <BobUIWidgets/private/qabstractitemdelegate_p.h>
#include <BobUIWidgets/private/qapplication_p.h>

Q_DECLARE_METATYPE(QAbstractItemDelegate::EndEditHint)

#if defined (Q_OS_WIN)
#include <bobui_windows.h>
#define Q_CHECK_PAINTEVENTS \
    if (::SwitchDesktop(::GetThreadDesktop(::GetCurrentThreadId())) == 0) \
        QSKIP("The widgets don't get the paint events");
#else
#define Q_CHECK_PAINTEVENTS
#endif

//Begin of class definitions

class TestItemDelegate : public QItemDelegate
{
public:
    TestItemDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}
    ~TestItemDelegate() {}

    void drawDisplay(QPainter *painter,
                     const QStyleOptionViewItem &option,
                     const QRect &rect, const QString &text) const override
    {
        displayText = text;
        displayFont = option.font;
        QItemDelegate::drawDisplay(painter, option, rect, text);
    }

    void drawDecoration(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QRect &rect, const QPixmap &pixmap) const override
    {
        decorationPixmap = pixmap;
        decorationRect = rect;
        QItemDelegate::drawDecoration(painter, option, rect, pixmap);
    }


    inline QRect textRectangle(QPainter * painter, const QRect &rect,
                               const QFont &font, const QString &text) const
    {
        return QItemDelegate::textRectangle(painter, rect, font, text);
    }

    inline void doLayout(const QStyleOptionViewItem &option,
                         QRect *checkRect, QRect *pixmapRect,
                         QRect *textRect, bool hint) const
    {
        QItemDelegate::doLayout(option, checkRect, pixmapRect, textRect, hint);
    }

    inline QRect rect(const QStyleOptionViewItem &option,
                      const QModelIndex &index, int role) const
    {
        return QItemDelegate::rect(option, index, role);
    }

    inline bool eventFilter(QObject *object, QEvent *event) override
    {
        return QItemDelegate::eventFilter(object, event);
    }

    inline bool editorEvent(QEvent *event,
                            QAbstractItemModel *model,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) override
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    // stored values for testing
    mutable QString displayText;
    mutable QFont displayFont;
    mutable QPixmap decorationPixmap;
    mutable QRect decorationRect;
};

class TestItemModel : public QAbstractTableModel
{
public:

    enum Roles {
        PixmapTestRole,
        ImageTestRole,
        IconTestRole,
        ColorTestRole,
        DoubleTestRole
    };

    TestItemModel(const QSize &size) : size(size) {}

    ~TestItemModel() {}

    int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return 1;
    }

    int columnCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return 1;
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        Q_UNUSED(index);
        static QPixmap pixmap(size);
        static QImage image(size, QImage::Format_Mono);
        static QIcon icon(pixmap);
        static QColor color(BobUI::green);

        switch (role) {
        case PixmapTestRole: return pixmap;
        case ImageTestRole:  return image;
        case IconTestRole:   return icon;
        case ColorTestRole:  return color;
        case DoubleTestRole:  return 10.00000001;
        default: break;
        }

        return QVariant();
    }

private:
    QSize size;
};

class tst_QItemDelegate : public QObject
{
    Q_OBJECT

private slots:
    void getSetCheck();
    void textRectangle_data();
    void textRectangle();
    void sizeHint_data();
    void sizeHint();
    void editorKeyPress_data();
    void editorKeyPress();
    void doubleEditorNegativeInput();
    void font_data();
    void font();
    void doLayout_data();
    void doLayout();
    void rect_data();
    void rect();
    void testEventFilter();
    void dateTimeEditor_data();
    void dateTimeEditor();
    void dateAndTimeEditorTest2();
    void uintEdit();
    void decoration_data();
    void decoration();
    void editorEvent_data();
    void editorEvent();
    void enterKey_data();
    void enterKey();
    void comboBox();
    void testLineEditValidation_data();
    void testLineEditValidation();

    void task257859_finalizeEdit();
    void BOBUIBUG4435_keepSelectionOnCheck();

    void BOBUIBUG16469_textForRole();
    void dateTextForRole_data();
    void dateTextForRole();

    void reuseEditor();

private:
#ifdef BOBUI_BUILD_INTERNAL
    struct RoleDelegate : public QItemDelegate
    {
        QString textForRole(BobUI::ItemDataRole role, const QVariant &value, const QLocale &locale)
        {
            QAbstractItemDelegatePrivate *d = reinterpret_cast<QAbstractItemDelegatePrivate *>(qGetPtrHelper(d_ptr));
            return d->textForRole(role, value, locale);
        }
    };
#endif

    const int m_fuzz = int(QGuiApplication::primaryScreen()->devicePixelRatio());
};


//End of class definitions

// Testing get/set functions
void tst_QItemDelegate::getSetCheck()
{
    QItemDelegate obj1;

    // QItemEditorFactory * QItemDelegate::itemEditorFactory()
    // void QItemDelegate::setItemEditorFactory(QItemEditorFactory *)
    QItemEditorFactory *var1 = new QItemEditorFactory;
    obj1.setItemEditorFactory(var1);
    QCOMPARE(var1, obj1.itemEditorFactory());
    obj1.setItemEditorFactory((QItemEditorFactory *)0);
    QCOMPARE((QItemEditorFactory *)0, obj1.itemEditorFactory());
    delete var1;

    QCOMPARE(obj1.hasClipping(), true);
    obj1.setClipping(false);
    QCOMPARE(obj1.hasClipping(), false);
    obj1.setClipping(true);
    QCOMPARE(obj1.hasClipping(), true);
}

void tst_QItemDelegate::textRectangle_data()
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int pm = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin);
    int margins = 2 * (pm + 1); // margin on each side of the text
    int height = fontMetrics.height();

    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QRect>("rect");
    BOBUIest::addColumn<QRect>("expected");

    BOBUIest::newRow("empty") << QString()
                           << QRect()
                           << QRect(0, 0, margins, height);
}

void tst_QItemDelegate::textRectangle()
{
    QFETCH(QString, text);
    QFETCH(QRect, rect);
    QFETCH(QRect, expected);

    QFont font;
    TestItemDelegate delegate;
    QRect result = delegate.textRectangle(0, rect, font, text);
    QVERIFY2(HighDpi::fuzzyCompare(result, expected, m_fuzz),
             HighDpi::msgRectMismatch(result, expected).constData());
}

void tst_QItemDelegate::sizeHint_data()
{
    BOBUIest::addColumn<QSize>("expected");

    QFont font;
    QFontMetrics fontMetrics(font);
    //int m = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    BOBUIest::newRow("empty")
        << QSize(0, fontMetrics.height());

}

void tst_QItemDelegate::sizeHint()
{
    QFETCH(QSize, expected);

    QModelIndex index;
    QStyleOptionViewItem option;

    TestItemDelegate delegate;
    QSize result = delegate.sizeHint(option, index);
    QCOMPARE(result, expected);
}

void tst_QItemDelegate::editorKeyPress_data()
{
    BOBUIest::addColumn<QString>("initial");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("foo bar")
        << QString("foo")
        << QString("bar");
}

void tst_QItemDelegate::editorKeyPress()
{
    QFETCH(QString, initial);
    QFETCH(QString, expected);

    QStandardItemModel model;
    model.appendRow(new QStandardItem(initial));

    QListView view;
    view.setModel(&model);
    view.show();

    QModelIndex index = model.index(0, 0);
    view.setCurrentIndex(index); // the editor will only selectAll on the current index
    view.edit(index);

    QList<QLineEdit*> lineEditors = view.viewport()->findChildren<QLineEdit *>();
    QCOMPARE(lineEditors.size(), 1);

    QLineEdit *editor = lineEditors.at(0);
    QCOMPARE(editor->selectedText(), initial);

    BOBUIest::keyClicks(editor, expected);
    BOBUIest::keyClick(editor, BobUI::Key_Enter);

    BOBUIRY_COMPARE(index.data().toString(), expected);
}

void tst_QItemDelegate::doubleEditorNegativeInput()
{
    QStandardItemModel model;

    QStandardItem *item = new QStandardItem;
    item->setData(10.0, BobUI::DisplayRole);
    model.appendRow(item);

    QListView view;
    view.setModel(&model);
    view.show();

    QModelIndex index = model.index(0, 0);
    view.setCurrentIndex(index); // the editor will only selectAll on the current index
    view.edit(index);

    QList<QDoubleSpinBox*> editors = view.viewport()->findChildren<QDoubleSpinBox *>();
    QCOMPARE(editors.size(), 1);

    QDoubleSpinBox *editor = editors.at(0);
    QCOMPARE(editor->value(), double(10));

    BOBUIest::keyClick(editor, BobUI::Key_Minus);
    BOBUIest::keyClick(editor, BobUI::Key_1);
    BOBUIest::keyClick(editor, BobUI::Key_0);
    BOBUIest::keyClick(editor, BobUI::Key_Comma); //support both , and . locales
    BOBUIest::keyClick(editor, BobUI::Key_Period);
    BOBUIest::keyClick(editor, BobUI::Key_0);
    BOBUIest::keyClick(editor, BobUI::Key_Enter);
    QApplication::processEvents();

    QCOMPARE(index.data().toString(), QString("-10"));
}

void tst_QItemDelegate::font_data()
{
    BOBUIest::addColumn<QString>("itemText");
    BOBUIest::addColumn<QString>("properties");
    BOBUIest::addColumn<QFont>("itemFont");
    BOBUIest::addColumn<QFont>("viewFont");

    QFont itemFont;
    itemFont.setItalic(true);
    QFont viewFont;

    BOBUIest::newRow("foo italic")
        << QString("foo")
        << QString("italic")
        << itemFont
        << viewFont;

    itemFont.setItalic(true);

    BOBUIest::newRow("foo bold")
        << QString("foo")
        << QString("bold")
        << itemFont
        << viewFont;

    itemFont.setFamily(itemFont.defaultFamily());

    BOBUIest::newRow("foo family")
        << QString("foo")
        << QString("family")
        << itemFont
        << viewFont;
 }

void tst_QItemDelegate::font()
{
    Q_CHECK_PAINTEVENTS

    QFETCH(QString, itemText);
    QFETCH(QString, properties);
    QFETCH(QFont, itemFont);
    QFETCH(QFont, viewFont);

    BOBUIableWidget table(1, 1);
    table.setFont(viewFont);

    TestItemDelegate *delegate = new TestItemDelegate(&table);
    table.setItemDelegate(delegate);
    table.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&table));

    BOBUIableWidgetItem *item = new BOBUIableWidgetItem;
    item->setText(itemText);
    item->setFont(itemFont);
    table.setItem(0, 0, item);

    QApplication::processEvents();

    BOBUIRY_COMPARE(delegate->displayText, item->text());
    if (properties.contains("italic")) {
        QCOMPARE(delegate->displayFont.italic(), item->font().italic());
    }
    if (properties.contains("bold")){
        QCOMPARE(delegate->displayFont.bold(), item->font().bold());
    }
    if (properties.contains("family")){
        QCOMPARE(delegate->displayFont.family(), item->font().family());
    }
}

//Testing the different QRect created by the doLayout function.
//Tests are made with different values for the QStyleOptionViewItem properties:
//decorationPosition and position.

void tst_QItemDelegate::doLayout_data()
{
    BOBUIest::addColumn<int>("position");
    BOBUIest::addColumn<int>("direction");
    BOBUIest::addColumn<bool>("hint");
    BOBUIest::addColumn<QRect>("itemRect");
    BOBUIest::addColumn<QRect>("checkRect");
    BOBUIest::addColumn<QRect>("pixmapRect");
    BOBUIest::addColumn<QRect>("textRect");
    BOBUIest::addColumn<QRect>("expectedCheckRect");
    BOBUIest::addColumn<QRect>("expectedPixmapRect");
    BOBUIest::addColumn<QRect>("expectedTextRect");

    int m = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    //int item = 400;
    //int check = 50;
    //int pixmap = 1000;
    //int text = 400;

    BOBUIest::newRow("top, left to right, hint")
        << (int)QStyleOptionViewItem::Top
        << (int)BobUI::LeftToRight
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50 + 2*m, 1000)
        << QRect(50 + 2*m, 0, 1000 + 2*m, 1000 + m)
        << QRect(50 + 2*m, 1000 + m, 1000 + 2*m, 400);
    /*
    BOBUIest::newRow("top, left to right, limited")
        << (int)QStyleOptionViewItem::Top
        << (int)BobUI::LeftToRight
        << false
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(m, (400/2) - (50/2), 50, 50)
        << QRect(50 + 2*m, 0, 1000, 1000)
        << QRect(50 + 2*m, 1000 + m, 400 - (50 + 2*m), 400 - 1000 - m);
    */
    BOBUIest::newRow("top, right to left, hint")
        << (int)QStyleOptionViewItem::Top
        << (int)BobUI::RightToLeft
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(1000 + 2 * m, 0, 50 + 2 * m, 1000)
        << QRect(0, 0, 1000 + 2 * m, 1000 + m)
        << QRect(0, 1000 + m, 1000 + 2 * m, 400);

    BOBUIest::newRow("bottom, left to right, hint")
        << (int)QStyleOptionViewItem::Bottom
        << (int)BobUI::LeftToRight
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50 + 2 * m, 1000)
        << QRect(50 + 2 * m, 400 + m, 1000 + 2 * m, 1000)
        << QRect(50 + 2 * m, 0, 1000 + 2 * m, 400 + m);

    BOBUIest::newRow("bottom, right to left, hint")
        << (int)QStyleOptionViewItem::Bottom
        << (int)BobUI::RightToLeft
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(1000 + 2 * m, 0, 50 + 2 * m, 1000)
        << QRect(0, 400 + m, 1000 + 2 * m, 1000)
        << QRect(0, 0, 1000 + 2 * m, 400 + m);

    BOBUIest::newRow("left, left to right, hint")
        << (int)QStyleOptionViewItem::Left
        << (int)BobUI::LeftToRight
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50 + 2 * m, 1000)
        << QRect(50 + 2 * m, 0, 1000 + 2 * m, 1000)
        << QRect(1050 + 4 * m, 0, 400 + 2 * m, 1000);

    BOBUIest::newRow("left, right to left, hint")
        << (int)QStyleOptionViewItem::Left
        << (int)BobUI::RightToLeft
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(1400 + 4 * m, 0, 50 + 2 * m, 1000)
        << QRect(400 + 2 * m, 0, 1000 + 2 * m, 1000)
        << QRect(0, 0, 400 + 2 * m, 1000);

    BOBUIest::newRow("right, left to right, hint")
        << (int)QStyleOptionViewItem::Right
        << (int)BobUI::LeftToRight
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50 + 2 * m, 1000)
        << QRect(450 + 4 * m, 0, 1000 + 2 * m, 1000)
        << QRect(50 + 2 * m, 0, 400 + 2 * m, 1000);

    BOBUIest::newRow("right, right to left, hint")
        << (int)QStyleOptionViewItem::Right
        << (int)BobUI::RightToLeft
        << true
        << QRect(0, 0, 400, 400)
        << QRect(0, 0, 50, 50)
        << QRect(0, 0, 1000, 1000)
        << QRect(0, 0, 400, 400)
        << QRect(1400 + 4 * m, 0, 50 + 2 * m, 1000)
        << QRect(0, 0, 1000 + 2 * m, 1000)
        << QRect(1000 + 2 * m, 0, 400 + 2 * m, 1000);
}

void tst_QItemDelegate::doLayout()
{
    QFETCH(int, position);
    QFETCH(int, direction);
    QFETCH(bool, hint);
    QFETCH(QRect, itemRect);
    QFETCH(QRect, checkRect);
    QFETCH(QRect, pixmapRect);
    QFETCH(QRect, textRect);
    QFETCH(QRect, expectedCheckRect);
    QFETCH(QRect, expectedPixmapRect);
    QFETCH(QRect, expectedTextRect);

    TestItemDelegate delegate;
    QStyleOptionViewItem option;

    option.rect = itemRect;
    option.decorationPosition = (QStyleOptionViewItem::Position)position;
    option.direction = (BobUI::LayoutDirection)direction;

    delegate.doLayout(option, &checkRect, &pixmapRect, &textRect, hint);

    QCOMPARE(checkRect, expectedCheckRect);
    QCOMPARE(pixmapRect, expectedPixmapRect);
    QCOMPARE(textRect, expectedTextRect);
}

void tst_QItemDelegate::rect_data()
{
    BOBUIest::addColumn<int>("role");
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<QRect>("expected");

    BOBUIest::newRow("pixmap")
        << (int)TestItemModel::PixmapTestRole
        << QSize(200, 300)
        << QRect(0, 0, 200, 300);

    BOBUIest::newRow("image")
        << (int)TestItemModel::ImageTestRole
        << QSize(200, 300)
        << QRect(0, 0, 200, 300);

    BOBUIest::newRow("icon")
        << (int)TestItemModel::IconTestRole
        << QSize(200, 300)
        << QRect(0, 0, 200, 300);

    BOBUIest::newRow("color")
        << (int)TestItemModel::ColorTestRole
        << QSize(200, 300)
        << QRect(0, 0, 200, 300);

    BOBUIest::newRow("double")
            << (int)TestItemModel::DoubleTestRole
            << QSize()
            << QRect();
}

void tst_QItemDelegate::rect()
{
    QFETCH(int, role);
    QFETCH(QSize, size);
    QFETCH(QRect, expected);

    TestItemModel model(size);
    QStyleOptionViewItem option;
    TestItemDelegate delegate;
    option.decorationSize = size;

    if (role == TestItemModel::DoubleTestRole)
        expected = delegate.textRectangle(0, QRect(), QFont(), QLatin1String("10.00000001"));

    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());
    QRect result = delegate.rect(option, index, role);
    QCOMPARE(result, expected);
}

//TODO : Add a test for the keyPress event
//with BobUI::Key_Enter and BobUI::Key_Return
void tst_QItemDelegate::testEventFilter()
{
    TestItemDelegate delegate;
    QWidget widget;
    QEvent *event;

    qRegisterMetaType<QAbstractItemDelegate::EndEditHint>("QAbstractItemDelegate::EndEditHint");

    QSignalSpy commitDataSpy(&delegate, SIGNAL(commitData(QWidget*)));
    QSignalSpy closeEditorSpy(&delegate,
                              SIGNAL(closeEditor(QWidget *,
                                                 QAbstractItemDelegate::EndEditHint)));

    //Subtest KeyPress
    //For each test we send a key event and check if signals were emitted.
    event = new QKeyEvent(QEvent::KeyPress, BobUI::Key_Tab, BobUI::NoModifier);
    QVERIFY(delegate.eventFilter(&widget, event));
    QCOMPARE(closeEditorSpy.size(), 1);
    QCOMPARE(commitDataSpy.size(), 1);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, BobUI::Key_Backtab, BobUI::NoModifier);
    QVERIFY(delegate.eventFilter(&widget, event));
    QCOMPARE(closeEditorSpy.size(), 2);
    QCOMPARE(commitDataSpy.size(), 2);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, BobUI::Key_Escape, BobUI::NoModifier);
    QVERIFY(delegate.eventFilter(&widget, event));
    QCOMPARE(closeEditorSpy.size(), 3);
    QCOMPARE(commitDataSpy.size(), 2);
    delete event;

    event = new QKeyEvent(QEvent::KeyPress, BobUI::Key_A, BobUI::NoModifier);
    QVERIFY(!delegate.eventFilter(&widget, event));
    QCOMPARE(closeEditorSpy.size(), 3);
    QCOMPARE(commitDataSpy.size(), 2);
    delete event;

    //Subtest focusEvent
    event = new QFocusEvent(QEvent::FocusOut);
    QVERIFY(!delegate.eventFilter(&widget, event));
    QCOMPARE(closeEditorSpy.size(), 4);
    QCOMPARE(commitDataSpy.size(), 3);
    delete event;
}

void tst_QItemDelegate::dateTimeEditor_data()
{
    BOBUIest::addColumn<BOBUIime>("time");
    BOBUIest::addColumn<QDate>("date");

    BOBUIest::newRow("data")
        << BOBUIime(7, 16, 34)
        << QDate(2006, 10, 31);
}

static QDateTimeEdit *findDateTimeEdit(const QWidget *widget)
{
    const auto dateTimeEditors = widget->findChildren<QDateTimeEdit *>();
    for (auto dateTimeEditor : dateTimeEditors) {
        if (qstrcmp(dateTimeEditor->metaObject()->className(), "QDateTimeEdit") == 0)
            return dateTimeEditor;
    }
    return nullptr;
}

void tst_QItemDelegate::dateTimeEditor()
{
    QFETCH(BOBUIime, time);
    QFETCH(QDate, date);

    BOBUIableWidgetItem *item1 = new BOBUIableWidgetItem;
    item1->setData(BobUI::DisplayRole, time);

    BOBUIableWidgetItem *item2 = new BOBUIableWidgetItem;
    item2->setData(BobUI::DisplayRole, date);

    BOBUIableWidgetItem *item3 = new BOBUIableWidgetItem;
    item3->setData(BobUI::DisplayRole, QDateTime(date, time));

    BOBUIableWidget widget(1, 3);
    widget.setWindowTitle(QLatin1String(BOBUIest::currentTestFunction())
                          + QLatin1String("::")
                          + QLatin1String(BOBUIest::currentDataTag()));
    widget.setItem(0, 0, item1);
    widget.setItem(0, 1, item2);
    widget.setItem(0, 2, item3);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    widget.editItem(item1);

    BOBUIestEventLoop::instance().enterLoop(1);


    BOBUIimeEdit *timeEditor = nullptr;
    auto viewport = widget.viewport();
    BOBUIRY_VERIFY( (timeEditor = viewport->findChild<BOBUIimeEdit *>()) );
    QCOMPARE(timeEditor->time(), time);
    // The data must actually be different in order for the model
    // to be updated.
    timeEditor->setTime(time.addSecs(60));

    widget.clearFocus();
    widget.setFocus();
    widget.editItem(item2);

    QDateEdit *dateEditor = nullptr;
    BOBUIRY_VERIFY( (dateEditor = viewport->findChild<QDateEdit *>()) );
    QCOMPARE(dateEditor->date(), date);
    dateEditor->setDate(date.addDays(60));

    widget.clearFocus();
    widget.setFocus();
    widget.editItem(item3);

    BOBUIestEventLoop::instance().enterLoop(1);

    QDateTimeEdit *dateTimeEditor = nullptr;
    BOBUIRY_VERIFY( (dateTimeEditor = findDateTimeEdit(viewport)) );
    QCOMPARE(dateTimeEditor->date(), date);
    QCOMPARE(dateTimeEditor->time(), time);
    dateTimeEditor->setTime(time.addSecs(600));
    widget.clearFocus();

    QCOMPARE(item1->data(BobUI::EditRole).userType(), int(QMetaType::BOBUIime));
    QCOMPARE(item2->data(BobUI::EditRole).userType(), int(QMetaType::QDate));
    QCOMPARE(item3->data(BobUI::EditRole).userType(), int(QMetaType::QDateTime));
}

// A delegate where we can either enforce a certain widget or use the standard widget.
class ChooseEditorDelegate : public QItemDelegate
{
public:
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &o, const QModelIndex &i) const override
    {
        if (m_editor) {
            m_editor->setParent(parent);
            return m_editor;
        }
        m_editor = QItemDelegate::createEditor(parent, o, i);
        return m_editor;
    }

    virtual void destroyEditor(QWidget *editor, const QModelIndex &i) const override
    {   // This is a reimplementation of QAbstractItemDelegate::destroyEditor just set the variable m_editor to 0
        // The only reason we do this is to avoid the not recommended direct delete of editor (destroyEditor uses deleteLater)
        QItemDelegate::destroyEditor(editor, i); // Allow destroy
        m_editor = 0;                            // but clear the variable
    }

    ChooseEditorDelegate(QObject *parent = nullptr) : QItemDelegate(parent) { }
    void setNextOpenEditor(QWidget *w) { m_editor = w; }
    QWidget* currentEditor() const { return m_editor; }
private:
    mutable QPointer<QWidget> m_editor;
};

// We could (nearly) use a normal BOBUIableView but in order not to add many seconds to the autotest
// (and save a few lines) we do this
class FastEditItemView : public BOBUIableView
{
public:
    QWidget* fastEdit(const QModelIndex &i) // Consider this as QAbstractItemView::edit( )
    {
        QStyleOptionViewItem option;
        initViewItemOption(&option);
        QWidget *v = itemDelegate()->createEditor(viewport(), option, i);
        if (v)
            itemDelegate()->setEditorData(v, i);
        return v;
    }
    void doCloseEditor(QWidget *editor) // Consider this as QAbstractItemView::closeEditor( )
    {
        itemDelegate()->destroyEditor(editor, QModelIndex());
    }
};

void tst_QItemDelegate::dateAndTimeEditorTest2()
{
    // prepare createeditor
    FastEditItemView w;
    QStandardItemModel s;
    s.setRowCount(2);
    s.setColumnCount(1);
    w.setModel(&s);
    ChooseEditorDelegate *d = new ChooseEditorDelegate(&w);
    w.setItemDelegate(d);
    const BOBUIime time1(3, 13, 37);
    const QDate date1(2013, 3, 7);

    QPointer<BOBUIimeEdit> timeEdit;
    QPointer<QDateEdit> dateEdit;
    QPointer<QDateTimeEdit> dateTimeEdit;

    // Do some checks
    // a. Open time editor on empty cell + write BOBUIime data
    const QModelIndex i1 = s.index(0, 0);
    timeEdit = new BOBUIimeEdit();
    d->setNextOpenEditor(timeEdit);
    QCOMPARE(w.fastEdit(i1), timeEdit.data());
    timeEdit->setTime(time1);
    d->setModelData(timeEdit, &s, i1);
    QCOMPARE(s.data(i1).metaType().id(), QMetaType::BOBUIime); // ensure that we wrote a time variant.
    QCOMPARE(s.data(i1).toTime(), time1);        // ensure that it is the correct time.
    w.doCloseEditor(timeEdit);
    QVERIFY(d->currentEditor() == 0); // should happen at doCloseEditor. We only test this once.

    // b. Test that automatic edit of a BOBUIime value is BOBUIimeEdit (and not QDateTimeEdit)
    QWidget *editor = w.fastEdit(i1);
    timeEdit = qobject_cast<BOBUIimeEdit*>(editor);
    QVERIFY(timeEdit);
    QCOMPARE(timeEdit->time(), time1);
    w.doCloseEditor(timeEdit);

    const BOBUIime time2(4, 14, 37);
    const QDate date2(2014, 4, 7);
    const QDateTime datetime1(date1, time1);
    const QDateTime datetime2(date2, time2);

    // c. Test that the automatic open of an QDateTime is QDateTimeEdit + value check + set check
    s.setData(i1, datetime2);
    editor = w.fastEdit(i1);
    timeEdit = qobject_cast<BOBUIimeEdit*>(editor);
    QVERIFY(!timeEdit);
    dateEdit = qobject_cast<QDateEdit*>(editor);
    QVERIFY(!dateEdit);
    dateTimeEdit =  qobject_cast<QDateTimeEdit*>(editor);
    QVERIFY(dateTimeEdit);
    QCOMPARE(dateTimeEdit->dateTime(), datetime2);
    dateTimeEdit->setDateTime(datetime1);
    d->setModelData(dateTimeEdit, &s, i1);
    QCOMPARE(s.data(i1).metaType().id(), QMetaType::QDateTime); // ensure that we wrote a datetime variant.
    QCOMPARE(s.data(i1).toDateTime(), datetime1);
    w.doCloseEditor(dateTimeEdit);

    // d. Open date editor on empty cell + write QDate data (similar to a)
    const QModelIndex i2 = s.index(1, 0);
    dateEdit = new QDateEdit();
    d->setNextOpenEditor(dateEdit);
    QCOMPARE(w.fastEdit(i2), dateEdit.data());
    dateEdit->setDate(date1);
    d->setModelData(dateEdit, &s, i2);
    QCOMPARE(s.data(i2).metaType().id(), QMetaType::QDate); // ensure that we wrote a time variant.
    QCOMPARE(s.data(i2).toDate(), date1);        // ensure that it is the correct date.
    w.doCloseEditor(dateEdit);

    // e. Test that the default editor editor (QDateEdit) on a QDate (index i2)  (similar to b)
    editor = w.fastEdit(i2);
    dateEdit = qobject_cast<QDateEdit*>(editor);
    QVERIFY(dateEdit);
    QCOMPARE(dateEdit->date(), date1);
    w.doCloseEditor(dateEdit);
}

void tst_QItemDelegate::uintEdit()
{
    QListView view;
    QStandardItemModel model;

    {
        QStandardItem *data=new QStandardItem;
        data->setEditable(true);
        data->setData(QVariant((uint)1), BobUI::DisplayRole);
        model.setItem(0, 0, data);
    }
    {
        QStandardItem *data=new QStandardItem;
        data->setEditable(true);
        data->setData(QVariant((uint)1), BobUI::DisplayRole);
        model.setItem(1, 0, data);
    }

    view.setModel(&model);
    view.setEditTriggers(QAbstractItemView::AllEditTriggers);

    const QModelIndex firstCell = model.index(0, 0);

    QCOMPARE(firstCell.data(BobUI::DisplayRole).userType(), static_cast<int>(QMetaType::UInt));

    view.selectionModel()->setCurrentIndex(model.index(0, 0), QItemSelectionModel::Select);
    view.edit(firstCell);

    QSpinBox *sb = view.findChild<QSpinBox*>();
    QVERIFY(sb);

    sb->stepUp();

    // Select another index to trigger the end of editing.
    const QModelIndex secondCell = model.index(1, 0);
    view.selectionModel()->setCurrentIndex(secondCell, QItemSelectionModel::Select);

    QCOMPARE(firstCell.data(BobUI::DisplayRole).userType(), static_cast<int>(QMetaType::UInt));
    QCOMPARE(firstCell.data(BobUI::DisplayRole).toUInt(), static_cast<uint>(2));


    view.edit(secondCell);

    // The first spinbox is deleted with deleteLater, so it is still there.
    QList<QSpinBox*> sbList = view.findChildren<QSpinBox*>();
    QCOMPARE(sbList.size(), 2);

    sb = sbList.at(1);

    sb->stepDown(); // 1 -> 0
    sb->stepDown(); // 0 (no effect)
    sb->stepDown(); // 0 (no effect)

    // Select another index to trigger the end of editing.
    view.selectionModel()->setCurrentIndex(firstCell, QItemSelectionModel::Select);

    QCOMPARE(secondCell.data(BobUI::DisplayRole).userType(), static_cast<int>(QMetaType::UInt));
    QCOMPARE(secondCell.data(BobUI::DisplayRole).toUInt(), static_cast<uint>(0));
}

void tst_QItemDelegate::decoration_data()
{
    BOBUIest::addColumn<int>("type");
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<QSize>("expected");

    int pm = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);

    BOBUIest::newRow("pixmap 30x30")
        << (int)QMetaType::QPixmap
        << QSize(30, 30)
        << QSize(30, 30);

    BOBUIest::newRow("image 30x30")
        << (int)QMetaType::QImage
        << QSize(30, 30)
        << QSize(30, 30);

//The default engine scales pixmaps down if required, but never up. For WinCE we need bigger IconSize than 30
    BOBUIest::newRow("icon 30x30")
        << (int)QMetaType::QIcon
        << QSize(60, 60)
        << QSize(pm, pm);

    BOBUIest::newRow("color 30x30")
        << (int)QMetaType::QColor
        << QSize(30, 30)
        << QSize(pm, pm);

    // This demands too much memory and potentially hangs. Feel free to uncomment
    // for your own testing.
//    BOBUIest::newRow("pixmap 30x30 big")
//        << (int)QMetaType::QPixmap
//        << QSize(1024, 1024)        // Over 1M
//        << QSize(1024, 1024);
}

void tst_QItemDelegate::decoration()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    Q_CHECK_PAINTEVENTS

    QFETCH(int, type);
    QFETCH(QSize, size);
    QFETCH(QSize, expected);

    BOBUIableWidget table(1, 1);
    TestItemDelegate delegate;
    table.setItemDelegate(&delegate);
    table.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&table));

    QVariant value;
    switch (type) {
    case QMetaType::QPixmap: {
        QPixmap pm(size);
        pm.fill(BobUI::black);
        value = pm;
        break;
    }
    case QMetaType::QImage: {
        QImage img(size, QImage::Format_Mono);
        memset(img.bits(), 0, img.sizeInBytes());
        value = img;
        break;
    }
    case QMetaType::QIcon: {
        QPixmap pm(size);
        pm.fill(BobUI::black);
        value = QIcon(pm);
        break;
    }
    case QMetaType::QColor:
        value = QColor(BobUI::green);
        break;
    default:
        break;
    }

    BOBUIableWidgetItem *item = new BOBUIableWidgetItem;
    item->setData(BobUI::DecorationRole, value);
    table.setItem(0, 0, item);
    item->setSelected(true);

    QApplication::processEvents();

    BOBUIRY_COMPARE(delegate.decorationRect.size(), expected);
}

void tst_QItemDelegate::editorEvent_data()
{
    BOBUIest::addColumn<int>("checkState");
    BOBUIest::addColumn<int>("flags");
    BOBUIest::addColumn<bool>("inCheck");
    BOBUIest::addColumn<int>("type");
    BOBUIest::addColumn<int>("button");
    BOBUIest::addColumn<bool>("edited");
    BOBUIest::addColumn<int>("expectedCheckState");

    const int defaultFlags = (int)(BobUI::ItemIsEditable
            |BobUI::ItemIsSelectable
            |BobUI::ItemIsUserCheckable
            |BobUI::ItemIsEnabled
            |BobUI::ItemIsDragEnabled
            |BobUI::ItemIsDropEnabled);

    BOBUIest::newRow("unchecked, checkable, release")
        << (int)(BobUI::Unchecked)
        << defaultFlags
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Checked);

    BOBUIest::newRow("checked, checkable, release")
        << (int)(BobUI::Checked)
        << defaultFlags
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Unchecked);

    BOBUIest::newRow("unchecked, checkable, release")
        << (int)(BobUI::Unchecked)
        << defaultFlags
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Checked);

    BOBUIest::newRow("unchecked, checkable, release, right button")
        << (int)(BobUI::Unchecked)
        << defaultFlags
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::RightButton)
        << false
        << (int)(BobUI::Unchecked);

    BOBUIest::newRow("unchecked, checkable, release outside")
        << (int)(BobUI::Unchecked)
        << defaultFlags
        << false
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << false
        << (int)(BobUI::Unchecked);

    BOBUIest::newRow("unchecked, checkable, dblclick")
        << (int)(BobUI::Unchecked)
        << defaultFlags
        << true
        << (int)(QEvent::MouseButtonDblClick)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Unchecked);

    BOBUIest::newRow("unchecked, tristate, release")
        << (int)(BobUI::Unchecked)
        << (int)(defaultFlags | BobUI::ItemIsAutoTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Checked);

    BOBUIest::newRow("partially checked, tristate, release")
        << (int)(BobUI::PartiallyChecked)
        << (int)(defaultFlags | BobUI::ItemIsAutoTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Checked);

    BOBUIest::newRow("checked, tristate, release")
        << (int)(BobUI::Checked)
        << (int)(defaultFlags | BobUI::ItemIsAutoTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Unchecked);

    BOBUIest::newRow("unchecked, user-tristate, release")
        << (int)(BobUI::Unchecked)
        << (int)(defaultFlags | BobUI::ItemIsUserTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::PartiallyChecked);

    BOBUIest::newRow("partially checked, user-tristate, release")
        << (int)(BobUI::PartiallyChecked)
        << (int)(defaultFlags | BobUI::ItemIsUserTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Checked);

    BOBUIest::newRow("checked, user-tristate, release")
        << (int)(BobUI::Checked)
        << (int)(defaultFlags | BobUI::ItemIsUserTristate)
        << true
        << (int)(QEvent::MouseButtonRelease)
        << (int)(BobUI::LeftButton)
        << true
        << (int)(BobUI::Unchecked);
}

void tst_QItemDelegate::editorEvent()
{
    QFETCH(int, checkState);
    QFETCH(int, flags);
    QFETCH(bool, inCheck);
    QFETCH(int, type);
    QFETCH(int, button);
    QFETCH(bool, edited);
    QFETCH(int, expectedCheckState);

    QStandardItemModel model(1, 1);
    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());

    QStandardItem *item = model.itemFromIndex(index);
    item->setText("foo");
    item->setCheckState((BobUI::CheckState)checkState);
    item->setFlags((BobUI::ItemFlags)flags);

    QStyleOptionViewItem option;
    option.rect = QRect(0, 0, 20, 20);
    option.state |= QStyle::State_Enabled;
    // mimic QStyledItemDelegate::initStyleOption logic
    option.features |= QStyleOptionViewItem::HasCheckIndicator | QStyleOptionViewItem::HasDisplay;
    option.checkState = BobUI::CheckState(checkState);

    const int checkMargin = qApp->style()->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, 0) + 1;
    QPoint pos = inCheck ? qApp->style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option, 0).center() + QPoint(checkMargin, 0) : QPoint(200,200);

    QEvent *event = new QMouseEvent((QEvent::Type)type,
                                    pos, pos,
                                    (BobUI::MouseButton)button,
                                    (BobUI::MouseButton)button,
                                    BobUI::NoModifier);
    TestItemDelegate delegate;
    bool wasEdited = delegate.editorEvent(event, &model, option, index);
    delete event;

    QApplication::processEvents();

    QCOMPARE(wasEdited, edited);
    QCOMPARE(index.data(BobUI::CheckStateRole).toInt(), expectedCheckState);
}

enum WidgetType
{
    LineEdit,
    TextEdit,
    PlainTextEdit
};
Q_DECLARE_METATYPE(WidgetType);

void tst_QItemDelegate::enterKey_data()
{
    BOBUIest::addColumn<WidgetType>("widget");
    BOBUIest::addColumn<int>("key");
    BOBUIest::addColumn<bool>("expectedFocus");

    BOBUIest::newRow("lineedit enter") << LineEdit << int(BobUI::Key_Enter) << false;
    BOBUIest::newRow("lineedit return") << LineEdit << int(BobUI::Key_Return) << false;
    BOBUIest::newRow("lineedit tab") << LineEdit << int(BobUI::Key_Tab) << false;
    BOBUIest::newRow("lineedit backtab") << LineEdit << int(BobUI::Key_Backtab) << false;

    BOBUIest::newRow("textedit enter") << TextEdit << int(BobUI::Key_Enter) << true;
    BOBUIest::newRow("textedit return") << TextEdit << int(BobUI::Key_Return) << true;
    BOBUIest::newRow("textedit tab") << TextEdit << int(BobUI::Key_Tab) << true;
    BOBUIest::newRow("textedit backtab") << TextEdit << int(BobUI::Key_Backtab) << false;

    BOBUIest::newRow("plaintextedit enter") << PlainTextEdit << int(BobUI::Key_Enter) << true;
    BOBUIest::newRow("plaintextedit return") << PlainTextEdit << int(BobUI::Key_Return) << true;
    BOBUIest::newRow("plaintextedit tab") << PlainTextEdit << int(BobUI::Key_Tab) << true;
    BOBUIest::newRow("plaintextedit backtab") << PlainTextEdit << int(BobUI::Key_Backtab) << false;
}

void tst_QItemDelegate::enterKey()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(WidgetType, widget);
    QFETCH(int, key);
    QFETCH(bool, expectedFocus);

    QStandardItemModel model;
    model.appendRow(new QStandardItem());

    QListView view;
    view.setModel(&model);
    view.show();
    view.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    struct TestDelegate : public QItemDelegate
    {
        WidgetType widgetType;
        virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const override
        {
            QWidget *editor = 0;
            switch(widgetType) {
                case LineEdit:
                    editor = new QLineEdit(parent);
                    break;
                case TextEdit:
                    editor = new BOBUIextEdit(parent);
                    break;
                case PlainTextEdit:
                    editor = new QPlainTextEdit(parent);
                    break;
            }
            editor->setObjectName(QString::fromLatin1("TheEditor"));
            return editor;
        }
    } delegate;

    delegate.widgetType = widget;

    view.setItemDelegate(&delegate);
    QModelIndex index = model.index(0, 0);
    view.setCurrentIndex(index); // the editor will only selectAll on the current index
    view.edit(index);

    QList<QWidget*> lineEditors = view.viewport()->findChildren<QWidget *>(QString::fromLatin1("TheEditor"));
    QCOMPARE(lineEditors.size(), 1);

    QPointer<QWidget> editor = lineEditors.at(0);
    QCOMPARE(editor->hasFocus(), true);

    BOBUIest::keyClick(editor, BobUI::Key(key));

    if (expectedFocus) {
        QVERIFY(!editor.isNull());
        QVERIFY(editor->hasFocus());
    } else {
        BOBUIRY_VERIFY(editor.isNull()); // editor deletion happens via deleteLater
    }
}

void tst_QItemDelegate::task257859_finalizeEdit()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStandardItemModel model;
    model.appendRow(new QStandardItem());

    QListView view;
    view.setModel(&model);
    view.show();
    view.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    QModelIndex index = model.index(0, 0);
    view.edit(index);

    QList<QLineEdit *> lineEditors = view.viewport()->findChildren<QLineEdit *>();
    QCOMPARE(lineEditors.size(), 1);

    QPointer<QWidget> editor = lineEditors.at(0);
    QCOMPARE(editor->hasFocus(), true);

    QDialog dialog;
    BOBUIimer::singleShot(500, &dialog, SLOT(close()));
    dialog.exec();
    BOBUIRY_VERIFY(!editor);
}

void tst_QItemDelegate::BOBUIBUG4435_keepSelectionOnCheck()
{
    QStandardItemModel model(3, 1);
    for (int i = 0; i < 3; ++i) {
        QStandardItem *item = new QStandardItem(QLatin1String("Item ") + QString::number(i));
        item->setCheckable(true);
        item->setFlags(BobUI::ItemIsSelectable | BobUI::ItemIsUserCheckable | BobUI::ItemIsEnabled);
        model.setItem(i, item);
    }
    BOBUIableView view;
    view.setModel(&model);
    view.setSelectionMode(QAbstractItemView::MultiSelection);
    view.setItemDelegate(new TestItemDelegate(&view));
    view.show();
    view.selectAll();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    QStyleOptionViewItem option;
    option.rect = view.visualRect(model.index(0, 0));
    // mimic QStyledItemDelegate::initStyleOption logic
    option.features = QStyleOptionViewItem::HasDisplay | QStyleOptionViewItem::HasCheckIndicator;
    option.checkState = BobUI::CheckState(model.index(0, 0).data(BobUI::CheckStateRole).toInt());
    const int checkMargin = qApp->style()->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, 0) + 1;
    QRect checkRect = qApp->style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option, 0);
    checkRect.translate(checkMargin, 0);
    // click into the check mark checks, but doesn't change selection
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, checkRect.center());
    QCOMPARE(model.item(0)->checkState(), BobUI::Checked);
    BOBUIRY_VERIFY(view.selectionModel()->isColumnSelected(0, QModelIndex()));
    // click outside the check mark doesn't check, and changes selection
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier,
                      checkRect.center() + QPoint(checkRect.width(), 0));
    BOBUIRY_VERIFY(!view.selectionModel()->isColumnSelected(0, QModelIndex()));
}

void tst_QItemDelegate::comboBox()
{
    BOBUIableWidgetItem *item1 = new BOBUIableWidgetItem;
    item1->setData(BobUI::DisplayRole, true);

    BOBUIableWidget widget(1, 1);
    widget.setItem(0, 0, item1);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));
    QApplicationPrivate::setActiveWindow(&widget);

    widget.editItem(item1);

    QComboBox *boolEditor = nullptr;
    BOBUIRY_VERIFY( (boolEditor = widget.viewport()->findChild<QComboBox*>()) );
    QCOMPARE(boolEditor->currentIndex(), 1); // True is selected initially.
    // The data must actually be different in order for the model
    // to be updated.
    boolEditor->setCurrentIndex(0);
    QCOMPARE(boolEditor->currentIndex(), 0); // Changed to false.

    widget.clearFocus();
    widget.setFocus();

    QVariant data = item1->data(BobUI::EditRole);
    QCOMPARE(data.userType(), (int)QMetaType::Bool);
    QCOMPARE(data.toBool(), false);
}

void tst_QItemDelegate::testLineEditValidation_data()
{
    BOBUIest::addColumn<int>("key");

    BOBUIest::newRow("enter") << int(BobUI::Key_Enter);
    BOBUIest::newRow("return") << int(BobUI::Key_Return);
    BOBUIest::newRow("tab") << int(BobUI::Key_Tab);
    BOBUIest::newRow("backtab") << int(BobUI::Key_Backtab);
    BOBUIest::newRow("escape") << int(BobUI::Key_Escape);
}

void tst_QItemDelegate::testLineEditValidation()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(int, key);

    struct TestDelegate : public QItemDelegate
    {
        virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {
            Q_UNUSED(option);
            Q_UNUSED(index);

            QLineEdit *editor = new QLineEdit(parent);
            QRegularExpression re("\\w+,\\w+"); // two words separated by a comma
            editor->setValidator(new QRegularExpressionValidator(re, editor));
            editor->setObjectName(QStringLiteral("TheEditor"));
            return editor;
        }
    } delegate;

    QStandardItemModel model;
    // need a couple of dummy items to test tab and back tab
    model.appendRow(new QStandardItem(QStringLiteral("dummy")));
    QStandardItem *item = new QStandardItem(QStringLiteral("abc,def"));
    model.appendRow(item);
    model.appendRow(new QStandardItem(QStringLiteral("dummy")));

    QListView view;
    view.setModel(&model);
    view.setItemDelegate(&delegate);
    view.show();
    view.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    QPointer<QLineEdit> editor;
    QPersistentModelIndex index = model.indexFromItem(item);

    view.setCurrentIndex(index);
    view.edit(index);

    const auto findEditors = [&]() {
        return view.findChildren<QLineEdit *>(QStringLiteral("TheEditor"));
    };
    QCOMPARE(findEditors().size(), 1);
    editor = findEditors().at(0);
    editor->clear();

    // first try to set a valid text
    BOBUIest::keyClicks(editor, QStringLiteral("foo,bar"));

    // close the editor
    BOBUIest::keyClick(editor, BobUI::Key(key));

    BOBUIRY_VERIFY(editor.isNull());
    if (key != BobUI::Key_Escape)
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("foo,bar"));
    else
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("abc,def"));

    // now an invalid (but partially matching) text
    view.setCurrentIndex(index);
    view.edit(index);

    BOBUIRY_COMPARE(findEditors().size(), 1);
    editor = findEditors().at(0);
    editor->clear();

    // edit
    BOBUIest::keyClicks(editor, QStringLiteral("foobar"));

    // try to close the editor
    BOBUIest::keyClick(editor, BobUI::Key(key));

    if (key != BobUI::Key_Escape) {
        QVERIFY(!editor.isNull());
        QCOMPARE(qApp->focusWidget(), editor.data());
        QCOMPARE(editor->text(), QStringLiteral("foobar"));
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("foo,bar"));
    } else {
        BOBUIRY_VERIFY(editor.isNull());
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("abc,def"));
    }

    // reset the view to forcibly close the editor
    view.reset();
    BOBUIRY_COMPARE(findEditors().size(), 0);

    // set a valid text again
    view.setCurrentIndex(index);
    view.edit(index);

    BOBUIRY_COMPARE(findEditors().size(), 1);
    editor = findEditors().at(0);
    editor->clear();

    // set a valid text
    BOBUIest::keyClicks(editor, QStringLiteral("gender,bender"));

    // close the editor
    BOBUIest::keyClick(editor, BobUI::Key(key));

    BOBUIRY_VERIFY(editor.isNull());
    if (key != BobUI::Key_Escape)
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("gender,bender"));
    else
        QCOMPARE(item->data(BobUI::DisplayRole).toString(), QStringLiteral("abc,def"));
}

void tst_QItemDelegate::BOBUIBUG16469_textForRole()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build");
#else
    RoleDelegate delegate;
    QLocale locale;

    const float f = 123.456f;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, f, locale), locale.toString(f));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, f, locale), locale.toString(f));
    const double d = 123.456;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, d, locale), locale.toString(d, 'g', 6));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, d, locale), locale.toString(d, 'g', 6));
    const int i = 1234567;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, i, locale), locale.toString(i));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, i, locale), locale.toString(i));
    const qlonglong ll = 1234567;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, ll, locale), locale.toString(ll));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, ll, locale), locale.toString(ll));
    const uint ui = 1234567;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, ui, locale), locale.toString(ui));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, ui, locale), locale.toString(ui));
    const qulonglong ull = 1234567;
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, ull, locale), locale.toString(ull));
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, ull, locale), locale.toString(ull));

    const QString text("text");
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, text, locale), text);
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, text, locale), text);
    const QString multipleLines("multiple\nlines");
    QString multipleLines2 = multipleLines;
    multipleLines2.replace(QLatin1Char('\n'), QChar::LineSeparator);
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, multipleLines, locale), multipleLines2);
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, multipleLines, locale), multipleLines);
#endif
}

void tst_QItemDelegate::dateTextForRole_data()
{
#ifdef BOBUI_BUILD_INTERNAL
    BOBUIest::addColumn<QDateTime>("when");

    BOBUIest::newRow("now") << QDateTime::currentDateTime(); // It's a local time
    QDate date(2013, 12, 11);
    BOBUIime time(10, 9, 8, 765);
    // Ensure we exercise every time-spec variant:
    BOBUIest::newRow("local") << QDateTime(date, time);
    BOBUIest::newRow("UTC") << QDateTime(date, time, BOBUIimeZone::UTC);
#  if BOBUI_CONFIG(timezone)
    BOBUIest::newRow("zone") << QDateTime(date, time, BOBUIimeZone("Europe/Dublin"));
#  endif
    BOBUIest::newRow("offset") << QDateTime(date, time, BOBUIimeZone::fromSecondsAheadOfUtc(36000));
#endif
}

void tst_QItemDelegate::dateTextForRole()
{
#ifndef BOBUI_BUILD_INTERNAL
    QSKIP("This test requires a developer build");
#else
    QFETCH(QDateTime, when);
    RoleDelegate delegate;
    QLocale locale;
# define CHECK(value) \
    QCOMPARE(delegate.textForRole(BobUI::DisplayRole, value, locale), locale.toString(value, QLocale::ShortFormat)); \
    QCOMPARE(delegate.textForRole(BobUI::ToolTipRole, value, locale), locale.toString(value, QLocale::LongFormat))

    CHECK(when);
    CHECK(when.date());
    CHECK(when.time());
# undef CHECK
#endif
}

void tst_QItemDelegate::reuseEditor()
{
    class ReusingDelegate: public QItemDelegate {
    public:
        using QItemDelegate::QItemDelegate;
        ~ReusingDelegate()
        {
            if (cached)
                cached->deleteLater();
        }

        QWidget* createEditor(QWidget* parent,
                                const QStyleOptionViewItem&,
                                const QModelIndex&) const override
        {
            auto *cb = new QComboBox(parent);
            cb->addItem("One");
            cb->addItem("Two");
            cb->setEditable(true);
            return cb;
        }

        void setEditorData(QWidget* editor, const QModelIndex& index)
        const override
        {
            auto *cb = qobject_cast<QComboBox*>(editor);
            cb->setCurrentText(index.data(BobUI::DisplayRole).toString());
        }

        void setModelData(QWidget* editor,
                          QAbstractItemModel* model,
                          const QModelIndex& index) const override
        {
            auto *cb = qobject_cast<QComboBox*>(editor);
            model->setData(index, cb->currentText(), BobUI::DisplayRole);
        }

        void destroyEditor(QWidget* editor, const QModelIndex&) const override
        {
            auto *cb = qobject_cast<QComboBox*>(editor);
            cb->setParent(nullptr); // How to completely detach the editor from treeview ?
            cb->hide();
            cb->setEnabled(false);
            cached = cb;
        }

    private:
        mutable QComboBox* cached = nullptr;
    };

    QStandardItemModel model;
    model.appendRow(new QStandardItem("One"));
    model.appendRow(new QStandardItem("Two"));

    ReusingDelegate delegate;

    BOBUIreeView tree;
    tree.setModel(&model);
    tree.setItemDelegate(&delegate);

    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&tree));

    tree.edit(model.index(0, 0));
    BOBUIRY_VERIFY(qobject_cast<QComboBox *>(tree.focusWidget()));

    tree.close();
}

// ### _not_ covered:

// editing with a custom editor factory

// painting when editing
// painting elided text
// painting wrapped text
// painting focus
// painting icon
// painting color
// painting check
// painting selected

// rect for invalid
// rect for pixmap
// rect for image
// rect for icon
// rect for check

BOBUIEST_MAIN(tst_QItemDelegate)
#include "tst_qitemdelegate.moc"
