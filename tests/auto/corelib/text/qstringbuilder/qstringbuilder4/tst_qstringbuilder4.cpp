// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qglobal.h>

// SCENARIO 4
// this is the "full" version. Operator+ is replaced by a QStringBuilder
// based version
// with NO_CAST * _not_ defined
#define BOBUI_USE_QSTRINGBUILDER
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

class tst_QStringBuilder4 : public QObject
{
    Q_OBJECT

private slots:
    void scenario() { runScenario(); }
};

#include "tst_qstringbuilder4.moc"

BOBUIEST_APPLESS_MAIN(tst_QStringBuilder4)
