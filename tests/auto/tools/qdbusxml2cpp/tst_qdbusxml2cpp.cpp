// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QLibraryInfo>
#include <BobUICore/QProcess>
#include <BobUICore/QRegularExpression>

// We just need the DBUS_TYPE_* constants, so use our own copy
#include "../../../../src/dbus/dbus_minimal_p.h"

class tst_qdbusxml2cpp : public QObject
{
    Q_OBJECT

    enum { Interface, Adaptor };

private slots:
    void initTestCase_data();
    void process_data();
    void process();
    void includeStyle_data();
    void includeStyle();
    void missingAnnotation_data();
    void missingAnnotation();
    void includeMoc_data();
    void includeMoc();
    void customNamespace_data();
    void customNamespace();
};

struct BasicTypeList {
    char dbusType[3];
    char cppType[24];
};
static const BasicTypeList basicTypeList[] =
{
    { DBUS_TYPE_BOOLEAN_AS_STRING, "bool" },
    { DBUS_TYPE_BYTE_AS_STRING, "uchar" },
    { DBUS_TYPE_INT16_AS_STRING, "short" },
    { DBUS_TYPE_UINT16_AS_STRING, "ushort" },
    { DBUS_TYPE_INT32_AS_STRING, "int" },
    { DBUS_TYPE_UINT32_AS_STRING, "uint" },
    { DBUS_TYPE_INT64_AS_STRING, "qlonglong" },
    { DBUS_TYPE_UINT64_AS_STRING, "qulonglong" },
    { DBUS_TYPE_DOUBLE_AS_STRING, "double" },
    { DBUS_TYPE_STRING_AS_STRING, "QString" },
    { DBUS_TYPE_OBJECT_PATH_AS_STRING, "QDBusObjectPath" },
    { DBUS_TYPE_SIGNATURE_AS_STRING, "QDBusSignature" },
#ifdef DBUS_TYPE_UNIX_FD_AS_STRING
    { DBUS_TYPE_UNIX_FD_AS_STRING, "QDBusUnixFileDescriptor" },
#endif
    { DBUS_TYPE_VARIANT_AS_STRING, "QDBusVariant" },
    { DBUS_TYPE_ARRAY_AS_STRING DBUS_TYPE_BYTE_AS_STRING, "QByteArray" },
    { DBUS_TYPE_ARRAY_AS_STRING DBUS_TYPE_STRING_AS_STRING, "QStringList" },
    { DBUS_TYPE_ARRAY_AS_STRING DBUS_TYPE_VARIANT_AS_STRING, "QVariantList" }
};
static const int basicTypeCount = sizeof(basicTypeList) / sizeof(basicTypeList[0]);

static QString stripHeader(QString output)
{
    static QRegularExpression header("^.*?(?=\\Rclass)", QRegularExpression::DotMatchesEverythingOption);
    return output.remove(header);
}

static void runTool(QProcess &process, const QByteArray &data,
                    const QStringList &flags)
{
    // test both interface and adaptor generation
    QFETCH_GLOBAL(QString, commandLineArg);

    // Run the tool
    const QString binpath = QLibraryInfo::path(QLibraryInfo::BinariesPath);
    QStringList arguments = { commandLineArg };
    arguments += flags;
    process.setArguments(arguments);
    process.setProgram(binpath + QLatin1String("/qdbusxml2cpp"));
    process.start(QIODevice::Text | QIODevice::ReadWrite);
    QVERIFY2(process.waitForStarted(), qPrintable(process.errorString()));

    static const char xmlHeader[] =
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE // \n is included
            "<node>\n"
            "  <interface name=\"local.name.is.not.important\">\n"
            "    <!-- begin data -->\n";
    static const char xmlFooter[] = "\n"
            "    <!-- end data -->\n"
            "  </interface>\n"
            "</node>\n";

    process.write(xmlHeader, sizeof(xmlHeader) - 1);
    process.write(data);
    process.write(xmlFooter, sizeof(xmlFooter) - 1);

    while (process.bytesToWrite())
        QVERIFY2(process.waitForBytesWritten(), qPrintable(process.errorString()));
    //    fprintf(stderr, "%s%s%s", xmlHeader, xmlSnippet.toLatin1().constData(), xmlFooter);

    process.closeWriteChannel();
    QVERIFY2(process.waitForFinished(), qPrintable(process.errorString()));
    QCOMPARE(process.exitStatus(), QProcess::NormalExit);
}

