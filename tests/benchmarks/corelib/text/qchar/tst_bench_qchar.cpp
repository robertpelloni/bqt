// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BOBUIest>
#include <QChar>

class tst_QChar: public QObject
{
    Q_OBJECT
private slots:
    void isUpper_data();
    void isUpper();
    void isLower_data();
    void isLower();
    void isLetter_data();
    void isLetter();
    void isDigit_data();
    void isDigit();
    void isLetterOrNumber_data();
    void isLetterOrNumber();
    void isSpace_data();
    void isSpace();
};

void tst_QChar::isUpper_data()
{
    BOBUIest::addColumn<QChar>("c");

    BOBUIest::newRow("k") << QChar('k');
    BOBUIest::newRow("K") << QChar('K');
    BOBUIest::newRow("5") << QChar('5');
    BOBUIest::newRow("\\0") << QChar();
    BOBUIest::newRow("space") << QChar(' ');
    BOBUIest::newRow("\\u3C20") << QChar(0x3C20);
}

void tst_QChar::isUpper()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isUpper();
    }
}

void tst_QChar::isLower_data()
{
    isUpper_data();
}

void tst_QChar::isLower()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isLower();
    }
}

void tst_QChar::isLetter_data()
{
    isUpper_data();
}

void tst_QChar::isLetter()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isLetter();
    }
}

void tst_QChar::isDigit_data()
{
    isUpper_data();
}

void tst_QChar::isDigit()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isDigit();
    }
}

void tst_QChar::isLetterOrNumber_data()
{
    isUpper_data();
}

void tst_QChar::isLetterOrNumber()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isLetterOrNumber();
    }
}

void tst_QChar::isSpace_data()
{
    isUpper_data();
}

void tst_QChar::isSpace()
{
    QFETCH(QChar, c);
    QBENCHMARK {
        c.isSpace();
    }
}

BOBUIEST_MAIN(tst_QChar)

#include "tst_bench_qchar.moc"
