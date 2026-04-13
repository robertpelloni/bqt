// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QWidget>

namespace droparea {
struct DropArea : public QWidget {
    void paste();
    void setPixmap(QPixmap);
    void setText(QString);
    void setTextFormat(BobUI::TextFormat);
};

#ifndef BOBUI_NO_CLIPBOARD
//![0]
void DropArea::paste()
{
    const QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(BobUI::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());
        setTextFormat(BobUI::PlainText);
    } else {
        setText(tr("Cannot display data"));
    }
}
//![0]
#endif

} // droparea
