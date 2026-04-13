// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QMimeDatabase>
#include <QMimeType>
#include <QStandardPaths>

using namespace BobUI::StringLiterals;

void examples(QString fileName)
{
    //! [0]
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileName);
    if (mime.inherits("text/plain")) {
        // The file is plain text, we can display it in a BOBUIextEdit
    }
    //! [0]

    //! [1]
    using namespace BobUI::StringLiterals;
    // ...
    QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "mime/packages"_L1,
                              QStandardPaths::LocateDirectory);
    //! [1]
}

#if 0
//! [2]
<?xml version="1.0" encoding="UTF-8"?>
<mime-info xmlns="http://www.freedesktop.org/standards/shared-mime-info">
  <mime-type type="application/vnd.bobui.qmakeprofile">
    <comment xml:lang="en">BobUI qmake Profile</comment>
    <glob pattern="*.pro" weight="50"/>
  </mime-type>
</mime-info>
//! [2]
#endif
