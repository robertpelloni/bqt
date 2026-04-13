// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QPainter>

namespace src_gui_painting_qbrush {

struct Wrapper : public QPaintDevice {
    void wrapper();
};
void Wrapper::wrapper() {


//! [0]
QPainter painter(this);

painter.setBrush(BobUI::cyan);
painter.setPen(BobUI::darkCyan);
painter.drawRect(0, 0, 100,100);

painter.setBrush(BobUI::NoBrush);
painter.setPen(BobUI::darkGreen);
painter.drawRect(40, 40, 100, 100);
//! [0]


} // Wrapper::wrapper
} // src_gui_painting_qbrush
