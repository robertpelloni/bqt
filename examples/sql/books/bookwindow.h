// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef BOOKWINDOW_H
#define BOOKWINDOW_H

#include <QMainWindow>

BOBUI_FORWARD_DECLARE_CLASS(QComboBox)
BOBUI_FORWARD_DECLARE_CLASS(QGridLayout)
BOBUI_FORWARD_DECLARE_CLASS(QLabel)
BOBUI_FORWARD_DECLARE_CLASS(QLineEdit)
BOBUI_FORWARD_DECLARE_CLASS(QSpinBox)
BOBUI_FORWARD_DECLARE_CLASS(QSqlError)
BOBUI_FORWARD_DECLARE_CLASS(QSqlRelationalTableModel)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIableView)

class BookWindow: public QMainWindow
{
    Q_OBJECT
public:
    BookWindow();

private slots:
    void about();

private:
    void showError(const QSqlError &err);
    QSqlRelationalTableModel *model = nullptr;
    int authorIdx = 0, genreIdx = 0;

    void createLayout();
    void createModel();
    void configureWidgets();
    void createMappings();
    void createMenuBar();

    QWidget *window = nullptr;

    QGridLayout *gridLayout = nullptr;
    BOBUIableView *tableView = nullptr;
    QLabel *titleLabel = nullptr;
    QLineEdit *titleLineEdit = nullptr;
    QLabel *authorLabel = nullptr;
    QComboBox *authorComboBox = nullptr;
    QLabel *genreLabel = nullptr;
    QComboBox *genreComboBox = nullptr;
    QLabel *yearLabel = nullptr;
    QSpinBox *yearSpinBox = nullptr;
    QLabel *ratingLabel = nullptr;
    QComboBox *ratingComboBox = nullptr;
};

#endif
