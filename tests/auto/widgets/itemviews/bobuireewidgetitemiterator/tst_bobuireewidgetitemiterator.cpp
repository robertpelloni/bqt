// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIreeWidget>
#include <BOBUIreeWidgetItemIterator>
#include <BOBUIest>

Q_DECLARE_METATYPE(BOBUIreeWidgetItemIterator::IteratorFlag)
Q_DECLARE_METATYPE(BOBUIreeWidgetItemIterator::IteratorFlags)

class tst_BOBUIreeWidgetItemIterator : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void postincrement();
    void preincrement();
    void postdecrement();
    void predecrement();
    void plus_eq_data();
    void plus_eq();
    void minus_eq_data();
    void minus_eq();
    void iteratorflags_data();
    void iteratorflags();
    void updateIfModifiedFromWidget_data();
    void updateIfModifiedFromWidget();
    void constructIteratorWithItem_data();
    void constructIteratorWithItem();
    void updateIteratorAfterDeletedItem_and_ContinueIteration_data();
    void updateIteratorAfterDeletedItem_and_ContinueIteration();
    void initializeIterator();
    void sortingEnabled();
private:
    BOBUIreeWidget *testWidget = nullptr;
};

void tst_BOBUIreeWidgetItemIterator::initTestCase()
{
    testWidget = new BOBUIreeWidget();
    testWidget->clear();
    testWidget->setColumnCount(2);
    testWidget->show();


    /**
     * These are default:
     *
     *           BobUI::ItemIsSelectable
     *          |BobUI::ItemIsUserCheckable
     *          |BobUI::ItemIsEnabled
     *          |BobUI::ItemIsDragEnabled
     *          |BobUI::ItemIsDropEnabled
     *
     */
    for (int i = 0; i <= 16; ++i) {
        BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(testWidget);
        const QString topS = QLatin1String("top") + QString::number(i);
        top->setText(0, topS);
        switch (i) {
            case 0:  top->setHidden(true);break;
            case 1:  top->setHidden(false);break;

            case 2:  top->setSelected(true);break;
            case 3:  top->setSelected(false);break;

            case 4:  top->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsSelectable);break;
            case 5:  top->setFlags(BobUI::ItemIsEnabled);break;

            case 6:  top->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsDragEnabled);break;
            case 7:  top->setFlags(BobUI::ItemIsEnabled);break;

            case 8:  top->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsDropEnabled);break;
            case 9:  top->setFlags(BobUI::ItemIsEnabled);break;

            case 10:  top->setFlags(BobUI::ItemIsEnabled);break;
            case 11:  top->setFlags({});break;

            case 12:  top->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsEditable);break;
            case 13:  top->setFlags(BobUI::ItemIsEnabled);break;

            case 14: top->setCheckState(0, BobUI::Checked);break;
            case 15: top->setCheckState(0, BobUI::Unchecked);break;
            case 16: top->setCheckState(0, BobUI::PartiallyChecked);break;
        }
        for (int j=0; j <= 16; ++j) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(top);
            child->setText(0, topS + QLatin1String(",child") + QString::number(j));
            switch (j) {
                case 0:  child->setHidden(true);break;
                case 1:  child->setHidden(false);break;

                case 2:  child->setSelected(true);break;
                case 3:  child->setSelected(false);break;

                case 4:  child->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsSelectable);break;
                case 5:  child->setFlags(BobUI::ItemIsEnabled);break;

                case 6:  child->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsDragEnabled);break;
                case 7:  child->setFlags(BobUI::ItemIsEnabled);break;

                case 8:  child->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsDropEnabled);break;
                case 9:  child->setFlags(BobUI::ItemIsEnabled);break;

                case 10:  child->setFlags(BobUI::ItemIsEnabled);break;
                case 11:  child->setFlags({});break;

                case 12:  child->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsEditable);break;
                case 13:  child->setFlags(BobUI::ItemIsEnabled);break;

                case 14: child->setCheckState(0, BobUI::Checked);break;
                case 15: child->setCheckState(0, BobUI::Unchecked);break;
                case 16: child->setCheckState(0, BobUI::PartiallyChecked);break;
            }

        }
    }
}

void tst_BOBUIreeWidgetItemIterator::cleanupTestCase()
{
    testWidget->hide();
    delete testWidget;
}

