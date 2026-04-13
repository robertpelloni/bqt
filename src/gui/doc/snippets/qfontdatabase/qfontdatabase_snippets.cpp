// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

namespace qfontdatabase_snippets {
void wrapper()
{
//! [0]
BOBUIreeWidget fontTree;
fontTree.setColumnCount(2);
fontTree.setHeaderLabels(QStringList() << "Font" << "Smooth Sizes");

const QStringList fontFamilies = QFontDatabase::families();
for (const QString &family : fontFamilies) {
    BOBUIreeWidgetItem *familyItem = new BOBUIreeWidgetItem(&fontTree);
    familyItem->setText(0, family);

    const QStringList fontStyles = QFontDatabase::styles(family);
    for (const QString &style : fontStyles) {
        BOBUIreeWidgetItem *styleItem = new BOBUIreeWidgetItem(familyItem);
        styleItem->setText(0, style);

        QString sizes;
        const QList<int> smoothSizes = QFontDatabase::smoothSizes(family, style);
        for (const auto &points : smoothSizes)
            sizes += QString::number(points) + ' ';

        styleItem->setText(1, sizes.trimmed());
    }
}
//! [0]
} // wrapper
} // qfontdatabase_snippets
