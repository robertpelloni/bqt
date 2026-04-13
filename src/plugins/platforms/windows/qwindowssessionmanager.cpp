// Copyright (C) 2013 Samuel Gaist <samuel.gaist@edeltech.ch>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowssessionmanager.h"

BOBUI_BEGIN_NAMESPACE

QWindowsSessionManager::QWindowsSessionManager(const QString &id, const QString &key)
    : QPlatformSessionManager(id, key)
{
}

bool QWindowsSessionManager::allowsInteraction()
{
    m_blockUserInput = false;
    return true;
}

bool QWindowsSessionManager::allowsErrorInteraction()
{
    m_blockUserInput = false;
    return true;
}

void QWindowsSessionManager::release()
{
    if (m_isActive)
        m_blockUserInput = true;
}

void QWindowsSessionManager::cancel()
{
    m_canceled = true;
}

BOBUI_END_NAMESPACE
