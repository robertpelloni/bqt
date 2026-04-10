// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylanddataoffer_p.h"
#include "qwaylanddatadevicemanager_p.h"
#include "qwaylanddisplay_p.h"

#include <BobUICore/private/qcore_unix_p.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformclipboard.h>

#include <BobUICore/QDebug>

using namespace std::chrono;

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

static QString plainText()
{
    return QStringLiteral("text/plain");
}

static QString utf8Text()
{
    return QStringLiteral("text/plain;charset=utf-8");
}

static QString uriList()
{
    return QStringLiteral("text/uri-list");
}

static QString mozUrl()
{
    return QStringLiteral("text/x-moz-url");
}

static QString portalFileTransfer()
{
    return QStringLiteral("application/vnd.portal.filetransfer");
}

static QByteArray convertData(const QString &originalMime, const QString &newMime, const QByteArray &data)
{
    if (originalMime == newMime)
        return data;

    // Convert text/x-moz-url, which is an UTF-16 string of
    // URL and page title pairs, all separated by line breaks, to text/uri-list.
    // see also bobuibase/src/plugins/platforms/xcb/qxcbmime.cpp
    if (originalMime == uriList() && newMime == mozUrl()) {
        if (data.size() > 1) {
            const quint8 byte0 = data.at(0);
            const quint8 byte1 = data.at(1);

            if ((byte0 == 0xff && byte1 == 0xfe) || (byte0 == 0xfe && byte1 == 0xff)
                || (byte0 != 0 && byte1 == 0) || (byte0 == 0 && byte1 != 0)) {
                QByteArray converted;
                const QString str = QString::fromUtf16(
                      reinterpret_cast<const char16_t *>(data.constData()), data.size() / 2);
                if (!str.isNull()) {
                    const auto urls = QStringView{str}.split(u'\n');
                    // Only the URL is interesting, skip the page title.
                    for (int i = 0; i < urls.size(); i += 2) {
                        const QUrl url(urls.at(i).trimmed().toString());
                        if (url.isValid()) {
                            converted += url.toEncoded();
                            converted += "\r\n";
                        }
                    }
                }
                return converted;
            // 8 byte encoding, remove a possible 0 at the end.
            } else {
                QByteArray converted = data;
                if (converted.endsWith('\0'))
                    converted.chop(1);
                converted += "\r\n";
                return converted;
            }
        }
    }

    return data;
}

QWaylandDataOffer::QWaylandDataOffer(QWaylandDisplay *display, struct ::wl_data_offer *offer)
    : BobUIWayland::wl_data_offer(offer)
    , m_display(display)
    , m_mimeData(new QWaylandMimeData(this))
{
}

QWaylandDataOffer::~QWaylandDataOffer()
{
    destroy();
}


QString QWaylandDataOffer::firstFormat() const
{
    if (m_mimeData->formats().isEmpty())
        return QString();

    return m_mimeData->formats().first();
}

QMimeData *QWaylandDataOffer::mimeData()
{
    return m_mimeData.data();
}

BobUI::DropActions QWaylandDataOffer::supportedActions() const
{
    if (version() < 3) {
        return BobUI::MoveAction | BobUI::CopyAction;
    }

    return m_supportedActions;
}

void QWaylandDataOffer::startReceiving(const QString &mimeType, int fd)
{
    receive(mimeType, fd);
    wl_display_flush(m_display->wl_display());
}

void QWaylandDataOffer::data_offer_offer(const QString &mime_type)
{
    m_mimeData->appendFormat(mime_type);
}

void QWaylandDataOffer::data_offer_action(uint32_t dnd_action)
{
    Q_UNUSED(dnd_action);
    // This is the compositor telling the drag target what action it should perform
    // It does not map nicely into BobUI final drop semantics, other than pretending there is only one supported action?
}

void QWaylandDataOffer::data_offer_source_actions(uint32_t source_actions)
{
    m_supportedActions = BobUI::DropActions();
    if (source_actions & WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE)
        m_supportedActions |= BobUI::MoveAction;
    if (source_actions & WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY)
        m_supportedActions |= BobUI::CopyAction;
}

QWaylandMimeData::QWaylandMimeData(QWaylandAbstractDataOffer *dataOffer)
    : m_dataOffer(dataOffer)
{
}

QWaylandMimeData::~QWaylandMimeData()
{
}

void QWaylandMimeData::appendFormat(const QString &mimeType)
{
    // "DELETE" is a potential leftover from XdndActionMode sent by e.g. Firefox, ignore it.
    if (mimeType != QLatin1String("DELETE")) {
        m_types << mimeType;
        m_data.remove(mimeType); // Clear previous contents
    }
}

bool QWaylandMimeData::hasFormat_sys(const QString &mimeType) const
{
    return formats().contains(mimeType);
}

QStringList QWaylandMimeData::formats_sys() const
{
    QStringList types;
    types.reserve(m_types.size());

    for (const QString &type : m_types) {
        QString mime = type;

        if (mime == utf8Text()) {
            mime = plainText();
        } else if (mime == mozUrl()) {
            mime = uriList();
        }

        if (!types.contains(mime)) {
            types << mime;
        }
    }

    return types;
}

QVariant QWaylandMimeData::retrieveData_sys(const QString &mimeType, QMetaType type) const
{
    Q_UNUSED(type);

    auto it = m_data.constFind(mimeType);
    if (it != m_data.constEnd())
        return *it;

    QString mime = mimeType;

    if (!m_types.contains(mimeType)) {
        if (mimeType == plainText() && m_types.contains(utf8Text()))
            mime = utf8Text();
        else if (mimeType == uriList() && m_types.contains(mozUrl()))
            mime = mozUrl();
        else
            return QVariant();
    }

    int pipefd[2];
    if (bobui_safe_pipe(pipefd) == -1) {
        qWarning("QWaylandMimeData: pipe2() failed");
        return QVariant();
    }

    m_dataOffer->startReceiving(mime, pipefd[1]);

    close(pipefd[1]);

    QByteArray content;
    if (readData(pipefd[0], content) != 0) {
        qWarning("QWaylandDataOffer: error reading data for mimeType %s", qPrintable(mimeType));
        content = QByteArray();
    }

    close(pipefd[0]);

    content = convertData(mimeType, mime, content);

    if (mimeType != portalFileTransfer())
        m_data.insert(mimeType, content);

    return content;
}

int QWaylandMimeData::readData(int fd, QByteArray &data) const
{
    struct pollfd readset;
    readset.fd = fd;
    readset.events = POLLIN;

    Q_FOREVER {
        int ready = bobui_safe_poll(&readset, 1, QDeadlineTimer(1s));
        if (ready < 0) {
            qWarning() << "QWaylandDataOffer: bobui_safe_poll() failed";
            return -1;
        } else if (ready == 0) {
            qWarning("QWaylandDataOffer: timeout reading from pipe");
            return -1;
        } else {
            char buf[4096];
            int n = BOBUI_READ(fd, buf, sizeof buf);

            if (n < 0) {
                qWarning("QWaylandDataOffer: read() failed");
                return -1;
            } else if (n == 0) {
                return 0;
            } else if (n > 0) {
                data.append(buf, n);
            }
        }
    }
}

}

BOBUI_END_NAMESPACE
