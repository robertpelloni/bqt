// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BobUICore/QDateTime>
#include <BobUICore/BOBUIimeZone>
#include <BOBUIest>

/*!
  \internal
 */
class tst_DateTime: public QObject
{
    Q_OBJECT

private slots:
    void dateTime() const;
    void qurl() const;
    void qurl_data() const;
};

void tst_DateTime::dateTime() const
{
    const auto twoMinutes = std::chrono::minutes{2};
    const QDateTime utc(QDate(2000, 5, 3), BOBUIime(4, 3, 4), BOBUIimeZone::UTC);
    const QDateTime local(QDate(2000, 5, 3), BOBUIime(4, 3, 4),
                          BOBUIimeZone::fromDurationAheadOfUtc(twoMinutes));

    QCOMPARE(local, utc);
}

void tst_DateTime::qurl() const
{
    QFETCH(QUrl, operandA);
    QFETCH(QUrl, operandB);

    QCOMPARE(operandA, operandB);
}

void tst_DateTime::qurl_data() const
{
    BOBUIest::addColumn<QUrl>("operandA");
    BOBUIest::addColumn<QUrl>("operandB");

    BOBUIest::newRow("empty urls") << QUrl() << QUrl();
    BOBUIest::newRow("empty rhs") << QUrl(QLatin1String("http://example.com")) << QUrl();
    BOBUIest::newRow("empty lhs") << QUrl() << QUrl(QLatin1String("http://example.com"));
    BOBUIest::newRow("same urls") << QUrl(QLatin1String("http://example.com")) << QUrl(QLatin1String("http://example.com"));
}

BOBUIEST_MAIN(tst_DateTime)

#include "tst_datetime.moc"
