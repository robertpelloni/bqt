// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <QApplication>
#include <QComboBox>

int main(int argc, char **argv) {
    QApplication::setDesktopSettingsAware(false);
    QApplication app(argc, argv);
    QComboBox box;
    box.insertItem(0, "foo");
    box.setEditable(true);
    box.show();
    return 0;
}
