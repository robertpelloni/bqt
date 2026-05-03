// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <QListView>
#include <QStandardItemModel>


class tst_QListView : public QObject
{
    Q_OBJECT

public:
    tst_QListView() = default;
    virtual ~tst_QListView() = default;

private slots:
    void benchSetCurrentIndex();
};

void tst_QListView::benchSetCurrentIndex()
{
    QStandardItemModel sm(50000, 1);
    QListView lv;
    lv.setModel(&sm);
    const int rc = lv.model()->rowCount();
    for (int i = 0; i < rc; i+= 100)
        lv.setRowHidden(i, true);
    lv.setCurrentIndex(lv.model()->index(0, 0, QModelIndex()));
    lv.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&lv));

    QBENCHMARK_ONCE {
        while (lv.currentIndex().row() < rc - 20)
            lv.setCurrentIndex(lv.model()->index(lv.currentIndex().row() + 17,
                                                 lv.currentIndex().column(),
                                                 QModelIndex()));
    }
}


BOBUIEST_MAIN(tst_QListView)
#include "tst_qlistview.moc"
