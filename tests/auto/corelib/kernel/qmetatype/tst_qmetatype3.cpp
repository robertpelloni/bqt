// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "tst_qmetatype.h"

#include <BobUICore/private/qmetaobjectbuilder_p.h>


void tst_QMetaType::automaticTemplateRegistration_2()
{
    FOR_EACH_STATIC_PRIMITIVE_TYPE(
      PRINT_2ARG_TEMPLATE
    )
}
