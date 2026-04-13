// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLEWIDGETS_H
#define QACCESSIBLEWIDGETS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <BobUIWidgets/qaccessiblewidget.h>

#if BOBUI_CONFIG(accessibility)

#include <BobUICore/QPointer>

BOBUI_BEGIN_NAMESPACE

class BOBUIextEdit;
class QStackedWidget;
class BOBUIoolBox;
class QMdiArea;
class QMdiSubWindow;
class QRubberBand;
class BOBUIextBrowser;
class QCalendarWidget;
class QAbstractItemView;
class QDockWidget;
class QDockWidgetLayout;
class QMainWindow;
class QPlainTextEdit;
class BOBUIextCursor;
class BOBUIextDocument;

#ifndef BOBUI_NO_CURSOR
class QAccessibleTextWidget : public QAccessibleWidgetV2,
                              public QAccessibleTextInterface,
                              public QAccessibleEditableTextInterface
{
public:
    QAccessibleTextWidget(QWidget *o, QAccessible::Role r = QAccessible::EditableText, const QString &name = QString());

    QAccessible::State state() const override;

    // QAccessibleTextInterface
    //  selection
    void selection(int selectionIndex, int *startOffset, int *endOffset) const override;
    int selectionCount() const override;
    void addSelection(int startOffset, int endOffset) override;
    void removeSelection(int selectionIndex) override;
    void setSelection(int selectionIndex, int startOffset, int endOffset) override;

    // cursor
    int cursorPosition() const override;
    void setCursorPosition(int position) override;

    // text
    QString text(int startOffset, int endOffset) const override;
    QString textBeforeOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                             int *startOffset, int *endOffset) const override;
    QString textAfterOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                            int *startOffset, int *endOffset) const override;
    QString textAtOffset(int offset, QAccessible::TextBoundaryType boundaryType,
                         int *startOffset, int *endOffset) const override;
    int characterCount() const override;

    // character <-> geometry
    QRect characterRect(int offset) const override;
    int offsetAtPoint(const QPoint &point) const override;

    QString attributes(int offset, int *startOffset, int *endOffset) const override;

    // QAccessibleEditableTextInterface
    void deleteText(int startOffset, int endOffset) override;
    void insertText(int offset, const QString &text) override;
    void replaceText(int startOffset, int endOffset, const QString &text) override;

    using QAccessibleWidgetV2::text;

protected:
    BOBUIextCursor textCursorForRange(int startOffset, int endOffset) const;
    virtual QPoint scrollBarPosition() const;
    // return the current text cursor at the caret position including a potential selection
    virtual BOBUIextCursor textCursor() const = 0;
    virtual void setTextCursor(const BOBUIextCursor &) = 0;
    virtual BOBUIextDocument *textDocument() const = 0;
    virtual QWidget *viewport() const = 0;
};

#if BOBUI_CONFIG(textedit)
class QAccessiblePlainTextEdit : public QAccessibleTextWidget
{
public:
    explicit QAccessiblePlainTextEdit(QWidget *o);

    QString text(QAccessible::Text t) const override;
    void setText(QAccessible::Text t, const QString &text) override;
    QAccessible::State state() const override;

    void *interface_cast(QAccessible::InterfaceType t) override;

    // QAccessibleTextInterface
    void scrollToSubstring(int startIndex, int endIndex) override;

    using QAccessibleTextWidget::text;

protected:
    QPlainTextEdit *plainTextEdit() const;

    QPoint scrollBarPosition() const override;
    BOBUIextCursor textCursor() const override;
    void setTextCursor(const BOBUIextCursor &textCursor) override;
    BOBUIextDocument *textDocument() const override;
    QWidget *viewport() const override;
};

class QAccessibleTextEdit : public QAccessibleTextWidget
{
public:
    explicit QAccessibleTextEdit(QWidget *o);

    QString text(QAccessible::Text t) const override;
    void setText(QAccessible::Text t, const QString &text) override;
    QAccessible::State state() const override;

    void *interface_cast(QAccessible::InterfaceType t) override;

    // QAccessibleTextInterface
    void scrollToSubstring(int startIndex, int endIndex) override;

