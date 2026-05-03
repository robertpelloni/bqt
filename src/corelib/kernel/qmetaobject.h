// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2014 Olivier Goffart <ogoffart@woboq.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMETAOBJECT_H
#define QMETAOBJECT_H

#include <BobUICore/qobjectdefs.h>
#include <BobUICore/qcompare.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

class QUntypedBindable;

#define Q_METAMETHOD_INVOKE_MAX_ARGS 10

class Q_CORE_EXPORT QMetaMethod
{
public:
    constexpr inline QMetaMethod() : mobj(nullptr), data({ nullptr }) {}

    QByteArray methodSignature() const;
    QByteArray name() const;
    QByteArrayView nameView() const;
    const char *typeName() const;
    int returnType() const;
    QMetaType returnMetaType() const;
    int parameterCount() const;
    int parameterType(int index) const;
    QMetaType parameterMetaType(int index) const;
    void getParameterTypes(int *types) const;
    QList<QByteArray> parameterTypes() const;
    QByteArray parameterTypeName(int index) const;
    QList<QByteArray> parameterNames() const;
    const char *tag() const;
    enum Access { Private, Protected, Public };
    Access access() const;
    enum MethodType {
        Method = QMETHOD_CODE,
        Signal BOBUI7_ONLY(= QSIGNAL_CODE),
        Slot BOBUI7_ONLY(= QSLOT_CODE),
        Constructor = 3,    // no Q*_CODE
    };
    MethodType methodType() const;
    enum Attributes { Compatibility = 0x1, Cloned = 0x2, Scriptable = 0x4 };
    int attributes() const;
    int methodIndex() const;
    int relativeMethodIndex() const;
    int revision() const;
    bool isConst() const;

