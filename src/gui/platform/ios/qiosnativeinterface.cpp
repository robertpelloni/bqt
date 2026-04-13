// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/private/qguiapplication_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace QNativeInterface::Private;

#if defined(Q_OS_VISIONOS)

/*!
    \class QNativeInterface::QVisionOSApplication
    \since 6.8
    \internal
    \preliminary
    \brief Native interface to QGuiApplication, to be retrieved from QPlatformIntegration.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QVisionOSApplication);

#endif // Q_OS_VISIONOS

BOBUI_END_NAMESPACE
