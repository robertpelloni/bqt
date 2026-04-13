// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qcoreapplication.h"
#include "private/qcoreapplication_p.h"
#include <private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

/*****************************************************************************
  QCoreApplication utility functions
 *****************************************************************************/
QString qAppFileName()
{
    // QCoreApplication::applicationFilePath() expects a canonical path
    static QString appFileName;
    if (appFileName.isEmpty()) {
        QCFType<CFURLRef> bundleURL(CFBundleCopyExecutableURL(CFBundleGetMainBundle()));
        if (bundleURL) {
            QCFString cfPath(CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle));
            if (cfPath)
                appFileName = cfPath;
        }
    }
    return appFileName;
}

BOBUI_END_NAMESPACE
