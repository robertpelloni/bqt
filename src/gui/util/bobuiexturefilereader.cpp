// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:no-parsing

#include "bobuiexturefilereader_p.h"

#include "qpkmhandler_p.h"
#include "qktxhandler_p.h"
#include "qastchandler_p.h"

#include <QFileInfo>

BOBUI_BEGIN_NAMESPACE

BOBUIextureFileHandler::~BOBUIextureFileHandler() = default;

BOBUIextureFileReader::BOBUIextureFileReader(QIODevice *device, const QString &fileName)
    : m_device(device), m_fileName(fileName)
{
}

BOBUIextureFileReader::~BOBUIextureFileReader()
{
    delete m_handler;
}

BOBUIextureFileData BOBUIextureFileReader::read()
{
    if (!canRead())
        return BOBUIextureFileData();
    return m_handler->read();
}

bool BOBUIextureFileReader::canRead()
{
    if (!checked) {
        checked = true;
        if (!init())
            return false;

        QByteArray headerBlock = m_device->peek(64);
        QFileInfo fi(m_fileName);
        QByteArray suffix = fi.suffix().toLower().toLatin1();
        QByteArray logName = fi.fileName().toUtf8();

        // Currently the handlers are hardcoded; later maybe a list of plugins
        if (QPkmHandler::canRead(suffix, headerBlock)) {
            m_handler = new QPkmHandler(m_device, logName);
        } else if (QKtxHandler::canRead(suffix, headerBlock)) {
            m_handler = new QKtxHandler(m_device, logName);
        } else if (QAstcHandler::canRead(suffix, headerBlock)) {
            m_handler = new QAstcHandler(m_device, logName);
        }
        // else if OtherHandler::canRead() ...etc.
    }
    return (m_handler != nullptr);
}

QList<QByteArray> BOBUIextureFileReader::supportedFileFormats()
{
    // Hardcoded for now
    return {QByteArrayLiteral("pkm"), QByteArrayLiteral("ktx"), QByteArrayLiteral("astc")};
}

bool BOBUIextureFileReader::init()
{
    if (!m_device)
        return false;
    return m_device->isReadable();
}

BOBUI_END_NAMESPACE
