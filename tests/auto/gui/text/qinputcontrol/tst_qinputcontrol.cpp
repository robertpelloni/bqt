// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <private/qinputcontrol_p.h>
#include <BobUIGui/QKeyEvent>

class tst_QInputControl: public QObject
{
    Q_OBJECT
private slots:
    void isAcceptableInput_data();
    void isAcceptableInput();
    void tabOnlyAcceptableInputForTextEdit();
};

void tst_QInputControl::isAcceptableInput_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<bool>("acceptable");

    BOBUIest::newRow("empty-string") << QString() << BobUI::KeyboardModifiers() << false;
    BOBUIest::newRow("zwnj") << QString(QChar(0x200C)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("zwnj-with-ctrl") << QString(QChar(0x200C)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("zwnj-with-ctrl-shift") << QString(QChar(0x200C)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("zwj") << QString(QChar(0x200D)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("zwj-with-ctrl") << QString(QChar(0x200D)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("zwj-with-ctrl-shift") << QString(QChar(0x200D)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("printable-latin") << QString(QLatin1Char('a')) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("printable-latin-with-ctrl") << QString(QLatin1Char('a')) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << false;
    BOBUIest::newRow("printable-latin-with-ctrl-shift") << QString(QLatin1Char('a')) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << false;
    BOBUIest::newRow("printable-hebrew") << QString(QChar(0x2135)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("private-use-area") << QString(QChar(0xE832)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("good-surrogate-0") << QString::fromUtf16(u"\U0001F44D") << BobUI::KeyboardModifiers() << true;
    {
        const QChar data[] = { QChar(0xD800), QChar(0xDC00) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("good-surrogate-1") << str << BobUI::KeyboardModifiers() << true;
    }
    {
        const QChar data[] = { QChar(0xD800), QChar(0xDFFF) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("good-surrogate-2") << str << BobUI::KeyboardModifiers() << true;
    }
    {
        const QChar data[] = { QChar(0xDBFF), QChar(0xDC00) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("good-surrogate-3") << str << BobUI::KeyboardModifiers() << true;
    }
    {
        const QChar data[] = { QChar(0xDBFF), QChar(0xDFFF) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("good-surrogate-4") << str << BobUI::KeyboardModifiers() << true;
    }
    {
        const QChar data[] = { QChar(0xD7FF), QChar(0xDC00) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("bad-surrogate-1") << str << BobUI::KeyboardModifiers() << false;
    }
    {
        const QChar data[] = { QChar(0xD7FF), QChar(0xDFFF) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("bad-surrogate-2") << str << BobUI::KeyboardModifiers() << false;
    }
    {
        const QChar data[] = { QChar(0xDC00), QChar(0xDC00) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("bad-surrogate-3") << str << BobUI::KeyboardModifiers() << false;
    }
    {
        const QChar data[] = { QChar(0xD800), QChar(0xE000) };
        const QString str = QString(data, 2);
        BOBUIest::newRow("bad-surrogate-4") << str << BobUI::KeyboardModifiers() << false;
    }
    {
        const QChar data[] = { QChar(0xD800) };
        const QString str = QString(data, 1);
        BOBUIest::newRow("bad-surrogate-5") << str << BobUI::KeyboardModifiers() << false;
    }
    BOBUIest::newRow("multiple-printable") << QStringLiteral("foobar") << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("rlm") << QString(QChar(0x200F)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("rlm-with-ctrl") << QString(QChar(0x200F)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("rlm-with-ctrl-shift") << QString(QChar(0x200F)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("lrm") << QString(QChar(0x200E)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("lrm-with-ctrl") << QString(QChar(0x200E)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("lrm-with-ctrl-shift") << QString(QChar(0x200E)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("rlo") << QString(QChar(0x202E)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("rlo-with-ctrl") << QString(QChar(0x202E)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("rlo-with-ctrl-shift") << QString(QChar(0x202E)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("lro") << QString(QChar(0x202D)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("lro-with-ctrl") << QString(QChar(0x202D)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("lro-with-ctrl-shift") << QString(QChar(0x202D)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("lre") << QString(QChar(0x202B)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("lre-with-ctrl") << QString(QChar(0x202B)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("lre-with-ctrl-shift") << QString(QChar(0x202B)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("rle") << QString(QChar(0x202A)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("rle-with-ctrl") << QString(QChar(0x202A)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("rle-with-ctrl-shift") << QString(QChar(0x202A)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;
    BOBUIest::newRow("pdf") << QString(QChar(0x202C)) << BobUI::KeyboardModifiers() << true;
    BOBUIest::newRow("pdf-with-ctrl") << QString(QChar(0x202C)) << BobUI::KeyboardModifiers(BobUI::ControlModifier) << true;
    BOBUIest::newRow("pdf-with-ctrl-shift") << QString(QChar(0x202C)) << BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier) << true;

}

void tst_QInputControl::isAcceptableInput()
{
    QFETCH(QString, text);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(bool, acceptable);

    QKeyEvent keyEvent(QKeyEvent::KeyPress, BobUI::Key_unknown, modifiers, text);

    {
        QInputControl inputControl(QInputControl::TextEdit);
        QCOMPARE(inputControl.isAcceptableInput(&keyEvent), acceptable);
    }

    {
        QInputControl inputControl(QInputControl::LineEdit);
        QCOMPARE(inputControl.isAcceptableInput(&keyEvent), acceptable);
    }
}

void tst_QInputControl::tabOnlyAcceptableInputForTextEdit()
{
    QKeyEvent keyEvent(QKeyEvent::KeyPress, BobUI::Key_unknown, BobUI::KeyboardModifiers(), QLatin1String("\t"));

    {
        QInputControl inputControl(QInputControl::TextEdit);
        QCOMPARE(inputControl.isAcceptableInput(&keyEvent), true);
    }

    {
        QInputControl inputControl(QInputControl::LineEdit);
        QCOMPARE(inputControl.isAcceptableInput(&keyEvent), false);
    }
}

BOBUIEST_MAIN(tst_QInputControl)
#include "tst_qinputcontrol.moc"

