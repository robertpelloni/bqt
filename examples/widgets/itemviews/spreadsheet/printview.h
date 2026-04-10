// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PRINTVIEW_H
#define PRINTVIEW_H

#include <BOBUIableView>
BOBUI_BEGIN_NAMESPACE
class QPrinter;
BOBUI_END_NAMESPACE

class PrintView : public BOBUIableView
{
    Q_OBJECT

public:
    PrintView();

public slots:
    void print(QPrinter *printer);
};

#endif // PRINTVIEW_H


