// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILEICONPROVIDER_H
#define QFILEICONPROVIDER_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qfileinfo.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUIGui/qicon.h>
#include <BobUIGui/qabstractfileiconprovider.h>

BOBUI_BEGIN_NAMESPACE


class QFileIconProviderPrivate;

class Q_WIDGETS_EXPORT QFileIconProvider : public QAbstractFileIconProvider
{
public:
    QFileIconProvider();
    ~QFileIconProvider();

    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo &info) const override;

private:
    Q_DECLARE_PRIVATE(QFileIconProvider)
    Q_DISABLE_COPY(QFileIconProvider)
};

BOBUI_END_NAMESPACE

#endif // QFILEICONPROVIDER_H
