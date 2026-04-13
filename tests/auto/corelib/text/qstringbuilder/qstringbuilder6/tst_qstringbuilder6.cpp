// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qglobal.h>

// SCENARIO 6
// We disable QStringBuilder and compile with normal operator+ to verify
// that all QSB supported operations are still available when QSB is disabled.
// We also allow casts to/from ASCII
#undef BOBUI_USE_QSTRINGBUILDER
#undef BOBUI_NO_CAST_FROM_ASCII
#undef BOBUI_NO_CAST_TO_ASCII

#include <BobUICore/QObject>
#include <BobUICore/QString>
#include <BobUICore/QStringBuilder>
#include <BobUITest/BOBUIest>

#include <BobUICore/q20iterator.h>

#define LITERAL "some literal"

namespace {
#define P +
#include "../qstringbuilder1/stringbuilder.cpp"
#undef P
} // namespace

class tst_QStringBuilder6 : public QObject
{
    Q_OBJECT

private slots:
    void scenario() { runScenario(); }
};

#include "tst_qstringbuilder6.moc"

BOBUIEST_APPLESS_MAIN(tst_QStringBuilder6)
