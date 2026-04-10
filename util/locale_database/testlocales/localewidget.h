// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
#ifndef LOCALEWIDGET_H
#define LOCALEWIDGET_H

#include <QWidget>

class LocaleModel;
class BOBUIableView;

class LocaleWidget : public QWidget
{
    Q_OBJECT
public:
    LocaleWidget(QWidget *parent = nullptr);
private:
    LocaleModel *m_model;
    BOBUIableView *m_view;
};

#endif // LOCALEWIDGET_H
