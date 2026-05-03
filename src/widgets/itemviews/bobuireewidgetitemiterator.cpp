// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <private/bobuireewidgetitemiterator_p.h>
#include "bobuireewidget.h"
#include "bobuireewidget_p.h"
#include "qwidgetitemdata_p.h"

BOBUI_BEGIN_NAMESPACE

/*!
  \class BOBUIreeWidgetItemIterator
  \ingroup model-view
  \inmodule BobUIWidgets

  \brief The BOBUIreeWidgetItemIterator class provides a way to iterate over the
  items in a BOBUIreeWidget instance.

  The iterator will walk the items in a pre-order traversal order, thus visiting the
  parent node \e before it continues to the child nodes.

  For example, the following code examples each item in a tree, checking the
  text in the first column against a user-specified search string:

  \snippet bobuireewidgetitemiterator-using/mainwindow.cpp 0

  It is also possible to filter out certain types of node by passing certain
  \l{IteratorFlag}{flags} to the constructor of BOBUIreeWidgetItemIterator.

  \sa BOBUIreeWidget, {Model/View Programming}, BOBUIreeWidgetItem
*/

/*!
    Constructs an iterator for the same BOBUIreeWidget as \a it. The
    current iterator item is set to point on the current item of \a it.
*/

BOBUIreeWidgetItemIterator::BOBUIreeWidgetItemIterator(const BOBUIreeWidgetItemIterator &it)
    :  d_ptr(new BOBUIreeWidgetItemIteratorPrivate(*(it.d_ptr))),
    current(it.current), flags(it.flags)
{
    Q_D(BOBUIreeWidgetItemIterator);
    Q_ASSERT(d->m_model);
    d->m_model->iterators.append(this);
}

/*!
    Constructs an iterator for the given \a widget that uses the specified \a flags
    to determine which items are found during iteration.
    The iterator is set to point to the first top-level item contained in the widget,
    or the next matching item if the top-level item doesn't match the flags.

    \sa BOBUIreeWidgetItemIterator::IteratorFlag
*/

BOBUIreeWidgetItemIterator::BOBUIreeWidgetItemIterator(BOBUIreeWidget *widget, IteratorFlags flags)
: current(nullptr), flags(flags)
{
    Q_ASSERT(widget);
    BOBUIreeModel *model = qobject_cast<BOBUIreeModel*>(widget->model());
    Q_ASSERT(model);
    d_ptr.reset(new BOBUIreeWidgetItemIteratorPrivate(this, model));
    model->iterators.append(this);
    if (!model->rootItem->children.isEmpty()) current = model->rootItem->child(0);
    if (current && !matchesFlags(current))
        ++(*this);
}

/*!
    Constructs an iterator for the given \a item that uses the specified \a flags
    to determine which items are found during iteration.
    The iterator is set to point to \a item, or the next matching item if \a item
    doesn't match the flags.

    \sa BOBUIreeWidgetItemIterator::IteratorFlag
*/

BOBUIreeWidgetItemIterator::BOBUIreeWidgetItemIterator(BOBUIreeWidgetItem *item, IteratorFlags flags)
    : d_ptr(new BOBUIreeWidgetItemIteratorPrivate(
                this, qobject_cast<BOBUIreeModel*>(item->view->model()))),
      current(item), flags(flags)
{
    Q_D(BOBUIreeWidgetItemIterator);
    Q_ASSERT(item);
    BOBUIreeModel *model = qobject_cast<BOBUIreeModel*>(item->view->model());
    Q_ASSERT(model);
    model->iterators.append(this);

    // Initialize m_currentIndex and m_parentIndex as it would be if we had traversed from
    // the beginning.
    BOBUIreeWidgetItem *parent = item;
    parent = parent->parent();
    BOBUIreeWidgetItem *root = d->m_model->rootItem;
    d->m_currentIndex = (parent ? parent : root)->indexOfChild(item);

    while (parent) {
        BOBUIreeWidgetItem *itm = parent;
        parent = parent->parent();
        const int index = (parent ? parent : root)->indexOfChild(itm);
        d->m_parentIndex.prepend(index);
    }

    if (current && !matchesFlags(current))
        ++(*this);
}

/*!
    Destroys the iterator.
*/

BOBUIreeWidgetItemIterator::~BOBUIreeWidgetItemIterator()
{
    d_func()->m_model->iterators.removeAll(this);
}

/*!
    Assignment. Makes a copy of \a it and returns a reference to its
    iterator.
*/

BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator=(const BOBUIreeWidgetItemIterator &it)
{
    Q_D(BOBUIreeWidgetItemIterator);
    if (d_func()->m_model != it.d_func()->m_model) {
        d_func()->m_model->iterators.removeAll(this);
        it.d_func()->m_model->iterators.append(this);
    }
    current = it.current;
    flags = it.flags;
    d->operator=(*it.d_func());
    return *this;
}

