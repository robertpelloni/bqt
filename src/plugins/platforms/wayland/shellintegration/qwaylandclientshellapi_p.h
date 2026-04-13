// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDCLIENTSHELLAPI_P_H
#define QWAYLANDCLIENTSHELLAPI_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// N O T E
// -------
// This file provides a supported API for creating client-side shell
// extensions. Source compatibility will be preserved, but we may break
// forward and backward binary compatibility, even in patch releases.
//
// The supported API contains these classes:
//
// BobUIWaylandClient::QWaylandShellSurface
// BobUIWaylandClient::QWaylandShellIntegration
// BobUIWaylandClient::QWaylandShellIntegrationPlugin

#include "BobUIWaylandClient/private/qwaylandshellsurface_p.h"
#include "BobUIWaylandClient/private/qwaylandshellintegration_p.h"
#include "BobUIWaylandClient/private/qwaylandshellintegrationplugin_p.h"

#endif // QWAYLANDCLIENTSHELLAPI_P_H