void tst_BOBUIreeWidgetItemIterator::iteratorflags_data()
{
  /*
  // Should preferably test for all these flags (and combinations).....

        All           = 0x00000000,
        Hidden        = 0x00000001,
        NotHidden     = 0x00000002,
        Selected      = 0x00000004,
        Unselected    = 0x00000008,
        Selectable    = 0x00000010,
        NotSelectable = 0x00000020,
        DragEnabled   = 0x00000040,
        DragDisabled  = 0x00000080,
        DropEnabled   = 0x00000100,
        DropDisabled  = 0x00000200,
        HasChildren   = 0x00000400,
        NoChildren    = 0x00000800,
        Checked       = 0x00001000,
        NotChecked    = 0x00002000,
        Enabled       = 0x00004000,
        Disabled      = 0x00008000,
        Editable      = 0x00010000,
        NotEditable   = 0x00020000
*/
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<BOBUIreeWidgetItemIterator::IteratorFlags>("iteratorflags");
    BOBUIest::addColumn<QStringList>("matches");

    BOBUIest::newRow("Match all") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::All)
                               << (QStringList()
                                                                << "top0"   << "top0,child0"  << "top0,child1"  << "top0,child2"  << "top0,child3"
                                                                            << "top0,child4"  << "top0,child5"  << "top0,child6"  << "top0,child7"
                                                                            << "top0,child8"  << "top0,child9"  << "top0,child10" << "top0,child11"
                                                                            << "top0,child12" << "top0,child13" << "top0,child14" << "top0,child15"
                                                                            << "top0,child16"
                                                                << "top1"   << "top1,child0"  << "top1,child1"  << "top1,child2"  << "top1,child3"
                                                                            << "top1,child4"  << "top1,child5"  << "top1,child6"  << "top1,child7"
                                                                            << "top1,child8"  << "top1,child9"  << "top1,child10" << "top1,child11"
                                                                            << "top1,child12" << "top1,child13" << "top1,child14" << "top1,child15"
                                                                            << "top1,child16"
                                                                << "top2"   << "top2,child0"  << "top2,child1"  << "top2,child2"  << "top2,child3"
                                                                            << "top2,child4"  << "top2,child5"  << "top2,child6"  << "top2,child7"
                                                                            << "top2,child8"  << "top2,child9"  << "top2,child10" << "top2,child11"
                                                                            << "top2,child12" << "top2,child13" << "top2,child14" << "top2,child15"
                                                                            << "top2,child16"
                                                                << "top3"   << "top3,child0"  << "top3,child1"  << "top3,child2"  << "top3,child3"
                                                                            << "top3,child4"  << "top3,child5"  << "top3,child6"  << "top3,child7"
                                                                            << "top3,child8"  << "top3,child9"  << "top3,child10" << "top3,child11"
                                                                            << "top3,child12" << "top3,child13" << "top3,child14" << "top3,child15"
                                                                            << "top3,child16"
                                                                << "top4"   << "top4,child0"  << "top4,child1"  << "top4,child2"  << "top4,child3"
                                                                            << "top4,child4"  << "top4,child5"  << "top4,child6"  << "top4,child7"
                                                                            << "top4,child8"  << "top4,child9"  << "top4,child10" << "top4,child11"
                                                                            << "top4,child12" << "top4,child13" << "top4,child14" << "top4,child15"
                                                                            << "top4,child16"
                                                                << "top5"   << "top5,child0"  << "top5,child1"  << "top5,child2"  << "top5,child3"
                                                                            << "top5,child4"  << "top5,child5"  << "top5,child6"  << "top5,child7"
                                                                            << "top5,child8"  << "top5,child9"  << "top5,child10" << "top5,child11"
                                                                            << "top5,child12" << "top5,child13" << "top5,child14" << "top5,child15"
                                                                            << "top5,child16"
                                                                << "top6"   << "top6,child0"  << "top6,child1"  << "top6,child2"  << "top6,child3"
                                                                            << "top6,child4"  << "top6,child5"  << "top6,child6"  << "top6,child7"
                                                                            << "top6,child8"  << "top6,child9"  << "top6,child10" << "top6,child11"
                                                                            << "top6,child12" << "top6,child13" << "top6,child14" << "top6,child15"
                                                                            << "top6,child16"
                                                                << "top7"   << "top7,child0"  << "top7,child1"  << "top7,child2"  << "top7,child3"
                                                                            << "top7,child4"  << "top7,child5"  << "top7,child6"  << "top7,child7"
                                                                            << "top7,child8"  << "top7,child9"  << "top7,child10" << "top7,child11"
                                                                            << "top7,child12" << "top7,child13" << "top7,child14" << "top7,child15"
                                                                            << "top7,child16"
                                                                << "top8"   << "top8,child0"  << "top8,child1"  << "top8,child2"  << "top8,child3"
                                                                            << "top8,child4"  << "top8,child5"  << "top8,child6"  << "top8,child7"
                                                                            << "top8,child8"  << "top8,child9"  << "top8,child10" << "top8,child11"
                                                                            << "top8,child12" << "top8,child13" << "top8,child14" << "top8,child15"
                                                                            << "top8,child16"
                                                                << "top9"   << "top9,child0"  << "top9,child1"  << "top9,child2"  << "top9,child3"
                                                                            << "top9,child4"  << "top9,child5"  << "top9,child6"  << "top9,child7"
                                                                            << "top9,child8"  << "top9,child9"  << "top9,child10" << "top9,child11"
                                                                            << "top9,child12" << "top9,child13" << "top9,child14" << "top9,child15"
                                                                            << "top9,child16"
                                                                << "top10"  << "top10,child0"  << "top10,child1"  << "top10,child2"  << "top10,child3"
                                                                            << "top10,child4"  << "top10,child5"  << "top10,child6"  << "top10,child7"
                                                                            << "top10,child8"  << "top10,child9"  << "top10,child10" << "top10,child11"
                                                                            << "top10,child12" << "top10,child13" << "top10,child14" << "top10,child15"
                                                                            << "top10,child16"
                                                                << "top11"  << "top11,child0"  << "top11,child1"  << "top11,child2"  << "top11,child3"
                                                                            << "top11,child4"  << "top11,child5"  << "top11,child6"  << "top11,child7"
                                                                            << "top11,child8"  << "top11,child9"  << "top11,child10" << "top11,child11"
                                                                            << "top11,child12" << "top11,child13" << "top11,child14" << "top11,child15"
                                                                            << "top11,child16"
                                                                << "top12"  << "top12,child0"  << "top12,child1"  << "top12,child2"  << "top12,child3"
                                                                            << "top12,child4"  << "top12,child5"  << "top12,child6"  << "top12,child7"
                                                                            << "top12,child8"  << "top12,child9"  << "top12,child10" << "top12,child11"
                                                                            << "top12,child12" << "top12,child13" << "top12,child14" << "top12,child15"
                                                                            << "top12,child16"
                                                                << "top13"  << "top13,child0"  << "top13,child1"  << "top13,child2"  << "top13,child3"
                                                                            << "top13,child4"  << "top13,child5"  << "top13,child6"  << "top13,child7"
                                                                            << "top13,child8"  << "top13,child9"  << "top13,child10" << "top13,child11"
                                                                            << "top13,child12" << "top13,child13" << "top13,child14" << "top13,child15"
                                                                            << "top13,child16"
                                                                << "top14"  << "top14,child0"  << "top14,child1"  << "top14,child2"  << "top14,child3"
                                                                            << "top14,child4"  << "top14,child5"  << "top14,child6"  << "top14,child7"
                                                                            << "top14,child8"  << "top14,child9"  << "top14,child10" << "top14,child11"
                                                                            << "top14,child12" << "top14,child13" << "top14,child14" << "top14,child15"
                                                                            << "top14,child16"
                                                                << "top15"  << "top15,child0"  << "top15,child1"  << "top15,child2"  << "top15,child3"
                                                                            << "top15,child4"  << "top15,child5"  << "top15,child6"  << "top15,child7"
                                                                            << "top15,child8"  << "top15,child9"  << "top15,child10" << "top15,child11"
                                                                            << "top15,child12" << "top15,child13" << "top15,child14" << "top15,child15"
                                                                            << "top15,child16"
                                                                << "top16"  << "top16,child0"  << "top16,child1"  << "top16,child2"  << "top16,child3"
                                                                            << "top16,child4"  << "top16,child5"  << "top16,child6"  << "top16,child7"
                                                                            << "top16,child8"  << "top16,child9"  << "top16,child10" << "top16,child11"
                                                                            << "top16,child12" << "top16,child13" << "top16,child14" << "top16,child15"
                                                                            << "top16,child16");

    BOBUIest::newRow("Match hidden") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Hidden)
                                  << (QStringList()
                                                                << "top0" << "top0,child0"  // fails due to hidden row
                                                                << "top1,child0"
                                                                << "top2,child0"
                                                                << "top3,child0"
                                                                << "top4,child0"
                                                                << "top5,child0"
                                                                << "top6,child0"
                                                                << "top7,child0"
                                                                << "top8,child0"
                                                                << "top9,child0"
                                                                << "top10,child0"
                                                                << "top11,child0"
                                                                << "top12,child0"
                                                                << "top13,child0"
                                                                << "top14,child0"
                                                                << "top15,child0"
                                      << "top16,child0");

    BOBUIest::newRow("Match not hidden") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::NotHidden)
                                      << (QStringList()
                                                                            << "top0,child1"  << "top0,child2"  << "top0,child3"
                                                                            << "top0,child4"  << "top0,child5"  << "top0,child6"  << "top0,child7"
                                                                            << "top0,child8"  << "top0,child9"  << "top0,child10" << "top0,child11"
                                                                            << "top0,child12" << "top0,child13" << "top0,child14" << "top0,child15"
                                                                            << "top0,child16"
                                                                << "top1"   << "top1,child1"  << "top1,child2"  << "top1,child3"
                                                                            << "top1,child4"  << "top1,child5"  << "top1,child6"  << "top1,child7"
                                                                            << "top1,child8"  << "top1,child9"  << "top1,child10" << "top1,child11"
                                                                            << "top1,child12" << "top1,child13" << "top1,child14" << "top1,child15"
                                                                            << "top1,child16"
                                                                << "top2"   << "top2,child1"  << "top2,child2"  << "top2,child3"
                                                                            << "top2,child4"  << "top2,child5"  << "top2,child6"  << "top2,child7"
                                                                            << "top2,child8"  << "top2,child9"  << "top2,child10" << "top2,child11"
                                                                            << "top2,child12" << "top2,child13" << "top2,child14" << "top2,child15"
                                                                            << "top2,child16"
                                                                << "top3"   << "top3,child1"  << "top3,child2"  << "top3,child3"
                                                                            << "top3,child4"  << "top3,child5"  << "top3,child6"  << "top3,child7"
                                                                            << "top3,child8"  << "top3,child9"  << "top3,child10" << "top3,child11"
                                                                            << "top3,child12" << "top3,child13" << "top3,child14" << "top3,child15"
                                                                            << "top3,child16"
                                                                << "top4"   << "top4,child1"  << "top4,child2"  << "top4,child3"
                                                                            << "top4,child4"  << "top4,child5"  << "top4,child6"  << "top4,child7"
                                                                            << "top4,child8"  << "top4,child9"  << "top4,child10" << "top4,child11"
                                                                            << "top4,child12" << "top4,child13" << "top4,child14" << "top4,child15"
                                                                            << "top4,child16"
                                                                << "top5"   << "top5,child1"  << "top5,child2"  << "top5,child3"
                                                                            << "top5,child4"  << "top5,child5"  << "top5,child6"  << "top5,child7"
                                                                            << "top5,child8"  << "top5,child9"  << "top5,child10" << "top5,child11"
                                                                            << "top5,child12" << "top5,child13" << "top5,child14" << "top5,child15"
                                                                            << "top5,child16"
                                                                << "top6"   << "top6,child1"  << "top6,child2"  << "top6,child3"
                                                                            << "top6,child4"  << "top6,child5"  << "top6,child6"  << "top6,child7"
                                                                            << "top6,child8"  << "top6,child9"  << "top6,child10" << "top6,child11"
                                                                            << "top6,child12" << "top6,child13" << "top6,child14" << "top6,child15"
                                                                            << "top6,child16"
                                                                << "top7"   << "top7,child1"  << "top7,child2"  << "top7,child3"
                                                                            << "top7,child4"  << "top7,child5"  << "top7,child6"  << "top7,child7"
                                                                            << "top7,child8"  << "top7,child9"  << "top7,child10" << "top7,child11"
                                                                            << "top7,child12" << "top7,child13" << "top7,child14" << "top7,child15"
                                                                            << "top7,child16"
                                                                << "top8"   << "top8,child1"  << "top8,child2"  << "top8,child3"
                                                                            << "top8,child4"  << "top8,child5"  << "top8,child6"  << "top8,child7"
                                                                            << "top8,child8"  << "top8,child9"  << "top8,child10" << "top8,child11"
                                                                            << "top8,child12" << "top8,child13" << "top8,child14" << "top8,child15"
                                                                            << "top8,child16"
                                                                << "top9"   << "top9,child1"  << "top9,child2"  << "top9,child3"
                                                                            << "top9,child4"  << "top9,child5"  << "top9,child6"  << "top9,child7"
                                                                            << "top9,child8"  << "top9,child9"  << "top9,child10" << "top9,child11"
                                                                            << "top9,child12" << "top9,child13" << "top9,child14" << "top9,child15"
                                                                            << "top9,child16"
                                                                << "top10"  << "top10,child1"  << "top10,child2"  << "top10,child3"
                                                                            << "top10,child4"  << "top10,child5"  << "top10,child6"  << "top10,child7"
                                                                            << "top10,child8"  << "top10,child9"  << "top10,child10" << "top10,child11"
                                                                            << "top10,child12" << "top10,child13" << "top10,child14" << "top10,child15"
                                                                            << "top10,child16"
                                                                << "top11"  << "top11,child1"  << "top11,child2"  << "top11,child3"
                                                                            << "top11,child4"  << "top11,child5"  << "top11,child6"  << "top11,child7"
                                                                            << "top11,child8"  << "top11,child9"  << "top11,child10" << "top11,child11"
                                                                            << "top11,child12" << "top11,child13" << "top11,child14" << "top11,child15"
                                                                            << "top11,child16"
                                                                << "top12"  << "top12,child1"  << "top12,child2"  << "top12,child3"
                                                                            << "top12,child4"  << "top12,child5"  << "top12,child6"  << "top12,child7"
                                                                            << "top12,child8"  << "top12,child9"  << "top12,child10" << "top12,child11"
                                                                            << "top12,child12" << "top12,child13" << "top12,child14" << "top12,child15"
                                                                            << "top12,child16"
                                                                << "top13"  << "top13,child1"  << "top13,child2"  << "top13,child3"
                                                                            << "top13,child4"  << "top13,child5"  << "top13,child6"  << "top13,child7"
                                                                            << "top13,child8"  << "top13,child9"  << "top13,child10" << "top13,child11"
                                                                            << "top13,child12" << "top13,child13" << "top13,child14" << "top13,child15"
                                                                            << "top13,child16"
                                                                << "top14"  << "top14,child1"  << "top14,child2"  << "top14,child3"
                                                                            << "top14,child4"  << "top14,child5"  << "top14,child6"  << "top14,child7"
                                                                            << "top14,child8"  << "top14,child9"  << "top14,child10" << "top14,child11"
                                                                            << "top14,child12" << "top14,child13" << "top14,child14" << "top14,child15"
                                                                            << "top14,child16"
                                                                << "top15"  << "top15,child1"  << "top15,child2"  << "top15,child3"
                                                                            << "top15,child4"  << "top15,child5"  << "top15,child6"  << "top15,child7"
                                                                            << "top15,child8"  << "top15,child9"  << "top15,child10" << "top15,child11"
                                                                            << "top15,child12" << "top15,child13" << "top15,child14" << "top15,child15"
                                                                            << "top15,child16"
                                                                << "top16"  << "top16,child1"  << "top16,child2"  << "top16,child3"
                                                                            << "top16,child4"  << "top16,child5"  << "top16,child6"  << "top16,child7"
                                                                            << "top16,child8"  << "top16,child9"  << "top16,child10" << "top16,child11"
                                                                            << "top16,child12" << "top16,child13" << "top16,child14" << "top16,child15"
                                                                            << "top16,child16");

    BOBUIest::newRow("Match selected") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Selected)
                                    << (QStringList()
                                                                << "top0,child2"
                                                                << "top1,child2"
                                                                << "top2" << "top2,child2"
                                                                << "top3,child2"
                                                                << "top4,child2"
                                                                << "top5,child2"
                                                                << "top6,child2"
                                                                << "top7,child2"
                                                                << "top8,child2"
                                                                << "top9,child2"
                                                                << "top10,child2"
                                                                << "top11,child2"
                                                                << "top12,child2"
                                                                << "top13,child2"
                                                                << "top14,child2"
                                                                << "top15,child2"
                                        << "top16,child2");

    BOBUIest::newRow("Match selectable") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Selectable)
                                      << (QStringList()
                                                                << "top0"   << "top0,child0"  << "top0,child1"    << "top0,child2"  << "top0,child3"
                                                                            << "top0,child4"
                                                                                                                  << "top0,child14" << "top0,child15"
                                                                            << "top0,child16"
                                                                << "top1"   << "top1,child0"  << "top1,child1"    << "top1,child2"  << "top1,child3"
                                                                            << "top1,child4"
                                                                                                                  << "top1,child14" << "top1,child15"
                                                                            << "top1,child16"
                                                                << "top2"   << "top2,child0"  << "top2,child1"    << "top2,child2"  << "top2,child3"
                                                                            << "top2,child4"
                                                                                                                  << "top2,child14" << "top2,child15"
                                                                            << "top2,child16"
                                                                << "top3"   << "top3,child0"  << "top3,child1"    << "top3,child2"  << "top3,child3"
                                                                            << "top3,child4"
                                                                                                                  << "top3,child14" << "top3,child15"
                                                                            << "top3,child16"
                                                                << "top4"   << "top4,child0"  << "top4,child1"    << "top4,child2"  << "top4,child3"
                                                                            << "top4,child4"
                                                                                                                  << "top4,child14" << "top4,child15"
                                                                            << "top4,child16"
                                                                /* "top5"*/ << "top5,child0"  << "top5,child1"    << "top5,child2"  << "top5,child3"
                                                                            << "top5,child4"
                                                                                                                  << "top5,child14" << "top5,child15"
                                                                            << "top5,child16"
                                                                /* "top6"*/ << "top6,child0"  << "top6,child1"    << "top6,child2"  << "top6,child3"
                                                                            << "top6,child4"
                                                                                                                  << "top6,child14" << "top6,child15"
                                                                            << "top6,child16"
                                                                /* "top7"*/ << "top7,child0"  << "top7,child1"    << "top7,child2"  << "top7,child3"
                                                                            << "top7,child4"
                                                                                                                  << "top7,child14" << "top7,child15"
                                                                            << "top7,child16"
                                                                /* "top8"*/ << "top8,child0"  << "top8,child1"    << "top8,child2"  << "top8,child3"
                                                                            << "top8,child4"
                                                                                                                  << "top8,child14" << "top8,child15"
                                                                            << "top8,child16"
                                                                /* "top9"*/ << "top9,child0"  << "top9,child1"    << "top9,child2"  << "top9,child3"
                                                                            << "top9,child4"
                                                                                                                  << "top9,child14" << "top9,child15"
                                                                            << "top9,child16"
                                                                /* "top10*/ << "top10,child0"  << "top10,child1"    << "top10,child2"  << "top10,child3"
                                                                            << "top10,child4"
                                                                                                                  << "top10,child14" << "top10,child15"
                                                                            << "top10,child16"
                                                                /* "top11*/ << "top11,child0"  << "top11,child1"    << "top11,child2"  << "top11,child3"
                                                                            << "top11,child4"
                                                                                                                  << "top11,child14" << "top11,child15"
                                                                            << "top11,child16"
                                                                /* "top12*/ << "top12,child0"  << "top12,child1"    << "top12,child2"  << "top12,child3"
                                                                            << "top12,child4"
                                                                                                                  << "top12,child14" << "top12,child15"
                                                                            << "top12,child16"
                                                                /* "top13*/ << "top13,child0"  << "top13,child1"    << "top13,child2"  << "top13,child3"
                                                                            << "top13,child4"
                                                                                                                  << "top13,child14" << "top13,child15"
                                                                            << "top13,child16"
                                                                << "top14"  << "top14,child0"  << "top14,child1"    << "top14,child2"  << "top14,child3"
                                                                            << "top14,child4"
                                                                                                                  << "top14,child14" << "top14,child15"
                                                                            << "top14,child16"
                                                                << "top15"  << "top15,child0"  << "top15,child1"    << "top15,child2"  << "top15,child3"
                                                                            << "top15,child4"
                                                                                                                  << "top15,child14" << "top15,child15"
                                                                            << "top15,child16"
                                                                << "top16"  << "top16,child0"  << "top16,child1"    << "top16,child2"  << "top16,child3"
                                                                            << "top16,child4"
                                                                                                                  << "top16,child14" << "top16,child15"
                                          << "top16,child16");


    BOBUIest::newRow("Match DragEnabled") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::DragEnabled)
                                       << (QStringList()
                                                                << "top0"   << "top0,child0"  << "top0,child1"    << "top0,child2"  << "top0,child3"
                                                                            << "top0,child6"
                                                                                                                  << "top0,child14" << "top0,child15"
                                                                            << "top0,child16"
                                                                << "top1"   << "top1,child0"  << "top1,child1"    << "top1,child2"  << "top1,child3"
                                                                            << "top1,child6"
                                                                                                                  << "top1,child14" << "top1,child15"
                                                                            << "top1,child16"
                                                                << "top2"   << "top2,child0"  << "top2,child1"    << "top2,child2"  << "top2,child3"
                                                                            << "top2,child6"
                                                                                                                  << "top2,child14" << "top2,child15"
                                                                            << "top2,child16"
                                                                << "top3"   << "top3,child0"  << "top3,child1"    << "top3,child2"  << "top3,child3"
                                                                            << "top3,child6"
                                                                                                                  << "top3,child14" << "top3,child15"
                                                                            << "top3,child16"
                                                                /* "top4"*/ << "top4,child0"  << "top4,child1"    << "top4,child2"  << "top4,child3"
                                                                            << "top4,child6"
                                                                                                                  << "top4,child14" << "top4,child15"
                                                                            << "top4,child16"
                                                                /* "top5"*/ << "top5,child0"  << "top5,child1"    << "top5,child2"  << "top5,child3"
                                                                            << "top5,child6"
                                                                                                                  << "top5,child14" << "top5,child15"
                                                                            << "top5,child16"
                                                                << "top6"   << "top6,child0"  << "top6,child1"    << "top6,child2"  << "top6,child3"
                                                                            << "top6,child6"
                                                                                                                  << "top6,child14" << "top6,child15"
                                                                            << "top6,child16"
                                                                /* "top7"*/ << "top7,child0"  << "top7,child1"    << "top7,child2"  << "top7,child3"
                                                                            << "top7,child6"
                                                                                                                  << "top7,child14" << "top7,child15"
                                                                            << "top7,child16"
                                                                /* "top8"*/ << "top8,child0"  << "top8,child1"    << "top8,child2"  << "top8,child3"
                                                                            << "top8,child6"
                                                                                                                  << "top8,child14" << "top8,child15"
                                                                            << "top8,child16"
                                                                /* "top9"*/ << "top9,child0"  << "top9,child1"    << "top9,child2"  << "top9,child3"
                                                                            << "top9,child6"
                                                                                                                  << "top9,child14" << "top9,child15"
                                                                            << "top9,child16"
                                                                /* "top10*/ << "top10,child0"  << "top10,child1"    << "top10,child2"  << "top10,child3"
                                                                            << "top10,child6"
                                                                                                                  << "top10,child14" << "top10,child15"
                                                                            << "top10,child16"
                                                                /* "top11*/ << "top11,child0"  << "top11,child1"    << "top11,child2"  << "top11,child3"
                                                                            << "top11,child6"
                                                                                                                  << "top11,child14" << "top11,child15"
                                                                            << "top11,child16"
                                                                /* "top12*/ << "top12,child0"  << "top12,child1"    << "top12,child2"  << "top12,child3"
                                                                            << "top12,child6"
                                                                                                                  << "top12,child14" << "top12,child15"
                                                                            << "top12,child16"
                                                                /* "top13*/ << "top13,child0"  << "top13,child1"    << "top13,child2"  << "top13,child3"
                                                                            << "top13,child6"
                                                                                                                  << "top13,child14" << "top13,child15"
                                                                            << "top13,child16"
                                                                << "top14"  << "top14,child0"  << "top14,child1"    << "top14,child2"  << "top14,child3"
                                                                            << "top14,child6"
                                                                                                                  << "top14,child14" << "top14,child15"
                                                                            << "top14,child16"
                                                                << "top15"  << "top15,child0"  << "top15,child1"    << "top15,child2"  << "top15,child3"
                                                                            << "top15,child6"
                                                                                                                  << "top15,child14" << "top15,child15"
                                                                            << "top15,child16"
                                                                << "top16"  << "top16,child0"  << "top16,child1"    << "top16,child2"  << "top16,child3"
                                                                            << "top16,child6"
                                                                                                                  << "top16,child14" << "top16,child15"
                                           << "top16,child16");

    BOBUIest::newRow("Match DragDisabled") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::DragDisabled)
                                        << (QStringList()

                                                                /* top0  */
                                                                            << "top0,child4" << "top0,child5"   << "top0,child7"    << "top0,child8"
                                                                            << "top0,child9" << "top0,child10"  << "top0,child11"   << "top0,child12"
                                                                            << "top0,child13"
                                                                /* top1  */
                                                                            << "top1,child4" << "top1,child5"   << "top1,child7"    << "top1,child8"
                                                                            << "top1,child9" << "top1,child10"  << "top1,child11"   << "top1,child12"
                                                                            << "top1,child13"
                                                                /* top2  */
                                                                            << "top2,child4" << "top2,child5"   << "top2,child7"    << "top2,child8"
                                                                            << "top2,child9" << "top2,child10"  << "top2,child11"   << "top2,child12"
                                                                            << "top2,child13"
                                                                /* top3  */
                                                                            << "top3,child4" << "top3,child5"   << "top3,child7"    << "top3,child8"
                                                                            << "top3,child9" << "top3,child10"  << "top3,child11"   << "top3,child12"
                                                                            << "top3,child13"
                                                                << "top4"
                                                                            << "top4,child4" << "top4,child5"   << "top4,child7"    << "top4,child8"
                                                                            << "top4,child9" << "top4,child10"  << "top4,child11"   << "top4,child12"
                                                                            << "top4,child13"
                                                                << "top5"
                                                                            << "top5,child4" << "top5,child5"   << "top5,child7"    << "top5,child8"
                                                                            << "top5,child9" << "top5,child10"  << "top5,child11"   << "top5,child12"
                                                                            << "top5,child13"
                                                                /* "top6"*/
                                                                            << "top6,child4" << "top6,child5"   << "top6,child7"    << "top6,child8"
                                                                            << "top6,child9" << "top6,child10"  << "top6,child11"   << "top6,child12"
                                                                            << "top6,child13"
                                                                << "top7"
                                                                            << "top7,child4" << "top7,child5"   << "top7,child7"    << "top7,child8"
                                                                            << "top7,child9" << "top7,child10"  << "top7,child11"   << "top7,child12"
                                                                            << "top7,child13"
                                                                << "top8"
                                                                            << "top8,child4" << "top8,child5"   << "top8,child7"    << "top8,child8"
                                                                            << "top8,child9" << "top8,child10"  << "top8,child11"   << "top8,child12"
                                                                            << "top8,child13"
                                                                << "top9"
                                                                            << "top9,child4" << "top9,child5"   << "top9,child7"    << "top9,child8"
                                                                            << "top9,child9" << "top9,child10"  << "top9,child11"   << "top9,child12"
                                                                            << "top9,child13"
                                                                << "top10"
                                                                            << "top10,child4" << "top10,child5"   << "top10,child7"    << "top10,child8"
                                                                            << "top10,child9" << "top10,child10"  << "top10,child11"   << "top10,child12"
                                                                            << "top10,child13"
                                                                << "top11"
                                                                            << "top11,child4" << "top11,child5"   << "top11,child7"    << "top11,child8"
                                                                            << "top11,child9" << "top11,child10"  << "top11,child11"   << "top11,child12"
                                                                            << "top11,child13"
                                                                << "top12"
                                                                            << "top12,child4" << "top12,child5"   << "top12,child7"    << "top12,child8"
                                                                            << "top12,child9" << "top12,child10"  << "top12,child11"   << "top12,child12"
                                                                            << "top12,child13"
                                                                << "top13"
                                                                            << "top13,child4" << "top13,child5"   << "top13,child7"    << "top13,child8"
                                                                            << "top13,child9" << "top13,child10"  << "top13,child11"   << "top13,child12"
                                                                            << "top13,child13"
                                                                /* top14 */
                                                                            << "top14,child4" << "top14,child5"   << "top14,child7"    << "top14,child8"
                                                                            << "top14,child9" << "top14,child10"  << "top14,child11"   << "top14,child12"
                                                                            << "top14,child13"
                                                                /* top15  */
                                                                            << "top15,child4" << "top15,child5"   << "top15,child7"    << "top15,child8"
                                                                            << "top15,child9" << "top15,child10"  << "top15,child11"   << "top15,child12"
                                                                            << "top15,child13"
                                                                /* top16  */
                                                                            << "top16,child4" << "top16,child5"   << "top16,child7"    << "top16,child8"
                                                                            << "top16,child9" << "top16,child10"  << "top16,child11"   << "top16,child12"
                                                                            << "top16,child13" );


    BOBUIest::newRow("Match DropEnabled") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::DropEnabled)
                                       << (QStringList()
                                                                << "top0"   << "top0,child0"  << "top0,child1"    << "top0,child2"  << "top0,child3"
                                                                            << "top0,child8"
                                                                                                                  << "top0,child14" << "top0,child15"
                                                                            << "top0,child16"
                                                                << "top1"   << "top1,child0"  << "top1,child1"    << "top1,child2"  << "top1,child3"
                                                                            << "top1,child8"
                                                                                                                  << "top1,child14" << "top1,child15"
                                                                            << "top1,child16"
                                                                << "top2"   << "top2,child0"  << "top2,child1"    << "top2,child2"  << "top2,child3"
                                                                            << "top2,child8"
                                                                                                                  << "top2,child14" << "top2,child15"
                                                                            << "top2,child16"
                                                                << "top3"   << "top3,child0"  << "top3,child1"    << "top3,child2"  << "top3,child3"
                                                                            << "top3,child8"
                                                                                                                  << "top3,child14" << "top3,child15"
                                                                            << "top3,child16"
                                                                /* "top4"*/ << "top4,child0"  << "top4,child1"    << "top4,child2"  << "top4,child3"
                                                                            << "top4,child8"
                                                                                                                  << "top4,child14" << "top4,child15"
                                                                            << "top4,child16"
                                                                /* "top5"*/ << "top5,child0"  << "top5,child1"    << "top5,child2"  << "top5,child3"
                                                                            << "top5,child8"
                                                                                                                  << "top5,child14" << "top5,child15"
                                                                            << "top5,child16"
                                                                /* "top6"*/ << "top6,child0"  << "top6,child1"    << "top6,child2"  << "top6,child3"
                                                                            << "top6,child8"
                                                                                                                  << "top6,child14" << "top6,child15"
                                                                            << "top6,child16"
                                                                /* "top7"*/ << "top7,child0"  << "top7,child1"    << "top7,child2"  << "top7,child3"
                                                                            << "top7,child8"
                                                                                                                  << "top7,child14" << "top7,child15"
                                                                            << "top7,child16"
                                                                << "top8"   << "top8,child0"  << "top8,child1"    << "top8,child2"  << "top8,child3"
                                                                            << "top8,child8"
                                                                                                                  << "top8,child14" << "top8,child15"
                                                                            << "top8,child16"
                                                                /* "top9"*/ << "top9,child0"  << "top9,child1"    << "top9,child2"  << "top9,child3"
                                                                            << "top9,child8"
                                                                                                                  << "top9,child14" << "top9,child15"
                                                                            << "top9,child16"
                                                                /* "top10*/ << "top10,child0"  << "top10,child1"    << "top10,child2"  << "top10,child3"
                                                                            << "top10,child8"
                                                                                                                  << "top10,child14" << "top10,child15"
                                                                            << "top10,child16"
                                                                /* "top11*/ << "top11,child0"  << "top11,child1"    << "top11,child2"  << "top11,child3"
                                                                            << "top11,child8"
                                                                                                                  << "top11,child14" << "top11,child15"
                                                                            << "top11,child16"
                                                                /* "top12*/ << "top12,child0"  << "top12,child1"    << "top12,child2"  << "top12,child3"
                                                                            << "top12,child8"
                                                                                                                  << "top12,child14" << "top12,child15"
                                                                            << "top12,child16"
                                                                /* "top13*/ << "top13,child0"  << "top13,child1"    << "top13,child2"  << "top13,child3"
                                                                            << "top13,child8"
                                                                                                                  << "top13,child14" << "top13,child15"
                                                                            << "top13,child16"
                                                                << "top14"  << "top14,child0"  << "top14,child1"    << "top14,child2"  << "top14,child3"
                                                                            << "top14,child8"
                                                                                                                  << "top14,child14" << "top14,child15"
                                                                            << "top14,child16"
                                                                << "top15"  << "top15,child0"  << "top15,child1"    << "top15,child2"  << "top15,child3"
                                                                            << "top15,child8"
                                                                                                                  << "top15,child14" << "top15,child15"
                                                                            << "top15,child16"
                                                                << "top16"  << "top16,child0"  << "top16,child1"    << "top16,child2"  << "top16,child3"
                                                                            << "top16,child8"
                                                                                                                  << "top16,child14" << "top16,child15"
                                           << "top16,child16");

    BOBUIest::newRow("Match HasChildren") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::HasChildren)
                                       << (QStringList() << "top0" << "top1" << "top2" << "top3" << "top4" << "top5"
                                           << "top6" << "top7" << "top8" << "top9" << "top10" << "top11" << "top12"
                                           << "top13" << "top14" << "top15" << "top16");

    BOBUIest::newRow("Match Checked") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Checked)
                                   << (QStringList()
                                                                            << "top0,child14"  << "top0,child16"
                                                                            << "top1,child14"  << "top1,child16"
                                                                            << "top2,child14"  << "top2,child16"
                                                                            << "top3,child14"  << "top3,child16"
                                                                            << "top4,child14"  << "top4,child16"
                                                                            << "top5,child14"  << "top5,child16"
                                                                            << "top6,child14"  << "top6,child16"
                                                                            << "top7,child14"  << "top7,child16"
                                                                            << "top8,child14"  << "top8,child16"
                                                                            << "top9,child14"  << "top9,child16"
                                                                            << "top10,child14" << "top10,child16"
                                                                            << "top11,child14" << "top11,child16"
                                                                            << "top12,child14" << "top12,child16"
                                                                            << "top13,child14" << "top13,child16"
                                                                << "top14"
                                                                            << "top14,child14" << "top14,child16"
                                                                            << "top15,child14" << "top15,child16"
                                                                << "top16"
                                       << "top16,child14" << "top16,child16");

    BOBUIest::newRow("Match NotChecked") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::NotChecked)
                                      << (QStringList()
                                                                << "top0"   << "top0,child0"  << "top0,child1"  << "top0,child2"  << "top0,child3"
                                                                            << "top0,child4"  << "top0,child5"  << "top0,child6"  << "top0,child7"
                                                                            << "top0,child8"  << "top0,child9"  << "top0,child10" << "top0,child11"
                                                                            << "top0,child12" << "top0,child13" <<                   "top0,child15"

                                                                << "top1"   << "top1,child0" << "top1,child1"  << "top1,child2"  << "top1,child3"
                                                                            << "top1,child4"  << "top1,child5"  << "top1,child6"  << "top1,child7"
                                                                            << "top1,child8"  << "top1,child9"  << "top1,child10" << "top1,child11"
                                                                            << "top1,child12" << "top1,child13"                   << "top1,child15"

                                                                << "top2"   << "top2,child0" << "top2,child1"  << "top2,child2"  << "top2,child3"
                                                                            << "top2,child4"  << "top2,child5"  << "top2,child6"  << "top2,child7"
                                                                            << "top2,child8"  << "top2,child9"  << "top2,child10" << "top2,child11"
                                                                            << "top2,child12" << "top2,child13"                   << "top2,child15"

                                                                << "top3"   << "top3,child0" << "top3,child1"  << "top3,child2"  << "top3,child3"
                                                                            << "top3,child4"  << "top3,child5"  << "top3,child6"  << "top3,child7"
                                                                            << "top3,child8"  << "top3,child9"  << "top3,child10" << "top3,child11"
                                                                            << "top3,child12" << "top3,child13"                   << "top3,child15"

                                                                << "top4"   << "top4,child0" << "top4,child1"  << "top4,child2"  << "top4,child3"
                                                                            << "top4,child4"  << "top4,child5"  << "top4,child6"  << "top4,child7"
                                                                            << "top4,child8"  << "top4,child9"  << "top4,child10" << "top4,child11"
                                                                            << "top4,child12" << "top4,child13"                   << "top4,child15"

                                                                << "top5"   << "top5,child0" << "top5,child1"  << "top5,child2"  << "top5,child3"
                                                                            << "top5,child4"  << "top5,child5"  << "top5,child6"  << "top5,child7"
                                                                            << "top5,child8"  << "top5,child9"  << "top5,child10" << "top5,child11"
                                                                            << "top5,child12" << "top5,child13"                   << "top5,child15"

                                                                << "top6"   << "top6,child0" << "top6,child1"  << "top6,child2"  << "top6,child3"
                                                                            << "top6,child4"  << "top6,child5"  << "top6,child6"  << "top6,child7"
                                                                            << "top6,child8"  << "top6,child9"  << "top6,child10" << "top6,child11"
                                                                            << "top6,child12" << "top6,child13"                   << "top6,child15"

                                                                << "top7"   << "top7,child0" << "top7,child1"  << "top7,child2"  << "top7,child3"
                                                                            << "top7,child4"  << "top7,child5"  << "top7,child6"  << "top7,child7"
                                                                            << "top7,child8"  << "top7,child9"  << "top7,child10" << "top7,child11"
                                                                            << "top7,child12" << "top7,child13"                   << "top7,child15"

                                                                << "top8"   << "top8,child0" << "top8,child1"  << "top8,child2"  << "top8,child3"
                                                                            << "top8,child4"  << "top8,child5"  << "top8,child6"  << "top8,child7"
                                                                            << "top8,child8"  << "top8,child9"  << "top8,child10" << "top8,child11"
                                                                            << "top8,child12" << "top8,child13"                   << "top8,child15"

                                                                << "top9"   << "top9,child0" << "top9,child1"  << "top9,child2"  << "top9,child3"
                                                                            << "top9,child4"  << "top9,child5"  << "top9,child6"  << "top9,child7"
                                                                            << "top9,child8"  << "top9,child9"  << "top9,child10" << "top9,child11"
                                                                            << "top9,child12" << "top9,child13"                   << "top9,child15"

                                                                << "top10"  << "top10,child0" << "top10,child1"  << "top10,child2"  << "top10,child3"
                                                                            << "top10,child4"  << "top10,child5"  << "top10,child6"  << "top10,child7"
                                                                            << "top10,child8"  << "top10,child9"  << "top10,child10" << "top10,child11"
                                                                            << "top10,child12" << "top10,child13"                   << "top10,child15"

                                                                << "top11"  << "top11,child0" << "top11,child1"  << "top11,child2"  << "top11,child3"
                                                                            << "top11,child4"  << "top11,child5"  << "top11,child6"  << "top11,child7"
                                                                            << "top11,child8"  << "top11,child9"  << "top11,child10" << "top11,child11"
                                                                            << "top11,child12" << "top11,child13"                   << "top11,child15"

                                                                << "top12"  << "top12,child0" << "top12,child1"  << "top12,child2"  << "top12,child3"
                                                                            << "top12,child4"  << "top12,child5"  << "top12,child6"  << "top12,child7"
                                                                            << "top12,child8"  << "top12,child9"  << "top12,child10" << "top12,child11"
                                                                            << "top12,child12" << "top12,child13"                   << "top12,child15"

                                                                << "top13"  << "top13,child0" << "top13,child1"  << "top13,child2"  << "top13,child3"
                                                                            << "top13,child4"  << "top13,child5"  << "top13,child6"  << "top13,child7"
                                                                            << "top13,child8"  << "top13,child9"  << "top13,child10" << "top13,child11"
                                                                            << "top13,child12" << "top13,child13"                   << "top13,child15"

                                                                /* "top14"*/<< "top14,child0" << "top14,child1"  << "top14,child2"  << "top14,child3"
                                                                            << "top14,child4"  << "top14,child5"  << "top14,child6"  << "top14,child7"
                                                                            << "top14,child8"  << "top14,child9"  << "top14,child10" << "top14,child11"
                                                                            << "top14,child12" << "top14,child13"                   << "top14,child15"

                                                                << "top15"  << "top15,child0" << "top15,child1"  << "top15,child2"  << "top15,child3"
                                                                            << "top15,child4"  << "top15,child5"  << "top15,child6"  << "top15,child7"
                                                                            << "top15,child8"  << "top15,child9"  << "top15,child10" << "top15,child11"
                                                                            << "top15,child12" << "top15,child13"                   << "top15,child15"

                                                                /* "top16"*/<< "top16,child0" << "top16,child1"  << "top16,child2"  << "top16,child3"
                                                                            << "top16,child4"  << "top16,child5"  << "top16,child6"  << "top16,child7"
                                                                            << "top16,child8"  << "top16,child9"  << "top16,child10" << "top16,child11"
                                          << "top16,child12" << "top16,child13" << "top16,child15");



    BOBUIest::newRow("Match Disabled") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Disabled)
                                    << (QStringList()
                                                                            << "top0,child11"
                                                                            << "top1,child11"
                                                                            << "top2,child11"
                                                                            << "top3,child11"
                                                                            << "top4,child11"
                                                                            << "top5,child11"
                                                                            << "top6,child11"
                                                                            << "top7,child11"
                                                                            << "top8,child11"
                                                                            << "top9,child11"
                                                                            << "top10,child11"
                                                                << "top11"
                                                                            << "top11,child0"
                                                                            << "top11,child1"
                                                                            << "top11,child2"
                                                                            << "top11,child3"
                                                                            << "top11,child4"
                                                                            << "top11,child5"
                                                                            << "top11,child6"
                                                                            << "top11,child7"
                                                                            << "top11,child8"
                                                                            << "top11,child9"
                                                                            << "top11,child10"
                                                                            << "top11,child11"
                                                                            << "top11,child12"
                                                                            << "top11,child13"
                                                                            << "top11,child14"
                                                                            << "top11,child15"
                                                                            << "top11,child16"

                                                                            << "top12,child11"
                                                                            << "top13,child11"
                                                                            << "top14,child11"
                                                                            << "top15,child11"
                                        << "top16,child11");

    BOBUIest::newRow("Match Editable") << 0 << BOBUIreeWidgetItemIterator::IteratorFlags(BOBUIreeWidgetItemIterator::Editable)
                                    << (QStringList()
                                                                            << "top0,child12"
                                                                            << "top1,child12"
                                                                            << "top2,child12"
                                                                            << "top3,child12"
                                                                            << "top4,child12"
                                                                            << "top5,child12"
                                                                            << "top6,child12"
                                                                            << "top7,child12"
                                                                            << "top8,child12"
                                                                            << "top9,child12"
                                                                            << "top10,child12"
                                                                            << "top11,child12"
                                                                << "top12"
                                                                            << "top12,child12"
                                                                            << "top13,child12"
                                                                            << "top14,child12"
                                                                            << "top15,child12"
                                        << "top16,child12");

    BOBUIest::newRow("Match mutually exclusive Hidden|NotHidden") << 0 << (BOBUIreeWidgetItemIterator::Hidden|BOBUIreeWidgetItemIterator::NotHidden)
                                                               << QStringList();
    BOBUIest::newRow("Match mutually exclusive Selected|Unselected") << 0 << (BOBUIreeWidgetItemIterator::Selected|BOBUIreeWidgetItemIterator::Unselected)
                                                                  << QStringList();
    BOBUIest::newRow("Match mutually exclusive Selectable|NotSelectable") << 0 << (BOBUIreeWidgetItemIterator::Selectable|BOBUIreeWidgetItemIterator::NotSelectable)
                                                                       << QStringList();
    BOBUIest::newRow("Match mutually exclusive DragEnabled|DragDisabled") << 0 << (BOBUIreeWidgetItemIterator::DragEnabled|BOBUIreeWidgetItemIterator::DragDisabled)
                                                                       << QStringList();
    BOBUIest::newRow("Match mutually exclusive DropEnabled|DropDisabled") << 0 << (BOBUIreeWidgetItemIterator::DropEnabled|BOBUIreeWidgetItemIterator::DropDisabled)
                                                                       << QStringList();
    BOBUIest::newRow("Match mutually exclusive HasChildren|NoChildren") << 0 << (BOBUIreeWidgetItemIterator::HasChildren|BOBUIreeWidgetItemIterator::NoChildren)
                                                                     << QStringList();
    BOBUIest::newRow("Match mutually exclusive Checked|NotChecked") << 0 << (BOBUIreeWidgetItemIterator::Checked|BOBUIreeWidgetItemIterator::NotChecked)
                                                                 << QStringList();
    BOBUIest::newRow("Match mutually exclusive Disabled|Enabled") << 0 << (BOBUIreeWidgetItemIterator::Disabled|BOBUIreeWidgetItemIterator::Enabled)
                                                               << QStringList();
    BOBUIest::newRow("Match mutually exclusive Editable|NotEditable") << 0 << (BOBUIreeWidgetItemIterator::Editable|BOBUIreeWidgetItemIterator::NotEditable)
                                                                   << QStringList();
}

