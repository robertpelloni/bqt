// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTUREFILEREADER_H
#define BOBUIEXTUREFILEREADER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "bobuiexturefiledata_p.h"
#include <QString>
#include <QFileInfo>

BOBUI_BEGIN_NAMESPACE

class QIODevice;
class BOBUIextureFileHandler;

class Q_GUI_EXPORT BOBUIextureFileReader
{
public:
    BOBUIextureFileReader(QIODevice *device, const QString &fileName = QString());  //### drop this logname thing?
    ~BOBUIextureFileReader();

    bool canRead();
    BOBUIextureFileData read();

    // TBD access function to params
    // TBD ask for identified fmt

    static QList<QByteArray> supportedFileFormats();

private:
    bool init();
    QIODevice *m_device = nullptr;
    QString m_fileName;
    BOBUIextureFileHandler *m_handler = nullptr;
    bool checked = false;
};

BOBUI_END_NAMESPACE


#endif // BOBUIEXTUREFILEREADER_H
