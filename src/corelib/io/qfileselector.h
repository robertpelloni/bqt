// Copyright (C) 2013 BlackBerry Limited. All rights reserved.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILESELECTOR_H
#define QFILESELECTOR_H

#include <BobUICore/QObject>
#include <BobUICore/QStringList>

BOBUI_BEGIN_NAMESPACE

class QFileSelectorPrivate;
class Q_CORE_EXPORT QFileSelector : public QObject
{
    Q_OBJECT
public:
    explicit QFileSelector(QObject *parent = nullptr);
    ~QFileSelector();

    QString select(const QString &filePath) const;
    QUrl select(const QUrl &filePath) const;

    QStringList extraSelectors() const;
    void setExtraSelectors(const QStringList &list);

    QStringList allSelectors() const;

private:
    Q_DECLARE_PRIVATE(QFileSelector)
};

BOBUI_END_NAMESPACE

#endif
