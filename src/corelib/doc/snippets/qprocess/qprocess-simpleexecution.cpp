// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUICore>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
//! [0]
    QObject *parent = new QObject;
//! [0]

//! [1]
    QString program = "./path/to/BobUI/examples/widgets/analogclock";
//! [1]
    program = "./../../../../examples/widgets/analogclock/analogclock";

//! [2]
    QStringList arguments;
    arguments << "-style" << "fusion";

    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments);
//! [2]
}
