// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTNATIVEEVENTFILTER_H
#define QABSTRACTNATIVEEVENTFILTER_H

#include <BobUICore/qnamespace.h>

BOBUI_BEGIN_NAMESPACE

class QAbstractNativeEventFilterPrivate;

class Q_CORE_EXPORT QAbstractNativeEventFilter
{
public:
    QAbstractNativeEventFilter();
    virtual ~QAbstractNativeEventFilter();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) = 0;

private:
    Q_DISABLE_COPY(QAbstractNativeEventFilter)
    QAbstractNativeEventFilterPrivate *d;
};

BOBUI_END_NAMESPACE

#endif /* QABSTRACTNATIVEEVENTFILTER_H */
