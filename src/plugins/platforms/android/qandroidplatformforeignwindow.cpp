// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qandroidplatformforeignwindow.h"
#include "androidjnimain.h"
#include <BobUICore/qvariant.h>
#include <qpa/qwindowsysteminterface.h>
#include <BobUICore/private/qjnihelpers_p.h>
#include <BobUICore/qjnitypes.h>

BOBUI_BEGIN_NAMESPACE

QAndroidPlatformForeignWindow::QAndroidPlatformForeignWindow(QWindow *window, WId nativeHandle)
    : QAndroidPlatformWindow(window)
    , m_view(reinterpret_cast<jobject>(nativeHandle))
    , m_nativeViewInserted(false)
{
}

void QAndroidPlatformForeignWindow::initialize()
{
    QAndroidPlatformWindow::initialize();

    if (isEmbeddingContainer()) {
        m_nativeViewId = m_view.callMethod<jint>("getId");
        return;
    }

    if (m_view.isValid())
        BobUIAndroid::setViewVisibility(m_view.object(), false);
}

QAndroidPlatformForeignWindow::~QAndroidPlatformForeignWindow()
{
    if (isEmbeddingContainer())
        return;

    if (m_view.isValid())
        BobUIAndroid::setViewVisibility(m_view.object(), false);

    m_nativeBobUIWindow.callMethod<void>("removeNativeView");

}

void QAndroidPlatformForeignWindow::setVisible(bool visible)
{
    if (isEmbeddingContainer()) {
        QAndroidPlatformWindow::setVisible(visible);
        return;
    }

    if (!m_view.isValid())
        return;

    BobUIAndroid::setViewVisibility(m_view.object(), visible);
    m_nativeBobUIWindow.callMethod<void>("setVisible", visible);

    if (!visible && m_nativeViewInserted) {
        m_nativeBobUIWindow.callMethod<void>("removeNativeView");
        m_nativeViewInserted = false;
    } else if (!m_nativeViewInserted) {
        addViewToWindow();
    }
}

void QAndroidPlatformForeignWindow::applicationStateChanged(BobUI::ApplicationState state)
{
    if (!isEmbeddingContainer()) {
        if (state <= BobUI::ApplicationHidden
                && m_nativeViewInserted) {
            m_nativeBobUIWindow.callMethod<void>("removeNativeView");
            m_nativeViewInserted = false;
        } else if (m_view.isValid() && !m_nativeViewInserted){
            addViewToWindow();
        }
    }

    QAndroidPlatformWindow::applicationStateChanged(state);
}

WId QAndroidPlatformForeignWindow::winId() const
{
    if (isEmbeddingContainer() && m_view.isValid())
        return reinterpret_cast<WId>(m_view.object());
    if (m_nativeBobUIWindow.isValid())
        return reinterpret_cast<WId>(m_nativeBobUIWindow.object());
    return 0L;
}

void QAndroidPlatformForeignWindow::addViewToWindow()
{
    if (isEmbeddingContainer())
        return;

    m_nativeBobUIWindow.callMethod<void>("setNativeView", m_view);
    m_nativeViewInserted = true;
}

BOBUI_END_NAMESPACE
