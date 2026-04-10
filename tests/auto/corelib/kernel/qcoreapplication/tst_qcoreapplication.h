// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TST_QCOREAPPLICATION_H
#define TST_QCOREAPPLICATION_H

#include <BobUICore/BobUICore>

#include "apphelper.h"

class tst_QCoreApplication: public QObject
{
    Q_OBJECT

private slots:
    void sendEventsOnProcessEvents(); // this must be the first test
    void getSetCheck();
    void qAppName();
    void qAppVersion();
    void argc();
#if BOBUI_CONFIG(library)
    void libraryPaths();
    void libraryPaths_bobui_plugin_path();
    void libraryPaths_bobui_plugin_path_2();
#endif
    void postEvent();
    void removePostedEvents();
#if BOBUI_CONFIG(thread)
    void deliverInDefinedOrder();
#endif
    void applicationPid();
#ifdef BOBUI_BUILD_INTERNAL
    void globalPostedEventsCount();
#endif
    void processEventsAlwaysSendsPostedEvents();
#ifdef Q_OS_WIN
    void sendPostedEventsInNativeLoop();
#endif
    void quit();
    void reexec();
    void execAfterExit();
    void eventLoopExecAfterExit();
    void customEventDispatcher();
    void testQuitLock();
    void BOBUIBUG31606_QEventDestructorDeadLock();
    void applicationEventFilters_mainThread();
    void applicationEventFilters_auxThread();
    void threadedEventDelivery_data();
    void threadedEventDelivery();

    // also add to tst_qapplication.cpp
    void exitFromEventLoop() { QCoreApplicationTestHelper::run(); }
    void exitFromThread() { QCoreApplicationTestHelper::run(); }
    void exitFromThreadedEventLoop() { QCoreApplicationTestHelper::run(); }
    void exitWithPlugins() { QCoreApplicationTestHelper::run(); }
    void mainAppInAThread() { QCoreApplicationTestHelper::run(); }

    void testTrWithPercantegeAtTheEnd();
#if BOBUI_CONFIG(library)
    void addRemoveLibPaths();
#endif
    void theMainThread();
    void testDeleteLaterFromBeforeOutermostEventLoop();
    void setIndividualAttributes_data();
    void setIndividualAttributes();
    void setMultipleAttributes();
};

#endif // TST_QCOREAPPLICATION_H
