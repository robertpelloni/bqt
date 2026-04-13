// Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOSCBUNDLE_P_H
#define QOSCBUNDLE_P_H

#include "qoscmessage_p.h"

#include <BobUICore/QList>

BOBUI_BEGIN_NAMESPACE

class QByteArray;

class QOscBundle
{
    QOscBundle(); // for QList, don't use
    friend class QList<QOscBundle>;
public:
    explicit QOscBundle(const QByteArray &data);

    bool isValid() const { return m_isValid; }
    QList<QOscBundle> bundles() const { return m_bundles; }
    QList<QOscMessage> messages() const { return m_messages; }

private:
    bool m_isValid;
    bool m_immediate;
    quint32 m_timeEpoch;
    quint32 m_timePico;
    QList<QOscBundle> m_bundles;
    QList<QOscMessage> m_messages;
};
Q_DECLARE_TYPEINFO(QOscBundle, Q_RELOCATABLE_TYPE);

BOBUI_END_NAMESPACE

#endif // QOSCBUNDLE_P_H
