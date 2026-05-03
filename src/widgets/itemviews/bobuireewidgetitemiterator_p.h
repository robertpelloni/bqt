// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIREEWIDGETITEMITERATOR_P_H
#define BOBUIREEWIDGETITEMITERATOR_P_H

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

#include <BobUICore/qstack.h>

#include "bobuireewidgetitemiterator.h"
#include "private/qglobal_p.h"
#if BOBUI_CONFIG(treewidget)

BOBUI_BEGIN_NAMESPACE

class BOBUIreeModel;
class BOBUIreeWidgetItem;

class BOBUIreeWidgetItemIteratorPrivate {
    Q_DECLARE_PUBLIC(BOBUIreeWidgetItemIterator)
public:
    BOBUIreeWidgetItemIteratorPrivate(BOBUIreeWidgetItemIterator *q, BOBUIreeModel *model)
        : m_currentIndex(0), m_model(model), q_ptr(q)
    {

    }

    BOBUIreeWidgetItemIteratorPrivate(const BOBUIreeWidgetItemIteratorPrivate& other)
        : m_currentIndex(other.m_currentIndex), m_model(other.m_model),
          m_parentIndex(other.m_parentIndex), q_ptr(other.q_ptr)
    {

    }

    BOBUIreeWidgetItemIteratorPrivate &operator=(const BOBUIreeWidgetItemIteratorPrivate& other)
    {
        m_currentIndex = other.m_currentIndex;
        m_parentIndex = other.m_parentIndex;
        m_model = other.m_model;
        return (*this);
    }

    ~BOBUIreeWidgetItemIteratorPrivate()
    {
    }

    BOBUIreeWidgetItem* nextSibling(const BOBUIreeWidgetItem* item) const;
    void ensureValidIterator(const BOBUIreeWidgetItem *itemToBeRemoved);

    BOBUIreeWidgetItem *next(const BOBUIreeWidgetItem *current);
    BOBUIreeWidgetItem *previous(const BOBUIreeWidgetItem *current);
private:
    int             m_currentIndex;
    BOBUIreeModel     *m_model;        // This iterator class should not have ownership of the model.
    QStack<int>     m_parentIndex;
    BOBUIreeWidgetItemIterator *q_ptr;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(treewidget)

#endif //BOBUIREEWIDGETITEMITERATOR_P_H
