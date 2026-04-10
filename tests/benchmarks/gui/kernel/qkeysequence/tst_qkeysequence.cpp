// Copyright (C) 2025 Vlad Zahorodnii <vlad.zahorodnii@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BobUIGui/qkeysequence.h>

class tst_QKeySequence : public QObject
{
    Q_OBJECT

private slots:
    void fromString_data();
    void fromString();
};

void tst_QKeySequence::fromString_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QKeySequence::SequenceFormat>("format");

    BOBUIest::newRow("empty (portable)") << QString() << QKeySequence::PortableText;
    BOBUIest::newRow("empty (native)") << QString() << QKeySequence::NativeText;

    BOBUIest::newRow("invalid (portable)") << QStringLiteral("pizza") << QKeySequence::PortableText;
    BOBUIest::newRow("invalid (native)") << QStringLiteral("pizza") << QKeySequence::NativeText;

    BOBUIest::newRow("F (portable)") << QStringLiteral("F") << QKeySequence::PortableText;
    BOBUIest::newRow("F (native)") << QStringLiteral("F") << QKeySequence::NativeText;

    BOBUIest::newRow("Ctrl+F (portable)") << QStringLiteral("Ctrl+F") << QKeySequence::PortableText;
    BOBUIest::newRow("Ctrl+F (native)") << QStringLiteral("Ctrl+F") << QKeySequence::NativeText;

    BOBUIest::newRow("Meta+Ctrl+Alt+Shift+F (portable)") << QStringLiteral("Meta+Ctrl+Alt+Shift+F") << QKeySequence::PortableText;
    BOBUIest::newRow("Meta+Ctrl+Alt+Shift+F (native)") << QStringLiteral("Meta+Ctrl+Alt+Shift+F") << QKeySequence::NativeText;
}

void tst_QKeySequence::fromString()
{
    QFETCH(QString, text);
    QFETCH(QKeySequence::SequenceFormat, format);

    QBENCHMARK {
        auto sequence = QKeySequence::fromString(text, format);
        Q_UNUSED(sequence)
    }
}

BOBUIEST_MAIN(tst_QKeySequence)
#include "tst_qkeysequence.moc"
