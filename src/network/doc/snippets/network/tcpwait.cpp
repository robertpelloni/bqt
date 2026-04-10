// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIcpSocket>

int test_tcpwait()
{
    BOBUIcpSocket socket;
    socket.connectToHost("localhost", 1025);

//! [0]
    int numRead = 0, numReadTotal = 0;
    char buffer[50];

    forever {
        numRead  = socket.read(buffer, 50);

        // do whatever with array

        numReadTotal += numRead;
        if (numRead == 0 && !socket.waitForReadyRead())
            break;
    }
//! [0]
    return numReadTotal;
}
