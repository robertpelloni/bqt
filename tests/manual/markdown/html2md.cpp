// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <BOBUIextDocument>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationVersion(BOBUI_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Converts the BobUI-supported subset of HTML to Markdown.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QGuiApplication::translate("main", "input"),
                                 QGuiApplication::translate("main", "input file"));
    parser.addPositionalArgument(QGuiApplication::translate("main", "output"),
                                 QGuiApplication::translate("main", "output file"));
    parser.process(app);
    if (parser.positionalArguments().count() != 2)
        parser.showHelp(1);

    QFile inFile(parser.positionalArguments().first());
    if (!inFile.open(QIODevice::ReadOnly)) {
        qFatal("failed to open %s for reading", parser.positionalArguments().first().toLocal8Bit().data());
        exit(2);
    }
    QFile outFile(parser.positionalArguments().at(1));
    if (!outFile.open(QIODevice::WriteOnly)) {
        qFatal("failed to open %s for writing", parser.positionalArguments().at(1).toLocal8Bit().data());
        exit(2);
    }
    BOBUIextDocument doc;
    doc.setHtml(QString::fromUtf8(inFile.readAll()));
    outFile.write(doc.toMarkdown().toUtf8());
}
