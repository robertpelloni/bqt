// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSortFilterProxyModel;
class BOBUIreeView;
BOBUI_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    void setSourceModel(QAbstractItemModel *model);

private slots:
    void filterRegularExpressionChanged();
    void filterColumnChanged();
    void sortChanged();

private:
    QSortFilterProxyModel *proxyModel;

    QGroupBox *sourceGroupBox;
    QGroupBox *proxyGroupBox;
    BOBUIreeView *sourceView;
    BOBUIreeView *proxyView;
    QCheckBox *filterCaseSensitivityCheckBox;
    QCheckBox *sortCaseSensitivityCheckBox;
    QLabel *filterPatternLabel;
    QLabel *filterSyntaxLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterPatternLineEdit;
    enum Syntax {
        RegularExpression,
        Wildcard,
        FixedString
    };

    QComboBox *filterSyntaxComboBox;
    QComboBox *filterColumnComboBox;
};

#endif // WINDOW_H
