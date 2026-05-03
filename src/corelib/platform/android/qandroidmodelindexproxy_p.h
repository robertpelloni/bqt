// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDMODELINDEXPROXY_P_H
#define QANDROIDMODELINDEXPROXY_P_H

#include <BobUICore/private/qandroidtypes_p.h>

#include <BobUICore/qabstractitemmodel.h>
#include <BobUICore/qjniobject.h>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjnitypes.h>

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

BOBUI_BEGIN_NAMESPACE

using namespace BobUIJniTypes;

class QAndroidItemModelProxy;

class Q_CORE_EXPORT QAndroidModelIndexProxy
{
public:
    static JBobUIModelIndex jInstance(QModelIndex modelIndex);
    static QModelIndex qInstance(JBobUIModelIndex jModelIndex);

    static jobject data(JNIEnv *env, jobject object, int role);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(data)

    static jlong internalId(JNIEnv *env, jobject object);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(internalId)

    static jboolean isValid(JNIEnv *env, jobject object);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(isValid)

    static JBobUIModelIndex parent(JNIEnv *env, jobject object);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(parent)

    static bool registerNatives(QJniEnvironment &env);
};

BOBUI_END_NAMESPACE

#endif // QANDROIDMODELINDEXPROXY_P_H
