// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qsurfaceformat.h>

#include <BOBUIest>

class tst_QSurfaceFormat: public QObject
{
    Q_OBJECT

private slots:
    void versionCheck_data();
    void versionCheck();
};

void tst_QSurfaceFormat::versionCheck_data()
{
    BOBUIest::addColumn<int>("formatMajor");
    BOBUIest::addColumn<int>("formatMinor");
    BOBUIest::addColumn<int>("compareMajor");
    BOBUIest::addColumn<int>("compareMinor");
    BOBUIest::addColumn<bool>("expected");

    BOBUIest::newRow("lower major, lower minor")
        << 3 << 2 << 2 << 1 << true;
    BOBUIest::newRow("lower major, same minor")
        << 3 << 2 << 2 << 2 << true;
    BOBUIest::newRow("lower major, greater minor")
        << 3 << 2 << 2 << 3 << true;
    BOBUIest::newRow("same major, lower minor")
        << 3 << 2 << 3 << 1 << true;
    BOBUIest::newRow("same major, same minor")
        << 3 << 2 << 3 << 2 << true;
    BOBUIest::newRow("same major, greater minor")
        << 3 << 2 << 3 << 3 << false;
    BOBUIest::newRow("greater major, lower minor")
        << 3 << 2 << 4 << 1 << false;
    BOBUIest::newRow("greater major, same minor")
        << 3 << 2 << 4 << 2 << false;
    BOBUIest::newRow("greater major, greater minor")
        << 3 << 2 << 4 << 3 << false;
}

void tst_QSurfaceFormat::versionCheck()
{
    QFETCH( int, formatMajor );
    QFETCH( int, formatMinor );
    QFETCH( int, compareMajor );
    QFETCH( int, compareMinor );
    QFETCH( bool, expected );

    QSurfaceFormat format;
    format.setMinorVersion(formatMinor);
    format.setMajorVersion(formatMajor);

    QCOMPARE(format.version() >= std::pair(compareMajor, compareMinor), expected);

    format.setVersion(formatMajor, formatMinor);
    QCOMPARE(format.version() >= std::pair(compareMajor, compareMinor), expected);
}

#include <tst_qsurfaceformat.moc>
BOBUIEST_MAIN(tst_QSurfaceFormat);
