// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qaccessiblewidgets_p.h"
#include "qabstracttextdocumentlayout.h"
#include "qapplication.h"
#include "qclipboard.h"
#include "bobuiextdocument.h"
#include "bobuiextobject.h"
#if BOBUI_CONFIG(textedit)
#include "qplaintextedit.h"
#include "bobuiextedit.h"
#include "private/bobuiextedit_p.h"
#endif
#include "bobuiextboundaryfinder.h"
#if BOBUI_CONFIG(scrollbar)
#include "qscrollbar.h"
#endif
#include "qdebug.h"
#include <QApplication>
#if BOBUI_CONFIG(stackedwidget)
#include <QStackedWidget>
#endif
#if BOBUI_CONFIG(toolbox)
#include <BOBUIoolBox>
#endif
#if BOBUI_CONFIG(mdiarea)
#include <QMdiArea>
#include <QMdiSubWindow>
#endif
#if BOBUI_CONFIG(dialogbuttonbox)
#include <QDialogButtonBox>
#endif
#include <limits.h>
#if BOBUI_CONFIG(rubberband)
#include <QRubberBand>
#endif
#if BOBUI_CONFIG(textbrowser)
#include <BOBUIextBrowser>
#endif
#if BOBUI_CONFIG(calendarwidget)
#include <QCalendarWidget>
#endif
#if BOBUI_CONFIG(itemviews)
#include <QAbstractItemView>
#endif
#if BOBUI_CONFIG(dockwidget)
#include <QDockWidget>
#include <private/qdockwidget_p.h>
#endif
#if BOBUI_CONFIG(mainwindow)
#include <QMainWindow>
#endif
#include <QFocusFrame>
#if BOBUI_CONFIG(menu)
#include <QMenu>
#endif

#if BOBUI_CONFIG(accessibility)

#include <BobUIGui/private/qaccessiblehelper_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QString bobui_accHotKey(const QString &text);

QWidgetList _q_ac_childWidgets(const QWidget *widget)
{
    QList<QWidget*> widgets;
    if (!widget)
        return widgets;
    for (QObject *o : widget->children()) {
        QWidget *w = qobject_cast<QWidget *>(o);
        if (!w)
            continue;
        QString objectName = w->objectName();
        if (!w->isWindow()
              && !qobject_cast<QFocusFrame*>(w)
#if BOBUI_CONFIG(menu)
              && !qobject_cast<QMenu*>(w)
#endif
              // Exclude widgets used as implementation details
              && objectName != "bobui_rubberband"_L1
              && objectName != "bobui_qmainwindow_extended_splitter"_L1
              && objectName != "bobui_spinbox_lineedit"_L1) {
            widgets.append(w);
        }
    }
    return widgets;
}

#if BOBUI_CONFIG(textedit) && !defined(BOBUI_NO_CURSOR)

QAccessiblePlainTextEdit::QAccessiblePlainTextEdit(QWidget* o)
  :QAccessibleTextWidget(o)
{
    Q_ASSERT(qobject_cast<QPlainTextEdit *>(widget()));
}

QPlainTextEdit* QAccessiblePlainTextEdit::plainTextEdit() const
{
    return static_cast<QPlainTextEdit *>(widget());
}

QString QAccessiblePlainTextEdit::text(QAccessible::Text t) const
{
    if (t == QAccessible::Value)
        return plainTextEdit()->toPlainText();

    return QAccessibleWidgetV2::text(t);
}

void QAccessiblePlainTextEdit::setText(QAccessible::Text t, const QString &text)
{
    if (t != QAccessible::Value) {
        QAccessibleWidgetV2::setText(t, text);
        return;
    }
    if (plainTextEdit()->isReadOnly())
        return;

    plainTextEdit()->setPlainText(text);
}

QAccessible::State QAccessiblePlainTextEdit::state() const
{
    QAccessible::State st = QAccessibleTextWidget::state();
    if (plainTextEdit()->isReadOnly())
        st.readOnly = true;
    else
        st.editable = true;
    return st;
}

void *QAccessiblePlainTextEdit::interface_cast(QAccessible::InterfaceType t)
{
    if (t == QAccessible::TextInterface)
        return static_cast<QAccessibleTextInterface*>(this);
    else if (t == QAccessible::EditableTextInterface)
        return static_cast<QAccessibleEditableTextInterface*>(this);
    return QAccessibleWidgetV2::interface_cast(t);
}

