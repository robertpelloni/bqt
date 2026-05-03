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

    BOBUIextDocument *document = new BOBUIextDocument(editor);
    BOBUIextCursor cursor(document);

    QImage image(64, 64, QImage::Format_RGB32);
    image.fill(qRgb(255, 160, 128));

//! [Adding a resource]
    document->addResource(BOBUIextDocument::ImageResource,
        QUrl("mydata://image.png"), QVariant(image));
//! [Adding a resource]

//! [Inserting an image with a cursor]
    BOBUIextImageFormat imageFormat;
    imageFormat.setName("mydata://image.png");
    cursor.insertImage(imageFormat);
//! [Inserting an image with a cursor]

    cursor.insertBlock();
    cursor.insertText("Code less. Create more.");

    editor->setDocument(document);
    editor->setWindowTitle(tr("Text Document Images"));
    editor->resize(320, 480);
    editor->show();

//! [Inserting an image using HTML]
    editor->append("<img src=\"mydata://image.png\" />");
//! [Inserting an image using HTML]

    return app.exec();
}
