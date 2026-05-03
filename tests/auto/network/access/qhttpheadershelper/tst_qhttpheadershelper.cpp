// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUINetwork/qhttpheaders.h>
#include <BobUINetwork/private/qhttpheadershelper_p.h>

#include <BobUITest/bobuiest.h>

using namespace BobUI::StringLiterals;

class tst_QHttpHeadersHelper : public QObject
{
    Q_OBJECT

private slots:
    void testCompareStrict();

private:
    static constexpr QAnyStringView n1{"name1"};
    static constexpr QAnyStringView n2{"name2"};
    static constexpr QAnyStringView v1{"value1"};
    static constexpr QAnyStringView v2{"value2"};
    static constexpr QAnyStringView N1{"NAME1"};
    static constexpr QAnyStringView N2{"NAME2"};
    static constexpr QAnyStringView V1{"VALUE1"};
    static constexpr QAnyStringView V2{"VALUE2"};
};

void tst_QHttpHeadersHelper::testCompareStrict()
{
    using namespace QHttpHeadersHelper;

    // Basic comparisons
    QHttpHeaders h1;
    QHttpHeaders h2;
    QVERIFY(compareStrict(h1, h2)); // empties
    h1.append(n1, v1);
    QVERIFY(compareStrict(h1, h1)); // self
    h2.append(n1, v1);
    QVERIFY(compareStrict(h1, h2));
    h1.append(n2, v2);
    QVERIFY(!compareStrict(h1, h2));
    h1.removeAll(n2);
    QVERIFY(compareStrict(h1, h2));

    // Order-sensitivity
    h1.clear();
    h2.clear();
    // Same headers but in different order
    h1.append(n1, v1);
    h1.append(n2, v2);
    h2.append(n2, v2);
    h2.append(n1, v1);
    QVERIFY(!compareStrict(h1, h2));

    // Different number of headers
    h1.clear();
    h2.clear();
    h1.append(n1, v1);
    h2.append(n1, v1);
    h2.append(n2, v2);
    QVERIFY(!compareStrict(h1, h2));

    // Same header name, multiple values
    h1.clear();
    h2.clear();
    h1.append(n1, v1);
    h1.append(n1, v2);
    h2.append(n1, v1);
    QVERIFY(!compareStrict(h1, h2));
    h2.append(n1, v2);
    QVERIFY(compareStrict(h1, h2));
}

BOBUIEST_MAIN(tst_QHttpHeadersHelper)
#include "tst_qhttpheadershelper.moc"