QPoint QAccessiblePlainTextEdit::scrollBarPosition() const
{
    QPoint result;
    result.setX(plainTextEdit()->horizontalScrollBar() ? plainTextEdit()->horizontalScrollBar()->sliderPosition() : 0);
    result.setY(plainTextEdit()->verticalScrollBar() ? plainTextEdit()->verticalScrollBar()->sliderPosition() : 0);
    return result;
}

BOBUIextCursor QAccessiblePlainTextEdit::textCursor() const
{
    return plainTextEdit()->textCursor();
}

void QAccessiblePlainTextEdit::setTextCursor(const BOBUIextCursor &textCursor)
{
    plainTextEdit()->setTextCursor(textCursor);
}

BOBUIextDocument* QAccessiblePlainTextEdit::textDocument() const
{
    return plainTextEdit()->document();
}

QWidget* QAccessiblePlainTextEdit::viewport() const
{
    return plainTextEdit()->viewport();
}

void QAccessiblePlainTextEdit::scrollToSubstring(int startIndex, int endIndex)
{
    //TODO: Not implemented
    Q_UNUSED(startIndex);
    Q_UNUSED(endIndex);
}


/*!
  \class QAccessibleTextEdit
  \brief The QAccessibleTextEdit class implements the QAccessibleInterface for richtext editors.
  \internal
*/

/*!
  \fn QAccessibleTextEdit::QAccessibleTextEdit(QWidget *widget)

  Constructs a QAccessibleTextEdit object for a \a widget.
*/
QAccessibleTextEdit::QAccessibleTextEdit(QWidget *o)
: QAccessibleTextWidget(o, QAccessible::EditableText)
{
    Q_ASSERT(qobject_cast<BOBUIextEdit *>(widget()));
}

/*! Returns the text edit. */
BOBUIextEdit *QAccessibleTextEdit::textEdit() const
{
    return static_cast<BOBUIextEdit *>(widget());
}

BOBUIextCursor QAccessibleTextEdit::textCursor() const
{
    return textEdit()->textCursor();
}

BOBUIextDocument *QAccessibleTextEdit::textDocument() const
{
    return textEdit()->document();
}

void QAccessibleTextEdit::setTextCursor(const BOBUIextCursor &textCursor)
{
    textEdit()->setTextCursor(textCursor);
}

QWidget *QAccessibleTextEdit::viewport() const
{
    return textEdit()->viewport();
}

QPoint QAccessibleTextEdit::scrollBarPosition() const
{
    QPoint result;
    result.setX(textEdit()->horizontalScrollBar() ? textEdit()->horizontalScrollBar()->sliderPosition() : 0);
    result.setY(textEdit()->verticalScrollBar() ? textEdit()->verticalScrollBar()->sliderPosition() : 0);
    return result;
}

QString QAccessibleTextEdit::text(QAccessible::Text t) const
{
    if (t == QAccessible::Value)
        return textEdit()->toPlainText();

    return QAccessibleWidgetV2::text(t);
}

void QAccessibleTextEdit::setText(QAccessible::Text t, const QString &text)
{
    if (t != QAccessible::Value) {
        QAccessibleWidgetV2::setText(t, text);
        return;
    }
    if (textEdit()->isReadOnly())
        return;

    textEdit()->setText(text);
}

QAccessible::State QAccessibleTextEdit::state() const
{
    QAccessible::State st = QAccessibleTextWidget::state();
    if (textEdit()->isReadOnly())
        st.readOnly = true;
    else
        st.editable = true;
    return st;
}

void *QAccessibleTextEdit::interface_cast(QAccessible::InterfaceType t)
{
    if (t == QAccessible::TextInterface)
        return static_cast<QAccessibleTextInterface*>(this);
    else if (t == QAccessible::EditableTextInterface)
        return static_cast<QAccessibleEditableTextInterface*>(this);
    return QAccessibleWidgetV2::interface_cast(t);
}

void QAccessibleTextEdit::scrollToSubstring(int startIndex, int endIndex)
{
    BOBUIextEdit *edit = textEdit();

    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(startIndex);
    QRect r = edit->cursorRect(cursor);

    cursor.setPosition(endIndex);
    r.setBottomRight(edit->cursorRect(cursor).bottomRight());

    r.moveTo(r.x() + edit->horizontalScrollBar()->value(),
             r.y() + edit->verticalScrollBar()->value());

    // E V I L, but ensureVisible is not public
    if (Q_UNLIKELY(!QMetaObject::invokeMethod(edit, "_q_ensureVisible", Q_ARG(QRectF, r))))
        qWarning("AccessibleTextEdit::scrollToSubstring failed!");
}

