// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QWidget>
#include <QStackedWidget>
#include <QMap>
#include <BOBUIabBar>
#include <QLabel>
#include <QLayout>
#include <BOBUIabWidget>
#include <QProxyStyle>
#include <qdebug.h>
#include "tabbarform.h"

class TabBarProxyStyle : public QProxyStyle
{
public:
    TabBarProxyStyle() : QProxyStyle(), alignment(BobUI::AlignLeft)
    { }

    int styleHint(StyleHint hint, const QStyleOption *option = 0,
                  const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const
    {
        if (hint == QStyle::SH_TabBar_Alignment)
            return alignment;

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }

    BobUI::Alignment alignment;
};

const int TabCount = 5;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto *proxyStyle = new TabBarProxyStyle;
    app.setStyle(proxyStyle);

    QWidget widget;
    QStackedWidget stackedWidget;
    BOBUIabBar tabBar;
    tabBar.setDocumentMode(true);
    tabBar.setTabsClosable(true);
    tabBar.setMovable(true);
    tabBar.setExpanding(false);

    // top
    tabBar.setShape(BOBUIabBar::RoundedNorth);
    // bottom
//    tabBar.setShape(BOBUIabBar::RoundedSouth);
    // left
//    tabBar.setShape(BOBUIabBar::RoundedWest);
    // right
//    tabBar.setShape(BOBUIabBar::RoundedEast);

    const auto shortLabel = QStringLiteral("Tab %1");
    const auto longLabel = QStringLiteral("An Extremely Long Tab Label %1");

    QMap<int, QWidget*> tabs;
    for (int i = 0; i < TabCount; i++) {
        QString tabNumberString = QString::number(i);
        QLabel *label = new QLabel(QStringLiteral("Tab %1 content").arg(tabNumberString));
        tabs[i] = label;
        label->setAlignment(BobUI::AlignCenter);
        stackedWidget.addWidget(label);
        tabBar.addTab(shortLabel.arg(tabNumberString));
    }

    QObject::connect(&tabBar, &BOBUIabBar::tabMoved, [&tabs](int from, int to) {
        QWidget *thisWidget = tabs[from];
        QWidget *thatWidget = tabs[to];
        tabs[from] = thatWidget;
        tabs[to] = thisWidget;
    });

    QObject::connect(&tabBar, &BOBUIabBar::currentChanged, [&stackedWidget, &tabs](int index) {
        if (index >= 0)
            stackedWidget.setCurrentWidget(tabs[index]);
    });

    QObject::connect(&tabBar, &BOBUIabBar::tabCloseRequested, [&stackedWidget, &tabBar, &tabs](int index) {
        QWidget *widget = tabs[index];
        tabBar.removeTab(index);
        for (int i = index + 1; i < TabCount; i++)
            tabs[i-1] = tabs[i];
        int currentIndex = tabBar.currentIndex();
        if (currentIndex >= 0)
            stackedWidget.setCurrentWidget(tabs[currentIndex]);
        delete widget;
    });

    QLayout *layout;
    switch (tabBar.shape()) {
    case BOBUIabBar::RoundedEast:
    case BOBUIabBar::TriangularEast:
        tabBar.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout = new QHBoxLayout(&widget);
        layout->addWidget(&stackedWidget);
        layout->addWidget(&tabBar);
        break;
    case BOBUIabBar::RoundedWest:
    case BOBUIabBar::TriangularWest:
        tabBar.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout = new QHBoxLayout(&widget);
        layout->addWidget(&tabBar);
        layout->addWidget(&stackedWidget);
        break;
    case BOBUIabBar::RoundedNorth:
    case BOBUIabBar::TriangularNorth:
        tabBar.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout = new QVBoxLayout(&widget);
        layout->addWidget(&tabBar);
        layout->addWidget(&stackedWidget);
        break;
    case BOBUIabBar::RoundedSouth:
    case BOBUIabBar::TriangularSouth:
        tabBar.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout = new QVBoxLayout(&widget);
        layout->addWidget(&stackedWidget);
        layout->addWidget(&tabBar);
        break;
    }

    TabBarForm form;
    layout->addWidget(&form);
    layout->setAlignment(&form, BobUI::AlignHCenter);

    form.ui->documentModeButton->setChecked(tabBar.documentMode());
    QObject::connect(form.ui->documentModeButton, &QCheckBox::toggled, [&] {
        tabBar.setDocumentMode(form.ui->documentModeButton->isChecked());
        // QMacStyle (and maybe other styles) requires a re-polish to get the right font
        QApplication::sendEvent(&tabBar, new QEvent(QEvent::ThemeChange));
    });

    form.ui->movableTabsButton->setChecked(tabBar.isMovable());
    QObject::connect(form.ui->movableTabsButton, &QCheckBox::toggled, [&] {
        tabBar.setMovable(form.ui->movableTabsButton->isChecked());
        tabBar.update();
    });

    form.ui->closableTabsButton->setChecked(tabBar.tabsClosable());
    QObject::connect(form.ui->closableTabsButton, &QCheckBox::toggled, [&] {
        tabBar.setTabsClosable(form.ui->closableTabsButton->isChecked());
        tabBar.update();
    });

    form.ui->expandingTabsButton->setChecked(tabBar.expanding());
    QObject::connect(form.ui->expandingTabsButton, &QCheckBox::toggled, [&] {
        tabBar.setExpanding(form.ui->expandingTabsButton->isChecked());
        tabBar.update();
    });

    form.ui->displayIconButton->setChecked(!tabBar.tabIcon(0).isNull());
    QObject::connect(form.ui->displayIconButton, &QCheckBox::toggled, [&] {
        const auto icon = form.ui->displayIconButton->isChecked() ?
                          tabBar.style()->standardIcon(QStyle::SP_ComputerIcon) : QIcon();
        for (int i = 0; i < tabBar.count(); i++)
            tabBar.setTabIcon(i, icon);
    });

    form.ui->longLabelButton->setChecked(false);
    QObject::connect(form.ui->longLabelButton, &QCheckBox::toggled, [&] {
        const auto &label = form.ui->longLabelButton->isChecked() ? longLabel : shortLabel;
        for (int i = 0; i < tabBar.count(); i++)
            tabBar.setTabText(i, label.arg(i));
    });

    QObject::connect(form.ui->shapeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        Q_UNUSED(index);
        // TODO
    });

    if (proxyStyle->alignment == BobUI::AlignLeft)
        form.ui->leftAlignedButton->setChecked(true);
    else if (proxyStyle->alignment == BobUI::AlignRight)
        form.ui->rightAlignedButton->setChecked(true);
    else
        form.ui->centeredButton->setChecked(true);
    QObject::connect(form.ui->textAlignmentGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [&](QAbstractButton *b) {
        proxyStyle->alignment = b == form.ui->leftAlignedButton ? BobUI::AlignLeft :
                                b == form.ui->rightAlignedButton ? BobUI::AlignRight : BobUI::AlignCenter;
        QApplication::sendEvent(&tabBar, new QEvent(QEvent::StyleChange));
    });

    layout->setContentsMargins(12, 12, 12, 12);
    widget.show();

    return app.exec();
}
