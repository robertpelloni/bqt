// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUICore>

// This test shows how to use asyncify to enable blocking the main
// thread on QEventLoop::exec(), while event processing continues.
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    BOBUIimer::singleShot(1000, []() {

        QEventLoop loop;
        BOBUIimer::singleShot(2000, [&loop]() {
            qDebug() << "Calling QEventLoop::quit()";
            loop.quit();
        });

        qDebug() << "Calling QEventLoop::exec()";
        loop.exec();
        qDebug() << "Returned from QEventLoop::exec()";
    });

    app.exec();
}
