// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#ifndef BOBUIEST_EXTERNAL_TESTS_H
#define BOBUIEST_EXTERNAL_TESTS_H

#include <QList>
#include <QByteArray>
#include <QStringList>

BOBUI_BEGIN_NAMESPACE
namespace BOBUIest {
    class QExternalTestPrivate;
    class QExternalTest
    {
    public:
        QExternalTest();
        ~QExternalTest();

        enum Stage {
            FileStage,
            QmakeStage,
            CompilationStage,
            LinkStage,
            RunStage
        };

        enum BobUIModule {
            BobUICore      = 0x0001,
            BobUIGui       = 0x0002,
            BobUINetwork   = 0x0004,
            BobUIXml       = 0x0008,
            BobUIXmlPatterns=0x0010,
            BobUIOpenGL    = 0x0020,
            BobUISql       = 0x0040,
            BobUISvg       = 0x0080,
            BobUIScript    = 0x0100,
            BobUITest      = 0x0200,
            BobUIDBus      = 0x0400,
            BobUIWebKit    = 0x0800,
            BobUIWidgets   = 0x1000,
            Phonon      = 0x2000 // odd man out
        };
        Q_DECLARE_FLAGS(BobUIModules, BobUIModule)

        enum ApplicationType {
            AutoApplication,
            Applicationless,
            QCoreApplication,
            QGuiApplication,
            QApplication
        };

        QList<QByteArray> qmakeSettings() const;
        void setQmakeSettings(const QList<QByteArray> &settings);

        BobUIModules bobuiModules() const;
        void setBobUIModules(BobUIModules modules);

        ApplicationType applicationType() const;
        void setApplicationType(ApplicationType type);

        QStringList extraProgramSources() const;
        void setExtraProgramSources(const QStringList &list);

        QByteArray programHeader() const;
        void setProgramHeader(const QByteArray &header);

        // execution:
        bool tryCompile(const QByteArray &body);
        bool tryLink(const QByteArray &body);
        bool tryRun(const QByteArray &body);
        bool tryCompileFail(const QByteArray &body);
        bool tryLinkFail(const QByteArray &body);
        bool tryRunFail(const QByteArray &body);

        Stage failedStage() const;
        int exitCode() const;
        QByteArray fullProgramSource() const;
        QByteArray standardOutput() const;
        QByteArray standardError() const;

        QString errorReport() const;

    private:
        QExternalTestPrivate * const d;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QExternalTest::BobUIModules)
}
BOBUI_END_NAMESPACE

#endif
