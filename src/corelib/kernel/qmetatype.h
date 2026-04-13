// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2018 Intel Corporation.
// Copyright (C) 2014 Olivier Goffart <ogoffart@woboq.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMETATYPE_H
#define QMETATYPE_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qcompare.h>
#include <BobUICore/qdatastream.h>
#include <BobUICore/qflags.h>
#include <BobUICore/qfloat16.h>
#include <BobUICore/qhashfunctions.h>
#include <BobUICore/qiterable.h>
#include <BobUICore/qmetacontainer.h>
#ifndef BOBUI_NO_QOBJECT
#include <BobUICore/qobjectdefs.h>
#endif
#include <BobUICore/qscopeguard.h>
#include <BobUICore/bobuitypetraits.h>

#include <array>
#include <new>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <optional>
#include <BobUICore/qxptype_traits.h>

#ifdef Bool
#error qmetatype.h must be included before any header file that defines Bool
#endif

BOBUI_BEGIN_NAMESPACE

// from qcborcommon.h
enum class QCborSimpleType : quint8;

template <typename T>
struct QMetaTypeId2;

template <typename T>
inline constexpr int qMetaTypeId();

// F is a tuple: (QMetaType::TypeName, QMetaType::TypeNameID, RealType)
#define BOBUI_FOR_EACH_STATIC_PRIMITIVE_NON_VOID_TYPE(F)\
    F(Bool, 1, bool) \
    F(Int, 2, int) \
    F(UInt, 3, uint) \
    F(LongLong, 4, qlonglong) \
    F(ULongLong, 5, qulonglong) \
    F(Double, 6, double) \
    F(Long, 32, long) \
    F(Short, 33, short) \
    F(Char, 34, char) \
    F(Char16, 56, char16_t) \
    F(Char32, 57, char32_t) \
    F(ULong, 35, ulong) \
    F(UShort, 36, ushort) \
    F(UChar, 37, uchar) \
    F(Float, 38, float) \
    F(SChar, 40, signed char) \
    F(Nullptr, 51, std::nullptr_t) \
    F(QCborSimpleType, 52, QCborSimpleType) \

#define BOBUI_FOR_EACH_STATIC_PRIMITIVE_TYPE(F)        \
    BOBUI_FOR_EACH_STATIC_PRIMITIVE_NON_VOID_TYPE(F)   \
    F(Void, 43, void) \

#define BOBUI_FOR_EACH_STATIC_PRIMITIVE_POINTER(F)     \
    F(VoidStar, 31, void*) \

#if BOBUI_CONFIG(easingcurve)
#define BOBUI_FOR_EACH_STATIC_EASINGCURVE(F)\
    F(QEasingCurve, 29, QEasingCurve)
#else
#define BOBUI_FOR_EACH_STATIC_EASINGCURVE(F)
#endif

#if BOBUI_CONFIG(itemmodel)
#define BOBUI_FOR_EACH_STATIC_ITEMMODEL_CLASS(F)\
    F(QModelIndex, 42, QModelIndex) \
    F(QPersistentModelIndex, 50, QPersistentModelIndex)
#else
#define BOBUI_FOR_EACH_STATIC_ITEMMODEL_CLASS(F)
#endif

#if BOBUI_CONFIG(regularexpression)
#  define BOBUI_FOR_EACH_STATIC_REGULAR_EXPRESSION(F) \
    F(QRegularExpression, 44, QRegularExpression)
#else
#  define BOBUI_FOR_EACH_STATIC_REGULAR_EXPRESSION(F)
#endif
#ifndef BOBUI_NO_VARIANT
#  define BOBUI_FOR_EACH_STATIC_QVARIANT(F) \
    F(QVariant, 41, QVariant)
#else
#  define BOBUI_FOR_EACH_STATIC_QVARIANT(F)
#endif

#define BOBUI_FOR_EACH_STATIC_CORE_CLASS(F)\
    F(QChar, 7, QChar) \
    F(QString, 10, QString) \
    F(QByteArray, 12, QByteArray) \
    F(QBitArray, 13, QBitArray) \
    F(QDate, 14, QDate) \
    F(BOBUIime, 15, BOBUIime) \
    F(QDateTime, 16, QDateTime) \
    F(QUrl, 17, QUrl) \
    F(QLocale, 18, QLocale) \
    F(QRect, 19, QRect) \
    F(QRectF, 20, QRectF) \
    F(QSize, 21, QSize) \
    F(QSizeF, 22, QSizeF) \
    F(QLine, 23, QLine) \
    F(QLineF, 24, QLineF) \
    F(QPoint, 25, QPoint) \
    F(QPointF, 26, QPointF) \
    BOBUI_FOR_EACH_STATIC_EASINGCURVE(F) \
    F(QUuid, 30, QUuid) \
    BOBUI_FOR_EACH_STATIC_QVARIANT(F) \
    BOBUI_FOR_EACH_STATIC_REGULAR_EXPRESSION(F) \
    F(QJsonValue, 45, QJsonValue) \
    F(QJsonObject, 46, QJsonObject) \
    F(QJsonArray, 47, QJsonArray) \
    F(QJsonDocument, 48, QJsonDocument) \
    F(QCborValue, 53, QCborValue) \
    F(QCborArray, 54, QCborArray) \
    F(QCborMap, 55, QCborMap) \
    F(Float16, 63, qfloat16) \
    BOBUI_FOR_EACH_STATIC_ITEMMODEL_CLASS(F)

#define BOBUI_FOR_EACH_STATIC_CORE_POINTER(F)\
    F(QObjectStar, 39, QObject*)

#ifndef BOBUI_NO_VARIANT
#  define BOBUI_FOR_EACH_STATIC_CORE_QVARIANT_TEMPLATE(F) \
    F(QVariantMap, 8, QVariantMap) \
    F(QVariantList, 9, QVariantList) \
    F(QVariantHash, 28, QVariantHash) \
    F(QVariantPair, 58, QVariantPair) \
    /**/
#else
#  define BOBUI_FOR_EACH_STATIC_CORE_QVARIANT_TEMPLATE(F)
#endif // BOBUI_NO_VARIANT

#define BOBUI_FOR_EACH_STATIC_CORE_TEMPLATE(F) \
    BOBUI_FOR_EACH_STATIC_CORE_QVARIANT_TEMPLATE(F) \
    F(QByteArrayList, 49, QByteArrayList) \
    F(QStringList, 11, QStringList)

#if BOBUI_CONFIG(shortcut)
#define BOBUI_FOR_EACH_STATIC_KEYSEQUENCE_CLASS(F)\
    F(QKeySequence, 0x100b, QKeySequence)
#else
#define BOBUI_FOR_EACH_STATIC_KEYSEQUENCE_CLASS(F)
#endif

#define BOBUI_FOR_EACH_STATIC_GUI_CLASS(F)\
    F(QFont, 0x1000, QFont) \
    F(QPixmap, 0x1001, QPixmap) \
    F(QBrush, 0x1002, QBrush) \
    F(QColor, 0x1003, QColor) \
    F(QPalette, 0x1004, QPalette) \
    F(QIcon, 0x1005, QIcon) \
    F(QImage, 0x1006, QImage) \
    F(QPolygon, 0x1007, QPolygon) \
    F(QRegion, 0x1008, QRegion) \
    F(QBitmap, 0x1009, QBitmap) \
    F(QCursor, 0x100a, QCursor) \
    BOBUI_FOR_EACH_STATIC_KEYSEQUENCE_CLASS(F) \
    F(QPen, 0x100c, QPen) \
    F(BOBUIextLength, 0x100d, BOBUIextLength) \
    F(BOBUIextFormat, 0x100e, BOBUIextFormat) \
    F(BOBUIransform, 0x1010, BOBUIransform) \
    F(QMatrix4x4, 0x1011, QMatrix4x4) \
    F(QVector2D, 0x1012, QVector2D) \
    F(QVector3D, 0x1013, QVector3D) \
    F(QVector4D, 0x1014, QVector4D) \
    F(QQuaternion, 0x1015, QQuaternion) \
    F(QPolygonF, 0x1016, QPolygonF) \
    F(QColorSpace, 0x1017, QColorSpace) \


#define BOBUI_FOR_EACH_STATIC_WIDGETS_CLASS(F)\
    F(QSizePolicy, 0x2000, QSizePolicy) \

// F is a tuple: (QMetaType::TypeName, QMetaType::TypeNameID, AliasingType, "RealType")
#define BOBUI_FOR_EACH_STATIC_ALIAS_TYPE(F)\
    F(ULong, -1, ulong, "unsigned long") \
    F(UInt, -1, uint, "unsigned int") \
    F(UShort, -1, ushort, "unsigned short") \
    F(UChar, -1, uchar, "unsigned char") \
    F(LongLong, -1, qlonglong, "long long") \
    F(ULongLong, -1, qulonglong, "unsigned long long") \
    F(SChar, -1, signed char, "qint8") \
    F(UChar, -1, uchar, "quint8") \
    F(Short, -1, short, "qint16") \
    F(UShort, -1, ushort, "quint16") \
    F(Int, -1, int, "qint32") \
    F(UInt, -1, uint, "quint32") \
    F(LongLong, -1, qlonglong, "qint64") \
    F(ULongLong, -1, qulonglong, "quint64") \
    F(QByteArrayList, -1, QByteArrayList, "QList<QByteArray>") \
    F(QStringList, -1, QStringList, "QList<QString>") \
    BOBUI_FOR_EACH_STATIC_VARIANT_ALIAS_TYPE(F)

#ifndef BOBUI_NO_VARIANT
#define BOBUI_FOR_EACH_STATIC_VARIANT_ALIAS_TYPE(F) \
    F(QVariantList, -1, QVariantList, "QList<QVariant>") \
    F(QVariantMap, -1, QVariantMap, "QMap<QString,QVariant>") \
    F(QVariantHash, -1, QVariantHash, "QHash<QString,QVariant>") \
    F(QVariantPair, -1, QVariantPair, "QPair<QVariant,QVariant>") \
    /**/
#else
#define BOBUI_FOR_EACH_STATIC_VARIANT_ALIAS_TYPE(F)
#endif

#define BOBUI_FOR_EACH_STATIC_TYPE(F)\
    BOBUI_FOR_EACH_STATIC_PRIMITIVE_TYPE(F)\
    BOBUI_FOR_EACH_STATIC_PRIMITIVE_POINTER(F)\
    BOBUI_FOR_EACH_STATIC_CORE_CLASS(F)\
    BOBUI_FOR_EACH_STATIC_CORE_POINTER(F)\
    BOBUI_FOR_EACH_STATIC_CORE_TEMPLATE(F)\
    BOBUI_FOR_EACH_STATIC_GUI_CLASS(F)\
    BOBUI_FOR_EACH_STATIC_WIDGETS_CLASS(F)\

#define BOBUI_DEFINE_METATYPE_ID(TypeName, Id, Name) \
    TypeName = Id,

#define BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_1ARG(F) \
    F(QList) \
    F(QQueue) \
    F(QStack) \
    F(QSet) \
    /*end*/

#define BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_2ARG(F) \
    F(QHash, class) \
    F(QMap, class)

#define BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_SMART_POINTER(F) \
    F(QSharedPointer) \
    F(QWeakPointer) \
    F(QPointer)

class QDataStream;
struct QMetaObject;

namespace BobUIPrivate
{

class QMetaTypeInterface;

// MSVC is the only supported compiler that includes the type of a variable in
// its mangled form, so it's not binary-compatible to drop the const in
// QMetaTypeInterfaceWrapper::metaType for it, which means we must keep the
// mutable field until BobUI 7.
#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0) || defined(BOBUI_BOOTSTRAPPED) || !defined(Q_CC_MSVC)
#  define QMTI_MUTABLE
using NonConstMetaTypeInterface = QMetaTypeInterface;
#else
#  define QMTI_MUTABLE mutable
using NonConstMetaTypeInterface = const QMetaTypeInterface;
#endif

class QMetaTypeInterface
{
public:

    /* Revision: Can increase if new field are added, or if semantics changes
       0: Initial Revision
       1: the meaning of the NeedsDestruction flag changed
    */
    static inline constexpr ushort CurrentRevision = 1;

    ushort revision;
    ushort alignment;
    uint size;
    uint flags;
    QMTI_MUTABLE QBasicAtomicInt typeId;

    using MetaObjectFn = const QMetaObject *(*)(const QMetaTypeInterface *);
    MetaObjectFn metaObjectFn;

    const char *name;

