// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <QDebug>
#include <BOBUIableView>
#include <QImage>
#include <QPainter>
#include <QHeaderView>
#include <QStandardItemModel>

class BobUITestTableModel: public QAbstractTableModel
{
    Q_OBJECT


public:
    BobUITestTableModel(int rows = 0, int columns = 0, QObject *parent = nullptr)
        : QAbstractTableModel(parent),
          row_count(rows),
          column_count(columns) {}

    int rowCount(const QModelIndex & = QModelIndex()) const override { return row_count; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return column_count; }
    bool isEditable(const QModelIndex &) const { return true; }

    QVariant data(const QModelIndex &idx, int role) const override
    {
        if (!idx.isValid() || idx.row() >= row_count || idx.column() >= column_count) {
            qWarning() << "Invalid modelIndex [%d,%d,%p]" << idx;
            return QVariant();
        }

        if (role == BobUI::DisplayRole || role == BobUI::EditRole)
            return QString("[%1,%2,%3]").arg(idx.row()).arg(idx.column()).arg(0);

        return QVariant();
    }

    bool insertRows(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start > row_count)
            return false;

        beginInsertRows(parent, start, start + count - 1);
        row_count += count;
        endInsertRows();
        return true;
    }

    bool removeRows(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start >= row_count || row_count < count)
            return false;

        beginRemoveRows(parent, start, start + count - 1);
        row_count -= count;
        endRemoveRows();
        return true;
    }

    bool insertColumns(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start > column_count)
            return false;

        beginInsertColumns(parent, start, start + count - 1);
        column_count += count;
        endInsertColumns();
        return true;
    }

    bool removeColumns(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start >= column_count || column_count < count)
            return false;

        beginRemoveColumns(parent, start, start + count - 1);
        column_count -= count;
        endRemoveColumns();
        return true;
    }

    int row_count;
    int column_count;
};




class tst_BOBUIableView : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIableView();
    virtual ~tst_BOBUIableView();

public slots:
    void init();
    void cleanup();

private slots:
    void spanInit();
    void spanDraw();
    void spanSelectColumn();
    void spanSelectAll();
    void rowInsertion_data();
    void rowInsertion();
    void rowRemoval_data();
    void rowRemoval();
    void columnInsertion_data();
    void columnInsertion();
    void columnRemoval_data();
    void columnRemoval();
    void sizeHintForColumnWhenHidden();
private:
    static inline void spanInit_helper(BOBUIableView *);
};

tst_BOBUIableView::tst_BOBUIableView()
{
}

tst_BOBUIableView::~tst_BOBUIableView()
{
}

void tst_BOBUIableView::init()
{
}

void tst_BOBUIableView::cleanup()
{
}

void tst_BOBUIableView::spanInit_helper(BOBUIableView *view)
{
    for (int i=0; i < 40; i++) {
        view->setSpan(1+i%2, 1+4*i, 1+i%3, 2);
    }

    for (int i=1; i < 40; i++) {
        view->setSpan(6 + i*7, 4, 4, 50);
    }
}

void tst_BOBUIableView::spanInit()
{
    BobUITestTableModel model(500, 500);
    BOBUIableView v;
    v.setModel(&model);

    QBENCHMARK {
        spanInit_helper(&v);
    }
}

