// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef VIEW_H
#define VIEW_H

#include <BobUIWidgets>
#include <BobUISql>

class ImageItem;
class InformationWindow;

//! [0]
class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(const QString &items, const QString &images, QWidget *parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
//! [0]

//! [1]
private Q_SLOTS:
    void updateImage(int id, const QString &fileName);
//! [1]

//! [2]
private:
    void addItems();
    InformationWindow *findWindow(int id) const;
    void showInformation(ImageItem *image);

    QGraphicsScene *scene;
    QList<InformationWindow *> informationWindows;
//! [2] //! [3]
    QSqlRelationalTableModel *itemTable;
};
//! [3]

#endif
