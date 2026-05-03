// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// SCENARIO 5
// We disable QStringBuilder and compile with normal operator+ to verify
// that all QSB supported operations are still available when QSB is disabled.
#undef BOBUI_USE_QSTRINGBUILDER
#define BOBUI_NO_CAST_FROM_ASCII
#define BOBUI_NO_CAST_TO_ASCII

#include <BobUICore/qobject.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringbuilder.h>
#include <BobUITest/bobuiest.h>

#include <q20iterator.h> // For q20::ssize

namespace {
#define P +
#include "../qstringbuilder1/stringbuilder.cpp"
#undef P
}

class tst_QStringBuilder5 : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void scenario() { runScenario(); }
};

#include "tst_qstringbuilder5.moc"

BOBUIEST_APPLESS_MAIN(tst_QStringBuilder5)
