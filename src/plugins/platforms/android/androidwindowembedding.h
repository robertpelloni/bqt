// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIANDROIDWINDOWEMBEDDING_H
#define BOBUIANDROIDWINDOWEMBEDDING_H

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

#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjnitypes.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(View, "android/view/View");

namespace BobUIAndroidWindowEmbedding
{
    bool registerNatives(QJniEnvironment& env);
    void createRootWindow(JNIEnv *, jclass, BobUIJniTypes::View rootView,
                          jint x, jint y,jint width, jint height);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(createRootWindow)
    void deleteWindow(JNIEnv *, jclass, jlong window);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(deleteWindow)
    void setWindowVisible(JNIEnv *, jclass, jlong window, jboolean visible);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(setWindowVisible)
    void resizeWindow(JNIEnv *, jclass, jlong windowRef, jint x, jint y, jint width, jint height);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(resizeWindow)
};

BOBUI_END_NAMESPACE

#endif // BOBUIANDROIDWINDOWEMBEDDING_H
