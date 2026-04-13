// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPATHSIMPLIFIER_P_H
#define QPATHSIMPLIFIER_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qpainterpath.h>
#include <BobUIGui/private/qdatabuffer_p.h>
#include <BobUIGui/private/qvectorpath_p.h>

BOBUI_BEGIN_NAMESPACE

// The returned vertices are in 8:8 fixed point format. The path is assumed to be in the range (-128, 128)x(-128, 128).
void qSimplifyPath(const QVectorPath &path, QDataBuffer<QPoint> &vertices, QDataBuffer<quint32> &indices, const BOBUIransform &matrix = BOBUIransform());
void qSimplifyPath(const QPainterPath &path, QDataBuffer<QPoint> &vertices, QDataBuffer<quint32> &indices, const BOBUIransform &matrix = BOBUIransform());

BOBUI_END_NAMESPACE

#endif
