// Copyright (C) 2013 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMDIALOGHELPERS_H
#define QANDROIDPLATFORMDIALOGHELPERS_H

#include <jni.h>

#include <QEventLoop>
#include <BobUICore/QJniObject>
#include <qpa/qplatformdialoghelper.h>

BOBUI_BEGIN_NAMESPACE

class QJniEnvironment;

namespace BobUIAndroidDialogHelpers {

class QAndroidPlatformMessageDialogHelper: public QPlatformMessageDialogHelper
{
    Q_OBJECT
public:
    QAndroidPlatformMessageDialogHelper();
    ~QAndroidPlatformMessageDialogHelper();

    void exec() override;
    bool show(BobUI::WindowFlags windowFlags, BobUI::WindowModality windowModality,
              QWindow *parent) override;
    void hide() override;

public slots:
    void dialogResult(int buttonID);

private:
    void addButtons(QSharedPointer<QMessageDialogOptions> opt, ButtonRole role);

private:
    bool m_shown = false;
    QEventLoop m_loop;
    QJniObject m_javaMessageDialog;
};


bool registerNatives(QJniEnvironment &env);

}

BOBUI_END_NAMESPACE

#endif // QANDROIDPLATFORMDIALOGHELPERS_H
