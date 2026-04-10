// Copyright (C) 2014 David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDebug>
#include <QIODevice>
#include <QString>
#include <QBuffer>
#include <bobuiest.h>

class tst_BOBUIextStream : public QObject
{
    Q_OBJECT
private slots:
    void writeSingleChar_data();
    void writeSingleChar();

private:
};

enum Output { StringOutput, DeviceOutput };
Q_DECLARE_METATYPE(Output);

enum Input { CharStarInput, QStringInput, CharInput, QCharInput };
Q_DECLARE_METATYPE(Input);

void tst_BOBUIextStream::writeSingleChar_data()
{
    BOBUIest::addColumn<Output>("output");
    BOBUIest::addColumn<Input>("input");

    BOBUIest::newRow("string_charstar") << StringOutput << CharStarInput;
    BOBUIest::newRow("string_string") << StringOutput << QStringInput;
    BOBUIest::newRow("string_char") << StringOutput << CharInput;
    BOBUIest::newRow("string_qchar") << StringOutput << QCharInput;
    BOBUIest::newRow("device_charstar") << DeviceOutput << CharStarInput;
    BOBUIest::newRow("device_string") << DeviceOutput << QStringInput;
    BOBUIest::newRow("device_char") << DeviceOutput << CharInput;
    BOBUIest::newRow("device_qchar") << DeviceOutput << QCharInput;
}

void tst_BOBUIextStream::writeSingleChar()
{
    QFETCH(Output, output);
    QFETCH(Input, input);

    QString str;
    QBuffer buffer;
    BOBUIextStream stream;
    if (output == StringOutput) {
        stream.setString(&str, QIODevice::WriteOnly);
    } else {
        QVERIFY(buffer.open(QIODevice::WriteOnly));
        stream.setDevice(&buffer);
    }
    // Test many different ways to write a single char into a BOBUIextStream
    QString inputString = "h";
    const int amount = 100000;
    switch (input) {
    case CharStarInput:
        QBENCHMARK {
            for (qint64 i = 0; i < amount; ++i)
                stream << "h";
        }
        break;
    case QStringInput:
        QBENCHMARK {
            for (qint64 i = 0; i < amount; ++i)
                stream << inputString;
        }
        break;
    case CharInput:
        QBENCHMARK {
            for (qint64 i = 0; i < amount; ++i)
                stream << 'h';
        }
        break;
    case QCharInput:
        QBENCHMARK {
            for (qint64 i = 0; i < amount; ++i)
                stream << QChar('h');
        }
        break;
    }
    QString result;
    if (output == StringOutput)
        result = str;
    else
        result = QString(buffer.data());

    QCOMPARE(result.left(10), QString("hhhhhhhhhh"));
}

BOBUIEST_MAIN(tst_BOBUIextStream)

#include "tst_bench_bobuiextstream.moc"