#endif // BOBUI_CONFIG(textedit) && BOBUI_NO_CURSOR

#if BOBUI_CONFIG(stackedwidget)
// ======================= QAccessibleStackedWidget ======================
QAccessibleStackedWidget::QAccessibleStackedWidget(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::LayeredPane)
{
    Q_ASSERT(qobject_cast<QStackedWidget *>(widget));
}

QAccessibleInterface *QAccessibleStackedWidget::childAt(int x, int y) const
{
    if (!stackedWidget()->isVisible())
        return nullptr;
    QWidget *currentWidget = stackedWidget()->currentWidget();
    if (!currentWidget)
        return nullptr;
    QPoint position = currentWidget->mapFromGlobal(QPoint(x, y));
    if (currentWidget->rect().contains(position))
        return child(stackedWidget()->currentIndex());
    return nullptr;
}

int QAccessibleStackedWidget::childCount() const
{
    return stackedWidget()->count();
}

int QAccessibleStackedWidget::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child)
        return -1;

    QWidget *widget = qobject_cast<QWidget*>(child->object());
    return stackedWidget()->indexOf(widget);
}

QAccessibleInterface *QAccessibleStackedWidget::child(int index) const
{
    if (index < 0 || index >= stackedWidget()->count())
        return nullptr;
    return QAccessible::queryAccessibleInterface(stackedWidget()->widget(index));
}

QStackedWidget *QAccessibleStackedWidget::stackedWidget() const
{
    return static_cast<QStackedWidget *>(object());
}
#endif // BOBUI_CONFIG(stackedwidget)

#if BOBUI_CONFIG(toolbox)
// ======================= QAccessibleToolBox ======================
QAccessibleToolBox::QAccessibleToolBox(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::LayeredPane)
{
    Q_ASSERT(qobject_cast<BOBUIoolBox *>(widget));
}

BOBUIoolBox * QAccessibleToolBox::toolBox() const
{
    return static_cast<BOBUIoolBox *>(object());
}
#endif // BOBUI_CONFIG(toolbox)

// ======================= QAccessibleMdiArea ======================
#if BOBUI_CONFIG(mdiarea)
QAccessibleMdiArea::QAccessibleMdiArea(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::LayeredPane)
{
    Q_ASSERT(qobject_cast<QMdiArea *>(widget));
}

int QAccessibleMdiArea::childCount() const
{
    return mdiArea()->subWindowList().size();
}

QAccessibleInterface *QAccessibleMdiArea::child(int index) const
{
    QList<QMdiSubWindow *> subWindows = mdiArea()->subWindowList();
    QWidget *targetObject = subWindows.value(index);
    if (!targetObject)
       return nullptr;
    return QAccessible::queryAccessibleInterface(targetObject);
}


int QAccessibleMdiArea::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child || !child->object() || mdiArea()->subWindowList().isEmpty())
        return -1;
    if (QMdiSubWindow *window = qobject_cast<QMdiSubWindow *>(child->object())) {
        return mdiArea()->subWindowList().indexOf(window);
    }
    return -1;
}

QMdiArea *QAccessibleMdiArea::mdiArea() const
{
    return static_cast<QMdiArea *>(object());
}

// ======================= QAccessibleMdiSubWindow ======================
QAccessibleMdiSubWindow::QAccessibleMdiSubWindow(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::Window)
{
    Q_ASSERT(qobject_cast<QMdiSubWindow *>(widget));
}

QString QAccessibleMdiSubWindow::text(QAccessible::Text textType) const
{
    if (textType == QAccessible::Name) {
        QString title = mdiSubWindow()->windowTitle();
        title.remove("[*]"_L1);
        return title;
    }
    return QAccessibleWidgetV2::text(textType);
}

void QAccessibleMdiSubWindow::setText(QAccessible::Text textType, const QString &text)
{
    if (textType == QAccessible::Name)
        mdiSubWindow()->setWindowTitle(text);
    else
        QAccessibleWidgetV2::setText(textType, text);
}

QAccessible::State QAccessibleMdiSubWindow::state() const
{
    QAccessible::State state;
    state.focusable = true;
    if (!mdiSubWindow()->isMaximized()) {
        state.movable = true;
        state.sizeable = true;
    }
    if (mdiSubWindow()->isAncestorOf(QApplication::focusWidget())
            || QApplication::focusWidget() == mdiSubWindow())
        state.focused = true;
    if (!mdiSubWindow()->isVisible())
        state.invisible = true;
    if (const QWidget *parent = mdiSubWindow()->parentWidget())
        if (!parent->contentsRect().contains(mdiSubWindow()->geometry()))
            state.offscreen = true;
    if (!mdiSubWindow()->isEnabled())
        state.disabled = true;
    return state;
}

