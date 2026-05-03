// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLINEEDIT_H
#define QLINEEDIT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qframe.h>
#include <BobUIGui/bobuiextcursor.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qmargins.h>

BOBUI_REQUIRE_CONFIG(lineedit);

BOBUI_BEGIN_NAMESPACE

class QValidator;
class QMenu;
class QLineEditPrivate;
class QCompleter;
class QStyleOptionFrame;
class QAbstractSpinBox;
class QDateTimeEdit;
class QIcon;
class BOBUIoolButton;

class Q_WIDGETS_EXPORT QLineEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged USER true)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(bool frame READ hasFrame WRITE setFrame)
    Q_PROPERTY(EchoMode echoMode READ echoMode WRITE setEchoMode)
    Q_PROPERTY(QString displayText READ displayText)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(BobUI::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool modified READ isModified WRITE setModified DESIGNABLE false)
    Q_PROPERTY(bool hasSelectedText READ hasSelectedText)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(bool dragEnabled READ dragEnabled WRITE setDragEnabled)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool undoAvailable READ isUndoAvailable)
    Q_PROPERTY(bool redoAvailable READ isRedoAvailable)
    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(BobUI::CursorMoveStyle cursorMoveStyle READ cursorMoveStyle WRITE setCursorMoveStyle)
    Q_PROPERTY(bool clearButtonEnabled READ isClearButtonEnabled WRITE setClearButtonEnabled)
public:
    enum ActionPosition {
        LeadingPosition,
        TrailingPosition
    };
    Q_ENUM(ActionPosition)

    explicit QLineEdit(QWidget *parent = nullptr);
    explicit QLineEdit(const QString &, QWidget *parent = nullptr);
    ~QLineEdit();

    QString text() const;

    QString displayText() const;

    QString placeholderText() const;
    void setPlaceholderText(const QString &);

    int maxLength() const;
    void setMaxLength(int);

    void setFrame(bool);
    bool hasFrame() const;

    void setClearButtonEnabled(bool enable);
    bool isClearButtonEnabled() const;

    enum EchoMode { Normal, NoEcho, Password, PasswordEchoOnEdit };
    Q_ENUM(EchoMode)
    EchoMode echoMode() const;
    void setEchoMode(EchoMode);

    bool isReadOnly() const;
    void setReadOnly(bool);

#ifndef BOBUI_NO_VALIDATOR
    void setValidator(const QValidator *);
    const QValidator * validator() const;
#endif

#if BOBUI_CONFIG(completer)
    void setCompleter(QCompleter *completer);
    QCompleter *completer() const;
#endif

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    int cursorPosition() const;
    void setCursorPosition(int);
    int cursorPositionAt(const QPoint &pos);

    void setAlignment(BobUI::Alignment flag);
    BobUI::Alignment alignment() const;

    void cursorForward(bool mark, int steps = 1);
    void cursorBackward(bool mark, int steps = 1);
    void cursorWordForward(bool mark);
    void cursorWordBackward(bool mark);
    void backspace();
    void del();
    void home(bool mark);
    void end(bool mark);

    bool isModified() const;
    void setModified(bool);

    void setSelection(int, int);
    bool hasSelectedText() const;
    QString selectedText() const;
    int selectionStart() const;
    int selectionEnd() const;
    int selectionLength() const;

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    void setDragEnabled(bool b);
    bool dragEnabled() const;

    void setCursorMoveStyle(BobUI::CursorMoveStyle style);
    BobUI::CursorMoveStyle cursorMoveStyle() const;

    QString inputMask() const;
    void setInputMask(const QString &inputMask);
    bool hasAcceptableInput() const;

    void setTextMargins(int left, int top, int right, int bottom);
    void setTextMargins(const QMargins &margins);
    QMargins textMargins() const;

#if BOBUI_CONFIG(action)
    using QWidget::addAction;
    void addAction(QAction *action, ActionPosition position);
    QAction *addAction(const QIcon &icon, ActionPosition position);
#endif

public Q_SLOTS:
    void setText(const QString &);
    void clear();
    void selectAll();
    void undo();
    void redo();
#ifndef BOBUI_NO_CLIPBOARD
    void cut();
    void copy() const;
    void paste();
#endif

public:
    void deselect();
    void insert(const QString &);
#ifndef BOBUI_NO_CONTEXTMENU
    QMenu *createStandardContextMenu();
#endif

Q_SIGNALS:
    void textChanged(const QString &);
    void textEdited(const QString &);
    void cursorPositionChanged(int, int);
    void returnPressed();
    void editingFinished();
    void selectionChanged();
    void inputRejected();

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;
    void paintEvent(QPaintEvent *) override;
#if BOBUI_CONFIG(draganddrop)
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *) override;
#endif
    void changeEvent(QEvent *) override;
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *) override;
#endif

    void inputMethodEvent(QInputMethodEvent *) override;
    virtual void initStyleOption(QStyleOptionFrame *option) const;
public:
    QVariant inputMethodQuery(BobUI::InputMethodQuery) const override;
    Q_INVOKABLE QVariant inputMethodQuery(BobUI::InputMethodQuery property, QVariant argument) const;
    void timerEvent(BOBUIimerEvent *) override;
    bool event(QEvent *) override;
protected:
    QRect cursorRect() const;

public:

private:
    friend class QAbstractSpinBox;
    friend class QAccessibleLineEdit;
    friend class QComboBox;
#ifdef BOBUI_KEYPAD_NAVIGATION
    friend class QDateTimeEdit;
#endif
    Q_DISABLE_COPY(QLineEdit)
    Q_DECLARE_PRIVATE(QLineEdit)
};

BOBUI_END_NAMESPACE

#endif // QLINEEDIT_H
