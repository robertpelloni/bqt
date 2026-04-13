// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qgraphicslayoutitem.h>
#include <float.h>
#include <limits.h>

class tst_QGraphicsLayoutItem : public QObject {
Q_OBJECT

private slots:
    void qgraphicslayoutitem();

    void contentsRect();
    void effectiveSizeHint_data();
    void effectiveSizeHint();
    void getContentsMargins();
    void isLayout_data();
    void isLayout();
    void maximumSize();
    void minimumSize();
    void parentLayoutItem_data();
    void parentLayoutItem();
    void preferredSize();
    void setMaximumSize_data();
    void setMaximumSize();
    void setMinimumSize_data();
    void setMinimumSize();
    void setPreferredSize_data();
    void setPreferredSize();
    void setSizePolicy_data();
    void setPreferredSize2();
    void setSizePolicy();
};

// Subclass that exposes the protected functions.
class SubQGraphicsLayoutItem : public QGraphicsLayoutItem {
public:
    SubQGraphicsLayoutItem(QGraphicsLayoutItem *par = 0, bool layout = false)
            : QGraphicsLayoutItem(par, layout), updateGeometryCalled(0)
        {}

    // QGraphicsLayoutItem::geometry is a pure virtual function
    QRectF geometry() const
        { return QRectF(); }

    // QGraphicsLayoutItem::setGeometry is a pure virtual function
    void setGeometry(QRectF const&) override {}

    // QGraphicsLayoutItem::sizeHint is a pure virtual function
    QSizeF sizeHint(BobUI::SizeHint, QSizeF const& = QSizeF()) const override
        { return QSizeF(); }

    void updateGeometry() override
        { updateGeometryCalled++; QGraphicsLayoutItem::updateGeometry(); }
    int updateGeometryCalled;

};

void tst_QGraphicsLayoutItem::qgraphicslayoutitem()
{
    SubQGraphicsLayoutItem layoutItem;
    layoutItem.contentsRect();
    layoutItem.effectiveSizeHint(BobUI::MinimumSize);
    layoutItem.geometry();
    QCOMPARE(layoutItem.isLayout(), false);
    layoutItem.maximumSize();
    layoutItem.minimumSize();
    QCOMPARE(layoutItem.parentLayoutItem(), nullptr);
    layoutItem.preferredSize();
    layoutItem.sizePolicy();
    layoutItem.sizeHint(BobUI::MinimumSize);
}

// QRectF contentsRect() const public
void tst_QGraphicsLayoutItem::contentsRect()
{
    SubQGraphicsLayoutItem layoutItem;
    QRectF f = layoutItem.contentsRect();
    QCOMPARE(f, QRectF(QPoint(), QSizeF(0, 0)));
}
Q_DECLARE_METATYPE(BobUI::SizeHint)
void tst_QGraphicsLayoutItem::effectiveSizeHint_data()
{
    BOBUIest::addColumn<BobUI::SizeHint>("sizeHint");
    BOBUIest::addColumn<QSizeF>("constraint");
    for (int i = 0; i < 15; ++i) {
        BOBUIestData &data = BOBUIest::newRow(QByteArray::number(i).constData());
        switch(i % 5) {
        case 0: data << BobUI::MinimumSize; break;
        case 1: data << BobUI::PreferredSize; break;
        case 2: data << BobUI::MaximumSize; break;
        case 3: data << BobUI::MinimumDescent; break;
        case 4: data << BobUI::NSizeHints; break;
        }
        switch(i % 3) {
        case 0: data << QSizeF(-1, -1); break;
        case 1: data << QSizeF(0, 0); break;
        case 2: data << QSizeF(10, 10); break;
        }
    }
}

// QSizeF effectiveSizeHint(BobUI::SizeHint which, QSizeF const& constraint = QSize()) const public
void tst_QGraphicsLayoutItem::effectiveSizeHint()
{
    QFETCH(BobUI::SizeHint, sizeHint);
    QFETCH(QSizeF, constraint);
    SubQGraphicsLayoutItem layoutItem;
    QSizeF r = layoutItem.effectiveSizeHint(sizeHint, constraint);
    if (constraint.width() != -1)
        QCOMPARE(r.width(), constraint.width());
    if (constraint.height() != -1)
        QCOMPARE(r.height(), constraint.height());
}

