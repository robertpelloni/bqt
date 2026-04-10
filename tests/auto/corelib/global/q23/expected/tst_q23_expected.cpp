// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc
// Mutz <marc.mutz@kdab.com> Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <private/qexpected_p.h>

#include <BOBUIest>
#include <QObject>
#include <BobUICore/qexceptionhandling.h>

class tst_q23_expected : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void value_throw_exception_unreachable_data();
    void value_throw_exception_unreachable();
};

void tst_q23_expected::value_throw_exception_unreachable_data()
{
    BOBUIest::addColumn<bool>("unexpected");
    BOBUIest::addRow("expected") << false;
}

void tst_q23_expected::value_throw_exception_unreachable()
{
    QFETCH(bool, unexpected);

    const int value = 3;
    const auto toExp = [](int value, bool unexpected) {
        return unexpected ? q23::make_unexpected(true) : q23::expected<int, bool>(value);
    };
    QCOMPARE(toExp(value, unexpected).value(), value);
}

BOBUIEST_GUILESS_MAIN(tst_q23_expected)
#include "tst_q23_expected.moc"
