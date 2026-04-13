// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QVECTOR_H
#define QVECTOR_H

#include <BobUICore/qlist.h>
#include <BobUICore/qcontainerfwd.h>

#if 0
#pragma bobui_class(QVector)
#pragma bobui_class(QMutableVectorIterator)
#pragma bobui_class(QVectorIterator)
#endif

BOBUI_BEGIN_NAMESPACE

#if !defined(BOBUI_NO_JAVA_STYLE_ITERATORS)
template<typename T>
using QMutableVectorIterator = QMutableListIterator<T>;
template<typename T>
using QVectorIterator = QListIterator<T>;
#endif

BOBUI_END_NAMESPACE

#endif // QVECTOR_H
