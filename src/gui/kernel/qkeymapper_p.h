// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef QKEYMAPPER_P_H
#define QKEYMAPPER_P_H

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
#include <qobject.h>
#include <private/qobject_p.h>
#include <qlist.h>
#include <qlocale.h>
#include <qevent.h>

#include <BobUICore/qnativeinterface.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QKeyMapper : public QObject
{
    Q_OBJECT
public:
    explicit QKeyMapper();
    ~QKeyMapper();

    static QKeyMapper *instance();
    static QList<QKeyCombination> possibleKeys(const QKeyEvent *e);

    BOBUI_DECLARE_NATIVE_INTERFACE_ACCESSOR(QKeyMapper)

private:
    Q_DISABLE_COPY_MOVE(QKeyMapper)
};

// ----------------- QNativeInterface -----------------

namespace QNativeInterface::Private {

#if BOBUI_CONFIG(evdev) || defined(Q_QDOC)
struct Q_GUI_EXPORT QEvdevKeyMapper
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QEvdevKeyMapper, 1, QKeyMapper)
    virtual void loadKeymap(const QString &filename) = 0;
    virtual void switchLang() = 0;
};
#endif

#if BOBUI_CONFIG(vxworksevdev) || defined(Q_QDOC)
struct Q_GUI_EXPORT QVxKeyMapper
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QVxKeyMapper, 1, QKeyMapper)
    virtual void loadKeymap(const QString &filename) = 0;
    virtual void switchLang() = 0;
};
#endif

} // QNativeInterface::Private


BOBUI_END_NAMESPACE

#endif // QKEYMAPPER_P_H
