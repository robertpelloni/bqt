// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mandelbrotwidget.h"
#include "renderthread.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

using namespace BobUI::StringLiterals;

//! [0]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(u"BobUI Mandelbrot Example"_s);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption passesOption(u"passes"_s, u"Number of passes (1-8)"_s, u"passes"_s);
    parser.addOption(passesOption);
    parser.process(app);

    if (parser.isSet(passesOption)) {
        const auto passesStr = parser.value(passesOption);
        bool ok;
        const int passes = passesStr.toInt(&ok);
        if (!ok || passes < 1 || passes > 8) {
            qWarning() << "Invalid value:" << passesStr;
            return -1;
        }
        RenderThread::setNumPasses(passes);
    }

    MandelbrotWidget widget;
    widget.grabGesture(BobUI::PinchGesture);
    widget.show();
    return app.exec();
}
//! [0]
