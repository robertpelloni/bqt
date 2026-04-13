// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuioolbox.h"

#include <qapplication.h>
#include <qeventloop.h>
#include <qlayout.h>
#include <qlist.h>
#include <qpainter.h>
#include <qscrollarea.h>
#include <qstyle.h>
#include <qstyleoption.h>
#if BOBUI_CONFIG(tooltip)
#include <bobuiooltip.h>
#endif
#include <qabstractbutton.h>

#include "qframe_p.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class BOBUIoolBoxButton : public QAbstractButton
{
    Q_OBJECT
public:
    BOBUIoolBoxButton(QWidget *parent)
        : QAbstractButton(parent), selected(false), indexInPage(-1)
    {
        setBackgroundRole(QPalette::Window);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        setFocusPolicy(BobUI::NoFocus);
    }

    inline void setSelected(bool b) { selected = b; update(); }
    inline void setIndex(int newIndex) { indexInPage = newIndex; }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void initStyleOption(QStyleOptionToolBox *opt) const;
    void paintEvent(QPaintEvent *) override;

private:
    bool selected;
    int indexInPage;
};


class BOBUIoolBoxPrivate : public QFramePrivate
{
    Q_DECLARE_PUBLIC(BOBUIoolBox)
public:
    struct Page
    {
        BOBUIoolBoxButton *button;
        QScrollArea *sv;
        QWidget *widget;

        inline void setText(const QString &text) { button->setText(text); }
        inline void setIcon(const QIcon &is) { button->setIcon(is); }
#if BOBUI_CONFIG(tooltip)
        inline void setToolTip(const QString &tip) { button->setToolTip(tip); }
        inline QString toolTip() const { return button->toolTip(); }
#endif
        inline QString text() const { return button->text(); }
        inline QIcon icon() const { return button->icon(); }

        inline bool operator==(const Page& other) const
        {
            return widget == other.widget;
        }
    };
    typedef std::vector<std::unique_ptr<Page>> PageList;

    inline BOBUIoolBoxPrivate()
        : currentPage(nullptr)
    {
    }
    void _q_buttonClicked();
    void _q_widgetDestroyed(QObject*);

    const Page *page(const QObject *widget) const;
    const Page *page(int index) const;
    Page *page(int index);

    void updateTabs();
    void relayout();

    PageList pageList;
    QVBoxLayout *layout;
    Page *currentPage;
};

const BOBUIoolBoxPrivate::Page *BOBUIoolBoxPrivate::page(const QObject *widget) const
{
    if (!widget)
        return nullptr;

    for (const auto &page : pageList) {
        if (page->widget == widget)
            return page.get();
    }
    return nullptr;
}

BOBUIoolBoxPrivate::Page *BOBUIoolBoxPrivate::page(int index)
{
    if (index >= 0 && index < static_cast<int>(pageList.size()))
        return pageList[index].get();
    return nullptr;
}

const BOBUIoolBoxPrivate::Page *BOBUIoolBoxPrivate::page(int index) const
{
    if (index >= 0 && index < static_cast<int>(pageList.size()))
        return pageList[index].get();
    return nullptr;
}

void BOBUIoolBoxPrivate::updateTabs()
{
    BOBUIoolBoxButton *lastButton = currentPage ? currentPage->button : nullptr;
    bool after = false;
    int index = 0;
    for (const auto &page : pageList) {
        BOBUIoolBoxButton *tB = page->button;
        // update indexes, since the updates are delayed, the indexes will be correct
        // when we actually paint.
        tB->setIndex(index);
        QWidget *tW = page->widget;
        if (after) {
            QPalette p = tB->palette();
            p.setColor(tB->backgroundRole(), tW->palette().color(tW->backgroundRole()));
            tB->setPalette(p);
            tB->update();
        } else if (tB->backgroundRole() != QPalette::Window) {
            tB->setBackgroundRole(QPalette::Window);
            tB->update();
        }
        after = tB == lastButton;
        ++index;
    }
}

