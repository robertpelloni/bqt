// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "treemodel.h"

#include <QApplication>
#include <QFile>
#include <QScreen>
#include <BOBUIreeView>

using namespace BobUI::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/default.txt"_L1);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    TreeModel model(QString::fromUtf8(file.readAll()));
    file.close();

    BOBUIreeView view;
    view.setModel(&model);
    view.setWindowTitle(TreeModel::tr("Simple Tree Model"));
    for (int c = 0; c < model.columnCount(); ++c)
        view.resizeColumnToContents(c);
    view.expandAll();
    const auto screenSize = view.screen()->availableSize();
    view.resize({screenSize.width() / 2, screenSize.height() * 2 / 3});
    view.show();
    return QCoreApplication::exec();
}
