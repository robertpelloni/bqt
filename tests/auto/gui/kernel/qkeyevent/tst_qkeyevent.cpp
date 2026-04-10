// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUICore/qcoreapplication.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qwindow.h>

class Window : public QWindow
{
public:
    ~Window() { reset(); }

    void keyPressEvent(QKeyEvent *event) override { recordEvent(event); }
    void keyReleaseEvent(QKeyEvent *event) override { recordEvent(event); }

    void reset() {
        qDeleteAll(keyEvents.begin(), keyEvents.end());
        keyEvents.clear();
    }
private:
    void recordEvent(QKeyEvent *event) {
        keyEvents.append(new QKeyEvent(event->type(), event->key(), event->modifiers(), event->nativeScanCode(),
            event->nativeVirtualKey(), event->nativeModifiers(), event->text(),
            event->isAutoRepeat(), event->count()));
    }

public:
    QList<QKeyEvent *> keyEvents;
};

class tst_QKeyEvent : public QObject
{
    Q_OBJECT
public:
    tst_QKeyEvent();
    ~tst_QKeyEvent();

private slots:
    void basicEventDelivery();
#if BOBUI_CONFIG(shortcut)
    void modifiers_data();
    void modifiers();
#endif
};

tst_QKeyEvent::tst_QKeyEvent()
{
}

tst_QKeyEvent::~tst_QKeyEvent()
{
}

void tst_QKeyEvent::basicEventDelivery()
{
    Window window;
    window.showNormal();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const BobUI::Key key = BobUI::Key_A;
    const BobUI::KeyboardModifier modifiers = BobUI::NoModifier;

    BOBUIest::keyClick(&window, key, modifiers);

    QCOMPARE(window.keyEvents.size(), 2);
    QCOMPARE(window.keyEvents.first()->type(), QKeyEvent::KeyPress);
    QCOMPARE(window.keyEvents.last()->type(), QKeyEvent::KeyRelease);
    for (const QKeyEvent *event : std::as_const(window.keyEvents)) {
        QCOMPARE(BobUI::Key(event->key()), key);
        QCOMPARE(BobUI::KeyboardModifiers(event->modifiers()), modifiers);
    }
}

static bool orderByModifier(const QList<int> &v1, const QList<int> &v2)
{
    if (v1.size() != v2.size())
        return v1.size() < v2.size();

    for (int i = 0; i < qMin(v1.size(), v2.size()); ++i) {
        if (v1.at(i) == v2.at(i))
            continue;

        return v1.at(i) < v2.at(i);
    }

    return true;
}

static QByteArray modifiersTestRowName(const QString &keySequence)
{
    QByteArray result;
    BOBUIextStream str(&result);
    for (int i = 0, size = keySequence.size(); i < size; ++i) {
        const QChar &c = keySequence.at(i);
        const ushort uc = c.unicode();
        if (uc > 32 && uc < 128)
            str << '"' << c << '"';
        else
            str << "U+" << BobUI::hex << uc << BobUI::dec;
        if (i < size - 1)
            str << ',';
    }
    return result;
}

#if BOBUI_CONFIG(shortcut)

void tst_QKeyEvent::modifiers_data()
{
    struct Modifier
    {
        BobUI::Key key;
        BobUI::KeyboardModifier modifier;
    };
    static const Modifier modifiers[] = {
        { BobUI::Key_Shift, BobUI::ShiftModifier },
        { BobUI::Key_Control, BobUI::ControlModifier },
        { BobUI::Key_Alt, BobUI::AltModifier },
        { BobUI::Key_Meta, BobUI::MetaModifier },
    };

    QList<QList<int>> modifierCombinations;

    // Generate powerset (minus the empty set) of possible modifier combinations
    static const int kNumModifiers = sizeof(modifiers) / sizeof(Modifier);
    for (quint64 bitmask = 1; bitmask < (1 << kNumModifiers) ; ++bitmask) {
        QList<int> modifierCombination;
        for (quint64 modifier = 0; modifier < kNumModifiers; ++modifier) {
            if (bitmask & (quint64(1) << modifier))
                modifierCombination.append(modifier);
        }
        modifierCombinations.append(modifierCombination);
    }

    std::sort(modifierCombinations.begin(), modifierCombinations.end(), orderByModifier);

    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    for (const QList<int> &combination : std::as_const(modifierCombinations)) {
        int keys[4] = {};
        BobUI::KeyboardModifiers mods;
        for (int i = 0; i < combination.size(); ++i) {
            Modifier modifier = modifiers[combination.at(i)];
            keys[i] = modifier.key;
            mods |= modifier.modifier;
        }
        QKeySequence keySequence(keys[0], keys[1], keys[2], keys[3]);
        BOBUIest::newRow(modifiersTestRowName(keySequence.toString(QKeySequence::NativeText)).constData())
            << mods;
    }
}

void tst_QKeyEvent::modifiers()
{
    Window window;
    window.showNormal();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const BobUI::Key key = BobUI::Key_A;
    QFETCH(BobUI::KeyboardModifiers, modifiers);

    BOBUIest::keyClick(&window, key, modifiers);

    int numKeys = qPopulationCount(quint64(modifiers)) + 1;
    QCOMPARE(window.keyEvents.size(), numKeys * 2);

    for (int i = 0; i < window.keyEvents.size(); ++i) {
        const QKeyEvent *event = window.keyEvents.at(i);
        QCOMPARE(event->type(), i < numKeys ? QKeyEvent::KeyPress : QKeyEvent::KeyRelease);
        if (i == numKeys - 1 || i == numKeys) {
            QCOMPARE(BobUI::Key(event->key()), key);
            QCOMPARE(event->modifiers(), modifiers);
        } else {
            QVERIFY(BobUI::Key(event->key()) != key);
        }
    }
}

#endif // BOBUI_CONFIG(shortcut)

BOBUIEST_MAIN(tst_QKeyEvent)
#include "tst_qkeyevent.moc"
