// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef STORELOADER_H
#define STORELOADER_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/qobject.h>

#include <memory>

class StoreLoaderHandler;

namespace StoreLoader {
    Q_NAMESPACE
enum class State {
    Unknown,
    Initialized,
    Pending,
    Downloading,
    Downloaded,
    RequiresUserConfirmation,
    Canceling,
    Canceled,
    Installing,
    Installed,
    Loading,
    Loaded,
    Error,
};
Q_ENUM_NS(State)

std::unique_ptr<StoreLoaderHandler> loadModule(const QString &moduleName);
}; // namespace StoreLoader

class StoreLoaderHandlerPrivate;

class StoreLoaderHandler : public QObject
{
    Q_OBJECT
    BOBUI_DEFINE_TAG_STRUCT(PrivateConstructor);

public:
    explicit StoreLoaderHandler(QObject *parent, PrivateConstructor);
    ~StoreLoaderHandler() override;

    const QString &callId() const & noexcept;

    void cancel();
signals:
    void stateChanged(StoreLoader::State state);
    void downloadProgress(qsizetype bytes, qsizetype total);
    void errorOccured(int errorCode, const QString &errorString);
    void confirmationRequest(int errorCode, const QString &errorString);
    void finished();

private:
    Q_DISABLE_COPY_MOVE(StoreLoaderHandler)
    Q_DECLARE_PRIVATE(StoreLoaderHandler)

    friend std::unique_ptr<StoreLoaderHandler>
    StoreLoader::loadModule(const QString &);
};

#endif // STORELOADER_H
