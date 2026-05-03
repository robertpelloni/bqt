// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QINTERNALMIMEDATA_P_H
#define QINTERNALMIMEDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qbytearray.h>
#include <BobUICore/qmimedata.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qstringlist.h>
#include <BobUICore/qvariant.h>
#include <BobUIGui/private/bobuiguiglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QEventLoop;
class QMouseEvent;
class QPlatformDrag;

class Q_GUI_EXPORT QInternalMimeData : public QMimeData
{
    Q_OBJECT
public:
    QInternalMimeData();
    ~QInternalMimeData();

    bool hasFormat(const QString &mimeType) const override;
    QStringList formats() const override;
    static bool canReadData(const QString &mimeType);


    static QStringList formatsHelper(const QMimeData *data);
    static bool hasFormatHelper(const QString &mimeType, const QMimeData *data);
    static QByteArray renderDataHelper(const QString &mimeType, const QMimeData *data);

protected:
    QVariant retrieveData(const QString &mimeType, QMetaType type) const override;

    virtual bool hasFormat_sys(const QString &mimeType) const = 0;
    virtual QStringList formats_sys() const = 0;
    virtual QVariant retrieveData_sys(const QString &mimeType, QMetaType type) const = 0;
};

BOBUI_END_NAMESPACE

#endif // QINTERNALMIMEDATA_P_H
