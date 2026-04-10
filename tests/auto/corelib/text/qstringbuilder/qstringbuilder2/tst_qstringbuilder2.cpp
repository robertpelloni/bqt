// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qglobal.h>

// SCENARIO 2
// this is the "full" version. Operator+ is replaced by a QStringBuilder
// based version
// with NO_CAST * defined
#define BOBUI_USE_QSTRINGBUILDER
#define BOBUI_NO_CAST_FROM_ASCII
#define BOBUI_NO_CAST_TO_ASCII

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

class tst_QStringBuilder2 : public QObject
{
    Q_OBJECT

private slots:
    void scenario() { runScenario(); }
};

#include "tst_qstringbuilder2.moc"

BOBUIEST_APPLESS_MAIN(tst_QStringBuilder2)
