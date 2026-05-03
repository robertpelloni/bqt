// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUINetwork/bobuicpserver.h>
#include <BobUINetwork/bobuicpsocket.h>
#include <BobUINetwork/qhostaddress.h>

#include <BobUICore/qcoreapplication.h>

#ifdef Q_OS_UNIX
#  include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (argc < 1) {
        fprintf(stderr, "Need a port number\n");
        return 1;
    }

    int port = QByteArrayView(argv[1]).toInt();
    BOBUIcpServer server;
    if (!server.listen(QHostAddress::LocalHost, port)) {
        fprintf(stderr, "Failed to listen: %s\n", server.errorString().toLatin1().constData());
        if (server.serverError() == BOBUIcpSocket::AddressInUseError) {
            // let's see if we can find the process that would be holding this
            // still open
#ifdef Q_OS_LINUX
            static const char *const ss_args[] = {
                "ss", "-nap", "sport", "=", argv[1], nullptr
            };
            dup2(STDERR_FILENO, STDOUT_FILENO);
            execvp(ss_args[0], const_cast<char **>(ss_args));
#endif
        }
        return 1;
    }

    printf("Listening\n");
    fflush(stdout);

    server.waitForNewConnection(5000);
    qFatal("Crash");
    return 0;
}
