// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>

class tst_QGraphicsPixmapItem : public QObject
{
    Q_OBJECT

private slots:
    void qgraphicspixmapitem_data();
    void qgraphicspixmapitem();
    void boundingRect_data();
    void boundingRect();
    void contains_data();
    void contains();
    void isObscuredBy_data();
    void isObscuredBy();
    void offset_data();
    void offset();
    void opaqueArea_data();
    void opaqueArea();
    void pixmap_data();
    void pixmap();
    void setPixmap_data();
    void setPixmap();
    void setShapeMode_data();
    void setShapeMode();
    void setTransformationMode_data();
    void setTransformationMode();
    void shape_data();
    void shape();
    void extension_data();
    void extension();
    void setExtension_data();
    void setExtension();
    void supportsExtension_data();
    void supportsExtension();
};

// Subclass that exposes the protected functions.
class SubQGraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    enum Extension {
        UserExtension = QGraphicsItem::UserExtension
    };
    SubQGraphicsPixmapItem(QGraphicsItem *parent = nullptr) : QGraphicsPixmapItem(parent)
    {
    }

    SubQGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr) : QGraphicsPixmapItem(pixmap, parent)
    {
    }

    QVariant call_extension(QVariant const& variant) const
        { return SubQGraphicsPixmapItem::extension(variant); }

    void call_setExtension(Extension extension, QVariant const& variant)
        { return SubQGraphicsPixmapItem::setExtension((QGraphicsItem::Extension)extension, variant); }

    bool call_supportsExtension(Extension extension) const
        { return SubQGraphicsPixmapItem::supportsExtension((QGraphicsItem::Extension)extension); }
};

void tst_QGraphicsPixmapItem::qgraphicspixmapitem_data()
{
}

void tst_QGraphicsPixmapItem::qgraphicspixmapitem()
{
    SubQGraphicsPixmapItem item;
    item.boundingRect();
    item.contains(QPoint());
    item.isObscuredBy(0);
    item.opaqueArea();
    //item.paint();
    QCOMPARE(item.offset(), QPointF());
    QCOMPARE(item.pixmap(), QPixmap());
    QCOMPARE(item.shapeMode(), QGraphicsPixmapItem::MaskShape);
    QCOMPARE(item.transformationMode(), BobUI::FastTransformation);
    item.setOffset(0, 0);
    item.setOffset(QPointF(0, 0));
    item.setPixmap(QPixmap());
    item.setShapeMode(QGraphicsPixmapItem::MaskShape);
    item.setTransformationMode(BobUI::FastTransformation);
    item.shape();
    item.type();
    item.call_extension(QVariant());
    item.call_setExtension(SubQGraphicsPixmapItem::UserExtension, QVariant());
    item.call_supportsExtension(SubQGraphicsPixmapItem::UserExtension);
}

void tst_QGraphicsPixmapItem::boundingRect_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QRectF>("boundingRect");
    BOBUIest::newRow("null") << QPixmap() << QRectF();
    BOBUIest::newRow("10x10") << QPixmap(10, 10) << QRectF(0, 0, 10, 10);
}

// public QRectF boundingRect() const
void tst_QGraphicsPixmapItem::boundingRect()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QRectF, boundingRect);

    SubQGraphicsPixmapItem item(pixmap);
    QCOMPARE(item.boundingRect(), boundingRect);
}

void tst_QGraphicsPixmapItem::contains_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QPointF>("point");
    BOBUIest::addColumn<bool>("contains");
    BOBUIest::newRow("null") << QPixmap() << QPointF() << false;
    BOBUIest::newRow("10x10, 100x100") << QPixmap(10, 10) << QPointF(100, 100) << false;
    BOBUIest::newRow("10x10, 5x5") << QPixmap(10, 10) << QPointF(5, 5) << true;
    BOBUIest::newRow("border-1") << QPixmap(10, 10) << QPointF(10.5, 10.5) << false;
    BOBUIest::newRow("border-2") << QPixmap(10, 10) << QPointF(-0.5, -0.5) << false;
}

