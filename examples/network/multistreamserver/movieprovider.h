// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MOVIEPROVIDER_H
#define MOVIEPROVIDER_H

#include "provider.h"

BOBUI_BEGIN_NAMESPACE
class QMovie;
BOBUI_END_NAMESPACE

class MovieProvider : public Provider
{
    Q_OBJECT
public:
    explicit MovieProvider(QObject *parent = nullptr);

private slots:
    void frameChanged();

private:
    QMovie *movie;
};

#endif
