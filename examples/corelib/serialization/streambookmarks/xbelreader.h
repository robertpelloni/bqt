// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef XBELREADER_H
#define XBELREADER_H

#include <QIcon>
#include <QXmlStreamReader>

BOBUI_BEGIN_NAMESPACE
class BOBUIreeWidget;
class BOBUIreeWidgetItem;
BOBUI_END_NAMESPACE

//! [0]
class XbelReader
{
public:
//! [1]
    XbelReader(BOBUIreeWidget *treeWidget);
//! [1]

    bool read(QIODevice *device);
    QString errorString() const;

private:
//! [2]
    void readXBEL();
    void readTitle(BOBUIreeWidgetItem *item);
    void readSeparator(BOBUIreeWidgetItem *item);
    void readFolder(BOBUIreeWidgetItem *item);
    void readBookmark(BOBUIreeWidgetItem *item);

    BOBUIreeWidgetItem *createChildItem(BOBUIreeWidgetItem *item);

    QXmlStreamReader xml;
    BOBUIreeWidget *treeWidget;
//! [2]

    QIcon folderIcon;
    QIcon bookmarkIcon;
};
//! [0]

#endif
