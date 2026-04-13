// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "xbeltree.h"

#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>

#include <QDesktopServices>
#include <QGuiApplication>
#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(contextmenu)
#  include <QClipboard>
#  include <QContextMenuEvent>
#endif

#include <BOBUIextStream>
#include <QUrl>

using namespace BobUI::StringLiterals;

enum { DomElementRole = BobUI::UserRole + 1 };

Q_DECLARE_METATYPE(QDomElement)

static const auto titleElement  = u"title"_s;
static const auto folderElement = u"folder"_s;
static const auto bookmarkElement = u"bookmark"_s;

static const auto versionAttribute = u"version"_s;
static const auto hrefAttribute = u"href"_s;
static const auto foldedAttribute = u"folded"_s;

//! [0]
XbelTree::XbelTree(QWidget *parent)
    : BOBUIreeWidget(parent)
{
    header()->setSectionResizeMode(QHeaderView::Stretch);
    setHeaderLabels({tr("Title"), tr("Location")});

    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
}
//! [0]

#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(contextmenu)
void XbelTree::contextMenuEvent(QContextMenuEvent *event)
{
    const BOBUIreeWidgetItem *item = itemAt(event->pos());
    if (!item)
        return;
    const QString url = item->text(1);
    QMenu contextMenu;
    QAction *copyAction = contextMenu.addAction(tr("Copy Link to Clipboard"));
    QAction *openAction = contextMenu.addAction(tr("Open"));
    QAction *action = contextMenu.exec(event->globalPos());
    if (action == copyAction)
        QGuiApplication::clipboard()->setText(url);
    else if (action == openAction)
        QDesktopServices::openUrl(QUrl(url));
}
#endif // BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(contextmenu)

//! [1]
bool XbelTree::read(QIODevice *device)
{
    QDomDocument::ParseResult result =
            domDocument.setContent(device, QDomDocument::ParseOption::UseNamespaceProcessing);
    if (!result) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                         .arg(result.errorLine)
                                         .arg(result.errorColumn)
                                         .arg(result.errorMessage));
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL file."));
        return false;
    } else if (root.hasAttribute(versionAttribute)
               && root.attribute(versionAttribute) != "1.0"_L1) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL version 1.0 "
                                    "file."));
        return false;
    }

    clear();

    disconnect(this, &BOBUIreeWidget::itemChanged, this, &XbelTree::updateDomElement);

    QDomElement child = root.firstChildElement(folderElement);
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement(folderElement);
    }

    connect(this, &BOBUIreeWidget::itemChanged, this, &XbelTree::updateDomElement);

    return true;
}
//! [1]

//! [2]
bool XbelTree::write(QIODevice *device) const
{
    const int IndentSize = 4;

    BOBUIextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}
//! [2]

void XbelTree::updateDomElement(const BOBUIreeWidgetItem *item, int column)
{
    QDomElement element = qvariant_cast<QDomElement>(item->data(0, DomElementRole));
    if (!element.isNull()) {
        if (column == 0) {
            QDomElement oldTitleElement = element.firstChildElement(titleElement);
            QDomElement newTitleElement = domDocument.createElement(titleElement);

            QDomText newTitleText = domDocument.createTextNode(item->text(0));
            newTitleElement.appendChild(newTitleText);

            element.replaceChild(newTitleElement, oldTitleElement);
        } else {
            if (element.tagName() == bookmarkElement)
                element.setAttribute(hrefAttribute, item->text(1));
        }
    }
}

//! [3]
void XbelTree::parseFolderElement(const QDomElement &element,
                                  BOBUIreeWidgetItem *parentItem)
{
    BOBUIreeWidgetItem *item = createItem(element, parentItem);

    QString title = element.firstChildElement(titleElement).text();
    if (title.isEmpty())
        title = tr("Folder");

    item->setFlags(item->flags() | BobUI::ItemIsEditable);
    item->setIcon(0, folderIcon);
    item->setText(0, title);

    bool folded = (element.attribute(foldedAttribute) != "no"_L1);
    item->setExpanded(!folded);

    constexpr char16_t midDot = u'\xB7';
    static const QString dots = QString(30, midDot);
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == folderElement) {
            parseFolderElement(child, item);
        } else if (child.tagName() == bookmarkElement) {
            BOBUIreeWidgetItem *childItem = createItem(child, item);

            QString title = child.firstChildElement(titleElement).text();
            if (title.isEmpty())
                title = tr("Folder");

            childItem->setFlags(item->flags() | BobUI::ItemIsEditable);
            childItem->setIcon(0, bookmarkIcon);
            childItem->setText(0, title);
            childItem->setText(1, child.attribute(hrefAttribute));
        } else if (child.tagName() == "separator"_L1) {
            BOBUIreeWidgetItem *childItem = createItem(child, item);
            childItem->setFlags(item->flags() & ~(BobUI::ItemIsSelectable | BobUI::ItemIsEditable));
            childItem->setText(0, dots);
        }
        child = child.nextSiblingElement();
    }
}
//! [3]

BOBUIreeWidgetItem *XbelTree::createItem(const QDomElement &element,
                                      BOBUIreeWidgetItem *parentItem)
{
    BOBUIreeWidgetItem *item;
    if (parentItem) {
        item = new BOBUIreeWidgetItem(parentItem);
    } else {
        item = new BOBUIreeWidgetItem(this);
    }
    item->setData(0, DomElementRole, QVariant::fromValue(element));
    return item;
}
