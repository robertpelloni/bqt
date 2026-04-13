// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QStyleOption>

#include <memory>

class tst_QStyleOption: public QObject
{
    Q_OBJECT

private slots:
    void qstyleoptioncast_data();
    void qstyleoptioncast();
};

void tst_QStyleOption::qstyleoptioncast_data()
{
    BOBUIest::addColumn<std::shared_ptr<QStyleOption>>("testOption");
    BOBUIest::addColumn<bool>("canCastToComplex");
    BOBUIest::addColumn<int>("type");

    // The shared_ptr ctor is templated; will always call the correct QStyleOption dtor
    using stylePtr = std::shared_ptr<QStyleOption>;

    BOBUIest::newRow("optionDefault") << stylePtr(new QStyleOption) << false << int(QStyleOption::SO_Default);
    BOBUIest::newRow("optionButton") << stylePtr(new QStyleOptionButton) << false << int(QStyleOption::SO_Button);
    BOBUIest::newRow("optionComboBox") << stylePtr(new QStyleOptionComboBox) << true << int(QStyleOption::SO_ComboBox);
    BOBUIest::newRow("optionComplex") << stylePtr(new QStyleOptionComplex) << true << int(QStyleOption::SO_Complex);
    BOBUIest::newRow("optionDockWidget") << stylePtr(new QStyleOptionDockWidget) << false << int(QStyleOption::SO_DockWidget);
    BOBUIest::newRow("optionFocusRect") << stylePtr(new QStyleOptionFocusRect) << false << int(QStyleOption::SO_FocusRect);
    BOBUIest::newRow("optionFrame") << stylePtr(new QStyleOptionFrame) << false << int(QStyleOption::SO_Frame);
    BOBUIest::newRow("optionHeader") << stylePtr(new QStyleOptionHeader) << false << int(QStyleOption::SO_Header);
    BOBUIest::newRow("optionMenuItem") << stylePtr(new QStyleOptionMenuItem) << false << int(QStyleOption::SO_MenuItem);
    BOBUIest::newRow("optionProgressBar") << stylePtr(new QStyleOptionProgressBar) << false << int(QStyleOption::SO_ProgressBar);
    BOBUIest::newRow("optionSlider") << stylePtr(new QStyleOptionSlider) << true << int(QStyleOption::SO_Slider);
    BOBUIest::newRow("optionSpinBox") << stylePtr(new QStyleOptionSpinBox) << true << int(QStyleOption::SO_SpinBox);
    BOBUIest::newRow("optionTab") << stylePtr(new QStyleOptionTab) << false << int(QStyleOption::SO_Tab);
    BOBUIest::newRow("optionTitleBar") << stylePtr(new QStyleOptionTitleBar) << true << int(QStyleOption::SO_TitleBar);
    BOBUIest::newRow("optionToolBox") << stylePtr(new QStyleOptionToolBox) << false << int(QStyleOption::SO_ToolBox);
    BOBUIest::newRow("optionToolButton") << stylePtr(new QStyleOptionToolButton) << true << int(QStyleOption::SO_ToolButton);
    BOBUIest::newRow("optionViewItem") << stylePtr(new QStyleOptionViewItem) << false << int(QStyleOption::SO_ViewItem);
    BOBUIest::newRow("optionGraphicsItem") << stylePtr(new QStyleOptionGraphicsItem) << false << int(QStyleOption::SO_GraphicsItem);
}

void tst_QStyleOption::qstyleoptioncast()
{
    QFETCH(const std::shared_ptr<QStyleOption>, testOption);
    QFETCH(bool, canCastToComplex);
    QFETCH(int, type);

    QCOMPARE_NE(testOption, nullptr);

    QCOMPARE_EQ(testOption->type, type);

    // Cast to common base class
    QStyleOption *castOption = qstyleoption_cast<QStyleOption*>(testOption.get());
    QVERIFY(castOption != nullptr);

    // Cast to complex base class
    castOption = qstyleoption_cast<QStyleOptionComplex*>(testOption.get());
    QCOMPARE(canCastToComplex, (castOption != nullptr));

    // Cast to combo box
    castOption = qstyleoption_cast<QStyleOptionComboBox*>(testOption.get());
    if (castOption)
        QCOMPARE_EQ(testOption->type, QStyleOption::SO_ComboBox);
    else
        QCOMPARE_NE(testOption->type, QStyleOption::SO_ComboBox);


    // Cast to button
    castOption = qstyleoption_cast<QStyleOptionButton*>(testOption.get());
    if (castOption)
        QCOMPARE_EQ(testOption->type, QStyleOption::SO_Button);
    else
        QCOMPARE_NE(testOption->type, QStyleOption::SO_Button);

    // Cast to lower version
    testOption->version += 1;
    castOption = qstyleoption_cast<QStyleOption*>(testOption.get());
    QVERIFY(castOption);

    // Cast a null pointer
    castOption = qstyleoption_cast<QStyleOption*>((QStyleOption*)0);
    QCOMPARE(castOption, nullptr);
}

BOBUIEST_MAIN(tst_QStyleOption)
#include "tst_qstyleoption.moc"

