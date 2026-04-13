// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTTEXTDOCUMENTLAYOUT_P_H
#define QABSTRACTTEXTDOCUMENTLAYOUT_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "private/qobject_p.h"
#include "bobuiextdocument_p.h"
#include "qabstracttextdocumentlayout.h"

#include "BobUICore/qhash.h"
#include <BobUICore/qpointer.h>

BOBUI_BEGIN_NAMESPACE

struct BOBUIextObjectHandler
{
    BOBUIextObjectHandler() : iface(nullptr) {}
    BOBUIextObjectInterface *iface;
    QPointer<QObject> component;
};
typedef QHash<int, BOBUIextObjectHandler> HandlerHash;

class Q_GUI_EXPORT QAbstractTextDocumentLayoutPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QAbstractTextDocumentLayout)

    inline QAbstractTextDocumentLayoutPrivate()
        : paintDevice(nullptr) {}
    ~QAbstractTextDocumentLayoutPrivate();

    inline void setDocument(BOBUIextDocument *doc) {
        document = doc;
        docPrivate = nullptr;
        if (doc)
            docPrivate = BOBUIextDocumentPrivate::get(doc);
    }

    static QAbstractTextDocumentLayoutPrivate *get(QAbstractTextDocumentLayout *layout)
    {
        return layout->d_func();
    }

    bool hasHandlers() const
    {
        return !handlers.isEmpty();
    }

    inline int _q_dynamicPageCountSlot() const
    { return q_func()->pageCount(); }
    inline QSizeF _q_dynamicDocumentSizeSlot() const
    { return q_func()->documentSize(); }

    HandlerHash handlers;

    void _q_handlerDestroyed(QObject *obj);
    QPaintDevice *paintDevice;

    BOBUIextDocument *document;
    BOBUIextDocumentPrivate *docPrivate;
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTTEXTDOCUMENTLAYOUT_P_H
