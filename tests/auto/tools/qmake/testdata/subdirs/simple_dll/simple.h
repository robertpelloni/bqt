// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef SIMPLE_H
#define SIMPLE_H

#include <qstring.h>

#ifdef SIMPLEDLL_MAKEDLL
# define SIMPLEDLL_EXPORT Q_DECL_EXPORT
#else
# define SIMPLEDLL_EXPORT Q_DECL_IMPORT
#endif

class SIMPLEDLL_EXPORT Simple
{
public:
    Simple();
    ~Simple();

    QString test();
};

#endif



