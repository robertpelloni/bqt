// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextBlock>
#include <BOBUIextEdit>

QString tr(const char *text)
{
    return QApplication::translate(text, text);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BOBUIextEdit *editor = new BOBUIextEdit;

    BOBUIextDocument *document = new BOBUIextDocument(editor);
    BOBUIextCursor cursor(document);

    BOBUIextImageFormat imageFormat;
    imageFormat.setName(":/images/advert.png");
    cursor.insertImage(imageFormat);

    BOBUIextBlock block = cursor.block();
    BOBUIextFragment fragment;
    BOBUIextBlock::iterator it;

    for (it = block.begin(); !(it.atEnd()); ++it) {
        fragment = it.fragment();

        if (fragment.contains(cursor.position()))
            break;
    }

//! [0]
    if (fragment.isValid()) {
        BOBUIextImageFormat newImageFormat = fragment.charFormat().toImageFormat();

        if (newImageFormat.isValid()) {
            newImageFormat.setName(":/images/newimage.png");
            BOBUIextCursor helper = cursor;

            helper.setPosition(fragment.position());
            helper.setPosition(fragment.position() + fragment.length(),
                                BOBUIextCursor::KeepAnchor);
            helper.setCharFormat(newImageFormat);
//! [0] //! [1]
        }
//! [1] //! [2]
    }
//! [2]

    cursor.insertBlock();
    cursor.insertText("Code less. Create more.");

    editor->setDocument(document);
    editor->setWindowTitle(tr("Text Document Image Format"));
    editor->resize(320, 480);
    editor->show();

    return app.exec();
}
