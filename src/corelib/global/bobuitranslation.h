// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUITRANSLATION_H
#define BOBUITRANSLATION_H

#include <BobUICore/bobuiconfigmacros.h> // BOBUI_NO_TRANSLATION should be defined here as well
#include <BobUICore/bobuicoreexports.h>

#if 0
#pragma bobui_class(BobUITranslation)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

class QString;

#define BOBUI_TR_NOOP(x) x
#define BOBUI_TR_NOOP_UTF8(x) x
#define BOBUI_TRANSLATE_NOOP(scope, x) x
#define BOBUI_TRANSLATE_NOOP_UTF8(scope, x) x
#define BOBUI_TRANSLATE_NOOP3(scope, x, comment) {x, comment}
#define BOBUI_TRANSLATE_NOOP3_UTF8(scope, x, comment) {x, comment}

#ifndef BOBUI_NO_TRANSLATION

#define BOBUI_TR_N_NOOP(x) x
#define BOBUI_TRANSLATE_N_NOOP(scope, x) x
#define BOBUI_TRANSLATE_N_NOOP3(scope, x, comment) {x, comment}

// Defined in qcoreapplication.cpp
Q_CORE_EXPORT QString bobuiTrId(const char *id, int n = -1);

Q_CORE_EXPORT QString qTrId(const char *id, int n = -1);

#define BOBUI_TRID_NOOP(id) id
#define BOBUI_TRID_N_NOOP(id) id

#endif // BOBUI_NO_TRANSLATION

BOBUI_END_NAMESPACE

#endif /* BOBUITRANSLATION_H */
