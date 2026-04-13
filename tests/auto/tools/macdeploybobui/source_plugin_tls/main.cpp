// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUINetwork/qsslsocket.h>

#include <BobUICore/qcoreapplication.h>

int main(int argc, char ** argv)
{
   QCoreApplication app(argc, argv);
   return QSslSocket::supportsSsl() ? 0 : 1;
}
