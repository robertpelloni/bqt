// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QImage>
#include <BOBUIextCursor>
#include <BOBUIextDocument>

namespace src_gui_text_bobuiextcursor {
BOBUIextDocument *textDocument = nullptr;

void wrapper0() {
BOBUIextCursor cursor;


//! [0]
cursor.clearSelection();
cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor);
cursor.insertText("Hello World");
//! [0]


//! [1]
QImage img;
textDocument->addResource(BOBUIextDocument::ImageResource, QUrl("myimage"), img);
cursor.insertImage("myimage");
//! [1]

} // wrapper0


void wrapper1() {
//! [2]
BOBUIextCursor cursor(textDocument);
cursor.beginEditBlock();
cursor.insertText("Hello");
cursor.insertText("World");
cursor.endEditBlock();

textDocument->undo();
//! [2]
} // wrapper1


void wrapper2() {
//! [3]
BOBUIextCursor cursor(textDocument);
cursor.beginEditBlock();
cursor.insertText("Hello");
cursor.insertText("World");
cursor.endEditBlock();

// ...

cursor.joinPreviousEditBlock();
cursor.insertText("Hey");
cursor.endEditBlock();

textDocument->undo();
//! [3]
} // wrapper2

} // src_gui_text_bobuiextcursor