int QAccessibleMdiSubWindow::childCount() const
{
    if (mdiSubWindow()->widget())
        return 1;
    return 0;
}

QAccessibleInterface *QAccessibleMdiSubWindow::child(int index) const
{
    QMdiSubWindow *source = mdiSubWindow();
    if (index != 0 || !source->widget())
        return nullptr;

    return QAccessible::queryAccessibleInterface(source->widget());
}

int QAccessibleMdiSubWindow::indexOfChild(const QAccessibleInterface *child) const
{
    if (child && child->object() && child->object() == mdiSubWindow()->widget())
        return 0;
    return -1;
}

QRect QAccessibleMdiSubWindow::rect() const
{
    if (mdiSubWindow()->isHidden())
        return QRect();
    if (!mdiSubWindow()->parent())
        return QAccessibleWidgetV2::rect();
    const QPoint pos = mdiSubWindow()->mapToGlobal(QPoint(0, 0));
    return QRect(pos, mdiSubWindow()->size());
}

QMdiSubWindow *QAccessibleMdiSubWindow::mdiSubWindow() const
{
    return static_cast<QMdiSubWindow *>(object());
}
#endif // BOBUI_CONFIG(mdiarea)

#if BOBUI_CONFIG(dialogbuttonbox)
// ======================= QAccessibleDialogButtonBox ======================
QAccessibleDialogButtonBox::QAccessibleDialogButtonBox(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::Grouping)
{
    Q_ASSERT(qobject_cast<QDialogButtonBox*>(widget));
}

#endif // BOBUI_CONFIG(dialogbuttonbox)

#if BOBUI_CONFIG(textbrowser) && !defined(BOBUI_NO_CURSOR)
QAccessibleTextBrowser::QAccessibleTextBrowser(QWidget *widget)
    : QAccessibleTextEdit(widget)
{
    Q_ASSERT(qobject_cast<BOBUIextBrowser *>(widget));
}

QAccessible::Role QAccessibleTextBrowser::role() const
{
    return QAccessible::StaticText;
}
#endif // BOBUI_CONFIG(textbrowser) && BOBUI_NO_CURSOR

#if BOBUI_CONFIG(calendarwidget)
// ===================== QAccessibleCalendarWidget ========================
QAccessibleCalendarWidget::QAccessibleCalendarWidget(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::Table)
{
    Q_ASSERT(qobject_cast<QCalendarWidget *>(widget));
}

int QAccessibleCalendarWidget::childCount() const
{
   return calendarWidget()->isNavigationBarVisible() ? 2 : 1;
}

int QAccessibleCalendarWidget::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child || !child->object() || childCount() <= 0)
        return -1;
    if (qobject_cast<QAbstractItemView *>(child->object()))
        return childCount() - 1; // FIXME
    return 0;
}

QAccessibleInterface *QAccessibleCalendarWidget::child(int index) const
{
    if (index < 0 || index >= childCount())
        return nullptr;

    if (childCount() > 1 && index == 0)
        return QAccessible::queryAccessibleInterface(navigationBar());

    return QAccessible::queryAccessibleInterface(calendarView());
}

QCalendarWidget *QAccessibleCalendarWidget::calendarWidget() const
{
    return static_cast<QCalendarWidget *>(object());
}

QAbstractItemView *QAccessibleCalendarWidget::calendarView() const
{
    for (QObject *child : calendarWidget()->children()) {
        if (child->objectName() == "bobui_calendar_calendarview"_L1)
            return static_cast<QAbstractItemView *>(child);
    }
    return nullptr;
}

QWidget *QAccessibleCalendarWidget::navigationBar() const
{
    for (QObject *child : calendarWidget()->children()) {
        if (child->objectName() == "bobui_calendar_navigationbar"_L1)
            return static_cast<QWidget *>(child);
    }
    return nullptr;
}
#endif // BOBUI_CONFIG(calendarwidget)

#if BOBUI_CONFIG(dockwidget)

// Dock Widget - order of children:
// - Content widget
// - Float button
// - Close button
// If there is a custom title bar widget, that one becomes child 1, after the content 0
// (in that case the buttons are ignored)
QAccessibleDockWidget::QAccessibleDockWidget(QWidget *widget)
    : QAccessibleWidgetV2(widget)
{
}

