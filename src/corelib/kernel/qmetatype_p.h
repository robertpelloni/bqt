// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMETATYPE_P_H
#define QMETATYPE_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include "qmetatype.h"

BOBUI_BEGIN_NAMESPACE

#define QMETATYPE_CONVERTER(To, From, assign_and_return) \
    case makePair(QMetaType::To, QMetaType::From): \
        if constexpr (QMetaType::To == QMetaType::From) \
            Q_UNREACHABLE();  /* can never get here */ \
        if (onlyCheck) \
            return true; \
        { \
            const From &source = *static_cast<const From *>(from); \
            To &result = *static_cast<To *>(to); \
            assign_and_return \
        }
#define QMETATYPE_CONVERTER_ASSIGN(To, From) \
    QMETATYPE_CONVERTER(To, From, result = To(source); return true;)

#define QMETATYPE_CONVERTER_FUNCTION(To, assign_and_return) \
        { \
            To &result = *static_cast<To *>(r); \
            assign_and_return \
        }

struct QMetaTypeModuleHelper
{
    static constexpr auto makePair(int from, int to) -> quint64
    {
        return (quint64(from) << 32) + quint64(to);
    }

    static const BobUIPrivate::QMetaTypeInterface *interfaceForType_dummy(int)
    {
        return nullptr;
    }

    static bool convert_dummy(const void *, int, void *, int)
    {
        return false;
    }

    decltype(&interfaceForType_dummy) interfaceForType = &interfaceForType_dummy;
    decltype(&convert_dummy) convert = &convert_dummy;
};

extern Q_CORE_EXPORT QMetaTypeModuleHelper qMetaTypeGuiHelper;
extern Q_CORE_EXPORT QMetaTypeModuleHelper qMetaTypeWidgetsHelper;

