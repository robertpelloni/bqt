// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qbaselinetest.h>
#include <qwidgetbaselinetest.h>
#include <BobUIWidgets>
#include <QByteArray>

class tst_Stylesheet : public QWidgetBaselineTest
{
    Q_OBJECT

public:
    tst_Stylesheet();

    void loadTestFiles();

    void doInit() override;

private slots:
    void tst_BOBUIoolButton_data();
    void tst_BOBUIoolButton();

    void tst_QScrollArea_data();
    void tst_QScrollArea();

    void tst_BOBUIreeView_data();
    void tst_BOBUIreeView();

    void tst_QHeaderView_data();
    void tst_QHeaderView();

    void tst_BOBUIableView_data();
    void tst_BOBUIableView();

    void tst_QListView_data();
    void tst_QListView();

private:
    QDir styleSheetDir;
};

tst_Stylesheet::tst_Stylesheet()
{
    QString baseDir = QFINDTESTDATA("qss/default.qss");
    styleSheetDir = QDir(QFileInfo(baseDir).path());
}

void tst_Stylesheet::doInit()
{
    QFETCH(QString, styleSheet);
    testWindow()->setStyleSheet(styleSheet);
}

void tst_Stylesheet::loadTestFiles()
{
    BOBUIest::addColumn<QString>("styleSheet");

    QStringList qssFiles;
    // first add generic test files
    for (const auto &qssFile : styleSheetDir.entryList({QStringLiteral("*.qss")}, QDir::Files | QDir::Readable))
        qssFiles << styleSheetDir.absoluteFilePath(qssFile);

    // then test-function specific files
    const QString testFunction = QString(BOBUIest::currentTestFunction()).remove("tst_").toLower();
    if (styleSheetDir.cd(testFunction)) {
        for (const auto &qssFile : styleSheetDir.entryList({QStringLiteral("*.qss")}, QDir::Files | QDir::Readable))
            qssFiles << styleSheetDir.absoluteFilePath(qssFile);
        styleSheetDir.cdUp();
    }

    for (const auto &qssFile : qssFiles) {
        QFileInfo fileInfo(qssFile);
        QFile file(qssFile);
        QVERIFY(file.open(QFile::ReadOnly));
        QString styleSheet = QString::fromUtf8(file.readAll());
        QBaselineTest::newRow(fileInfo.baseName().toUtf8()) << styleSheet;
    }
}

void tst_Stylesheet::tst_BOBUIoolButton_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_BOBUIoolButton()
{
    const QIcon trashIcon = QApplication::style()->standardIcon(QStyle::SP_TrashIcon);

    QVBoxLayout *vbox = new QVBoxLayout;

    QHBoxLayout *normalButtons = new QHBoxLayout;
    for (const auto &buttonStyle : {BobUI::ToolButtonIconOnly, BobUI::ToolButtonTextOnly,
                                    BobUI::ToolButtonTextUnderIcon, BobUI::ToolButtonTextBesideIcon}) {
        BOBUIoolButton *normal = new BOBUIoolButton;
        normal->setToolButtonStyle(buttonStyle);
        normal->setText("Norm");
        normal->setIcon(trashIcon);
        normalButtons->addWidget(normal);
    }
    vbox->addLayout(normalButtons);

    QHBoxLayout *arrowButtons = new QHBoxLayout;
    for (const auto &arrowType : {BobUI::LeftArrow, BobUI::RightArrow, BobUI::UpArrow, BobUI::DownArrow}) {
        BOBUIoolButton *arrow = new BOBUIoolButton;
        arrow->setText("Arrs");
        arrow->setArrowType(arrowType);
        arrowButtons->addWidget(arrow);
    }
    vbox->addLayout(arrowButtons);

    QHBoxLayout *arrowWithTextButtons = new QHBoxLayout;
    for (const auto &buttonStyle : {BobUI::ToolButtonTextOnly,
                                    BobUI::ToolButtonTextUnderIcon, BobUI::ToolButtonTextBesideIcon}) {
        BOBUIoolButton *arrow = new BOBUIoolButton;
        arrow->setText("ArrTxt");
        arrow->setArrowType(BobUI::UpArrow);
        arrow->setToolButtonStyle(buttonStyle);
        arrowWithTextButtons->addWidget(arrow);
    }
    vbox->addLayout(arrowWithTextButtons);

    QHBoxLayout *menuButtons = new QHBoxLayout;
    for (const auto &popupMode : {BOBUIoolButton::InstantPopup, BOBUIoolButton::MenuButtonPopup,
                                  BOBUIoolButton::DelayedPopup}) {
        BOBUIoolButton *menuButton = new BOBUIoolButton;
        menuButton->setText("PppMd");
        menuButton->setIcon(trashIcon);
        QMenu *menuButtonMenu = new QMenu;
        menuButtonMenu->addAction(QIcon(":/icons/align-left.png"), "Left");
        menuButtonMenu->addAction(QIcon(":/icons/align-right.png"), "Right");
        menuButtonMenu->addAction(QIcon(":/icons/align-center.png"), "Center");
        menuButton->setMenu(menuButtonMenu);
        menuButton->setPopupMode(popupMode);
        menuButtons->addWidget(menuButton);
    }
    vbox->addLayout(menuButtons);
    testWindow()->setLayout(vbox);

    makeVisible();
    QBASELINE_TEST(takeSnapshot());
}

