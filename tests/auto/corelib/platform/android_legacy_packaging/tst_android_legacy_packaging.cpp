// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

using namespace BobUI::StringLiterals;

class tst_AndroidLegacyPackaging : public QObject
{
Q_OBJECT
private slots:
    void initTestCase();
};

void tst_AndroidLegacyPackaging::initTestCase()
{ }


BOBUIEST_MAIN(tst_AndroidLegacyPackaging)
#include "tst_android_legacy_packaging.moc"