    using QAccessibleTextWidget::text;

protected:
    BOBUIextEdit *textEdit() const;

    QPoint scrollBarPosition() const override;
    BOBUIextCursor textCursor() const override;
    void setTextCursor(const BOBUIextCursor &textCursor) override;
    BOBUIextDocument *textDocument() const override;
    QWidget *viewport() const override;
};
#endif // BOBUI_CONFIG(textedit)
#endif  //BOBUI_NO_CURSOR

class QAccessibleStackedWidget : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleStackedWidget(QWidget *widget);

    QAccessibleInterface *childAt(int x, int y) const override;
    int childCount() const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    QAccessibleInterface *child(int index) const override;

protected:
    QStackedWidget *stackedWidget() const;
};

class QAccessibleToolBox : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleToolBox(QWidget *widget);

// FIXME we currently expose the toolbox but it is not keyboard navigatable
// and the accessible hierarchy is not exactly beautiful.
//    int childCount() const;
//    QAccessibleInterface *child(int index) const;
//    int indexOfChild(const QAccessibleInterface *child) const;

protected:
    BOBUIoolBox *toolBox() const;
};

#if BOBUI_CONFIG(mdiarea)
class QAccessibleMdiArea : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleMdiArea(QWidget *widget);

    int childCount() const override;
    QAccessibleInterface *child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;

protected:
    QMdiArea *mdiArea() const;
};

class QAccessibleMdiSubWindow : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleMdiSubWindow(QWidget *widget);

    QString text(QAccessible::Text textType) const override;
    void setText(QAccessible::Text textType, const QString &text) override;
    QAccessible::State state() const override;
    int childCount() const override;
    QAccessibleInterface *child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    QRect rect() const override;

protected:
    QMdiSubWindow *mdiSubWindow() const;
};
#endif // BOBUI_CONFIG(mdiarea)

#if BOBUI_CONFIG(dialogbuttonbox)
class QAccessibleDialogButtonBox : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleDialogButtonBox(QWidget *widget);
};
#endif

#if BOBUI_CONFIG(textbrowser) && !defined(BOBUI_NO_CURSOR)
class QAccessibleTextBrowser : public QAccessibleTextEdit
{
public:
    explicit QAccessibleTextBrowser(QWidget *widget);

    QAccessible::Role role() const override;
};
#endif // BOBUI_CONFIG(textbrowser) && BOBUI_NO_CURSOR

#if BOBUI_CONFIG(calendarwidget)
class QAccessibleCalendarWidget : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleCalendarWidget(QWidget *widget);

    int childCount() const override;
    int indexOfChild(const QAccessibleInterface *child) const override;

    QAccessibleInterface *child(int index) const override;

protected:
    QCalendarWidget *calendarWidget() const;

private:
    QAbstractItemView *calendarView() const;
    QWidget *navigationBar() const;
};
#endif // BOBUI_CONFIG(calendarwidget)

#if BOBUI_CONFIG(dockwidget)
class QAccessibleDockWidget: public QAccessibleWidgetV2
{
public:
    explicit QAccessibleDockWidget(QWidget *widget);
    QAccessibleInterface *child(int index) const override;
    int indexOfChild(const QAccessibleInterface *child) const override;
    int childCount() const override;
    QRect rect () const override;
    QString text(QAccessible::Text t) const override;
    QAccessible::Role role() const override;

    QDockWidget *dockWidget() const;
protected:
    QDockWidgetLayout *dockWidgetLayout() const;
};

#endif // BOBUI_CONFIG(dockwidget)

#if BOBUI_CONFIG(mainwindow)
class QAccessibleMainWindow : public QAccessibleWidgetV2
{
public:
    explicit QAccessibleMainWindow(QWidget *widget);

    QAccessibleInterface *child(int index) const override;
    int childCount() const override;
    int indexOfChild(const QAccessibleInterface *iface) const override;
    QAccessibleInterface *childAt(int x, int y) const override;
    QMainWindow *mainWindow() const;

};
#endif // BOBUI_CONFIG(mainwindow)

#endif // BOBUI_CONFIG(accessibility)

BOBUI_END_NAMESPACE

#endif // QACESSIBLEWIDGETS_H
