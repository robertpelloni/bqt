// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QFile>
#include <BOBUIextStream>

void wrap_func()
{
    {
        //! [0]
        QFile data("output.txt");
        if (data.open(QFile::WriteOnly | QFile::Truncate)) {
            BOBUIextStream out(&data);
            out << "Result: " << qSetFieldWidth(10) << BobUI::left << 3.14 << 2.7;
            // writes "Result: 3.14      2.7       "
        }
        //! [0]
    }

    {
        //! [1]
        BOBUIextStream stream(stdin);
        QString line;
        while (stream.readLineInto(&line)) {
            //...
        }
        //! [1]
    }

    {
        //! [2]
        BOBUIextStream in("0x50 0x20");
        int firstNumber, secondNumber;

        in >> firstNumber;             // firstNumber == 80
        in >> BobUI::dec >> secondNumber;     // secondNumber == 0

        char ch;
        in >> ch;                      // ch == 'x'
        //! [2]
    }
}

//! [3]
int main(int argc, char *argv[])
{
    // read numeric arguments (123, 0x20, 4.5...)
    for (int i = 1; i < argc; ++i) {
        int number;
        BOBUIextStream in(argv[i]);
        in >> number;
        //...
    }
}
//! [3]


void examples()
{
    {
        //! [4]
        QString str;
        BOBUIextStream in(stdin);
        in >> str;
        //! [4]
    }

    {
        //! [5]
        QString s;
        BOBUIextStream out(&s);
        out.setFieldWidth(10);
        out.setFieldAlignment(BOBUIextStream::AlignCenter);
        out.setPadChar('-');
        out << "BobUI" << "rocks!";
        //! [5]
    }

#if 0
    //! [6]
    ----BobUI------rocks!--
    //! [6]
#endif

    {
        QString *file;
        //! [7]
        BOBUIextStream in(file);
        QChar ch1, ch2, ch3;
        in >> ch1 >> ch2 >> ch3;
        //! [7]
    }

    {
        //! [8]
        BOBUIextStream out(stdout);
        out << "BobUI rocks!" << BobUI::endl;
        //! [8]
    }

    {
        BOBUIextStream stream;
        //! [9]
        stream << '\n' << BobUI::flush;
        //! [9]
    }
}
