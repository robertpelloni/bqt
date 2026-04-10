// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#ifdef Q_OS_UNIX
#include <locale.h>
#endif

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <BOBUIemporaryFile>
#include <QStringConverter>
#include <BOBUIcpSocket>
#include <BOBUIemporaryDir>
#include <BOBUIextStream>
#if BOBUI_CONFIG(process)
# include <QProcess>
#endif
#include "../../../network-settings.h"
#include <BobUITest/private/qemulationdetector_p.h>

using namespace BobUI::StringLiterals;

BOBUI_BEGIN_NAMESPACE
template<> struct QMetaTypeId<QIODevice::OpenModeFlag>
{ enum { Defined = 1 }; static inline int bobui_metatype_id() { return QMetaType::Int; } };
BOBUI_END_NAMESPACE

class tst_BOBUIextStream : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIextStream();

public slots:
    void initTestCase();
    void cleanup();
    void cleanupTestCase();

private slots:
    void getSetCheck();
    void construction();

    // lines
    void readLineFromDevice_data() { generateLineData(false); }
    void readLineFromDevice();
    void readLineFromString_data() { generateLineData(true); }
    void readLineFromString();
    void readLineFromTextDevice_data() { generateLineData(false); }
    void readLineFromTextDevice();
    void readLineUntilNull();
    void readLineMaxlen_data();
    void readLineMaxlen();
    void readLinesFromBufferCRCR();
    void readLineInto();

    // all
    void readAllFromDevice_data() { generateAllData(false); }
    void readAllFromDevice();
    void readAllFromString_data() { generateAllData(true); }
    void readAllFromString();
    void readLineFromStringThenChangeString();

    // device tests
    void setDevice();

    // char operators
    void QChar_operators_FromDevice_data() { generateOperatorCharData(false); }
    void QChar_operators_FromDevice();
    void char16_t_operators_FromDevice_data() { generateOperatorCharData(false); }
    void char16_t_operators_FromDevice();
    void char_operators_FromDevice_data() { generateOperatorCharData(false); }
    void char_operators_FromDevice();

    // natural number read operator
    void signedShort_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void signedShort_read_operator_FromDevice() { integral_read_operator_FromDevice<signed short>(); }
    void unsignedShort_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void unsignedShort_read_operator_FromDevice() { integral_read_operator_FromDevice<unsigned short>(); }
    void signedInt_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void signedInt_read_operator_FromDevice() { integral_read_operator_FromDevice<signed int>(); }
    void unsignedInt_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void unsignedInt_read_operator_FromDevice() { integral_read_operator_FromDevice<unsigned int>(); }
    void qlonglong_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void qlonglong_read_operator_FromDevice() { integral_read_operator_FromDevice<qlonglong>(); }
    void qulonglong_read_operator_FromDevice_data() { generateNaturalNumbersData(false); }
    void qulonglong_read_operator_FromDevice() { integral_read_operator_FromDevice<qulonglong>(); }

    // natural number write operator
    void signedShort_write_operator_ToDevice_data();
    void signedShort_write_operator_ToDevice() { integral_write_operator_ToDevice<signed short>(); }
    void unsignedShort_write_operator_ToDevice_data();
    void unsignedShort_write_operator_ToDevice() { integral_write_operator_ToDevice<unsigned short>(); }
    void signedInt_write_operator_ToDevice_data();
    void signedInt_write_operator_ToDevice() { integral_write_operator_ToDevice<signed int>(); }
    void unsignedInt_write_operator_ToDevice_data();
    void unsignedInt_write_operator_ToDevice() { integral_write_operator_ToDevice<unsigned int>(); }
    void qlonglong_write_operator_ToDevice_data();
    void qlonglong_write_operator_ToDevice() { integral_write_operator_ToDevice<qlonglong>(); }
    void qulonglong_write_operator_ToDevice_data();
    void qulonglong_write_operator_ToDevice() { integral_write_operator_ToDevice<qulonglong>(); }

    void int_read_with_locale_data();
    void int_read_with_locale();

    void int_write_with_locale_data();
    void int_write_with_locale();

    // real number read operator
    void float_read_operator_FromDevice_data() { generateRealNumbersData(false); }
    void float_read_operator_FromDevice() { real_read_operator_FromDevice<float>(); }
    void double_read_operator_FromDevice_data() { generateRealNumbersData(false); }
    void double_read_operator_FromDevice() { real_read_operator_FromDevice<double>(); }

    // real number write operator
    void float_write_operator_ToDevice_data() { generateRealNumbersDataWrite(); }
    void float_write_operator_ToDevice() { real_write_operator_ToDevice<float>(); }
    void double_write_operator_ToDevice_data() { generateRealNumbersDataWrite(); }
    void double_write_operator_ToDevice() { real_write_operator_ToDevice<double>(); }

    void double_write_with_flags_data();
    void double_write_with_flags();

    void double_write_with_precision_data();
    void double_write_with_precision();

    // text read operators
    void charPtr_read_operator_FromDevice_data() { generateStringData(false); }
    void charPtr_read_operator_FromDevice();
    void stringRef_read_operator_FromDevice_data() { generateStringData(false); }
    void stringRef_read_operator_FromDevice();
    void byteArray_read_operator_FromDevice_data() { generateStringData(false); }
    void byteArray_read_operator_FromDevice();

    // text write operators
    void string_write_operator_ToDevice_data();
    void string_write_operator_ToDevice();
    void latin1String_write_operator_ToDevice();
    void stringref_write_operator_ToDevice();
    void stringview_write_operator_ToDevice();

    // bool operator
    void stream_bool_operator_Test();

    // other
    void skipWhiteSpace_data();
    void skipWhiteSpace();
    void lineCount_data();
    void lineCount();
    void performance();
    void hexTest_data();
    void hexTest();
    void binTest_data();
    void binTest();
    void octTest_data();
    void octTest();
    void zeroTermination();
    void ws_manipulator();
    void stillOpenWhenAtEnd();
    void readNewlines_data();
    void readNewlines();
    void seek();
    void pos();
    void pos2();
    void pos3LargeFile();
    void readStdin();
    void readAllFromStdin();
    void readLineFromStdin();
    void read();
    void qbool();
    void forcePoint();
    void forceSign();
    void read0d0d0a();
    void numeralCase_data();
    void numeralCase();
    void nanInf();
    void utf8IncompleteAtBufferBoundary_data();
    void utf8IncompleteAtBufferBoundary();
    void writeSeekWriteNoBOM();

    // status
    void status_real_read_data();
    void status_real_read();
    void status_integer_read();
    void status_word_read();
    void status_write_error();

    // use case tests
    void useCase1();
    void useCase2();

    // manipulators
    void manipulators_data();
    void manipulators();

    // UTF-16 BOM (Byte Order Mark)
    void generateBOM();
    void readBomSeekBackReadBomAgain();

    // Regression tests for old bugs
    void alignAccountingStyle();
    void setEncoding();

    void textModeOnEmptyRead();

    void autodetectUnicode_data();
    void autodetectUnicode();

private:
    void generateLineData(bool for_QString) const;
    void generateAllData(bool for_QString) const;
    void generateOperatorCharData(bool for_QString) const;
    template <typename Whole> void integral_read_operator_FromDevice() const;
    void generateNaturalNumbersData(bool for_QString) const;
    template <typename Whole> void integral_write_operator_ToDevice() const;
    template <typename Real> void real_read_operator_FromDevice() const;
    void generateRealNumbersData(bool for_QString) const;
    void generateStringData(bool for_QString) const;
    template <typename Real> void real_write_operator_ToDevice() const;
    void generateRealNumbersDataWrite() const;

    BOBUIemporaryDir tempDir;
    QString testFileName;
#ifdef BUILTIN_TESTDATA
    QSharedPointer<BOBUIemporaryDir> m_dataDir;
#endif
    const QString m_rfc3261FilePath;
};

void runOnExit()
{
    QByteArray buffer;
    BOBUIextStream(&buffer) << "This will try to use QStringConverter::Utf8" << BobUI::endl;
}
Q_DESTRUCTOR_FUNCTION(runOnExit)

tst_BOBUIextStream::tst_BOBUIextStream()
    : tempDir(QDir::tempPath() + "/tst_bobuiextstream.XXXXXX")
    , m_rfc3261FilePath(QFINDTESTDATA("rfc3261.txt"))
{
}

void tst_BOBUIextStream::initTestCase()
{
    QVERIFY2(tempDir.isValid(), qPrintable(tempDir.errorString()));
    QVERIFY(!m_rfc3261FilePath.isEmpty());

    testFileName = tempDir.path() + "/testfile";

#ifdef BUILTIN_TESTDATA
    m_dataDir = QEXTRACTTESTDATA("/");
    QVERIFY2(QDir::setCurrent(m_dataDir->path()), qPrintable("Could not chdir to " + m_dataDir->path()));
#else
    // chdir into the testdata dir and refer to our helper apps with relative paths
    QString testdata_dir = QFileInfo(QFINDTESTDATA("stdinProcess")).absolutePath();
    QVERIFY2(QDir::setCurrent(testdata_dir), qPrintable("Could not chdir to " + testdata_dir));
#endif
}

// Testing get/set functions
void tst_BOBUIextStream::getSetCheck()
{
    // Initialize codecs
    BOBUIextStream obj1;
    // BOBUIextStream::encoding()
    // BOBUIextStream::setEncoding()
    obj1.setEncoding(QStringConverter::Utf32BE);
    QCOMPARE(QStringConverter::Utf32BE, obj1.encoding());
    obj1.setEncoding(QStringConverter::Utf8);
    QCOMPARE(QStringConverter::Utf8, obj1.encoding());

    // bool BOBUIextStream::autoDetectUnicode()
    // void BOBUIextStream::setAutoDetectUnicode(bool)
    obj1.setAutoDetectUnicode(false);
    QCOMPARE(false, obj1.autoDetectUnicode());
    obj1.setAutoDetectUnicode(true);
    QCOMPARE(true, obj1.autoDetectUnicode());

    // bool BOBUIextStream::generateByteOrderMark()
    // void BOBUIextStream::setGenerateByteOrderMark(bool)
    obj1.setGenerateByteOrderMark(false);
    QCOMPARE(false, obj1.generateByteOrderMark());
    obj1.setGenerateByteOrderMark(true);
    QCOMPARE(true, obj1.generateByteOrderMark());

    // QIODevice * BOBUIextStream::device()
    // void BOBUIextStream::setDevice(QIODevice *)
    QFile *var4 = new QFile;
    obj1.setDevice(var4);
    QCOMPARE(static_cast<QIODevice *>(var4), obj1.device());
    obj1.setDevice((QIODevice *)0);
    QCOMPARE((QIODevice *)0, obj1.device());
    delete var4;

    // Status BOBUIextStream::status()
    // void BOBUIextStream::setStatus(Status)
    obj1.setStatus(BOBUIextStream::Status(BOBUIextStream::Ok));
    QCOMPARE(BOBUIextStream::Status(BOBUIextStream::Ok), obj1.status());
    obj1.setStatus(BOBUIextStream::Status(BOBUIextStream::ReadPastEnd));
    QCOMPARE(BOBUIextStream::Status(BOBUIextStream::ReadPastEnd), obj1.status());
    obj1.resetStatus();
    obj1.setStatus(BOBUIextStream::Status(BOBUIextStream::ReadCorruptData));
    QCOMPARE(BOBUIextStream::Status(BOBUIextStream::ReadCorruptData), obj1.status());

    // FieldAlignment BOBUIextStream::fieldAlignment()
    // void BOBUIextStream::setFieldAlignment(FieldAlignment)
    obj1.setFieldAlignment(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignLeft));
    QCOMPARE(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignLeft), obj1.fieldAlignment());
    obj1.setFieldAlignment(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignRight));
    QCOMPARE(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignRight), obj1.fieldAlignment());
    obj1.setFieldAlignment(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignCenter));
    QCOMPARE(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignCenter), obj1.fieldAlignment());
    obj1.setFieldAlignment(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignAccountingStyle));
    QCOMPARE(BOBUIextStream::FieldAlignment(BOBUIextStream::AlignAccountingStyle), obj1.fieldAlignment());

    // QChar BOBUIextStream::padChar()
    // void BOBUIextStream::setPadChar(QChar)
    QChar var7 = 'Q';
    obj1.setPadChar(var7);
    QCOMPARE(var7, obj1.padChar());
    obj1.setPadChar(QChar());
    QCOMPARE(QChar(), obj1.padChar());

    // int BOBUIextStream::fieldWidth()
    // void BOBUIextStream::setFieldWidth(int)
    obj1.setFieldWidth(0);
    QCOMPARE(0, obj1.fieldWidth());
    obj1.setFieldWidth(INT_MIN);
    QCOMPARE(INT_MIN, obj1.fieldWidth());
    obj1.setFieldWidth(INT_MAX);
    QCOMPARE(INT_MAX, obj1.fieldWidth());

    // NumberFlags BOBUIextStream::numberFlags()
    // void BOBUIextStream::setNumberFlags(NumberFlags)
    obj1.setNumberFlags(BOBUIextStream::NumberFlags(BOBUIextStream::ShowBase));
    QCOMPARE(BOBUIextStream::NumberFlags(BOBUIextStream::ShowBase), obj1.numberFlags());
    obj1.setNumberFlags(BOBUIextStream::NumberFlags(BOBUIextStream::ForcePoint));
    QCOMPARE(BOBUIextStream::NumberFlags(BOBUIextStream::ForcePoint), obj1.numberFlags());
    obj1.setNumberFlags(BOBUIextStream::NumberFlags(BOBUIextStream::ForceSign));
    QCOMPARE(BOBUIextStream::NumberFlags(BOBUIextStream::ForceSign), obj1.numberFlags());
    obj1.setNumberFlags(BOBUIextStream::NumberFlags(BOBUIextStream::UppercaseBase));
    QCOMPARE(BOBUIextStream::NumberFlags(BOBUIextStream::UppercaseBase), obj1.numberFlags());
    obj1.setNumberFlags(BOBUIextStream::NumberFlags(BOBUIextStream::UppercaseDigits));
    QCOMPARE(BOBUIextStream::NumberFlags(BOBUIextStream::UppercaseDigits), obj1.numberFlags());

    // int BOBUIextStream::integerBase()
    // void BOBUIextStream::setIntegerBase(int)
    obj1.setIntegerBase(0);
    QCOMPARE(0, obj1.integerBase());
    obj1.setIntegerBase(INT_MIN);
    QCOMPARE(INT_MIN, obj1.integerBase());
    obj1.setIntegerBase(INT_MAX);
    QCOMPARE(INT_MAX, obj1.integerBase());

    // RealNumberNotation BOBUIextStream::realNumberNotation()
    // void BOBUIextStream::setRealNumberNotation(RealNumberNotation)
    obj1.setRealNumberNotation(BOBUIextStream::RealNumberNotation(BOBUIextStream::SmartNotation));
    QCOMPARE(BOBUIextStream::RealNumberNotation(BOBUIextStream::SmartNotation), obj1.realNumberNotation());
    obj1.setRealNumberNotation(BOBUIextStream::RealNumberNotation(BOBUIextStream::FixedNotation));
    QCOMPARE(BOBUIextStream::RealNumberNotation(BOBUIextStream::FixedNotation), obj1.realNumberNotation());
    obj1.setRealNumberNotation(BOBUIextStream::RealNumberNotation(BOBUIextStream::ScientificNotation));
    QCOMPARE(BOBUIextStream::RealNumberNotation(BOBUIextStream::ScientificNotation), obj1.realNumberNotation());

    // int BOBUIextStream::realNumberPrecision()
    // void BOBUIextStream::setRealNumberPrecision(int)
    obj1.setRealNumberPrecision(0);
    QCOMPARE(0, obj1.realNumberPrecision());
    obj1.setRealNumberPrecision(INT_MIN);
    QCOMPARE(6, obj1.realNumberPrecision()); // Setting a negative precision reverts it to the default value (6).
    obj1.setRealNumberPrecision(INT_MAX);
    QCOMPARE(INT_MAX, obj1.realNumberPrecision());
}

