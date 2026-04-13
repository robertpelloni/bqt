// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIREEWIDGETITEMITERATOR_H
#define BOBUIREEWIDGETITEMITERATOR_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qscopedpointer.h>

BOBUI_REQUIRE_CONFIG(treewidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIreeWidget;
class BOBUIreeWidgetItem;
class BOBUIreeModel;

class BOBUIreeWidgetItemIteratorPrivate;
class Q_WIDGETS_EXPORT BOBUIreeWidgetItemIterator
{
    friend class BOBUIreeModel;

public:
    enum IteratorFlag {
        All           = 0x00000000,
        Hidden        = 0x00000001,
        NotHidden     = 0x00000002,
        Selected      = 0x00000004,
        Unselected    = 0x00000008,
        Selectable    = 0x00000010,
        NotSelectable = 0x00000020,
        DragEnabled   = 0x00000040,
        DragDisabled  = 0x00000080,
        DropEnabled   = 0x00000100,
        DropDisabled  = 0x00000200,
        HasChildren   = 0x00000400,
        NoChildren    = 0x00000800,
        Checked       = 0x00001000,
        NotChecked    = 0x00002000,
        Enabled       = 0x00004000,
        Disabled      = 0x00008000,
        Editable      = 0x00010000,
        NotEditable   = 0x00020000,
        UserFlag      = 0x01000000 // The first flag that can be used by the user.
    };
    Q_DECLARE_FLAGS(IteratorFlags, IteratorFlag)

    BOBUIreeWidgetItemIterator(const BOBUIreeWidgetItemIterator &it);
    explicit BOBUIreeWidgetItemIterator(BOBUIreeWidget *widget, IteratorFlags flags = All);
    explicit BOBUIreeWidgetItemIterator(BOBUIreeWidgetItem *item, IteratorFlags flags = All);
    ~BOBUIreeWidgetItemIterator();

    BOBUIreeWidgetItemIterator &operator=(const BOBUIreeWidgetItemIterator &it);

    BOBUIreeWidgetItemIterator &operator++();
    inline const BOBUIreeWidgetItemIterator operator++(int);
    inline BOBUIreeWidgetItemIterator &operator+=(int n);

    BOBUIreeWidgetItemIterator &operator--();
    inline const BOBUIreeWidgetItemIterator operator--(int);
    inline BOBUIreeWidgetItemIterator &operator-=(int n);

    inline BOBUIreeWidgetItem *operator*() const;

private:
    bool matchesFlags(const BOBUIreeWidgetItem *item) const;
    QScopedPointer<BOBUIreeWidgetItemIteratorPrivate> d_ptr;
    BOBUIreeWidgetItem *current;
    IteratorFlags flags;
    Q_DECLARE_PRIVATE(BOBUIreeWidgetItemIterator)
};

inline const BOBUIreeWidgetItemIterator BOBUIreeWidgetItemIterator::operator++(int)
{
    BOBUIreeWidgetItemIterator it = *this;
    ++(*this);
    return it;
}

inline const BOBUIreeWidgetItemIterator BOBUIreeWidgetItemIterator::operator--(int)
{
    BOBUIreeWidgetItemIterator it = *this;
    --(*this);
    return it;
}

inline BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator+=(int n)
{
    if (n < 0)
        return (*this) -= (-n);
    while (current && n--)
        ++(*this);
    return *this;
}

inline BOBUIreeWidgetItemIterator &BOBUIreeWidgetItemIterator::operator-=(int n)
{
    if (n < 0)
        return (*this) += (-n);
    while (current && n--)
        --(*this);
    return *this;
}

inline BOBUIreeWidgetItem *BOBUIreeWidgetItemIterator::operator*() const
{
    return current;
}

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIreeWidgetItemIterator::IteratorFlags)

BOBUI_END_NAMESPACE
#endif // BOBUIREEWIDGETITEMITERATOR_H