/*!
    The prefix \c{++} operator (\c{++it}) advances the iterator to the next matching item
    and returns a reference to the resulting iterator.
    Sets the current pointer to \nullptr if the current item is the last matching item.
*/

BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator++()
{
    if (current)
        do {
            current = d_func()->next(current);
        } while (current && !matchesFlags(current));
    return *this;
}

/*!
    The prefix \c{--} operator (\c{--it}) advances the iterator to the previous matching item
    and returns a reference to the resulting iterator.
    Sets the current pointer to \nullptr if the current item is the first matching item.
*/

BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator--()
{
    if (current)
        do {
            current = d_func()->previous(current);
        } while (current && !matchesFlags(current));
    return *this;
}

/*!
  \internal
*/
bool BOBUIreeWidgetItemIterator::matchesFlags(const BOBUIreeWidgetItem *item) const
{
    if (!item)
        return false;

    if (flags == All)
        return true;

    {
        BobUI::ItemFlags itemFlags = item->flags();
        if ((flags & Selectable) && !(itemFlags & BobUI::ItemIsSelectable))
            return false;
        if ((flags & NotSelectable) && (itemFlags & BobUI::ItemIsSelectable))
            return false;
        if ((flags & DragEnabled) && !(itemFlags & BobUI::ItemIsDragEnabled))
            return false;
        if ((flags & DragDisabled) && (itemFlags & BobUI::ItemIsDragEnabled))
            return false;
        if ((flags & DropEnabled) && !(itemFlags & BobUI::ItemIsDropEnabled))
            return false;
        if ((flags & DropDisabled) && (itemFlags & BobUI::ItemIsDropEnabled))
            return false;
        if ((flags & Enabled) && !(itemFlags & BobUI::ItemIsEnabled))
            return false;
        if ((flags & Disabled) && (itemFlags & BobUI::ItemIsEnabled))
            return false;
        if ((flags & Editable) && !(itemFlags & BobUI::ItemIsEditable))
            return false;
        if ((flags & NotEditable) && (itemFlags & BobUI::ItemIsEditable))
            return false;
    }

    if (flags & (Checked|NotChecked)) {
        // ### We only test the check state for column 0
        BobUI::CheckState check = item->checkState(0);
        // PartiallyChecked matches as Checked.
        if ((flags & Checked) && (check == BobUI::Unchecked))
            return false;
        if ((flags & NotChecked) && (check != BobUI::Unchecked))
            return false;
    }

    if ((flags & HasChildren) && !item->childCount())
        return false;
    if ((flags & NoChildren) && item->childCount())
        return false;

    if ((flags & Hidden) && !item->isHidden())
        return false;
    if ((flags & NotHidden) && item->isHidden())
        return false;

    if ((flags & Selected) && !item->isSelected())
        return false;
    if ((flags & Unselected) && item->isSelected())
        return false;

    return true;
}

/*
 * Implementation of BOBUIreeWidgetItemIteratorPrivate
 */
BOBUIreeWidgetItem* BOBUIreeWidgetItemIteratorPrivate::nextSibling(const BOBUIreeWidgetItem* item) const
{
    Q_ASSERT(item);
    BOBUIreeWidgetItem *next = nullptr;
    if (BOBUIreeWidgetItem *par = item->parent()) {
        int i = par->indexOfChild(const_cast<BOBUIreeWidgetItem*>(item));
        next = par->child(i + 1);
    } else {
        BOBUIreeWidget *tw = item->treeWidget();
        int i = tw->indexOfTopLevelItem(const_cast<BOBUIreeWidgetItem*>(item));
        next = tw->topLevelItem(i + 1);
    }
    return next;
}

BOBUIreeWidgetItem *BOBUIreeWidgetItemIteratorPrivate::next(const BOBUIreeWidgetItem *current)
{
    if (!current) return nullptr;

    BOBUIreeWidgetItem *next = nullptr;
    if (current->childCount()) {
        // walk the child
        m_parentIndex.push(m_currentIndex);
        m_currentIndex = 0;
        next = current->child(0);
    } else {
        // walk the sibling
        BOBUIreeWidgetItem *parent = current->parent();
        next = parent ? parent->child(m_currentIndex + 1)
                      : m_model->rootItem->child(m_currentIndex + 1);
        while (!next && parent) {
            // if we had no sibling walk up the parent and try the sibling of that
            parent = parent->parent();
            m_currentIndex = m_parentIndex.pop();
            next = parent ? parent->child(m_currentIndex + 1)
                          : m_model->rootItem->child(m_currentIndex + 1);
        }
        if (next) ++(m_currentIndex);
    }
    return next;
}

