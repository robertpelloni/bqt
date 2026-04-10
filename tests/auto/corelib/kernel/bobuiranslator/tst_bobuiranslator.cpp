// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QWaitCondition>
#include <QMutex>
#include <QStandardPaths>
#include <bobuiranslator.h>
#include <qfile.h>
#include <bobuiemporarydir.h>

#include <BobUICore/private/qlocale_p.h>

#ifdef Q_OS_ANDROID
#include <QDirIterator>
#endif

#if !defined(BOBUI_NO_SYSTEMLOCALE) && defined(BOBUI_BUILD_INTERNAL)
// from tst_qlocale.cpp; override the system locale with one that supports multiple
// languages
class MySystemLocale : public QSystemLocale
{
    Q_DISABLE_COPY_MOVE(MySystemLocale)
public:
    MySystemLocale(const QStringList &languages)
        : m_languages(languages), m_locale(languages.first())
        , m_id(QLocaleId::fromName(languages.first()))
    {
    }

    QVariant query(QueryType type, QVariant &&/*in*/) const override
    {
        switch (type) {
        case UILanguages:
            return QVariant(m_languages);
        case LanguageId:
            return m_id.language_id;
        case TerritoryId:
            return m_id.territory_id;
        case ScriptId:
            return m_id.script_id;

        default:
            break;
        }
        return QVariant();
    }

    QLocale fallbackLocale() const override
    {
        return m_locale;
    }

private:
    QStringList m_languages;
    const QLocale m_locale;
    const QLocaleId m_id;
};
#endif // !defined(BOBUI_NO_SYSTEMLOCALE) && defined(BOBUI_BUILD_INTERNAL)

class tst_BOBUIranslator : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIranslator();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void initTestCase();
    void init();

    void load_data();
    void load();
    void loadLocale_data();
    void loadLocale();
    void threadLoad();
    void install();
    void testLanguageChange();
    void plural();
    void translate_qm_file_generated_with_msgfmt();
    void loadDirectory();
    void dependencies();
    void translationInThreadWhileInstallingTranslator();

private:
    int languageChangeEventCounter;
    QSharedPointer<BOBUIemporaryDir> dataDir;
};

tst_BOBUIranslator::tst_BOBUIranslator()
    : languageChangeEventCounter(0)
{
    qApp->installEventFilter(this);
}

void tst_BOBUIranslator::initTestCase()
{
    dataDir = QEXTRACTTESTDATA(QStringLiteral("/tst_bobuiranslator"));
    QVERIFY2(!dataDir.isNull(), qPrintable("Could not extract test data"));
}

void tst_BOBUIranslator::init()
{
    QVERIFY2(QDir::setCurrent(dataDir->path()),
             qPrintable("Could not chdir to " + dataDir->path()));
}

bool tst_BOBUIranslator::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ++languageChangeEventCounter;
    return false;
}

void tst_BOBUIranslator::load_data()
{
    BOBUIest::addColumn<QString>("filepath");
    BOBUIest::addColumn<bool>("isEmpty");
    BOBUIest::addColumn<QString>("translation");
    BOBUIest::addColumn<QString>("language");

    BOBUIest::newRow("hellotr_la") << "hellotr_la.qm" << false << "Hallo Welt!" << "de";
    BOBUIest::newRow("hellotr_empty") << "hellotr_empty.qm" << true << "" << "";
}

void tst_BOBUIranslator::load()
{
    QFETCH(QString, filepath);
    QFETCH(bool, isEmpty);
    QFETCH(QString, translation);
    QFETCH(QString, language);

    {
        BOBUIranslator tor;
        QVERIFY(tor.load(QFileInfo(filepath).baseName()));
        QCOMPARE(tor.isEmpty(), isEmpty);
        QCOMPARE(tor.translate("QPushButton", "Hello world!"), translation);
        QCOMPARE(tor.filePath(), filepath);
        QCOMPARE(tor.language(), language);
    }

    {
        QFile file(filepath);
        QVERIFY(file.open(QFile::ReadOnly));
        QByteArray data = file.readAll();
        BOBUIranslator tor;
        QVERIFY(tor.load((const uchar *)data.constData(), data.length()));
        QCOMPARE(tor.isEmpty(), isEmpty);
        QCOMPARE(tor.translate("QPushButton", "Hello world!"), translation);
        QCOMPARE(tor.filePath(), "");
        QCOMPARE(tor.language(), language);
    }

    {
        BOBUIranslator tor;
        QString path = QString(":/tst_bobuiranslator/%1").arg(filepath);
        QVERIFY(tor.load(path));
        QCOMPARE(tor.isEmpty(), isEmpty);
        QCOMPARE(tor.translate("QPushButton", "Hello world!"), translation);
        QCOMPARE(tor.filePath(), path);
        QCOMPARE(tor.language(), language);
    }
}

