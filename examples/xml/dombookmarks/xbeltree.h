// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef XBELTREE_H
#define XBELTREE_H

#include <QDomDocument>
#include <QIcon>
#include <BOBUIreeWidget>

//! [0]
class XbelTree : public BOBUIreeWidget
{
    Q_OBJECT

public:
    explicit XbelTree(QWidget *parent = nullptr);

    bool read(QIODevice *device);
    bool write(QIODevice *device) const;

protected:
#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(contextmenu)
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif

private slots:
    void updateDomElement(const BOBUIreeWidgetItem *item, int column);

private:
    void parseFolderElement(const QDomElement &element,
                            BOBUIreeWidgetItem *parentItem = nullptr);
    BOBUIreeWidgetItem *createItem(const QDomElement &element,
                                BOBUIreeWidgetItem *parentItem = nullptr);

    QDomDocument domDocument;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};
//! [0]

#endif
