// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QICONENGINE_P_H
#define QICONENGINE_P_H

#include <BobUIGui/private/bobuiguiglobal_p.h>

#ifndef BOBUI_NO_ICON
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

#include <BobUIGui/QIcon>
#include <BobUIGui/QIconEngine>

BOBUI_BEGIN_NAMESPACE

class QIconEngine;

class QProxyIconEngine : public QIconEngine
{
public:
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

    void addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state) override;
    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state) override;

    QString key() const override;
    QIconEngine *clone() const override;
    bool read(QDataStream &in) override;
    bool write(QDataStream &out) const override;

    QList<QSize> availableSizes(QIcon::Mode mode = QIcon::Normal,
                                    QIcon::State state = QIcon::Off) override;

    QString iconName() override;
    bool isNull() override;
    QPixmap scaledPixmap(const QSize &size, QIcon::Mode mode, QIcon::State state, qreal scale) override;

    void virtual_hook(int id, void *data) override;
protected:
    virtual QIconEngine *proxiedEngine() const = 0;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_ICON

#endif // QICONENGINE_P_H
