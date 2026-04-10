// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

import * as names from 'names.js';

function main() {
    startApplication("keypadnavigation");
    test.compare(waitForObjectExists(names.scrollAreaLineEditQLineEdit).focus, true);
    mouseClick(waitForObject(names.scrollAreaLabel7QLabel), 146, 24, BobUI.NoModifier, BobUI.LeftButton);
    type(waitForObject(names.scrollAreaLineEditQLineEdit), "<Tab>");
    test.compare(waitForObjectExists(names.scrollAreaHorizontalSliderQSlider).sliderPosition, 0);
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Tab>");
    scrollTo(waitForObject(names.scrollAreaHorizontalSliderQSlider), 0);
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Tab>");
    scrollTo(waitForObject(names.scrollAreaHorizontalSliderQSlider), 0);
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Tab>");
    type(waitForObject(names.scrollAreaDateEditQDateEdit), "<Tab>");
    scrollTo(waitForObject(names.scrollAreaHorizontalSliderQSlider), 0);
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Down>");
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Down>");
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Right>");
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Left>");
    type(waitForObject(names.scrollAreaHorizontalSliderQSlider), "<Tab>");
    test.compare(waitForObjectExists(names.scrollAreaBobUISpinboxLineeditQLineEdit).displayText, "1/1/00");
    test.compare(waitForObjectExists(names.scrollAreaBobUISpinboxLineeditQLineEdit).focus, true);
    mouseClick(waitForObject(names.scrollAreaScrollAreaWidgetContentsQWidget), 247, 183, BobUI.NoModifier, BobUI.LeftButton);
    type(waitForObject(names.scrollAreaDateEditQDateEdit), "<Tab>");
    type(waitForObject(names.scrollAreaDateEditQDateEdit), "<Tab>");
    type(waitForObject(names.scrollAreaDateEditQDateEdit), "<Tab>");
    test.compare(waitForObjectExists(names.scrollAreaComboBoxQComboBox).focus, true);
    scrollTo(waitForObject(names.scrollAreaQScrollBar), 186);
    type(waitForObject(names.scrollAreaComboBoxQComboBox), "<Tab>");
    test.compare(waitForObjectExists(names.scrollAreaPlainTextEditQPlainTextEdit).focus, true);
    clickButton(waitForObject(names.scrollAreaPushButtonQPushButton));
    test.compare(waitForObjectExists(names.scrollAreaPushButtonQPushButton).focus, true);
    type(waitForObject(names.scrollAreaPushButtonQPushButton), "<Tab>");
    test.compare(waitForObjectExists(names.groupBoxRadioButtonQRadioButton).checked, false);
    type(waitForObject(names.groupBoxRadioButtonQRadioButton), "<Tab>");
    test.compare(waitForObjectExists(names.groupBox2CheckBoxQCheckBox).checked, false);
    test.compare(waitForObjectExists(names.groupBox2CheckBoxQCheckBox).focus, true);
    type(waitForObject(names.groupBox2CheckBoxQCheckBox), "<Tab>");
    test.compare(waitForObjectExists(names.groupBox2CheckBox2QCheckBox).focus, true);
}
