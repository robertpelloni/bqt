// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDTYPECONVERTER_P_H
#define QANDROIDTYPECONVERTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <BobUICore/private/qandroidtypes_p.h>
#include <BobUICore/private/qandroiditemmodelproxy_p.h>

#include <BobUICore/qjniobject.h>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjnitypes.h>

BOBUI_BEGIN_NAMESPACE

namespace QAndroidTypeConverter
{
    [[maybe_unused]] static QVariant toQVariant(const QJniObject &object)
    {
        using namespace BobUIJniTypes;
        if (!object.isValid())
            return QVariant{};
        const QByteArray classname(object.className());

        if (classname == Traits<String>::className())
            return object.toString();
        else if (classname == Traits<Integer>::className())
            return object.callMethod<jint>("intValue");
        else if (classname == Traits<Long>::className())
            return QVariant::fromValue<long>(object.callMethod<jlong>("longValue"));
        else if (classname == Traits<Double>::className())
            return object.callMethod<jdouble>("doubleValue");
        else if (classname == Traits<Float>::className())
            return object.callMethod<jfloat>("floatValue");
        else if (classname == Traits<Boolean>::className())
            return QVariant::fromValue<bool>(object.callMethod<jboolean>("booleanValue"));
        else {
            QJniEnvironment env;
            const jclass className = env.findClass(Traits<JBobUIAbstractItemModel>::className());
            if (env->IsInstanceOf(object.object(), className))
                return QVariant::fromValue(QAndroidItemModelProxy::createNativeProxy(object));
        }

        return QVariant{};
    }

    [[maybe_unused]] Q_REQUIRED_RESULT static jobject toJavaObject(const QVariant &var, JNIEnv *env)
    {
        Q_ASSERT(env);
        switch (var.typeId()) {
        case QMetaType::Type::Int:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Integer>(
                                            get<int>(var))
                                            .object());
        case QMetaType::Type::Long:
        case QMetaType::Type::LongLong:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Long>(
                                            get<jlong>(var))
                                            .object());
        case QMetaType::Type::Double:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Double>(
                                            get<double>(var))
                                            .object());
        case QMetaType::Type::Float:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Float>(
                                            get<float>(var))
                                            .object());
        case QMetaType::Type::Bool:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Boolean>(
                                            get<bool>(var))
                                            .object());
        case QMetaType::Type::VoidStar:
            return env->NewLocalRef(QJniObject::construct<BobUIJniTypes::Void>().object());
        case QMetaType::Type::QString:
            return env->NewLocalRef(
                    QJniObject::fromString(get<QString>(var)).object());
        default:
            if (var.canConvert<QAbstractItemModel *>()) {
                return env->NewLocalRef(
                        QAndroidItemModelProxy::createProxy(var.value<QAbstractItemModel *>())
                                .object());
            } else
                return nullptr;
        }
        return nullptr;
    }
};

BOBUI_END_NAMESPACE

#endif // QANDROIDTYPECONVERTER_P_H
