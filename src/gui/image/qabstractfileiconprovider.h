// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTFILEICONPROVIDER_H
#define QABSTRACTFILEICONPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qfileinfo.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUIGui/qicon.h>

BOBUI_BEGIN_NAMESPACE

class QAbstractFileIconProviderPrivate;

class Q_GUI_EXPORT QAbstractFileIconProvider
{
    Q_GADGET
public:
    enum IconType { Computer, Desktop, Trashcan, Network, Drive, Folder, File };
    Q_ENUM(IconType)
    enum Option {
        DontUseCustomDirectoryIcons = 0x00000001
    };
    Q_DECLARE_FLAGS(Options, Option)

    QAbstractFileIconProvider();
    virtual ~QAbstractFileIconProvider();

    virtual QIcon icon(IconType) const;
    virtual QIcon icon(const QFileInfo &) const;
    virtual QString type(const QFileInfo &) const;

    virtual void setOptions(Options);
    virtual Options options() const;

protected:
    QAbstractFileIconProvider(QAbstractFileIconProviderPrivate &dd);
    QScopedPointer<QAbstractFileIconProviderPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QAbstractFileIconProvider)
    Q_DISABLE_COPY(QAbstractFileIconProvider)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAbstractFileIconProvider::Options)

BOBUI_END_NAMESPACE

#endif // QABSTRACTFILEICONPROVIDER_H