// public bool contains(QPointF const& point) const
void tst_QGraphicsPixmapItem::contains()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QPointF, point);
    QFETCH(bool, contains);

    // At the time of writing, by default pixmaps will have:
    //  - The same pixel format of the primary screen (which is platform dependent and may contain alpha)
    //  - Uninitialized pixels, potentially including an alpha channel
    //  - A ShapeMode of Mask (which mean it will use the alpha channel as a mask for contains())
    // This means that in order to prevent undefined behavior in this test, we either need to set
    // the shapeMode to something else, or set the pixels of the pixmap.
    pixmap.fill(); // Filling the pixmap to be on the safe side.

    SubQGraphicsPixmapItem item(pixmap);
    QCOMPARE(item.contains(point), contains);
}

void tst_QGraphicsPixmapItem::isObscuredBy_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QPixmap>("otherPixmap");
    BOBUIest::addColumn<bool>("isObscuredBy");
    BOBUIest::newRow("null") << QPixmap() << QPixmap() << false;
    BOBUIest::newRow("(10, 10) vs. (5, 5)") << QPixmap(10, 10) << QPixmap(5, 5) << false;
    BOBUIest::newRow("(5, 5) vs. (10, 10)") << QPixmap(5, 5) << QPixmap(10, 10) << true;
    BOBUIest::newRow("(10, 10) vs. (10, 10)") << QPixmap(10, 10) << QPixmap(10, 10) << false;
    BOBUIest::newRow("(9, 9) vs. (10, 10)") << QPixmap(8, 8) << QPixmap(10, 10) << true;
    BOBUIest::newRow("(10, 10) vs. (9, 9)") << QPixmap(10, 10) << QPixmap(8, 8) << false;
}

// public bool isObscuredBy(QGraphicsItem const* item) const
void tst_QGraphicsPixmapItem::isObscuredBy()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QPixmap, otherPixmap);
    QFETCH(bool, isObscuredBy);
    pixmap.fill();
    otherPixmap.fill();

    SubQGraphicsPixmapItem *item = new SubQGraphicsPixmapItem(pixmap);
    SubQGraphicsPixmapItem *otherItem = new SubQGraphicsPixmapItem(otherPixmap);

    item->setOffset(-pixmap.width() / 2.0, -pixmap.height() / 2.0);
    otherItem->setOffset(-otherPixmap.width() / 2.0, -otherPixmap.height() / 2.0);

    QGraphicsScene scene;
    scene.addItem(item);
    scene.addItem(otherItem);
    otherItem->setZValue(1);

    QCOMPARE(item->isObscuredBy(otherItem), isObscuredBy);
}

void tst_QGraphicsPixmapItem::offset_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QPointF>("offset");
    BOBUIest::newRow("null") << QPixmap() << QPointF();
    BOBUIest::newRow("10x10, 1x1") << QPixmap(10, 10) << QPointF(1, 1);
}

// public QPointF offset() const
void tst_QGraphicsPixmapItem::offset()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QPointF, offset);

    SubQGraphicsPixmapItem item(pixmap);
    item.setOffset(offset);
    QCOMPARE(item.offset(), offset);

    // ### test actual painting and compare pixmap with offseted one?
}

Q_DECLARE_METATYPE(QPainterPath)
void tst_QGraphicsPixmapItem::opaqueArea_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QPainterPath>("opaqueArea");
    BOBUIest::newRow("null") << QPixmap() << QPainterPath();
    // Currently QGraphicsPixmapItem just calls QGraphicsItem test there
}

// public QPainterPath opaqueArea() const
void tst_QGraphicsPixmapItem::opaqueArea()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QPainterPath, opaqueArea);

    SubQGraphicsPixmapItem item;
    QCOMPARE(item.opaqueArea(), opaqueArea);
}

void tst_QGraphicsPixmapItem::pixmap_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::newRow("null") << QPixmap();
    BOBUIest::newRow("10x10") << QPixmap(10, 10);
}

// public QPixmap pixmap() const
void tst_QGraphicsPixmapItem::pixmap()
{
    QFETCH(QPixmap, pixmap);

    SubQGraphicsPixmapItem item(pixmap);
    QCOMPARE(item.pixmap(), pixmap);
}

void tst_QGraphicsPixmapItem::setPixmap_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::newRow("null") << QPixmap();
    BOBUIest::newRow("10x10") << QPixmap(10, 10);
}

// public void setPixmap(QPixmap const& pixmap)
void tst_QGraphicsPixmapItem::setPixmap()
{
    QFETCH(QPixmap, pixmap);

    SubQGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    QCOMPARE(item.pixmap(), pixmap);
}

