// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/QCoreApplication>

#include <BobUIDBus/private/qdbusutil_p.h>
#include <BobUIDBus/private/qdbus_symbols_p.h>

DEFINEFUNC(dbus_bool_t, dbus_signature_validate, (const char       *signature,
                                                DBusError        *error),
           (signature, error), return)
DEFINEFUNC(dbus_bool_t, dbus_signature_validate_single, (const char       *signature,
                                                         DBusError        *error),
           (signature, error), return)
DEFINEFUNC(dbus_bool_t, dbus_type_is_basic, (int            typecode),
           (typecode), return)
DEFINEFUNC(dbus_bool_t, dbus_type_is_fixed, (int            typecode),
           (typecode), return)

class tst_QDBusType : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void isValidFixedType_data();
    void isValidFixedType();
    void isValidBasicType_data();
    void isValidBasicType();
    void isValidSingleSignature_data();
    void isValidSingleSignature();
    void isValidArray_data();
    void isValidArray();
    void isValidSignature_data();
    void isValidSignature();
};

enum { Invalid = false, Valid = true };

static void addColumns()
{
    // All tests use these two columns only
    BOBUIest::addColumn<QString>("data");
    BOBUIest::addColumn<bool>("result");
    BOBUIest::addColumn<bool>("isValid");
}

// ---- type adds ---
static void addFixedTypes()
{
    BOBUIest::newRow("bool") << DBUS_TYPE_BOOLEAN_AS_STRING << true << true;
    BOBUIest::newRow("byte") << DBUS_TYPE_BYTE_AS_STRING << true << true;
    BOBUIest::newRow("int16") << DBUS_TYPE_INT16_AS_STRING << true << true;
    BOBUIest::newRow("uint16") << DBUS_TYPE_UINT16_AS_STRING << true << true;
    BOBUIest::newRow("int32") << DBUS_TYPE_INT32_AS_STRING << true << true;
    BOBUIest::newRow("uint32") << DBUS_TYPE_UINT32_AS_STRING << true << true;
    BOBUIest::newRow("int64") << DBUS_TYPE_INT64_AS_STRING << true << true;
    BOBUIest::newRow("uint64") << DBUS_TYPE_UINT64_AS_STRING << true << true;
    BOBUIest::newRow("double") << DBUS_TYPE_DOUBLE_AS_STRING << true << true;

#ifdef DBUS_TYPE_UNIX_FD_AS_STRING
#  ifndef BOBUI_LINKED_LIBDBUS
    // We have got the macro from dbus_minimal_p.h, so we need to check if
    // the library recognizes this as valid type first.
    // The following function was added for Unix FD support, so if it is
    // present, so is support for Unix FDs.
#    if BOBUI_CONFIG(library)
    bool supportsUnixFds = qdbus_resolve_conditionally("dbus_connection_can_send_type");
#    else
    bool supportsUnixFds = false;
#    endif
#  else
    bool supportsUnixFds = true;
#  endif
    if (supportsUnixFds)
        BOBUIest::newRow("unixfd") << DBUS_TYPE_UNIX_FD_AS_STRING << true << true;
#endif
}

static void addInvalidSingleLetterTypes()
{
    QChar nulString[] = { '\0' };
    BOBUIest::newRow("nul") << QString(nulString, 1) << false << false;
    BOBUIest::newRow("tilde") << "~" << false << false;
    BOBUIest::newRow("struct-begin") << "(" << false << false;
    BOBUIest::newRow("struct-end") << ")" << false << false;
    BOBUIest::newRow("dict-entry-begin") << "{" << false << false;
    BOBUIest::newRow("dict-entry-end") << "}" << false << false;
    BOBUIest::newRow("array-no-element") << "a" << false << false;
}

static void addBasicTypes(bool basicsAreValid)
{
    addFixedTypes();
    BOBUIest::newRow("string") << DBUS_TYPE_STRING_AS_STRING << basicsAreValid << true;
    BOBUIest::newRow("object-path") << DBUS_TYPE_OBJECT_PATH_AS_STRING << basicsAreValid << true;
    BOBUIest::newRow("signature") << DBUS_TYPE_SIGNATURE_AS_STRING << basicsAreValid << true;
}

static void addVariant(bool variantIsValid)
{
    BOBUIest::newRow("variant") << "v" << variantIsValid << true;
}