    using DefaultCtrFn = void (*)(const QMetaTypeInterface *, void *);
    DefaultCtrFn defaultCtr;
    using CopyCtrFn = void (*)(const QMetaTypeInterface *, void *, const void *);
    CopyCtrFn copyCtr;
    using MoveCtrFn = void (*)(const QMetaTypeInterface *, void *, void *);
    MoveCtrFn moveCtr;
    using DtorFn = void (*)(const QMetaTypeInterface *, void *);
    DtorFn dtor;
    using EqualsFn = bool (*)(const QMetaTypeInterface *, const void *, const void *);
    EqualsFn equals;
    using LessThanFn = bool (*)(const QMetaTypeInterface *, const void *, const void *);
    LessThanFn lessThan;
    using DebugStreamFn = void (*)(const QMetaTypeInterface *, QDebug &, const void *);
    DebugStreamFn debugStream;
    using DataStreamOutFn = void (*)(const QMetaTypeInterface *, QDataStream &, const void *);
    DataStreamOutFn dataStreamOut;
    using DataStreamInFn = void (*)(const QMetaTypeInterface *, QDataStream &, void *);
    DataStreamInFn dataStreamIn;

    using LegacyRegisterOp = void (*)();
    LegacyRegisterOp legacyRegisterOp;
};
#undef QMTI_MUTABLE

/*!
    This template is used for implicit conversion from type From to type To.
    \internal
*/
template<typename From, typename To>
To convertImplicit(const From& from)
{
    return from;
}

    template<typename T, bool>
    struct SequentialValueTypeIsMetaType;
    template<typename T, bool>
    struct AssociativeValueTypeIsMetaType;
    template<typename T, bool>
    struct IsMetaTypePair;
    template<typename, typename>
    struct MetaTypeSmartPointerHelper;

    template<typename T>
    struct IsEnumOrFlags : std::disjunction<std::is_enum<T>, IsQFlags<T>> {};
}  // namespace BobUIPrivate

class Q_CORE_EXPORT QMetaType {
public:
#ifndef Q_QDOC
    // The code that actually gets compiled.
    enum Type {
        // these are merged with QVariant
        BOBUI_FOR_EACH_STATIC_TYPE(BOBUI_DEFINE_METATYPE_ID)

        FirstCoreType = Bool,
        LastCoreType = Float16,
        FirstGuiType = QFont,
        LastGuiType = QColorSpace,
        FirstWidgetsType = QSizePolicy,
        LastWidgetsType = QSizePolicy,
        HighestInternalId = LastWidgetsType,

        QReal = sizeof(qreal) == sizeof(double) ? Double : Float,

        UnknownType = 0,
        User = 65536
    };
#else
    // If we are using QDoc it fakes the Type enum looks like this.
    enum Type {
        UnknownType = 0, Bool = 1, Int = 2, UInt = 3, LongLong = 4, ULongLong = 5,
        Double = 6, Long = 32, Short = 33, Char = 34, ULong = 35, UShort = 36,
        UChar = 37, Float = 38,
        VoidStar = 31,
        QChar = 7, QString = 10, QStringList = 11, QByteArray = 12,
        QBitArray = 13, QDate = 14, BOBUIime = 15, QDateTime = 16, QUrl = 17,
        QLocale = 18, QRect = 19, QRectF = 20, QSize = 21, QSizeF = 22,
        QLine = 23, QLineF = 24, QPoint = 25, QPointF = 26,
        QEasingCurve = 29, QUuid = 30, QVariant = 41, QModelIndex = 42,
        QPersistentModelIndex = 50, QRegularExpression = 44,
        QJsonValue = 45, QJsonObject = 46, QJsonArray = 47, QJsonDocument = 48,
        QByteArrayList = 49, QObjectStar = 39, SChar = 40,
        Void = 43,
        Nullptr = 51,
        QVariantMap = 8, QVariantList = 9, QVariantHash = 28, QVariantPair = 58,
        QCborSimpleType = 52, QCborValue = 53, QCborArray = 54, QCborMap = 55,
        Char16 = 56, Char32 = 57,
        Int128 = 59, UInt128 = 60, Float128 = 61, BFloat16 = 62, Float16 = 63,

        // Gui types
        QFont = 0x1000, QPixmap = 0x1001, QBrush = 0x1002, QColor = 0x1003, QPalette = 0x1004,
        QIcon = 0x1005, QImage = 0x1006, QPolygon = 0x1007, QRegion = 0x1008, QBitmap = 0x1009,
        QCursor = 0x100a, QKeySequence = 0x100b, QPen = 0x100c, BOBUIextLength = 0x100d, BOBUIextFormat = 0x100e,
        BOBUIransform = 0x1010, QMatrix4x4 = 0x1011, QVector2D = 0x1012,
        QVector3D = 0x1013, QVector4D = 0x1014, QQuaternion = 0x1015, QPolygonF = 0x1016, QColorSpace = 0x1017,

        // Widget types
        QSizePolicy = 0x2000,

        // Start-point for client-code types:
        User = 65536
    };
#endif

    enum TypeFlag {
        NeedsConstruction = 0x1,
        NeedsDestruction = 0x2,
        RelocatableType = 0x4,
#if BOBUI_DEPRECATED_SINCE(6, 0)
        MovableType Q_DECL_ENUMERATOR_DEPRECATED_X("Use RelocatableType instead.") = RelocatableType,
#endif
        PointerToQObject = 0x8,
        IsEnumeration = 0x10,
        SharedPointerToQObject = 0x20,
        WeakPointerToQObject = 0x40,
        TrackingPointerToQObject = 0x80,
        IsUnsignedEnumeration = 0x100,
        IsGadget = 0x200,
        PointerToGadget = 0x400,
        IsPointer = 0x800,
        IsQmlList =0x1000, // used in the QML engine to recognize QQmlListProperty<T> and list<T>
        IsConst = 0x2000,
        // since 6.5:
        NeedsCopyConstruction = 0x4000,
        NeedsMoveConstruction = 0x8000,
    };
    Q_DECLARE_FLAGS(TypeFlags, TypeFlag)

