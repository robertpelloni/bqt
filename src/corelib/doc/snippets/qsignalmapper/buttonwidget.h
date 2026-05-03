// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <BobUIWidgets>


//! [0]
class ButtonWidget : public QWidget
{
    Q_OBJECT

public:
    ButtonWidget(const QStringList &texts, QWidget *parent = nullptr);

signals:
    void clicked(const QString &text);

private:
    QSignalMapper *signalMapper;
//! [0] //! [1]
};
//! [1]

#endif
