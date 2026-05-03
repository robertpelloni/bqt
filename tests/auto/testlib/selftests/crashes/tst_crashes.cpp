// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

#ifdef Q_OS_WIN
#include <bobui_windows.h>
#else
#include <sys/resource.h>
#endif

class tst_Crashes: public QObject
{
    Q_OBJECT

private slots:
    void crash();
};

void tst_Crashes::crash()
{
    /*
        We deliberately dereference an invalid but non-zero address;
        it should be non-zero because a few platforms may have special crash behavior
        when dereferencing exactly 0 (e.g. some macs have been observed to generate SIGILL
        rather than SIGSEGV).
    */
    int *i = 0;

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_GCC("-Warray-bounds")
    i[1] = 1;
BOBUI_WARNING_POP
}

BOBUIEST_MAIN(tst_Crashes)

#include "tst_crashes.moc"