static void checkOneFile(const QString &fileName, const QByteArray &expected)
{
    QFile file(fileName);
    QVERIFY(file.exists());
    const auto guard = QScopeGuard([&](){ QFile::remove(fileName); });

    QVERIFY(file.open(QFile::Text | QFile::ReadOnly));
    QByteArray text = file.readAll();
    QVERIFY(text.contains(expected));
}

static void checkTwoFiles(const QString &headerName, const QString &sourceName, const QByteArray &expected)
{
    QFile headerFile(headerName);
    QFile sourceFile(sourceName);

    QVERIFY(headerFile.exists());
    const auto headerGuard = QScopeGuard([&](){ QFile::remove(headerName); });

    QVERIFY(sourceFile.exists());
    const auto sourceGuard = QScopeGuard([&](){ QFile::remove(sourceName); });

    QVERIFY(sourceFile.open(QFile::Text | QFile::ReadOnly));
    QByteArray text = sourceFile.readAll();
    QVERIFY(text.contains(expected));
}

void tst_qdbusxml2cpp::initTestCase_data()
{
    BOBUIest::addColumn<int>("outputMode");
    BOBUIest::addColumn<QString>("commandLineArg");
    BOBUIest::newRow("interface") << int(Interface) << "-p";
    BOBUIest::newRow("adaptor") << int(Adaptor) << "-a";
}