QDockWidgetLayout *QAccessibleDockWidget::dockWidgetLayout() const
{
    return qobject_cast<QDockWidgetLayout*>(dockWidget()->layout());
}

int QAccessibleDockWidget::childCount() const
{
    if (dockWidget()->titleBarWidget()) {
        return dockWidget()->widget() ? 2 : 1;
    }
    return dockWidgetLayout()->count();
}

QAccessibleInterface *QAccessibleDockWidget::child(int index) const
{
    if (dockWidget()->titleBarWidget()) {
        if ((!dockWidget()->widget() && index == 0) || (index == 1))
            return QAccessible::queryAccessibleInterface(dockWidget()->titleBarWidget());
        if (index == 0)
            return QAccessible::queryAccessibleInterface(dockWidget()->widget());
    } else {
        QLayoutItem *item = dockWidgetLayout()->itemAt(index);
        if (item)
            return QAccessible::queryAccessibleInterface(item->widget());
    }
    return nullptr;
}

int QAccessibleDockWidget::indexOfChild(const QAccessibleInterface *child) const
{
    if (!child || !child->object() || child->object()->parent() != object())
        return -1;

    if (dockWidget()->titleBarWidget() == child->object()) {
        return dockWidget()->widget() ? 1 : 0;
    }

    return dockWidgetLayout()->indexOf(qobject_cast<QWidget*>(child->object()));
}

QRect QAccessibleDockWidget::rect() const
{
    QRect rect;

    if (dockWidget()->isFloating()) {
        rect = dockWidget()->frameGeometry();
    } else {
        rect = dockWidget()->rect();
        rect.moveTopLeft(dockWidget()->mapToGlobal(rect.topLeft()));
    }

    return rect;
}

QDockWidget *QAccessibleDockWidget::dockWidget() const
{
    return static_cast<QDockWidget *>(object());
}

QString QAccessibleDockWidget::text(QAccessible::Text t) const
{
    if (t == QAccessible::Name) {
        return bobui_accStripAmp(dockWidget()->windowTitle());
    } else if (t == QAccessible::Accelerator) {
        return bobui_accHotKey(dockWidget()->windowTitle());
    }
    return QString();
}

QAccessible::Role QAccessibleDockWidget::role() const
{
    if (dockWidget()->isFloating())
        return QAccessible::Window;

    return QAccessible::Pane;
}

#endif // BOBUI_CONFIG(dockwidget)

#ifndef BOBUI_NO_CURSOR

QAccessibleTextWidget::QAccessibleTextWidget(QWidget *o, QAccessible::Role r, const QString &name)
    : QAccessibleWidgetV2(o, r, name)
{

}

QAccessible::State QAccessibleTextWidget::state() const
{
    QAccessible::State s = QAccessibleWidgetV2::state();
    s.selectableText = true;
    s.multiLine = true;
    return s;
}

QRect QAccessibleTextWidget::characterRect(int offset) const
{
    BOBUIextBlock block = textDocument()->findBlock(offset);
    if (!block.isValid())
        return QRect();

    BOBUIextLayout *layout = block.layout();
    QPointF layoutPosition = layout->position();
    int relativeOffset = offset - block.position();
    BOBUIextLine line = layout->lineForTextPosition(relativeOffset);

    QRect r;

    if (line.isValid()) {
        qreal x = line.cursorToX(relativeOffset);

        BOBUIextCharFormat format;
        BOBUIextBlock::iterator iter = block.begin();
        if (iter.atEnd())
            format = block.charFormat();
        else {
            while (!iter.atEnd() && !iter.fragment().contains(offset))
                ++iter;
            if (iter.atEnd()) // newline should have same format as preceding character
                --iter;
            format = iter.fragment().charFormat();
        }

        QFontMetrics fm(format.font());
        const QString ch = text(offset, offset + 1);
        if (!ch.isEmpty()) {
            int w = fm.horizontalAdvance(ch);
            int h = fm.height();
            r = QRect(layoutPosition.x() + x, layoutPosition.y() + line.y() + line.ascent() + fm.descent() - h,
                      w, h);
            r.moveTo(viewport()->mapToGlobal(r.topLeft()));
        }
        r.translate(-scrollBarPosition());
    }

    return r;
}

int QAccessibleTextWidget::offsetAtPoint(const QPoint &point) const
{
    QPoint p = viewport()->mapFromGlobal(point);
    // convert to document coordinates
    p += scrollBarPosition();
    return textDocument()->documentLayout()->hitTest(p, BobUI::ExactHit);
}

int QAccessibleTextWidget::selectionCount() const
{
    return textCursor().hasSelection() ? 1 : 0;
}

