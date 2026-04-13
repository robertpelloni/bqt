// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QLinearGradient>

namespace brush {
void wrapper() {


//! [0]
QLinearGradient linearGrad(QPointF(100, 100), QPointF(200, 200));
linearGrad.setColorAt(0, BobUI::black);
linearGrad.setColorAt(1, BobUI::white);
//! [0]


//! [1]
QRadialGradient radialGrad(QPointF(100, 100), 100);
radialGrad.setColorAt(0, BobUI::red);
radialGrad.setColorAt(0.5, BobUI::blue);
radialGrad.setColorAt(1, BobUI::green);
//! [1]

} // wrapper
} // brush
