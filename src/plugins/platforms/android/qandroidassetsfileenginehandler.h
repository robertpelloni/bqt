// Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDASSETSFILEENGINEHANDLER_H
#define QANDROIDASSETSFILEENGINEHANDLER_H

#include <BobUICore/private/qabstractfileengine_p.h>
#include <QCache>
#include <QMutex>
#include <QSharedPointer>

#include <android/asset_manager.h>

BOBUI_BEGIN_NAMESPACE

class AndroidAssetsFileEngineHandler: public QAbstractFileEngineHandler
{
    Q_DISABLE_COPY_MOVE(AndroidAssetsFileEngineHandler)
public:
    AndroidAssetsFileEngineHandler();
    std::unique_ptr<QAbstractFileEngine> create(const QString &fileName) const override;

private:
    AAssetManager *m_assetManager;
};

BOBUI_END_NAMESPACE

#endif // QANDROIDASSETSFILEENGINEHANDLER_H
