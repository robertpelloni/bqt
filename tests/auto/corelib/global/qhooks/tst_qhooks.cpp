// Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Volker Krause <volker.krause@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BobUICore/private/qhooks_p.h>

class tst_QHooks: public QObject
{
    Q_OBJECT

private slots:
    void cleanup();
    void testVersion();
    void testAddRemoveObject();
    void testChaining();
};

void tst_QHooks::cleanup()
{
    bobuiHookData[QHooks::AddQObject] = 0;
    bobuiHookData[QHooks::RemoveQObject] = 0;
}

void tst_QHooks::testVersion()
{
    QVERIFY(bobuiHookData[QHooks::HookDataVersion] >= 3);
    QCOMPARE(bobuiHookData[QHooks::HookDataSize], (quintptr)QHooks::LastHookIndex);
    QCOMPARE(bobuiHookData[QHooks::BobUIVersion], (quintptr)BOBUI_VERSION);
}

static int objectCount = 0;

static void objectAddHook(QObject*)
{
    ++objectCount;
}

static void objectRemoveHook(QObject*)
{
    --objectCount;
}

void tst_QHooks::testAddRemoveObject()
{
    QCOMPARE(bobuiHookData[QHooks::AddQObject], (quintptr)0);
    QCOMPARE(bobuiHookData[QHooks::RemoveQObject], (quintptr)0);

    bobuiHookData[QHooks::AddQObject] = (quintptr)&objectAddHook;
    bobuiHookData[QHooks::RemoveQObject] = (quintptr)&objectRemoveHook;

    QCOMPARE(objectCount, 0);
    QObject *obj = new QObject;
    QVERIFY(objectCount > 0);
    delete obj;
    QCOMPARE(objectCount, 0);
}

static QList<QString> hookOrder;

static QHooks::AddQObjectCallback existingAddHook = 0;
static QHooks::RemoveQObjectCallback existingRemoveHook = 0;

static void firstAddHook(QObject *)
{
    hookOrder.append(QLatin1String("firstAddHook"));
}

static void firstRemoveHook(QObject *)
{
    hookOrder.append(QLatin1String("firstRemoveHook"));
}

static void secondAddHook(QObject *object)
{
    if (existingAddHook)
        existingAddHook(object);

    hookOrder.append(QLatin1String("secondAddHook"));
}

static void secondRemoveHook(QObject *object)
{
    if (existingRemoveHook)
        existingRemoveHook(object);

    hookOrder.append(QLatin1String("secondRemoveHook"));
}

// Tests that it's possible to "chain" hooks together (i.e. have multiple hooks)
void tst_QHooks::testChaining()
{
    QCOMPARE(bobuiHookData[QHooks::AddQObject], (quintptr)0);
    QCOMPARE(bobuiHookData[QHooks::RemoveQObject], (quintptr)0);

    // Set the add and remove hooks (could just skip this and go straight to the next step,
    // but it's for illustrative purposes).
    bobuiHookData[QHooks::AddQObject] = (quintptr)&firstAddHook;
    bobuiHookData[QHooks::RemoveQObject] = (quintptr)&firstRemoveHook;

    // Store them so that we can call them later.
    existingAddHook = reinterpret_cast<QHooks::AddQObjectCallback>(bobuiHookData[QHooks::AddQObject]);
    existingRemoveHook = reinterpret_cast<QHooks::RemoveQObjectCallback>(bobuiHookData[QHooks::RemoveQObject]);

    // Overide them with hooks that call them first.
    bobuiHookData[QHooks::AddQObject] = (quintptr)&secondAddHook;
    bobuiHookData[QHooks::RemoveQObject] = (quintptr)&secondRemoveHook;

    QObject *obj = new QObject;
    QCOMPARE(hookOrder.size(), 2);
    QCOMPARE(hookOrder.at(0), QLatin1String("firstAddHook"));
    QCOMPARE(hookOrder.at(1), QLatin1String("secondAddHook"));
    delete obj;
    QCOMPARE(hookOrder.size(), 4);
    QCOMPARE(hookOrder.at(2), QLatin1String("firstRemoveHook"));
    QCOMPARE(hookOrder.at(3), QLatin1String("secondRemoveHook"));

    hookOrder.clear();
}

BOBUIEST_APPLESS_MAIN(tst_QHooks)
#include "tst_qhooks.moc"
