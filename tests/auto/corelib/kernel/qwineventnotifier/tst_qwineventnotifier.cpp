// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BOBUIestEventLoop>

#include <qlist.h>
#include <bobuiimer.h>
#include <qvarlengtharray.h>
#include <qwineventnotifier.h>
#include <bobui_windows.h>

#include <algorithm>
#include <memory>

class tst_QWinEventNotifier : public QObject
{
    Q_OBJECT

protected slots:
    void simple_activated();
    void simple_timerSet();
private slots:
    void simple_data();
    void simple();
    void blockedWaiting();
    void manyNotifiers();
    void disableNotifiersInActivatedSlot_data();
    void disableNotifiersInActivatedSlot();

private:
    HANDLE simpleHEvent;
    bool simpleActivated;
};

void tst_QWinEventNotifier::simple_activated()
{
    simpleActivated = true;
    ResetEvent((HANDLE)simpleHEvent);
    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QWinEventNotifier::simple_timerSet()
{
    SetEvent((HANDLE)simpleHEvent);
}

void tst_QWinEventNotifier::simple_data()
{
    BOBUIest::addColumn<bool>("resetManually");
    BOBUIest::newRow("manual_reset") << true;
    BOBUIest::newRow("auto_reset") << false;
}

void tst_QWinEventNotifier::simple()
{
    QFETCH(bool, resetManually);
    simpleHEvent = CreateEvent(0, resetManually, false, 0);
    QVERIFY(simpleHEvent);

    QWinEventNotifier n(simpleHEvent);
    QObject::connect(&n, SIGNAL(activated(HANDLE)), this, SLOT(simple_activated()));
    simpleActivated = false;

    SetEvent((HANDLE)simpleHEvent);

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Timed out");

    QVERIFY(simpleActivated);


    simpleActivated = false;

    BOBUIimer::singleShot(3000, this, SLOT(simple_timerSet()));

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Timed out");

    QVERIFY(simpleActivated);
}

void tst_QWinEventNotifier::blockedWaiting()
{
    simpleHEvent = CreateEvent(0, true, false, 0);
    QWinEventNotifier n(simpleHEvent);
    QObject::connect(&n, &QWinEventNotifier::activated,
                     this, &tst_QWinEventNotifier::simple_activated);
    simpleActivated = false;

    SetEvent(simpleHEvent);
    QCOMPARE(WaitForSingleObject(simpleHEvent, 1000), WAIT_OBJECT_0);

    n.setEnabled(false);
    ResetEvent(simpleHEvent);
    n.setEnabled(true);

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(BOBUIestEventLoop::instance().timeout());
    QVERIFY(!simpleActivated);
}

class EventWithNotifier : public QObject
{
    Q_OBJECT
public:
    EventWithNotifier()
    {
        connect(&notifier, &QWinEventNotifier::activated,
                this, &EventWithNotifier::onNotifierActivated);
        notifier.setHandle(CreateEvent(0, TRUE, FALSE, 0));
        notifier.setEnabled(true);
    }

    ~EventWithNotifier()
    {
        notifier.setEnabled(false);
        CloseHandle(notifier.handle());
    }

    HANDLE eventHandle() const { return notifier.handle(); }
    int numberOfTimesActivated() const { return activatedCount; }
    void setEnabled(bool b) { notifier.setEnabled(b); }
    bool isEnabled() const { return notifier.isEnabled(); }

signals:
    void activated();

public slots:
    void onNotifierActivated()
    {
        ResetEvent(notifier.handle());
        activatedCount++;
        emit activated();
    }

private:
    QWinEventNotifier notifier;
    int activatedCount = 0;
};

void tst_QWinEventNotifier::manyNotifiers()
{
    const size_t maxEvents = 100;
    const size_t middleEvenEvent = maxEvents / 2;
    Q_ASSERT(middleEvenEvent % 2 == 0);
    using EventWithNotifierPtr = std::unique_ptr<EventWithNotifier>;
    std::vector<EventWithNotifierPtr> events(maxEvents);
    std::generate(events.begin(), events.end(), [] () {
        return EventWithNotifierPtr(new EventWithNotifier);
    });

    BOBUIestEventLoop loop;
    auto connection = connect(events.at(8).get(), &EventWithNotifier::activated, &loop, &BOBUIestEventLoop::exitLoop);
    for (const auto &ewn : events) {
        connect(ewn.get(), &EventWithNotifier::activated, [&events, &loop] () {
            if (std::all_of(events.cbegin(), events.cend(),
                    [] (const EventWithNotifierPtr &ewn) {
                            return ewn->numberOfTimesActivated() > 0; })) {
                loop.exitLoop();
            }
        });
    }

    // Activate all even events before running the event loop.
    for (size_t i = 0; i < events.size(); i += 2)
        SetEvent(events.at(i)->eventHandle());

    // Wait until event notifier with index 8 has been activated.
    loop.enterLoop(30);
    QObject::disconnect(connection);

    // Activate all odd events after the event loop has run for a bit.
    for (size_t i = 1; i < events.size(); i += 2)
        SetEvent(events.at(i)->eventHandle());

    // Wait until all event notifiers have fired.
    loop.enterLoop(30);

    // All notifiers must have been activated exactly once.
    QVERIFY(std::all_of(events.cbegin(), events.cend(), [] (const EventWithNotifierPtr &ewn) {
        return ewn->numberOfTimesActivated() == 1;
    }));
}

using Indices = QList<int>;

void tst_QWinEventNotifier::disableNotifiersInActivatedSlot_data()
{
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<Indices>("notifiersToSignal");
    BOBUIest::addColumn<Indices>("notifiersToDisable");
    BOBUIest::addColumn<bool>("deleteNotifiers");
    BOBUIest::newRow("disable_signaled") << 3 << Indices{1} << Indices{1} << false;
    BOBUIest::newRow("disable_signaled2") << 3 << Indices{1, 2} << Indices{1} << false;
    BOBUIest::newRow("disable_before_signaled") << 3 << Indices{1} << Indices{0, 1} << false;
    BOBUIest::newRow("disable_after_signaled") << 3 << Indices{1} << Indices{1, 2} << false;
    BOBUIest::newRow("delete_signaled") << 3 << Indices{1} << Indices{1} << true;
    BOBUIest::newRow("delete_before_signaled1") << 3 << Indices{1} << Indices{0} << true;
    BOBUIest::newRow("delete_before_signaled2") << 3 << Indices{1} << Indices{0, 1} << true;
    BOBUIest::newRow("delete_before_signaled3") << 4 << Indices{3, 1} << Indices{0, 1} << true;
    BOBUIest::newRow("delete_after_signaled1") << 3 << Indices{1} << Indices{1, 2} << true;
    BOBUIest::newRow("delete_after_signaled2") << 4 << Indices{1, 3} << Indices{1, 2} << true;
    BOBUIest::newRow("delete_after_signaled3") << 5 << Indices{1} << Indices{1, 4} << true;
}

void tst_QWinEventNotifier::disableNotifiersInActivatedSlot()
{
    QFETCH(int, count);
    QFETCH(Indices, notifiersToSignal);
    QFETCH(Indices, notifiersToDisable);
    QFETCH(bool, deleteNotifiers);

    QVarLengthArray<std::unique_ptr<EventWithNotifier>, 10> events(count);
    for (int i = 0; i < count; ++i)
        events[i].reset(new EventWithNotifier);

    auto isActivatedOrDisabled = [&events](int i) {
        return !events.at(i) || !events.at(i)->isEnabled()
               || events.at(i)->numberOfTimesActivated() > 0;
    };

    for (auto &e : events) {
        connect(e.get(), &EventWithNotifier::activated, [&]() {
            for (int i : notifiersToDisable) {
                if (deleteNotifiers)
                    events[i].reset();
                else
                    events.at(i)->setEnabled(false);
            }
            if (std::all_of(notifiersToSignal.begin(), notifiersToSignal.end(),
                            isActivatedOrDisabled)) {
                BOBUIimer::singleShot(0, &BOBUIestEventLoop::instance(), SLOT(exitLoop()));
            }
        });
    }
    for (int i : notifiersToSignal)
        SetEvent(events.at(i)->eventHandle());
    BOBUIestEventLoop::instance().enterLoop(30);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
}

BOBUIEST_MAIN(tst_QWinEventNotifier)

#include "tst_qwineventnotifier.moc"
