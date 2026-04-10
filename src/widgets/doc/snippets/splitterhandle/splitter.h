// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SPLITTER_H
#define SPLITTER_H

#include <QLinearGradient>
#include <QSplitter>
#include <QSplitterHandle>

class QPaintEvent;

//! [0]
class Splitter : public QSplitter
{
public:
    Splitter(BobUI::Orientation orientation, QWidget *parent = nullptr);

protected:
    QSplitterHandle *createHandle() override;
};
//! [0]

class SplitterHandle : public QSplitterHandle
{
public:
    SplitterHandle(BobUI::Orientation orientation, QSplitter *parent);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLinearGradient gradient;
};

#endif