void tst_qdbusxml2cpp::process_data()
{
    BOBUIest::addColumn<QString>("xmlSnippet");
    BOBUIest::addColumn<QRegularExpression>("interfaceSearch");
    BOBUIest::addColumn<QRegularExpression>("adaptorSearch");

    // -- class info --
    BOBUIest::newRow("classinfo")
            << ""
            << QRegularExpression("staticInterfaceName\\(\\)\\s+"
                                  "{ return \"local\\.name\\.is\\.not\\.important\"\\; }")
            << QRegularExpression("Q_CLASSINFO\\(\"D-Bus Interface\", \"local\\.name\\.is\\.not\\.important\"\\)");

    // -- properties --
    for (int i = 0; i < basicTypeCount; ++i) {
        QRegularExpression rx(QString("\\bQ_PROPERTY\\(%1 PropertyIsPresent "
                                                      "READ propertyIsPresent WRITE setPropertyIsPresent\\b")
                                              .arg(basicTypeList[i].cppType));
        BOBUIest::newRow(QByteArray("property-") + basicTypeList[i].dbusType)
                << QString("<property type=\"%1\" name=\"PropertyIsPresent\" access=\"readwrite\" />")
                   .arg(basicTypeList[i].dbusType)
                << rx << rx;
    }

    BOBUIest::newRow("property-readonly-multi")
            << "<property type=\"i\" name=\"Value\" access=\"read\"></property>"
            << QRegularExpression("\\bQ_PROPERTY\\(int Value READ value(?! WRITE)")
            << QRegularExpression("\\bQ_PROPERTY\\(int Value READ value(?! WRITE)");
    BOBUIest::newRow("property-readonly")
            << "<property type=\"i\" name=\"Value\" access=\"read\" />"
            << QRegularExpression("\\bQ_PROPERTY\\(int Value READ value(?! WRITE)")
            << QRegularExpression("\\bQ_PROPERTY\\(int Value READ value(?! WRITE)");
    BOBUIest::newRow("property-writeonly")
            << "<property type=\"i\" name=\"Value\" access=\"write\" />"
            << QRegularExpression("\\bQ_PROPERTY\\(int Value WRITE setValue\\b")
            << QRegularExpression("\\bQ_PROPERTY\\(int Value WRITE setValue\\b");

    BOBUIest::newRow("property-getter-setter")
            << "<property type=\"b\" name=\"Enabled\" access=\"readwrite\">"
               "<annotation name=\"org.bobuiproject.BobUIDBus.PropertyGetter\" value=\"wasEnabled\" />"
               "<annotation name=\"org.bobuiproject.BobUIDBus.PropertySetter\" value=\"setEnabledFlag\" />"
               "</property>"
            << QRegularExpression("\\bQ_PROPERTY\\(bool Enabled READ wasEnabled WRITE setEnabledFlag\\b.*"
                                  "\\bbool wasEnabled\\(\\) const.*" // no semi-colon
                                  "\\bvoid setEnabledFlag\\(bool", QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("\\bQ_PROPERTY\\(bool Enabled READ wasEnabled WRITE setEnabledFlag\\b.*"
                                  "\\bbool wasEnabled\\(\\) const;.*" // has semi-colon
                                  "\\bvoid setEnabledFlag\\(bool", QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("property-complex")
            << "<property type=\"(ii)\" name=\"Position\" access=\"readwrite\">"
               "<annotation name=\"org.bobuiproject.BobUIDBus.BobUITypeName\" value=\"Point\"/>"
               "</property>"
            << QRegularExpression("\\bQ_PROPERTY\\(Point Position READ position WRITE setPosition\\b")
            << QRegularExpression("\\bQ_PROPERTY\\(Point Position READ position WRITE setPosition\\b");

    // -- methods --
    for (int i = 0; i < basicTypeCount; ++i) {
        BOBUIest::newRow(QByteArray("method-") + basicTypeList[i].dbusType)
                << QString("<method name=\"Method\">"
                           "<arg type=\"%1\" direction=\"out\"/>"
                           "<arg type=\"%1\" direction=\"in\"/>"
                           "</method>")
                   .arg(basicTypeList[i].dbusType)
                << QRegularExpression(QString("Q_SLOTS:.*\\bQDBusPendingReply<%1> Method\\((const )?%1 ")
                                      .arg(basicTypeList[i].cppType), QRegularExpression::DotMatchesEverythingOption)
                << QRegularExpression(QString("Q_SLOTS:.*\\b%1 Method\\((const )?%1 &?in0\\);"
                                              ".*%1 .*::Method\\((const )?%1 &?in0\\)\n{\n"
                                              ".*%1 out0{};")
                                      .arg(basicTypeList[i].cppType), QRegularExpression::DotMatchesEverythingOption);
    }

    BOBUIest::newRow("method-name")
        << "<method name=\"Method\">"
            "<arg type=\"s\" direction=\"in\"/>"
            "<annotation name=\"org.bobuiproject.BobUIDBus.MethodName\" value=\"MethodRenamed\" />"
            "</method>"
        << QRegularExpression("Q_SLOTS:.*QDBusPendingReply<> MethodRenamed\\(const QString &\\w*",
                                QRegularExpression::DotMatchesEverythingOption)
        << QRegularExpression("Q_SLOTS:.*void MethodRenamed\\(const QString &\\w*",
                                QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-complex")
            << "<method name=\"Method\">"
               "<arg type=\"(dd)\" direction=\"in\"/>"
               "<arg type=\"(ii)\" direction=\"out\"/>"
               "<annotation name=\"org.bobuiproject.BobUIDBus.BobUITypeName.Out0\" value=\"Point\"/>"
               "<annotation name=\"org.bobuiproject.BobUIDBus.BobUITypeName.In0\" value=\"PointF\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*\\bQDBusPendingReply<Point> Method\\(PointF ",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*\\bPoint Method\\(PointF in0\\);"
                                  ".*Point .*::Method\\(PointF in0\\)\n{\n"
                                  ".*Point out0{};",
                                  QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-ss")
            << "<method name=\"Method\">"
               "<arg type=\"s\" direction=\"in\"/>"
               "<arg type=\"s\" direction=\"in\"/>"
               "<arg type=\"s\" direction=\"out\"/>"
               "<arg type=\"s\" direction=\"out\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*QDBusPendingReply<QString, QString> Method\\(const QString &\\w*, const QString &\\w*\\)"
                                  ".*inline QDBusReply<QString> Method\\(const QString &\\w*, const QString &\\w*, QString &\\w*\\)",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*QString Method\\(const QString &\\w*, const QString &\\w*, QString &",
                                  QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-deprecated-0out")
            << "<method name=\"Method\">"
               "<annotation name=\"org.freedesktop.DBus.Deprecated\" value=\"true\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*Q_DECL_DEPRECATED inline QDBusPendingReply<> Method\\(\\)",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*\n\\s*void Method\\(\\)",  // no Q_DECL_DEPRECATED
                                  QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-deprecated-2out")
            << "<method name=\"Method\">"
               "<annotation name=\"org.freedesktop.DBus.Deprecated\" value=\"true\"/>"
               "<arg type=\"s\" direction=\"out\"/>"
               "<arg type=\"s\" direction=\"out\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*Q_DECL_DEPRECATED inline QDBusPendingReply<QString, QString> Method\\(\\)"
                                  ".*Q_DECL_DEPRECATED inline QDBusReply<QString> Method\\(QString &\\w*\\)",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*\n\\s*QString Method\\(QString &", // no Q_DECL_DEPRECATED
                                  QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-noreply")
            << "<method name=\"Method\">"
               "<annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*Q_NOREPLY inline void Method\\(\\).*\\bQDBus::NoBlock\\b",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*Q_NOREPLY void Method\\(",
                                  QRegularExpression::DotMatchesEverythingOption);

    BOBUIest::newRow("method-deprecated-noreply")
            << "<method name=\"Method\">"
               "<annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\"/>"
               "<annotation name=\"org.freedesktop.DBus.Deprecated\" value=\"true\"/>"
               "</method>"
            << QRegularExpression("Q_SLOTS:.*Q_DECL_DEPRECATED Q_NOREPLY inline void Method\\(\\).*\\bQDBus::NoBlock\\b",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression("Q_SLOTS:.*Q_NOREPLY void Method\\(",
                                  QRegularExpression::DotMatchesEverythingOption);

    // -- signals --
    for (int i = 0; i < basicTypeCount; ++i) {
        QRegularExpression rx(QString("Q_SIGNALS:.*\\bvoid Signal\\((const )?%1\\b")
                              .arg(basicTypeList[i].cppType),
                              QRegularExpression::DotMatchesEverythingOption);
        BOBUIest::newRow(QByteArray("signal-") + basicTypeList[i].dbusType)
                << QString("<signal name=\"Signal\">"
                           "<arg type=\"%1\"/>"
                           "</signal>")
                   .arg(basicTypeList[i].dbusType)
                << rx << rx;
    }

    QRegularExpression rx(R"(Q_SIGNALS:.*\b\Qvoid Signal(const QVariantMap &map);\E)",
                          QRegularExpression::DotMatchesEverythingOption);
    BOBUIest::newRow("signal-complex")
            << R"(<signal name="Signal">
                    <arg type="a{sv}" name="map"/>
                    <annotation name="org.bobuiproject.BobUIDBus.BobUITypeName.Out0" value="QVariantMap"/>"
                  </signal>)"
            << rx << rx;

    BOBUIest::newRow("signal-deprecated")
            << R"(<signal name="Signal">
                    <annotation name="org.freedesktop.DBus.Deprecated" value="true"/>
                  </signal>)"
            << QRegularExpression(R"(Q_SIGNALS:.*\bQ_DECL_DEPRECATED void Signal\(\))",
                                  QRegularExpression::DotMatchesEverythingOption)
            << QRegularExpression(R"(Q_SIGNALS:.*\n\s*void Signal\(\))",    // no Q_DECL_DEPRECATED
                                  QRegularExpression::DotMatchesEverythingOption);
}

void tst_qdbusxml2cpp::process()
{
    QFETCH(QString, xmlSnippet);
    QFETCH(QRegularExpression, interfaceSearch);
    QFETCH(QRegularExpression, adaptorSearch);
    QVERIFY2(interfaceSearch.isValid(), qPrintable(interfaceSearch.errorString()));
    QVERIFY2(adaptorSearch.isValid(), qPrintable(adaptorSearch.errorString()));

    QFETCH_GLOBAL(int, outputMode);
    QProcess process;
    QStringList flags = {"-", "-N"};
    runTool(process, xmlSnippet.toLatin1(), flags);
    if (BOBUIest::currentTestFailed()) return;

    QByteArray errOutput = process.readAllStandardError();
    QVERIFY2(errOutput.isEmpty(), errOutput);
    QCOMPARE(process.exitCode(), 0);

    QByteArray fullOutput = process.readAll();
    QString output = stripHeader(QString::fromLatin1(fullOutput));
    QVERIFY2(!output.isEmpty(), fullOutput);
    if (outputMode == Interface)
        QVERIFY2(output.count(interfaceSearch) == 1, qPrintable(interfaceSearch.pattern() + "\nin\n" + output));
    else
        QVERIFY2(output.count(adaptorSearch) == 1, qPrintable(adaptorSearch.pattern() + "\nin\n" + output));
}

void tst_qdbusxml2cpp::includeStyle_data()
{
    BOBUIest::addColumn<bool>("isGlobal");
    BOBUIest::addColumn<QByteArray>("expected");

    BOBUIest::newRow("localInclude") <<  false << QByteArray("#include \"test.hpp\"");
    BOBUIest::newRow("globalInclude") <<  true << QByteArray("#include <test.hpp>");
}

void tst_qdbusxml2cpp::includeStyle()
{
    QFETCH(bool, isGlobal);
    QFETCH(QByteArray, expected);

    QProcess process;
    QStringList flags = {"-", "-N", (isGlobal ? "-I" : "-i"), "test.hpp"};

    runTool(process,QByteArray{},flags);
    QCOMPARE(process.exitCode(), 0);

    QByteArray errOutput = process.readAllStandardError();
    QVERIFY2(errOutput.isEmpty(), errOutput);

    QByteArray fullOutput = process.readAll();
    QVERIFY(!fullOutput.isEmpty());
    QVERIFY(fullOutput.contains(expected));
}

void tst_qdbusxml2cpp::missingAnnotation_data()
{
    BOBUIest::addColumn<QString>("xmlSnippet");
    BOBUIest::addColumn<QString>("annotationName");
    BOBUIest::addColumn<QString>("location");

    BOBUIest::newRow("property")
            << R"(<property type="%1" name="name" access="readwrite"/>)"
            << "org.bobuiproject.BobUIDBus.BobUITypeName"
            << "7:2";
    BOBUIest::newRow("method-in")
            << R"(<method name="Method">
                    <arg type="%1" name="name" direction="in"/>
                  </method>)"
            << "org.bobuiproject.BobUIDBus.BobUITypeName.In0"
            << "8:22";
    BOBUIest::newRow("method-out")
            << R"(<method name="Method">
                    <arg type="%1" name="name" direction="out"/>
                  </method>)"
            << "org.bobuiproject.BobUIDBus.BobUITypeName.Out0"
            << "8:22";
    BOBUIest::newRow("signal")
            << R"(<signal name="Signal">
                    <arg type="%1" name="name"/>
                  </signal>)"
            << "org.bobuiproject.BobUIDBus.BobUITypeName.Out0"
            << "8:22";
    BOBUIest::newRow("signal-out")
            << R"(<signal name="Signal">
                    <arg type="%1" name="name" direction="out"/>
                  </signal>)"
            << "org.bobuiproject.BobUIDBus.BobUITypeName.Out0"
            << "8:22";
}

