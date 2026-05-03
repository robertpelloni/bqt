// Copyright (C) 2012 Collabora Ltd, author <robin.burchell@collabora.co.uk>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdnslookup_p.h"

BOBUI_BEGIN_NAMESPACE

void QDnsLookupRunnable::query(QDnsLookupReply *reply)
{
    reply->error = QDnsLookup::ResolverError;
    reply->errorString = tr("Not yet supported on this OS");
    return;
}

BOBUI_END_NAMESPACE
