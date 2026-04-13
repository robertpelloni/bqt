// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QGraphicsLayout>

class FlowLayout : public QGraphicsLayout
{
public:
    FlowLayout(QGraphicsLayoutItem *parent = nullptr);
    inline void addItem(QGraphicsLayoutItem *item);
    void insertItem(int index, QGraphicsLayoutItem *item);
    void setSpacing(BobUI::Orientations o, qreal spacing);
    qreal spacing(BobUI::Orientation o) const;

    // inherited functions
    void setGeometry(const QRectF &geom) override;

    int count() const override;
    QGraphicsLayoutItem *itemAt(int index) const override;
    void removeAt(int index) override;

protected:
    QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint = QSizeF()) const override;

private:
    qreal doLayout(const QRectF &geom, bool applyNewGeometry) const;
    QSizeF minSize(const QSizeF &constraint) const;
    QSizeF prefSize() const;
    QSizeF maxSize() const;

    QList<QGraphicsLayoutItem *> m_items;
    qreal m_spacing[2] = {6, 6};
};


inline void FlowLayout::addItem(QGraphicsLayoutItem *item)
{
    insertItem(-1, item);
}

#endif // FLOWLAYOUT_H
