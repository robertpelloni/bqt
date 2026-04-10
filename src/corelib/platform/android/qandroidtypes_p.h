// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDTYPES_P_H
#define QANDROIDTYPES_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <BobUICore/qjniobject.h>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjnitypes.h>

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_DECLARE_JNI_CLASS_STANDARD_TYPES
Q_DECLARE_JNI_CLASS(Void, "java/lang/Void");
Q_DECLARE_JNI_CLASS(Integer, "java/lang/Integer");
Q_DECLARE_JNI_CLASS(Long, "java/lang/Long");
Q_DECLARE_JNI_CLASS(Double, "java/lang/Double");
Q_DECLARE_JNI_CLASS(Float, "java/lang/Float");
Q_DECLARE_JNI_CLASS(Boolean, "java/lang/Boolean");
Q_DECLARE_JNI_CLASS(String, "java/lang/String");
Q_DECLARE_JNI_CLASS(Class, "java/lang/Class");

Q_DECLARE_JNI_CLASS(HashMap, "java/util/HashMap")
Q_DECLARE_JNI_CLASS(Set, "java/util/Set")
#endif

Q_DECLARE_JNI_CLASS(JBobUIAbstractItemModel, "org/bobuiproject/bobui/android/BobUIAbstractItemModel")
Q_DECLARE_JNI_CLASS(JBobUIAndroidItemModelProxy, "org/bobuiproject/bobui/android/BobUIAndroidItemModelProxy")
Q_DECLARE_JNI_CLASS(JBobUIModelIndex, "org/bobuiproject/bobui/android/BobUIModelIndex")

BOBUI_END_NAMESPACE

#endif // QANDROIDTYPES_P_H
