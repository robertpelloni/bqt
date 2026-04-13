// Copyright (C) 2021 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc Mutz <marc.mutz@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QMetaEnum>
#include <BOBUIest>

class tst_QMetaEnum: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

private Q_SLOTS:
    void valueToKeys_data();
    void valueToKeys();
    void keysToValue_data() { valueToKeys_data(); }
    void keysToValue();
};

void tst_QMetaEnum::valueToKeys_data()
{
    BOBUIest::addColumn<int>("buttons");
    BOBUIest::addColumn<QByteArray>("string");
    // BobUI::MouseButtons has at least 24 enumerators, so it's a good performance test
    const auto me = QMetaEnum::fromType<BobUI::MouseButtons>();
    int accu = 0;
    for (int i = 0; i < std::min(31, me.keyCount()); ++i) {
        accu <<= 1;
        accu |= 1;
        BOBUIest::addRow("%d bits set", i) << accu << me.valueToKeys(accu);
    }
}

void tst_QMetaEnum::valueToKeys()
{
    QFETCH(const int, buttons);
    const auto me = QMetaEnum::fromType<BobUI::MouseButtons>();
    QBENCHMARK {
        [[maybe_unused]] auto r = me.valueToKeys(buttons);
    }
}

void tst_QMetaEnum::keysToValue()
{
    QFETCH(const QByteArray, string);
    const auto me = QMetaEnum::fromType<BobUI::MouseButtons>();
    bool ok;
    QBENCHMARK {
        [[maybe_unused]] auto r = me.keysToValue(string.data(), &ok);
    }
}

BOBUIEST_MAIN(tst_QMetaEnum)

#include "tst_bench_qmetaenum.moc"