namespace {
/*!
    \internal
    \brief Helper class for AttributeFormatter

    This class is returned from AttributeFormatter's indexing operator to act
    as a proxy for the following assignment.

    It uses perfect forwarding in its assignment operator to amend the RHS
    with the formatting of the key, using QStringBuilder. Consequently, the
    RHS can be anything that QStringBuilder supports.
*/
class AttributeFormatterRef {
    QString &string;
    const char *key;
    friend class AttributeFormatter;
    AttributeFormatterRef(QString &string, const char *key) : string(string), key(key) {}
public:
    template <typename RHS>
    void operator=(RHS &&rhs)
    { string += QLatin1StringView(key) + u':' + std::forward<RHS>(rhs) + u';'; }
};

/*!
    \internal
    \brief Small string-builder class that supports a map-like API to serialize key-value pairs.
    \code
    AttributeFormatter attrs;
    attrs["foo"] = QLatinString("hello") + world + u'!';
    \endcode
    The key type is always \c{const char*}, and the right-hand-side can
    be any QStringBuilder expression.

    Breaking it down, this class provides the indexing operator, stores
    the key in an instance of, and then returns, AttributeFormatterRef,
    which is the class that provides the assignment part of the operation.
*/
class AttributeFormatter {
    QString string;
public:
    AttributeFormatterRef operator[](const char *key)
    { return AttributeFormatterRef(string, key); }

    QString toFormatted() const { return string; }
};
} // unnamed namespace

