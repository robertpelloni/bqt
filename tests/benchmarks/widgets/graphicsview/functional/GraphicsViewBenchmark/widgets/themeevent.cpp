// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "themeevent.h"

ThemeEvent::ThemeEvent( const QString &newTheme, Type type) : QEvent(type),
    m_theme(newTheme)
{

}

ThemeEvent::~ThemeEvent()
{

}