void tst_BOBUIranslator::loadLocale_data()
{
    BOBUIest::addColumn<QLocale>("wantedLocale");
    BOBUIest::addColumn<QStringList>("languages");

    // variation of translation files for the same language
    BOBUIest::addRow("US English")
                            << QLocale("en-US")
                            << QStringList{"en-US", "en"};
    BOBUIest::addRow("Australia")
                            << QLocale("en-AU")
                            << QStringList{"en-Latn-AU", "en-AU", "en"};
    BOBUIest::addRow("Taiwan") << QLocale("zh-TW") << QStringList{"zh-TW", "zh"};

    // This produces a QLocale::uiLanguages list of
    // {"en-NO", "en-Latn-NO", "nb-NO", "nb-Latn-NO", "nb",
    //  "de-DE", "de-Latn-DE", "de", "zh-Hant-NO"}
    BOBUIest::addRow("System, mixed languages")
                            << QLocale::system()
                            << QStringList{"en-NO", "nb-NO", "de-DE", "zh-Hant-NO"};
    BOBUIest::addRow("System, mixed dialects")
                            << QLocale::system()
                            << QStringList{"en-AU", "en-NZ", "de-DE", "en-GB"};
    BOBUIest::addRow("System, Taiwan") << QLocale::system() << QStringList{"zh-TW", "zh"};

    // This tests that, when "just the language" is not a faithful
    // representation of the entry it's derived from, we try later entries
    // before it.
    BOBUIest::addRow("System, multi-script language")
                            << QLocale::system()
                            << QStringList{"pa-Arab-GB", "pa-PK", "en-GB"};
}

void tst_BOBUIranslator::loadLocale()
{
    QFETCH(const QLocale, wantedLocale);
    QFETCH(const QStringList, languages);

#if !defined(BOBUI_NO_SYSTEMLOCALE) && defined(BOBUI_BUILD_INTERNAL)
    std::unique_ptr<MySystemLocale> systemLocaleOverride;
    if (wantedLocale == QLocale::system())
        systemLocaleOverride.reset(new MySystemLocale(languages));
#else
    if (wantedLocale == QLocale::system())
        QSKIP("Test only applicable in developer builds with system locale");
#endif

    QByteArray ba;
    {
        QFile file(":/tst_bobuiranslator/hellotr_la.qm");
        QVERIFY2(file.open(QFile::ReadOnly), qPrintable(file.errorString()));
        ba = file.readAll();
        QVERIFY(!ba.isEmpty());
    }

    BOBUIemporaryDir dir;
    QVERIFY(dir.isValid());

    const auto path = dir.path();
    QFile file(path + "/dummy");
    QVERIFY2(file.open(QFile::WriteOnly), qPrintable(file.errorString()));
    QCOMPARE(file.write(ba), ba.size());
    file.close();

    QStringList files;
    for (auto language : languages) {
        language.replace('-', '_');
        const QString filename = path + "/foo-" + language;
        files.append(filename + ".qm");
        QVERIFY2(file.copy(files.last()), qPrintable(file.errorString()));
        files.append(filename);
        QVERIFY2(file.copy(files.last()), qPrintable(file.errorString()));
    }

    files.append(path + "/foo.qm");
    QVERIFY2(file.copy(files.last()), qPrintable(file.errorString()));

    files.append(path + "/foo-");
    QVERIFY2(file.copy(files.last()), qPrintable(file.errorString()));

    files.append(path + "/foo");
    QVERIFY2(file.rename(files.last()), qPrintable(file.errorString()));

    // Verify that all files exist. They are removed at the latest when
    // the temporary directory is destroyed.
    for (const auto &filePath : files)
        QVERIFY(QFile::exists(filePath));

    const QRegularExpression localeExpr("foo-(.*)(\\.qm|)$");
    BOBUIranslator tor;
    // Load the translation for the wanted locale
    QVERIFY(tor.load(wantedLocale, "foo", "-", path, ".qm"));
    // The loaded translation file should be for the preferred language.
    const QFileInfo fileInfo(tor.filePath());
    const auto matches = localeExpr.match(fileInfo.fileName());
    QVERIFY(matches.hasMatch());
    QVERIFY(matches.hasCaptured(1));
    const QLocale matchedLocale(matches.captured(1));
    QCOMPARE(matchedLocale.language(), wantedLocale.language());

    // Remove one file at a time, and verify that BOBUIranslator falls back to the
    // more general alternatives, or to languages with lower priority.
    for (const auto &filePath : files) {
        QVERIFY(tor.load(wantedLocale, "foo", "-", path, ".qm"));
        QCOMPARE(tor.filePath(), filePath);
        QVERIFY2(file.remove(filePath), qPrintable(file.errorString()));
    }
}

