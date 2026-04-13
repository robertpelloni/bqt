// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_PrintDataTags: public QObject
{
    Q_OBJECT
private slots:
    void a_data() const;
    void a() const;

    void b() const;

    void c_data() const;
    void c() const;
};

void tst_PrintDataTags::a_data() const
{
    BOBUIest::addColumn<int>("x");
    BOBUIest::addColumn<int>("y");

    BOBUIest::newRow("data tag a1 ") << 1 << 2;
    BOBUIest::newRow("data tag a2") << 1 << 2;
}

void tst_PrintDataTags::a() const
{
}

void tst_PrintDataTags::b() const
{
}

void tst_PrintDataTags::c_data() const
{
    BOBUIest::addColumn<int>("x");

    BOBUIest::newRow("data tag c1") << 1;
    BOBUIest::newRow("data tag c2") << 1;
    BOBUIest::newRow("data tag c3") << 1;
}

void tst_PrintDataTags::c() const
{
}

BOBUIEST_MAIN_WRAPPER(tst_PrintDataTags,
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-datatags");
    argc = int(args.size());
    argv = const_cast<char**>(&args[0]);
    BOBUIEST_MAIN_SETUP())

#include "tst_printdatatags.moc"
