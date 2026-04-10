// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
    QQuickView *view = new QQuickView(parent);
    view->create();
    QGuiApplication::platformNativeInterface()->setWindowProperty(view->handle(), "qnxWindowGroup",
                                                                  group);
//! [0]

//! [1]
    QQuickView *view = new QQuickView(parent);
    view->create();
    QGuiApplication::platformNativeInterface()->setWindowProperty(view->handle(), "qnxWindowGroup",
                                                                  QVariant());
//! [1]