void tst_qdbusxml2cpp::missingAnnotation()
{
    QFETCH(QString, xmlSnippet);
    QFETCH(QString, annotationName);
    QFETCH(QString, location);

    QString type = "(ii)";
    QProcess process;
    QStringList flags = {"-", "-N"};
    runTool(process, xmlSnippet.arg(type).toLatin1(),flags);
    if (BOBUIest::currentTestFailed()) return;

    // it must have failed
    QString errOutput = QString::fromLatin1(process.readAllStandardError().trimmed());
    QCOMPARE(process.exitCode(), 1);
    QCOMPARE(process.readAllStandardOutput(), QByteArray());
    QVERIFY(!errOutput.isEmpty());

    // check it did suggest the right annotation
    QString expected = R"(<standard input>:%3: error: unknown type `%1'
<standard input>:%3: note: you should add <annotation name="%2" value="<type>"/>)";
    expected = expected.arg(type, annotationName, location);
    QCOMPARE(errOutput, expected);
}

void tst_qdbusxml2cpp::includeMoc_data()
{
    BOBUIest::addColumn<QString>("filenames");
    BOBUIest::addColumn<QByteArray>("expected");
    BOBUIest::addColumn<QByteArray>("warning");

    BOBUIest::newRow("combined-h") << "foo.h" << QByteArray("#include \"foo.moc\"") << QByteArray("");
    BOBUIest::newRow("combined-cpp") << "foo.cpp" << QByteArray("#include \"foo.moc\"") << QByteArray("");
    BOBUIest::newRow("combined-cc") << "foo.cc" << QByteArray("#include \"foo.moc\"") << QByteArray("");
    BOBUIest::newRow("without extension") << "foo" << QByteArray("#include \"moc_foo.cpp\"") << QByteArray("");
    BOBUIest::newRow("cpp-only") << ":foo.cpp" << QByteArray("#include \"moc_foo.cpp\"")
                              << QByteArray("warning: no header name is provided, assuming it to be \"foo.h\"");
    BOBUIest::newRow("header-and-cpp") << "foo_h.h:foo.cpp" << QByteArray("#include \"moc_foo_h.cpp\"") << QByteArray("");

    BOBUIest::newRow("combined-cpp with dots") << "foo.bar.cpp" << QByteArray("#include \"foo.bar.moc\"") << QByteArray("");
    BOBUIest::newRow("without extension with dots") << "foo.bar" << QByteArray("#include \"moc_foo.bar.cpp\"") << QByteArray("");
    BOBUIest::newRow("header-and-cpp with dots") << "foo.bar_h.h:foo.bar.cpp" << QByteArray("#include \"moc_foo.bar_h.cpp\"") << QByteArray("");
}

