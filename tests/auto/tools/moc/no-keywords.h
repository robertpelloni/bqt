// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef NO_KEYWORDS_H
#define NO_KEYWORDS_H

#define BOBUI_NO_KEYWORDS
#undef signals
#undef slots
#undef emit
#define signals FooBar
#define slots Baz
#define emit Yoyodyne

#include <BobUICore/BobUICore>

#ifdef BOBUI_CONCURRENT_LIB
#include <BobUIConcurrent/BobUIConcurrent>
#endif
#ifdef BOBUI_NETWORK_LIB
#include <BobUINetwork/BobUINetwork>
#endif
#ifdef BOBUI_SQL_LIB
#include <BobUISql/BobUISql>
#endif
#ifdef BOBUI_DBUS_LIB
#include <BobUIDBus/BobUIDBus>
#endif

#undef signals
#undef slots
#undef emit

class MyBooooooostishClass : public QObject
{
    Q_OBJECT
public:
    inline MyBooooooostishClass() {}

Q_SIGNALS:
    void mySignal();

public Q_SLOTS:
    inline void mySlot() { mySignal(); }

private:
    Q_DECL_UNUSED_MEMBER int signals;
    Q_DECL_UNUSED_MEMBER double slots;
};

#define signals Q_SIGNALS
#define slots Q_SLOTS
#define emit Q_EMIT
#undef BOBUI_NO_KEYWORDS

#endif // NO_KEYWORDS_H