Q_DECLARE_METATYPE(QGraphicsPixmapItem::ShapeMode)
void tst_QGraphicsPixmapItem::setShapeMode_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QGraphicsPixmapItem::ShapeMode>("mode");
    BOBUIest::newRow("MaskShape") << QPixmap() << QGraphicsPixmapItem::MaskShape;
    BOBUIest::newRow("BoundingRectShape") << QPixmap() << QGraphicsPixmapItem::BoundingRectShape;
    BOBUIest::newRow("HeuristicMaskShape") << QPixmap() << QGraphicsPixmapItem::HeuristicMaskShape;
}

// public void setShapeMode(QGraphicsPixmapItem::ShapeMode mode)
void tst_QGraphicsPixmapItem::setShapeMode()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QGraphicsPixmapItem::ShapeMode, mode);

    SubQGraphicsPixmapItem item(pixmap);
    item.setShapeMode(mode);
    QCOMPARE(item.shapeMode(), mode);
}

Q_DECLARE_METATYPE(BobUI::TransformationMode)
void tst_QGraphicsPixmapItem::setTransformationMode_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<BobUI::TransformationMode>("mode");
    BOBUIest::newRow("FastTransformation") << QPixmap() << BobUI::FastTransformation;
    BOBUIest::newRow("SmoothTransformation") << QPixmap() << BobUI::SmoothTransformation;
}

// public void setTransformationMode(BobUI::TransformationMode mode)
void tst_QGraphicsPixmapItem::setTransformationMode()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(BobUI::TransformationMode, mode);

    SubQGraphicsPixmapItem item(pixmap);
    item.setTransformationMode(mode);
    QCOMPARE(item.transformationMode(), mode);
}

void tst_QGraphicsPixmapItem::shape_data()
{
    BOBUIest::addColumn<QPixmap>("pixmap");
    BOBUIest::addColumn<QPainterPath>("shape");
    BOBUIest::newRow("null") << QPixmap() << QPainterPath();
    // ### what does a normal shape look like?
}

// public QPainterPath shape() const
void tst_QGraphicsPixmapItem::shape()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QPainterPath, shape);

    SubQGraphicsPixmapItem item(pixmap);
    QCOMPARE(item.shape(), shape);
}

Q_DECLARE_METATYPE(SubQGraphicsPixmapItem::Extension)
void tst_QGraphicsPixmapItem::extension_data()
{
    BOBUIest::addColumn<QVariant>("variant");
    BOBUIest::addColumn<QVariant>("extension");
    BOBUIest::newRow("null") << QVariant() << QVariant();
}

// protected QVariant extension(QVariant const& variant) const
void tst_QGraphicsPixmapItem::extension()
{
    QFETCH(QVariant, variant);
    QFETCH(QVariant, extension);

    SubQGraphicsPixmapItem item;
    QCOMPARE(item.call_extension(variant), extension);
}

void tst_QGraphicsPixmapItem::setExtension_data()
{
    BOBUIest::addColumn<SubQGraphicsPixmapItem::Extension>("extension");
    BOBUIest::addColumn<QVariant>("variant");
    BOBUIest::newRow("null") << SubQGraphicsPixmapItem::UserExtension << QVariant();
}

// protected void setExtension(QGraphicsItem::Extension extension, QVariant const& variant)
void tst_QGraphicsPixmapItem::setExtension()
{
    QFETCH(SubQGraphicsPixmapItem::Extension, extension);
    QFETCH(QVariant, variant);

    SubQGraphicsPixmapItem item;
    item.call_setExtension(extension, variant);
}

void tst_QGraphicsPixmapItem::supportsExtension_data()
{
    BOBUIest::addColumn<SubQGraphicsPixmapItem::Extension>("extension");
    BOBUIest::addColumn<bool>("supportsExtension");
    BOBUIest::newRow("null") << SubQGraphicsPixmapItem::UserExtension << false;
}

// protected bool supportsExtension(QGraphicsItem::Extension extension) const
void tst_QGraphicsPixmapItem::supportsExtension()
{
    QFETCH(SubQGraphicsPixmapItem::Extension, extension);
    QFETCH(bool, supportsExtension);

    SubQGraphicsPixmapItem item;
    QCOMPARE(item.call_supportsExtension(extension), supportsExtension);
}

BOBUIEST_MAIN(tst_QGraphicsPixmapItem)
#include "tst_qgraphicspixmapitem.moc"

