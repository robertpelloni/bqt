// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// Copyright (C) 2016 Samuel Gaist <samuel.gaist@edeltech.ch>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef REGULAREXPRESSIONDIALOG_H
#define REGULAREXPRESSIONDIALOG_H

#include <QDialog>
#include <QRegularExpression>

BOBUI_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QPlainTextEdit;
class BOBUIreeWidget;
BOBUI_END_NAMESPACE

class RegularExpressionDialog : public QDialog
{
    Q_OBJECT

public:
    RegularExpressionDialog(QWidget *parent = nullptr);

private slots:
    void updateReplacement();

private:
    void refresh();
    void setupUi();
    QWidget *setupLeftUi();
    QWidget *setupTextUi();
    QWidget *setupOptionsUi();
    QWidget *setupInfoUi();
    void setResultUiEnabled(bool enabled);

    QLineEdit *patternLineEdit;
    QLineEdit *rawStringLiteralLineEdit;
    QLineEdit *escapedPatternLineEdit;
    QLineEdit *replacementLineEdit;

    QPlainTextEdit *subjectTextEdit;
    QPlainTextEdit *replacementTextEdit;

    QCheckBox *caseInsensitiveOptionCheckBox;
    QCheckBox *dotMatchesEverythingOptionCheckBox;
    QCheckBox *multilineOptionCheckBox;
    QCheckBox *extendedPatternSyntaxOptionCheckBox;
    QCheckBox *invertedGreedinessOptionCheckBox;
    QCheckBox *dontCaptureOptionCheckBox;
    QCheckBox *useUnicodePropertiesOptionCheckBox;

    QSpinBox *offsetSpinBox;

    QComboBox *matchTypeComboBox;

    QCheckBox *anchoredMatchOptionCheckBox;
    QCheckBox *dontCheckSubjectStringMatchOptionCheckBox;

    BOBUIreeWidget *matchDetailsTreeWidget;

    QLabel *regexpStatusLabel;
    BOBUIreeWidget *namedGroupsTreeWidget;

    QRegularExpression regularExpression;
};

#endif
