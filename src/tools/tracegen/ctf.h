// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef CTF_H
#define CTF_H

struct Provider;
class QFile;

void writeCtf(QFile &device, const Provider &p);

#endif // CTF_H
