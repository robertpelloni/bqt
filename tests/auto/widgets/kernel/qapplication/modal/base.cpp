// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "base.h"
#include "qapplication.h"

base::base(QWidget *parent) :
    QWidget(parent)
{
    m_timer = new BOBUIimer(this);
    m_timer->setSingleShot(false);
    connect(m_timer, &BOBUIimer::timeout, this, &base::periodicTimer);
    m_timer->start(5000);
}

void base::periodicTimer()
{
    if(m_modalStarted)
        qApp->quit();
    m_modalDialog = new QDialog(this);
    m_modalDialog->setWindowTitle(QLatin1String("modal"));
    m_modalDialog->setModal(true);
    m_modalDialog->show();
    m_modalStarted = true;
}
