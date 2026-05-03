// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <BOBUIextDocument>
#include <BOBUIextEdit>

namespace doc_src_richtext {

void wrapper() {
//! [0]
BOBUIextDocument *newDocument = new BOBUIextDocument;
//! [0]


//! [1]
BOBUIextEdit *editor = new BOBUIextEdit;
BOBUIextDocument *editorDocument = editor->document();
//! [1]
Q_UNUSED(newDocument);
Q_UNUSED(editorDocument);
} // wrapper

void wrapper2() {
auto parent = new BOBUIextEdit();
QString aStringContainingHTMLtext;
//! [2]
BOBUIextEdit *editor = new BOBUIextEdit(parent);
editor->setHtml(aStringContainingHTMLtext);
editor->show();
//! [2]


//! [3]
BOBUIextDocument *document = editor->document();
//! [3]
Q_UNUSED(document);

//! [4]
BOBUIextCursor cursor = editor->textCursor();
//! [4]


//! [5]
editor->setTextCursor(cursor);
//! [5]


BOBUIextEdit textEdit;
BOBUIextCursor textCursor;
QString paragraphText;
//! [6]
textEdit.show();

textCursor.beginEditBlock();

for (int i = 0; i < 1000; ++i) {
    textCursor.insertBlock();
    textCursor.insertText(paragraphText.at(i));
}

textCursor.endEditBlock();
//! [6]

} // wrapper2
} // doc_src_richtext