void tst_BOBUIextStream::cleanup()
{
    QCoreApplication::instance()->processEvents();
}

void tst_BOBUIextStream::cleanupTestCase()
{
#ifdef BUILTIN_TESTDATA
    QDir::setCurrent(QCoreApplication::applicationDirPath());
#endif
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::construction()
{
    BOBUIextStream stream;
    QCOMPARE(stream.encoding(), QStringConverter::Utf8);
    QCOMPARE(stream.device(), static_cast<QIODevice *>(0));
    QCOMPARE(stream.string(), static_cast<QString *>(0));

    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIextStream: No device");
    QVERIFY(stream.atEnd());

    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIextStream: No device");
    QCOMPARE(stream.readAll(), QString());

}

void tst_BOBUIextStream::generateLineData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QStringList>("lines");

    // latin-1
    BOBUIest::newRow("emptyer") << QByteArray() << QStringList();
    BOBUIest::newRow("lf") << QByteArray("\n") << (QStringList() << "");
    BOBUIest::newRow("crlf") << QByteArray("\r\n") << (QStringList() << "");
    BOBUIest::newRow("oneline/nothing") << QByteArray("ole") << (QStringList() << "ole");
    BOBUIest::newRow("oneline/lf") << QByteArray("ole\n") << (QStringList() << "ole");
    BOBUIest::newRow("oneline/crlf") << QByteArray("ole\r\n") << (QStringList() << "ole");
    BOBUIest::newRow("twolines/lf/lf") << QByteArray("ole\ndole\n") << (QStringList() << "ole" << "dole");
    BOBUIest::newRow("twolines/crlf/crlf") << QByteArray("ole\r\ndole\r\n") << (QStringList() << "ole" << "dole");
    BOBUIest::newRow("twolines/lf/crlf") << QByteArray("ole\ndole\r\n") << (QStringList() << "ole" << "dole");
    BOBUIest::newRow("twolines/lf/nothing") << QByteArray("ole\ndole") << (QStringList() << "ole" << "dole");
    BOBUIest::newRow("twolines/crlf/nothing") << QByteArray("ole\r\ndole") << (QStringList() << "ole" << "dole");
    BOBUIest::newRow("threelines/lf/lf/lf") << QByteArray("ole\ndole\ndoffen\n") << (QStringList() << "ole" << "dole" << "doffen");
    BOBUIest::newRow("threelines/crlf/crlf/crlf") << QByteArray("ole\r\ndole\r\ndoffen\r\n") << (QStringList() << "ole" << "dole" << "doffen");
    BOBUIest::newRow("threelines/crlf/crlf/nothing") << QByteArray("ole\r\ndole\r\ndoffen") << (QStringList() << "ole" << "dole" << "doffen");

    if (!for_QString) {
        // utf-8
        BOBUIest::newRow("utf8/twolines")
            << QByteArray("\xef\xbb\xbf"
                          "\x66\x67\x65\x0a"
                          "\x66\x67\x65\x0a", 11)
            << (QStringList() << "fge" << "fge");

        // utf-16
        // one line
        BOBUIest::newRow("utf16-BE/nothing")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65", 8) << (QStringList() << QLatin1String("\345ge"));
        BOBUIest::newRow("utf16-LE/nothing")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00", 8) << (QStringList() << QLatin1String("\345ge"));
        BOBUIest::newRow("utf16-BE/lf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 10) << (QStringList() << QLatin1String("\345ge"));
        BOBUIest::newRow("utf16-LE/lf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 10) << (QStringList() << QLatin1String("\345ge"));

        // two lines
        BOBUIest::newRow("utf16-BE/twolines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 18)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge"));
        BOBUIest::newRow("utf16-LE/twolines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 18)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge"));

        // three lines
        BOBUIest::newRow("utf16-BE/threelines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 26)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge") << QLatin1String("\345ge"));
        BOBUIest::newRow("utf16-LE/threelines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 26)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge") << QLatin1String("\345ge"));

        // utf-32
        BOBUIest::newRow("utf32-BE/twolines")
            << QByteArray("\x00\x00\xfe\xff"
                          "\x00\x00\x00\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a"
                          "\x00\x00\x00\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a", 36)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge"));
        BOBUIest::newRow("utf32-LE/twolines")
            << QByteArray("\xff\xfe\x00\x00"
                          "\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a\x00\x00\x00"
                          "\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a\x00\x00\x00", 36)
            << (QStringList() << QLatin1String("\345ge") << QLatin1String("\345ge"));
    }

    // partials
    BOBUIest::newRow("cr") << QByteArray("\r") << (QStringList() << "");
    BOBUIest::newRow("oneline/cr") << QByteArray("ole\r") << (QStringList() << "ole");
    if (!for_QString)
        BOBUIest::newRow("utf16-BE/cr") << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65\x00\x0d", 10) << (QStringList() << QLatin1String("\345ge"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineFromDevice()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    QFile::remove(testFileName);
    QFile file(testFileName);
    QVERIFY(file.open(QFile::ReadWrite));
    QCOMPARE(file.write(data), qlonglong(data.size()));
    QVERIFY(file.flush());
    file.seek(0);

    BOBUIextStream stream(&file);
    QStringList list;
    while (!stream.atEnd())
        list << stream.readLine();

    QCOMPARE(list, lines);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineMaxlen_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QStringList>("lines");

    BOBUIest::newRow("Hey")
        << QString("Hey")
        << (QStringList() << QString("Hey") << QString(""));
    BOBUIest::newRow("Hey\\n")
        << QString("Hey\n")
        << (QStringList() << QString("Hey") << QString(""));
    BOBUIest::newRow("HelloWorld")
        << QString("HelloWorld")
        << (QStringList() << QString("Hello") << QString("World"));
    BOBUIest::newRow("Helo\\nWorlds")
        << QString("Helo\nWorlds")
        << (QStringList() << QString("Helo") << QString("World"));
    BOBUIest::newRow("AAAAA etc.")
        << QString(16385, QLatin1Char('A'))
        << (QStringList() << QString("AAAAA") << QString("AAAAA"));
    BOBUIest::newRow("multibyte string")
        << QString::fromUtf8("\341\233\222\341\233\226\341\232\251\341\232\271\341\232\242\341\233\232\341\232\240\n")
        << (QStringList() << QString::fromUtf8("\341\233\222\341\233\226\341\232\251\341\232\271\341\232\242")
            << QString::fromUtf8("\341\233\232\341\232\240"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineMaxlen()
{
    QFETCH(QString, input);
    QFETCH(QStringList, lines);
    for (int i = 0; i < 2; ++i) {
        bool useDevice = (i == 1);
        BOBUIextStream stream;
        QFile::remove("testfile");
        QFile file("testfile");
        if (useDevice) {
            QVERIFY(file.open(QIODevice::ReadWrite));
            file.write(input.toUtf8());
            file.seek(0);
            stream.setDevice(&file);
            stream.setEncoding(QStringConverter::Utf8);
        } else {
            stream.setString(&input);
        }

        QStringList list;
        list << stream.readLine(5);
        list << stream.readLine(5);

        QCOMPARE(list, lines);
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLinesFromBufferCRCR()
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QByteArray data("0123456789\r\r\n");

    for (int i = 0; i < 10000; ++i)
        buffer.write(data);

    buffer.close();
    if (buffer.open(QIODevice::ReadOnly|QIODevice::Text)) {
        BOBUIextStream stream(&buffer);
        while (!stream.atEnd())
            QCOMPARE(stream.readLine(), QString("0123456789"));
    }
}

class ErrorDevice : public QIODevice
{
protected:
    qint64 readData(char *data, qint64 maxlen) override
    {
        Q_UNUSED(data);
        Q_UNUSED(maxlen);
        return -1;
    }

    qint64 writeData(const char *data, qint64 len) override
    {
        Q_UNUSED(data);
        Q_UNUSED(len);
        return -1;
    }
};

void tst_BOBUIextStream::readLineInto()
{
    QByteArray data = "1\n2\n3";

    BOBUIextStream ts(&data);
    QString line;

    ts.readLineInto(&line);
    QCOMPARE(line, QStringLiteral("1"));

    ts.readLineInto(nullptr, 0); // read the second line, but don't store it

    ts.readLineInto(&line);
    QCOMPARE(line, QStringLiteral("3"));

    QVERIFY(!ts.readLineInto(&line));
    QVERIFY(line.isEmpty());

    QFile file(m_rfc3261FilePath);
    QVERIFY(file.open(QFile::ReadOnly));

    ts.setDevice(&file);
    line.reserve(1);
    int maxLineCapacity = line.capacity();

    while (ts.readLineInto(&line)) {
        QVERIFY(line.capacity() >= maxLineCapacity);
        maxLineCapacity = line.capacity();
    }

    line = "Test string";
    ErrorDevice errorDevice;
    QVERIFY(errorDevice.open(QIODevice::ReadOnly));
    ts.setDevice(&errorDevice);

    QVERIFY(!ts.readLineInto(&line));
    QVERIFY(line.isEmpty());
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineFromString()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    QString dataString = data;

    BOBUIextStream stream(&dataString, QIODevice::ReadOnly);
    QStringList list;
    while (!stream.atEnd())
        list << stream.readLine();

    QCOMPARE(list, lines);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineFromStringThenChangeString()
{
    QString first = "First string";
    QString second = "Second string";

    BOBUIextStream stream(&first, QIODevice::ReadOnly);
    QString result = stream.readLine();
    QCOMPARE(first, result);

    stream.setString(&second, QIODevice::ReadOnly);
    result = stream.readLine();
    QCOMPARE(second, result);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::setDevice()
{
    // Check that the read buffer is reset after setting a new device
    QByteArray data1("Hello World");
    QByteArray data2("How are you");

    QBuffer bufferOld(&data1);
    bufferOld.open(QIODevice::ReadOnly);

    QBuffer bufferNew(&data2);
    bufferNew.open(QIODevice::ReadOnly);

    QString text;
    BOBUIextStream stream(&bufferOld);
    QVERIFY(stream >> text);
    QCOMPARE(text, QString("Hello"));

    stream.setDevice(&bufferNew);
    QVERIFY(stream >> text);
    QCOMPARE(text, QString("How"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineFromTextDevice()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    for (int i = 0; i < 8; ++i) {
        QBuffer buffer(&data);
        if (i < 4)
            QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));
        else
            QVERIFY(buffer.open(QIODevice::ReadOnly));

        BOBUIextStream stream(&buffer);
        QStringList list;
        while (!stream.atEnd()) {
            stream.pos(); // <- triggers side effects
            QString line;

            if (i & 1) {
                QChar c;
                while (!stream.atEnd()) {
                    if (stream >> c) {
                        if (c != QLatin1Char('\n') && c != QLatin1Char('\r'))
                            line += c;
                        if (c == QLatin1Char('\n'))
                            break;
                    }
                }
            } else {
                line = stream.readLine();
            }

            if ((i & 3) == 3 && !QString(BOBUIest::currentDataTag()).contains("utf16"))
                stream.seek(stream.pos());
            list << line;
        }
        QCOMPARE(list, lines);
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateAllData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QString>("output");

    // latin-1
    BOBUIest::newRow("empty") << QByteArray() << QString();
    BOBUIest::newRow("latin1-a") << QByteArray("a") << QString("a");
    BOBUIest::newRow("latin1-a\\r") << QByteArray("a\r") << QString("a\r");
    BOBUIest::newRow("latin1-a\\r\\n") << QByteArray("a\r\n") << QString("a\r\n");
    BOBUIest::newRow("latin1-a\\n") << QByteArray("a\n") << QString("a\n");

    // utf-16
    if (!for_QString) {
        // one line
        BOBUIest::newRow("utf16-BE/nothing")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65", 8) << QString::fromLatin1("\345ge");
        BOBUIest::newRow("utf16-LE/nothing")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00", 8) << QString::fromLatin1("\345ge");
        BOBUIest::newRow("utf16-BE/lf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 10) << QString::fromLatin1("\345ge\n");
        BOBUIest::newRow("utf16-LE/lf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 10) << QString::fromLatin1("\345ge\n");
        BOBUIest::newRow("utf16-BE/crlf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0d\x00\x0a", 12) << QString::fromLatin1("\345ge\r\n");
        BOBUIest::newRow("utf16-LE/crlf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0d\x00\x0a\x00", 12) << QString::fromLatin1("\345ge\r\n");

        // two lines
        BOBUIest::newRow("utf16-BE/twolines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 18)
            << QString::fromLatin1("\345ge\n\345ge\n");
        BOBUIest::newRow("utf16-LE/twolines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 18)
            << QString::fromLatin1("\345ge\n\345ge\n");

        // three lines
        BOBUIest::newRow("utf16-BE/threelines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 26)
            << QString::fromLatin1("\345ge\n\345ge\n\345ge\n");
        BOBUIest::newRow("utf16-LE/threelines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 26)
            << QString::fromLatin1("\345ge\n\345ge\n\345ge\n");
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineUntilNull()
{
    QFile file(m_rfc3261FilePath);
    QVERIFY(file.open(QFile::ReadOnly));

    BOBUIextStream stream(&file);
    for (int i = 0; i < 15066; ++i) {
        QString line = stream.readLine();
        QVERIFY(!line.isNull());
        QVERIFY(!line.isNull());
    }
    QVERIFY(!stream.readLine().isNull());
    QVERIFY(stream.readLine().isNull());
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readAllFromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);

    BOBUIextStream stream(&buffer);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readAllFromString()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QString str = input;

    BOBUIextStream stream(&str);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::skipWhiteSpace_data()
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QChar>("output");

    // latin1
    BOBUIest::newRow("empty") << QByteArray() << QChar('\0');
    BOBUIest::newRow(" one") << QByteArray(" one") << QChar('o');
    BOBUIest::newRow("\\none") << QByteArray("\none") << QChar('o');
    BOBUIest::newRow("\\n one") << QByteArray("\n one") << QChar('o');
    BOBUIest::newRow(" \\r\\n one") << QByteArray(" \r\n one") << QChar('o');

    // utf-16
    BOBUIest::newRow("utf16-BE (empty)") << QByteArray("\xfe\xff", 2) << QChar('\0');
    BOBUIest::newRow("utf16-BE ( one)") << QByteArray("\xfe\xff\x00 \x00o\x00n\x00e", 10) << QChar('o');
    BOBUIest::newRow("utf16-BE (\\none)") << QByteArray("\xfe\xff\x00\n\x00o\x00n\x00e", 10) << QChar('o');
    BOBUIest::newRow("utf16-BE (\\n one)") << QByteArray("\xfe\xff\x00\n\x00 \x00o\x00n\x00e", 12) << QChar('o');
    BOBUIest::newRow("utf16-BE ( \\r\\n one)") << QByteArray("\xfe\xff\x00 \x00\r\x00\n\x00 \x00o\x00n\x00e", 16) << QChar('o');

    BOBUIest::newRow("utf16-LE (empty)") << QByteArray("\xff\xfe", 2) << QChar('\0');
    BOBUIest::newRow("utf16-LE ( one)") << QByteArray("\xff\xfe \x00o\x00n\x00e\x00", 10) << QChar('o');
    BOBUIest::newRow("utf16-LE (\\none)") << QByteArray("\xff\xfe\n\x00o\x00n\x00e\x00", 10) << QChar('o');
    BOBUIest::newRow("utf16-LE (\\n one)") << QByteArray("\xff\xfe\n\x00 \x00o\x00n\x00e\x00", 12) << QChar('o');
    BOBUIest::newRow("utf16-LE ( \\r\\n one)") << QByteArray("\xff\xfe \x00\r\x00\n\x00 \x00o\x00n\x00e\x00", 16) << QChar('o');
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::skipWhiteSpace()
{
    QFETCH(QByteArray, input);
    QFETCH(QChar, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);

    BOBUIextStream stream(&buffer);
    stream.skipWhiteSpace();

    QChar tmp;
    stream >> tmp;

    QCOMPARE(tmp, output);

    QString str = input;
    BOBUIextStream stream2(&input);
    stream2.skipWhiteSpace();

    stream2 >> tmp;

    QCOMPARE(tmp, output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::lineCount_data()
{
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<int>("lineCount");

    BOBUIest::newRow("empty") << QByteArray() << 0;
    BOBUIest::newRow("oneline") << QByteArray("a\n") << 1;
    BOBUIest::newRow("twolines") << QByteArray("a\nb\n") << 2;
    BOBUIest::newRow("oneemptyline") << QByteArray("\n") << 1;
    BOBUIest::newRow("twoemptylines") << QByteArray("\n\n") << 2;
    BOBUIest::newRow("buffersize-1 line") << QByteArray(16382, '\n') << 16382;
    BOBUIest::newRow("buffersize line") << QByteArray(16383, '\n') << 16383;
    BOBUIest::newRow("buffersize+1 line") << QByteArray(16384, '\n') << 16384;
    BOBUIest::newRow("buffersize+2 line") << QByteArray(16385, '\n') << 16385;

    QFile file(m_rfc3261FilePath);
    QVERIFY(file.open(QFile::ReadOnly));
    BOBUIest::newRow("rfc3261") << file.readAll() << 15067;
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::lineCount()
{
    QFETCH(QByteArray, data);
    QFETCH(int, lineCount);

    QFile out("out.txt");
    QVERIFY(out.open(QFile::WriteOnly));

    BOBUIextStream lineReader(data);
    int lines = 0;
    while (!lineReader.atEnd()) {
        QString line = lineReader.readLine();
        out.write(line.toLatin1() + "\n");
        ++lines;
    }

    out.close();
    QCOMPARE(lines, lineCount);
}

// ------------------------------------------------------------------------------
struct CompareIndicesForArray
{
    int *array;
    CompareIndicesForArray(int *array) : array(array) {}
    bool operator() (const int i1, const int i2)
    {
        return array[i1] < array[i2];
    }
};

void tst_BOBUIextStream::performance()
{
    // Phase #1 - test speed of reading a huge text file with QFile.
    QElapsedTimer stopWatch;

    const int N = 3;
    const char * readMethods[N] = {
        "QFile::readLine()",
        "BOBUIextStream::readLine()",
        "BOBUIextStream::readLine(QString *)"
    };
    int elapsed[N] = {0, 0, 0};

        stopWatch.start();
        int nlines1 = 0;
        QFile file(m_rfc3261FilePath);
        QVERIFY(file.open(QFile::ReadOnly));

        while (!file.atEnd()) {
            ++nlines1;
            file.readLine();
        }

        elapsed[0] = stopWatch.restart();

        int nlines2 = 0;
        QFile file2(m_rfc3261FilePath);
        QVERIFY(file2.open(QFile::ReadOnly));

        BOBUIextStream stream(&file2);
        while (!stream.atEnd()) {
            ++nlines2;
            stream.readLine();
        }

        elapsed[1] = stopWatch.restart();

        int nlines3 = 0;
        QFile file3(m_rfc3261FilePath);
        QVERIFY(file3.open(QFile::ReadOnly));

        BOBUIextStream stream2(&file3);
        QString line;
        while (stream2.readLineInto(&line))
            ++nlines3;

        elapsed[2] = stopWatch.restart();

        QCOMPARE(nlines1, nlines2);
        QCOMPARE(nlines2, nlines3);

    for (int i = 0; i < N; i++) {
        qDebug("%s used %.3f seconds to read the file", readMethods[i],
               elapsed[i] / 1000.0);
    }

    int idx[N] = {0, 1, 2};
    std::sort(idx, idx + N, CompareIndicesForArray(elapsed));

    for (int i = 0; i < N-1; i++) {
        int i1 = idx[i];
        int i2 = idx[i+1];
        qDebug("Reading by %s is %.2fx faster than by %s",
               readMethods[i1],
               double(elapsed[i2]) / double(elapsed[i1]),
               readMethods[i2]);
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::hexTest_data()
{
    BOBUIest::addColumn<qlonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");

    BOBUIest::newRow("0") << Q_INT64_C(0) << QByteArray("0x0");
    BOBUIest::newRow("1") << Q_INT64_C(1) << QByteArray("0x1");
    BOBUIest::newRow("2") << Q_INT64_C(2) << QByteArray("0x2");
    BOBUIest::newRow("3") << Q_INT64_C(3) << QByteArray("0x3");
    BOBUIest::newRow("4") << Q_INT64_C(4) << QByteArray("0x4");
    BOBUIest::newRow("5") << Q_INT64_C(5) << QByteArray("0x5");
    BOBUIest::newRow("6") << Q_INT64_C(6) << QByteArray("0x6");
    BOBUIest::newRow("7") << Q_INT64_C(7) << QByteArray("0x7");
    BOBUIest::newRow("8") << Q_INT64_C(8) << QByteArray("0x8");
    BOBUIest::newRow("9") << Q_INT64_C(9) << QByteArray("0x9");
    BOBUIest::newRow("a") << Q_INT64_C(0xa) << QByteArray("0xa");
    BOBUIest::newRow("b") << Q_INT64_C(0xb) << QByteArray("0xb");
    BOBUIest::newRow("c") << Q_INT64_C(0xc) << QByteArray("0xc");
    BOBUIest::newRow("d") << Q_INT64_C(0xd) << QByteArray("0xd");
    BOBUIest::newRow("e") << Q_INT64_C(0xe) << QByteArray("0xe");
    BOBUIest::newRow("f") << Q_INT64_C(0xf) << QByteArray("0xf");
    BOBUIest::newRow("-1") << Q_INT64_C(-1) << QByteArray("-0x1");
    BOBUIest::newRow("0xffffffff") << Q_INT64_C(0xffffffff) << QByteArray("0xffffffff");
    BOBUIest::newRow("0xfffffffffffffffe") << Q_INT64_C(0xfffffffffffffffe) << QByteArray("-0x2");
    BOBUIest::newRow("0xffffffffffffffff") << Q_INT64_C(0xffffffffffffffff) << QByteArray("-0x1");
    BOBUIest::newRow("0x7fffffffffffffff") << Q_INT64_C(0x7fffffffffffffff) << QByteArray("0x7fffffffffffffff");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::hexTest()
{
    QFETCH(qlonglong, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    BOBUIextStream stream(&array);

    stream << BobUI::showbase << BobUI::hex << number;
    stream.flush();
    QCOMPARE(array, data);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::binTest_data()
{
    BOBUIest::addColumn<int>("number");
    BOBUIest::addColumn<QByteArray>("data");

    BOBUIest::newRow("0") << 0 << QByteArray("0b0");
    BOBUIest::newRow("1") << 1 << QByteArray("0b1");
    BOBUIest::newRow("2") << 2 << QByteArray("0b10");
    BOBUIest::newRow("5") << 5 << QByteArray("0b101");
    BOBUIest::newRow("-1") << -1 << QByteArray("-0b1");
    BOBUIest::newRow("11111111") << 0xff << QByteArray("0b11111111");
    BOBUIest::newRow("1111111111111111") << 0xffff << QByteArray("0b1111111111111111");
    BOBUIest::newRow("1111111011111110") << 0xfefe << QByteArray("0b1111111011111110");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::binTest()
{
    QFETCH(int, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    BOBUIextStream stream(&array);

    QVERIFY(stream << BobUI::showbase << BobUI::bin << number);
    stream.flush();
    QCOMPARE(array.constData(), data.constData());
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::octTest_data()
{
    BOBUIest::addColumn<int>("number");
    BOBUIest::addColumn<QByteArray>("data");

    BOBUIest::newRow("0") << 0 << QByteArray("00");
    BOBUIest::newRow("40") << 40 << QByteArray("050");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::octTest()
{
    QFETCH(int, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    BOBUIextStream stream(&array);

    QVERIFY(stream << BobUI::showbase << BobUI::oct << number);
    stream.flush();
    QCOMPARE(array, data);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::zeroTermination()
{
    BOBUIextStream stream;
    char c = '@';

    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIextStream: No device");
    QVERIFY(stream >> c);
    QCOMPARE(c, '\0');

    c = '@';

    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIextStream: No device");
    QVERIFY(stream >> &c);
    QCOMPARE(c, '\0');
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::ws_manipulator()
{
    {
        QString string = "a b c d";
        BOBUIextStream stream(&string);

        char a, b, c, d;
        QVERIFY(stream >> a >> b >> c >> d);
        QCOMPARE(a, 'a');
        QCOMPARE(b, ' ');
        QCOMPARE(c, 'b');
        QCOMPARE(d, ' ');
    }
    {
        QString string = "a b c d";
        BOBUIextStream stream(&string);

        char a, b, c, d;
        QVERIFY(stream >> a >> BobUI::ws >> b >> BobUI::ws >> c >> BobUI::ws >> d);
        QCOMPARE(a, 'a');
        QCOMPARE(b, 'b');
        QCOMPARE(c, 'c');
        QCOMPARE(d, 'd');
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::stillOpenWhenAtEnd()
{
    QFile file(QFINDTESTDATA("tst_bobuiextstream.cpp"));
    QVERIFY(file.open(QFile::ReadOnly));

    BOBUIextStream stream(&file);
    while (!stream.readLine().isNull()) {}
    QVERIFY(file.isOpen());

#ifdef BOBUI_TEST_SERVER
    if (!BobUINetworkSettings::verifyConnection(BobUINetworkSettings::imapServerName(), 143))
        QSKIP("No network test server available");
#else
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
#endif

    BOBUIcpSocket socket;
    socket.connectToHost(BobUINetworkSettings::imapServerName(), 143);
    BOBUIRY_VERIFY_WITH_TIMEOUT(socket.bytesAvailable() > 0, 20000);

    BOBUIextStream stream2(&socket);
    while (!stream2.readLine().isNull()) {}
    QVERIFY(socket.isOpen());
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readNewlines_data()
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QString>("output");

    BOBUIest::newRow("empty") << QByteArray() << QString();
    BOBUIest::newRow("\\r\\n") << QByteArray("\r\n") << QString("\n");
    BOBUIest::newRow("\\r\\r\\n") << QByteArray("\r\r\n") << QString("\n");
    BOBUIest::newRow("\\r\\n\\r\\n") << QByteArray("\r\n\r\n") << QString("\n\n");
    BOBUIest::newRow("\\n") << QByteArray("\n") << QString("\n");
    BOBUIest::newRow("\\n\\n") << QByteArray("\n\n") << QString("\n\n");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readNewlines()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly | QBuffer::Text);
    BOBUIextStream stream(&buffer);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::seek()
{
    QFile file(m_rfc3261FilePath);
    QVERIFY(file.open(QFile::ReadOnly));

    BOBUIextStream stream(&file);
    QString tmp;
    QVERIFY(stream >> tmp);
    QCOMPARE(tmp, QString::fromLatin1("Network"));

    // BOBUIextStream::seek(0) should both clear its internal read/write buffers
    // and seek the device.
    for (int i = 0; i < 4; ++i) {
        stream.seek(12 + i);
        QVERIFY(stream >> tmp);
        QCOMPARE(tmp, QString("Network").mid(i));
    }
    for (int i = 0; i < 4; ++i) {
        stream.seek(16 - i);
        QVERIFY(stream >> tmp);
        QCOMPARE(tmp, QString("Network").mid(4 - i));
    }
    stream.seek(139181);
    QVERIFY(stream >> tmp);
    QCOMPARE(tmp, QString("information"));
    stream.seek(388683);
    QVERIFY(stream >> tmp);
    QCOMPARE(tmp, QString("telephone"));

    // Also test this with a string
    QString words = QLatin1String("thisisa");
    BOBUIextStream stream2(&words, QIODevice::ReadOnly);
    QVERIFY(stream2 >> tmp);
    QCOMPARE(tmp, QString::fromLatin1("thisisa"));

    for (int i = 0; i < 4; ++i) {
        stream2.seek(i);
        QVERIFY(stream2 >> tmp);
        QCOMPARE(tmp, QString("thisisa").mid(i));
    }
    for (int i = 0; i < 4; ++i) {
        stream2.seek(4 - i);
        QVERIFY(stream2 >> tmp);
        QCOMPARE(tmp, QString("thisisa").mid(4 - i));
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::pos()
{
    {
        // Strings
        QString str("this is a test");
        BOBUIextStream stream(&str, QIODevice::ReadWrite);

        QCOMPARE(stream.pos(), qint64(0));
        for (int i = 0; i <= str.size(); ++i) {
            QVERIFY(stream.seek(i));
            QCOMPARE(stream.pos(), qint64(i));
        }
        for (int j = str.size(); j >= 0; --j) {
            QVERIFY(stream.seek(j));
            QCOMPARE(stream.pos(), qint64(j));
        }

        QVERIFY(stream.seek(0));

        QChar ch;
        QVERIFY(stream >> ch);
        QCOMPARE(ch, QChar('t'));

        QCOMPARE(stream.pos(), qint64(1));
        QVERIFY(stream.seek(1));
        QCOMPARE(stream.pos(), qint64(1));
        QVERIFY(stream.seek(0));

        QString strtmp;
        QVERIFY(stream >> strtmp);
        QCOMPARE(strtmp, QString("this"));

        QCOMPARE(stream.pos(), qint64(4));
        stream.seek(0);
        stream.seek(4);

        QVERIFY(stream >> ch);
        QCOMPARE(ch, QChar(' '));
        QCOMPARE(stream.pos(), qint64(5));

        stream.seek(10);
        QVERIFY(stream >> strtmp);
        QCOMPARE(strtmp, QString("test"));
        QCOMPARE(stream.pos(), qint64(14));
    }
    {
        // Latin1 device
        QFile file(m_rfc3261FilePath);
        QVERIFY(file.open(QIODevice::ReadOnly));

        BOBUIextStream stream(&file);

        QCOMPARE(stream.pos(), qint64(0));

        for (int i = 0; i <= file.size(); i += 7) {
            QVERIFY(stream.seek(i));
            QCOMPARE(stream.pos(), qint64(i));
        }
        for (int j = file.size(); j >= 0; j -= 7) {
            QVERIFY(stream.seek(j));
            QCOMPARE(stream.pos(), qint64(j));
        }

        stream.seek(0);

        QString strtmp;
        QVERIFY(stream >> strtmp);
        QCOMPARE(strtmp, QString("Network"));
        QCOMPARE(stream.pos(), qint64(19));

        stream.seek(2598);
        QCOMPARE(stream.pos(), qint64(2598));
        QVERIFY(stream >> strtmp);
        QCOMPARE(stream.pos(), qint64(2607));
        QCOMPARE(strtmp, QString("locations"));
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::pos2()
{
    QByteArray data("abcdef\r\nghijkl\r\n");
    QBuffer buffer(&data);
    QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));

    BOBUIextStream stream(&buffer);

    QChar ch;

    QCOMPARE(stream.pos(), qint64(0));
    QVERIFY(stream >> ch);
    QCOMPARE(ch, QChar('a'));
    QCOMPARE(stream.pos(), qint64(1));

    QString str;
    QVERIFY(stream >> str);
    QCOMPARE(str, QString("bcdef"));
    QCOMPARE(stream.pos(), qint64(6));

    QVERIFY(stream >> str);
    QCOMPARE(str, QString("ghijkl"));
    QCOMPARE(stream.pos(), qint64(14));

    // Seek back and try again
    stream.seek(1);
    QCOMPARE(stream.pos(), qint64(1));
    QVERIFY(stream >> str);
    QCOMPARE(str, QString("bcdef"));
    QCOMPARE(stream.pos(), qint64(6));

    stream.seek(6);
    QVERIFY(stream >> str);
    QCOMPARE(str, QString("ghijkl"));
    QCOMPARE(stream.pos(), qint64(14));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::pos3LargeFile()
{
    // NOTE: The unusual spacing is to ensure non-1-character whitespace.
    constexpr auto lineString = " 0  1  2\t3  4\t \t5  6  7  8   9 \n"_L1;
    {
        QFile file(testFileName);
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
        BOBUIextStream out( &file );
        // Approximately 5kb text file (more is too slow (BOBUIBUG-138435))
        const int NbLines = (5 * 1024) / lineString.size() + 1;
        for (int line = 0; line < NbLines; ++line)
            QVERIFY(out << lineString);
        // File is automatically flushed and closed on destruction.
    }
    QFile file(testFileName);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    BOBUIextStream in( &file );
    constexpr int testValues[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    qint64 expectedLineEnd = 0;
#ifdef Q_OS_WIN // CRLF platform
    constexpr int crlfAdjustment = 1;
#else
    constexpr int crlfAdjustment = 0;
#endif
    const auto expectedLineLength = lineString.size() + crlfAdjustment;
    QCOMPARE(in.pos(), 0);
    while (true) {
        for (size_t i = 0; i < std::size(testValues); ++i) {
            int value = -42;
            if (!(in >> value)) {
                // End case, i == 0 && eof reached.
                QCOMPARE(i, 0);
                QCOMPARE(in.status(), BOBUIextStream::ReadPastEnd);
                return;
            }
            QCOMPARE(value, testValues[i]);
        }
        expectedLineEnd += expectedLineLength;
        // Final space and newline are not consumed until next read.
        QCOMPARE(in.pos(), expectedLineEnd - 2 - crlfAdjustment);
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readStdin()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QProcess stdinProcess;
    stdinProcess.start("stdinProcess/stdinProcess");
    stdinProcess.setReadChannel(QProcess::StandardError);

    BOBUIextStream stream(&stdinProcess);
    QVERIFY(stream << "1" << BobUI::endl);
    QVERIFY(stream << "2" << BobUI::endl);
    QVERIFY(stream << "3" << BobUI::endl);

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));

    int a, b, c;
    QVERIFY(stream >> a >> b >> c);
    QCOMPARE(a, 1);
    QCOMPARE(b, 2);
    QCOMPARE(c, 3);
#endif
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readAllFromStdin()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QProcess stdinProcess;
    stdinProcess.start("readAllStdinProcess/readAllStdinProcess", {}, QIODevice::ReadWrite | QIODevice::Text);
    stdinProcess.setReadChannel(QProcess::StandardError);

    BOBUIextStream stream(&stdinProcess);
    stream.setEncoding(QStringConverter::Latin1);
    QVERIFY(stream << "hello world" << BobUI::flush);

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));
    QCOMPARE(stream.readAll(), QString::fromLatin1("hello world\n"));
#endif
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::readLineFromStdin()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QProcess stdinProcess;
    stdinProcess.start("readLineStdinProcess/readLineStdinProcess", {}, QIODevice::ReadWrite | QIODevice::Text);
    stdinProcess.setReadChannel(QProcess::StandardError);

    stdinProcess.write("abc\n");
    QVERIFY(stdinProcess.waitForReadyRead());
    QCOMPARE(stdinProcess.readAll(), "abc");

    stdinProcess.write("def\n");
    QVERIFY(stdinProcess.waitForReadyRead());
    QCOMPARE(stdinProcess.readAll(), "def");

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));
#endif
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::read()
{
    {
        QFile::remove("testfile");
        QFile file("testfile");
        QVERIFY(file.open(QFile::WriteOnly));
        file.write("4.15 abc ole");
        file.close();

        QVERIFY(file.open(QFile::ReadOnly));
        BOBUIextStream stream(&file);
        QCOMPARE(stream.read(0), QString(""));
        QCOMPARE(stream.read(4), QString("4.15"));
        QCOMPARE(stream.read(4), QString(" abc"));
        stream.seek(1);
        QCOMPARE(stream.read(4), QString(".15 "));
        stream.seek(1);
        QCOMPARE(stream.read(4), QString(".15 "));
        stream.seek(2);
        QCOMPARE(stream.read(4), QString("15 a"));
        // ### add tests for reading \r\n etc..
    }

    {
        // File larger than BOBUIEXTSTREAM_BUFFERSIZE
        QFile::remove("testfile");
        QFile file("testfile");
        QVERIFY(file.open(QFile::WriteOnly));
        for (int i = 0; i < 16384 / 8; ++i)
            file.write("01234567");
        file.write("0");
        file.close();

        QVERIFY(file.open(QFile::ReadOnly));
        BOBUIextStream stream(&file);
        QCOMPARE(stream.read(10), QString("0123456701"));
        QCOMPARE(stream.read(10), QString("2345670123"));
        QCOMPARE(stream.readAll().size(), 16385-20);
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::qbool()
{
    QString s;
    BOBUIextStream stream(&s);
    QVERIFY(stream << s.contains(QString("hei")));
    QCOMPARE(s, QString("0"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::forcePoint()
{
    QString str;
    BOBUIextStream stream(&str);
    QVERIFY(stream << BobUI::fixed << BobUI::forcepoint << 1.0 << ' ' << 1 << ' ' << 0
                   << ' ' << -1.0 << ' ' << -1);
    QCOMPARE(str, QString("1.000000 1 0 -1.000000 -1"));

    str.clear();
    stream.seek(0);
    QVERIFY(stream << BobUI::scientific << BobUI::forcepoint << 1.0 << ' ' << 1
                   << ' ' << 0 << ' ' << -1.0 << ' ' << -1);
    QCOMPARE(str, QString("1.000000e+00 1 0 -1.000000e+00 -1"));

    str.clear();
    stream.seek(0);
    stream.setRealNumberNotation(BOBUIextStream::SmartNotation);
    QVERIFY(stream << BobUI::forcepoint << 1.0 << ' ' << 1 << ' ' << 0 << ' ' << -1.0 << ' ' << -1);
    QCOMPARE(str, QString("1.00000 1 0 -1.00000 -1"));

}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::forceSign()
{
    QString str;
    BOBUIextStream stream(&str);
    QVERIFY(stream << BobUI::forcesign << 1.2 << ' ' << -1.2 << ' ' << 0);
    QCOMPARE(str, QString("+1.2 -1.2 +0"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::read0d0d0a()
{
    QFile file(QFINDTESTDATA("task113817.txt"));
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

    BOBUIextStream stream(&file);
    while (!stream.atEnd())
        stream.readLine();
}

// ------------------------------------------------------------------------------

Q_DECLARE_METATYPE(BOBUIextStreamFunction);

// Also tests that we can have namespaces that conflict with our BOBUIextStream constants.
namespace ws {
BOBUIextStream &noop(BOBUIextStream &s) { return s; }
}

void tst_BOBUIextStream::numeralCase_data()
{
    BOBUIextStreamFunction noop_ = ws::noop;
    BOBUIextStreamFunction bin  = BobUI::bin;
    BOBUIextStreamFunction oct  = BobUI::oct;
    BOBUIextStreamFunction hex  = BobUI::hex;
    BOBUIextStreamFunction base  = BobUI::showbase;
    BOBUIextStreamFunction ucb   = BobUI::uppercasebase;
    BOBUIextStreamFunction lcb   = BobUI::lowercasebase;
    BOBUIextStreamFunction ucd   = BobUI::uppercasedigits;
    BOBUIextStreamFunction lcd   = BobUI::lowercasedigits;

    BOBUIest::addColumn<BOBUIextStreamFunction>("func1");
    BOBUIest::addColumn<BOBUIextStreamFunction>("func2");
    BOBUIest::addColumn<BOBUIextStreamFunction>("func3");
    BOBUIest::addColumn<BOBUIextStreamFunction>("func4");
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<QString>("expected");
    BOBUIest::newRow("dec 1") << noop_ << noop_ << noop_ << noop_ << 31 << "31";
    BOBUIest::newRow("dec 2") << noop_ << base  << noop_ << noop_ << 31 << "31";

    BOBUIest::newRow("hex 1")  << hex  << noop_ << noop_ << noop_ << 31 << "1f";
    BOBUIest::newRow("hex 2")  << hex  << noop_ << noop_ << lcd   << 31 << "1f";
    BOBUIest::newRow("hex 3")  << hex  << noop_ << ucb   << noop_ << 31 << "1f";
    BOBUIest::newRow("hex 4")  << hex  << noop_ << noop_ << ucd   << 31 << "1F";
    BOBUIest::newRow("hex 5")  << hex  << noop_ << lcb   << ucd   << 31 << "1F";
    BOBUIest::newRow("hex 6")  << hex  << noop_ << ucb   << ucd   << 31 << "1F";
    BOBUIest::newRow("hex 7")  << hex  << base  << noop_ << noop_ << 31 << "0x1f";
    BOBUIest::newRow("hex 8")  << hex  << base  << lcb   << lcd   << 31 << "0x1f";
    BOBUIest::newRow("hex 9")  << hex  << base  << ucb   << noop_ << 31 << "0X1f";
    BOBUIest::newRow("hex 10") << hex  << base  << ucb   << lcd   << 31 << "0X1f";
    BOBUIest::newRow("hex 11") << hex  << base  << noop_ << ucd   << 31 << "0x1F";
    BOBUIest::newRow("hex 12") << hex  << base  << lcb   << ucd   << 31 << "0x1F";
    BOBUIest::newRow("hex 13") << hex  << base  << ucb   << ucd   << 31 << "0X1F";

    BOBUIest::newRow("bin 1") << bin  << noop_ << noop_ << noop_ << 31 << "11111";
    BOBUIest::newRow("bin 2") << bin  << base  << noop_ << noop_ << 31 << "0b11111";
    BOBUIest::newRow("bin 3") << bin  << base  << lcb   << noop_ << 31 << "0b11111";
    BOBUIest::newRow("bin 4") << bin  << base  << ucb   << noop_ << 31 << "0B11111";
    BOBUIest::newRow("bin 5") << bin  << base  << noop_ << ucd   << 31 << "0b11111";
    BOBUIest::newRow("bin 6") << bin  << base  << lcb   << ucd   << 31 << "0b11111";
    BOBUIest::newRow("bin 7") << bin  << base  << ucb   << ucd   << 31 << "0B11111";

    BOBUIest::newRow("oct 1") << oct  << noop_ << noop_ << noop_ << 31 << "37";
    BOBUIest::newRow("oct 2") << oct  << base  << noop_ << noop_ << 31 << "037";
}

void tst_BOBUIextStream::numeralCase()
{
    QFETCH(BOBUIextStreamFunction, func1);
    QFETCH(BOBUIextStreamFunction, func2);
    QFETCH(BOBUIextStreamFunction, func3);
    QFETCH(BOBUIextStreamFunction, func4);
    QFETCH(int, value);
    QFETCH(QString, expected);

    QString str;
    BOBUIextStream stream(&str);
    QVERIFY(stream << func1 << func2 << func3 << func4 << value);
    QCOMPARE(str, expected);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::nanInf()
{
    // Cannot use test data in this function, as comparing nans and infs isn't
    // well defined.
    QString str("nan NAN nAn +nan +NAN +nAn -nan -NAN -nAn"
                " inf INF iNf +inf +INF +iNf -inf -INF -iNf");

    BOBUIextStream stream(&str);

    double tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;
    QVERIFY(stream >> tmpD); QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;

    stream.seek(0);

    float tmpF = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0); tmpD = 0;
    QVERIFY(stream >> tmpF); QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0);

    QString s;
    BOBUIextStream out(&s);
    QVERIFY(out << qInf() << ' ' << -qInf() << ' ' << qQNaN()
                << BobUI::uppercasedigits << ' '
                << qInf() << ' ' << -qInf() << ' ' << qQNaN()
                << BobUI::flush);

    QCOMPARE(s, QString("inf -inf nan INF -INF NAN"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::utf8IncompleteAtBufferBoundary_data()
{
    BOBUIest::addColumn<bool>("useLocale");

    BOBUIest::newRow("utf8") << false;

    // is this locale UTF-8?
    if (QString(QChar::ReplacementCharacter).toLocal8Bit() == "\xef\xbf\xbd")
        BOBUIest::newRow("locale") << true;
}

void tst_BOBUIextStream::utf8IncompleteAtBufferBoundary()
{
    QFile::remove(testFileName);
    QFile data(testFileName);

    QString lineContents = QString::fromUtf8("\342\200\223" // U+2013 EN DASH
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223");

    QVERIFY(data.open(QFile::WriteOnly | QFile::Truncate));
    {
        BOBUIextStream out(&data);
        out.setEncoding(QStringConverter::Utf8);
        out.setFieldWidth(3);

        for (int i = 0; i < 1000; ++i) {
            QVERIFY(out << i << lineContents << BobUI::endl);
        }
    }
    data.close();

    QVERIFY(data.open(QFile::ReadOnly));
    BOBUIextStream in(&data);

    QFETCH(bool, useLocale);
    if (!useLocale)
        in.setEncoding(QStringConverter::Utf8);
    else
        in.setEncoding(QStringConverter::System);

    int i = 0;
    do {
        QString line = in.readLine().trimmed();
        ++i;
        QVERIFY2(line.endsWith(lineContents), QString("Line %1: %2").arg(i).arg(line).toLocal8Bit());
    } while (!in.atEnd());
}

// ------------------------------------------------------------------------------

// Make sure we don't write a BOM after seek()ing

void tst_BOBUIextStream::writeSeekWriteNoBOM()
{

    //First with the default codec (normally either latin-1 or UTF-8)

    QBuffer out;
    out.open(QIODevice::WriteOnly);
    BOBUIextStream stream(&out);

    int number = 0;
    QString sizeStr = QLatin1String("Size=")
        + QString::number(number).rightJustified(10, QLatin1Char('0'));
    QVERIFY(stream << sizeStr << BobUI::endl);
    QVERIFY(stream << "Version=" << QString::number(14) << BobUI::endl);
    QVERIFY(stream << "blah blah blah" << BobUI::endl);
    stream.flush();

    QCOMPARE(out.data(), "Size=0000000000\nVersion=14\nblah blah blah\n");

    // Now overwrite the size header item
    number = 42;
    stream.seek(0);
    sizeStr = QLatin1String("Size=")
        + QString::number(number).rightJustified(10, QLatin1Char('0'));
    QVERIFY(stream << sizeStr << BobUI::endl);
    stream.flush();

    // Check buffer is still OK
    QCOMPARE(out.data(), "Size=0000000042\nVersion=14\nblah blah blah\n");


    //Then UTF-16

    QBuffer out16;
    out16.open(QIODevice::WriteOnly);
    BOBUIextStream stream16(&out16);
    stream16.setEncoding(QStringConverter::Utf16);

    QVERIFY(stream16 << "one" << "two" << QLatin1String("three"));
    stream16.flush();

    // save that output
    const QByteArray first = out16.data();

    stream16.seek(0);
    QVERIFY(stream16 << "one");
    stream16.flush();

    QCOMPARE(out16.data(), first);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateOperatorCharData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QChar>("qchar_output");
    BOBUIest::addColumn<char>("char_output");
    BOBUIest::addColumn<QByteArray>("write_output");
    BOBUIest::addColumn<bool>("status");

    BOBUIest::newRow("empty") << QByteArray() << QChar('\0') << '\0'
                           << QByteArray("\0", 1) << false;
    BOBUIest::newRow("a") << QByteArray("a") << QChar('a') << 'a'
                       << QByteArray("a") << true;
    BOBUIest::newRow("\\na") << QByteArray("\na") << QChar('\n') << '\n'
                          << QByteArray("\n") << true;
    BOBUIest::newRow("\\0") << QByteArray("\0") << QChar('\0') << '\0'
                         << QByteArray("\0", 1) << false;
    BOBUIest::newRow("\\xff") << QByteArray("\xff") << QChar('\xff') << '\xff'
                           << QByteArray("\xff") << true;
    BOBUIest::newRow("\\xfe") << QByteArray("\xfe") << QChar('\xfe') << '\xfe'
                           << QByteArray("\xfe") << true;

    if (!for_QString) {
        BOBUIest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << QChar('\0') << '\0'
                                          << QByteArray("\0", 1) << false;
        BOBUIest::newRow("utf16-BE (a)") << QByteArray("\xff\xfe\x61\x00", 4) << QChar('a') << 'a'
                                      << QByteArray("a") << true;
        BOBUIest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << QChar('\0') << '\0'
                                          << QByteArray("\0", 1) << false;
        BOBUIest::newRow("utf16-LE (a)") << QByteArray("\xfe\xff\x00\x61", 4) << QChar('a') << 'a'
                                      << QByteArray("a") << true;
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::QChar_operators_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QChar, qchar_output);
    QFETCH(QByteArray, write_output);
    QFETCH(bool, status);

    QBuffer buf(&input);
    buf.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buf);
    stream.setEncoding(QStringConverter::Latin1);
    QChar tmp;
    QCOMPARE(static_cast<bool>(stream >> tmp), status);
    QCOMPARE(tmp, qchar_output);

    QBuffer writeBuf;
    writeBuf.open(QBuffer::WriteOnly);

    BOBUIextStream writeStream(&writeBuf);
    writeStream.setEncoding(QStringConverter::Latin1);
    QVERIFY(writeStream << qchar_output);
    writeStream.flush();

    QCOMPARE(writeBuf.data(), write_output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::char16_t_operators_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(const QChar, qchar_output);
    QFETCH(const QByteArray, write_output);
    QFETCH(bool, status);
    const char16_t char16_t_output = qchar_output.unicode();

    QBuffer buf(&input);
    buf.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buf);
    stream.setEncoding(QStringConverter::Latin1);
    char16_t tmp;
    QCOMPARE(static_cast<bool>(stream >> tmp), status);
    QCOMPARE(tmp, qchar_output);

    QBuffer writeBuf;
    writeBuf.open(QBuffer::WriteOnly);

    BOBUIextStream writeStream(&writeBuf);
    writeStream.setEncoding(QStringConverter::Latin1);
    QVERIFY(writeStream << char16_t_output);
    writeStream.flush();

    QCOMPARE(writeBuf.data(), write_output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::char_operators_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(char, char_output);
    QFETCH(QByteArray, write_output);
    QFETCH(bool, status);

    QBuffer buf(&input);
    buf.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buf);
    stream.setEncoding(QStringConverter::Latin1);
    char tmp;
    QCOMPARE(static_cast<bool>(stream >> tmp), status);
    QCOMPARE(tmp, char_output);

    QBuffer writeBuf;
    writeBuf.open(QBuffer::WriteOnly);

    BOBUIextStream writeStream(&writeBuf);
    writeStream.setEncoding(QStringConverter::Latin1);
    QVERIFY(writeStream << char_output);
    writeStream.flush();

    QCOMPARE(writeBuf.data(), write_output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateNaturalNumbersData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<qulonglong>("output");

    BOBUIest::newRow("empty") << QByteArray() << qulonglong(0);
    BOBUIest::newRow("a") << QByteArray("a") << qulonglong(0);
    BOBUIest::newRow(" ") << QByteArray(" ") << qulonglong(0);
    BOBUIest::newRow("0") << QByteArray("0") << qulonglong(0);
    BOBUIest::newRow("1") << QByteArray("1") << qulonglong(1);
    BOBUIest::newRow("12") << QByteArray("12") << qulonglong(12);
    BOBUIest::newRow("-12") << QByteArray("-12") << qulonglong(-12);
    BOBUIest::newRow("-0") << QByteArray("-0") << qulonglong(0);
    BOBUIest::newRow(" 1") << QByteArray(" 1") << qulonglong(1);
    BOBUIest::newRow(" \\r\\n\\r\\n123") << QByteArray(" \r\n\r\n123") << qulonglong(123);

    // bit boundary tests
    BOBUIest::newRow("127") << QByteArray("127") << qulonglong(127);
    BOBUIest::newRow("128") << QByteArray("128") << qulonglong(128);
    BOBUIest::newRow("129") << QByteArray("129") << qulonglong(129);
    BOBUIest::newRow("-127") << QByteArray("-127") << qulonglong(-127);
    BOBUIest::newRow("-128") << QByteArray("-128") << qulonglong(-128);
    BOBUIest::newRow("-129") << QByteArray("-129") << qulonglong(-129);
    BOBUIest::newRow("32767") << QByteArray("32767") << qulonglong(32767);
    BOBUIest::newRow("32768") << QByteArray("32768") << qulonglong(32768);
    BOBUIest::newRow("32769") << QByteArray("32769") << qulonglong(32769);
    BOBUIest::newRow("-32767") << QByteArray("-32767") << qulonglong(-32767);
    BOBUIest::newRow("-32768") << QByteArray("-32768") << qulonglong(-32768);
    BOBUIest::newRow("-32769") << QByteArray("-32769") << qulonglong(-32769);
    BOBUIest::newRow("65537") << QByteArray("65537") << qulonglong(65537);
    BOBUIest::newRow("65536") << QByteArray("65536") << qulonglong(65536);
    BOBUIest::newRow("65535") << QByteArray("65535") << qulonglong(65535);
    BOBUIest::newRow("-65537") << QByteArray("-65537") << qulonglong(-65537);
    BOBUIest::newRow("-65536") << QByteArray("-65536") << qulonglong(-65536);
    BOBUIest::newRow("-65535") << QByteArray("-65535") << qulonglong(-65535);
    BOBUIest::newRow("2147483646") << QByteArray("2147483646") << qulonglong(2147483646);
    BOBUIest::newRow("2147483647") << QByteArray("2147483647") << qulonglong(2147483647);
    BOBUIest::newRow("2147483648") << QByteArray("2147483648") << Q_UINT64_C(2147483648);
    BOBUIest::newRow("-2147483646") << QByteArray("-2147483646") << qulonglong(-2147483646);
    BOBUIest::newRow("-2147483647") << QByteArray("-2147483647") << qulonglong(-2147483647);
    BOBUIest::newRow("-2147483648") << QByteArray("-2147483648") << quint64(-2147483648LL);
    BOBUIest::newRow("4294967296") << QByteArray("4294967296") << Q_UINT64_C(4294967296);
    BOBUIest::newRow("4294967297") << QByteArray("4294967297") << Q_UINT64_C(4294967297);
    BOBUIest::newRow("4294967298") << QByteArray("4294967298") << Q_UINT64_C(4294967298);
    BOBUIest::newRow("-4294967296") << QByteArray("-4294967296") << quint64(-4294967296);
    BOBUIest::newRow("-4294967297") << QByteArray("-4294967297") << quint64(-4294967297);
    BOBUIest::newRow("-4294967298") << QByteArray("-4294967298") << quint64(-4294967298);
    BOBUIest::newRow("9223372036854775807") << QByteArray("9223372036854775807") << Q_UINT64_C(9223372036854775807);
    BOBUIest::newRow("9223372036854775808") << QByteArray("9223372036854775808") << Q_UINT64_C(9223372036854775808);
    BOBUIest::newRow("9223372036854775809") << QByteArray("9223372036854775809") << Q_UINT64_C(9223372036854775809);
    BOBUIest::newRow("18446744073709551615") << QByteArray("18446744073709551615") << Q_UINT64_C(18446744073709551615);
    BOBUIest::newRow("18446744073709551616") << QByteArray("18446744073709551616") << Q_UINT64_C(0);
    BOBUIest::newRow("18446744073709551617") << QByteArray("18446744073709551617") << Q_UINT64_C(1);
    // 18446744073709551617 bytes should be enough for anyone.... ;-)

    // hex tests
    BOBUIest::newRow("0x0") << QByteArray("0x0") << qulonglong(0);
    BOBUIest::newRow("0x") << QByteArray("0x") << qulonglong(0);
    BOBUIest::newRow("0x1") << QByteArray("0x1") << qulonglong(1);
    BOBUIest::newRow("0xf") << QByteArray("0xf") << qulonglong(15);
    BOBUIest::newRow("0xdeadbeef") << QByteArray("0xdeadbeef") << Q_UINT64_C(3735928559);
    BOBUIest::newRow("0XDEADBEEF") << QByteArray("0XDEADBEEF") << Q_UINT64_C(3735928559);
    BOBUIest::newRow("0xdeadbeefZzzzz") << QByteArray("0xdeadbeefZzzzz") << Q_UINT64_C(3735928559);
    BOBUIest::newRow("  0xdeadbeefZzzzz") << QByteArray("  0xdeadbeefZzzzz") << Q_UINT64_C(3735928559);

    // oct tests
    BOBUIest::newRow("00") << QByteArray("00") << qulonglong(0);
    BOBUIest::newRow("0141") << QByteArray("0141") << qulonglong(97);
    BOBUIest::newRow("01419999") << QByteArray("01419999") << qulonglong(97);
    BOBUIest::newRow("  01419999") << QByteArray("  01419999") << qulonglong(97);

    // bin tests
    BOBUIest::newRow("0b0") << QByteArray("0b0") << qulonglong(0);
    BOBUIest::newRow("0b1") << QByteArray("0b1") << qulonglong(1);
    BOBUIest::newRow("0b10") << QByteArray("0b10") << qulonglong(2);
    BOBUIest::newRow("0B10") << QByteArray("0B10") << qulonglong(2);
    BOBUIest::newRow("0b101010") << QByteArray("0b101010") << qulonglong(42);
    BOBUIest::newRow("0b1010102345") << QByteArray("0b1010102345") << qulonglong(42);
    BOBUIest::newRow("  0b1010102345") << QByteArray("  0b1010102345") << qulonglong(42);

    // utf-16 tests
    if (!for_QString) {
        BOBUIest::newRow("utf16-BE (empty)") << QByteArray("\xfe\xff", 2) << qulonglong(0);
        BOBUIest::newRow("utf16-BE (0xdeadbeef)")
            << QByteArray("\xfe\xff"
                          "\x00\x30\x00\x78\x00\x64\x00\x65\x00\x61\x00\x64\x00\x62\x00\x65\x00\x65\x00\x66", 22)
            << Q_UINT64_C(3735928559);
        BOBUIest::newRow("utf16-LE (empty)") << QByteArray("\xff\xfe", 2) << Q_UINT64_C(0);
        BOBUIest::newRow("utf16-LE (0xdeadbeef)")
            << QByteArray("\xff\xfe"
                          "\x30\x00\x78\x00\x64\x00\x65\x00\x61\x00\x64\x00\x62\x00\x65\x00\x65\x00\x66\x00", 22)
            << Q_UINT64_C(3735928559);
    }
}

// ------------------------------------------------------------------------------
template <typename Whole>
void tst_BOBUIextStream::integral_read_operator_FromDevice() const
{
    QFETCH(QByteArray, input);
    QFETCH(qulonglong, output);
    BOBUIextStream stream(&input);
    Whole sh;
    stream >> sh;
    QCOMPARE(sh, Whole(output));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateRealNumbersData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<double>("output");

    BOBUIest::newRow("empty") << QByteArray() << 0.0;
    BOBUIest::newRow("a") << QByteArray("a") << 0.0;
    BOBUIest::newRow("1.0") << QByteArray("1.0") << 1.0;
    BOBUIest::newRow(" 1") << QByteArray(" 1") << 1.0;
    BOBUIest::newRow(" \\r\\n1.2") << QByteArray(" \r\n1.2") << 1.2;
    BOBUIest::newRow("3.14") << QByteArray("3.14") << 3.14;
    BOBUIest::newRow("-3.14") << QByteArray("-3.14") << -3.14;
    BOBUIest::newRow(" -3.14") << QByteArray(" -3.14") << -3.14;
    BOBUIest::newRow("314e-02") << QByteArray("314e-02") << 3.14;
    BOBUIest::newRow("314E-02") << QByteArray("314E-02") << 3.14;
    BOBUIest::newRow("314e+02") << QByteArray("314e+02") << 31400.;
    BOBUIest::newRow("314E+02") << QByteArray("314E+02") << 31400.;

    // ### add numbers with exponents

    if (!for_QString) {
        BOBUIest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << 0.0;
        BOBUIest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << 0.0;
    }
}

// ------------------------------------------------------------------------------
template <typename Real>
void tst_BOBUIextStream::real_read_operator_FromDevice() const
{
    QFETCH(QByteArray, input);
    QFETCH(double, output);
    BOBUIextStream stream(&input);
    Real sh;
    stream >> sh;
    QCOMPARE(sh, Real(output));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateStringData(bool for_QString) const
{
    BOBUIest::addColumn<QByteArray>("input");
    BOBUIest::addColumn<QByteArray>("array_output");
    BOBUIest::addColumn<QString>("string_output");
    BOBUIest::addColumn<bool>("status");

    BOBUIest::newRow("empty") << QByteArray() << QByteArray() << QString() << false;
    BOBUIest::newRow("a") << QByteArray("a") << QByteArray("a") << QString("a") << true;
    BOBUIest::newRow("a b") << QByteArray("a b") << QByteArray("a") << QString("a") << true;
    BOBUIest::newRow(" a b") << QByteArray(" a b") << QByteArray("a") << QString("a") << true;
    BOBUIest::newRow("a1") << QByteArray("a1") << QByteArray("a1") << QString("a1") << true;
    BOBUIest::newRow("a1 b1") << QByteArray("a1 b1") << QByteArray("a1") << QString("a1") << true;
    BOBUIest::newRow(" a1 b1") << QByteArray(" a1 b1") << QByteArray("a1") << QString("a1") << true;
    BOBUIest::newRow("\\n\\n\\nole i dole\\n") << QByteArray("\n\n\nole i dole\n")
                                            << QByteArray("ole") << QString("ole") << true;

    if (!for_QString) {
        BOBUIest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << QByteArray()
                                          << QString() << false;
        BOBUIest::newRow("utf16-BE (corrupt)") << QByteArray("\xff", 1) << QByteArray("\xc3\xbf")
                                            << QString::fromUtf8("\xc3\xbf") << true;
        BOBUIest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << QByteArray()
                                          << QString() << false;
        BOBUIest::newRow("utf16-LE (corrupt)") << QByteArray("\xfe", 1) << QByteArray("\xc3\xbe")
                                            << QString::fromUtf8("\xc3\xbe") << true;
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::charPtr_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, array_output);
    QFETCH(bool, status);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buffer);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    char buf[1024];
    QCOMPARE(static_cast<bool>(stream >> buf), status);

    QCOMPARE((const char *)buf, array_output.constData());
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::stringRef_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, string_output);
    QFETCH(bool, status);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buffer);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    QString tmp;
    QCOMPARE(static_cast<bool>(stream >> tmp), status);

    QCOMPARE(tmp, string_output);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::byteArray_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, array_output);
    QFETCH(bool, status);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    BOBUIextStream stream(&buffer);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    QByteArray array;
    QCOMPARE(static_cast<bool>(stream >> array), status);

    QCOMPARE(array, array_output);
}

// ------------------------------------------------------------------------------
template <typename Whole>
void tst_BOBUIextStream::integral_write_operator_ToDevice() const
{
    QFETCH(qulonglong, number);
    QFETCH(QByteArray, data);
    QFETCH(QByteArray, dataWithSeparators);

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    BOBUIextStream stream(&buffer);
    stream.setLocale(QLocale::c());
    QVERIFY(stream << Whole(number));
    stream.flush();
    QCOMPARE(buffer.data(), data);

    QLocale locale("en-US");
    buffer.reset(); buffer.buffer().clear();
    stream.setLocale(locale);
    QVERIFY(stream << Whole(number));
    stream.flush();
    QCOMPARE(buffer.data(), dataWithSeparators);

    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    buffer.reset(); buffer.buffer().clear();
    stream.setLocale(locale);
    QVERIFY(stream << Whole(number));
    stream.flush();
    QCOMPARE(buffer.data(), data);

    locale = QLocale("de-DE");
    buffer.reset(); buffer.buffer().clear();
    stream.setLocale(locale);
    QVERIFY(stream << Whole(number));
    stream.flush();
    QCOMPARE(buffer.data(), dataWithSeparators.replace(',', '.'));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::signedShort_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("-32768") << QByteArray("-32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("-32767") << QByteArray("-32,767");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-32768") << quint64(-32768) << QByteArray("-32768") << QByteArray("-32,768");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::unsignedShort_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768") << QByteArray("32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769") << QByteArray("32,769");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("1") << QByteArray("1");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::signedInt_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768") << QByteArray("32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769") << QByteArray("32,769");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536") << QByteArray("65,536");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537") << QByteArray("65,537");
    BOBUIest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647") << QByteArray("2,147,483,647");
    BOBUIest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("-2147483648") << QByteArray("-2,147,483,648");
    BOBUIest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("-2147483647") << QByteArray("-2,147,483,647");
    BOBUIest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-2147483648") << quint64(-2147483648) << QByteArray("-2147483648") << QByteArray("-2,147,483,648");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::unsignedInt_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("4294967295") << QByteArray("4,294,967,295");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768") << QByteArray("32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769") << QByteArray("32,769");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536") << QByteArray("65,536");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537") << QByteArray("65,537");
    BOBUIest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647") << QByteArray("2,147,483,647");
    BOBUIest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648") << QByteArray("2,147,483,648");
    BOBUIest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649") << QByteArray("2,147,483,649");
    BOBUIest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295") << QByteArray("4,294,967,295");
    BOBUIest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("1") << QByteArray("1");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::qlonglong_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768") << QByteArray("32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769") << QByteArray("32,769");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536") << QByteArray("65,536");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537") << QByteArray("65,537");
    BOBUIest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647") << QByteArray("2,147,483,647");
    BOBUIest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648") << QByteArray("2,147,483,648");
    BOBUIest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649") << QByteArray("2,147,483,649");
    BOBUIest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295") << QByteArray("4,294,967,295");
    BOBUIest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("4294967296") << QByteArray("4,294,967,296");
    BOBUIest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("4294967297") << QByteArray("4,294,967,297");
    BOBUIest::newRow("9223372036854775807") << Q_UINT64_C(9223372036854775807) << QByteArray("9223372036854775807") << QByteArray("9,223,372,036,854,775,807");
    BOBUIest::newRow("9223372036854775808") << Q_UINT64_C(9223372036854775808) << QByteArray("-9223372036854775808") << QByteArray("-9,223,372,036,854,775,808");
    BOBUIest::newRow("9223372036854775809") << Q_UINT64_C(9223372036854775809) << QByteArray("-9223372036854775807") << QByteArray("-9,223,372,036,854,775,807");
    BOBUIest::newRow("18446744073709551615") << Q_UINT64_C(18446744073709551615) << QByteArray("-1") << QByteArray("-1");
    BOBUIest::newRow("-9223372036854775808") << quint64(Q_INT64_C(-9223372036854775807) - 1) << QByteArray("-9223372036854775808") << QByteArray("-9,223,372,036,854,775,808");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::qulonglong_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<qulonglong>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("1") << Q_UINT64_C(1) << QByteArray("1") << QByteArray("1");
    BOBUIest::newRow("-1") << quint64(-1) << QByteArray("18446744073709551615") << QByteArray("18,446,744,073,709,551,615");
    BOBUIest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767") << QByteArray("32,767");
    BOBUIest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768") << QByteArray("32,768");
    BOBUIest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769") << QByteArray("32,769");
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535") << QByteArray("65,535");
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536") << QByteArray("65,536");
    BOBUIest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537") << QByteArray("65,537");
    BOBUIest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647") << QByteArray("2,147,483,647");
    BOBUIest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648") << QByteArray("2,147,483,648");
    BOBUIest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649") << QByteArray("2,147,483,649");
    BOBUIest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295") << QByteArray("4,294,967,295");
    BOBUIest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("4294967296") << QByteArray("4,294,967,296");
    BOBUIest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("4294967297") << QByteArray("4,294,967,297");
    BOBUIest::newRow("9223372036854775807") << Q_UINT64_C(9223372036854775807) << QByteArray("9223372036854775807") << QByteArray("9,223,372,036,854,775,807");
    BOBUIest::newRow("9223372036854775808") << Q_UINT64_C(9223372036854775808) << QByteArray("9223372036854775808") << QByteArray("9,223,372,036,854,775,808");
    BOBUIest::newRow("9223372036854775809") << Q_UINT64_C(9223372036854775809) << QByteArray("9223372036854775809") << QByteArray("9,223,372,036,854,775,809");
    BOBUIest::newRow("18446744073709551615") << Q_UINT64_C(18446744073709551615) << QByteArray("18446744073709551615") << QByteArray("18,446,744,073,709,551,615");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::generateRealNumbersDataWrite() const
{
    BOBUIest::addColumn<double>("number");
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<QByteArray>("dataWithSeparators");

    BOBUIest::newRow("0") << 0.0 << QByteArray("0") << QByteArray("0");
    BOBUIest::newRow("3.14") << 3.14 << QByteArray("3.14") << QByteArray("3.14");
    BOBUIest::newRow("-3.14") << -3.14 << QByteArray("-3.14") << QByteArray("-3.14");
    BOBUIest::newRow("1.2e+10") << 1.2e+10 << QByteArray("1.2e+10") << QByteArray("1.2e+10");
    BOBUIest::newRow("-1.2e+10") << -1.2e+10 << QByteArray("-1.2e+10") << QByteArray("-1.2e+10");
    BOBUIest::newRow("12345") << 12345. << QByteArray("12345") << QByteArray("12,345");
}

// ------------------------------------------------------------------------------
template <typename Real>
void tst_BOBUIextStream::real_write_operator_ToDevice() const
{
    QFETCH(double, number);
    QFETCH(QByteArray, data);
    QFETCH(QByteArray, dataWithSeparators);

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    BOBUIextStream stream(&buffer);
    stream.setLocale(QLocale::c());
    Real f = Real(number);
    QVERIFY(stream << f);
    stream.flush();
    QCOMPARE(buffer.data(), data);

    buffer.reset();
    stream.setLocale(QLocale("en-US"));
    QVERIFY(stream << f);
    stream.flush();
    QCOMPARE(buffer.data(), dataWithSeparators);
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::string_write_operator_ToDevice_data()
{
    BOBUIest::addColumn<QByteArray>("bytedata");
    BOBUIest::addColumn<QString>("stringdata");
    BOBUIest::addColumn<QByteArray>("result");

    BOBUIest::newRow("empty") << QByteArray("", 1) << QString(1, '\0') << QByteArray("", 1);
    BOBUIest::newRow("a") << QByteArray("a") << QString("a") << QByteArray("a");
    BOBUIest::newRow("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon")
        << QString("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon");

    // ### get the utf16-be test on its legs.
    /*
    BOBUIest::newRow("utf16-BE (a cow jumped over the moon)")
        << QByteArray("\xff\xfe\x00\x61\x00\x20\x00\x63\x00\x6f\x00\x77\x00\x20\x00\x6a\x00\x75\x00\x6d\x00\x70\x00\x65\x00\x64\x00\x20\x00\x6f\x00\x76\x00\x65\x00\x72\x00\x20\x00\x74\x00\x68\x00\x65\x00\x20\x00\x6d\x00\x6f\x00\x6f\x00\x6e\x00\x0a", 56)
        << QString("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon");
    */
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::string_write_operator_ToDevice()
{
    QFETCH(QByteArray, bytedata);
    QFETCH(QString, stringdata);
    QFETCH(QByteArray, result);

    {
        // char*
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        BOBUIextStream stream(&buf);
        stream.setEncoding(QStringConverter::Latin1);
        stream.setAutoDetectUnicode(true);

        QVERIFY(stream << bytedata.constData());
        stream.flush();
        // Size information is discarded, so "empty" doesn't get its (size 1)
        // null byte transmitted, so only compare to constData(), ignoring size.
        QCOMPARE(buf.data().constData(), result.constData());
    }
    {
        // QByteArray
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        BOBUIextStream stream(&buf);
        stream.setEncoding(QStringConverter::Latin1);
        stream.setAutoDetectUnicode(true);

        QVERIFY(stream << bytedata);
        stream.flush();
        QCOMPARE(buf.data(), result);
    }
    {
        // QString
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        BOBUIextStream stream(&buf);
        stream.setEncoding(QStringConverter::Latin1);
        stream.setAutoDetectUnicode(true);

        QVERIFY(stream << stringdata);
        stream.flush();
        QCOMPARE(buf.data(), result);
    }
}

void tst_BOBUIextStream::latin1String_write_operator_ToDevice()
{
    QBuffer buf;
    buf.open(QBuffer::WriteOnly);
    BOBUIextStream stream(&buf);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    QVERIFY(stream << QLatin1String("No explicit length"));
    QVERIFY(stream << QLatin1String("Explicit length - ignore this part", 15));
    stream.flush();
    QCOMPARE(buf.data(), "No explicit lengthExplicit length");
}

void tst_BOBUIextStream::stringref_write_operator_ToDevice()
{
    QBuffer buf;
    buf.open(QBuffer::WriteOnly);
    BOBUIextStream stream(&buf);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    const QStringView expected = u"No explicit lengthExplicit length";

    QVERIFY(stream << expected.left(18));
    QVERIFY(stream << expected.mid(18));
    stream.flush();
    QCOMPARE(buf.data(), "No explicit lengthExplicit length");
}

void tst_BOBUIextStream::stringview_write_operator_ToDevice()
{
    QBuffer buf;
    buf.open(QBuffer::WriteOnly);
    BOBUIextStream stream(&buf);
    const QStringView expected = u"expectedStringView";
    QVERIFY(stream << expected);
    stream.flush();
    QCOMPARE(buf.data(), "expectedStringView");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::stream_bool_operator_Test()
{
    const QStringView expected = u"expectedStringView";
    QString line("exp");
    BOBUIextStream stream(&line);
    QChar ch1, ch2, ch3;

    QVERIFY(stream >> ch1);
    QCOMPARE(ch1, 'e');
    QVERIFY(stream >> ch2);
    QCOMPARE(ch2, 'x');
    QVERIFY(stream >> ch3);
    QCOMPARE(ch3, 'p');

    QVERIFY(stream << "ectedStringView");
    stream.flush();
    QCOMPARE(*stream.string(), expected);

    BOBUIextStream emptyStream("");
    QVERIFY(!(emptyStream >> ch1));
    QVERIFY(!(emptyStream << "Hello"));

    BOBUIextStream textStream("1 2 3 error");
    int n;
    for (int i = 0; i < 3; ++i) {
        QVERIFY(textStream >> n);
        QCOMPARE_EQ(n, i + 1);
    }
    QVERIFY(!textStream.atEnd());
    QVERIFY(!(textStream >> n));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::useCase1()
{
    QFile::remove("testfile");
    QFile file("testfile");
    QVERIFY(file.open(QFile::ReadWrite));

    {
        BOBUIextStream stream(&file);
        stream.setEncoding(QStringConverter::Latin1);
        stream.setAutoDetectUnicode(true);

        stream << 4.15 << ' ' << QByteArray("abc") << ' ' << QString("ole");
    }

    file.seek(0);
    QCOMPARE(file.readAll(), "4.15 abc ole");
    file.seek(0);

    {
        double d;
        QByteArray a;
        QString s;
        BOBUIextStream stream(&file);
        stream.setEncoding(QStringConverter::Latin1);
        stream.setAutoDetectUnicode(true);

        stream >> d;
        stream >> a;
        stream >> s;

        QCOMPARE(d, 4.15);
        QCOMPARE(a, "abc");
        QCOMPARE(s, QString("ole"));
    }
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::useCase2()
{
    QFile::remove("testfile");
    QFile file("testfile");
    QVERIFY(file.open(QFile::ReadWrite));

    BOBUIextStream stream(&file);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    stream << 4.15 << ' ' << QByteArray("abc") << ' ' << QString("ole");

    file.close();
    QVERIFY(file.open(QFile::ReadWrite));

    QCOMPARE(file.readAll(), "4.15 abc ole");

    file.close();
    QVERIFY(file.open(QFile::ReadWrite));

    double d;
    QByteArray a;
    QString s;
    BOBUIextStream stream2(&file);
    stream2.setEncoding(QStringConverter::Latin1);
    stream2.setAutoDetectUnicode(true);

    QVERIFY(stream2 >> d);
    QVERIFY(stream2 >> a);
    QVERIFY(stream2 >> s);

    QCOMPARE(d, 4.15);
    QCOMPARE(a, "abc");
    QCOMPARE(s, "ole");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::manipulators_data()
{
    BOBUIest::addColumn<int>("base");
    BOBUIest::addColumn<BOBUIextStream::FieldAlignment>("alignFlag");
    BOBUIest::addColumn<BOBUIextStream::NumberFlags>("numberFlag");
    BOBUIest::addColumn<int>("width");
    BOBUIest::addColumn<double>("realNumber");
    BOBUIest::addColumn<qlonglong>("intNumber");
    BOBUIest::addColumn<QString>("textData");
    BOBUIest::addColumn<QByteArray>("result");

    BOBUIest::newRow("no flags")
        << 10 << BOBUIextStream::AlignLeft << BOBUIextStream::NumberFlags{}
        << 0  << 5.0 << 5LL << QString("five") << QByteArray("55five");
    BOBUIest::newRow("rightadjust")
        << 10 << BOBUIextStream::AlignRight << BOBUIextStream::NumberFlags{}
        << 10 << 5.0 << 5LL << QString("five") << QByteArray("         5         5      five");
    BOBUIest::newRow("leftadjust")
        << 10 << BOBUIextStream::AlignLeft << BOBUIextStream::NumberFlags{}
        << 10 << 5.0 << 5LL << QString("five") << QByteArray("5         5         five      ");
    BOBUIest::newRow("showpos-wide")
        << 10 << BOBUIextStream::AlignRight << BOBUIextStream::NumberFlags{BOBUIextStream::ForceSign}
        << 10 << 5.0 << 5LL << QString("five") << QByteArray("        +5        +5      five");
    BOBUIest::newRow("showpos-pi")
        << 10 << BOBUIextStream::AlignRight << BOBUIextStream::NumberFlags{BOBUIextStream::ForceSign}
        << 5 << 3.14 << -5LL << QString("five") << QByteArray("+3.14   -5 five");
    BOBUIest::newRow("min-value")
        << 10 << BOBUIextStream::AlignRight << BOBUIextStream::NumberFlags{} << 5
        << 3.14 << (std::numeric_limits<qlonglong>::min)()
        << QString("five") << QByteArray(" 3.14-9223372036854775808 five");
    BOBUIest::newRow("hex-lower")
        << 16 << BOBUIextStream::AlignRight << BOBUIextStream::NumberFlags{BOBUIextStream::ShowBase}
        << 5 << 3.14 << -5LL << QString("five") << QByteArray(" 3.14 -0x5 five");
    BOBUIest::newRow("hex-upper")
        << 16 << BOBUIextStream::AlignRight
        << (BOBUIextStream::ShowBase | BOBUIextStream::UppercaseBase)
        << 5 << 3.14 << -5LL << QString("five") << QByteArray(" 3.14 -0X5 five");
    BOBUIest::newRow("hex-negative")
        << 16 << BOBUIextStream::AlignRight << (BOBUIextStream::ShowBase | BOBUIextStream::ForceSign)
        << 5 << 3.14 << -5LL << QString("five") << QByteArray("+3.14 -0x5 five");
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::manipulators()
{
    QFETCH(int, base);
    QFETCH(BOBUIextStream::FieldAlignment, alignFlag);
    QFETCH(BOBUIextStream::NumberFlags, numberFlag);
    QFETCH(int, width);
    QFETCH(double, realNumber);
    QFETCH(qlonglong, intNumber);
    QFETCH(QString, textData);
    QFETCH(QByteArray, result);

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);

    BOBUIextStream stream(&buffer);
    stream.setEncoding(QStringConverter::Latin1);
    stream.setAutoDetectUnicode(true);

    stream.setIntegerBase(base);
    stream.setFieldAlignment(alignFlag);
    stream.setNumberFlags(numberFlag);
    stream.setFieldWidth(width);
    QVERIFY(stream << realNumber);
    QVERIFY(stream << intNumber);
    QVERIFY(stream << textData);
    stream.flush();

    QCOMPARE(buffer.data(), result);
}

void tst_BOBUIextStream::generateBOM()
{
    QFile::remove("bom.txt");
    {
        QFile file("bom.txt");
        QVERIFY(file.open(QFile::ReadWrite | QFile::Truncate));

        BOBUIextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf16LE);
        stream << "Hello" << BobUI::endl;

        file.close();
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("\x48\x00\x65\00\x6c\00\x6c\00\x6f\x00\x0a\x00", 12));
    }

    QFile::remove("bom.txt");
    {
        QFile file("bom.txt");
        QVERIFY(file.open(QFile::ReadWrite | QFile::Truncate));

        BOBUIextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf16LE);
        QVERIFY(stream << BobUI::bom << "Hello" << BobUI::endl);

        file.close();
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("\xff\xfe\x48\x00\x65\00\x6c\00\x6c\00\x6f\x00\x0a\x00", 14));
    }
}

void tst_BOBUIextStream::readBomSeekBackReadBomAgain()
{
    QFile::remove("utf8bom");
    QFile file("utf8bom");
    QVERIFY(file.open(QFile::ReadWrite));
    file.write("\xef\xbb\xbf""Andreas");
    file.seek(0);
    QCOMPARE(file.pos(), qint64(0));

    BOBUIextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    QString Andreas;
    QVERIFY(stream >> Andreas);
    QCOMPARE(Andreas, QString("Andreas"));
    stream.seek(0);
    QVERIFY(stream >> Andreas);
    QCOMPARE(Andreas, QString("Andreas"));
}

// ------------------------------------------------------------------------------
void tst_BOBUIextStream::status_real_read_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<double>("expected_f");
    BOBUIest::addColumn<QString>("expected_w");
    BOBUIest::addColumn<QList<int> >("results");

    BOBUIest::newRow("1.23 abc   ") << QString("1.23 abc   ") << 1.23 << QString("abc")
                              << (QList<int>()
                                  << (int)BOBUIextStream::Ok
                                  << (int)BOBUIextStream::ReadCorruptData
                                  << (int)BOBUIextStream::Ok
                                  << (int)BOBUIextStream::Ok
                                  << (int)BOBUIextStream::ReadPastEnd);
}

void tst_BOBUIextStream::status_real_read()
{
    QFETCH(QString, input);
    QFETCH(double, expected_f);
    QFETCH(QString, expected_w);
    QFETCH(QList<int>, results);

    BOBUIextStream s(&input);
    double f = 0.0;
    QString w;
    s >> f;
    QCOMPARE((int)s.status(), results.at(0));
    QCOMPARE(f, expected_f);
    s >> f;
    QCOMPARE((int)s.status(), results.at(1));
    s.resetStatus();
    QCOMPARE((int)s.status(), results.at(2));
    s >> w;
    QCOMPARE((int)s.status(), results.at(3));
    QCOMPARE(w, expected_w);
    s >> f;
    QCOMPARE((int)s.status(), results.at(4));
}

void tst_BOBUIextStream::status_integer_read()
{
    BOBUIextStream s("123 abc   ");
    int i;
    QString w;
    QVERIFY(s >> i);
    QCOMPARE(s.status(), BOBUIextStream::Ok);
    QVERIFY(!(s >> i));
    QCOMPARE(s.status(), BOBUIextStream::ReadCorruptData);
    s.resetStatus();
    QCOMPARE(s.status(), BOBUIextStream::Ok);
    QVERIFY(s >> w);
    QCOMPARE(s.status(), BOBUIextStream::Ok);
    QCOMPARE(w, QString("abc"));
    QVERIFY(!(s >> i));
    QCOMPARE(s.status(), BOBUIextStream::ReadPastEnd);
}

void tst_BOBUIextStream::status_word_read()
{
    BOBUIextStream s("abc ");
    QString w;
    QVERIFY(s >> w);
    QCOMPARE(s.status(), BOBUIextStream::Ok);
    QVERIFY(!(s >> w));
    QCOMPARE(s.status(), BOBUIextStream::ReadPastEnd);
}

class FakeBuffer : public QBuffer
{
protected:
    qint64 writeData(const char *c, qint64 i) override { return m_lock ? 0 : QBuffer::writeData(c, i); }
public:
    FakeBuffer(bool locked = false) : m_lock(locked) {}
    void setLocked(bool locked) { m_lock = locked; }
private:
    bool m_lock;
};

void tst_BOBUIextStream::status_write_error()
{
    FakeBuffer fb(false);
    QVERIFY(fb.open(QBuffer::ReadWrite));
    BOBUIextStream fs(&fb);
    fs.setEncoding(QStringConverter::Latin1);
    /* first write some initial content */
    QVERIFY(fs << "hello");
    fs.flush();
    QCOMPARE(fs.status(), BOBUIextStream::Ok);
    QCOMPARE(fb.data(), "hello");
    /* then test that writing can cause an error */
    fb.setLocked(true);
    QVERIFY(fs << "error");
    fs.flush();
    QCOMPARE(fs.status(), BOBUIextStream::WriteFailed);
    QCOMPARE(fb.data(), "hello");
    /* finally test that writing after an error doesn't change the stream any more */
    fb.setLocked(false);
    fs << "can't do that";
    fs.flush();
    QCOMPARE(fs.status(), BOBUIextStream::WriteFailed);
    QCOMPARE(fb.data(), "hello");
}

void tst_BOBUIextStream::alignAccountingStyle()
{
    {
    QString result;
    BOBUIextStream out(&result);
    out.setFieldAlignment(BOBUIextStream::AlignAccountingStyle);
    out.setFieldWidth(4);
    out.setPadChar('0');
    out << -1;
    QCOMPARE(result, QLatin1String("-001"));
    }

    {
    QString result;
    BOBUIextStream out(&result);
    out.setFieldAlignment(BOBUIextStream::AlignAccountingStyle);
    out.setFieldWidth(4);
    out.setPadChar('0');
    QVERIFY(out << "-1");
    QCOMPARE(result, QLatin1String("00-1"));
    }

    {
    QString result;
    BOBUIextStream out(&result);
    out.setFieldAlignment(BOBUIextStream::AlignAccountingStyle);
    out.setFieldWidth(6);
    out.setPadChar('0');
    QVERIFY(out << -1.2);
    QCOMPARE(result, QLatin1String("-001.2"));
    }

    {
    QString result;
    BOBUIextStream out(&result);
    out.setFieldAlignment(BOBUIextStream::AlignAccountingStyle);
    out.setFieldWidth(6);
    out.setPadChar('0');
    QVERIFY(out << "-1.2");
    QCOMPARE(result, QLatin1String("00-1.2"));
    }
}

void tst_BOBUIextStream::setEncoding()
{
    QByteArray ba("\xe5 v\xe6r\n\xc3\xa5 v\xc3\xa6r\n");
    QString res = QLatin1String("\xe5 v\xe6r");

    BOBUIextStream stream(ba);
    stream.setEncoding(QStringConverter::Latin1);
    QCOMPARE(stream.readLine(),res);
    stream.setEncoding(QStringConverter::Utf8);
    QCOMPARE(stream.readLine(),res);
}

void tst_BOBUIextStream::double_write_with_flags_data()
{
    BOBUIest::addColumn<double>("number");
    BOBUIest::addColumn<QString>("output");
    BOBUIest::addColumn<int>("numberFlags");
    BOBUIest::addColumn<int>("realNumberNotation");

    BOBUIest::newRow("-ForceSign") << -1.23 << QString("-1.23") << (int)BOBUIextStream::ForceSign << 0;
    BOBUIest::newRow("+ForceSign") << 1.23 << QString("+1.23") << (int)BOBUIextStream::ForceSign << 0;
    BOBUIest::newRow("inf") << qInf() << QString("inf") << 0 << 0;
    BOBUIest::newRow("-inf") << -qInf() << QString("-inf") << 0 << 0;
    BOBUIest::newRow("inf uppercase") << qInf() << QString("INF") << (int)BOBUIextStream::UppercaseDigits << 0;
    BOBUIest::newRow("-inf uppercase") << -qInf() << QString("-INF") << (int)BOBUIextStream::UppercaseDigits << 0;
    BOBUIest::newRow("nan") << qQNaN() << QString("nan") << 0 << 0;
    BOBUIest::newRow("NAN") << qQNaN() << QString("NAN") << (int)BOBUIextStream::UppercaseDigits << 0;
    BOBUIest::newRow("scientific") << 1.234567e+02 << QString("1.234567e+02") << 0  << (int)BOBUIextStream::ScientificNotation;
    BOBUIest::newRow("scientific2") << 1.234567e+02 << QString("1.234567e+02") << (int)BOBUIextStream::UppercaseBase << (int)BOBUIextStream::ScientificNotation;
    BOBUIest::newRow("scientific uppercase") << 1.234567e+02 << QString("1.234567E+02") << (int)BOBUIextStream::UppercaseDigits << (int)BOBUIextStream::ScientificNotation;
}

void tst_BOBUIextStream::double_write_with_flags()
{
    QFETCH(double, number);
    QFETCH(QString, output);
    QFETCH(int, numberFlags);
    QFETCH(int, realNumberNotation);

    QString buf;
    BOBUIextStream stream(&buf);
    if (numberFlags)
        stream.setNumberFlags(BOBUIextStream::NumberFlag(numberFlags));
    if (realNumberNotation)
        stream.setRealNumberNotation(BOBUIextStream::RealNumberNotation(realNumberNotation));
    QVERIFY(stream << number);
    QCOMPARE(buf, output);
}

void tst_BOBUIextStream::double_write_with_precision_data()
{
    BOBUIest::addColumn<int>("precision");
    BOBUIest::addColumn<double>("value");
    BOBUIest::addColumn<QString>("result");

    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIextStream::setRealNumberPrecision: Invalid precision (-1)");
    BOBUIest::newRow("-1") << -1 << 3.14159 << QString("3.14159");
    BOBUIest::newRow("0") << 0 << 3.14159 << QString("3");
    BOBUIest::newRow("1") << 1 << 3.14159 << QString("3");
    BOBUIest::newRow("2") << 2 << 3.14159 << QString("3.1");
    BOBUIest::newRow("3") << 3 << 3.14159 << QString("3.14");
    BOBUIest::newRow("5") << 5 << 3.14159 << QString("3.1416");
    BOBUIest::newRow("6") << 6 << 3.14159 << QString("3.14159");
    BOBUIest::newRow("7") << 7 << 3.14159 << QString("3.14159");
    BOBUIest::newRow("10") << 10 << 3.14159 << QString("3.14159");
}

void tst_BOBUIextStream::double_write_with_precision()
{
    QFETCH(int, precision);
    QFETCH(double, value);
    QFETCH(QString, result);

    QString buf;
    BOBUIextStream stream(&buf);
    stream.setRealNumberPrecision(precision);
    QVERIFY(stream << value);
    QCOMPARE(buf, result);
}

void tst_BOBUIextStream::int_read_with_locale_data()
{
    BOBUIest::addColumn<QString>("locale");
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<int>("output");
    BOBUIest::addColumn<bool>("status");

    BOBUIest::newRow("C -123") << QString("C") << QString("-123") << -123 << true;
    BOBUIest::newRow("C +123") << QString("C") << QString("+123") << 123 << true;
    BOBUIest::newRow("C 12345") << QString("C") << QString("12345") << 12345 << true;
    BOBUIest::newRow("C 12,345") << QString("C") << QString("12,345") << 12 << true;
    BOBUIest::newRow("C 12.345") << QString("C") << QString("12.345") << 12 << true;

    BOBUIest::newRow("de_DE -123") << QString("de_DE") << QString("-123") << -123 << true;
    BOBUIest::newRow("de_DE +123") << QString("de_DE") << QString("+123") << 123 << true;
    BOBUIest::newRow("de_DE 12345") << QString("de_DE") << QString("12345") << 12345 << true;
    BOBUIest::newRow("de_DE 12.345") << QString("de_DE") << QString("12.345") << 12345 << true;
    BOBUIest::newRow("de_DE .12345") << QString("de_DE") << QString(".12345") << 0 << false;
}

void tst_BOBUIextStream::int_read_with_locale()
{
    QFETCH(QString, locale);
    QFETCH(QString, input);
    QFETCH(int, output);
    QFETCH(bool, status);

    BOBUIextStream stream(&input);
    stream.setLocale(QLocale(locale));
    int result;
    QCOMPARE(static_cast<bool>(stream >> result), status);
    QCOMPARE(result, output);
}

void tst_BOBUIextStream::int_write_with_locale_data()
{
    BOBUIest::addColumn<QString>("locale");
    BOBUIest::addColumn<int>("numberFlags");
    BOBUIest::addColumn<int>("input");
    BOBUIest::addColumn<QString>("output");
    BOBUIest::addColumn<int>("fieldWidth");
    BOBUIest::addColumn<BOBUIextStream::FieldAlignment>("fieldAlignment");

    const auto alignDefault = BOBUIextStream().fieldAlignment();
    constexpr int forceSign = BOBUIextStream::ForceSign;

    BOBUIest::newRow("C -123") << u"C"_s << 0 << -123 << u"-123"_s << 0 << alignDefault;
    BOBUIest::newRow("C +123") << u"C"_s << forceSign << 123 << u"+123"_s << 0 << alignDefault;
    BOBUIest::newRow("C 12345") << u"C"_s << 0 << 12345 << u"12345"_s << 0 << alignDefault;

    BOBUIest::newRow("de_DE -123") << u"de_DE"_s << 0 << -123 << u"-123"_s << 0 << alignDefault;
    BOBUIest::newRow("de_DE +123") << u"de_DE"_s << forceSign << 123 << u"+123"_s << 0 << alignDefault;
    BOBUIest::newRow("de_DE 12345") << u"de_DE"_s << 0 << 12345 << u"12.345"_s << 0 << alignDefault;

    constexpr auto alignAccountingStyle = BOBUIextStream::FieldAlignment::AlignAccountingStyle;

    {
        const QLocale loc("ar_EG"_L1);
        // Arabic as spoken in Egypt has a two-code-point negativeSign():
        const auto minus = loc.negativeSign();
        QCOMPARE(minus.size(), 2);
        // ditto positiveSign():
        const auto plus = loc.positiveSign();
        QCOMPARE(plus.size(), 2);

        BOBUIest::addRow("ar_EG -123") << u"ar_EG"_s << 0 << -123
                                    << (minus + u"     ١٢٣")
                                    << 10 << alignAccountingStyle;
        BOBUIest::newRow("ar_EG +123") << u"ar_EG"_s << forceSign << 123
                                    << (plus + u"     ١٢٣")
                                    << 10 << alignAccountingStyle;
        BOBUIest::newRow("ar_EG 12345") << u"ar_EG"_s << 0 << 12345
                                     << u"    ١٢٬٣٤٥"_s
                                     << 10 << alignAccountingStyle;
    }
}

void tst_BOBUIextStream::int_write_with_locale()
{
    QFETCH(QString, locale);
    QFETCH(int, numberFlags);
    QFETCH(int, input);
    QFETCH(QString, output);
    QFETCH(const int, fieldWidth);
    QFETCH(const BOBUIextStream::FieldAlignment, fieldAlignment);

    QString result;
    BOBUIextStream stream(&result);
    stream.setLocale(QLocale(locale));
    stream.setFieldAlignment(fieldAlignment);
    if (numberFlags)
        stream.setNumberFlags(BOBUIextStream::NumberFlags(numberFlags));
    if (fieldWidth)
        stream.setFieldWidth(fieldWidth);

    QVERIFY(stream << input);
    QCOMPARE(result, output);
}

void tst_BOBUIextStream::textModeOnEmptyRead()
{
    const QString filename(tempDir.path() + QLatin1String("/textmodetest.txt"));

    QFile file(filename);
    QVERIFY2(file.open(QIODevice::ReadWrite | QIODevice::Text), qPrintable(file.errorString()));
    BOBUIextStream stream(&file);
    QVERIFY(file.isTextModeEnabled());
    QString emptyLine = stream.readLine(); // Text mode flag cleared here
    QVERIFY(file.isTextModeEnabled());
}

void tst_BOBUIextStream::autodetectUnicode_data()
{
    BOBUIest::addColumn<QStringConverter::Encoding>("encoding");
    BOBUIest::newRow("Utf8") << QStringConverter::Utf8;
    BOBUIest::newRow("Utf16BE") << QStringConverter::Utf16BE;
    BOBUIest::newRow("Utf16LE") << QStringConverter::Utf16LE;
    BOBUIest::newRow("Utf32BE") << QStringConverter::Utf32BE;
    BOBUIest::newRow("Utf32LE") << QStringConverter::Utf32LE;
}

void tst_BOBUIextStream::autodetectUnicode()
{
    QFETCH(QStringConverter::Encoding, encoding);

    BOBUIemporaryFile file;
    QVERIFY(file.open());

    QString original("HelloWorld👋");

    {
        BOBUIextStream out(&file);
        out.setGenerateByteOrderMark(true);
        out.setEncoding(encoding);
        out << original;
    }
    file.seek(0);
    {
        BOBUIextStream in(&file);
        QString actual;
        QVERIFY(in >> actual);
        QCOMPARE(actual, original);
        QCOMPARE(in.encoding(), encoding);
    }
    file.seek(0);
    // Again, but change order of calls to BOBUIextStream...
    {
        BOBUIextStream out(&file);
        out.setEncoding(encoding);
        out.setGenerateByteOrderMark(true);
        out << original;
    }
    file.seek(0);
    {
        BOBUIextStream in(&file);
        QString actual;
        QVERIFY(in >> actual);
        QCOMPARE(actual, original);
        QCOMPARE(in.encoding(), encoding);
    }
}


// ------------------------------------------------------------------------------

BOBUIEST_MAIN(tst_BOBUIextStream)
#include "tst_bobuiextstream.moc"

