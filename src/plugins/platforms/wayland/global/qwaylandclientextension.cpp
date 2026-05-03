// Copyright (C) 2017 Erik Larsson.
// Copyright (C) 2021 David Redondo <bobui@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandclientextension.h"
#include "qwaylandclientextension_p.h"
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandintegration_p.h>

BOBUI_BEGIN_NAMESPACE

using RegistryGlobal = BobUIWaylandClient::QWaylandDisplay::RegistryGlobal;
using namespace BobUI::StringLiterals;

QWaylandClientExtensionPrivate::QWaylandClientExtensionPrivate()
{
    // Keep the possibility to use a custom waylandIntegration as a plugin,
    // but also add the possibility to run it as a QML component.
    waylandIntegration = BobUIWaylandClient::QWaylandIntegration::instance();
    if (!waylandIntegration)
        waylandIntegration = new BobUIWaylandClient::QWaylandIntegration("wayland"_L1);
}

void QWaylandClientExtensionPrivate::globalAdded(const RegistryGlobal &global)
{
    Q_Q(QWaylandClientExtension);
    if (!active && global.interface == QLatin1String(q->extensionInterface()->name)) {
        q->bind(global.registry, global.id, global.version);
        active = true;
        emit q->activeChanged();
    }
}

void QWaylandClientExtensionPrivate::globalRemoved(const RegistryGlobal &global)
{
    Q_Q(QWaylandClientExtension);
    if (active && global.interface == QLatin1String(q->extensionInterface()->name)) {
        active = false;
        emit q->activeChanged();
    }
}

/*!
    \class QWaylandClientExtension
    \internal
*/

void QWaylandClientExtension::initialize()
{
    Q_D(QWaylandClientExtension);
    if (d->active) {
        return;
    }
    const BobUIWaylandClient::QWaylandDisplay *display = d->waylandIntegration->display();
    const auto globals = display->globals();
    auto global =
            std::find_if(globals.cbegin(), globals.cend(), [this](const RegistryGlobal &global) {
                return global.interface == QLatin1String(extensionInterface()->name);
            });
    if (global != globals.cend()) {
        bind(global->registry, global->id, global->version);
        d->active = true;
        emit activeChanged();
    }
}

QWaylandClientExtension::QWaylandClientExtension(const int ver)
    : QObject(*new QWaylandClientExtensionPrivate())
{
    Q_D(QWaylandClientExtension);
    d->version = ver;
    auto display = d->waylandIntegration->display();
    QObjectPrivate::connect(display, &BobUIWaylandClient::QWaylandDisplay::globalAdded, d,
                            &QWaylandClientExtensionPrivate::globalAdded);
    QObjectPrivate::connect(display, &BobUIWaylandClient::QWaylandDisplay::globalRemoved, d,
                            &QWaylandClientExtensionPrivate::globalRemoved);
    // This function uses virtual functions and we don't want it to be called from the constructor.
    QMetaObject::invokeMethod(this, "initialize", BobUI::QueuedConnection);
}

QWaylandClientExtension::~QWaylandClientExtension()
{
}

BobUIWaylandClient::QWaylandIntegration *QWaylandClientExtension::integration() const
{
    Q_D(const QWaylandClientExtension);
    return d->waylandIntegration;
}

int QWaylandClientExtension::version() const
{
    Q_D(const QWaylandClientExtension);
    return d->version;
}

void QWaylandClientExtension::setVersion(const int ver)
{
    Q_D(QWaylandClientExtension);
    if (d->version != ver) {
        d->version = ver;
        emit versionChanged();
    }
}

bool QWaylandClientExtension::isActive() const
{
    Q_D(const QWaylandClientExtension);
    return d->active;
}

BOBUI_END_NAMESPACE

#include "moc_qwaylandclientextension.cpp"