    static void registerNormalizedTypedef(const BOBUI_PREPEND_NAMESPACE(QByteArray) &normalizedTypeName, QMetaType type);

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_6_0
    static int type(const char *typeName)
    { return QMetaType::fromName(typeName).id(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static int type(const BOBUI_PREPEND_NAMESPACE(QByteArray) &typeName)
    { return QMetaType::fromName(typeName).id(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static const char *typeName(int type)
    { return QMetaType(type).name(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static int sizeOf(int type)
    { return int(QMetaType(type).sizeOf()); }
    BOBUI_DEPRECATED_VERSION_6_0
    static TypeFlags typeFlags(int type)
    { return QMetaType(type).flags(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static const QMetaObject *metaObjectForType(int type)
    { return QMetaType(type).metaObject(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static void *create(int type, const void *copy = nullptr)
    { return QMetaType(type).create(copy); }
    BOBUI_DEPRECATED_VERSION_6_0
    static void destroy(int type, void *data)
    { return QMetaType(type).destroy(data); }
    BOBUI_DEPRECATED_VERSION_6_0
    static void *construct(int type, void *where, const void *copy)
    { return QMetaType(type).construct(where, copy); }
    BOBUI_DEPRECATED_VERSION_6_0
    static void destruct(int type, void *where)
    { return QMetaType(type).destruct(where); }
#endif
    static bool isRegistered(int type);

    explicit QMetaType(int type);
    explicit constexpr QMetaType(const BobUIPrivate::QMetaTypeInterface *d) : d_ptr(d) {}
    constexpr QMetaType() = default;

#if BOBUI_CORE_REMOVED_SINCE(6, 9)
    bool isValid() const;
    bool isRegistered() const;
#endif
    constexpr bool isValid(BOBUI6_DECL_NEW_OVERLOAD) const noexcept;
    inline bool isRegistered(BOBUI6_DECL_NEW_OVERLOAD) const noexcept;
    void registerType() const
    {
        // "register" is a reserved keyword
        registerHelper();
    }
#if BOBUI_CORE_REMOVED_SINCE(6, 1) || defined(Q_QDOC)
    int id() const;
#else
    // ### BobUI 7: Remove traces of out of line version
    // unused int parameter is used to avoid ODR violation
    int id(int = 0) const
    {
        return registerHelper();
    }
#endif
    constexpr qsizetype sizeOf() const;
    constexpr qsizetype alignOf() const;
    constexpr TypeFlags flags() const;
    constexpr const QMetaObject *metaObject() const;
    constexpr const char *name() const;

    void *create(const void *copy = nullptr) const;
    void destroy(void *data) const;
    void *construct(void *where, const void *copy = nullptr) const;
    void destruct(void *data) const;
    QPartialOrdering compare(const void *lhs, const void *rhs) const;
    bool equals(const void *lhs, const void *rhs) const;

    bool isDefaultConstructible() const noexcept { return d_ptr && isDefaultConstructible(d_ptr); }
    bool isCopyConstructible() const noexcept { return d_ptr && isCopyConstructible(d_ptr); }
    bool isMoveConstructible() const noexcept { return d_ptr && isMoveConstructible(d_ptr); }
    bool isDestructible() const noexcept { return d_ptr && isDestructible(d_ptr); }
    bool isEqualityComparable() const;
    bool isOrdered() const;

#ifndef BOBUI_NO_DATASTREAM
    bool save(QDataStream &stream, const void *data) const;
    bool load(QDataStream &stream, void *data) const;
    bool hasRegisteredDataStreamOperators() const;

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_6_0
    static bool save(QDataStream &stream, int type, const void *data)
    { return QMetaType(type).save(stream, data); }
    BOBUI_DEPRECATED_VERSION_6_0
    static bool load(QDataStream &stream, int type, void *data)
    { return QMetaType(type).load(stream, data); }
#endif
#endif

    QMetaType underlyingType() const;

    template<typename T>
    constexpr static QMetaType fromType();
    static QMetaType fromName(QByteArrayView name);
private:
    friend bool comparesEqual(const QMetaType &lhs,
                              const QMetaType &rhs)
    {
        if (lhs.d_ptr == rhs.d_ptr)
            return true;
        if (!lhs.d_ptr || !rhs.d_ptr)
            return false; // one type is undefined, the other is defined
        // avoid id call if we already have the id
        const int aId = lhs.id();
        const int bId = rhs.id();
        return aId == bId;
    }
    Q_DECLARE_EQUALITY_COMPARABLE_NON_NOEXCEPT(QMetaType)
#ifndef BOBUI_NO_DEBUG_STREAM
private:
    friend Q_CORE_EXPORT QDebug operator<<(QDebug d, QMetaType m);
public:
    bool debugStream(QDebug& dbg, const void *rhs);
    bool hasRegisteredDebugStreamOperator() const;

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_6_0
    static bool debugStream(QDebug& dbg, const void *rhs, int typeId)
    { return QMetaType(typeId).debugStream(dbg, rhs); }
    template<typename T>
    BOBUI_DEPRECATED_VERSION_6_0
    static bool hasRegisteredDebugStreamOperator()
    { return QMetaType::fromType<T>().hasRegisteredDebugStreamOperator(); }
    BOBUI_DEPRECATED_VERSION_6_0
    static bool hasRegisteredDebugStreamOperator(int typeId)
    { return QMetaType(typeId).hasRegisteredDebugStreamOperator(); }
#endif
#endif

public:
    // type erased converter function
    using ConverterFunction = std::function<bool(const void *src, void *target)>;

    // type erased mutable view, primarily for containers
    using MutableViewFunction = std::function<bool(void *src, void *target)>;

    // implicit conversion supported like double -> float
    template<typename From, typename To>
    static bool registerConverter()
    {
        return registerConverter<From, To>(BobUIPrivate::convertImplicit<From, To>);
    }

    // member function as in "QString QFont::toString() const"
    template<typename From, typename To>
    static bool registerConverter(To(From::*function)() const)
    {
        static_assert((!QMetaTypeId2<To>::IsBuiltIn || !QMetaTypeId2<From>::IsBuiltIn),
            "QMetaType::registerConverter: At least one of the types must be a custom type.");

        const QMetaType fromType = QMetaType::fromType<From>();
        const QMetaType toType = QMetaType::fromType<To>();
        auto converter = [function](const void *from, void *to) -> bool {
            const From *f = static_cast<const From *>(from);
            To *t = static_cast<To *>(to);
            *t = (f->*function)();
            return true;
        };
        return registerConverterImpl<From, To>(converter, fromType, toType);
    }

    // member function
    template<typename From, typename To>
    static bool registerMutableView(To(From::*function)())
    {
        static_assert((!QMetaTypeId2<To>::IsBuiltIn || !QMetaTypeId2<From>::IsBuiltIn),
            "QMetaType::registerMutableView: At least one of the types must be a custom type.");

        const QMetaType fromType = QMetaType::fromType<From>();
        const QMetaType toType = QMetaType::fromType<To>();
        auto view = [function](void *from, void *to) -> bool {
            From *f = static_cast<From *>(from);
            To *t = static_cast<To *>(to);
            *t = (f->*function)();
            return true;
        };
        return registerMutableViewImpl<From, To>(view, fromType, toType);
    }

    // member function as in "double QString::toDouble(bool *ok = nullptr) const"
    template<typename From, typename To>
    static bool registerConverter(To(From::*function)(bool*) const)
    {
        static_assert((!QMetaTypeId2<To>::IsBuiltIn || !QMetaTypeId2<From>::IsBuiltIn),
            "QMetaType::registerConverter: At least one of the types must be a custom type.");

        const QMetaType fromType = QMetaType::fromType<From>();
        const QMetaType toType = QMetaType::fromType<To>();
        auto converter = [function](const void *from, void *to) -> bool {
            const From *f = static_cast<const From *>(from);
            To *t = static_cast<To *>(to);
            bool result = true;
            *t = (f->*function)(&result);
            if (!result)
                *t = To();
            return result;
        };
        return registerConverterImpl<From, To>(converter, fromType, toType);
    }

    // functor or function pointer
    template<typename From, typename To, typename UnaryFunction>
    static bool registerConverter(UnaryFunction function)
    {
        static_assert((!QMetaTypeId2<To>::IsBuiltIn || !QMetaTypeId2<From>::IsBuiltIn),
            "QMetaType::registerConverter: At least one of the types must be a custom type.");

        const QMetaType fromType = QMetaType::fromType<From>();
        const QMetaType toType = QMetaType::fromType<To>();
        auto converter = [function = std::move(function)](const void *from, void *to) -> bool {
            const From *f = static_cast<const From *>(from);
            To *t = static_cast<To *>(to);
            auto &&r = function(*f);
            if constexpr (std::is_same_v<q20::remove_cvref_t<decltype(r)>, std::optional<To>>) {
                if (!r)
                    return false;
                *t = *std::forward<decltype(r)>(r);
            } else {
                *t = std::forward<decltype(r)>(r);
            }
            return true;
        };
        return registerConverterImpl<From, To>(std::move(converter), fromType, toType);
    }

    // functor or function pointer
    template<typename From, typename To, typename UnaryFunction>
    static bool registerMutableView(UnaryFunction function)
    {
        static_assert((!QMetaTypeId2<To>::IsBuiltIn || !QMetaTypeId2<From>::IsBuiltIn),
            "QMetaType::registerMutableView: At least one of the types must be a custom type.");

        const QMetaType fromType = QMetaType::fromType<From>();
        const QMetaType toType = QMetaType::fromType<To>();
        auto view = [function = std::move(function)](void *from, void *to) -> bool {
            From *f = static_cast<From *>(from);
            To *t = static_cast<To *>(to);
            *t = function(*f);
            return true;
        };
        return registerMutableViewImpl<From, To>(std::move(view), fromType, toType);
    }

private:
    template<typename From, typename To>
    static bool registerConverterImpl(ConverterFunction converter, QMetaType fromType, QMetaType toType)
    {
        if (registerConverterFunction(std::move(converter), fromType, toType)) {
            static const auto unregister = qScopeGuard([=] {
                unregisterConverterFunction(fromType, toType);
            });
            return true;
        } else {
            return false;
        }
    }

    template<typename From, typename To>
    static bool registerMutableViewImpl(MutableViewFunction view, QMetaType fromType, QMetaType toType)
    {
        if (registerMutableViewFunction(std::move(view), fromType, toType)) {
            static const auto unregister = qScopeGuard([=] {
               unregisterMutableViewFunction(fromType, toType);
            });
            return true;
        } else {
            return false;
        }
    }
public:

    static bool convert(QMetaType fromType, const void *from, QMetaType toType, void *to);
    static bool canConvert(QMetaType fromType, QMetaType toType);

    static bool view(QMetaType fromType, void *from, QMetaType toType, void *to);
    static bool canView(QMetaType fromType, QMetaType toType);
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_6_0
    static bool convert(const void *from, int fromTypeId, void *to, int toTypeId)
    { return convert(QMetaType(fromTypeId), from, QMetaType(toTypeId), to); }
    BOBUI_DEPRECATED_VERSION_6_0
    static bool compare(const void *lhs, const void *rhs, int typeId, int *result)
    {
        QMetaType t(typeId);
        auto c = t.compare(lhs, rhs);
        if (c == QPartialOrdering::Unordered) {
            *result = 0;
            return false;
        } else if (c == QPartialOrdering::Less) {
            *result = -1;
            return true;
        } else if (c == QPartialOrdering::Equivalent) {
            *result = 0;
            return true;
        } else {
            *result = 1;
            return true;
        }
    }
    BOBUI_DEPRECATED_VERSION_6_0
    static bool equals(const void *lhs, const void *rhs, int typeId, int *result)
    {
        QMetaType t(typeId);
        if (!t.isEqualityComparable())
            return false;
        *result = t.equals(lhs, rhs) ? 0 : -1;
        return true;
    }
#endif

    template<typename From, typename To>
    static bool hasRegisteredConverterFunction()
    {
        return hasRegisteredConverterFunction(
                    QMetaType::fromType<From>(), QMetaType::fromType<To>());
    }

    static bool hasRegisteredConverterFunction(QMetaType fromType, QMetaType toType);

    template<typename From, typename To>
    static bool hasRegisteredMutableViewFunction()
    {
        return hasRegisteredMutableViewFunction(
                    QMetaType::fromType<From>(), QMetaType::fromType<To>());
    }

    static bool hasRegisteredMutableViewFunction(QMetaType fromType, QMetaType toType);

#ifndef Q_QDOC
    template<typename, bool> friend struct BobUIPrivate::SequentialValueTypeIsMetaType;
    template<typename, bool> friend struct BobUIPrivate::AssociativeValueTypeIsMetaType;
    template<typename, bool> friend struct BobUIPrivate::IsMetaTypePair;
    template<typename, typename> friend struct BobUIPrivate::MetaTypeSmartPointerHelper;
#endif
    static bool registerConverterFunction(const ConverterFunction &f, QMetaType from, QMetaType to);
    static void unregisterConverterFunction(QMetaType from, QMetaType to);

    static bool registerMutableViewFunction(const MutableViewFunction &f, QMetaType from, QMetaType to);
    static void unregisterMutableViewFunction(QMetaType from, QMetaType to);

    static void unregisterMetaType(QMetaType type);

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    constexpr const BobUIPrivate::QMetaTypeInterface *iface() { return d_ptr; }
#endif
    constexpr const BobUIPrivate::QMetaTypeInterface *iface() const { return d_ptr; }

private:
    Q_DECL_PURE_FUNCTION static bool isDefaultConstructible(const BobUIPrivate::QMetaTypeInterface *) noexcept;
    Q_DECL_PURE_FUNCTION static bool isCopyConstructible(const BobUIPrivate::QMetaTypeInterface *) noexcept;
    Q_DECL_PURE_FUNCTION static bool isMoveConstructible(const BobUIPrivate::QMetaTypeInterface *) noexcept;
    Q_DECL_PURE_FUNCTION static bool isDestructible(const BobUIPrivate::QMetaTypeInterface *) noexcept;

#if BOBUI_CORE_REMOVED_SINCE(6, 5)
    int idHelper() const;
#endif
    static int registerHelper(const BobUIPrivate::QMetaTypeInterface *iface);
    int registerHelper() const
    {
        if (d_ptr) {
            if (int id = d_ptr->typeId.loadRelaxed())
                return id;
            return registerHelper(d_ptr);
        }
        return 0;
    }

    friend int qRegisterMetaType(QMetaType meta);

    friend class QVariant;
    const BobUIPrivate::QMetaTypeInterface *d_ptr = nullptr;
};

#undef BOBUI_DEFINE_METATYPE_ID

Q_DECLARE_OPERATORS_FOR_FLAGS(QMetaType::TypeFlags)

#define BOBUI_METATYPE_PRIVATE_DECLARE_TYPEINFO(C, F)  \
    }                                               \
    Q_DECLARE_TYPEINFO(BobUIMetaTypePrivate:: C, (F)); \
    namespace BobUIMetaTypePrivate {


namespace BobUIMetaTypePrivate {

class QPairVariantInterfaceImpl
{
public:
    const void *_pair;
    QMetaType _metaType_first;
    QMetaType _metaType_second;

    typedef void (*getFunc)(const void * const *p, void *);

    getFunc _getFirst;
    getFunc _getSecond;

    template<class T>
    static void getFirstImpl(const void * const *pair, void *dataPtr)
    { *static_cast<typename T::first_type *>(dataPtr) = static_cast<const T*>(*pair)->first; }
    template<class T>
    static void getSecondImpl(const void * const *pair, void *dataPtr)
    { *static_cast<typename T::second_type *>(dataPtr) = static_cast<const T*>(*pair)->second; }

public:
    template<class T> QPairVariantInterfaceImpl(const T*p)
      : _pair(p)
      , _metaType_first(QMetaType::fromType<typename T::first_type>())
      , _metaType_second(QMetaType::fromType<typename T::second_type>())
      , _getFirst(getFirstImpl<T>)
      , _getSecond(getSecondImpl<T>)
    {
    }

    constexpr QPairVariantInterfaceImpl()
      : _pair(nullptr)
      , _getFirst(nullptr)
      , _getSecond(nullptr)
    {
    }

    inline void first(void *dataPtr) const { _getFirst(&_pair, dataPtr); }
    inline void second(void *dataPtr) const { _getSecond(&_pair, dataPtr); }
};
BOBUI_METATYPE_PRIVATE_DECLARE_TYPEINFO(QPairVariantInterfaceImpl, Q_RELOCATABLE_TYPE)

template<typename From>
struct QPairVariantInterfaceConvertFunctor;

template<typename T, typename U>
struct QPairVariantInterfaceConvertFunctor<std::pair<T, U> >
{
    QPairVariantInterfaceImpl operator()(const std::pair<T, U>& f) const
    {
        return QPairVariantInterfaceImpl(&f);
    }
};

}

class QObject;

#define BOBUI_FORWARD_DECLARE_SHARED_POINTER_TYPES_ITER(Name) \
    template <class T> class Name; \

BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_SMART_POINTER(BOBUI_FORWARD_DECLARE_SHARED_POINTER_TYPES_ITER)

namespace BobUIPrivate
{
    namespace detail {
    template<typename T, typename ODR_VIOLATION_PREVENTER>
    struct is_complete_helper
    {
        template<typename U>
        static auto check(U *) -> std::integral_constant<bool, sizeof(U) != 0>;
        static auto check(...) -> std::false_type;
        using type = decltype(check(static_cast<T *>(nullptr)));
    };
    } // namespace detail

    template <typename T, typename ODR_VIOLATION_PREVENTER>
    struct is_complete : detail::is_complete_helper<std::remove_reference_t<T>, ODR_VIOLATION_PREVENTER>::type {};

    template <typename T> struct MetatypeDecay              { using type = T; };
    template <typename T> struct MetatypeDecay<const T>     { using type = T; };
    template <typename T> struct MetatypeDecay<const T &>   { using type = T; };

    template <typename T> struct IsPointerDeclaredOpaque  :
            std::disjunction<std::is_member_pointer<T>,
                             std::is_function<std::remove_pointer_t<T>>>
    {};
    template <> struct IsPointerDeclaredOpaque<void *>      : std::true_type {};
    template <> struct IsPointerDeclaredOpaque<const void *> : std::true_type {};

    // Note: this does not check that T = U* isn't pointing to a
    // forward-declared type. You may want to combine with
    // checkTypeIsSuitableForMetaType().
    template<typename T>
    struct IsPointerToTypeDerivedFromQObject
    {
        enum { Value = false };
    };

    // Specialize to avoid sizeof(void) warning
    template<>
    struct IsPointerToTypeDerivedFromQObject<void*>
    {
        enum { Value = false };
    };
    template<>
    struct IsPointerToTypeDerivedFromQObject<const void*>
    {
        enum { Value = false };
    };
    template<>
    struct IsPointerToTypeDerivedFromQObject<QObject*>
    {
        enum { Value = true };
    };

    template<typename T>
    struct IsPointerToTypeDerivedFromQObject<T*>
    {
        typedef qint8 yes_type;
        typedef qint64 no_type;

#ifndef BOBUI_NO_QOBJECT
        static yes_type checkType(QObject* );
        static yes_type checkType(const QObject* );
#endif
        static no_type checkType(...);
        enum { Value = sizeof(checkType(static_cast<T*>(nullptr))) == sizeof(yes_type) };
    };

    template<typename T, typename Enable = void>
    struct IsGadgetHelper { enum { IsRealGadget = false, IsGadgetOrDerivedFrom = false }; };

    template<typename T>
    struct IsGadgetHelper<T, typename T::BobUIGadgetHelper>
    {
        template <typename X>
        static char checkType(void (X::*)());
        static void *checkType(void (T::*)());
        enum {
            IsRealGadget = sizeof(checkType(&T::bobui_check_for_QGADGET_macro)) == sizeof(void *),
            IsGadgetOrDerivedFrom = true
        };
    };

    template <typename T>
    using IsRealGadget = std::bool_constant<IsGadgetHelper<T>::IsRealGadget>;

    template<typename T, typename Enable = void>
    struct IsPointerToGadgetHelper { enum { IsRealGadget = false, IsGadgetOrDerivedFrom = false }; };

    template<typename T>
    struct IsPointerToGadgetHelper<T*, typename T::BobUIGadgetHelper>
    {
        using BaseType = T;
        template <typename X>
        static char checkType(void (X::*)());
        static void *checkType(void (T::*)());
        enum {
            IsRealGadget = !IsPointerToTypeDerivedFromQObject<T*>::Value && sizeof(checkType(&T::bobui_check_for_QGADGET_macro)) == sizeof(void *),
            IsGadgetOrDerivedFrom = !IsPointerToTypeDerivedFromQObject<T*>::Value
        };
    };


    template<typename T> char bobui_getEnumMetaObject(const T&);

    template<typename T>
    struct IsQEnumHelper {
        static const T &declval();
        // If the type was declared with Q_ENUM, the friend bobui_getEnumMetaObject() declared in the
        // Q_ENUM macro will be chosen by ADL, and the return type will be QMetaObject*.
        // Otherwise the chosen overload will be the catch all template function
        // bobui_getEnumMetaObject(T) which returns 'char'
        enum { Value = sizeof(bobui_getEnumMetaObject(declval())) == sizeof(QMetaObject*) };
    };
    template<> struct IsQEnumHelper<void> { enum { Value = false }; };

    template<typename T, typename Enable = void>
    struct MetaObjectForType
    {
        static constexpr const QMetaObject *value() { return nullptr; }
        using MetaObjectFn = const QMetaObject *(*)(const QMetaTypeInterface *);
        static constexpr MetaObjectFn metaObjectFunction = nullptr;
    };
#ifndef BOBUI_NO_QOBJECT
    template<typename T>
    struct MetaObjectForType<T*, typename std::enable_if<IsPointerToTypeDerivedFromQObject<T*>::Value>::type>
    {
        static constexpr const QMetaObject *value() { return &T::staticMetaObject; }
        static constexpr const QMetaObject *metaObjectFunction(const QMetaTypeInterface *) { return &T::staticMetaObject; }
    };
    template<typename T>
    struct MetaObjectForType<T, std::enable_if_t<
        std::disjunction_v<
            std::bool_constant<IsGadgetHelper<T>::IsGadgetOrDerivedFrom>,
            std::is_base_of<QObject, T>
        >
    >>
    {
        static constexpr const QMetaObject *value() { return &T::staticMetaObject; }
        static constexpr const QMetaObject *metaObjectFunction(const QMetaTypeInterface *) { return &T::staticMetaObject; }
    };
    template<typename T>
    struct MetaObjectForType<T, typename std::enable_if<IsPointerToGadgetHelper<T>::IsGadgetOrDerivedFrom>::type>
    {
        static constexpr const QMetaObject *value()
        {
            return &IsPointerToGadgetHelper<T>::BaseType::staticMetaObject;
        }
        static constexpr const QMetaObject *metaObjectFunction(const QMetaTypeInterface *) { return value(); }
    };
    template<typename T>
    struct MetaObjectForType<T, typename std::enable_if<IsQEnumHelper<T>::Value>::type >
    {
        static constexpr const QMetaObject *value() { return bobui_getEnumMetaObject(T()); }
        static constexpr const QMetaObject *metaObjectFunction(const QMetaTypeInterface *) { return value(); }
    };
#endif

    template<typename T>
    struct IsSharedPointerToTypeDerivedFromQObject
    {
        enum { Value = false };
    };

    template<typename T>
    struct IsSharedPointerToTypeDerivedFromQObject<QSharedPointer<T> > : IsPointerToTypeDerivedFromQObject<T*>
    {
    };

    template<typename T>
    struct IsWeakPointerToTypeDerivedFromQObject
    {
        enum { Value = false };
    };

    template<typename T>
    struct IsWeakPointerToTypeDerivedFromQObject<QWeakPointer<T> > : IsPointerToTypeDerivedFromQObject<T*>
    {
    };

    template<typename T>
    struct IsTrackingPointerToTypeDerivedFromQObject
    {
        enum { Value = false };
    };

    template<typename T>
    struct IsTrackingPointerToTypeDerivedFromQObject<QPointer<T> >
    {
        enum { Value = true };
    };

    template<typename T>
    struct IsSequentialContainer
    {
        enum { Value = false };
    };

    template<typename T>
    struct IsAssociativeContainer
    {
        enum { Value = false };
    };

    template<typename T, bool = BobUIPrivate::IsSequentialContainer<T>::Value>
    struct SequentialContainerTransformationHelper
    {
        static bool registerConverter()
        {
            return false;
        }

        static bool registerMutableView()
        {
            return false;
        }
    };

    template<typename T, bool = QMetaTypeId2<typename T::value_type>::Defined>
    struct SequentialValueTypeIsMetaType
    {
        static bool registerConverter()
        {
            return false;
        }

        static bool registerMutableView()
        {
            return false;
        }
    };

    template<typename T>
    struct SequentialContainerTransformationHelper<T, true> : SequentialValueTypeIsMetaType<T>
    {
    };

    template<typename T, bool = BobUIPrivate::IsAssociativeContainer<T>::Value>
    struct AssociativeContainerTransformationHelper
    {
        static bool registerConverter()
        {
            return false;
        }

        static bool registerMutableView()
        {
            return false;
        }
    };

    template<typename T, bool = QMetaTypeId2<typename T::key_type>::Defined>
    struct AssociativeKeyTypeIsMetaType
    {
        static bool registerConverter()
        {
            return false;
        }

        static bool registerMutableView()
        {
            return false;
        }
    };

    template<typename T, bool = QMetaTypeId2<typename T::mapped_type>::Defined>
    struct AssociativeMappedTypeIsMetaType
    {
        static bool registerConverter()
        {
            return false;
        }

        static bool registerMutableView()
        {
            return false;
        }
    };

    template<typename T>
    struct AssociativeContainerTransformationHelper<T, true> : AssociativeKeyTypeIsMetaType<T>
    {
    };

    template<typename T, bool = QMetaTypeId2<typename T::first_type>::Defined
                                && QMetaTypeId2<typename T::second_type>::Defined>
    struct IsMetaTypePair
    {
        static bool registerConverter()
        {
            return false;
        }
    };

    template<typename T>
    struct IsMetaTypePair<T, true>
    {
        inline static bool registerConverter();
    };

    template<typename T>
    struct IsPair
    {
        static bool registerConverter()
        {
            return false;
        }
    };
    template<typename T, typename U>
    struct IsPair<std::pair<T, U> > : IsMetaTypePair<std::pair<T, U> > {};

    template<typename T>
    struct MetaTypePairHelper : IsPair<T> {};

    template<typename T, typename = void>
    struct MetaTypeSmartPointerHelper
    {
        static bool registerConverter() { return false; }
    };

#if BOBUI_CONFIG(future)
    template<typename T>
    struct MetaTypeQFutureHelper
    {
        static bool registerConverter() { return false; }
    };
#endif

    template <typename X> static constexpr bool checkTypeIsSuitableForMetaType()
    {
        using T = typename MetatypeDecay<X>::type;
        static_assert(is_complete<T, void>::value || std::is_void_v<T>,
                "Meta Types must be fully defined");
        static_assert(!std::is_reference_v<T>,
                "Meta Types cannot be non-const references or rvalue references.");
        if constexpr (std::is_pointer_v<T> && !IsPointerDeclaredOpaque<T>::value) {
            using Pointed = std::remove_pointer_t<T>;
            static_assert(is_complete<Pointed, void>::value,
                    "Pointer Meta Types must either point to fully-defined types "
                    "or be declared with Q_DECLARE_OPAQUE_POINTER(T *)");
        }
        return true;
    }
} // namespace BobUIPrivate

template <typename T, int =
    BobUIPrivate::IsPointerToTypeDerivedFromQObject<T>::Value ? QMetaType::PointerToQObject :
    BobUIPrivate::IsRealGadget<T>::value                      ? QMetaType::IsGadget :
    BobUIPrivate::IsPointerToGadgetHelper<T>::IsRealGadget    ? QMetaType::PointerToGadget :
    BobUIPrivate::IsQEnumHelper<T>::Value                     ? QMetaType::IsEnumeration : 0>
struct QMetaTypeIdQObject
{
    enum {
        Defined = 0
    };
};

template <typename T>
struct QMetaTypeId : public QMetaTypeIdQObject<T>
{
};

template <typename T>
struct QMetaTypeId2
{
    using NameAsArrayType = void;
    enum { Defined = QMetaTypeId<T>::Defined, IsBuiltIn=false };
    static inline constexpr int bobui_metatype_id() { return QMetaTypeId<T>::bobui_metatype_id(); }
};

template <typename T>
struct QMetaTypeId2<const T&> : QMetaTypeId2<T> {};

template <typename T>
struct QMetaTypeId2<T&>
{
    using NameAsArrayType = void;
    enum { Defined = false, IsBuiltIn = false };
    static inline constexpr int bobui_metatype_id() { return 0; }
};

namespace BobUIPrivate {
    template <typename T, bool Defined = QMetaTypeId2<T>::Defined>
    struct QMetaTypeIdHelper {
        static inline constexpr int bobui_metatype_id()
        { return QMetaTypeId2<T>::bobui_metatype_id(); }
    };
    template <typename T> struct QMetaTypeIdHelper<T, false> {
        static inline constexpr int bobui_metatype_id()
        { return -1; }
    };

    // Function pointers don't derive from QObject
    template <typename Result, typename... Args>
    struct IsPointerToTypeDerivedFromQObject<Result(*)(Args...)> { enum { Value = false }; };

    template<typename T>
    inline constexpr bool IsQmlListType = false;

    template<typename T, bool = std::is_enum<T>::value>
    constexpr bool IsUnsignedEnum = false;
    template<typename T>
    constexpr bool IsUnsignedEnum<T, true> = !std::is_signed_v<std::underlying_type_t<T>>;

    template<typename T, bool defined>
    struct MetaTypeDefinedHelper
    {
        enum DefinedType { Defined = defined };
    };

    template<typename SmartPointer>
    struct QSmartPointerConvertFunctor
    {
        QObject* operator()(const SmartPointer &p) const
        {
            return p.operator->();
        }
    };

    // hack to delay name lookup to instantiation time by making
    // EnableInternalData a dependent name:
    template <typename T>
    struct EnableInternalDataWrap;

    template<typename T>
    struct QSmartPointerConvertFunctor<QWeakPointer<T> >
    {
        QObject* operator()(const QWeakPointer<T> &p) const
        {
            return BobUIPrivate::EnableInternalDataWrap<T>::internalData(p);
        }
    };
}

template <typename T>
int qRegisterNormalizedMetaTypeImplementation(const BOBUI_PREPEND_NAMESPACE(QByteArray) &normalizedTypeName)
{
#ifndef BOBUI_NO_QOBJECT
    Q_ASSERT_X(normalizedTypeName == QMetaObject::normalizedType(normalizedTypeName.constData()),
               "qRegisterNormalizedMetaType",
               "qRegisterNormalizedMetaType was called with a not normalized type name, "
               "please call qRegisterMetaType instead.");
#endif

    const QMetaType metaType = QMetaType::fromType<T>();
    const int id = metaType.id();

    BobUIPrivate::SequentialContainerTransformationHelper<T>::registerConverter();
    BobUIPrivate::SequentialContainerTransformationHelper<T>::registerMutableView();
    BobUIPrivate::AssociativeContainerTransformationHelper<T>::registerConverter();
    BobUIPrivate::AssociativeContainerTransformationHelper<T>::registerMutableView();
    BobUIPrivate::MetaTypePairHelper<T>::registerConverter();
    BobUIPrivate::MetaTypeSmartPointerHelper<T>::registerConverter();
#if BOBUI_CONFIG(future)
    BobUIPrivate::MetaTypeQFutureHelper<T>::registerConverter();
#endif

    if (normalizedTypeName != metaType.name())
        QMetaType::registerNormalizedTypedef(normalizedTypeName, metaType);

    return id;
}

// This primary template calls the -Implementation, like all other specialisations should.
// But the split allows to
// - in a header:
//   - define a specialization of this template calling an out-of-line function
//     (BOBUI_DECL_METATYPE_EXTERN{,_TAGGED})
// - in the .cpp file:
//   - define the out-of-line wrapper to call the -Implementation
//     (BOBUI_IMPL_METATYPE_EXTERN{,_TAGGED})
// The _TAGGED variants let you choose a tag (must be a C identifier) to disambiguate
// the out-of-line function; the non-_TAGGED variants use the passed class name as tag.
template <typename T>
int qRegisterNormalizedMetaType(const BOBUI_PREPEND_NAMESPACE(QByteArray) &normalizedTypeName)
{
    return qRegisterNormalizedMetaTypeImplementation<T>(normalizedTypeName);
}

#if defined(BOBUI_BOOTSTRAPPED)
#define BOBUI_DECL_METATYPE_EXTERN_TAGGED(TYPE, TAG, EXPORT)
#define BOBUI_IMPL_METATYPE_EXTERN_TAGGED(TYPE, TAG)
#else
#define BOBUI_DECL_METATYPE_EXTERN_TAGGED(TYPE, TAG, EXPORT) \
    BOBUI_BEGIN_NAMESPACE \
    EXPORT int qRegisterNormalizedMetaType_ ## TAG (const QByteArray &); \
    template <> inline int qRegisterNormalizedMetaType< TYPE >(const QByteArray &name) \
    { return qRegisterNormalizedMetaType_ ## TAG (name); } \
    BOBUI_END_NAMESPACE \
    Q_DECLARE_METATYPE(TYPE) \
    /* end */
#define BOBUI_IMPL_METATYPE_EXTERN_TAGGED(TYPE, TAG) \
    int qRegisterNormalizedMetaType_ ## TAG (const QByteArray &name) \
    { return qRegisterNormalizedMetaTypeImplementation< TYPE >(name); } \
    /* end */
#endif
#define BOBUI_DECL_METATYPE_EXTERN(TYPE, EXPORT) \
    BOBUI_DECL_METATYPE_EXTERN_TAGGED(TYPE, TYPE, EXPORT)
#define BOBUI_IMPL_METATYPE_EXTERN(TYPE) \
    BOBUI_IMPL_METATYPE_EXTERN_TAGGED(TYPE, TYPE)

template <typename T>
int qRegisterMetaType(const char *typeName)
{
#ifdef BOBUI_NO_QOBJECT
    BOBUI_PREPEND_NAMESPACE(QByteArray) normalizedTypeName = typeName;
#else
    BOBUI_PREPEND_NAMESPACE(QByteArray) normalizedTypeName = QMetaObject::normalizedType(typeName);
#endif
    return qRegisterNormalizedMetaType<T>(normalizedTypeName);
}

template <typename T>
inline constexpr int qMetaTypeId()
{
    if constexpr (bool(QMetaTypeId2<T>::IsBuiltIn)) {
        // this has the same result as the below code, but avoids asking the
        // compiler to load a global variable whose value we know at compile
        // time
        return QMetaTypeId2<T>::MetaType;
    } else {
        return QMetaType::fromType<T>().id();
    }
}

template <typename T>
inline constexpr int qRegisterMetaType()
{
    int id = qMetaTypeId<T>();
    return id;
}

inline int qRegisterMetaType(QMetaType meta)
{
    return meta.registerHelper();
}

#ifndef BOBUI_NO_QOBJECT
template <typename T>
struct QMetaTypeIdQObject<T*, QMetaType::PointerToQObject>
{
    enum {
        Defined = 1
    };

    static int bobui_metatype_id()
    {
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatype_id.loadAcquire())
            return id;
        const char *const cName = T::staticMetaObject.className();
        QByteArray typeName;
        typeName.reserve(strlen(cName) + 1);
        typeName.append(cName).append('*');
        const int newId = qRegisterNormalizedMetaType<T *>(typeName);
        metatype_id.storeRelease(newId);
        return newId;
    }
};

template <typename T>
struct QMetaTypeIdQObject<T, QMetaType::IsGadget>
{
    enum {
        Defined = std::is_default_constructible<T>::value
    };

    static int bobui_metatype_id()
    {
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatype_id.loadAcquire())
            return id;
        const char *const cName = T::staticMetaObject.className();
        const int newId = qRegisterNormalizedMetaType<T>(cName);
        metatype_id.storeRelease(newId);
        return newId;
    }
};

template <typename T>
struct QMetaTypeIdQObject<T*, QMetaType::PointerToGadget>
{
    enum {
        Defined = 1
    };

    static int bobui_metatype_id()
    {
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatype_id.loadAcquire())
            return id;
        const char *const cName = T::staticMetaObject.className();
        QByteArray typeName;
        typeName.reserve(strlen(cName) + 1);
        typeName.append(cName).append('*');
        const int newId = qRegisterNormalizedMetaType<T *>(typeName);
        metatype_id.storeRelease(newId);
        return newId;
    }
};

template <typename T>
struct QMetaTypeIdQObject<T, QMetaType::IsEnumeration>
{
    enum {
        Defined = 1
    };

    static int bobui_metatype_id()
    {
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatype_id.loadAcquire())
            return id;
        const char *eName = bobui_getEnumName(T());
        const char *cName = bobui_getEnumMetaObject(T())->className();
        QByteArray typeName;
        typeName.reserve(strlen(cName) + 2 + strlen(eName));
        typeName.append(cName).append("::").append(eName);
        const int newId = qRegisterNormalizedMetaType<T>(typeName);
        metatype_id.storeRelease(newId);
        return newId;
    }
};
#endif

#define Q_DECLARE_OPAQUE_POINTER(POINTER)                               \
    BOBUI_BEGIN_NAMESPACE namespace BobUIPrivate {                            \
    template <> struct IsPointerDeclaredOpaque<POINTER>                 \
        : std::true_type {};                                            \
    } BOBUI_END_NAMESPACE                                                  \
    /**/

#ifndef Q_MOC_RUN
#define Q_DECLARE_METATYPE(TYPE) Q_DECLARE_METATYPE_IMPL(TYPE)
#define Q_DECLARE_METATYPE_IMPL(TYPE)                                   \
    BOBUI_BEGIN_NAMESPACE                                                  \
    template <>                                                         \
    struct QMetaTypeId< TYPE >                                          \
    {                                                                   \
        enum { Defined = 1 };                                           \
        static_assert(BobUIPrivate::checkTypeIsSuitableForMetaType<TYPE>());   \
        static int bobui_metatype_id()                                     \
            {                                                           \
                Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
                if (const int id = metatype_id.loadAcquire())           \
                    return id;                                          \
                constexpr auto arr = BobUIPrivate::typenameHelper<TYPE>(); \
                auto name = arr.data();                                 \
                if (QByteArrayView(name) == (#TYPE)) {                  \
                    const int id = qRegisterNormalizedMetaType<TYPE>(name); \
                    metatype_id.storeRelease(id);                       \
                    return id;                                          \
                }                                                       \
                const int newId = qRegisterMetaType< TYPE >(#TYPE);     \
                metatype_id.storeRelease(newId);                        \
                return newId;                                           \
            }                                                           \
    };                                                                  \
    BOBUI_END_NAMESPACE
#endif // Q_MOC_RUN

#define Q_DECLARE_BUILTIN_METATYPE(TYPE, METATYPEID, NAME) \
    BOBUI_BEGIN_NAMESPACE \
    template<> struct QMetaTypeId2<NAME> \
    { \
        using NameAsArrayType = std::array<char, sizeof(#NAME)>; \
        enum { Defined = 1, IsBuiltIn = true, MetaType = METATYPEID };   \
        static inline constexpr int bobui_metatype_id() { return METATYPEID; } \
        static constexpr NameAsArrayType nameAsArray = { #NAME }; \
    }; \
    BOBUI_END_NAMESPACE

#define BOBUI_FORWARD_DECLARE_STATIC_TYPES_ITER(TypeName, TypeId, Name) \
    class Name;

BOBUI_FOR_EACH_STATIC_CORE_CLASS(BOBUI_FORWARD_DECLARE_STATIC_TYPES_ITER)
BOBUI_FOR_EACH_STATIC_GUI_CLASS(BOBUI_FORWARD_DECLARE_STATIC_TYPES_ITER)
BOBUI_FOR_EACH_STATIC_WIDGETS_CLASS(BOBUI_FORWARD_DECLARE_STATIC_TYPES_ITER)

#undef BOBUI_FORWARD_DECLARE_STATIC_TYPES_ITER

#define Q_DECLARE_METATYPE_TEMPLATE_1ARG(SINGLE_ARG_TEMPLATE) \
BOBUI_BEGIN_NAMESPACE \
template <typename T> \
struct QMetaTypeId< SINGLE_ARG_TEMPLATE<T> > \
{ \
    enum { \
        Defined = QMetaTypeId2<T>::Defined \
    }; \
    static int bobui_metatype_id() \
    { \
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.loadRelaxed()) \
            return id; \
        const char *tName = QMetaType::fromType<T>().name(); \
        Q_ASSERT(tName); \
        const size_t tNameLen = qstrlen(tName); \
        QByteArray typeName; \
        typeName.reserve(sizeof(#SINGLE_ARG_TEMPLATE) + 1 + tNameLen + 1 + 1); \
        typeName.append(#SINGLE_ARG_TEMPLATE, int(sizeof(#SINGLE_ARG_TEMPLATE)) - 1) \
            .append('<').append(tName, tNameLen); \
        typeName.append('>'); \
        const int newId = qRegisterNormalizedMetaType< SINGLE_ARG_TEMPLATE<T> >(typeName); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
}; \
BOBUI_END_NAMESPACE

#define Q_DECLARE_METATYPE_TEMPLATE_2ARG(DOUBLE_ARG_TEMPLATE) \
BOBUI_BEGIN_NAMESPACE \
template<typename T, typename U> \
struct QMetaTypeId< DOUBLE_ARG_TEMPLATE<T, U> > \
{ \
    enum { \
        Defined = QMetaTypeId2<T>::Defined && QMetaTypeId2<U>::Defined \
    }; \
    static int bobui_metatype_id() \
    { \
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.loadAcquire()) \
            return id; \
        const char *tName = QMetaType::fromType<T>().name(); \
        const char *uName = QMetaType::fromType<U>().name(); \
        Q_ASSERT(tName); \
        Q_ASSERT(uName); \
        const size_t tNameLen = qstrlen(tName); \
        const size_t uNameLen = qstrlen(uName); \
        QByteArray typeName; \
        typeName.reserve(sizeof(#DOUBLE_ARG_TEMPLATE) + 1 + tNameLen + 1 + uNameLen + 1 + 1); \
        typeName.append(#DOUBLE_ARG_TEMPLATE, int(sizeof(#DOUBLE_ARG_TEMPLATE)) - 1) \
            .append('<').append(tName, tNameLen).append(',').append(uName, uNameLen); \
        typeName.append('>'); \
        const int newId = qRegisterNormalizedMetaType< DOUBLE_ARG_TEMPLATE<T, U> >(typeName); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
}; \
BOBUI_END_NAMESPACE

namespace BobUIPrivate {

template<typename T, bool /* isSharedPointerToQObjectDerived */ = false>
struct SharedPointerMetaTypeIdHelper
{
    enum {
        Defined = 0
    };
    static int bobui_metatype_id()
    {
        return -1;
    }
};

}

#define Q_DECLARE_SMART_POINTER_METATYPE(SMART_POINTER) \
BOBUI_BEGIN_NAMESPACE \
namespace BobUIPrivate { \
template<typename T> \
struct SharedPointerMetaTypeIdHelper<SMART_POINTER<T>, true> \
{ \
    enum { \
        Defined = 1 \
    }; \
    static int bobui_metatype_id() \
    { \
        Q_CONSTINIT static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.loadAcquire()) \
            return id; \
        const char * const cName = T::staticMetaObject.className(); \
        QByteArray typeName; \
        typeName.reserve(sizeof(#SMART_POINTER) + 1 + strlen(cName) + 1); \
        typeName.append(#SMART_POINTER, int(sizeof(#SMART_POINTER)) - 1) \
            .append('<').append(cName).append('>'); \
        const int newId = qRegisterNormalizedMetaType< SMART_POINTER<T> >(typeName); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
}; \
template<typename T> \
struct MetaTypeSmartPointerHelper<SMART_POINTER<T> , \
        typename std::enable_if<IsPointerToTypeDerivedFromQObject<T*>::Value && !std::is_const_v<T>>::type> \
{ \
    static bool registerConverter() \
    { \
        const QMetaType to = QMetaType(QMetaType::QObjectStar); \
        if (!QMetaType::hasRegisteredConverterFunction(QMetaType::fromType<SMART_POINTER<T>>(), to)) { \
            BobUIPrivate::QSmartPointerConvertFunctor<SMART_POINTER<T> > o; \
            return QMetaType::registerConverter<SMART_POINTER<T>, QObject*>(o); \
        } \
        return true; \
    } \
}; \
} \
template <typename T> \
struct QMetaTypeId< SMART_POINTER<T> > \
    : BobUIPrivate::SharedPointerMetaTypeIdHelper< SMART_POINTER<T>, \
                                                BobUIPrivate::IsPointerToTypeDerivedFromQObject<T*>::Value> \
{ \
};\
BOBUI_END_NAMESPACE

#define Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(SINGLE_ARG_TEMPLATE) \
    BOBUI_BEGIN_NAMESPACE \
    namespace BobUIPrivate { \
    template<typename T> \
    struct IsSequentialContainer<SINGLE_ARG_TEMPLATE<T> > \
    { \
        enum { Value = true }; \
    }; \
    } \
    BOBUI_END_NAMESPACE \
    Q_DECLARE_METATYPE_TEMPLATE_1ARG(SINGLE_ARG_TEMPLATE)

#define Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE_ITER(TEMPLATENAME) \
    Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(TEMPLATENAME)

BOBUI_END_NAMESPACE

BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_1ARG(Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE_ITER)

#undef Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE_ITER

Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(std::vector)
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(std::list)

#define Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(TEMPLATENAME) \
    BOBUI_BEGIN_NAMESPACE \
    namespace BobUIPrivate { \
    template<typename T, typename U> \
    struct IsAssociativeContainer<TEMPLATENAME<T, U> > \
    { \
        enum { Value = true }; \
    }; \
    } \
    BOBUI_END_NAMESPACE \
    Q_DECLARE_METATYPE_TEMPLATE_2ARG(TEMPLATENAME)

Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(QHash)
Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(QMap)
Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(std::map)

Q_DECLARE_METATYPE_TEMPLATE_2ARG(std::pair)

#define Q_DECLARE_METATYPE_TEMPLATE_SMART_POINTER_ITER(TEMPLATENAME) \
    Q_DECLARE_SMART_POINTER_METATYPE(TEMPLATENAME)

BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_SMART_POINTER(Q_DECLARE_METATYPE_TEMPLATE_SMART_POINTER_ITER)

BOBUI_BEGIN_NAMESPACE

#undef Q_DECLARE_METATYPE_TEMPLATE_SMART_POINTER_ITER

BOBUI_END_NAMESPACE

BOBUI_FOR_EACH_STATIC_TYPE(Q_DECLARE_BUILTIN_METATYPE)


BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {
// out-of-line helpers to reduce template code bloat ("SCARY") and improve compile times:
Q_CORE_EXPORT bool hasRegisteredConverterFunctionToPairVariantInterface(QMetaType m);
Q_CORE_EXPORT bool hasRegisteredConverterFunctionToIterableMetaSequence(QMetaType m);
Q_CORE_EXPORT bool hasRegisteredMutableViewFunctionToIterableMetaSequence(QMetaType m);
Q_CORE_EXPORT bool hasRegisteredConverterFunctionToIterableMetaAssociation(QMetaType m);
Q_CORE_EXPORT bool hasRegisteredMutableViewFunctionToIterableMetaAssociation(QMetaType m);
}

template <typename T>
inline bool BobUIPrivate::IsMetaTypePair<T, true>::registerConverter()
{
    if (!BobUIPrivate::hasRegisteredConverterFunctionToPairVariantInterface(QMetaType::fromType<T>())) {
        BobUIMetaTypePrivate::QPairVariantInterfaceConvertFunctor<T> o;
        return QMetaType::registerConverter<T, BobUIMetaTypePrivate::QPairVariantInterfaceImpl>(o);
    }
    return true;
}

namespace BobUIPrivate {

template<typename From>
struct QSequentialIterableConvertFunctor
{
    QIterable<QMetaSequence> operator()(const From &f) const
    {
        return QIterable<QMetaSequence>(QMetaSequence::fromContainer<From>(), &f);
    }
};

template<typename From>
struct QSequentialIterableMutableViewFunctor
{
    QIterable<QMetaSequence> operator()(From &f) const
    {
        return QIterable<QMetaSequence>(QMetaSequence::fromContainer<From>(), &f);
    }
};

template<typename T>
struct SequentialValueTypeIsMetaType<T, true>
{
    static bool registerConverter()
    {
        if (!BobUIPrivate::hasRegisteredConverterFunctionToIterableMetaSequence(QMetaType::fromType<T>())) {
            QSequentialIterableConvertFunctor<T> o;
            return QMetaType::registerConverter<T, QIterable<QMetaSequence>>(o);
        }
        return true;
    }

    static bool registerMutableView()
    {
        if (!BobUIPrivate::hasRegisteredMutableViewFunctionToIterableMetaSequence(QMetaType::fromType<T>())) {
            QSequentialIterableMutableViewFunctor<T> o;
            return QMetaType::registerMutableView<T, QIterable<QMetaSequence>>(o);
        }
        return true;
    }
};

template<typename From>
struct QAssociativeIterableConvertFunctor
{
    QIterable<QMetaAssociation> operator()(const From &f) const
    {
        return QIterable<QMetaAssociation>(QMetaAssociation::fromContainer<From>(), &f);
    }
};

template<typename From>
struct QAssociativeIterableMutableViewFunctor
{
    QIterable<QMetaAssociation> operator()(From &f) const
    {
        return QIterable<QMetaAssociation>(QMetaAssociation::fromContainer<From>(), &f);
    }
};

// Mapped type can be omitted, for example in case of a set.
// However, if it is available, we want to instantiate the metatype here.
template<typename T>
struct AssociativeKeyTypeIsMetaType<T, true> : AssociativeMappedTypeIsMetaType<T>
{
    static bool registerConverter()
    {
        if (!BobUIPrivate::hasRegisteredConverterFunctionToIterableMetaAssociation(QMetaType::fromType<T>())) {
            QAssociativeIterableConvertFunctor<T> o;
            return QMetaType::registerConverter<T, QIterable<QMetaAssociation>>(o);
        }
        return true;
    }

    static bool registerMutableView()
    {
        if (!BobUIPrivate::hasRegisteredMutableViewFunctionToIterableMetaAssociation(QMetaType::fromType<T>())) {
            QAssociativeIterableMutableViewFunctor<T> o;
            return QMetaType::registerMutableView<T, QIterable<QMetaAssociation>>(o);
        }
        return true;
    }
};

struct BOBUIypeNormalizer
{
    char *output;
    int len = 0;
    char last = 0;

private:
    static constexpr bool is_ident_char(char s)
    {
        return ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9')
                || s == '_');
    }
    static constexpr bool is_space(char s) { return (s == ' ' || s == '\t' || s == '\n'); }
    static constexpr bool is_number(char s) { return s >= '0' && s <= '9'; }
    static constexpr bool starts_with_token(const char *b, const char *e, const char *token,
                                            bool msvcKw = false)
    {
        while (b != e && *token && *b == *token) {
            b++;
            token++;
        }
        if (*token)
            return false;
#ifdef Q_CC_MSVC
        /// On MSVC, keywords like class or struct are not separated with spaces in constexpr
        /// context
        if (msvcKw && !is_ident_char(*b))
            return true;
#endif
        Q_UNUSED(msvcKw);
        return b == e || !is_ident_char(*b);
    }
    static constexpr bool skipToken(const char *&x, const char *e, const char *token,
                                    bool msvcKw = false)
    {
        if (!starts_with_token(x, e, token, msvcKw))
            return false;
        while (*token++)
            x++;
        while (x != e && is_space(*x))
            x++;
        return true;
    }
    static constexpr const char *skipString(const char *x, const char *e)
    {
        char delim = *x;
        x++;
        while (x != e && *x != delim) {
            if (*x == '\\') {
                x++;
                if (x == e)
                    return e;
            }
            x++;
        }
        if (x != e)
            x++;
        return x;
    }
    static constexpr const char *skipTemplate(const char *x, const char *e, bool stopAtComa = false)
    {
        int scopeDepth = 0;
        int templateDepth = 0;
        while (x != e) {
            switch (*x) {
            case '<':
                if (!scopeDepth)
                    templateDepth++;
                break;
            case ',':
                if (stopAtComa && !scopeDepth && !templateDepth)
                    return x;
                break;
            case '>':
                if (!scopeDepth)
                    if (--templateDepth < 0)
                        return x;
                break;
            case '(':
            case '[':
            case '{':
                scopeDepth++;
                break;
            case '}':
            case ']':
            case ')':
                scopeDepth--;
                break;
            case '\'':
                if (is_number(x[-1]))
                    break;
                Q_FALLTHROUGH();
            case '\"':
                x = skipString(x, e);
                continue;
            }
            x++;
        }
        return x;
    }

    constexpr void append(char x)
    {
        last = x;
        len++;
        if (output)
            *output++ = x;
    }

    constexpr void replaceLast(char x)
    {
        last = x;
        if (output)
            *(output - 1) = x;
    }

    constexpr void appendStr(const char *x)
    {
        while (*x)
            append(*x++);
    }

    constexpr void normalizeIntegerTypes(const char *&begin, const char *end)
    {
        int numLong = 0;
        int numSigned = 0;
        int numUnsigned = 0;
        int numInt = 0;
        int numShort = 0;
        int numChar = 0;
        while (begin < end) {
            if (skipToken(begin, end, "long")) {
                numLong++;
                continue;
            }
            if (skipToken(begin, end, "int")) {
                numInt++;
                continue;
            }
            if (skipToken(begin, end, "short")) {
                numShort++;
                continue;
            }
            if (skipToken(begin, end, "unsigned")) {
                numUnsigned++;
                continue;
            }
            if (skipToken(begin, end, "signed")) {
                numSigned++;
                continue;
            }
            if (skipToken(begin, end, "char")) {
                numChar++;
                continue;
            }
#ifdef Q_CC_MSVC
            if (skipToken(begin, end, "__int64")) {
                numLong = 2;
                continue;
            }
#endif
            break;
        }
        if (numLong == 2)
            append('q'); // q(u)longlong
        if (numSigned && numChar)
            appendStr("signed ");
        else if (numUnsigned)
            appendStr("u");
        if (numChar)
            appendStr("char");
        else if (numShort)
            appendStr("short");
        else if (numLong == 1)
            appendStr("long");
        else if (numLong == 2)
            appendStr("longlong");
        else if (numUnsigned || numSigned || numInt)
            appendStr("int");
    }

    constexpr void skipStructClassOrEnum(const char *&begin, const char *end)
    {
        // discard 'struct', 'class', and 'enum'; they are optional
        // and we don't want them in the normalized signature
        skipToken(begin, end, "struct", true) || skipToken(begin, end, "class", true)
                || skipToken(begin, end, "enum", true);
    }

    constexpr void skipBobUINamespace(const char *&begin, const char *end)
    {
#ifdef BOBUI_NAMESPACE
        const char *nsbeg = begin;
        if (skipToken(nsbeg, end, BOBUI_STRINGIFY(BOBUI_NAMESPACE)) && nsbeg + 2 < end && nsbeg[0] == ':'
            && nsbeg[1] == ':') {
            begin = nsbeg + 2;
            while (begin != end && is_space(*begin))
                begin++;
        }
#else
        Q_UNUSED(begin);
        Q_UNUSED(end);
#endif
    }

public:
#if defined(Q_CC_CLANG) || defined (Q_CC_GNU)
    // this is much simpler than the full type normalization below
    // the reason is that the signature returned by Q_FUNC_INFO is already
    // normalized to the largest degree, and we need to do only small adjustments
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end)
    {
        // bail out if there is an anonymous struct
        std::string_view name(begin, end-begin);
#if defined (Q_CC_CLANG)
        if (name.find("anonymous ") != std::string_view::npos)
            return normalizeType(begin, end);
#endif
        if (name.find("unnamed ") != std::string_view::npos)
            return normalizeType(begin, end);
        while (begin < end) {
            if (*begin == ' ') {
                if (last == ',' || last == '>' || last == '<' || last == '*' || last == '&') {
                    ++begin;
                    continue;
                }
            }
            if (last == ' ') {
                if (*begin == '*' || *begin == '&' || *begin == '(') {
                    replaceLast(*begin);
                    ++begin;
                    continue;
                }
            }
            if (!is_ident_char(last)) {
                skipStructClassOrEnum(begin, end);
                if (begin == end)
                    break;

                skipBobUINamespace(begin, end);
                if (begin == end)
                    break;

                normalizeIntegerTypes(begin, end);
                if (begin == end)
                    break;
            }
            append(*begin);
            ++begin;
        }
        return len;
    }
#else
    // MSVC needs the full normalization, as it puts the const in a different
    // place than we expect
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end)
    { return normalizeType(begin, end); }
#endif

    constexpr int normalizeType(const char *begin, const char *end, bool adjustConst = true)
    {
        // Trim spaces
        while (begin != end && is_space(*begin))
            begin++;
        while (begin != end && is_space(*(end - 1)))
            end--;

        if (begin == end)
            return len;

        // Convert 'char const *' into 'const char *'. Start at index 1,
        // not 0, because 'const char *' is already OK.
        const char *cst = begin + 1;
        if (*begin == '\'' || *begin == '"')
            cst = skipString(begin, end);
        bool seenStar = false;
        bool hasMiddleConst = false;
        while (cst < end) {
            if (*cst == '\"' || (*cst == '\'' && !is_number(cst[-1]))) {
                cst = skipString(cst, end);
                if (cst == end)
                    break;
            }

            // We mustn't convert 'char * const *' into 'const char **'
            // and we must beware of 'Bar<const Bla>'.
            if (*cst == '&' || *cst == '*' || *cst == '[') {
                seenStar = *cst != '&' || cst != (end - 1);
                break;
            }
            if (*cst == '<') {
                cst = skipTemplate(cst + 1, end);
                if (cst == end)
                    break;
            }
            cst++;
            const char *skipedCst = cst;
            if (!is_ident_char(*(cst - 1)) && skipToken(skipedCst, end, "const")) {
                const char *testEnd = end;
                while (skipedCst < testEnd--) {
                    if (*testEnd == '*' || *testEnd == '['
                        || (*testEnd == '&' && testEnd != (end - 1))) {
                        seenStar = true;
                        break;
                    }
                    if (*testEnd == '>')
                        break;
                }
                if (adjustConst && !seenStar) {
                    if (*(end - 1) == '&')
                        end--;
                } else {
                    appendStr("const ");
                }
                normalizeType(begin, cst, false);
                begin = skipedCst;
                hasMiddleConst = true;
                break;
            }
        }
        if (skipToken(begin, end, "const")) {
            if (adjustConst && !seenStar) {
                if (*(end - 1) == '&')
                    end--;
            } else {
                appendStr("const ");
            }
        }
        if (seenStar && adjustConst) {
            const char *e = end;
            if (*(end - 1) == '&' && *(end - 2) != '&')
                e--;
            while (begin != e && is_space(*(e - 1)))
                e--;
            const char *token = "tsnoc"; // 'const' reverse, to check if it ends with const
            while (*token && begin != e && *(--e) == *token++)
                ;
            if (!*token && begin != e && !is_ident_char(*(e - 1))) {
                while (begin != e && is_space(*(e - 1)))
                    e--;
                end = e;
            }
        }

        skipStructClassOrEnum(begin, end);
        skipBobUINamespace(begin, end);

        if (skipToken(begin, end, "QVector")) {
            // Replace QVector by QList
            appendStr("QList");
        }

        if (skipToken(begin, end, "QPair")) {
            // replace QPair by std::pair
            appendStr("std::pair");
        }

        if (!hasMiddleConst)
            // Normalize the integer types
            normalizeIntegerTypes(begin, end);

        bool spaceSkiped = true;
        while (begin != end) {
            char c = *begin++;
            if (is_space(c)) {
                spaceSkiped = true;
            } else if ((c == '\'' && !is_number(last)) || c == '\"') {
                begin--;
                auto x = skipString(begin, end);
                while (begin < x)
                    append(*begin++);
            } else {
                if (spaceSkiped && is_ident_char(last) && is_ident_char(c))
                    append(' ');
                append(c);
                spaceSkiped = false;
                if (c == '<') {
                    do {
                        // template recursion
                        const char *tpl = skipTemplate(begin, end, true);
                        normalizeType(begin, tpl, false);
                        if (tpl == end)
                            return len;
                        append(*tpl);
                        begin = tpl;
                    } while (*begin++ == ',');
                }
            }
        }
        return len;
    }
};

// Normalize the type between begin and end, and store the data in the output. Returns the length.
// The idea is to first run this function with nullptr as output to allocate the output with the
// size
constexpr int qNormalizeType(const char *begin, const char *end, char *output)
{
    return BOBUIypeNormalizer { output }.normalizeType(begin, end);
}

template<typename T>
struct is_std_pair : std::false_type {};

template <typename T1_, typename T2_>
struct is_std_pair<std::pair<T1_, T2_>> : std::true_type {
    using T1 = T1_;
    using T2 = T2_;
};

namespace TypeNameHelper {
template<typename T>
constexpr auto typenameHelper()
{
    if constexpr (is_std_pair<T>::value) {
        using T1 = typename is_std_pair<T>::T1;
        using T2 = typename is_std_pair<T>::T2;
        std::remove_const_t<std::conditional_t<bool (QMetaTypeId2<T1>::IsBuiltIn), typename QMetaTypeId2<T1>::NameAsArrayType, decltype(typenameHelper<T1>())>> t1Name {};
        std::remove_const_t<std::conditional_t<bool (QMetaTypeId2<T2>::IsBuiltIn), typename QMetaTypeId2<T2>::NameAsArrayType, decltype(typenameHelper<T2>())>> t2Name {};
        if constexpr (bool (QMetaTypeId2<T1>::IsBuiltIn) ) {
            t1Name = QMetaTypeId2<T1>::nameAsArray;
        } else {
            t1Name = typenameHelper<T1>();
        }
        if constexpr (bool(QMetaTypeId2<T2>::IsBuiltIn)) {
            t2Name = QMetaTypeId2<T2>::nameAsArray;
        } else {
            t2Name = typenameHelper<T2>();
        }
        constexpr auto nonTypeDependentLen = sizeof("std::pair<,>");
        constexpr auto t1Len = t1Name.size() - 1;
        constexpr auto t2Len = t2Name.size() - 1;
        constexpr auto length = nonTypeDependentLen + t1Len + t2Len;
        std::array<char, length + 1> result {};
        constexpr auto prefix = "std::pair<";
        int currentLength = 0;
        for (; currentLength < int(sizeof("std::pair<") - 1); ++currentLength)
            result[currentLength] = prefix[currentLength];
        for (int i = 0; i < int(t1Len); ++currentLength, ++i)
            result[currentLength] = t1Name[i];
        result[currentLength++] = ',';
        for (int i = 0; i < int(t2Len); ++currentLength, ++i)
            result[currentLength] = t2Name[i];
        result[currentLength++] = '>';
        result[currentLength++] = '\0';
        return result;
    } else {
        constexpr auto prefix = sizeof(
#ifdef BOBUI_NAMESPACE
            BOBUI_STRINGIFY(BOBUI_NAMESPACE) "::"
#endif
#if defined(Q_CC_MSVC) && defined(Q_CC_CLANG)
            "auto __cdecl BobUIPrivate::TypeNameHelper::typenameHelper(void) [T = "
#elif defined(Q_CC_MSVC)
            "auto __cdecl BobUIPrivate::TypeNameHelper::typenameHelper<"
#elif defined(Q_CC_CLANG)
            "auto BobUIPrivate::TypeNameHelper::typenameHelper() [T = "
#elif defined(Q_CC_GHS)
            "auto BobUIPrivate::TypeNameHelper::typenameHelper<T>()[with T="
#else
            "constexpr auto BobUIPrivate::TypeNameHelper::typenameHelper() [with T = "
#endif
            ) - 1;
#if defined(Q_CC_MSVC) && !defined(Q_CC_CLANG)
        constexpr int suffix = sizeof(">(void)");
#else
        constexpr int suffix = sizeof("]");
#endif

#if defined(Q_CC_GNU_ONLY) && Q_CC_GNU_ONLY < 804
        auto func = Q_FUNC_INFO;
        const char *begin = func + prefix;
        const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
        // This is an upper bound of the size since the normalized signature should always be smaller
        constexpr int len = sizeof(Q_FUNC_INFO) - suffix - prefix;
#else
        constexpr auto func = Q_FUNC_INFO;
        constexpr const char *begin = func + prefix;
        constexpr const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
        constexpr int len = BOBUIypeNormalizer{ nullptr }.normalizeTypeFromSignature(begin, end);
#endif
        std::array<char, len + 1> result {};
        BOBUIypeNormalizer{ result.data() }.normalizeTypeFromSignature(begin, end);
        return result;
    }
}
} // namespace TypeNameHelper
using TypeNameHelper::typenameHelper;

template<typename T, typename = void>
struct BuiltinMetaType : std::integral_constant<int, 0>
{
};
template<typename T>
struct BuiltinMetaType<T, std::enable_if_t<QMetaTypeId2<T>::IsBuiltIn>>
    : std::integral_constant<int, QMetaTypeId2<T>::MetaType>
{
};

template<typename T, bool = (BOBUIypeTraits::has_operator_equal_v<T> && !std::is_pointer_v<T>)>
struct QEqualityOperatorForType
{
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_FLOAT_COMPARE
    static bool equals(const QMetaTypeInterface *, const void *a, const void *b)
    { return *reinterpret_cast<const T *>(a) == *reinterpret_cast<const T *>(b); }
BOBUI_WARNING_POP
};

template<typename T>
struct QEqualityOperatorForType <T, false>
{
    static constexpr QMetaTypeInterface::EqualsFn equals = nullptr;
};

template<typename T, bool = (BOBUIypeTraits::has_operator_less_than_v<T> && !std::is_pointer_v<T>)>
struct QLessThanOperatorForType
{
    static bool lessThan(const QMetaTypeInterface *, const void *a, const void *b)
    { return *reinterpret_cast<const T *>(a) < *reinterpret_cast<const T *>(b); }
};

template<typename T>
struct QLessThanOperatorForType <T, false>
{
    static constexpr QMetaTypeInterface::LessThanFn lessThan = nullptr;
};

template<typename T, bool = (BOBUIypeTraits::has_ostream_operator_v<QDebug, T> && !std::is_pointer_v<T>)>
struct QDebugStreamOperatorForType
{
    static constexpr QMetaTypeInterface::DebugStreamFn debugStream = nullptr;
};

#ifndef BOBUI_NO_DEBUG_STREAM
template<typename T>
struct QDebugStreamOperatorForType <T, true>
{
    static void debugStream(const QMetaTypeInterface *, QDebug &dbg, const void *a)
    { dbg << *reinterpret_cast<const T *>(a); }
};
#endif

template<typename T, bool = BOBUIypeTraits::has_stream_operator_v<QDataStream, T>>
struct QDataStreamOperatorForType
{
    static constexpr QMetaTypeInterface::DataStreamOutFn dataStreamOut = nullptr;
    static constexpr QMetaTypeInterface::DataStreamInFn dataStreamIn = nullptr;
};

#ifndef BOBUI_NO_DATASTREAM
template<typename T>
struct QDataStreamOperatorForType <T, true>
{
    static void dataStreamOut(const QMetaTypeInterface *, QDataStream &ds, const void *a)
    { ds << *reinterpret_cast<const T *>(a); }
    static void dataStreamIn(const QMetaTypeInterface *, QDataStream &ds, void *a)
    { ds >> *reinterpret_cast<T *>(a); }
};
#endif

// Performance optimization:
//
// Don't add all these symbols to the dynamic symbol tables on ELF systems and
// on Darwin. Each library is going to have a copy anyway and QMetaType already
// copes with some of these being "hidden" (see QMetaType::idHelper()). We may
// as well let the linker know it can always use the local copy.
//
// This is currently not enabled for GCC due to
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106023

#if !defined(Q_OS_WIN) && defined(Q_CC_CLANG)
#  pragma GCC visibility push(hidden)
#endif

// ### BobUI 7: consider removing this infrastructure if nothing uses it
// (see also getCopyCtr())
namespace QMetaTypeCopyTraits
{
    // Hack to suppress deprecation warnings from types
    // with deprecated copy operations, cf. BOBUIBUG-132752
    template <typename T>
    using HasDeprecatedCopyConstructorTest = typename T::_q_hasDeprecatedCopyConstructor;

#if !defined(BOBUI_BOOTSTRAPPED)
    Q_CORE_EXPORT void warnAboutDeprecatedCopy(const char *name);
#endif
} // namespace QMetaTypeCopyTraits

template<typename S>
class QMetaTypeForType
{
public:
    static constexpr decltype(typenameHelper<S>()) name = typenameHelper<S>();

    static constexpr unsigned flags()
    {
        uint flags = 0;
        if constexpr (BOBUIypeInfo<S>::isRelocatable)
            flags |= QMetaType::RelocatableType;
        if constexpr (!std::is_default_constructible_v<S> || !BOBUIypeInfo<S>::isValueInitializationBitwiseZero)
            flags |= QMetaType::NeedsConstruction;
        if constexpr (!std::is_trivially_destructible_v<S>)
            flags |= QMetaType::NeedsDestruction;
        if constexpr (!std::is_trivially_copy_constructible_v<S>)
            flags |= QMetaType::NeedsCopyConstruction;
        if constexpr (!std::is_trivially_move_constructible_v<S>)
            flags |= QMetaType::NeedsMoveConstruction;
        if constexpr (IsPointerToTypeDerivedFromQObject<S>::Value)
            flags |= QMetaType::PointerToQObject;
        if constexpr (IsSharedPointerToTypeDerivedFromQObject<S>::Value)
            flags |= QMetaType::SharedPointerToQObject;
        if constexpr (IsWeakPointerToTypeDerivedFromQObject<S>::Value)
            flags |= QMetaType::WeakPointerToQObject;
        if constexpr (IsTrackingPointerToTypeDerivedFromQObject<S>::Value)
            flags |= QMetaType::TrackingPointerToQObject;
        if constexpr (IsEnumOrFlags<S>::value)
            flags |= QMetaType::IsEnumeration;
        if constexpr (IsGadgetHelper<S>::IsGadgetOrDerivedFrom)
            flags |= QMetaType::IsGadget;
        if constexpr (IsPointerToGadgetHelper<S>::IsGadgetOrDerivedFrom)
            flags |= QMetaType::PointerToGadget;
        if constexpr (std::is_pointer_v<S>)
            flags |= QMetaType::IsPointer;
        if constexpr (IsUnsignedEnum<S>)
            flags |= QMetaType::IsUnsignedEnumeration;
        if constexpr (IsQmlListType<S>)
            flags |= QMetaType::IsQmlList;
        if constexpr (std::is_const_v<std::remove_pointer_t<S>>)
            flags |= QMetaType::IsConst;
        return flags;
    }

    static constexpr QMetaTypeInterface::DefaultCtrFn getDefaultCtr()
    {
        if constexpr (std::is_default_constructible_v<S> && !BOBUIypeInfo<S>::isValueInitializationBitwiseZero) {
            return [](const QMetaTypeInterface *, void *addr) { new (addr) S(); };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaTypeInterface::CopyCtrFn getCopyCtr()
    {
        if constexpr (std::is_copy_constructible_v<S> && !std::is_trivially_copy_constructible_v<S>) {
            return [](const QMetaTypeInterface *, void *addr, const void *other) {
                if constexpr (qxp::is_detected_v<QMetaTypeCopyTraits::HasDeprecatedCopyConstructorTest, S>) {
#if !defined(BOBUI_BOOTSTRAPPED)
                    QMetaTypeCopyTraits::warnAboutDeprecatedCopy(getName());
#endif
                    BOBUI_IGNORE_DEPRECATIONS(new (addr) S(*reinterpret_cast<const S *>(other));)
                } else {
                    new (addr) S(*reinterpret_cast<const S *>(other));
                }
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaTypeInterface::MoveCtrFn getMoveCtr()
    {
        if constexpr (std::is_move_constructible_v<S> && !std::is_trivially_move_constructible_v<S>) {
            return [](const QMetaTypeInterface *, void *addr, void *other) {
                new (addr) S(std::move(*reinterpret_cast<S *>(other)));
            };
        } else {
            return nullptr;
        }
    }

    static constexpr QMetaTypeInterface::DtorFn getDtor()
    {
        if constexpr (std::is_destructible_v<S> && !std::is_trivially_destructible_v<S>)
            return [](const QMetaTypeInterface *, void *addr) {
                reinterpret_cast<S *>(addr)->~S();
            };
        else
            return nullptr;
    }

    static constexpr QMetaTypeInterface::LegacyRegisterOp getLegacyRegister()
    {
        if constexpr (QMetaTypeId2<S>::Defined && !QMetaTypeId2<S>::IsBuiltIn) {
            return []() { QMetaTypeId2<S>::bobui_metatype_id(); };
        } else {
            return nullptr;
        }
    }

    static constexpr const char *getName()
    {
        if constexpr (bool(QMetaTypeId2<S>::IsBuiltIn)) {
            return QMetaTypeId2<S>::nameAsArray.data();
        } else {
            return name.data();
        }
    }
};

template<typename T>
struct QMetaTypeInterfaceWrapper
{
    // if the type ID for T is known at compile-time, then we can declare
    // the QMetaTypeInterface object const; otherwise, we declare it as
    // non-const and the .typeId is updated by QMetaType::idHelper().
    static constexpr bool IsConstMetaTypeInterface = !!BuiltinMetaType<T>::value;
    using InterfaceType = std::conditional_t<IsConstMetaTypeInterface, const QMetaTypeInterface, NonConstMetaTypeInterface>;

    static inline InterfaceType metaType = {
        /*.revision=*/ QMetaTypeInterface::CurrentRevision,
        /*.alignment=*/ alignof(T),
        /*.size=*/ sizeof(T),
        /*.flags=*/ QMetaTypeForType<T>::flags(),
        /*.typeId=*/ BuiltinMetaType<T>::value,
        /*.metaObjectFn=*/ MetaObjectForType<T>::metaObjectFunction,
        /*.name=*/ QMetaTypeForType<T>::getName(),
        /*.defaultCtr=*/ QMetaTypeForType<T>::getDefaultCtr(),
        /*.copyCtr=*/ QMetaTypeForType<T>::getCopyCtr(),
        /*.moveCtr=*/ QMetaTypeForType<T>::getMoveCtr(),
        /*.dtor=*/ QMetaTypeForType<T>::getDtor(),
        /*.equals=*/ QEqualityOperatorForType<T>::equals,
        /*.lessThan=*/ QLessThanOperatorForType<T>::lessThan,
        /*.debugStream=*/ QDebugStreamOperatorForType<T>::debugStream,
        /*.dataStreamOut=*/ QDataStreamOperatorForType<T>::dataStreamOut,
        /*.dataStreamIn=*/ QDataStreamOperatorForType<T>::dataStreamIn,
        /*.legacyRegisterOp=*/ QMetaTypeForType<T>::getLegacyRegister()
    };
};
template<typename T> struct QMetaTypeInterfaceWrapper<T &> {};


#if !defined(Q_OS_WIN) && defined(Q_CC_CLANG)
#  pragma GCC visibility pop
#endif

template<>
class QMetaTypeInterfaceWrapper<void>
{
public:
    static constexpr QMetaTypeInterface metaType =
    {
        /*.revision=*/ 0,
        /*.alignment=*/ 0,
        /*.size=*/ 0,
        /*.flags=*/ 0,
        /*.typeId=*/ BuiltinMetaType<void>::value,
        /*.metaObjectFn=*/ nullptr,
        /*.name=*/ "void",
        /*.defaultCtr=*/ nullptr,
        /*.copyCtr=*/ nullptr,
        /*.moveCtr=*/ nullptr,
        /*.dtor=*/ nullptr,
        /*.equals=*/ nullptr,
        /*.lessThan=*/ nullptr,
        /*.debugStream=*/ nullptr,
        /*.dataStreamOut=*/ nullptr,
        /*.dataStreamIn=*/ nullptr,
        /*.legacyRegisterOp=*/ nullptr
    };
};

/*
 MSVC instantiates extern templates
(https://developercommunity.visualstudio.com/t/c11-extern-templates-doesnt-work-for-class-templat/157868)

 The INTEGRITY compiler apparently does too.

 On Windows (with other compilers or whenever MSVC is fixed), we can't declare
 QMetaTypeInterfaceWrapper with __declspec(dllimport) because taking its
 address is not a core constant expression.
 */
#if !defined(BOBUI_BOOTSTRAPPED) && !defined(Q_CC_MSVC) && !defined(Q_OS_INTEGRITY)

#ifdef BOBUI_NO_DATA_RELOCATION
#  define BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER(TypeName, Id, Name)          \
    extern template class Q_CORE_EXPORT QMetaTypeForType<Name>;
#else
#  define BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER(TypeName, Id, Name)          \
    extern template class Q_CORE_EXPORT QMetaTypeForType<Name>;                 \
    extern template struct Q_CORE_EXPORT QMetaTypeInterfaceWrapper<Name>;
#endif

BOBUI_FOR_EACH_STATIC_PRIMITIVE_NON_VOID_TYPE(BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER)
BOBUI_FOR_EACH_STATIC_PRIMITIVE_POINTER(BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER)
BOBUI_FOR_EACH_STATIC_CORE_CLASS(BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER)
BOBUI_FOR_EACH_STATIC_CORE_POINTER(BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER)
BOBUI_FOR_EACH_STATIC_CORE_TEMPLATE(BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER)
#undef BOBUI_METATYPE_DECLARE_EXTERN_TEMPLATE_ITER
#endif

template<typename T>
struct QRemovePointerLike
{
    using type = std::remove_pointer_t<T>;
};

#define Q_REMOVE_POINTER_LIKE_IMPL(Pointer) \
template <typename T> \
struct QRemovePointerLike<Pointer<T>> \
{ \
    using type = T; \
};

BOBUI_FOR_EACH_AUTOMATIC_TEMPLATE_SMART_POINTER(Q_REMOVE_POINTER_LIKE_IMPL)
#undef Q_REMOVE_POINTER_LIKE_IMPL

template<typename T>
constexpr const QMetaTypeInterface *qMetaTypeInterfaceForType()
{
    // don't check the type is suitable here
    using Ty = typename MetatypeDecay<T>::type;
    return &QMetaTypeInterfaceWrapper<Ty>::metaType;
}

// Relaxed vesion of the above, used by moc-generated code to create the
// metatype array without requiring types to be complete and allowing
// references. Unique is passed to is_complete and must be a different unique
// type; if it is void, this function is equal to qMetaTypeInterfaceForType()
// above.
template<typename Unique, typename T>
constexpr const QMetaTypeInterface *qTryMetaTypeInterfaceForType()
{
    using Ty = typename MetatypeDecay<T>::type;
    using Tz = typename QRemovePointerLike<Ty>::type;

    if constexpr (std::is_void_v<Tz>) {
        // early out to avoid expanding the rest of the templates
        return &QMetaTypeInterfaceWrapper<Ty>::metaType;
    } else if constexpr (std::is_void_v<Unique>) {
        checkTypeIsSuitableForMetaType<Ty>();
        return &QMetaTypeInterfaceWrapper<Ty>::metaType;
    } else if constexpr (std::is_reference_v<Tz>) {
        return nullptr;
    } else if constexpr (!is_complete<Tz, Unique>::value) {
        return nullptr;
    } else {
        // don't check the type is suitable here
        return &QMetaTypeInterfaceWrapper<Ty>::metaType;
    }
}

} // namespace BobUIPrivate

template<typename T>
constexpr QMetaType QMetaType::fromType()
{
    BobUIPrivate::checkTypeIsSuitableForMetaType<T>();
    return QMetaType(BobUIPrivate::qMetaTypeInterfaceForType<T>());
}

constexpr bool QMetaType::isValid(BOBUI6_IMPL_NEW_OVERLOAD) const noexcept
{
    return d_ptr;
}

bool QMetaType::isRegistered(BOBUI6_IMPL_NEW_OVERLOAD) const noexcept
{
    return d_ptr && d_ptr->typeId.loadRelaxed();
}

constexpr qsizetype QMetaType::sizeOf() const
{
    return d_ptr ? d_ptr->size : 0;
}

constexpr qsizetype QMetaType::alignOf() const
{
    return d_ptr ? d_ptr->alignment : 0;
}

constexpr QMetaType::TypeFlags QMetaType::flags() const
{
    return d_ptr ? TypeFlags(d_ptr->flags) : TypeFlags{};
}

constexpr const QMetaObject *QMetaType::metaObject() const
{
    return d_ptr && d_ptr->metaObjectFn ? d_ptr->metaObjectFn(d_ptr) : nullptr;
}

constexpr const char *QMetaType::name() const
{
    return d_ptr ? d_ptr->name : nullptr;
}

inline size_t qHash(QMetaType type, size_t seed = 0)
{
    // We cannot use d_ptr here since the same type in different DLLs
    // might result in different pointers!
    return qHash(type.id(), seed);
}

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(BobUIMetaTypePrivate::QPairVariantInterfaceImpl,
                               QPairVariantInterfaceImpl, Q_CORE_EXPORT)

#endif // QMETATYPE_H