QSize BOBUIoolBoxButton::sizeHint() const
{
    QSize iconSize(8, 8);
    if (!icon().isNull()) {
        int icone = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, parentWidget() /* BOBUIoolBox */);
        iconSize += QSize(icone + 2, icone);
    }
    QSize textSize = fontMetrics().size(BobUI::TextShowMnemonic, text()) + QSize(0, 8);

    return QSize(iconSize.width() + textSize.width(), qMax(iconSize.height(), textSize.height()));
}

QSize BOBUIoolBoxButton::minimumSizeHint() const
{
    if (icon().isNull())
        return QSize();
    int icone = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, parentWidget() /* BOBUIoolBox */);
    return QSize(icone + 8, icone + 8);
}

void BOBUIoolBoxButton::initStyleOption(QStyleOptionToolBox *option) const
{
    if (!option)
        return;
    option->initFrom(this);
    if (selected)
        option->state |= QStyle::State_Selected;
    if (isDown())
        option->state |= QStyle::State_Sunken;
    option->text = text();
    option->icon = icon();

    BOBUIoolBox *toolBox = static_cast<BOBUIoolBox *>(parentWidget()); // I know I'm in a tool box.
    const int widgetCount = toolBox->count();
    const int currIndex = toolBox->currentIndex();
    if (widgetCount == 1) {
        option->position = QStyleOptionToolBox::OnlyOneTab;
    } else if (indexInPage == 0) {
        option->position = QStyleOptionToolBox::Beginning;
    } else if (indexInPage == widgetCount - 1) {
        option->position = QStyleOptionToolBox::End;
    } else {
        option->position = QStyleOptionToolBox::Middle;
    }
    if (currIndex == indexInPage - 1) {
        option->selectedPosition = QStyleOptionToolBox::PreviousIsSelected;
    } else if (currIndex == indexInPage + 1) {
        option->selectedPosition = QStyleOptionToolBox::NextIsSelected;
    } else {
        option->selectedPosition = QStyleOptionToolBox::NotAdjacent;
    }
}

void BOBUIoolBoxButton::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    QPainter *p = &paint;
    QStyleOptionToolBox opt;
    initStyleOption(&opt);
    style()->drawControl(QStyle::CE_ToolBoxTab, &opt, p, parentWidget());
}

/*!
    \class BOBUIoolBox

    \brief The BOBUIoolBox class provides a column of tabbed widget items.


    \ingroup basicwidgets
    \inmodule BobUIWidgets

    A toolbox is a widget that displays a column of tabs one above the
    other, with the current item displayed below the current tab.
    Every tab has an index position within the column of tabs. A tab's
    item is a QWidget.

    Each item has an itemText(), an optional itemIcon(), an optional
    itemToolTip(), and a widget(). The item's attributes can be
    changed with setItemText(), setItemIcon(), and
    setItemToolTip(). Each item can be enabled or disabled
    individually with setItemEnabled().

    Items are added using addItem(), or inserted at particular
    positions using insertItem(). The total number of items is given
    by count(). Items can be deleted with delete, or removed from the
    toolbox with removeItem(). Combining removeItem() and insertItem()
    allows you to move items to different positions.

    The index of the current item widget is returned by currentIndex(),
    and set with setCurrentIndex(). The index of a particular item can
    be found using indexOf(), and the item at a given index is returned
    by item().

    The currentChanged() signal is emitted when the current item is
    changed.

    \sa BOBUIabWidget
*/

/*!
    \fn void BOBUIoolBox::currentChanged(int index)

    This signal is emitted when the current item is changed. The new
    current item's index is passed in \a index, or -1 if there is no
    current item.
*/


/*!
    Constructs a new toolbox with the given \a parent and the flags, \a f.
*/
BOBUIoolBox::BOBUIoolBox(QWidget *parent, BobUI::WindowFlags f)
    :  QFrame(*new BOBUIoolBoxPrivate, parent, f)
{
    Q_D(BOBUIoolBox);
    d->layout = new QVBoxLayout(this);
    d->layout->setContentsMargins(QMargins());
    setBackgroundRole(QPalette::Button);
}

/*!
    Destroys the toolbox.
*/

BOBUIoolBox::~BOBUIoolBox()
{
}

/*!
    \fn int BOBUIoolBox::addItem(QWidget *w, const QString &text)
    \overload

    Adds the widget \a w in a new tab at bottom of the toolbox. The
    new tab's text is set to \a text. Returns the new tab's index.
*/

