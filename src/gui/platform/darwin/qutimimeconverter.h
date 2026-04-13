// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QUTIMIMECONVERTER_H
#define QUTIMIMECONVERTER_H

#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qlist.h>

BOBUI_BEGIN_NAMESPACE

class QByteArray;
class QString;
class QVariant;
class QMimeData;

class Q_GUI_EXPORT QUtiMimeConverter
{
    Q_DISABLE_COPY(QUtiMimeConverter)
public:
    enum class HandlerScopeFlag : uint8_t
    {
        DnD            = 0x01,
        Clipboard      = 0x02,
        BobUI_compatible  = 0x04,
        BobUI3_compatible = 0x08,
        All            = DnD|Clipboard,
        AllCompatible  = All|BobUI_compatible
    };
    Q_DECLARE_FLAGS(HandlerScope, HandlerScopeFlag)

    QUtiMimeConverter();
    virtual ~QUtiMimeConverter();

    HandlerScope scope() const { return m_scope; }
    bool canConvert(const QString &mime, const QString &uti) const { return mimeForUti(uti) == mime; }

    // for converting from BobUI
    virtual QList<QByteArray> convertFromMime(const QString &mime, const QVariant &data, const QString &uti) const = 0;
    virtual QString utiForMime(const QString &mime) const = 0;

    // for converting to BobUI
    virtual QString mimeForUti(const QString &uti) const = 0;
    virtual QVariant convertToMime(const QString &mime, const QList<QByteArray> &data, const QString &uti) const = 0;
    virtual int count(const QMimeData *mimeData) const;

private:
    friend class QMacMimeTypeName;
    friend class QMacMimeAny;

    explicit QUtiMimeConverter(HandlerScope scope);

    const HandlerScope m_scope;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QUtiMimeConverter::HandlerScope)


BOBUI_END_NAMESPACE

#endif