QString QAccessibleTextWidget::attributes(int offset, int *startOffset, int *endOffset) const
{
    /* The list of attributes can be found at:
     http://linuxfoundation.org/collaborate/workgroups/accessibility/iaccessible2/textattributes
    */

    // IAccessible2 defines -1 as length and -2 as cursor position
    if (offset == -2)
        offset = cursorPosition();

    const int charCount = characterCount();

    // -1 doesn't make much sense here, but it's better to return something
    // screen readers may ask for text attributes at the cursor pos which may be equal to length
    if (offset == -1 || offset == charCount)
        offset = charCount - 1;

    if (offset < 0 || offset > charCount) {
        *startOffset = -1;
        *endOffset = -1;
        return QString();
    }


    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(offset);
    BOBUIextBlock block = cursor.block();

    int blockStart = block.position();
    int blockEnd = blockStart + block.length();

    BOBUIextBlock::iterator iter = block.begin();
    int lastFragmentIndex = blockStart;
    while (!iter.atEnd()) {
        BOBUIextFragment f = iter.fragment();
        if (f.contains(offset))
            break;
        lastFragmentIndex = f.position() + f.length();
        ++iter;
    }

    BOBUIextCharFormat charFormat;
    if (!iter.atEnd()) {
        BOBUIextFragment fragment = iter.fragment();
        charFormat = fragment.charFormat();
        int pos = fragment.position();
        // text block and fragment may overlap, use the smallest common range
        *startOffset = qMax(pos, blockStart);
        *endOffset = qMin(pos + fragment.length(), blockEnd);
    } else {
        charFormat = block.charFormat();
        *startOffset = lastFragmentIndex;
        *endOffset = blockEnd;
    }
    Q_ASSERT(*startOffset <= offset);
    Q_ASSERT(*endOffset >= offset);

    BOBUIextBlockFormat blockFormat = cursor.blockFormat();

    const QFont charFormatFont = charFormat.font();

    AttributeFormatter attrs;
    QString family = charFormatFont.families().value(0, QString());
    if (!family.isEmpty()) {
        family = family.replace(u'\\', "\\\\"_L1);
        family = family.replace(u':', "\\:"_L1);
        family = family.replace(u',', "\\,"_L1);
        family = family.replace(u'=', "\\="_L1);
        family = family.replace(u';', "\\;"_L1);
        family = family.replace(u'\"', "\\\""_L1);
        attrs["font-family"] = u'"' + family + u'"';
    }

    int fontSize = int(charFormatFont.pointSize());
    if (fontSize)
        attrs["font-size"] = QString::fromLatin1("%1pt").arg(fontSize);

    //Different weight values are not handled
    attrs["font-weight"] = QString::fromLatin1(charFormatFont.weight() > QFont::Normal ? "bold" : "normal");

    QFont::Style style = charFormatFont.style();
    attrs["font-style"] = QString::fromLatin1((style == QFont::StyleItalic) ? "italic" : ((style == QFont::StyleOblique) ? "oblique": "normal"));

    attrs["text-line-through-type"] = charFormatFont.strikeOut() ? "single"_L1 : "none"_L1;

    BOBUIextCharFormat::UnderlineStyle underlineStyle = charFormat.underlineStyle();
    if (underlineStyle == BOBUIextCharFormat::NoUnderline && charFormatFont.underline()) // underline could still be set in the default font
        underlineStyle = BOBUIextCharFormat::SingleUnderline;
    QString underlineStyleValue;
    switch (underlineStyle) {
        case BOBUIextCharFormat::NoUnderline:
            break;
        case BOBUIextCharFormat::SingleUnderline:
            underlineStyleValue = QStringLiteral("solid");
            break;
        case BOBUIextCharFormat::DashUnderline:
            underlineStyleValue = QStringLiteral("dash");
            break;
        case BOBUIextCharFormat::DotLine:
            underlineStyleValue = QStringLiteral("dash");
            break;
        case BOBUIextCharFormat::DashDotLine:
            underlineStyleValue = QStringLiteral("dot-dash");
            break;
        case BOBUIextCharFormat::DashDotDotLine:
            underlineStyleValue = QStringLiteral("dot-dot-dash");
            break;
        case BOBUIextCharFormat::WaveUnderline:
            underlineStyleValue = QStringLiteral("wave");
            break;
        case BOBUIextCharFormat::SpellCheckUnderline:
            underlineStyleValue = QStringLiteral("wave"); // this is not correct, but provides good approximation at least
            break;
        default:
            qWarning() << "Unknown BOBUIextCharFormat::UnderlineStyle value " << underlineStyle << " could not be translated to IAccessible2 value";
            break;
    }
    if (!underlineStyleValue.isNull()) {
        attrs["text-underline-style"] = underlineStyleValue;
        attrs["text-underline-type"] = QStringLiteral("single"); // if underlineStyleValue is set, there is an underline, and BobUI does not support other than single ones
    } // else both are "none" which is the default - no need to set them

    if (block.textDirection() == BobUI::RightToLeft)
        attrs["writing-mode"] = QStringLiteral("rl");

    BOBUIextCharFormat::VerticalAlignment alignment = charFormat.verticalAlignment();
    attrs["text-position"] = QString::fromLatin1((alignment == BOBUIextCharFormat::AlignSubScript) ? "sub" : ((alignment == BOBUIextCharFormat::AlignSuperScript) ? "super" : "baseline" ));

    QBrush background = charFormat.background();
    if (background.style() == BobUI::SolidPattern) {
        attrs["background-color"] = QString::fromLatin1("rgb(%1,%2,%3)").arg(background.color().red()).arg(background.color().green()).arg(background.color().blue());
    }

    QBrush foreground = charFormat.foreground();
    if (foreground.style() == BobUI::SolidPattern) {
        attrs["color"] = QString::fromLatin1("rgb(%1,%2,%3)").arg(foreground.color().red()).arg(foreground.color().green()).arg(foreground.color().blue());
    }

    switch (blockFormat.alignment() & (BobUI::AlignLeft | BobUI::AlignRight | BobUI::AlignHCenter | BobUI::AlignJustify)) {
    case BobUI::AlignLeft:
        attrs["text-align"] = QStringLiteral("left");
        break;
    case BobUI::AlignRight:
        attrs["text-align"] = QStringLiteral("right");
        break;
    case BobUI::AlignHCenter:
        attrs["text-align"] = QStringLiteral("center");
        break;
    case BobUI::AlignJustify:
        attrs["text-align"] = QStringLiteral("justify");
        break;
    }

    return attrs.toFormatted();
}

int QAccessibleTextWidget::cursorPosition() const
{
    return textCursor().position();
}

void QAccessibleTextWidget::selection(int selectionIndex, int *startOffset, int *endOffset) const
{
    *startOffset = *endOffset = 0;
    BOBUIextCursor cursor = textCursor();

    if (selectionIndex != 0 || !cursor.hasSelection())
        return;

    *startOffset = cursor.selectionStart();
    *endOffset = cursor.selectionEnd();
}

QString QAccessibleTextWidget::text(int startOffset, int endOffset) const
{
    BOBUIextCursor cursor(textCursor());

    cursor.setPosition(startOffset, BOBUIextCursor::MoveAnchor);
    cursor.setPosition(endOffset, BOBUIextCursor::KeepAnchor);

    return cursor.selectedText().replace(QChar(QChar::ParagraphSeparator), u'\n');
}

QPoint QAccessibleTextWidget::scrollBarPosition() const
{
    return QPoint(0, 0);
}