/*!
    \fn int BOBUIoolBox::addItem(QWidget *widget, const QIcon &iconSet,const QString &text)
    Adds the \a widget in a new tab at bottom of the toolbox. The
    new tab's text is set to \a text, and the \a iconSet is
    displayed to the left of the \a text.  Returns the new tab's index.
*/

/*!
    \fn int BOBUIoolBox::insertItem(int index, QWidget *widget, const QString &text)
    \overload

    Inserts the \a widget at position \a index, or at the bottom
    of the toolbox if \a index is out of range. The new item's text is
    set to \a text. Returns the new item's index.
*/

/*!
    Inserts the \a widget at position \a index, or at the bottom
    of the toolbox if \a index is out of range. The new item's text
    is set to \a text, and the \a icon is displayed to the left of
    the \a text. Returns the new item's index.
*/

int BOBUIoolBox::insertItem(int index, QWidget *widget, const QIcon &icon, const QString &text)
{
    if (!widget)
        return -1;

    Q_D(BOBUIoolBox);
    connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(_q_widgetDestroyed(QObject*)));

    auto newPage = std::make_unique<BOBUIoolBoxPrivate::Page>();
    auto &c = *newPage;
    c.widget = widget;
    c.button = new BOBUIoolBoxButton(this);
    c.button->setObjectName("bobui_toolbox_toolboxbutton"_L1);
    connect(c.button, SIGNAL(clicked()), this, SLOT(_q_buttonClicked()));

    c.sv = new QScrollArea(this);
    c.sv->setWidget(widget);
    c.sv->setWidgetResizable(true);
    c.sv->hide();
    c.sv->setFrameStyle(QFrame::NoFrame);

    c.setText(text);
    c.setIcon(icon);

    if (index < 0 || index >= static_cast<int>(d->pageList.size())) {
        index = static_cast<int>(d->pageList.size());
        d->pageList.push_back(std::move(newPage));
        d->layout->addWidget(c.button);
        d->layout->addWidget(c.sv);
        if (index == 0)
            setCurrentIndex(index);
    } else {
        d->pageList.insert(d->pageList.cbegin() + index, std::move(newPage));
        d->relayout();
        if (d->currentPage) {
            QWidget *current = d->currentPage->widget;
            int oldindex = indexOf(current);
            if (index <= oldindex) {
                d->currentPage = nullptr; // trigger change
                setCurrentIndex(oldindex);
            }
        }
    }

    c.button->show();

    d->updateTabs();
    itemInserted(index);
    return index;
}

void BOBUIoolBoxPrivate::_q_buttonClicked()
{
    Q_Q(BOBUIoolBox);
    BOBUIoolBoxButton *tb = qobject_cast<BOBUIoolBoxButton*>(q->sender());
    QWidget* item = nullptr;
    for (const auto &page : pageList) {
        if (page->button == tb) {
            item = page->widget;
            break;
        }
    }

    q->setCurrentIndex(q->indexOf(item));
}

/*!
    \property BOBUIoolBox::count
    \brief The number of items contained in the toolbox.

    By default, this property has a value of 0.
*/

int BOBUIoolBox::count() const
{
    Q_D(const BOBUIoolBox);
    return static_cast<int>(d->pageList.size());
}

void BOBUIoolBox::setCurrentIndex(int index)
{
    Q_D(BOBUIoolBox);
    BOBUIoolBoxPrivate::Page *c = d->page(index);
    if (!c || d->currentPage == c)
        return;

    c->button->setSelected(true);
    if (d->currentPage) {
        d->currentPage->sv->hide();
        d->currentPage->button->setSelected(false);
    }
    d->currentPage = c;
    d->currentPage->sv->show();
    d->updateTabs();
    emit currentChanged(index);
}

void BOBUIoolBoxPrivate::relayout()
{
    Q_Q(BOBUIoolBox);
    delete layout;
    layout = new QVBoxLayout(q);
    layout->setContentsMargins(QMargins());
    for (const auto &page : pageList) {
        layout->addWidget(page->button);
        layout->addWidget(page->sv);
    }
}

