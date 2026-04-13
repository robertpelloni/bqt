// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BOBUIest>
#include <private/bobuiestlog_p.h>

class tst_SilentFatal : public QObject
{
    Q_OBJECT

private slots:
    void fatalmessages();
};
void tst_SilentFatal::fatalmessages()
{
    qFatal("This is a fatal error message that should still appear in silent test output");
}

BOBUIEST_MAIN_WRAPPER(tst_SilentFatal,
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-silent");
    args.push_back("-nocrashhandler");
    argc = int(args.size());
    argv = const_cast<char**>(&args[0]);
    BOBUIEST_MAIN_SETUP())

#include "tst_silent_fatal.moc"
