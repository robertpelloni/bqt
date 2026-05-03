// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDARWINSECURITYSCOPEDFILEENGINE_H
#define QDARWINSECURITYSCOPEDFILEENGINE_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//
// We mean it.
//

#include <BobUICore/qurl.h>

Q_FORWARD_DECLARE_OBJC_CLASS(NSURL);

BOBUI_BEGIN_NAMESPACE

Q_CORE_EXPORT QUrl bobui_apple_urlFromPossiblySecurityScopedURL(NSURL *url);

BOBUI_END_NAMESPACE

#endif // QDARWINSECURITYSCOPEDFILEENGINE_H