namespace BobUIMetaTypePrivate {
template<typename T>
struct TypeDefinition
{
    static const bool IsAvailable = true;
};

// Ignore these types, as incomplete
#ifdef BOBUI_BOOTSTRAPPED
template<> struct TypeDefinition<qfloat16> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QBitArray> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QByteArrayList> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QCborArray> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QCborMap> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QCborSimpleType> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QCborValue> { static const bool IsAvailable = false; };
#if BOBUI_CONFIG(easingcurve)
template<> struct TypeDefinition<QEasingCurve> { static const bool IsAvailable = false; };
#endif
template<> struct TypeDefinition<QJsonArray> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QJsonDocument> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QJsonObject> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QJsonValue> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QUrl> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QUuid> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QRect> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QRectF> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QSize> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QSizeF> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QLine> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QLineF> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QPoint> { static const bool IsAvailable = false; };
template<> struct TypeDefinition<QPointF> { static const bool IsAvailable = false; };
#endif
#if !BOBUI_CONFIG(regularexpression)
template<> struct TypeDefinition<QRegularExpression> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_CURSOR
template<> struct TypeDefinition<QCursor> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_MATRIX4X4
template<> struct TypeDefinition<QMatrix4x4> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_VECTOR2D
template<> struct TypeDefinition<QVector2D> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_VECTOR3D
template<> struct TypeDefinition<QVector3D> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_VECTOR4D
template<> struct TypeDefinition<QVector4D> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_QUATERNION
template<> struct TypeDefinition<QQuaternion> { static const bool IsAvailable = false; };
#endif
#ifdef BOBUI_NO_ICON
template<> struct TypeDefinition<QIcon> { static const bool IsAvailable = false; };
#endif

template <typename T> inline bool isInterfaceFor(const BobUIPrivate::QMetaTypeInterface *iface)
{
    // typeId for built-in types are fixed and require no registration
    static_assert(QMetaTypeId2<T>::IsBuiltIn, "This function only works for built-in types");
    static constexpr int typeId = BobUIPrivate::BuiltinMetaType<T>::value;
    return iface->typeId.loadRelaxed() == typeId;
}

template <typename FPointer>
inline bool checkMetaTypeFlagOrPointer(const BobUIPrivate::QMetaTypeInterface *iface, FPointer ptr, QMetaType::TypeFlag Flag)
{
    // helper to the isXxxConstructible & isDestructible functions below: a
    // meta type has the trait if the trait is trivial or we have the pointer
    // to perform the operation
    Q_ASSERT(!isInterfaceFor<void>(iface));
    Q_ASSERT(iface->size);
    return ptr != nullptr || (iface->flags & Flag) == 0;
}

inline bool isDefaultConstructible(const BobUIPrivate::QMetaTypeInterface *iface) noexcept
{
    return checkMetaTypeFlagOrPointer(iface, iface->defaultCtr, QMetaType::NeedsConstruction);
}

inline bool isCopyConstructible(const BobUIPrivate::QMetaTypeInterface *iface) noexcept
{
    return checkMetaTypeFlagOrPointer(iface, iface->copyCtr, QMetaType::NeedsCopyConstruction);
}

inline bool isMoveConstructible(const BobUIPrivate::QMetaTypeInterface *iface) noexcept
{
    return checkMetaTypeFlagOrPointer(iface, iface->moveCtr, QMetaType::NeedsMoveConstruction);
}

inline bool isDestructible(const BobUIPrivate::QMetaTypeInterface *iface) noexcept
{
    /* For metatypes of revision 1, the NeedsDestruction was set even for trivially
       destructible types, but their dtor pointer would be null.
       For that reason, we need the additional check here.
     */
    return iface->revision < 1 ||
           checkMetaTypeFlagOrPointer(iface, iface->dtor, QMetaType::NeedsDestruction);
}

inline void defaultConstruct(const BobUIPrivate::QMetaTypeInterface *iface, void *where)
{
    Q_ASSERT(isDefaultConstructible(iface));
    if (iface->defaultCtr)
        iface->defaultCtr(iface, where);
    else
        memset(where, 0, iface->size);
}

inline void copyConstruct(const BobUIPrivate::QMetaTypeInterface *iface, void *where, const void *copy)
{
    Q_ASSERT(isCopyConstructible(iface));
    if (iface->copyCtr)
        iface->copyCtr(iface, where, copy);
    else
        memcpy(where, copy, iface->size);
}

inline void moveConstruct(const BobUIPrivate::QMetaTypeInterface *iface, void *where, void *copy)
{
    Q_ASSERT(isMoveConstructible(iface));
    if (iface->moveCtr)
        iface->moveCtr(iface, where, copy);
    else
        memcpy(where, copy, iface->size);
}

inline void construct(const BobUIPrivate::QMetaTypeInterface *iface, void *where, const void *copy)
{
    if (copy)
        copyConstruct(iface, where, copy);
    else
        defaultConstruct(iface, where);
}

inline void destruct(const BobUIPrivate::QMetaTypeInterface *iface, void *where)
{
    Q_ASSERT(isDestructible(iface));
    if (iface->dtor)
        iface->dtor(iface, where);
}

const char *typedefNameForType(const BobUIPrivate::QMetaTypeInterface *type_d);

template<typename T>
static const BOBUI_PREPEND_NAMESPACE(BobUIPrivate::QMetaTypeInterface) *getInterfaceFromType()
{
    if constexpr (BobUIMetaTypePrivate::TypeDefinition<T>::IsAvailable) {
        return &BOBUI_PREPEND_NAMESPACE(BobUIPrivate::QMetaTypeInterfaceWrapper)<T>::metaType;
    }
    return nullptr;
}

#define BOBUI_METATYPE_CONVERT_ID_TO_TYPE(MetaTypeName, MetaTypeId, RealName)                         \
    case QMetaType::MetaTypeName:                                                                  \
        return BobUIMetaTypePrivate::getInterfaceFromType<RealName>();

} //namespace BobUIMetaTypePrivate

BOBUI_END_NAMESPACE

#endif // QMETATYPE_P_H