void tst_BOBUIreeWidgetItemIterator::iteratorflags()
{
    QFETCH(int, start);
    QFETCH(BOBUIreeWidgetItemIterator::IteratorFlags, iteratorflags);
    QFETCH(QStringList, matches);

    BOBUIreeWidgetItemIterator it(testWidget, iteratorflags);
    it += start;
    int iMatch = 0;
    while (*it && iMatch < matches.size()) {
        BOBUIreeWidgetItem *item = *it;
        QCOMPARE(item->text(0), matches[iMatch]);
        ++it;
        ++iMatch;
    }
    // Make sure the expected result does not contain *more* items than the actual result.
    QCOMPARE(iMatch, matches.size());
}

void tst_BOBUIreeWidgetItemIterator::preincrement()
{
    BOBUIreeWidgetItemIterator it(testWidget, BOBUIreeWidgetItemIterator::All);
    BOBUIreeWidgetItem *item = *(++it);
    // should be the second one
    QCOMPARE(item->text(0), QString("top0,child0"));
}

void tst_BOBUIreeWidgetItemIterator::postincrement()
{
    BOBUIreeWidgetItemIterator it(testWidget, BOBUIreeWidgetItemIterator::All);
    BOBUIreeWidgetItem *item = *(it++);
    // should be the first one
    QCOMPARE(item->text(0), QString("top0"));
}

