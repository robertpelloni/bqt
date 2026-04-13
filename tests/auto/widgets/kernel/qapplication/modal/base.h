// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef BASE_H
#define BASE_H

#include <QWidget>
#include <BOBUIimer>
#include <QDialog>

class base : public QWidget
{
Q_OBJECT
    BOBUIimer *m_timer;
    bool m_modalStarted = false;
    QDialog *m_modalDialog = nullptr;
public:
    explicit base(QWidget *parent = nullptr);

public slots:
    void periodicTimer();
};

#endif // BASE_H