auto pageEquals = [](const BOBUIoolBoxPrivate::Page *page) {
    return [page](const std::unique_ptr<BOBUIoolBoxPrivate::Page> &ptr) {
        return ptr.get() == page;
    };
};

void BOBUIoolBoxPrivate::_q_widgetDestroyed(QObject *object)
{
    Q_Q(BOBUIoolBox);

    const BOBUIoolBoxPrivate::Page * const c = page(object);
    if (!c)
        return;

    layout->removeWidget(c->sv);
    layout->removeWidget(c->button);
    c->sv->deleteLater(); // page might still be a child of sv
    delete c->button;

    bool removeCurrent = c == currentPage;
    pageList.erase(std::remove_if(pageList.begin(), pageList.end(), pageEquals(c)), pageList.end());

    if (pageList.empty()) {
        currentPage = nullptr;
        emit q->currentChanged(-1);
    } else if (removeCurrent) {
        currentPage = nullptr;
        q->setCurrentIndex(0);
    }
}

/*!
    Removes the item at position \a index from the toolbox. Note that
    the widget is \e not deleted.
*/

void BOBUIoolBox::removeItem(int index)
{
    Q_D(BOBUIoolBox);
    if (QWidget *w = widget(index)) {
        disconnect(w, SIGNAL(destroyed(QObject*)), this, SLOT(_q_widgetDestroyed(QObject*)));
        w->setParent(this);
        // destroy internal data
        d->_q_widgetDestroyed(w);
        itemRemoved(index);
    }
}


/*!
    \property BOBUIoolBox::currentIndex
    \brief the index of the current item

    By default, for an empty toolbox, this property has a value of -1.

    \sa indexOf(), widget()
*/


int BOBUIoolBox::currentIndex() const
{
    Q_D(const BOBUIoolBox);
    return d->currentPage ? indexOf(d->currentPage->widget) : -1;
}

/*!
    Returns a pointer to the current widget, or \nullptr if there is
    no such item.

    \sa currentIndex(), setCurrentWidget()
*/

QWidget * BOBUIoolBox::currentWidget() const
{
    Q_D(const BOBUIoolBox);
    return d->currentPage ? d->currentPage->widget : nullptr;
}

/*!
  Makes\a widget the current widget. The \a widget must be an item in this tool box.

  \sa addItem(), setCurrentIndex(), currentWidget()
 */
void BOBUIoolBox::setCurrentWidget(QWidget *widget)
{
    int i = indexOf(widget);
    if (Q_UNLIKELY(i < 0))
        qWarning("BOBUIoolBox::setCurrentWidget: widget not contained in tool box");
    else
        setCurrentIndex(i);
}

/*!
    Returns the widget at position \a index, or \nullptr if there is
    no such item.
*/

QWidget *BOBUIoolBox::widget(int index) const
{
    Q_D(const BOBUIoolBox);
    if (index < 0 || index >= static_cast<int>(d->pageList.size()))
        return nullptr;
    return d->pageList[index]->widget;
}

/*!
    Returns the index of \a widget, or -1 if the item does not
    exist.
*/

int BOBUIoolBox::indexOf(const QWidget *widget) const
{
    Q_D(const BOBUIoolBox);
    const BOBUIoolBoxPrivate::Page *c = (widget ? d->page(widget) : nullptr);
    if (!c)
        return -1;
    const auto it = std::find_if(d->pageList.cbegin(), d->pageList.cend(), pageEquals(c));
    if (it == d->pageList.cend())
        return -1;
    return static_cast<int>(it - d->pageList.cbegin());
}

/*!
    If \a enabled is true then the item at position \a index is enabled; otherwise
    the item at position \a index is disabled.
*/

void BOBUIoolBox::setItemEnabled(int index, bool enabled)
{
    Q_D(BOBUIoolBox);
    BOBUIoolBoxPrivate::Page *c = d->page(index);
    if (!c)
        return;

    c->button->setEnabled(enabled);
    if (!enabled && c == d->currentPage) {
        int curIndexUp = index;
        int curIndexDown = curIndexUp;
        const int count = static_cast<int>(d->pageList.size());
        while (curIndexUp > 0 || curIndexDown < count-1) {
            if (curIndexDown < count-1) {
                if (d->page(++curIndexDown)->button->isEnabled()) {
                    index = curIndexDown;
                    break;
                }
            }
            if (curIndexUp > 0) {
                if (d->page(--curIndexUp)->button->isEnabled()) {
                    index = curIndexUp;
                    break;
                }
            }
        }
        setCurrentIndex(index);
    }
}