void tst_BOBUIreeWidgetItemIterator::predecrement()
{
    BOBUIreeWidgetItemIterator it(testWidget, BOBUIreeWidgetItemIterator::All);
    BOBUIreeWidgetItem *item = *(++it);
    // should be the second one
    QCOMPARE(item->text(0), QString("top0,child0"));

    item = *(--it);
    QCOMPARE(item->text(0), QString("top0"));

}

void tst_BOBUIreeWidgetItemIterator::postdecrement()
{
    BOBUIreeWidgetItemIterator it(testWidget, BOBUIreeWidgetItemIterator::All);
    BOBUIreeWidgetItem *item = *(it++);
    // should be the first one
    QCOMPARE(item->text(0), QString("top0"));

    //Iterator points to second one
    item = *(it--);
    QCOMPARE(item->text(0), QString("top0,child0"));

}

void tst_BOBUIreeWidgetItemIterator::plus_eq_data()
{
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<int>("addition");
    BOBUIest::addColumn<BOBUIreeWidgetItemIterator::IteratorFlag>("iteratorflags");
    BOBUIest::addColumn<QString>("expecteditem");

    BOBUIest::newRow("+=0") << 0 << 0 << BOBUIreeWidgetItemIterator::All << QString("top0");
    BOBUIest::newRow("+=1") << 0 << 1 << BOBUIreeWidgetItemIterator::All << QString("top0,child0");
    BOBUIest::newRow("+=2") << 0 << 2 << BOBUIreeWidgetItemIterator::All << QString("top0,child1");
    BOBUIest::newRow("+=(-1)") << 1 << -1 << BOBUIreeWidgetItemIterator::All << QString("top0");
    BOBUIest::newRow("+=(-2)") << 3 << -2 << BOBUIreeWidgetItemIterator::All << QString("top0,child0");
}

