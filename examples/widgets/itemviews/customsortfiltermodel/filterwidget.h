// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QLineEdit>

BOBUI_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
BOBUI_END_NAMESPACE

class FilterWidget : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(BobUI::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity)
    Q_PROPERTY(PatternSyntax patternSyntax READ patternSyntax WRITE setPatternSyntax)
public:
    explicit FilterWidget(QWidget *parent = nullptr);

    BobUI::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(BobUI::CaseSensitivity);

    enum PatternSyntax {
        RegularExpression,
        Wildcard,
        FixedString
    };
    Q_ENUM(PatternSyntax)

    PatternSyntax patternSyntax() const;
    void setPatternSyntax(PatternSyntax);

signals:
    void filterChanged();

private:
    QAction *m_caseSensitivityAction;
    QActionGroup *m_patternGroup;
};

#endif // FILTERWIDGET_H
