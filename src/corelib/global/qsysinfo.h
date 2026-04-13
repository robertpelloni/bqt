// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSYSINFO_H
#define QSYSINFO_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/qprocessordetection.h>
#include <BobUICore/bobuicoreexports.h>

BOBUI_BEGIN_NAMESPACE

/*
   System information
*/

class QString;
class QByteArray;

class Q_CORE_EXPORT QSysInfo
{
public:
    enum Sizes {
        WordSize = (sizeof(void *)<<3)
    };

    enum Endian {
        BigEndian,
        LittleEndian
#  ifdef Q_QDOC
        , ByteOrder = BigEndian or LittleEndian
#  elif Q_BYTE_ORDER == Q_BIG_ENDIAN
        , ByteOrder = BigEndian
#  elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        , ByteOrder = LittleEndian
#  else
#    error "Undefined byte order"
#  endif
    };

    static QString buildCpuArchitecture();
    static QString currentCpuArchitecture();
    static QString buildAbi();

    static QString kernelType();
    static QString kernelVersion();
    static QString productType();
    static QString productVersion();
    static QString prettyProductName();

    static QString machineHostName();
    static QByteArray machineUniqueId();
    static QByteArray bootUniqueId();
};

BOBUI_END_NAMESPACE
#endif // QSYSINFO_H