static void addSingleSignatures()
{
    addBasicTypes(Valid);
    addVariant(Valid);
    BOBUIest::newRow("struct-1") << "(y)" << true;
    BOBUIest::newRow("struct-2") << "(yy)" << true;
    BOBUIest::newRow("struct-3") << "(yyv)" << true;

    BOBUIest::newRow("struct-nested-1") << "((y))" << true;
    BOBUIest::newRow("struct-nested-2") << "((yy))" << true;
    BOBUIest::newRow("struct-nested-3") << "(y(y))" << true;
    BOBUIest::newRow("struct-nested-4") << "((y)y)" << true;
    BOBUIest::newRow("struct-nested-5") << "(y(y)y)" << true;
    BOBUIest::newRow("struct-nested-6") << "((y)(y))" << true;

    BOBUIest::newRow("array-1") << "as" << true;
    BOBUIest::newRow("struct-array-1") << "(as)" << true;
    BOBUIest::newRow("struct-array-2") << "(yas)" << true;
    BOBUIest::newRow("struct-array-3") << "(asy)" << true;
    BOBUIest::newRow("struct-array-4") << "(yasy)" << true;

    BOBUIest::newRow("dict-1") << "a{sy}" << true;
    BOBUIest::newRow("dict-2") << "a{sv}" << true;
    BOBUIest::newRow("dict-struct-1") << "a{s(y)}" << true;
    BOBUIest::newRow("dict-struct-2") << "a{s(yyyy)}" << true;
    BOBUIest::newRow("dict-struct-array") << "a{s(ay)}" << true;
    BOBUIest::newRow("dict-array") << "a{sas}" << true;
    BOBUIest::newRow("dict-array-struct") << "a{sa(y)}" << true;

    addInvalidSingleLetterTypes();
    BOBUIest::newRow("naked-dict-empty") << "{}" << false;
    BOBUIest::newRow("naked-dict-missing-value") << "{i}" << false;

    BOBUIest::newRow("dict-empty") << "a{}" << false;
    BOBUIest::newRow("dict-missing-value") << "a{i}" << false;
    BOBUIest::newRow("dict-non-basic-key") << "a{vi}" << false;
    BOBUIest::newRow("dict-struct-key") << "a{(y)y}" << false;
    BOBUIest::newRow("dict-missing-close") << "a{sv" << false;
    BOBUIest::newRow("dict-mismatched-close") << "a{sv)" << false;
    BOBUIest::newRow("dict-missing-value-close") << "a{s" << false;

    BOBUIest::newRow("empty-struct") << "()" << false;
    BOBUIest::newRow("struct-missing-close") << "(s" << false;
    BOBUIest::newRow("struct-nested-missing-close-1") << "((s)" << false;
    BOBUIest::newRow("struct-nested-missing-close-2") << "((s" << false;

    BOBUIest::newRow("struct-ending-array-no-element") << "(a)" << false;
}

static void addNakedDictEntry()
{
    BOBUIest::newRow("naked-dict-entry") << "{sv}" << false;
}

// ---- tests ----

void tst_QDBusType::isValidFixedType_data()
{
    addColumns();
    addBasicTypes(Invalid);
    addVariant(Invalid);
    addInvalidSingleLetterTypes();
}

void tst_QDBusType::isValidFixedType()
{
    QFETCH(QString, data);
    QFETCH(bool, result);
    QFETCH(bool, isValid);
    QVERIFY2(data.size() == 1, "Test is malformed, this function must test only one-letter types");
    QVERIFY(isValid || !result);

    int type = data.at(0).unicode();
    if (isValid)
        QCOMPARE(bool(q_dbus_type_is_fixed(type)), result);
    QCOMPARE(QDBusUtil::isValidFixedType(type), result);
}

void tst_QDBusType::isValidBasicType_data()
{
    addColumns();
    addBasicTypes(Valid);
    addVariant(Invalid);
    addInvalidSingleLetterTypes();
}

void tst_QDBusType::isValidBasicType()
{
    QFETCH(QString, data);
    QFETCH(bool, result);
    QFETCH(bool, isValid);
    QVERIFY2(data.size() == 1, "Test is malformed, this function must test only one-letter types");
    QVERIFY(isValid || !result);

    int type = data.at(0).unicode();
    if (isValid)
        QCOMPARE(bool(q_dbus_type_is_basic(type)), result);
    QCOMPARE(QDBusUtil::isValidBasicType(type), result);
}

void tst_QDBusType::isValidSingleSignature_data()
{
    addColumns();
    BOBUIest::newRow("empty") << "" << false;
    addSingleSignatures();
    addNakedDictEntry();
}

void tst_QDBusType::isValidSingleSignature()
{
    QFETCH(QString, data);
    QFETCH(bool, result);

    QCOMPARE(bool(q_dbus_signature_validate_single(data.toLatin1(), 0)), result);
    QCOMPARE(QDBusUtil::isValidSingleSignature(data), result);
}

void tst_QDBusType::isValidArray_data()
{
    addColumns();
    BOBUIest::newRow("empty") << "" << false;
    addSingleSignatures();
}

void tst_QDBusType::isValidArray()
{
    QFETCH(QString, data);
    QFETCH(bool, result);

    data.prepend(QLatin1Char('a'));
    QCOMPARE(bool(q_dbus_signature_validate_single(data.toLatin1(), 0)), result);
    QCOMPARE(QDBusUtil::isValidSingleSignature(data), result);

    data.prepend(QLatin1Char('a'));
    QCOMPARE(bool(q_dbus_signature_validate_single(data.toLatin1(), 0)), result);
    QCOMPARE(QDBusUtil::isValidSingleSignature(data), result);
}

void tst_QDBusType::isValidSignature_data()
{
    addColumns();
    BOBUIest::newRow("empty") << "" << true;
    addSingleSignatures();
    addNakedDictEntry();
}

void tst_QDBusType::isValidSignature()
{
    QFETCH(QString, data);
    QFETCH(bool, result);

    data.append(data);
    if (!data.isEmpty() && data.at(0).unicode()) {
        // libdbus-1 API can't deal with string containing NULs
        QCOMPARE(bool(q_dbus_signature_validate(data.toLatin1(), nullptr)), result);
    }
    QCOMPARE(QDBusUtil::isValidSignature(data), result);
}

BOBUIEST_MAIN(tst_QDBusType)

#include "tst_qdbustype.moc"
