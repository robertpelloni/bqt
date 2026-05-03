// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAINPUTCONTEXT_H
#define QCOCOAINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>
#include <BobUICore/QLocale>
#include <BobUICore/QPointer>

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

class QCocoaInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    explicit QCocoaInputContext();
    ~QCocoaInputContext();

    bool isValid() const override { return true; }

    void setFocusObject(QObject *object) override;

    void commit() override;
    void reset() override;

    QLocale locale() const override { return m_locale; }
    void updateLocale();

private:
    QPointer<QWindow> m_focusWindow;
    QLocale m_locale;
    QMacNotificationObserver m_inputSourceObserver;
};

BOBUI_END_NAMESPACE

#endif // QCOCOAINPUTCONTEXT_H
