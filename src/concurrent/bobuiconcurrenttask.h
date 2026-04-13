// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICONCURRENTTASK_H
#define BOBUICONCURRENTTASK_H

#if 0
#pragma bobui_class(BobUIConcurrentTask)
#endif

#if !defined(BOBUI_NO_CONCURRENT)

#include <BobUIConcurrent/bobuiaskbuilder.h>

BOBUI_BEGIN_NAMESPACE

#ifdef Q_QDOC

namespace BobUIConcurrent {

template <class Task>
[[nodiscard]]
BOBUIaskBuilder<Task> task(Task &&task);

} // namespace BobUIConcurrent

#else

namespace BobUIConcurrent {

template <class Task>
[[nodiscard]]
constexpr auto task(Task &&t) { return BOBUIaskBuilder(std::forward<Task>(t)); }

} // namespace BobUIConcurrent

#endif // Q_QDOC

BOBUI_END_NAMESPACE

#endif // !defined(BOBUI_NO_CONCURRENT)

#endif // BOBUICONCURRENTTASK_H
