// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BOBUIextEdit>

class TextEdit : public BOBUIextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);

    bool canInsertFromMimeData(const QMimeData *source) const override;
    void insertFromMimeData(const QMimeData *source) override;
};

TextEdit::TextEdit(QWidget *parent)
    : BOBUIextEdit(parent)
{
}

//! [0]
bool TextEdit::canInsertFromMimeData( const QMimeData *source ) const
{
    if (source->hasImage())
        return true;
    else
        return BOBUIextEdit::canInsertFromMimeData(source);
}
//! [0]

//! [1]
void TextEdit::insertFromMimeData( const QMimeData *source )
{
    if (source->hasImage())
    {
        QImage image = qvariant_cast<QImage>(source->imageData());
        BOBUIextCursor cursor = this->textCursor();
        BOBUIextDocument *document = this->document();
        document->addResource(BOBUIextDocument::ImageResource, QUrl("image"), image);
        cursor.insertImage("image");
    }
}
//! [1]
