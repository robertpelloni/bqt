// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUICore/qjnitypes.h>
#include <BobUICore/qjniarray.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class tst_QJniTypes : public QObject
{
    Q_OBJECT

public:
    tst_QJniTypes() = default;

    static void nativeClassMethod(JNIEnv *, jclass, int);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(nativeClassMethod);

private slots:
    void initTestCase();
    void nativeMethod();
    void construct();
    void stringTypeCantBeArgument();
};

struct BobUIJavaWrapper {};
template<>
struct BobUIJniTypes::Traits<BobUIJavaWrapper>
{
    static constexpr auto signature()
    {
        return BobUIJniTypes::CTString("Lorg/bobuiproject/bobui/android/BobUIJavaWrapper;");
    }
};

struct BobUICustomJniObject : QJniObject {};

template<>
struct BobUIJniTypes::Traits<BobUICustomJniObject>
{
    static constexpr auto signature()
    {
        return BobUIJniTypes::CTString("Lorg/bobuiproject/bobui/android/BobUICustomJniObject;");
    }
};

static_assert(BobUIJniTypes::Traits<BobUIJavaWrapper>::signature() == "Lorg/bobuiproject/bobui/android/BobUIJavaWrapper;");
static_assert(BobUIJniTypes::Traits<BobUIJavaWrapper>::signature() != "Ljava/lang/Object;");
static_assert(!(BobUIJniTypes::Traits<BobUIJavaWrapper>::signature() == "X"));

Q_DECLARE_JNI_CLASS(JavaType, "org/bobuiproject/bobui/JavaType");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::JavaType>::signature() == "Lorg/bobuiproject/bobui/JavaType;");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::JavaType[]>::signature() == "[Lorg/bobuiproject/bobui/JavaType;");

static_assert(BobUIJniTypes::Traits<jstring>::className() == "java/lang/String");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::String>::className() == "java/lang/String");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::String>::signature() == "Ljava/lang/String;");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::String[]>::signature() == "[Ljava/lang/String;");

Q_DECLARE_JNI_CLASS(BobUITextToSpeech, "org/bobuiproject/bobui/android/speech/BobUITextToSpeech")
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::BobUITextToSpeech>::className() == "org/bobuiproject/bobui/android/speech/BobUITextToSpeech");

// declaring two types Size in different packages
Q_DECLARE_JNI_CLASS(android, util, Size)
Q_DECLARE_JNI_CLASS(org, bobuiproject, android, Size)

static_assert(BobUIJniTypes::Traits<BobUIJniTypes::android::util::Size>::className() == "android/util/Size");
static_assert(BobUIJniTypes::Traits<BobUIJniTypes::org::bobuiproject::android::Size>::className() == "org/bobuiproject/android/Size");

using namespace BobUIJniTypes::org::bobuiproject;
static_assert(BobUIJniTypes::Traits<android::Size>::className() == "org/bobuiproject/android/Size");

static_assert(BobUIJniTypes::fieldSignature<jint>() == "I");
static_assert(BobUIJniTypes::fieldSignature<jint[]>() == "[I");
static_assert(BobUIJniTypes::fieldSignature<jint>() != "X");
static_assert(BobUIJniTypes::fieldSignature<jint>() != "Ljava/lang/Object;");
static_assert(BobUIJniTypes::fieldSignature<jlong>() == "J");
static_assert(BobUIJniTypes::fieldSignature<jstring>() == "Ljava/lang/String;");
static_assert(BobUIJniTypes::fieldSignature<jobject>() == "Ljava/lang/Object;");
static_assert(BobUIJniTypes::fieldSignature<jobject[]>() == "[Ljava/lang/Object;");
static_assert(BobUIJniTypes::fieldSignature<jobjectArray>() == "[Ljava/lang/Object;");
static_assert(BobUIJniTypes::fieldSignature<QJniObject>() == "Ljava/lang/Object;");
static_assert(BobUIJniTypes::fieldSignature<BobUIJavaWrapper>() == "Lorg/bobuiproject/bobui/android/BobUIJavaWrapper;");
static_assert(BobUIJniTypes::fieldSignature<BobUIJavaWrapper[]>() == "[Lorg/bobuiproject/bobui/android/BobUIJavaWrapper;");
static_assert(BobUIJniTypes::fieldSignature<BobUICustomJniObject>() == "Lorg/bobuiproject/bobui/android/BobUICustomJniObject;");

