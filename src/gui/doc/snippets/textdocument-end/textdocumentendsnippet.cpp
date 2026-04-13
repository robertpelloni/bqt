// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <iostream>

namespace textdocumentendsnippet {
void wrapper()
{
    QString contentString("One\nTwp\nThree");
    BOBUIextDocument *doc = new BOBUIextDocument(contentString);

//! [0]
for (BOBUIextBlock it = doc->begin(); it != doc->end(); it = it.next())
    std::cout << it.text().toStdString() << "\n";
//! [0]

} // wrapper
} //textdocumentendsnippet
