// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextEdit>

QString tr(const char *text)
{
    return QApplication::translate(text, text);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BOBUIextEdit *editor = new BOBUIextEdit();

    BOBUIextCursor cursor(editor->textCursor());
    cursor.movePosition(BOBUIextCursor::Start);

    BOBUIextCharFormat plainFormat(cursor.charFormat());
    BOBUIextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(BobUI::red);

    cursor.insertText(tr("Text can be displayed in a variety of "
                                  "different character "
                                  "formats. "), plainFormat);
    cursor.insertText(tr("We can emphasize text by making it "));
    cursor.insertText(tr("italic, give it a different color "));
    cursor.insertText(tr("to the default text color, underline it, "));
    cursor.insertText(tr("and use many other effects."));

    QString searchString = tr("text");

    BOBUIextDocument *document = editor->document();
//! [0]
    BOBUIextCursor newCursor(document);

    while (!newCursor.isNull() && !newCursor.atEnd()) {
        newCursor = document->find(searchString, newCursor);

        if (!newCursor.isNull()) {
            newCursor.movePosition(BOBUIextCursor::WordRight,
                                   BOBUIextCursor::KeepAnchor);

            newCursor.mergeCharFormat(colorFormat);
        }
//! [0] //! [1]
    }
//! [1]

    editor->setWindowTitle(tr("Text Document Find"));
    editor->resize(320, 480);
    editor->show();
    return app.exec();
}