void tst_BOBUIreeWidgetItemIterator::plus_eq()
{
    QFETCH(int, start);
    QFETCH(int, addition);
    QFETCH(BOBUIreeWidgetItemIterator::IteratorFlag, iteratorflags);
    QFETCH(QString, expecteditem);

    BOBUIreeWidgetItemIterator it(testWidget, iteratorflags);
    it += start;
    it += addition;
    BOBUIreeWidgetItem *item = *it;

    QVERIFY(item);
    QCOMPARE(item->text(0), expecteditem);

}

void tst_BOBUIreeWidgetItemIterator::minus_eq_data()
{
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<int>("subtraction");
    BOBUIest::addColumn<BOBUIreeWidgetItemIterator::IteratorFlag>("iteratorflags");
    BOBUIest::addColumn<QString>("expecteditem");

    BOBUIest::newRow("0-=0") << 0 << 0 << BOBUIreeWidgetItemIterator::All << QString("top0");
    BOBUIest::newRow("2-=1") << 2 << 1 << BOBUIreeWidgetItemIterator::All << QString("top0,child0");
    BOBUIest::newRow("4-=2") << 4 << 2 << BOBUIreeWidgetItemIterator::All << QString("top0,child1");
    BOBUIest::newRow("0-=(-1)") << 0 << -1 << BOBUIreeWidgetItemIterator::All << QString("top0,child0");
    BOBUIest::newRow("0-=(-2)") << 0 << -2 << BOBUIreeWidgetItemIterator::All << QString("top0,child1");
    BOBUIest::newRow("18-=1") << 18 << 1 << BOBUIreeWidgetItemIterator::All << QString("top0,child16");
    BOBUIest::newRow("1-=1") << 1 << 1 << BOBUIreeWidgetItemIterator::All << QString("top0");
}

