// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFileSystemModel model;
    QWidget window;
    BOBUIreeView *tree = new BOBUIreeView(&window);
    tree->setMaximumSize(1000, 600);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(tree);

    window.setLayout(layout);
    model.setRootPath("");
    tree->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    tree->setModel(&model);

    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    tree->header()->setStretchLastSection(false);

    window.setWindowTitle(QObject::tr("Dir View"));
    tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    window.show();

    return app.exec();
}