void tst_qdbusxml2cpp::includeMoc()
{
    QFETCH(QString, filenames);
    QFETCH(QByteArray, expected);
    QFETCH(QByteArray, warning);

    QProcess process;
    QStringList flags = {filenames, "--moc"};
    runTool(process,QByteArray{},flags);
    QByteArray errOutput = process.readAllStandardError();
    QVERIFY(errOutput.startsWith(warning));
    QCOMPARE(process.exitCode(), 0);

    QStringList parts = filenames.split(u':');
    QFileInfo first{parts.first()};

    const bool firstHasSuffix = QStringList({"h", "cpp", "cc"}).contains(first.suffix());

    if ((parts.size() == 1) && firstHasSuffix) {
        checkOneFile(parts.first(), expected);
    } else if ((parts.size() == 1) && (!firstHasSuffix)) {
        QString headerName{parts.first()};
        headerName += ".h";
        QString sourceName{parts.first()};
        sourceName += ".cpp";

        checkTwoFiles(headerName, sourceName, expected);
    } else if ((parts.size() == 2) && (parts.first().isEmpty())) {
        checkOneFile(parts.last(), expected);
    }
    else if ((parts.size() == 2) && !parts.first().isEmpty() && !parts.last().isEmpty()) {
        checkTwoFiles(parts.first(), parts.last(), expected);
    }
}