BOBUIreeWidgetItem *BOBUIreeWidgetItemIteratorPrivate::previous(const BOBUIreeWidgetItem *current)
{
    if (!current) return nullptr;

    BOBUIreeWidgetItem *prev = nullptr;
    // walk the previous sibling
    BOBUIreeWidgetItem *parent = current->parent();
    prev = parent ? parent->child(m_currentIndex - 1)
                  : m_model->rootItem->child(m_currentIndex - 1);
    if (prev) {
        // Yes, we had a previous sibling but we need go down to the last leafnode.
        --m_currentIndex;
        while (prev && prev->childCount()) {
            m_parentIndex.push(m_currentIndex);
            m_currentIndex = prev->childCount() - 1;
            prev = prev->child(m_currentIndex);
        }
    } else if (parent) {
        m_currentIndex = m_parentIndex.pop();
        prev = parent;
    }
    return prev;
}

void BOBUIreeWidgetItemIteratorPrivate::ensureValidIterator(const BOBUIreeWidgetItem *itemToBeRemoved)
{
    Q_Q(BOBUIreeWidgetItemIterator);
    Q_ASSERT(itemToBeRemoved);

    if (!q->current) return;
    BOBUIreeWidgetItem *nextItem = q->current;

    // Do not walk to the ancestor to find the other item if they have the same parent.
    if (nextItem->parent() != itemToBeRemoved->parent()) {
        while (nextItem->parent() && nextItem != itemToBeRemoved) {
            nextItem = nextItem->parent();
        }
    }
    // If the item to be removed is an ancestor of the current iterator item,
    // we need to adjust the iterator.
    if (nextItem == itemToBeRemoved) {
        BOBUIreeWidgetItem *parent = nextItem;
        nextItem = nullptr;
        while (parent && !nextItem) {
            nextItem = nextSibling(parent);
            parent = parent->parent();
        }
        if (nextItem) {
            // Ooooh... Set the iterator to the next valid item
            *q = BOBUIreeWidgetItemIterator(nextItem, q->flags);
            if (!(q->matchesFlags(nextItem))) ++(*q);
        } else {
            // set it to null.
            q->current = nullptr;
            m_parentIndex.clear();
            return;
        }
    }
    if (nextItem->parent() == itemToBeRemoved->parent()) {
        // They have the same parent, i.e. we have to adjust the m_currentIndex member of the iterator
        // if the deleted item is to the left of the nextItem.

        BOBUIreeWidgetItem *par = itemToBeRemoved->parent();   // We know they both have the same parent.
        BOBUIreeWidget *tw = itemToBeRemoved->treeWidget();    // ..and widget
        int indexOfItemToBeRemoved = par ? par->indexOfChild(const_cast<BOBUIreeWidgetItem *>(itemToBeRemoved))
            : tw->indexOfTopLevelItem(const_cast<BOBUIreeWidgetItem *>(itemToBeRemoved));
        int indexOfNextItem = par ? par->indexOfChild(nextItem) : tw->indexOfTopLevelItem(nextItem);

        if (indexOfItemToBeRemoved <= indexOfNextItem) {
            // A sibling to the left of us was deleted, adjust the m_currentIndex member of the iterator.
            // Note that the m_currentIndex will be wrong until the item is actually removed!
            m_currentIndex--;
        }
    }
}

/*!
  \fn const BOBUIreeWidgetItemIterator BOBUIreeWidgetItemIterator::operator++(int)

  The postfix ++ operator (it++) advances the iterator to the next matching item
  and returns an iterator to the previously current item.
*/

/*!
  \fn BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator+=(int n)

  Makes the iterator go forward by \a n matching items. (If n is negative, the
  iterator goes backward.)

  If the current item is beyond the last item, the current item pointer is
  set to \nullptr. Returns the resulting iterator.
*/

/*!
  \fn const BOBUIreeWidgetItemIterator BOBUIreeWidgetItemIterator::operator--(int)

  The postfix -- operator (it--) makes the preceding matching item current and returns an iterator to the previously current item.
*/

/*!
  \fn BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator-=(int n)

  Makes the iterator go backward by \a n matching items. (If n is negative, the
  iterator goes forward.)

  If the current item is ahead of the last item, the current item pointer is
  set to \nullptr. Returns the resulting iterator.
*/

/*!
  \fn BOBUIreeWidgetItem *BOBUIreeWidgetItemIterator::operator*() const

  Dereference operator. Returns a pointer to the current item.
*/


/*!
    \enum BOBUIreeWidgetItemIterator::IteratorFlag

    These flags can be passed to a BOBUIreeWidgetItemIterator constructor
    (OR-ed together if more than one is used), so that the iterator
    will only iterate over items that match the given flags.

    \value All
    \value Hidden
    \value NotHidden
    \value Selected
    \value Unselected
    \value Selectable
    \value NotSelectable
    \value DragEnabled
    \value DragDisabled
    \value DropEnabled
    \value DropDisabled
    \value HasChildren
    \value NoChildren
    \value Checked
    \value NotChecked
    \value Enabled
    \value Disabled
    \value Editable
    \value NotEditable
    \value UserFlag
*/

BOBUI_END_NAMESPACE
