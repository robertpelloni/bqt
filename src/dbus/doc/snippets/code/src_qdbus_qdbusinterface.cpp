// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QDBusInterface>
#include <QDBusReply>

void DBusInterface_main()
{
//! [0]
QDBusInterface remoteApp( "com.example.Calculator", "/Calculator/Operations",
                          "org.mathematics.RPNCalculator" );
remoteApp.call( "PushOperand", 2 );
remoteApp.call( "PushOperand", 2 );
remoteApp.call( "ExecuteOperation", "+" );
QDBusReply<int> reply = remoteApp.call( "PopOperand" );

if ( reply.isValid() )
    printf( "%d", reply.value() );          // prints 4
//! [0]
}
