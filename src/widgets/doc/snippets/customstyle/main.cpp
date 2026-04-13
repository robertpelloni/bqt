// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [using a custom style]
#include <BobUIWidgets>

#include "customstyle.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(new CustomStyle);
    QApplication app(argc, argv);
    QSpinBox spinBox;
    spinBox.show();
    return app.exec();
}
//! [using a custom style]
