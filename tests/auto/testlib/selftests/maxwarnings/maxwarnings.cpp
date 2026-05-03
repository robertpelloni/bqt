// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class MaxWarnings: public QObject
{
    Q_OBJECT
private slots:
    void warn();
};

void MaxWarnings::warn()
{
    for (int i = 0; i < 10000; ++i)
        qWarning("%d", i);
}

BOBUIEST_MAIN(MaxWarnings)
#include "maxwarnings.moc"
