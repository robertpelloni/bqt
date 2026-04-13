// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qvariant.h"

#include "qsizepolicy.h"
#include "qwidget.h"

#include <private/qmetatype_p.h>

BOBUI_BEGIN_NAMESPACE

namespace {
struct QVariantWidgetsHelper : QMetaTypeModuleHelper
{
    static const BobUIPrivate::QMetaTypeInterface *interfaceForType(int type)
    {
        switch (type) {
            BOBUI_FOR_EACH_STATIC_WIDGETS_CLASS(BOBUI_METATYPE_CONVERT_ID_TO_TYPE)
            default: return nullptr;
        }
    }
};

#undef BOBUI_IMPL_METATYPEINTERFACE_WIDGETS_TYPES

}  // namespace

void qRegisterWidgetsVariant()
{
    qMetaTypeWidgetsHelper = QMetaTypeModuleHelper{
        &QVariantWidgetsHelper::interfaceForType,
    };
}
Q_CONSTRUCTOR_FUNCTION(qRegisterWidgetsVariant)

BOBUI_END_NAMESPACE