/*!
    Sets the text of the item at position \a index to \a text.

    If the provided text contains an ampersand character ('&'), a
    mnemonic is automatically created for it. The character that
    follows the '&' will be used as the shortcut key. Any previous
    mnemonic will be overwritten, or cleared if no mnemonic is defined
    by the text. See the \l {QShortcut#mnemonic}{QShortcut}
    documentation for details (to display an actual ampersand, use
    '&&').
*/

void BOBUIoolBox::setItemText(int index, const QString &text)
{
    Q_D(BOBUIoolBox);
    BOBUIoolBoxPrivate::Page *c = d->page(index);
    if (c)
        c->setText(text);
}

/*!
    Sets the icon of the item at position \a index to \a icon.
*/

void BOBUIoolBox::setItemIcon(int index, const QIcon &icon)
{
    Q_D(BOBUIoolBox);
    BOBUIoolBoxPrivate::Page *c = d->page(index);
    if (c)
        c->setIcon(icon);
}

#if BOBUI_CONFIG(tooltip)
/*!
    Sets the tooltip of the item at position \a index to \a toolTip.
*/

void BOBUIoolBox::setItemToolTip(int index, const QString &toolTip)
{
    Q_D(BOBUIoolBox);
    BOBUIoolBoxPrivate::Page *c = d->page(index);
    if (c)
        c->setToolTip(toolTip);
}
#endif // BOBUI_CONFIG(tooltip)

/*!
    Returns \c true if the item at position \a index is enabled; otherwise returns \c false.
*/

bool BOBUIoolBox::isItemEnabled(int index) const
{
    Q_D(const BOBUIoolBox);
    const BOBUIoolBoxPrivate::Page *c = d->page(index);
    return c && c->button->isEnabled();
}

/*!
    Returns the text of the item at position \a index, or an empty string if
    \a index is out of range.
*/

QString BOBUIoolBox::itemText(int index) const
{
    Q_D(const BOBUIoolBox);
    const BOBUIoolBoxPrivate::Page *c = d->page(index);
    return (c ? c->text() : QString());
}

/*!
    Returns the icon of the item at position \a index, or a null
    icon if \a index is out of range.
*/

QIcon BOBUIoolBox::itemIcon(int index) const
{
    Q_D(const BOBUIoolBox);
    const BOBUIoolBoxPrivate::Page *c = d->page(index);
    return (c ? c->icon() : QIcon());
}

#if BOBUI_CONFIG(tooltip)
/*!
    Returns the tooltip of the item at position \a index, or an
    empty string if \a index is out of range.
*/

QString BOBUIoolBox::itemToolTip(int index) const
{
    Q_D(const BOBUIoolBox);
    const BOBUIoolBoxPrivate::Page *c = d->page(index);
    return (c ? c->toolTip() : QString());
}
#endif // BOBUI_CONFIG(tooltip)

/*! \reimp */
void BOBUIoolBox::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}

/*! \reimp */
void BOBUIoolBox::changeEvent(QEvent *ev)
{
    Q_D(BOBUIoolBox);
    if (ev->type() == QEvent::StyleChange)
        d->updateTabs();
    QFrame::changeEvent(ev);
}

/*!
  This virtual handler is called after a new item was added or
  inserted at position \a index.

  \sa itemRemoved()
 */
void BOBUIoolBox::itemInserted(int index)
{
    Q_UNUSED(index);
}

/*!
  This virtual handler is called after an item was removed from
  position \a index.

  \sa itemInserted()
 */
void BOBUIoolBox::itemRemoved(int index)
{
    Q_UNUSED(index);
}

/*! \reimp */
bool BOBUIoolBox::event(QEvent *e)
{
    return QFrame::event(e);
}

BOBUI_END_NAMESPACE

#include "moc_bobuioolbox.cpp"
#include "bobuioolbox.moc"