static_assert(BobUIJniTypes::methodSignature<void>() == "()V");
static_assert(BobUIJniTypes::methodSignature<void>() != "()X");
static_assert(BobUIJniTypes::methodSignature<void, jint>() == "(I)V");
static_assert(BobUIJniTypes::methodSignature<void, jint, jstring>() == "(ILjava/lang/String;)V");
static_assert(BobUIJniTypes::methodSignature<jlong, jint, jclass>() == "(ILjava/lang/Class;)J");
static_assert(BobUIJniTypes::methodSignature<jobject, jint, jstring>() == "(ILjava/lang/String;)Ljava/lang/Object;");
static_assert(BobUIJniTypes::methodSignature<BobUIJniTypes::JavaType, jint, jstring>()
                                      == "(ILjava/lang/String;)Lorg/bobuiproject/bobui/JavaType;");

static_assert(BobUIJniTypes::isPrimitiveType<jint>());
static_assert(BobUIJniTypes::isPrimitiveType<void>());
static_assert(!BobUIJniTypes::isPrimitiveType<jobject>());
static_assert(!BobUIJniTypes::isPrimitiveType<BobUICustomJniObject>());

static_assert(!BobUIJniTypes::isObjectType<jint>());
static_assert(!BobUIJniTypes::isObjectType<void>());
static_assert(BobUIJniTypes::isObjectType<jobject>());
static_assert(BobUIJniTypes::isObjectType<jobjectArray>());
static_assert(BobUIJniTypes::isObjectType<BobUICustomJniObject>());

static_assert(!BobUIJniTypes::isArrayType<jint>());
static_assert(BobUIJniTypes::isArrayType<jint[]>());
static_assert(BobUIJniTypes::isArrayType<jobject[]>());
static_assert(BobUIJniTypes::isArrayType<jobjectArray>());
static_assert(BobUIJniTypes::isArrayType<BobUIJavaWrapper[]>());

static_assert(BobUIJniTypes::CTString("ABCDE").startsWith("ABC"));
static_assert(BobUIJniTypes::CTString("ABCDE").startsWith("A"));
static_assert(BobUIJniTypes::CTString("ABCDE").startsWith("ABCDE"));
static_assert(!BobUIJniTypes::CTString("ABCDE").startsWith("ABCDEF"));
static_assert(!BobUIJniTypes::CTString("ABCDE").startsWith("9AB"));
static_assert(BobUIJniTypes::CTString("ABCDE").startsWith('A'));
static_assert(!BobUIJniTypes::CTString("ABCDE").startsWith('B'));

static_assert(BobUIJniTypes::Traits<QJniArray<jobject>>::signature() == "[Ljava/lang/Object;");
static_assert(BobUIJniTypes::Traits<QJniArray<jbyte>>::signature() == "[B");
static_assert(BobUIJniTypes::isObjectType<QJniArray<jbyte>>());

static_assert(BobUIJniTypes::CTString("ABCDE").endsWith("CDE"));
static_assert(BobUIJniTypes::CTString("ABCDE").endsWith("E"));
static_assert(BobUIJniTypes::CTString("ABCDE").endsWith("ABCDE"));
static_assert(!BobUIJniTypes::CTString("ABCDE").endsWith("DEF"));
static_assert(!BobUIJniTypes::CTString("ABCDE").endsWith("ABCDEF"));
static_assert(BobUIJniTypes::CTString("ABCDE").endsWith('E'));
static_assert(!BobUIJniTypes::CTString("ABCDE").endsWith('F'));

enum UnscopedEnum {};
enum class ScopedEnum {};
enum class IntEnum : int {};
enum class UnsignedEnum : unsigned {};
enum class Int8Enum : int8_t {};
enum class ShortEnum : short {};
enum class LongEnum : quint64 {};
enum class JIntEnum : jint {};

static_assert(BobUIJniTypes::Traits<UnscopedEnum>::signature() == "I");
static_assert(BobUIJniTypes::Traits<ScopedEnum>::signature() == "I");
static_assert(BobUIJniTypes::Traits<IntEnum>::signature() == "I");
static_assert(BobUIJniTypes::Traits<UnsignedEnum>::signature() == "I");
static_assert(BobUIJniTypes::Traits<Int8Enum>::signature() == "B");
static_assert(BobUIJniTypes::Traits<LongEnum>::signature() == "J");
static_assert(BobUIJniTypes::Traits<JIntEnum>::signature() == "I");

void tst_QJniTypes::initTestCase()
{

}

static bool nativeFunction(JNIEnv *, jclass, int, jstring, quint64)
{
    return true;
}
Q_DECLARE_JNI_NATIVE_METHOD(nativeFunction)

static_assert(BobUIJniTypes::nativeMethodSignature(nativeFunction) == "(ILjava/lang/String;J)Z");

