// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandserverbufferintegration_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandServerBuffer::QWaylandServerBuffer()
{
}

QWaylandServerBuffer::~QWaylandServerBuffer()
{
}

QWaylandServerBuffer::Format QWaylandServerBuffer::format() const
{
    return m_format;
}

QSize QWaylandServerBuffer::size() const
{
    return m_size;
}

void QWaylandServerBuffer::setUserData(void *userData)
{
    m_user_data = userData;
}

void *QWaylandServerBuffer::userData() const
{
    return m_user_data;
}

QWaylandServerBufferIntegration::QWaylandServerBufferIntegration()
{
}
QWaylandServerBufferIntegration::~QWaylandServerBufferIntegration()
{
}

}

BOBUI_END_NAMESPACE
