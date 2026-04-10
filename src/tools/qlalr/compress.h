// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef COMPRESS_H
#define COMPRESS_H

#include <BobUICore/qlist.h>

class Compress
{
public:
  Compress ();

  void operator () (int *table, int row_count, int column_count);

public:
    QList<int> index;
    QList<int> info;
    QList<int> check;
};

#endif // COMPRESS_H
