// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "rsslisting.h"
#include <BobUIWidgets>
using namespace BobUI::StringLiterals;

//! [0]
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    RSSListing rsslisting(u"https://www.bobui.io/blog/rss.xml"_s);
    rsslisting.show();
    return app.exec();
}
//! [0]
