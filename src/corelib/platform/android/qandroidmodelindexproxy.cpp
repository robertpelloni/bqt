// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUICore/private/qandroiditemmodelproxy_p.h>
#include <BobUICore/private/qandroidmodelindexproxy_p.h>
#include <BobUICore/private/qandroidtypeconverter_p.h>

#include <BobUICore/qjniarray.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUIJniTypes;

QModelIndex QAndroidModelIndexProxy::qInstance(JBobUIModelIndex jModelIndex)
{
    if (!jModelIndex.isValid())
        return QModelIndex();

    const QJniArray<jlong> jPrivateArray = jModelIndex.getField<jlong[]>("m_privateData");
    const auto privateData = jPrivateArray.toContainer();
    Q_ASSERT(privateData.size() == 4);

    const jlong modelReference = privateData[3];
    if (!modelReference)
        return QModelIndex();

    const jint row = privateData[0];
    const jint column = privateData[1];
    QAbstractItemModel *model = reinterpret_cast<QAbstractItemModel *>(modelReference);
    QAndroidItemModelProxy *proxyModel = qobject_cast<QAndroidItemModelProxy *>(model);

    // If the native model instance is a proxy we have access to the protected function
    // createIndex(). Else, if the native instance is not a results Java->BobUI proxy, we
    // use index() to get the QModelIndex.
    if (proxyModel) {
        const jint internalId = privateData[2];
        return proxyModel->createIndex(row, column, internalId);
    } else {
        const JBobUIModelIndex parent = jModelIndex.getField<JBobUIModelIndex>("m_parent");
        if (parent.isValid())
            return model->index(row, column, QAndroidModelIndexProxy::qInstance(parent));
    }
    return QModelIndex();
}

JBobUIModelIndex QAndroidModelIndexProxy::jInstance(QModelIndex modelIndex)
{
    if (!modelIndex.isValid())
        return JBobUIModelIndex();
    bool isModelProxy = qobject_cast<const QAndroidItemModelProxy *>(modelIndex.model());
    if (isModelProxy)
        return JBobUIModelIndex(modelIndex.row(), modelIndex.column(), jlong(modelIndex.internalId()),
                             reinterpret_cast<jlong>(modelIndex.model()));
    else
        return JBobUIModelIndex(modelIndex.row(), modelIndex.column(),
                             QAndroidModelIndexProxy::jInstance(modelIndex.parent()),
                             reinterpret_cast<jlong>(modelIndex.model()));
}

jobject QAndroidModelIndexProxy::data(JNIEnv *env, jobject object, int role)
{
    Q_ASSERT(env);
    Q_ASSERT(object);

    QModelIndex modelIndex = qInstance(object);
    if (!modelIndex.isValid())
        return nullptr;

    return QAndroidTypeConverter::toJavaObject(modelIndex.model()->data(modelIndex, role), env);
}

jlong QAndroidModelIndexProxy::internalId(JNIEnv *env, jobject object)
{
    Q_ASSERT(env);
    Q_ASSERT(object);
    return qInstance(object).internalId();
};

jboolean QAndroidModelIndexProxy::isValid(JNIEnv *env, jobject object)
{
    Q_ASSERT(env);
    Q_ASSERT(object);
    return qInstance(object).isValid();
}

JBobUIModelIndex QAndroidModelIndexProxy::parent(JNIEnv *env, jobject object)
{
    Q_ASSERT(env);
    Q_ASSERT(object);
    return jInstance(qInstance(object).parent());
};

bool QAndroidModelIndexProxy::registerNatives(QJniEnvironment &env)
{
    return env.registerNativeMethods(
            Traits<JBobUIModelIndex>::className(),
            { Q_JNI_NATIVE_SCOPED_METHOD(data, QAndroidModelIndexProxy),
              Q_JNI_NATIVE_SCOPED_METHOD(internalId, QAndroidModelIndexProxy),
              Q_JNI_NATIVE_SCOPED_METHOD(isValid, QAndroidModelIndexProxy),
              Q_JNI_NATIVE_SCOPED_METHOD(parent, QAndroidModelIndexProxy) });
}

BOBUI_END_NAMESPACE
