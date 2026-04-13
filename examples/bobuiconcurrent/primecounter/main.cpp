// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets/qapplication.h>
#include "primecounter.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("BobUIProject");
    app.setApplicationName(QApplication::translate("main", "Prime Counter"));

    PrimeCounter dialog;
    dialog.setWindowTitle(QApplication::translate("main", "Prime Counter"));
    dialog.show();

    return app.exec();
}