// void getContentsMargins(qreal* left, qreal* top, qreal* right, qreal* bottom) const public
void tst_QGraphicsLayoutItem::getContentsMargins()
{
    SubQGraphicsLayoutItem layoutItem;
    qreal left;
    qreal top;
    qreal right;
    qreal bottom;
    layoutItem.getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, (qreal)0);
    QCOMPARE(top, (qreal)0);
    QCOMPARE(right, (qreal)0);
    QCOMPARE(bottom, (qreal)0);
}

void tst_QGraphicsLayoutItem::isLayout_data()
{
    BOBUIest::addColumn<bool>("isLayout");
    BOBUIest::newRow("no") << false;
    BOBUIest::newRow("yes") << true;
}

// bool isLayout() const public
void tst_QGraphicsLayoutItem::isLayout()
{
    QFETCH(bool, isLayout);
    SubQGraphicsLayoutItem layoutItem(0, isLayout);
    QCOMPARE(layoutItem.isLayout(), isLayout);
}

// QSizeF maximumSize() const public
void tst_QGraphicsLayoutItem::maximumSize()
{
    SubQGraphicsLayoutItem layoutItem;
    QCOMPARE(layoutItem.maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    // layoutItem.effectiveSizeHint(BobUI::MaximumSize);
}

// QSizeF minimumSize() const public
void tst_QGraphicsLayoutItem::minimumSize()
{
    SubQGraphicsLayoutItem layoutItem;
    QCOMPARE(layoutItem.minimumSize(), QSizeF(0, 0));
    // layoutItem.effectiveSizeHint(BobUI::MinimumSize);
}

void tst_QGraphicsLayoutItem::parentLayoutItem_data()
{
    BOBUIest::addColumn<bool>("parent");
    BOBUIest::newRow("no") << false;
    BOBUIest::newRow("yes") << true;
}

// QGraphicsLayoutItem* parentLayoutItem() const public
void tst_QGraphicsLayoutItem::parentLayoutItem()
{
    QFETCH(bool, parent);
    SubQGraphicsLayoutItem parentLayout;
    SubQGraphicsLayoutItem layoutItem(parent ? &parentLayout : 0);
    QCOMPARE(layoutItem.parentLayoutItem(), static_cast<QGraphicsLayoutItem*>( parent ? &parentLayout : 0));
}

// QSizeF preferredSize() const public
void tst_QGraphicsLayoutItem::preferredSize()
{
    SubQGraphicsLayoutItem layoutItem;
    QCOMPARE(layoutItem.preferredSize(), QSizeF(0, 0));
    // layoutItem.effectiveSizeHint(BobUI::PreferredSize));
}

void tst_QGraphicsLayoutItem::setMaximumSize_data()
{
    BOBUIest::addColumn<QSizeF>("size");
    BOBUIest::addColumn<QSizeF>("outputSize");
    BOBUIest::newRow("-1") << QSizeF(-1, -1) << QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    BOBUIest::newRow("0") << QSizeF(0, 0) << QSizeF(0, 0);
    BOBUIest::newRow("10") << QSizeF(10, 10) << QSizeF(10, 10);
}

// void setMaximumSize(QSizeF const& size) public
void tst_QGraphicsLayoutItem::setMaximumSize()
{
    QFETCH(QSizeF, size);
    QFETCH(QSizeF, outputSize);
    SubQGraphicsLayoutItem layoutItem;
    QSizeF oldSize = layoutItem.maximumSize();
    layoutItem.setMaximumSize(size);
    if (size.isValid())
        QCOMPARE(layoutItem.updateGeometryCalled, (oldSize == size) ? 0 : 1);
    else
        QVERIFY(!layoutItem.updateGeometryCalled);
    layoutItem.setMinimumSize(1, 1);

    QVERIFY(layoutItem.maximumSize().width() <= outputSize.width());
    QVERIFY(layoutItem.maximumSize().height() <= outputSize.height());
    QVERIFY(layoutItem.minimumSize().width() <= outputSize.width());
    QVERIFY(layoutItem.minimumSize().height() <= outputSize.height());
    QVERIFY(layoutItem.preferredSize().width() <= outputSize.width());
    QVERIFY(layoutItem.preferredSize().height() <= outputSize.height());
}

void tst_QGraphicsLayoutItem::setMinimumSize_data()
{
    BOBUIest::addColumn<QSizeF>("size");
    BOBUIest::newRow("-1") << QSizeF(-1, -1);
    BOBUIest::newRow("0") << QSizeF(0, 0);
    BOBUIest::newRow("10") << QSizeF(10, 10);
}

// void setMinimumSize(QSizeF const& size) public
void tst_QGraphicsLayoutItem::setMinimumSize()
{
    QFETCH(QSizeF, size);
    SubQGraphicsLayoutItem layoutItem;
    QSizeF oldSize = layoutItem.minimumSize();

    layoutItem.setMinimumSize(size);
    if (size.isValid()) {
        QEXPECT_FAIL("0", "updateGeometry() is called when it doesn't have to be.", Continue);
        QCOMPARE(layoutItem.updateGeometryCalled, (oldSize == size) ? 0 : 1);
    } else {
        QVERIFY(!layoutItem.updateGeometryCalled);
    }
    layoutItem.setMaximumSize(5, 5);
    QEXPECT_FAIL("10", "layoutItem.maximumSize().width() < size.width()", Abort);
    QVERIFY(layoutItem.maximumSize().width() >= size.width());
    QVERIFY(layoutItem.maximumSize().height() >= size.height());
    QVERIFY(layoutItem.minimumSize().width() >= size.width());
    QVERIFY(layoutItem.minimumSize().height() >= size.height());
    QVERIFY(layoutItem.preferredSize().width() >= size.width());
    QVERIFY(layoutItem.preferredSize().height() >= size.height());
}

void tst_QGraphicsLayoutItem::setPreferredSize_data()
{
    BOBUIest::addColumn<QSizeF>("size");
    BOBUIest::newRow("-1") << QSizeF(-1, -1);
    BOBUIest::newRow("0") << QSizeF(0, 0);
    BOBUIest::newRow("10") << QSizeF(10, 10);
}

// void setPreferredSize(QSizeF const& size) public
void tst_QGraphicsLayoutItem::setPreferredSize()
{
    QFETCH(QSizeF, size);
    SubQGraphicsLayoutItem layoutItem;
    QSizeF oldSize = layoutItem.preferredSize();
    layoutItem.setPreferredSize(size);
    if (size.isValid())
       QCOMPARE(layoutItem.preferredSize(), size);

    if (size.isValid()) {
        QEXPECT_FAIL("0", "updateGeometry() is called when it doesn't have to be.", Continue);
        QCOMPARE(layoutItem.updateGeometryCalled, (oldSize == size) ? 0 : 1);
    } else {
        QVERIFY(!layoutItem.updateGeometryCalled);
    }
}

void tst_QGraphicsLayoutItem::setPreferredSize2()
{
    SubQGraphicsLayoutItem layoutItem;
    layoutItem.setPreferredSize(QSizeF(30, -1));
    QCOMPARE(layoutItem.preferredWidth(), qreal(30));
}

void tst_QGraphicsLayoutItem::setSizePolicy_data()
{
    BOBUIest::addColumn<QSizePolicy>("policy");
    BOBUIest::newRow("default") << QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::DefaultType);
    BOBUIest::newRow("rand") << QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
}

// void setSizePolicy(QSizePolicy const& policy) public
void tst_QGraphicsLayoutItem::setSizePolicy()
{
    QFETCH(QSizePolicy, policy);
    SubQGraphicsLayoutItem layoutItem;
    QSizePolicy defaultPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::DefaultType);
    QCOMPARE(layoutItem.sizePolicy(), defaultPolicy);

    layoutItem.setSizePolicy(policy);
    QCOMPARE(layoutItem.sizePolicy(), policy);
    QCOMPARE(layoutItem.updateGeometryCalled, (defaultPolicy == policy) ? 0 : 1);
}

BOBUIEST_MAIN(tst_QGraphicsLayoutItem)
#include "tst_qgraphicslayoutitem.moc"