void tst_qdbusxml2cpp::customNamespace_data()
{
    BOBUIest::addColumn<QByteArray>("namesp");

    BOBUIest::newRow("simple") << QByteArray("lancetest");
    BOBUIest::newRow("double") << QByteArray("lance::test");
}

void tst_qdbusxml2cpp::customNamespace()
{
    QFETCH(QByteArray, namesp);

    QProcess process;
    QStringList flags = {"-", "--namespace", namesp};

    runTool(process,QByteArray{},flags);
    QCOMPARE(process.exitCode(), 0);

    QByteArray errOutput = process.readAllStandardError();
    QVERIFY2(errOutput.isEmpty(), errOutput);

    QByteArray fullOutput = process.readAll();
    QVERIFY(!fullOutput.isEmpty());

    // twice: once in the header, once in the implementation
    static constexpr qsizetype requiredNameSpaceCount = 2;
    QCOMPARE(fullOutput.count("namespace " + namesp + " {"), requiredNameSpaceCount);

    static constexpr QByteArrayView endMarker("} // end of namespace ");
    qsizetype startFrom = 0;
    for (qsizetype i = 0; i < requiredNameSpaceCount; ++i) {
        // make sure the namespace is there
        qsizetype namespaceStart = fullOutput.indexOf("namespace " + namesp + " {", startFrom);
        qsizetype namespaceEnd = fullOutput.indexOf(endMarker + namesp, namespaceStart);
        QCOMPARE_GE(namespaceStart, 0);
        QCOMPARE_GT(namespaceEnd, namespaceStart);
        startFrom = namespaceEnd + endMarker.size() + namesp.size();

        // make sure we cover a useful part of the source code with the namespace:
        auto partOutput = QByteArrayView(fullOutput)
                .slice(namespaceStart, startFrom - namespaceStart);
        QCOMPARE(partOutput.count("{"), partOutput.count("}"));
    }
}

BOBUIEST_MAIN(tst_qdbusxml2cpp)

#include "tst_qdbusxml2cpp.moc"
