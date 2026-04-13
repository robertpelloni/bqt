// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qoperatingsystemversion_p.h"

#import <Foundation/Foundation.h>

#include <BobUICore/qfile.h>
#include <BobUICore/qversionnumber.h>

#if !defined(BOBUI_BOOTSTRAPPED) && BOBUI_CONFIG(process)
#include <BobUICore/qprocess.h>
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QOperatingSystemVersionBase QOperatingSystemVersionBase::current_impl()
{
    NSOperatingSystemVersion osv = NSProcessInfo.processInfo.operatingSystemVersion;
    QVersionNumber versionNumber(osv.majorVersion, osv.minorVersion, osv.patchVersion);

    if (versionNumber.majorVersion() == 10 && versionNumber.minorVersion() >= 16) {
        // The process is running in system version compatibility mode,
        // due to the executable being built against a pre-macOS 11 SDK.
        // This might happen even if we require a more recent SDK for
        // building BobUI applications, as the BobUI 'app' might be a plugin
        // hosted inside a host that used an earlier SDK. But, since we
        // require a recent SDK for the BobUI app itself, the application
        // should be prepared for versions numbers beyond 10, and we can
        // resolve the real version number here.
#if !defined(BOBUI_BOOTSTRAPPED) && BOBUI_CONFIG(process)
        QProcess sysctl;
        QProcessEnvironment nonCompatEnvironment;
        nonCompatEnvironment.insert("SYSTEM_VERSION_COMPAT"_L1, "0"_L1);
        sysctl.setProcessEnvironment(nonCompatEnvironment);
        sysctl.start("/usr/sbin/sysctl"_L1, QStringList() << "-b"_L1 << "kern.osproductversion"_L1);
        if (sysctl.waitForFinished()) {
            auto versionString = QString::fromLatin1(sysctl.readAll());
            auto nonCompatSystemVersion = QVersionNumber::fromString(versionString);
            if (!nonCompatSystemVersion.isNull())
                versionNumber = nonCompatSystemVersion;
        }
#endif
    }

    QOperatingSystemVersionBase operatingSystemVersion;
    operatingSystemVersion.m_os = currentType();
    operatingSystemVersion.m_major = versionNumber.majorVersion();
    operatingSystemVersion.m_minor = versionNumber.minorVersion();
    operatingSystemVersion.m_micro = versionNumber.microVersion();

    return operatingSystemVersion;
}

BOBUI_END_NAMESPACE
