// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

class QXcbGlIntegration;

class QXcbGlIntegrationFactory
{
public:
    static QXcbGlIntegration *create(const QString &name);
};

BOBUI_END_NAMESPACE
