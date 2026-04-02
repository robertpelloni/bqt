#include <QtTest/QtTest>
#include <QObject>
#include <QKeyEvent>
#include "../../omnicore/include/OmniInputManager.h"

// A dummy target to verify event reception
class MockTarget : public QObject {
    Q_OBJECT
public:
    MockTarget() : receivedKeyCount(0) {}

    bool event(QEvent *e) override {
        if (e->type() == QEvent::KeyPress) {
            auto *ke = static_cast<QKeyEvent*>(e);
            lastReceivedKey = ke->key();
            receivedKeyCount++;
            return true;
        }
        return QObject::event(e);
    }

    int receivedKeyCount;
    int lastReceivedKey;
};

class tst_OmniInputManager : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testDeviceRegistration();
    void testIndependentFocusTrees();
    void testConcurrentEventRouting();
    void cleanupTestCase();

private:
    OmniInputManager* m_manager;
};

void tst_OmniInputManager::initTestCase() {
    m_manager = OmniInputManager::instance();
}

void tst_OmniInputManager::testDeviceRegistration() {
    m_manager->registerDevice("test-kb-1", "Test Keyboard 1", "keyboard");
    m_manager->registerDevice("test-kb-2", "Test Keyboard 2", "keyboard");

    bool foundKb1 = false;
    bool foundKb2 = false;

    for (const auto& dev : m_manager->devices()) {
        if (dev.id == "test-kb-1") foundKb1 = true;
        if (dev.id == "test-kb-2") foundKb2 = true;
    }

    QVERIFY(foundKb1);
    QVERIFY(foundKb2);
}

void tst_OmniInputManager::testIndependentFocusTrees() {
    MockTarget targetA;
    MockTarget targetB;

    m_manager->setDeviceFocus("test-kb-1", &targetA);
    m_manager->setDeviceFocus("test-kb-2", &targetB);

    QCOMPARE(m_manager->deviceFocus("test-kb-1"), &targetA);
    QCOMPARE(m_manager->deviceFocus("test-kb-2"), &targetB);

    // Verify that changing one doesn't affect the other
    MockTarget targetC;
    m_manager->setDeviceFocus("test-kb-1", &targetC);

    QCOMPARE(m_manager->deviceFocus("test-kb-1"), &targetC);
    QCOMPARE(m_manager->deviceFocus("test-kb-2"), &targetB); // B should remain untouched
}

void tst_OmniInputManager::testConcurrentEventRouting() {
    MockTarget targetA;
    MockTarget targetB;

    m_manager->setDeviceFocus("test-kb-1", &targetA);
    m_manager->setDeviceFocus("test-kb-2", &targetB);

    // Simulate User A typing 'A' (Qt::Key_A = 0x41)
    QKeyEvent eventA(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    bool routedA = m_manager->routeKeyEvent("test-kb-1", &eventA);

    QVERIFY(routedA);
    QCOMPARE(targetA.receivedKeyCount, 1);
    QCOMPARE(targetA.lastReceivedKey, Qt::Key_A);
    QCOMPARE(targetB.receivedKeyCount, 0); // User B should receive nothing

    // Simulate User B typing 'B' (Qt::Key_B = 0x42)
    QKeyEvent eventB(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier, "B");
    bool routedB = m_manager->routeKeyEvent("test-kb-2", &eventB);

    QVERIFY(routedB);
    QCOMPARE(targetB.receivedKeyCount, 1);
    QCOMPARE(targetB.lastReceivedKey, Qt::Key_B);
    QCOMPARE(targetA.receivedKeyCount, 1); // User A should remain at 1
}

void tst_OmniInputManager::cleanupTestCase() {
    // The manager is a singleton, no deletion needed here
}

QTEST_MAIN(tst_OmniInputManager)
#include "tst_OmniInputManager.moc"