void tst_BOBUIreeWidgetItemIterator::minus_eq()
{
    QFETCH(int, start);
    QFETCH(int, subtraction);
    QFETCH(BOBUIreeWidgetItemIterator::IteratorFlag, iteratorflags);
    QFETCH(QString, expecteditem);

    BOBUIreeWidgetItemIterator it(testWidget, iteratorflags);
    it += start;
    it -= subtraction;
    BOBUIreeWidgetItem *item = *it;
    // should be the first one
    QVERIFY(item);
    QCOMPARE(item->text(0), expecteditem);
}

void tst_BOBUIreeWidgetItemIterator::updateIfModifiedFromWidget_data()
{
    BOBUIest::addColumn<int>("topLevelItems");
    BOBUIest::addColumn<int>("childItems");
    BOBUIest::addColumn<int>("grandChildItems");
    BOBUIest::addColumn<BOBUIreeWidgetItemIterator::IteratorFlag>("iteratorflags");
    BOBUIest::addColumn<int>("removeindex");
    BOBUIest::addColumn<int>("expecteditemindex");
    BOBUIest::addColumn<QString>("expecteditemvalue");
    BOBUIest::addColumn<QString>("expectedUpdatedCurrent");
    BOBUIest::addColumn<int>("expecteditemIsNull");

    BOBUIest::newRow("Remove 3, check 1") << 3 << 3 << 0 << BOBUIreeWidgetItemIterator::All
                << 3 << 1 << QString("top0,child0") << QString("top1") << 0;
    BOBUIest::newRow("Remove 1, check 0") << 3 << 3 << 0 << BOBUIreeWidgetItemIterator::All
                << 1 << 0 << QString("top0") << QString("top0,child1") << 0;
    BOBUIest::newRow("Remove 2, check 2") << 3 << 3 << 0 << BOBUIreeWidgetItemIterator::All
                << 2 << 2 << QString("top0,child2") << QString("top0,child2") << 0;
    BOBUIest::newRow("Remove 0, check 0") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 0 << 0 << QString("top1") << QString("top1") << 0;
    BOBUIest::newRow("Remove top1, check top1") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 13 << 13 << QString("top2") << QString("top2") << 0;
    BOBUIest::newRow("Remove top0, check top1") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 0 << 13 << QString("top1") << QString("top1") << 0;
    BOBUIest::newRow("Remove (top0,child1), check (top0,child1)") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 5 << 5 << QString("top0,child2") << QString("top0,child2") << 0;
    BOBUIest::newRow("Remove (t0,c0) check (t0,c0)") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 1 << 1 << QString("top0,child1") << QString("top0,child1") << 0;
    BOBUIest::newRow("Remove (t0,c1) check (t0,c1)") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 5 << 5 << QString("top0,child2") << QString("top0,child2") << 0;
    BOBUIest::newRow("Remove (t0) check (t0,c1)") << 3 << 3 << 0 << BOBUIreeWidgetItemIterator::All
                << 0 << 4 << QString("top1") << QString("top1") << 0;
    BOBUIest::newRow("Remove (t0) check (t0,c0,g1)") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 0 << 3 << QString("top1") << QString("top1") << 0;
    BOBUIest::newRow("Remove (top2), check if top2 is null") << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All
                << 2*13 << 2*13 << QString() << QString() << 1;
    BOBUIest::newRow("Remove last item, check if iterator::current returns 0")
                << 3 << 0 << 0 << BOBUIreeWidgetItemIterator::All << 2 << 2 << QString() << QString() << 1;
    BOBUIest::newRow("remove 1, iterator points to 3, should move to 1")
                << 3 << 3 << 3 << BOBUIreeWidgetItemIterator::All << 1 << 3 << QString("top0,child1") << QString("top0,child1") << 0;
}

