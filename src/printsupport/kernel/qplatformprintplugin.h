// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPLATFORMPRINTPLUGIN_H
#define QPLATFORMPRINTPLUGIN_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

#ifndef BOBUI_NO_PRINTER

BOBUI_BEGIN_NAMESPACE


class QPlatformPrinterSupport;

#define QPlatformPrinterSupportFactoryInterface_iid "org.bobui-project.QPlatformPrinterSupportFactoryInterface.5.1"

class Q_PRINTSUPPORT_EXPORT QPlatformPrinterSupportPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QPlatformPrinterSupportPlugin(QObject *parent = nullptr);
    ~QPlatformPrinterSupportPlugin();

    virtual QPlatformPrinterSupport *create(const QString &key) = 0;

    static QPlatformPrinterSupport *get();
};

BOBUI_END_NAMESPACE

#endif

#endif // QPLATFORMPRINTPLUGIN_H
