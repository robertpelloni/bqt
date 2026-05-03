// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QProcess>

bool zip()
{
//! [0]
    QProcess gzip;
    gzip.start("gzip", QStringList() << "-c");
    if (!gzip.waitForStarted())
        return false;

    gzip.write("BobUI rocks!");
    gzip.closeWriteChannel();

    if (!gzip.waitForFinished())
        return false;

    QByteArray result = gzip.readAll();
//! [0]

    gzip.start("gzip", QStringList() << "-d" << "-c");
    gzip.write(result);
    gzip.closeWriteChannel();

    if (!gzip.waitForFinished())
        return false;

    qDebug("Result: %s", gzip.readAll().data());
    return true;
}


int main()
{
    zip();
    return 0;
}