static void populate3Levels(BOBUIreeWidget &tw, int topLevelItems, int childItems, int grandChildItems)
{
    for (int i1 = 0; i1 < topLevelItems; ++i1) {
        BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(&tw);
        const QString top1S = QLatin1String("top") + QString::number(i1);
        top->setText(0, top1S);
        for (int i2 = 0; i2 < childItems; ++i2) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(top);
            const QString childS = top1S + QLatin1String(",child") + QString::number(i2);
            child->setText(0, childS);
            for (int i3 = 0; i3 < grandChildItems; ++i3) {
                BOBUIreeWidgetItem *grandChild = new BOBUIreeWidgetItem(child);
                grandChild->setText(0, childS + QLatin1String(",grandchild") + QString::number(i3));
            }
        }
    }
}

void tst_BOBUIreeWidgetItemIterator::updateIfModifiedFromWidget()
{
    QFETCH(int, topLevelItems);
    QFETCH(int, childItems);
    QFETCH(int, grandChildItems);
    QFETCH(BOBUIreeWidgetItemIterator::IteratorFlag, iteratorflags);
    QFETCH(int, removeindex);
    QFETCH(int, expecteditemindex);
    QFETCH(QString, expecteditemvalue);
    QFETCH(QString, expectedUpdatedCurrent);
    QFETCH(int, expecteditemIsNull);

    BOBUIreeWidget tw;
    tw.clear();
    tw.setColumnCount(2);
    populate3Levels(tw, topLevelItems, childItems, grandChildItems);

    BOBUIreeWidgetItemIterator it(&tw, iteratorflags);
    it+=expecteditemindex;
    BOBUIreeWidgetItemIterator itRemove(&tw, BOBUIreeWidgetItemIterator::IteratorFlags(iteratorflags));
    itRemove+=removeindex;
    BOBUIreeWidgetItem *item = *itRemove;
    QVERIFY(item);
    delete item;
    item = *it;
    if (expecteditemIsNull) {
        QVERIFY(!item);
    } else {
        QVERIFY(item);
        QCOMPARE(item->text(0), expecteditemvalue);
        item = *itRemove;
        if (expectedUpdatedCurrent.isNull())
            QVERIFY(!item);
        else
            QCOMPARE(item->text(0), expectedUpdatedCurrent);
    }
}

