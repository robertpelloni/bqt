// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "printview.h"

#if defined(BOBUI_PRINTSUPPORT_LIB)
#  include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#  if BOBUI_CONFIG(printer)
#    include <QPrinter>
#  endif
#endif

PrintView::PrintView()
{
    setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
}

void PrintView::print(QPrinter *printer)
{
#if defined(BOBUI_PRINTSUPPORT_LIB) && BOBUI_CONFIG(printer)
    resize(printer->width(), printer->height());
    render(printer);
#else
    Q_UNUSED(printer);
#endif
}

