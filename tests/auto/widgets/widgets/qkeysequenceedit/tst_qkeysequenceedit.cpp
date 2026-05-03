// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QString>

Q_DECLARE_METATYPE(BobUI::Key)
Q_DECLARE_METATYPE(BobUI::KeyboardModifiers)

class tst_QKeySequenceEdit : public QObject
{
    Q_OBJECT

private slots:
    void testSetters();
    void testKeys_data();
    void testKeys();
    void testLineEditContents();
    void testMaximumSequenceLength();
    void testFinishingKeyCombinations_data();
    void testFinishingKeyCombinations();
};

void tst_QKeySequenceEdit::testSetters()
{
    QKeySequenceEdit edit;
    QSignalSpy spy(&edit, SIGNAL(keySequenceChanged(QKeySequence)));
    QCOMPARE(edit.keySequence(), QKeySequence());

    edit.setKeySequence(QKeySequence::New);
    QCOMPARE(edit.keySequence(), QKeySequence(QKeySequence::New));

    edit.clear();
    QCOMPARE(edit.keySequence(), QKeySequence());

    QCOMPARE(spy.size(), 2);
}

void tst_QKeySequenceEdit::testKeys_data()
{
    BOBUIest::addColumn<BobUI::Key>("key");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<QKeySequence>("keySequence");

    BOBUIest::newRow("1") << BobUI::Key_N << BobUI::KeyboardModifiers(BobUI::ControlModifier) << QKeySequence("Ctrl+N");
    BOBUIest::newRow("2") << BobUI::Key_N << BobUI::KeyboardModifiers(BobUI::AltModifier) << QKeySequence("Alt+N");
    BOBUIest::newRow("3") << BobUI::Key_N << BobUI::KeyboardModifiers(BobUI::ShiftModifier) << QKeySequence("Shift+N");
    BOBUIest::newRow("4") << BobUI::Key_N << BobUI::KeyboardModifiers(BobUI::ControlModifier  | BobUI::ShiftModifier) << QKeySequence("Ctrl+Shift+N");
}

void tst_QKeySequenceEdit::testMaximumSequenceLength()
{
    //
    // GIVEN:
    //  - A QKeySequenceEdit with maxKeyCount == 1
    //  - A QKeySequence with more than one key
    //
    QKeySequenceEdit edit;
    edit.setMaximumSequenceLength(1);
    QCOMPARE(edit.maximumSequenceLength(), 1);

    QKeySequence multi("Ctrl+X, S");
    QCOMPARE(multi.count(), 2);

    //
    // WHEN: setting the key sequence on the edit
    //
    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "QKeySequenceEdit: setting a key sequence of length 2 when "
                         "maximumSequenceLength is 1, truncating.");
    edit.setKeySequence(multi);

    //
    // THEN:
    //  - the maxKeyCount property doesn't change
    //  - the key sequence is truncated to maxKeyCount
    //  - and won't un-truncate by increasing maxKeyCount
    //
    QCOMPARE(edit.maximumSequenceLength(), 1);
    const auto edited = edit.keySequence();
    QCOMPARE(edited.count(), 1);
    QCOMPARE(edited, QKeySequence("Ctrl+X"));
    edit.setMaximumSequenceLength(2);
    QCOMPARE(edit.keySequence(), edited);
}

void tst_QKeySequenceEdit::testKeys()
{
    QFETCH(BobUI::Key, key);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(QKeySequence, keySequence);
    QKeySequenceEdit edit;

    QSignalSpy spy(&edit, SIGNAL(editingFinished()));
    BOBUIest::keyPress(&edit, key, modifiers);
    BOBUIest::keyRelease(&edit, key, modifiers);

    QCOMPARE(spy.size(), 0);
    QCOMPARE(edit.keySequence(), keySequence);
    BOBUIRY_COMPARE(spy.size(), 1);
}

void tst_QKeySequenceEdit::testLineEditContents()
{
    QKeySequenceEdit edit;
    QLineEdit *le = edit.findChild<QLineEdit*>();
    QVERIFY(le);

    QCOMPARE(le->text(), QString());

    edit.setKeySequence(QKeySequence::New);
    QCOMPARE(edit.keySequence(), QKeySequence(QKeySequence::New));

    edit.clear();
    QCOMPARE(le->text(), QString());

    edit.setKeySequence(QKeySequence::New);
    QVERIFY(le->text() != QString());

    edit.setKeySequence(QKeySequence());
    QCOMPARE(le->text(), QString());
}

void tst_QKeySequenceEdit::testFinishingKeyCombinations_data()
{
    BOBUIest::addColumn<BobUI::Key>("key");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<QKeySequence>("keySequence");

    BOBUIest::newRow("1") << BobUI::Key_Backtab << BobUI::KeyboardModifiers(BobUI::NoModifier) << QKeySequence("Backtab");
    BOBUIest::newRow("2") << BobUI::Key_Tab << BobUI::KeyboardModifiers(BobUI::NoModifier) << QKeySequence("Tab");
    BOBUIest::newRow("3") << BobUI::Key_Return << BobUI::KeyboardModifiers(BobUI::NoModifier) << QKeySequence("Return");
    BOBUIest::newRow("4") << BobUI::Key_Enter << BobUI::KeyboardModifiers(BobUI::NoModifier) << QKeySequence("Enter");
    BOBUIest::newRow("5") << BobUI::Key_Enter << BobUI::KeyboardModifiers(BobUI::ShiftModifier) << QKeySequence("Shift+Enter");
}

void tst_QKeySequenceEdit::testFinishingKeyCombinations()
{
    QFETCH(BobUI::Key, key);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(QKeySequence, keySequence);
    QKeySequenceEdit edit;

    QSignalSpy spy(&edit, SIGNAL(editingFinished()));
    QCOMPARE(spy.size(), 0);

    edit.setFinishingKeyCombinations({QKeyCombination(modifiers, key)});
    BOBUIest::keyPress(&edit, key, modifiers);
    BOBUIest::keyRelease(&edit, key, modifiers);

    QCOMPARE(edit.keySequence(), QKeySequence());
    BOBUIRY_COMPARE(spy.size(), 1);

    edit.setFinishingKeyCombinations({});
    BOBUIest::keyPress(&edit, key, modifiers);
    BOBUIest::keyRelease(&edit, key, modifiers);

    QCOMPARE(edit.keySequence(), keySequence);
    BOBUIRY_COMPARE(spy.size(), 2);
}

BOBUIEST_MAIN(tst_QKeySequenceEdit)
#include "tst_qkeysequenceedit.moc"
