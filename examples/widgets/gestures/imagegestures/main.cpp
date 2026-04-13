// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "mainwidget.h"

static void showHelp(QCommandLineParser &parser, const QString errorMessage = QString())
{
    QString text;
    BOBUIextStream str(&text);
    str << "<html><head/><body>";
    if (!errorMessage.isEmpty())
        str << "<p>" << errorMessage << "</p>";
    str << "<pre>" << parser.helpText() << "</pre></body></html>";
    QMessageBox box(errorMessage.isEmpty() ? QMessageBox::Information : QMessageBox::Warning,
                    QGuiApplication::applicationDisplayName(), text,
                    QMessageBox::Ok);
    box.setTextInteractionFlags(BobUI::TextBrowserInteraction);
    box.exec();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(BOBUI_VERSION_STR);
    QCoreApplication::setApplicationName(QStringLiteral("imagegestures"));
    QGuiApplication::setApplicationDisplayName(QStringLiteral("Image Gestures Example"));

    QCommandLineParser commandLineParser;
    const QCommandLineOption disablePanOption("no-pan", "Disable pan gesture");
    commandLineParser.addOption(disablePanOption);
    const QCommandLineOption disablePinchOption("no-pinch", "Disable pinch gesture");
    commandLineParser.addOption(disablePinchOption);
    const QCommandLineOption disableSwipeOption("no-swipe", "Disable swipe gesture");
    commandLineParser.addOption(disableSwipeOption);
    const QCommandLineOption helpOption = commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(QStringLiteral("Directory"),
                                            QStringLiteral("Directory to display"));

    const QString description = QGuiApplication::applicationDisplayName()
        + QLatin1String("\n\nEnable \"debug\" on the logging category \"bobui.examples.imagegestures\" in order to\n"
                        "in order to obtain verbose information about BobUI's gesture event processing,\n"
                        "for example by setting the environment variables BOBUI_LOGGING_RULES to\n"
                        "bobui.examples.imagegestures.debug=true\n");
    commandLineParser.setApplicationDescription(description);

    commandLineParser.process(QCoreApplication::arguments());

    QStringList arguments = commandLineParser.positionalArguments();
    if (!arguments.isEmpty() && !QFileInfo(arguments.front()).isDir()) {
        showHelp(commandLineParser,
                 QLatin1Char('"') + QDir::toNativeSeparators(arguments.front())
                 + QStringLiteral("\" is not a directory."));
        return -1;
    }

    QList<BobUI::GestureType> gestures;
    if (!commandLineParser.isSet(disablePanOption))
        gestures << BobUI::PanGesture;
    if (!commandLineParser.isSet(disablePinchOption))
        gestures << BobUI::PinchGesture;
    if (!commandLineParser.isSet(disableSwipeOption))
        gestures << BobUI::SwipeGesture;

    MainWidget w;
    w.grabGestures(gestures);
    w.show();

    if (arguments.isEmpty()) {
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        const QString directory =
            QFileDialog::getExistingDirectory(&w, "Select image folder",
                                              picturesLocations.isEmpty() ? QString() : picturesLocations.front());
        if (directory.isEmpty())
            return 0;
        arguments.append(directory);
    }

    w.openDirectory(arguments.front());

    return app.exec();
}
