// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef NONGADGETPARENTCLASS_H
#define NONGADGETPARENTCLASS_H

#include <BobUICore/qobjectdefs.h>

namespace NonGadgetParent {

struct Base {};
struct Derived : Base { Q_GADGET };

}

#endif // NONGADGETPARENTCLASS_H

