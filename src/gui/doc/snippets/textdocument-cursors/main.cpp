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
    BOBUIextEdit *editor = new BOBUIextEdit;

//! [0]
    BOBUIextDocument *document = editor->document();
    BOBUIextCursor redCursor(document);
//! [0] //! [1]
    BOBUIextCursor blueCursor(document);
//! [1]

    BOBUIextCharFormat redFormat(redCursor.charFormat());
    redFormat.setForeground(BobUI::red);
    BOBUIextCharFormat blueFormat(blueCursor.charFormat());
    blueFormat.setForeground(BobUI::blue);

    redCursor.setCharFormat(redFormat);
    blueCursor.setCharFormat(blueFormat);

    for (int i = 0; i < 20; ++i) {
        if (i % 2 == 0)
            redCursor.insertText(tr("%1 ").arg(i), redFormat);
        if (i % 5 == 0)
            blueCursor.insertText(tr("%1 ").arg(i), blueFormat);
    }

    editor->setWindowTitle(tr("Text Document Cursors"));
    editor->resize(320, 480);
    editor->show();
    return app.exec();
}