void tst_Stylesheet::tst_QScrollArea_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_QScrollArea()
{
    QHBoxLayout *layout = new QHBoxLayout;
    BOBUIableWidget *table = new BOBUIableWidget(20, 20);
    layout->addWidget(table);
    testWindow()->setLayout(layout);

    makeVisible();
    QBASELINE_TEST(takeSnapshot());
}

void tst_Stylesheet::tst_BOBUIreeView_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_BOBUIreeView()
{
    QHBoxLayout *layout = new QHBoxLayout;
    BOBUIreeWidget *tw = new BOBUIreeWidget();
    tw->header()->hide();
    layout->addWidget(tw);

    enum {
        Unchecked           = 0,
        Checked             = 1,
        Children            = 2,
        Disabled            = 3,
        CheckedDisabled     = 4,
        ChildrenDisabled    = 5,
        NConfigs
    };

    for (int i = 0; i < NConfigs; ++i) {
        BOBUIreeWidgetItem *topLevelItem = new BOBUIreeWidgetItem(tw, QStringList{QString("top %1").arg(i)});
        switch (i) {
        case Unchecked:
        case Disabled:
            topLevelItem->setCheckState(0, BobUI::Unchecked);
            break;
        case Checked:
        case CheckedDisabled:
            topLevelItem->setCheckState(0, BobUI::Checked);
            break;
        case Children:
        case ChildrenDisabled:
            topLevelItem->setCheckState(0, BobUI::PartiallyChecked);
            topLevelItem->setExpanded(true);
            for (int j = 0; j < 2; ++j) {
                BOBUIreeWidgetItem *childItem = new BOBUIreeWidgetItem(topLevelItem, QStringList{QString("child %1").arg(j)});
                childItem->setCheckState(0, j % 2 ? BobUI::Unchecked : BobUI::Checked);
            }
            break;
        }
        topLevelItem->setDisabled(i >= Disabled);
    }
    testWindow()->setLayout(layout);
    tw->setRootIsDecorated(true);
    makeVisible();

    QBASELINE_CHECK_DEFERRED(takeSnapshot(), "rootDecorated");
    tw->setRootIsDecorated(false);
    QBASELINE_CHECK_DEFERRED(takeSnapshot(), "rootNotDecorated");

    tw->topLevelItem(Children)->child(0)->setSelected(true);
    QBASELINE_CHECK_DEFERRED(takeSnapshot(), "itemSelected");

    testWindow()->resize(testWindow()->size() * 2);
    QBASELINE_CHECK_DEFERRED(takeSnapshot(), "withEmptyArea");
}

void tst_Stylesheet::tst_QHeaderView_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_QHeaderView()
{
    QHBoxLayout *layout = new QHBoxLayout;
    BOBUIableWidget *tw = new BOBUIableWidget(10, 10);
    tw->setCurrentCell(1, 1);
    layout->addWidget(tw);
    testWindow()->setLayout(layout);
    makeVisible();
    QBASELINE_TEST(takeSnapshot());
}

void tst_Stylesheet::tst_BOBUIableView_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_BOBUIableView()
{
    QHBoxLayout *layout = new QHBoxLayout;
    BOBUIableWidget *tw = new BOBUIableWidget(2, 3);
    layout->addWidget(tw);
    testWindow()->setLayout(layout);
    makeVisible();
    QBASELINE_TEST(takeSnapshot());
}

void tst_Stylesheet::tst_QListView_data()
{
    loadTestFiles();
}

void tst_Stylesheet::tst_QListView()
{
    QStringListModel m({ "Berlin", "Paris", "London" });
    QListView *v = new QListView;
    v->setModel(&m);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(v);
    testWindow()->setLayout(layout);
    makeVisible();
    QBASELINE_TEST(takeSnapshot());
}

QBASELINETEST_MAIN(tst_Stylesheet)

#include "tst_baseline_stylesheet.moc"
