// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef FORWARDDECLARED_H
#define FORWARDDECLARED_H

extern int forwardDeclaredDestructorRunCount;
class ForwardDeclared;

#ifdef BOBUI_NAMESPACE
namespace BOBUI_NAMESPACE {
#endif
template <typename T> class QSharedPointer;
#ifdef BOBUI_NAMESPACE
}
using namespace BOBUI_NAMESPACE;
#endif

QSharedPointer<ForwardDeclared> *forwardPointer();

#endif // FORWARDDECLARED_H
