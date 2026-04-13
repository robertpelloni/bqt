// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPROXYSTYLE_P_H
#define QPROXYSTYLE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qcommonstyle.h"
#include "qcommonstyle_p.h"
#include "qproxystyle.h"

#include <BobUICore/qpointer.h>

#ifndef BOBUI_NO_STYLE_PROXY

BOBUI_BEGIN_NAMESPACE

class QProxyStylePrivate : public QCommonStylePrivate
{
    Q_DECLARE_PUBLIC(QProxyStyle)
public:
    void ensureBaseStyle() const;
private:
    QProxyStylePrivate() :
    QCommonStylePrivate(), baseStyle(nullptr) {}
    mutable QPointer <QStyle> baseStyle;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_STYLE_PROXY

#endif //QPROXYSTYLE_P_H