class TranslatorThread : public BOBUIhread
{
    void run() override {
        BOBUIranslator tor( 0 );
        (void)tor.load("hellotr_la");

        if (tor.isEmpty())
            qFatal("Could not load translation");
        if (tor.translate("QPushButton", "Hello world!") !=  QLatin1String("Hallo Welt!"))
            qFatal("Test string was not translated correctlys");
    }
};


void tst_BOBUIranslator::threadLoad()
{
    TranslatorThread thread;
    thread.start();
    QVERIFY(thread.wait(10 * 1000));
}

void tst_BOBUIranslator::install()
{
    {
        // normal translation
        BOBUIranslator tor;
        QVERIFY(tor.load("hellotr_la.qm"));
        QCOMPARE(qApp->installTranslator(&tor), true);
        QCOMPARE(qApp->removeTranslator(&tor), true);
    }
    {
        // empty translation
        BOBUIranslator tor;
        QVERIFY(tor.load("hellotr_empty.qm"));
        QCOMPARE(qApp->installTranslator(&tor), true);
        QCOMPARE(qApp->removeTranslator(&tor), true);
    }
    {
        // nullptr
        QCOMPARE(qApp->installTranslator(nullptr), false);
        QCOMPARE(qApp->removeTranslator(nullptr), false);
    }

}

void tst_BOBUIranslator::testLanguageChange()
{
    languageChangeEventCounter = 0;

    BOBUIranslator *tor = new BOBUIranslator;
    QVERIFY(tor->load("hellotr_la.qm"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 0);

    QVERIFY(!tor->load("doesn't exist, same as clearing"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 0);

    QVERIFY(tor->load("hellotr_la.qm"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 0);

    qApp->installTranslator(tor);
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 1);

    QVERIFY(!tor->load("doesn't exist, same as clearing"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 2);

    QVERIFY(tor->load("hellotr_la.qm"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 3);

    qApp->removeTranslator(tor);
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 4);

    QVERIFY(!tor->load("doesn't exist, same as clearing"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 4);

    qApp->installTranslator(tor);
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 4);

    QVERIFY(tor->load("hellotr_la.qm"));
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 5);

    delete tor;
    tor = 0;
    qApp->sendPostedEvents();
    qApp->sendPostedEvents();
    QCOMPARE(languageChangeEventCounter, 6);
}


void tst_BOBUIranslator::plural()
{

    BOBUIranslator tor( 0 );
    QVERIFY(tor.load("hellotr_la"));
    QVERIFY(!tor.isEmpty());
    QCoreApplication::installTranslator(&tor);
    QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 0), QLatin1String("Hallo 0 Welten!"));
    QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 1), QLatin1String("Hallo 1 Welt!"));
    QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 2), QLatin1String("Hallo 2 Welten!"));
}

