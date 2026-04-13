// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFSCOMPLETOR_P_H
#define QFSCOMPLETOR_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qcompleter.h"
#include <BobUIGui/qfilesystemmodel.h>

BOBUI_REQUIRE_CONFIG(fscompleter);

BOBUI_BEGIN_NAMESPACE

/*!
    QCompleter that can deal with QFileSystemModel
  */
class Q_WIDGETS_EXPORT QFSCompleter :  public QCompleter {
public:
    explicit QFSCompleter(QFileSystemModel *model, QObject *parent = nullptr)
        : QCompleter(model, parent), proxyModel(nullptr), sourceModel(model)
    {
#if defined(Q_OS_WIN)
        setCaseSensitivity(BobUI::CaseInsensitive);
#endif
    }
    QString pathFromIndex(const QModelIndex &index) const override;
    QStringList splitPath(const QString& path) const override;

    QAbstractProxyModel *proxyModel;
    QFileSystemModel *sourceModel;
};

BOBUI_END_NAMESPACE

#endif // QFSCOMPLETOR_P_H