void tst_BOBUIableView::spanDraw()
{
    BobUITestTableModel model(500, 500);
    BOBUIableView v;
    v.setModel(&model);

    spanInit_helper(&v);
    v.show();
    v.resize(500,500);
    BOBUIest::qWait(30);

    QImage image(500, 500, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    QBENCHMARK {
        v.render(&painter);
    }
}

void tst_BOBUIableView::spanSelectAll()
{
    BobUITestTableModel model(500, 500);
    BOBUIableView v;
    v.setModel(&model);

    spanInit_helper(&v);
    v.show();
    BOBUIest::qWait(30);

    QBENCHMARK {
        v.selectAll();
    }
}

void tst_BOBUIableView::spanSelectColumn()
{
    BobUITestTableModel model(500, 500);
    BOBUIableView v;
    v.setModel(&model);

    spanInit_helper(&v);
    v.show();
    BOBUIest::qWait(30);

    QBENCHMARK {
        v.selectColumn(22);
    }
}

typedef QList<QRect> SpanList;
Q_DECLARE_METATYPE(SpanList)

void spansData()
{
    BOBUIest::addColumn<SpanList>("spans");

    BOBUIest::newRow("Without spans")
        << SpanList();

    BOBUIest::newRow("With spans")
            << (SpanList()
                  << QRect(0, 1, 1, 2)
                  << QRect(1, 2, 1, 2)
                  << QRect(2, 2, 1, 5)
                  << QRect(2, 8, 1, 2)
                  << QRect(3, 4, 1, 2)
                  << QRect(4, 4, 1, 4)
                  << QRect(5, 6, 1, 3)
                  << QRect(6, 7, 1, 3));
}

void tst_BOBUIableView::rowInsertion_data()
{
    spansData();
}

void tst_BOBUIableView::rowInsertion()
{
    QFETCH(const SpanList, spans);

    BobUITestTableModel model(10, 10);
    BOBUIableView view;
    view.setModel(&model);

    for (QRect span : spans)
        view.setSpan(span.top(), span.left(), span.height(), span.width());
    view.show();
    BOBUIest::qWait(50);

    QBENCHMARK_ONCE {
        view.model()->insertRows(0, 2);
        view.model()->insertRows(5, 2);
        view.model()->insertRows(8, 2);
        view.model()->insertRows(12, 2);
    }
}

void tst_BOBUIableView::rowRemoval_data()
{
    spansData();
}

void tst_BOBUIableView::rowRemoval()
{
    QFETCH(const SpanList, spans);

    BobUITestTableModel model(10, 10);
    BOBUIableView view;
    view.setModel(&model);

    for (QRect span : spans)
        view.setSpan(span.top(), span.left(), span.height(), span.width());
    view.show();
    BOBUIest::qWait(50);

    QBENCHMARK_ONCE {
        view.model()->removeRows(3, 3);
    }
}

void tst_BOBUIableView::columnInsertion_data()
{
    spansData();
}

void tst_BOBUIableView::columnInsertion()
{
    QFETCH(const SpanList, spans);

    BobUITestTableModel model(10, 10);
    BOBUIableView view;
    view.setModel(&model);

    // Same set as for rowInsertion, just swapping columns and rows.
    for (QRect span : spans)
        view.setSpan(span.left(), span.top(), span.width(), span.height());
    view.show();
    BOBUIest::qWait(50);

    QBENCHMARK_ONCE {
        view.model()->insertColumns(0, 2);
        view.model()->insertColumns(5, 2);
        view.model()->insertColumns(8, 2);
        view.model()->insertColumns(12, 2);
    }
}

void tst_BOBUIableView::columnRemoval_data()
{
    spansData();
}

void tst_BOBUIableView::columnRemoval()
{
    QFETCH(const SpanList, spans);

    BobUITestTableModel model(10, 10);
    BOBUIableView view;
    view.setModel(&model);

    // Same set as for rowRemoval, just swapping columns and rows.
    for (QRect span : spans)
        view.setSpan(span.left(), span.top(), span.width(), span.height());
    view.show();
    BOBUIest::qWait(50);

    QBENCHMARK_ONCE {
        view.model()->removeColumns(3, 3);
    }
}

void tst_BOBUIableView::sizeHintForColumnWhenHidden()
{
    BOBUIableView view;
    QStandardItemModel model(12500, 6);
    for (int r = 0; r < model.rowCount(); ++r)
        for (int c = 0; c < model.columnCount(); ++c) {
            QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(r).arg(c));
            model.setItem(r, c, item);
        }

    view.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view.setModel(&model);
    QBENCHMARK_ONCE {
        view.horizontalHeader()->resizeSection(0, 10); // this force resizeSections - on a hidden view.
    }

}

BOBUIEST_MAIN(tst_BOBUIableView)
#include "tst_bobuiableview.moc"
