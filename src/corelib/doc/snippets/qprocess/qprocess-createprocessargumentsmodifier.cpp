// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QProcess>
#include <BobUICore/qglobal.h>

#ifdef Q_OS_WIN
#include <BobUICore/bobui_windows.h>

int main(int argc, char *argv[])
{
//! [0]
    QProcess process;
    process.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_CONSOLE;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED
                                           | FOREGROUND_INTENSITY;
    });
    process.start("C:\\Windows\\System32\\cmd.exe", QStringList() << "/k" << "title" << "The Child Process");
//! [0]
}
#endif
