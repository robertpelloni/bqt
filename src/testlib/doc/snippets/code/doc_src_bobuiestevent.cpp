// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIest>
#include <QLineEdit>

#include "doc_src_bobuiestevent.h"

void TestGui::testGui_data()
{
QWidget *myParent = nullptr;
//! [0]
BOBUIestEventList events;
events.addKeyClick('a');
events.addKeyClick(BobUI::Key_Backspace);
events.addDelay(200);
QLineEdit *lineEdit = new QLineEdit(myParent);
// ...
events.simulate(lineEdit);
events.simulate(lineEdit);
//! [0]
}
