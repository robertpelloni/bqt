// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QFileDialog>
#include <BobUIGui>

int loadFileDialog()
{
    //![0]
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("/Users/foo/Code/bobui5")
         << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec()) {
        // ...
    }
    //![0]
    return 1;
}
