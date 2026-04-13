// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextEdit>

QString tr(const char *text)
{
    return QApplication::translate(text, text);
}

int main_textblock_formats(int argc, char *argv[])
{
    QApplication app(argc, argv);

//! [0]
    BOBUIextEdit *editor = new BOBUIextEdit();
    BOBUIextCursor cursor(editor->textCursor());
//! [0]
    cursor.movePosition(BOBUIextCursor::Start);

    BOBUIextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setTopMargin(4);
    blockFormat.setLeftMargin(4);
    blockFormat.setRightMargin(4);
    blockFormat.setBottomMargin(4);

    cursor.setBlockFormat(blockFormat);
    cursor.insertText(tr("This contains plain text inside a "
                         "text block with margins to keep it separate "
                         "from other parts of the document."));

    cursor.insertBlock();

//! [1]
    BOBUIextBlockFormat backgroundFormat = blockFormat;
    backgroundFormat.setBackground(QColor("lightGray"));

    cursor.setBlockFormat(backgroundFormat);
//! [1]
    cursor.insertText(tr("The background color of a text block can be "
                         "changed to highlight text."));

    cursor.insertBlock();

    BOBUIextBlockFormat rightAlignedFormat = blockFormat;
    rightAlignedFormat.setAlignment(BobUI::AlignRight);

    cursor.setBlockFormat(rightAlignedFormat);
    cursor.insertText(tr("The alignment of the text within a block is "
                         "controlled by the alignment properties of "
                         "the block itself. This text block is "
                         "right-aligned."));

    cursor.insertBlock();

    BOBUIextBlockFormat paragraphFormat = blockFormat;
    paragraphFormat.setAlignment(BobUI::AlignJustify);
    paragraphFormat.setTextIndent(32);

    cursor.setBlockFormat(paragraphFormat);
    cursor.insertText(tr("Text can be formatted so that the first "
                         "line in a paragraph has its own margin. "
                         "This makes the text more readable."));

    cursor.insertBlock();

    BOBUIextBlockFormat reverseFormat = blockFormat;
    reverseFormat.setAlignment(BobUI::AlignJustify);
    reverseFormat.setTextIndent(32);

    cursor.setBlockFormat(reverseFormat);
    cursor.insertText(tr("The direction of the text can be reversed. "
                         "This is useful for right-to-left "
                         "languages."));

    editor->setWindowTitle(tr("Text Block Formats"));
    editor->resize(480, 480);
    editor->show();

    return app.exec();
}