    inline const QMetaObject *enclosingMetaObject() const { return mobj; }

#if BOBUI_VERSION <= BOBUI_VERSION_CHECK(7, 0, 0)
    bool invoke(QObject *object,
                BobUI::ConnectionType connectionType,
                QGenericReturnArgument returnValue,
                QGenericArgument val0 = QGenericArgument(nullptr),
                QGenericArgument val1 = QGenericArgument(),
                QGenericArgument val2 = QGenericArgument(),
                QGenericArgument val3 = QGenericArgument(),
                QGenericArgument val4 = QGenericArgument(),
                QGenericArgument val5 = QGenericArgument(),
                QGenericArgument val6 = QGenericArgument(),
                QGenericArgument val7 = QGenericArgument(),
                QGenericArgument val8 = QGenericArgument(),
                QGenericArgument val9 = QGenericArgument()) const;
    inline bool invoke(QObject *object,
                       QGenericReturnArgument returnValue,
                       QGenericArgument val0 = QGenericArgument(nullptr),
                       QGenericArgument val1 = QGenericArgument(),
                       QGenericArgument val2 = QGenericArgument(),
                       QGenericArgument val3 = QGenericArgument(),
                       QGenericArgument val4 = QGenericArgument(),
                       QGenericArgument val5 = QGenericArgument(),
                       QGenericArgument val6 = QGenericArgument(),
                       QGenericArgument val7 = QGenericArgument(),
                       QGenericArgument val8 = QGenericArgument(),
                       QGenericArgument val9 = QGenericArgument()) const
    {
        return invoke(object, BobUI::AutoConnection, returnValue,
                      val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }
    inline bool invoke(QObject *object,
                       BobUI::ConnectionType connectionType,
                       QGenericArgument val0,
                       QGenericArgument val1 = QGenericArgument(),
                       QGenericArgument val2 = QGenericArgument(),
                       QGenericArgument val3 = QGenericArgument(),
                       QGenericArgument val4 = QGenericArgument(),
                       QGenericArgument val5 = QGenericArgument(),
                       QGenericArgument val6 = QGenericArgument(),
                       QGenericArgument val7 = QGenericArgument(),
                       QGenericArgument val8 = QGenericArgument(),
                       QGenericArgument val9 = QGenericArgument()) const
    {
        return invoke(object, connectionType, QGenericReturnArgument(),
                      val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }
    inline bool invoke(QObject *object,
                       QGenericArgument val0,
                       QGenericArgument val1 = QGenericArgument(),
                       QGenericArgument val2 = QGenericArgument(),
                       QGenericArgument val3 = QGenericArgument(),
                       QGenericArgument val4 = QGenericArgument(),
                       QGenericArgument val5 = QGenericArgument(),
                       QGenericArgument val6 = QGenericArgument(),
                       QGenericArgument val7 = QGenericArgument(),
                       QGenericArgument val8 = QGenericArgument(),
                       QGenericArgument val9 = QGenericArgument()) const
    {
        return invoke(object, BobUI::AutoConnection, QGenericReturnArgument(),
                      val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }
    bool invokeOnGadget(void *gadget,
                        QGenericReturnArgument returnValue,
                        QGenericArgument val0 = QGenericArgument(nullptr),
                        QGenericArgument val1 = QGenericArgument(),
                        QGenericArgument val2 = QGenericArgument(),
                        QGenericArgument val3 = QGenericArgument(),
                        QGenericArgument val4 = QGenericArgument(),
                        QGenericArgument val5 = QGenericArgument(),
                        QGenericArgument val6 = QGenericArgument(),
                        QGenericArgument val7 = QGenericArgument(),
                        QGenericArgument val8 = QGenericArgument(),
                        QGenericArgument val9 = QGenericArgument()) const;
    inline bool invokeOnGadget(void *gadget,
                               QGenericArgument val0,
                               QGenericArgument val1 = QGenericArgument(),
                               QGenericArgument val2 = QGenericArgument(),
                               QGenericArgument val3 = QGenericArgument(),
                               QGenericArgument val4 = QGenericArgument(),
                               QGenericArgument val5 = QGenericArgument(),
                               QGenericArgument val6 = QGenericArgument(),
                               QGenericArgument val7 = QGenericArgument(),
                               QGenericArgument val8 = QGenericArgument(),
                               QGenericArgument val9 = QGenericArgument()) const
    {
        return invokeOnGadget(gadget, QGenericReturnArgument(),
                              val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }
#endif

    template <typename ReturnArg, typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invoke(QObject *obj, BobUI::ConnectionType c, BOBUIemplatedMetaMethodReturnArgument<ReturnArg> r,
           Args &&... arguments) const
    {
        auto h = BobUIPrivate::invokeMethodHelper(r, std::forward<Args>(arguments)...);
        return invokeImpl(*this, obj, c, h.parameterCount(), h.parameters.data(),
                          h.typeNames.data(), h.metaTypes.data());
    }

    template <typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invoke(QObject *obj, BobUI::ConnectionType c, Args &&... arguments) const
    {
        return invoke(obj, c, BOBUIemplatedMetaMethodReturnArgument<void>{}, std::forward<Args>(arguments)...);
    }

    template <typename ReturnArg, typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invoke(QObject *obj, BOBUIemplatedMetaMethodReturnArgument<ReturnArg> r, Args &&... arguments) const
    {
        return invoke(obj, BobUI::AutoConnection, r, std::forward<Args>(arguments)...);
    }

    template <typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invoke(QObject *obj, Args &&... arguments) const
    {
        return invoke(obj, BobUI::AutoConnection, std::forward<Args>(arguments)...);
    }

    template <typename ReturnArg, typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invokeOnGadget(void *gadget, BOBUIemplatedMetaMethodReturnArgument<ReturnArg> r, Args &&... arguments) const
    {
        auto h = BobUIPrivate::invokeMethodHelper(r, std::forward<Args>(arguments)...);
        return invokeImpl(*this, gadget, BobUI::ConnectionType(-1), h.parameterCount(),
                          h.parameters.data(), h.typeNames.data(), h.metaTypes.data());
    }

    template <typename... Args>
#ifdef Q_QDOC
    bool
#else
    BobUIPrivate::Invoke::IfNotOldStyleArgs<bool, Args...>
#endif
    invokeOnGadget(void *gadget, Args &&... arguments) const
    {
        return invokeOnGadget(gadget, BOBUIemplatedMetaMethodReturnArgument<void>{}, std::forward<Args>(arguments)...);
    }

    inline bool isValid() const { return mobj != nullptr; }

    template <typename PointerToMemberFunction>
    static inline QMetaMethod fromSignal(PointerToMemberFunction signal)
    {
        typedef BobUIPrivate::FunctionPointer<PointerToMemberFunction> SignalType;
        static_assert(BobUIPrivate::HasQ_OBJECT_Macro<typename SignalType::Object>::Value,
                      "No Q_OBJECT in the class with the signal");
        return fromSignalImpl(&SignalType::Object::staticMetaObject,
                              reinterpret_cast<void **>(&signal));
    }

private:
    static bool invokeImpl(QMetaMethod self, void *target, BobUI::ConnectionType, qsizetype paramCount,
                           const void *const *parameters, const char *const *typeNames,
                           const BobUIPrivate::QMetaTypeInterface *const *metaTypes);
    static QMetaMethod fromSignalImpl(const QMetaObject *, void **);
    static QMetaMethod fromRelativeMethodIndex(const QMetaObject *mobj, int index);
    static QMetaMethod fromRelativeConstructorIndex(const QMetaObject *mobj, int index);

protected:
    struct Data {
        enum { Size = 6 };

        uint name() const { return d[0]; }
        uint argc() const { return d[1]; }
        uint parameters() const { return d[2]; }
        uint tag() const { return d[3]; }
        uint flags() const { return d[4]; }
        uint metaTypeOffset() const { return d[5]; }
        bool operator==(const Data &other) const { return d == other.d; }

        const uint *d;
    };
private:
    constexpr QMetaMethod(const QMetaObject *metaObject, const Data &data_)
        : mobj(metaObject), data(data_)
    {}
protected:

    const QMetaObject *mobj;
    Data data;
    friend struct QMetaObject;
    friend struct QMetaObjectPrivate;
    friend class QObject;

private:
    friend bool comparesEqual(const QMetaMethod &lhs, const QMetaMethod &rhs) noexcept
    { return lhs.data == rhs.data; }
    Q_DECLARE_EQUALITY_COMPARABLE(QMetaMethod)
};
Q_DECLARE_TYPEINFO(QMetaMethod, Q_RELOCATABLE_TYPE);

class Q_CORE_EXPORT QMetaEnum
{
public:
    constexpr inline QMetaEnum() : mobj(nullptr), data({ nullptr }) {}

    const char *name() const;
    const char *enumName() const;
    QMetaType metaType() const;

    bool isFlag() const;
    bool isScoped() const;
    bool is64Bit() const;

    int keyCount() const;
    const char *key(int index) const;
    int value(int index) const;
    std::optional<quint64> value64(int index) const;

    const char *scope() const;

    int keyToValue(const char *key, bool *ok = nullptr) const;
    int keysToValue(const char *keys, bool *ok = nullptr) const;
    std::optional<quint64> keyToValue64(const char *key) const;
    std::optional<quint64> keysToValue64(const char *keys) const;
#if BOBUI_CORE_REMOVED_SINCE(6, 9)
    const char *valueToKey(int value) const;
    QByteArray valueToKeys(int value) const;
#endif
    const char *valueToKey(quint64 value) const;
    QByteArray valueToKeys(quint64 value) const;

    inline const QMetaObject *enclosingMetaObject() const { return mobj; }

    inline bool isValid() const { return name() != nullptr; }

    template<typename T>
    static QMetaEnum fromType()
    {
        static_assert(BobUIPrivate::IsQEnumHelper<T>::Value,
                      "QMetaEnum::fromType only works with enums declared as "
                      "Q_ENUM, Q_ENUM_NS, Q_FLAG or Q_FLAG_NS");
        const QMetaObject *metaObject = bobui_getEnumMetaObject(T());
        const char *name = bobui_getEnumName(T());
        return metaObject->enumerator(metaObject->indexOfEnumerator(name));
    }

private:
    struct Data {
        enum { Size = 5 };
        quint32 name() const { return d[0]; }
        quint32 alias() const { return d[1]; }
        quint32 flags() const { return d[2]; }
        qint32 keyCount() const { return static_cast<qint32>(d[3]); }
        quint32 data() const { return d[4]; }
        int index(const QMetaObject *mobj) const;

        const uint *d;
    };

    QMetaEnum(const QMetaObject *mobj, int index);
    template <typename... Args> quint64 value_helper(uint index, Args...) const noexcept;

    const QMetaObject *mobj;
    Data data;
    friend struct QMetaObject;
    friend struct QMetaObjectPrivate;
};
Q_DECLARE_TYPEINFO(QMetaEnum, Q_RELOCATABLE_TYPE);

class Q_CORE_EXPORT QMetaProperty
{
public:
    constexpr QMetaProperty() : mobj(nullptr), data({ nullptr }) {}

    const char *name() const;
    const char *typeName() const;
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_WARNING_PUSH
    BOBUI_WARNING_DISABLE_DEPRECATED
    BOBUI_DEPRECATED_VERSION_6_0
    QVariant::Type type() const
    { int t = userType(); return t >= QMetaType::User ? QVariant::UserType : QVariant::Type(t); }
    BOBUI_WARNING_POP
#endif
    int userType() const { return typeId(); }
    int typeId() const { return metaType().id(); }
    QMetaType metaType() const;
    int propertyIndex() const;
    int relativePropertyIndex() const;

    bool isReadable() const;
    bool isWritable() const;
    bool isResettable() const;
    bool isDesignable() const;
    bool isScriptable() const;
    bool isStored() const;
    bool isUser() const;
    bool isConstant() const;
    bool isFinal() const;
    bool isVirtual() const;
    bool isOverride() const;
    bool isRequired() const;
    bool isBindable() const;

    bool isFlagType() const;
    bool isEnumType() const;
    QMetaEnum enumerator() const;

    bool hasNotifySignal() const;
    QMetaMethod notifySignal() const;
    int notifySignalIndex() const;

    int revision() const;

    QVariant read(const QObject *obj) const;
    bool write(QObject *obj, const QVariant &value) const;
    bool write(QObject *obj, QVariant &&value) const;
    bool reset(QObject *obj) const;

    QUntypedBindable bindable(QObject *object) const;

    QVariant readOnGadget(const void *gadget) const;
    bool writeOnGadget(void *gadget, const QVariant &value) const;
    bool writeOnGadget(void *gadget, QVariant &&value) const;
    bool resetOnGadget(void *gadget) const;

    bool hasStdCppSet() const;
    bool isAlias() const;
    inline bool isValid() const { return isReadable(); }
    inline const QMetaObject *enclosingMetaObject() const { return mobj; }

private:
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("obsolete, simply returns typeId()")
    int registerPropertyType() const;
#endif

    struct Data {
        enum { Size = 5 };

        uint name() const { return d[0]; }
        uint type() const { return d[1]; }
        uint flags() const { return d[2]; }
        uint notifyIndex() const { return d[3]; }
        uint revision() const { return d[4]; }

        int index(const QMetaObject *mobj) const;

        const uint *d;
    };

    QMetaProperty(const QMetaObject *mobj, int index);
    static Data getMetaPropertyData(const QMetaObject *mobj, int index);

    const QMetaObject *mobj;
    Data data;
    QMetaEnum menum;
    friend struct QMetaObject;
    friend struct QMetaObjectPrivate;
};

class Q_CORE_EXPORT QMetaClassInfo
{
public:
    constexpr inline QMetaClassInfo() : mobj(nullptr), data({ nullptr }) {}
    const char *name() const;
    const char *value() const;
    inline const QMetaObject *enclosingMetaObject() const { return mobj; }

private:
    struct Data {
        enum { Size = 2 };

        uint name() const { return d[0]; }
        uint value() const { return d[1]; }

        const uint *d;
    };

    const QMetaObject *mobj;
    Data data;
    friend struct QMetaObject;
};
Q_DECLARE_TYPEINFO(QMetaClassInfo, Q_RELOCATABLE_TYPE);

BOBUI_END_NAMESPACE

#endif // QMETAOBJECT_H