QString QAccessibleTextWidget::textBeforeOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                                int *startOffset, int *endOffset) const
{
    return bobui_accTextBeforeOffsetHelper(*this, textCursor(), offset, boundaryType, startOffset,
                                        endOffset);
}

QString QAccessibleTextWidget::textAfterOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                              int *startOffset, int *endOffset) const
{
    return bobui_accTextAfterOffsetHelper(*this, textCursor(), offset, boundaryType, startOffset,
                                       endOffset);
}

QString QAccessibleTextWidget::textAtOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                                            int *startOffset, int *endOffset) const
{
    return bobui_accTextAtOffsetHelper(*this, textCursor(), offset, boundaryType, startOffset,
                                    endOffset);
}

void QAccessibleTextWidget::setCursorPosition(int position)
{
    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(position);
    setTextCursor(cursor);
}

void QAccessibleTextWidget::addSelection(int startOffset, int endOffset)
{
    setSelection(0, startOffset, endOffset);
}

void QAccessibleTextWidget::removeSelection(int selectionIndex)
{
    if (selectionIndex != 0)
        return;

    BOBUIextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

void QAccessibleTextWidget::setSelection(int selectionIndex, int startOffset, int endOffset)
{
    if (selectionIndex != 0)
        return;

    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(startOffset, BOBUIextCursor::MoveAnchor);
    cursor.setPosition(endOffset, BOBUIextCursor::KeepAnchor);
    setTextCursor(cursor);
}

int QAccessibleTextWidget::characterCount() const
{
    BOBUIextCursor cursor = textCursor();
    cursor.movePosition(BOBUIextCursor::End);
    return cursor.position();
}

BOBUIextCursor QAccessibleTextWidget::textCursorForRange(int startOffset, int endOffset) const
{
    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(startOffset, BOBUIextCursor::MoveAnchor);
    cursor.setPosition(endOffset, BOBUIextCursor::KeepAnchor);

    return cursor;
}

void QAccessibleTextWidget::deleteText(int startOffset, int endOffset)
{
    BOBUIextCursor cursor = textCursorForRange(startOffset, endOffset);
    cursor.removeSelectedText();
}

void QAccessibleTextWidget::insertText(int offset, const QString &text)
{
    BOBUIextCursor cursor = textCursor();
    cursor.setPosition(offset);
    cursor.insertText(text);
}

void QAccessibleTextWidget::replaceText(int startOffset, int endOffset, const QString &text)
{
    BOBUIextCursor cursor = textCursorForRange(startOffset, endOffset);
    cursor.removeSelectedText();
    cursor.insertText(text);
}
#endif // BOBUI_NO_CURSOR


#if BOBUI_CONFIG(mainwindow)
QAccessibleMainWindow::QAccessibleMainWindow(QWidget *widget)
    : QAccessibleWidgetV2(widget, QAccessible::Window) { }

QAccessibleInterface *QAccessibleMainWindow::child(int index) const
{
    QList<QWidget*> kids = _q_ac_childWidgets(mainWindow());
    if (index >= 0 && index < kids.size()) {
        return QAccessible::queryAccessibleInterface(kids.at(index));
    }
    return nullptr;
}

int QAccessibleMainWindow::childCount() const
{
    QList<QWidget*> kids = _q_ac_childWidgets(mainWindow());
    return kids.size();
}

int QAccessibleMainWindow::indexOfChild(const QAccessibleInterface *iface) const
{
    QList<QWidget*> kids = _q_ac_childWidgets(mainWindow());
    return kids.indexOf(static_cast<QWidget*>(iface->object()));
}

QAccessibleInterface *QAccessibleMainWindow::childAt(int x, int y) const
{
    QWidget *w = widget();
    if (!w->isVisible())
        return nullptr;
    QPoint gp = w->mapToGlobal(QPoint(0, 0));
    if (!QRect(gp.x(), gp.y(), w->width(), w->height()).contains(x, y))
        return nullptr;

    const QWidgetList kids = _q_ac_childWidgets(mainWindow());
    QPoint rp = mainWindow()->mapFromGlobal(QPoint(x, y));
    for (QWidget *child : kids) {
        if (!child->isWindow() && !child->isHidden() && child->geometry().contains(rp)) {
            return QAccessible::queryAccessibleInterface(child);
        }
    }
    return nullptr;
}

QMainWindow *QAccessibleMainWindow::mainWindow() const
{
    return qobject_cast<QMainWindow *>(object());
}

#endif // BOBUI_CONFIG(mainwindow)

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)
