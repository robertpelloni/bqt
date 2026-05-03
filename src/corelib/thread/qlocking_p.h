// Copyright (C) 2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc Mutz <marc.mutz@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOCKING_P_H
#define QLOCKING_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience of
// qmutex.cpp and qmutex_unix.cpp. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qmutex.h>
#include <BobUICore/private/qglobal_p.h>

#include <mutex>

BOBUI_BEGIN_NAMESPACE

//
// This API is bridging the time until we can depend on C++17:
//
// - bobui_scoped_lock returns a lock that cannot be unlocked again before the end of the scope
// - bobui_unique_lock returns a lock that can be unlock()ed and moved around
// - for compat with QMutexLocker, bobui_unique_lock supports passing by pointer.
//   Do NOT use this overload lightly; it's only for cases such as where a Q_GLOBAL_STATIC
//   may have already been deleted. In particular, do NOT port from
//       QMutexLocker locker(&mutex);
//   to
//       auto locker = bobui_unique_lock(&mutex);
//   as this will not port automatically to std::unique_lock come C++17!
//
// The intent, come C++17, is to replace
//     bobui_scoped_lock(mutex);
//     bobui_unique_lock(mutex); // except bobui_unique_lock(&mutex)
// with
//     std::scoped_lock(mutex);
//     std::unique_lock(mutex);
// resp. (C++17 meaning CTAD, guaranteed copy elision + scoped_lock available on all platforms),
// so please use these functions only in ways which don't break this mechanical search & replace.
//

namespace {

template <typename Mutex, typename Lock =
# if defined(__cpp_lib_scoped_lock) && __cpp_lib_scoped_lock >= 201703L
          std::scoped_lock
# else
          std::lock_guard
# endif
          <typename std::decay<Mutex>::type>
>
Lock bobui_scoped_lock(Mutex &mutex)
{
    return Lock(mutex);
}

template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock bobui_unique_lock(Mutex &mutex)
{
    return Lock(mutex);
}

template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock bobui_unique_lock(Mutex *mutex)
{
    return mutex ? Lock(*mutex) : Lock() ;
}

} // unnamed namespace

BOBUI_END_NAMESPACE

#endif // QLOCKING_P_H
