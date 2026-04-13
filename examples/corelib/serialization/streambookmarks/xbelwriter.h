// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef XBELWRITER_H
#define XBELWRITER_H

#include <QXmlStreamWriter>

BOBUI_BEGIN_NAMESPACE
class BOBUIreeWidget;
class BOBUIreeWidgetItem;
BOBUI_END_NAMESPACE

//! [0]
class XbelWriter
{
public:
    explicit XbelWriter(const BOBUIreeWidget *treeWidget);
    bool writeFile(QIODevice *device);

private:
    void writeItem(const BOBUIreeWidgetItem *item);
    QXmlStreamWriter xml;
    const BOBUIreeWidget *treeWidget;
};
//! [0]

#endif
