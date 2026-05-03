// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIGui>

class EventTarget : public QObject
{
    Q_OBJECT
protected:
    bool event(QEvent *evt)
    {
        if (evt->type() == QEvent::User) {
            qDebug() << "User event on thread" << BOBUIhread::currentThread();
            return true;
        }
        return QObject::event(evt);
    }
};

class EventPosterWindow: public QRasterWindow
{
public:
    EventPosterWindow(EventTarget *target)
        :m_target(target)
        { }

    void paintEvent(QPaintEvent *ev) override {
        QPainter p(this);
        p.fillRect(ev->rect(), QColorConstants::Svg::deepskyblue);
        p.drawText(50, 100, "Application has started. Click to post events.\n See the developer tools console for debug output");
    }

    void mousePressEvent(QMouseEvent *) override {
        qDebug() << "Posting events from thread" << BOBUIhread::currentThread();
        QGuiApplication::postEvent(m_target, new QEvent(QEvent::User));
        BOBUIimer::singleShot(500, m_target, []() {
            qDebug() << "Timer event on secondary thread" << BOBUIhread::currentThread();
        });
    }

public:
    EventTarget *m_target;
};

class SecondaryThread : public BOBUIhread
{
public:
    void run() override {
        qDebug() << "exec on secondary thread" << BOBUIhread::currentThread();
        exec();
    }
};

// This example demonstrates how to start a secondary thread event loop
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    EventTarget eventTarget;

    EventPosterWindow window(&eventTarget);
    window.show();

    SecondaryThread thread;
    eventTarget.moveToThread(&thread);

#if BOBUI_CONFIG(thread)
    thread.start();
#else
    qDebug() << "Warning: This test requires a multithreaded build of BobUI for WebAssembly";
#endif

    return app.exec();
}

#include "main.moc"