static QString nativeFunctionStrings(JNIEnv *, jclass, const QString &, const BobUIJniTypes::String &)
{
    return QString();
}
Q_DECLARE_JNI_NATIVE_METHOD(nativeFunctionStrings)

static_assert(BobUIJniTypes::nativeMethodSignature(nativeFunctionStrings)
                == "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

static int forwardDeclaredNativeFunction(JNIEnv *, jobject, bool);
Q_DECLARE_JNI_NATIVE_METHOD(forwardDeclaredNativeFunction)
static int forwardDeclaredNativeFunction(JNIEnv *, jobject, bool) { return 0; }
static_assert(BobUIJniTypes::nativeMethodSignature(forwardDeclaredNativeFunction) == "(Z)I");

static_assert(BobUIJniTypes::nativeMethodSignature(tst_QJniTypes::nativeClassMethod) == "(I)V");
void tst_QJniTypes::nativeClassMethod(JNIEnv *, jclass, int) {}

void tst_QJniTypes::nativeMethod()
{
    using namespace BobUIJniMethods;
    {
        const auto method = Q_JNI_NATIVE_METHOD(nativeFunction);
        QVERIFY(method.fnPtr == &(nativeFunction_Helper::call<bool, jclass, int, jstring, quint64>));
        QCOMPARE(method.name, "nativeFunction");
        QCOMPARE(method.signature, "(ILjava/lang/String;J)Z");
    }

    {
        const auto method = Q_JNI_NATIVE_METHOD(forwardDeclaredNativeFunction);
        QVERIFY(method.fnPtr == &(forwardDeclaredNativeFunction_Helper::call<int, jobject, bool>));
    }

    {
        const auto method = Q_JNI_NATIVE_SCOPED_METHOD(nativeClassMethod, tst_QJniTypes);
        QVERIFY(method.fnPtr == &(nativeClassMethod_BobUIJniMethod::call<void, jclass, int>));
    }
}

void tst_QJniTypes::construct()
{
    using namespace BobUIJniTypes;

    const QString text = u"Java String"_s;
    String str(text);
    QVERIFY(str.isValid());
    QCOMPARE(str.toString(), text);

    jobject jref = nullptr; // must be jobject, not jstring
    {
        // if jref would be a jstring, then this would call the
        // Java String copy constructor!
        String jstr(jref);
        QVERIFY(!jstr.isValid());
    }
    jref = str.object<jstring>();
    {
        String jstr(jref);
        QVERIFY(jstr.isValid());
        QCOMPARE(jstr.toString(), text);
    }

    String str2 = str;
    QCOMPARE(str.toString(), text);
    String str3 = std::move(str2);
    QCOMPARE(str3.toString(), text);
}

template <typename ...Arg>
static constexpr bool isValidArgument(Arg &&...) noexcept
{
    return BobUIJniTypes::ValidSignatureTypesDetail<q20::remove_cvref_t<Arg>...>;
}

enum class Overload
{
    ClassNameAndMethod,
    OnlyMethod,
};

template <typename Ret, typename ...Args
#ifndef Q_QDOC
    , BobUIJniTypes::IfValidSignatureTypes<Ret, Args...> = true
#endif
>
static constexpr auto callStaticMethod(const char *className, const char *methodName, Args &&...)
{
    Q_UNUSED(className);
    Q_UNUSED(methodName);
    return Overload::ClassNameAndMethod;
}

template <typename Klass, typename Ret, typename ...Args
#ifndef Q_QDOC
    , BobUIJniTypes::IfValidSignatureTypes<Ret, Args...> = true
#endif
>
static constexpr auto callStaticMethod(const char *methodName, Args &&...)
{
    Q_UNUSED(methodName);
    return Overload::OnlyMethod;
}

void tst_QJniTypes::stringTypeCantBeArgument()
{
    const char *methodName = "staticEchoMethod";

    static_assert(!isValidArgument(BobUIJniTypes::Traits<BobUIJniTypes::JavaType>::className()));
    static_assert(!isValidArgument("someFunctionName"));
    static_assert(!isValidArgument(methodName));
    static_assert(!isValidArgument(BobUIJniTypes::Traits<BobUIJniTypes::JavaType>::className(),
                                   "someFunctionName", methodName, 42));

    static_assert(callStaticMethod<jstring, jint>("class name", "method name", 42)
                  == Overload::ClassNameAndMethod);
    static_assert(callStaticMethod<BobUIJniTypes::JavaType, jint>("method name", 42)
                  == Overload::OnlyMethod);
}

BOBUI_END_NAMESPACE

BOBUIEST_MAIN(tst_QJniTypes)

#include "tst_qjnitypes.moc"
