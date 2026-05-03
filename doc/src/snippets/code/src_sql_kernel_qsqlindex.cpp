// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
QStringList list = myIndex.toStringList();
QStringList::Iterator it = list.begin();
while(it != list.end()) {
    myProcessing(*it);
    ++it;
}
//! [0]
