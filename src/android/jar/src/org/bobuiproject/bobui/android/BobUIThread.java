// Copyright (C) 2018 BogDan Vatra <bogdan@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import java.util.ArrayList;
import java.util.concurrent.Semaphore;

class BobUIThread {
    private final ArrayList<Runnable> m_pendingRunnables = new ArrayList<>();
    private boolean m_exit = false;
    private final Thread m_bobuiThread = new Thread(new Runnable() {
        @Override
        public void run() {
            while (!m_exit) {
                try {
                    ArrayList<Runnable> pendingRunnables;
                    synchronized (m_bobuiThread) {
                        if (m_pendingRunnables.isEmpty())
                            m_bobuiThread.wait();
                        pendingRunnables = new ArrayList<>(m_pendingRunnables);
                        m_pendingRunnables.clear();
                    }
                    for (Runnable runnable : pendingRunnables)
                        runnable.run();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });

    BobUIThread() {
        m_bobuiThread.setName("bobuiMainLoopThread");
        m_bobuiThread.start();
    }

    void post(final Runnable runnable) {
        synchronized (m_bobuiThread) {
            m_pendingRunnables.add(runnable);
            m_bobuiThread.notify();
        }
    }

    void sleep(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void run(final Runnable runnable) {
        final Semaphore sem = new Semaphore(0);
        synchronized (m_bobuiThread) {
            m_pendingRunnables.add(() -> {
                runnable.run();
                sem.release();
            });
            m_bobuiThread.notify();
        }
        try {
            sem.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void exit()
    {
        m_exit = true;
        synchronized (m_bobuiThread) {
            m_bobuiThread.notify();
        }
        try {
            m_bobuiThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    boolean isAlive()
    {
        return m_bobuiThread.isAlive();
    }
}