void tst_BOBUIreeWidgetItemIterator::updateIteratorAfterDeletedItem_and_ContinueIteration_data()
{
    BOBUIest::addColumn<int>("topLevelItems");
    BOBUIest::addColumn<int>("childItems");
    BOBUIest::addColumn<int>("grandChildItems");       // Populate the tree data
    // we have one iterator pointing to an item in the tree.
    // This iterator will be updated if we delete the item it is pointing to.
    //
    BOBUIest::addColumn<int>("removeindex");                   // The index of the node we want to remove
    BOBUIest::addColumn<int>("iterator_initial_index");        // The new expected index of
    BOBUIest::addColumn<int>("iterator_advance_after_removal");
    BOBUIest::addColumn<QString>("iterator_new_value");        // The new current item value of the iterator
    BOBUIest::newRow("Remove 13, it points to 25, it-=1. We should get top0,child2,grandchild2") << 3 << 3 << 3 << 13 << 25 << -1 << QString("top0,child2,grandchild2");
    BOBUIest::newRow("Remove 0, it points to 12, it+=1. We should get top1,child0") << 3 << 3 << 3 << 0 << 12 << 1 << QString("top1,child0");
    BOBUIest::newRow("Remove 0, it points to 12, it-=1. We should get 0")   << 3 << 3 << 3 << 0 << 12 << -1 << QString();
    BOBUIest::newRow("Remove 0, it points to 1, it+=1. We should get top2") << 4 << 0 << 0 << 0 << 1 << 1 << QString("top2");
    BOBUIest::newRow("Remove 2, it points to 1, it+=0. We should get top1") << 4 << 0 << 0 << 2 << 1 << 0 << QString("top1");
    BOBUIest::newRow("Remove 2, it points to 1, it+=1. We should get top3") << 4 << 0 << 0 << 2 << 1 << 1 << QString("top3");
    BOBUIest::newRow("Remove 1, it points to 2, it+=1. We should get top0,child2") << 3 << 3 << 0 << 1 << 2 << 1 << QString("top0,child2");
    BOBUIest::newRow("Remove 1, it points to 2, it+=0. We should get top0,child1") << 3 << 3 << 0 << 1 << 2 << 0 << QString("top0,child1");
    BOBUIest::newRow("Remove 1, it points to 2, it-=1. We should get top0") << 3 << 3 << 0 << 1 << 2 << -1 << QString("top0");
}

void tst_BOBUIreeWidgetItemIterator::updateIteratorAfterDeletedItem_and_ContinueIteration()
{
    QFETCH(int, topLevelItems);
    QFETCH(int, childItems);
    QFETCH(int, grandChildItems);
    QFETCH(int, removeindex);
    QFETCH(int, iterator_initial_index);
    QFETCH(int, iterator_advance_after_removal);
    QFETCH(QString, iterator_new_value);

    BOBUIreeWidget tw;
    tw.clear();
    tw.setColumnCount(2);
    populate3Levels(tw, topLevelItems, childItems, grandChildItems);

    BOBUIreeWidgetItemIterator it(&tw, BOBUIreeWidgetItemIterator::All);
    it += iterator_initial_index;
    BOBUIreeWidgetItemIterator itRemove(&tw, BOBUIreeWidgetItemIterator::All);
    itRemove+=removeindex;
    BOBUIreeWidgetItem *item = *itRemove;
    QVERIFY(item);
    delete item;
    it+=iterator_advance_after_removal;
    if (iterator_new_value.isNull())
        QCOMPARE((*it), nullptr);
    else
        QCOMPARE((*it)->text(0), iterator_new_value);
}

void tst_BOBUIreeWidgetItemIterator::constructIteratorWithItem_data()
{
    BOBUIest::addColumn<int>("indextoitem");
    BOBUIest::addColumn<BOBUIreeWidgetItemIterator::IteratorFlag>("iteratorflags");
    BOBUIest::addColumn<QString>("expecteditem");

    BOBUIest::newRow("index 0")            << 0 << BOBUIreeWidgetItemIterator::All << QString("top0");
    BOBUIest::newRow("index 1")            << 1 << BOBUIreeWidgetItemIterator::All << QString("top0,child0");
    BOBUIest::newRow("index 2")            << 2 << BOBUIreeWidgetItemIterator::All << QString("top0,child1");
    BOBUIest::newRow("index 30")           << 30 << BOBUIreeWidgetItemIterator::All << QString("top1,child11");
    BOBUIest::newRow("305 (last item)")    << 305 << BOBUIreeWidgetItemIterator::All << QString("top16,child16");
    BOBUIest::newRow("index 0, advance to next matching node") << 0 << BOBUIreeWidgetItemIterator::NotHidden << QString("top0,child1");
}

void tst_BOBUIreeWidgetItemIterator::constructIteratorWithItem()
{
    QFETCH(int, indextoitem);
    QFETCH(BOBUIreeWidgetItemIterator::IteratorFlag, iteratorflags);
    QFETCH(QString, expecteditem);

    BOBUIreeWidgetItemIterator it(testWidget);
    it+=indextoitem;
    BOBUIreeWidgetItem *item = *it;
    BOBUIreeWidgetItemIterator it2(item, BOBUIreeWidgetItemIterator::IteratorFlags(iteratorflags));
    BOBUIreeWidgetItem *item2 = *it2;

    QVERIFY(item2);
    QCOMPARE(item2->text(0), expecteditem);

}

void tst_BOBUIreeWidgetItemIterator::initializeIterator()
{
    BOBUIreeWidget tw;
    BOBUIreeWidgetItemIterator it(&tw);

    QCOMPARE((*it), nullptr);
}

void tst_BOBUIreeWidgetItemIterator::sortingEnabled()
{
    BOBUIreeWidget *tree = new BOBUIreeWidget;
    tree->setColumnCount(2);
    tree->headerItem()->setText(0, "Id");
    tree->headerItem()->setText(1, "Color");

    tree->setSortingEnabled(true);
    tree->sortByColumn(0, BobUI::AscendingOrder);

    BOBUIreeWidgetItem *second = new BOBUIreeWidgetItem;
    second->setText(0, "2");
    second->setText(1, "second");
    BOBUIreeWidgetItem *first = new BOBUIreeWidgetItem;
    first->setText(0, "1");
    first->setText(1, "first");

    tree->addTopLevelItem(second);
    tree->addTopLevelItem(first);

    BOBUIreeWidgetItemIterator it(tree);
    QCOMPARE(*it, first);
    ++it;
    QCOMPARE(*it, second);
}

BOBUIEST_MAIN(tst_BOBUIreeWidgetItemIterator)
#include "tst_bobuireewidgetitemiterator.moc"
