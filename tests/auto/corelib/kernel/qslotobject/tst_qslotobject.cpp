// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qobject.h>

#include <BobUICore/qscopedvaluerollback.h>

#include <BobUITest/bobuiest.h>

class tst_QSlotObject : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void uniquePtr();
    void sharedPtr();
};

void tst_QSlotObject::uniquePtr()
{
    using Prototype = void(*)();
    bool exists = false;
    BobUIPrivate::SlotObjUniquePtr p;
    QVERIFY(!p);
    auto rb = std::make_unique<QScopedValueRollback<bool>>(exists, true); // make movable
    p.reset(BobUIPrivate::makeCallableObject<Prototype>([rb = std::move(rb)] {}));
    QVERIFY(p);
    QVERIFY(exists);
    p.reset();
    QVERIFY(!p);
    QVERIFY(!exists);
}

void tst_QSlotObject::sharedPtr()
{
    using Prototype = void(*)();
    bool exists = false;
    BobUIPrivate::SlotObjUniquePtr p;
    auto rb = std::make_unique<QScopedValueRollback<bool>>(exists, true); // make movable
    p.reset(BobUIPrivate::makeCallableObject<Prototype>([rb = std::move(rb)] {}));
    QVERIFY(p);
    QVERIFY(exists);

    BobUIPrivate::SlotObjSharedPtr sp{std::move(p)};
    QVERIFY(!p);
    QVERIFY(exists);

    {
        const auto copy = sp;
        QVERIFY(sp);
        QVERIFY(copy);
        QVERIFY(exists);

        sp = nullptr; // SlotObjSharedPtr doesn't have a reset()...
        QVERIFY(!sp);
        QVERIFY(copy);
        QVERIFY(exists);
    } // `copy` goes out of scope

    QVERIFY(!exists);
}

BOBUIEST_APPLESS_MAIN(tst_QSlotObject)
#include "tst_qslotobject.moc"