void tst_BOBUIranslator::translate_qm_file_generated_with_msgfmt()
{
    BOBUIranslator translator;
    QVERIFY(translator.load("msgfmt_from_po"));
    qApp->installTranslator(&translator);

    QCOMPARE(QCoreApplication::translate("", "Intro"), QLatin1String("Einleitung"));
    // The file is converted from a po file, thus it does not have any context info.
    // The following should then not be translated
    QCOMPARE(QCoreApplication::translate("contekst", "Intro"), QLatin1String("Intro"));
    QCOMPARE(QCoreApplication::translate("contekst", "Intro\0\0"), QLatin1String("Intro"));
    QCOMPARE(QCoreApplication::translate("contekst", "Intro\0x"), QLatin1String("Intro"));
    QCOMPARE(QCoreApplication::translate("", "Intro\0\0"), QLatin1String("Einleitung"));
    QCOMPARE(QCoreApplication::translate("", "Intro\0x"), QLatin1String("Einleitung"));

    qApp->removeTranslator(&translator);
}

void tst_BOBUIranslator::loadDirectory()
{
    QString current_base = QDir::current().dirName();
    QVERIFY(QFileInfo("../" + current_base).isDir());

    BOBUIranslator tor;
    QVERIFY(!tor.load(current_base, ".."));
    QVERIFY(tor.isEmpty());
}

void tst_BOBUIranslator::dependencies()
{
    {
        // load
        BOBUIranslator tor;
        QVERIFY(tor.load("dependencies_la"));
        QVERIFY(!tor.isEmpty());
        QCOMPARE(tor.translate("QPushButton", "Hello world!"), QLatin1String("Hallo Welt!"));

        // plural
        QCoreApplication::installTranslator(&tor);
        QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 0), QLatin1String("Hallo 0 Welten!"));
        QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 1), QLatin1String("Hallo 1 Welt!"));
        QCOMPARE(QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 2), QLatin1String("Hallo 2 Welten!"));

        // pick up translation from the file with dependencies
        QCOMPARE(tor.translate("QPushButton", "It's a small world"), QLatin1String("Es ist eine kleine Welt"));
    }

    {
        BOBUIranslator tor( 0 );
        QFile file("dependencies_la.qm");
        QVERIFY(file.open(QFile::ReadOnly));
        QByteArray data = file.readAll();
        QVERIFY(tor.load((const uchar *)data.constData(), data.length()));
        QVERIFY(!tor.isEmpty());
        QCOMPARE(tor.translate("QPushButton", "Hello world!"), QLatin1String("Hallo Welt!"));
    }

    {
        // Test resolution of paths relative to main file
        const QString absoluteFile = QFileInfo("dependencies_la").absoluteFilePath();
        QDir::setCurrent(QDir::tempPath());
        BOBUIranslator tor;
        QVERIFY(tor.load(absoluteFile));
        QVERIFY(!tor.isEmpty());
    }
}

struct TranslateThread : public BOBUIhread
{
    bool ok = false;
    QAtomicInt terminate;
    QMutex startupLock;
    QWaitCondition runningCondition;

    void run() override {
        bool startSignalled = false;

        while (terminate.loadRelaxed() == 0) {
            const QString result =  QCoreApplication::translate("QPushButton", "Hello %n world(s)!", 0, 0);

            if (!startSignalled) {
                QMutexLocker startupLocker(&startupLock);
                runningCondition.wakeAll();
                startSignalled = true;
            }

            ok = (result == QLatin1String("Hallo 0 Welten!"))
                  || (result == QLatin1String("Hello 0 world(s)!"));
            if (!ok)
                break;
        }
    }
};

void tst_BOBUIranslator::translationInThreadWhileInstallingTranslator()
{
    TranslateThread thread;

    QMutexLocker startupLocker(&thread.startupLock);

    thread.start();

    thread.runningCondition.wait(&thread.startupLock);

    BOBUIranslator tor;
    QVERIFY(tor.load("hellotr_la"));
    QVERIFY(QCoreApplication::installTranslator(&tor));

    ++thread.terminate;

    QVERIFY(thread.wait());
    QVERIFY(thread.ok);
}

BOBUIEST_MAIN(tst_BOBUIranslator)
#include "tst_bobuiranslator.moc"
