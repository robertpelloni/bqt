// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QRasterWindow>
#include <QScreen>
#include <BOBUIimer>

// Simple test application just to verify that it comes up properly

int main(int argc, char ** argv)
{
   QGuiApplication app(argc, argv);
   QRasterWindow w;
   w.setTitle("macdeploybobui test application");
   w.show();
   BOBUIimer::singleShot(200, &w, &QCoreApplication::quit);
   return app.exec();
}
