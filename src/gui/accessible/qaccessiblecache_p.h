// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLECACHE_P
#define QACCESSIBLECACHE_P

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
#include <BobUICore/qobject.h>
#include <BobUICore/qhash.h>

#include "qaccessible.h"

#if BOBUI_CONFIG(accessibility)

Q_FORWARD_DECLARE_OBJC_CLASS(BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement));

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QAccessibleObjectDestroyedEvent :public QAccessibleEvent
{
public:
    /* A regular event, used when the object is still alive
    */
    inline QAccessibleObjectDestroyedEvent(QObject *obj)
        : QAccessibleEvent(obj, QAccessible::ObjectDestroyed)
    {
    }
    /* A regular event, used when there is no object
    */
    inline QAccessibleObjectDestroyedEvent(QAccessibleInterface *iface)
        : QAccessibleEvent(static_cast<QObject *>(nullptr), QAccessible::ObjectDestroyed)
    {
        m_uniqueId = QAccessible::uniqueId(iface);
    }
    /* An event with object() == nullptr, and uniqueid set,
    used during object destruction */
    inline QAccessibleObjectDestroyedEvent(QAccessible::Id uniqueId)
        : QAccessibleEvent(static_cast<QObject *>(nullptr), QAccessible::ObjectDestroyed)
    {
        m_uniqueId = uniqueId;
    }
    ~QAccessibleObjectDestroyedEvent();
};

class Q_GUI_EXPORT QAccessibleCache  :public QObject
{
    Q_OBJECT

public:
    ~QAccessibleCache() override;
    static QAccessibleCache *instance();
    QAccessibleInterface *interfaceForId(QAccessible::Id id) const;
    QAccessible::Id idForInterface(QAccessibleInterface *iface) const;
    QAccessible::Id idForObject(QObject *obj) const;
    bool containsObject(QObject *obj) const;
    QAccessible::Id insert(QObject *object, QAccessibleInterface *iface) const;
    void deleteInterface(QAccessible::Id id, QObject *obj = nullptr);
    void sendObjectDestroyedEvent(QObject *obj);
#ifdef Q_OS_APPLE
    BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement) *elementForId(QAccessible::Id axid) const;
    bool insertElement(QAccessible::Id axid, BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement) *element) const;
#endif

private Q_SLOTS:
    void objectDestroyed(QObject *obj);

private:
    QAccessible::Id acquireId() const;

    mutable QHash<QAccessible::Id, QAccessibleInterface *> idToInterface;
    mutable QHash<QAccessibleInterface *, QAccessible::Id> interfaceToId;
    mutable QMultiHash<QObject *, std::pair<QAccessible::Id, const QMetaObject*>> objectToId;

#ifdef Q_OS_APPLE
    void removeAccessibleElement(QAccessible::Id axid);
    mutable QHash<QAccessible::Id, BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement) *> accessibleElements;
#endif

    friend class QAccessible;
    friend class QAccessibleInterface;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif
