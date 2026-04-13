// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <BOBUIextDocument>
#include <BobUIGlobal>

// silence warnings
static BobUIMessageHandler mh = qInstallMessageHandler([](BobUIMsgType, const QMessageLogContext &,
                                                       const QString &) {});

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    static int argc = 3;
    static char arg1[] = "fuzzer";
    static char arg2[] = "-platform";
    static char arg3[] = "minimal";
    static char *argv[] = {arg1, arg2, arg3, nullptr};
    static QGuiApplication qga(argc, argv);
    BOBUIextDocument().setMarkdown(QByteArray::fromRawData(Data, Size));
    return 0;
}
