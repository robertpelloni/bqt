// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/qpropertytesthelper_p.h>
#include <QSignalSpy>

#include <bobuiimeline.h>

class tst_BOBUIimeLine : public QObject
{
  Q_OBJECT
private slots:
    void range();
    void currentTime();
    void bindableCurrentTime();
    void duration();
    void bindableDuration();
    void frameRate();
    void bindableUpdateInterval();
    void value();
    void currentFrame();
    void loopCount();
    void bindableLoopCount();
    void interpolation();
    void reverse_data();
    void reverse();
    void toggleDirection();
    void bindableDirection();
    void frameChanged();
    void stopped();
    void finished();
    void isRunning();
    void multipleTimeLines();
    void sineCurve();
    void cosineCurve();
    void outOfRange();
    void stateInFinishedSignal();
    void resume();
    void restart();
    void setPaused();
    void automatedBindableTests();

protected slots:
    void finishedSlot();

protected:
    BOBUIimeLine::State state;
    BOBUIimeLine * view;
};

void tst_BOBUIimeLine::range()
{
    BOBUIimeLine timeLine(200);
    QCOMPARE(timeLine.startFrame(), 0);
    QCOMPARE(timeLine.endFrame(), 0);
    timeLine.setFrameRange(0, 1);
    QCOMPARE(timeLine.startFrame(), 0);
    QCOMPARE(timeLine.endFrame(), 1);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.startFrame(), 10);
    QCOMPARE(timeLine.endFrame(), 20);

    timeLine.setStartFrame(6);
    QCOMPARE(timeLine.startFrame(), 6);
    timeLine.setEndFrame(16);
    QCOMPARE(timeLine.endFrame(), 16);

    // Verify that you can change the range in the timeLine
    timeLine.setFrameRange(1000, 2000);
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();  // make sure that the logic works for a running timeline
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::Running);
    timeLine.setCurrentTime(timeLine.duration()/2);
    int oldValue = timeLine.currentFrame();
    timeLine.setFrameRange(0, 500);
    QVERIFY(timeLine.currentFrame() < oldValue);
    timeLine.setEndFrame(10000);
    timeLine.setStartFrame(5000);
    QVERIFY(timeLine.currentFrame() > oldValue);
    timeLine.setFrameRange(0, 500);
    BOBUIRY_VERIFY(spy.size() > 1);
    QVERIFY(timeLine.currentFrame() < oldValue);
}

void tst_BOBUIimeLine::currentTime()
{
    BOBUIimeLine timeLine(2000);
    timeLine.setUpdateInterval((timeLine.duration()/2) / 33);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.currentTime(), 0);
    timeLine.start();
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentTime() > timeLine.duration()/2 - timeLine.duration()/4);
    QVERIFY(timeLine.currentTime() < timeLine.duration()/2 + timeLine.duration()/4);
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), timeLine.duration());

    QSignalSpy spy(&timeLine, &BOBUIimeLine::valueChanged);
    QVERIFY(spy.isValid());
    spy.clear();
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.setCurrentTime(timeLine.duration()/2);
    QCOMPARE(spy.size(), 1);
    spy.clear();
    QCOMPARE(timeLine.currentTime(), timeLine.duration()/2);
    timeLine.resume();
    // Let it update on its own
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentTime() > timeLine.duration()/2);
    QVERIFY(timeLine.currentTime() < timeLine.duration());
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), timeLine.duration());

    // Reverse should decrease the currentTime
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.start();
    // Let it update on its own
    int currentTime = timeLine.currentTime();
    BOBUIRY_VERIFY(timeLine.currentTime() > currentTime);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    currentTime = timeLine.currentTime();
    timeLine.setDirection(BOBUIimeLine::Backward);
    BOBUIRY_VERIFY(timeLine.currentTime() < currentTime);
    timeLine.stop();
}

void tst_BOBUIimeLine::bindableCurrentTime()
{
    BOBUIimeLine timeLine(2000);
    QProperty<int> currentTimeObserver([&]() { return timeLine.currentTime(); });

    timeLine.setUpdateInterval((timeLine.duration() / 2) / 33);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.currentTime(), 0);
    QCOMPARE(currentTimeObserver.value(), 0);
    QCOMPARE(currentTimeObserver.value(), timeLine.currentTime());

    timeLine.start();
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::Running);
    QCOMPARE(currentTimeObserver.value(), timeLine.currentTime());
    BOBUIRY_VERIFY(timeLine.currentTime() > timeLine.duration() / 2 - timeLine.duration() / 4);
    QVERIFY(timeLine.currentTime() < timeLine.duration() / 2 + timeLine.duration() / 4);
    QCOMPARE(currentTimeObserver.value(), timeLine.currentTime());

    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), timeLine.duration());
    QCOMPARE(currentTimeObserver.value(), timeLine.currentTime());

    QSignalSpy spy(&timeLine, &BOBUIimeLine::valueChanged);
    QVERIFY(spy.isValid());
    spy.clear();
    QProperty<int> referenceCurrentTime(timeLine.duration() / 2);
    timeLine.bindableCurrentTime().setBinding([&]() { return referenceCurrentTime.value(); });
    QCOMPARE(spy.size(), 1);
    // setting it a second time to check that valueChanged() is emitted only once
    referenceCurrentTime = timeLine.duration() / 2;
    QCOMPARE(spy.size(), 1);

    spy.clear();
    QCOMPARE(timeLine.currentTime(), timeLine.duration() / 2);
    QCOMPARE(currentTimeObserver.value(), timeLine.duration() / 2);
    timeLine.resume();
    // Let it update on its own
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(currentTimeObserver.value() > timeLine.duration() / 2);
    QVERIFY(currentTimeObserver.value() < timeLine.duration());
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(currentTimeObserver.value(), timeLine.duration());
    // the resume above should have broken the connection to referenceCurrentTime, check that:
    spy.clear();
    referenceCurrentTime = 0;
    QCOMPARE(currentTimeObserver.value(), timeLine.duration());
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIimeLine::duration()
{
    BOBUIimeLine timeLine(200);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.duration(), 200);
    timeLine.setDuration(1000);
    QCOMPARE(timeLine.duration(), 1000);

    timeLine.start();
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentTime() > 0);
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), 1000);
    // The duration shouldn't change
    QCOMPARE(timeLine.duration(), 1000);
}

void tst_BOBUIimeLine::bindableDuration()
{
    BOBUIimeLine timeLine(200);
    QProperty<int> durationObserver;
    durationObserver.setBinding([&]() { return timeLine.duration(); });
    QCOMPARE(durationObserver.value(), timeLine.duration());

    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.duration(), 200);

    QProperty<int> referenceDuration(500);
    timeLine.bindableDuration().setBinding([&]() { return referenceDuration.value(); });
    QCOMPARE(durationObserver.value(), referenceDuration.value());

    QCOMPARE(timeLine.duration(), 500);

    timeLine.start();
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentTime() > 0);
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), 500);
    // The duration shouldn't change
    QCOMPARE(timeLine.duration(), 500);

    referenceDuration = 30;
    QCOMPARE(timeLine.duration(), 30);
    QCOMPARE(durationObserver.value(), 30);
}

void tst_BOBUIimeLine::frameRate()
{
    BOBUIimeLine timeLine;
    timeLine.setFrameRange(100, 2000);
    QCOMPARE(timeLine.updateInterval(), 1000 / 25);
    timeLine.setUpdateInterval(1000 / 60);
    QCOMPARE(timeLine.updateInterval(), 1000 / 60);

    // Default speed
    timeLine.setUpdateInterval(1000 / 33);
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    int slowCount = spy.size();

    // Faster!!
    timeLine.setUpdateInterval(1000 / 100);
    spy.clear();
    timeLine.setCurrentTime(0);
    timeLine.start();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QVERIFY2(slowCount < spy.size(), QByteArray::number(spy.size()));
}

void tst_BOBUIimeLine::bindableUpdateInterval()
{
    BOBUIimeLine timeLine;
    timeLine.setFrameRange(100, 2000);

    QProperty<int> updateIntervalObserver;
    updateIntervalObserver.setBinding([&]() { return timeLine.updateInterval(); });

    QCOMPARE(updateIntervalObserver.value(), 1000 / 25);
    QProperty<int> updateIntervalReference(1000 / 60);
    timeLine.bindableUpdateInterval().setBinding([&]() { return updateIntervalReference.value(); });

    updateIntervalReference = 1000 / 60;
    QCOMPARE(updateIntervalObserver.value(), 1000 / 60);

    // Default speed
    updateIntervalReference = 1000 / 33;
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIest::qWait(timeLine.duration() * 2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    int slowCount = spy.size();

    // Faster!!
    updateIntervalReference = 1000 / 100;
    spy.clear();
    timeLine.setCurrentTime(0);
    timeLine.start();
    BOBUIest::qWait(timeLine.duration() * 2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QVERIFY2(slowCount < spy.size(), QByteArray::number(spy.size()));
}

void tst_BOBUIimeLine::value()
{
    BOBUIimeLine timeLine(4500); // Should be at least 5% under 5000ms
    QCOMPARE(timeLine.currentValue(), 0.0);

    // Default speed
    QSignalSpy spy(&timeLine, &BOBUIimeLine::valueChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIRY_VERIFY(timeLine.currentValue() > 0);
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentValue(), 1.0);
    QVERIFY(spy.size() > 0);

    // Reverse should decrease the value
    timeLine.setCurrentTime(100);
    timeLine.start();
    // Let it update on its own
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentValue());
    qreal value = timeLine.currentValue();
    timeLine.setDirection(BOBUIimeLine::Backward);
    BOBUIRY_VERIFY(timeLine.currentValue() < value);
    timeLine.stop();
}

void tst_BOBUIimeLine::currentFrame()
{
    BOBUIimeLine timeLine(2000);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.currentFrame(), 10);

    // Default speed
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIRY_VERIFY(timeLine.currentFrame() > 10);
    BOBUIRY_COMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(timeLine.currentFrame(), 20);

    // Reverse should decrease the value
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.start();
    // Let it update on its own
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    BOBUIRY_VERIFY(timeLine.currentTime() > timeLine.duration()/2); // wait for continuation
    int value = timeLine.currentFrame();
    timeLine.setDirection(BOBUIimeLine::Backward);
    BOBUIRY_VERIFY(timeLine.currentFrame() < value);
    timeLine.stop();
}

void tst_BOBUIimeLine::loopCount()
{
    BOBUIimeLine timeLine(200);
    QCOMPARE(timeLine.loopCount(), 1);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.loopCount(), 1);
    timeLine.setLoopCount(0);
    QCOMPARE(timeLine.loopCount(), 0);

    // Default speed endless looping
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIest::qWait(timeLine.duration());
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    //QCOMPARE(timeLine.currentFrame(), 20);
    BOBUIest::qWait(timeLine.duration()*6);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QVERIFY(timeLine.currentTime() >= 0);
    QVERIFY(timeLine.currentFrame() >= 10);
    QVERIFY(timeLine.currentFrame() <= 20);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    timeLine.stop();

    timeLine.setDuration(2500); // some platforms have a very low resolution timer
    timeLine.setFrameRange(0, 2);
    timeLine.setLoopCount(4);

    QSignalSpy finishedSpy(&timeLine, &BOBUIimeLine::finished);
    QSignalSpy frameChangedSpy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(frameChangedSpy.isValid());
    QEventLoop loop;
    connect(&timeLine, SIGNAL(finished()), &loop, SLOT(quit()));


    for(int i=0;i<2;i++) {

        timeLine.start();
        // we clear the list after the start so we don't catch
        // a frameChanged signal for the frame 0 at the beginning
        finishedSpy.clear();
        frameChangedSpy.clear();

        loop.exec();

        QCOMPARE(finishedSpy.size(), 1);
        QCOMPARE(frameChangedSpy.size(), 11);
        for (int i = 0; i < 11; ++i)
            QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), (i+1) % 3);
    }

    timeLine.setDirection(BOBUIimeLine::Backward);
    timeLine.start();
    loop.exec();

    QCOMPARE(finishedSpy.size(), 2);
    QCOMPARE(frameChangedSpy.size(), 22);
    for (int i = 11; i < 22; ++i) {
        QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), 2 - (i+2) % 3);
    }
}

void tst_BOBUIimeLine::bindableLoopCount()
{
    BOBUIimeLine timeLine(200);
    QProperty<int> referenceLoopCount(1);
    timeLine.bindableLoopCount().setBinding([&]() { return referenceLoopCount.value(); });
    QProperty<int> loopCountObserver([&]() { return timeLine.loopCount(); });

    QCOMPARE(referenceLoopCount.value(), 1);
    QCOMPARE(timeLine.loopCount(), 1);
    QCOMPARE(loopCountObserver.value(), 1);

    timeLine.setFrameRange(10, 20);

    QCOMPARE(referenceLoopCount.value(), 1);
    QCOMPARE(timeLine.loopCount(), 1);
    QCOMPARE(loopCountObserver.value(), 1);

    referenceLoopCount = 0;

    QCOMPARE(referenceLoopCount.value(), 0);
    QCOMPARE(timeLine.loopCount(), 0);
    QCOMPARE(loopCountObserver.value(), 0);

    // Default speed endless looping
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIest::qWait(timeLine.duration());
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    // QCOMPARE(timeLine.currentFrame(), 20);
    BOBUIest::qWait(timeLine.duration() * 6);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QVERIFY(timeLine.currentTime() >= 0);
    QVERIFY(timeLine.currentFrame() >= 10);
    QVERIFY(timeLine.currentFrame() <= 20);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    timeLine.stop();

    timeLine.setDuration(2500); // some platforms have a very low resolution timer
    timeLine.setFrameRange(0, 2);
    referenceLoopCount = 4;

    QSignalSpy finishedSpy(&timeLine, &BOBUIimeLine::finished);
    QSignalSpy frameChangedSpy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(finishedSpy.isValid());
    QVERIFY(frameChangedSpy.isValid());
    QEventLoop loop;
    connect(&timeLine, SIGNAL(finished()), &loop, SLOT(quit()));

    for (int i = 0; i < 2; i++) {

        timeLine.start();
        // we clear the list after the start so we don't catch
        // a frameChanged signal for the frame 0 at the beginning
        finishedSpy.clear();
        frameChangedSpy.clear();

        loop.exec();

        QCOMPARE(finishedSpy.size(), 1);
        QCOMPARE(frameChangedSpy.size(), 11);
        for (int i = 0; i < 11; ++i)
            QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), (i + 1) % 3);
    }

    timeLine.setDirection(BOBUIimeLine::Backward);
    timeLine.start();
    loop.exec();

    QCOMPARE(finishedSpy.size(), 2);
    QCOMPARE(frameChangedSpy.size(), 22);
    for (int i = 11; i < 22; ++i)
        QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), 2 - (i + 2) % 3);
}

void tst_BOBUIimeLine::interpolation()
{
    // also tests bindableEasingCurve
    BOBUIimeLine timeLine(400);
    QProperty<QEasingCurve> easingCurveObserver([&]() { return timeLine.easingCurve(); });

    QCOMPARE(timeLine.easingCurve(), QEasingCurve::InOutSine);
    QCOMPARE(easingCurveObserver.value(), QEasingCurve::InOutSine);

    timeLine.setFrameRange(100, 200);
    QProperty<QEasingCurve> referenceEasingCurve(QEasingCurve::Linear);
    timeLine.bindableEasingCurve().setBinding([&]() { return referenceEasingCurve.value(); });
    QCOMPARE(timeLine.easingCurve(), QEasingCurve::Linear);
    QCOMPARE(easingCurveObserver.value(), QEasingCurve::Linear);

    // smooth
    referenceEasingCurve = QEasingCurve::InOutSine;
    QCOMPARE(timeLine.easingCurve(), QEasingCurve::InOutSine);
    QCOMPARE(easingCurveObserver.value(), QEasingCurve::InOutSine);

    timeLine.start();
    BOBUIest::qWait(100);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    int firstValue = timeLine.currentFrame();
    BOBUIest::qWait(200);
    int endValue = timeLine.currentFrame();
    timeLine.stop();
    timeLine.setCurrentTime(0);

    // linear
    referenceEasingCurve = QEasingCurve::Linear;

    QCOMPARE(timeLine.easingCurve(), QEasingCurve::Linear);
    QCOMPARE(easingCurveObserver.value(), QEasingCurve::Linear);

    timeLine.start();
    BOBUIest::qWait(100);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);

    // Smooth accellerates slowly so in the beginning so it is farther behind
    if (firstValue >= timeLine.currentFrame())
        QEXPECT_FAIL("", "BOBUIBUG-24796: BOBUIimeLine exhibits inconsistent timing behaviour", Abort);
    QVERIFY(firstValue < timeLine.currentFrame());
    BOBUIest::qWait(200);
    QVERIFY(endValue > timeLine.currentFrame());
    timeLine.stop();
}

void tst_BOBUIimeLine::reverse_data()
{
    BOBUIest::addColumn<int>("duration");
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<int>("end");
    BOBUIest::addColumn<int>("direction");
    BOBUIest::addColumn<int>("direction2");
    BOBUIest::addColumn<int>("direction3");
    BOBUIest::addColumn<int>("startTime");
    BOBUIest::addColumn<int>("currentFrame");
    BOBUIest::addColumn<qreal>("currentValue");
    BOBUIest::addColumn<int>("wait");
    BOBUIest::addColumn<int>("state");
    BOBUIest::addColumn<int>("wait2");

    BOBUIest::newRow("start at end") << 200 << 1000 << 2000 << (int)BOBUIimeLine::Backward << (int)BOBUIimeLine::Forward << (int)BOBUIimeLine::Backward << 200 << 2000 << qreal(1.0) << 40 << (int)BOBUIimeLine::Running << 140;
    BOBUIest::newRow("start at half") << 200 << 1000 << 2000 << (int)BOBUIimeLine::Backward << (int)BOBUIimeLine::Forward << (int)BOBUIimeLine::Backward << 100 << 1500 << qreal(0.5) << 40 << (int)BOBUIimeLine::Running << 140;
    BOBUIest::newRow("start at quarter") << 200 << 1000 << 2000 << (int)BOBUIimeLine::Backward << (int)BOBUIimeLine::Forward << (int)BOBUIimeLine::Backward << 50 << 1250 << qreal(0.25) << 40 << (int)BOBUIimeLine::Running << 140;
}

void tst_BOBUIimeLine::reverse()
{
    QFETCH(int, duration);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(int, direction);
    QFETCH(int, direction2);
    QFETCH(int, direction3);
    QFETCH(int, startTime);
    QFETCH(int, currentFrame);
    QFETCH(qreal, currentValue);
    QFETCH(int, wait);
    QFETCH(int, state);
    QFETCH(int, wait2);

    BOBUIimeLine timeLine(duration);
    timeLine.setEasingCurve(QEasingCurve::Linear);
    timeLine.setFrameRange(start, end);

    timeLine.setDirection((BOBUIimeLine::Direction)direction);
    timeLine.setDirection((BOBUIimeLine::Direction)direction2);
    timeLine.setDirection((BOBUIimeLine::Direction)direction3);
    QCOMPARE(timeLine.direction(), ((BOBUIimeLine::Direction)direction));

    timeLine.setCurrentTime(startTime);
    timeLine.setDirection((BOBUIimeLine::Direction)direction);
    timeLine.setDirection((BOBUIimeLine::Direction)direction2);
    timeLine.setDirection((BOBUIimeLine::Direction)direction3);

    QCOMPARE(timeLine.currentFrame(), currentFrame);
    QCOMPARE(timeLine.currentValue(), currentValue);
    timeLine.start();

    BOBUIest::qWait(wait);
    QCOMPARE(timeLine.state(), (BOBUIimeLine::State)state);
    int firstValue = timeLine.currentFrame();
    timeLine.setDirection((BOBUIimeLine::Direction)direction2);
    timeLine.setDirection((BOBUIimeLine::Direction)direction3);
    timeLine.setDirection((BOBUIimeLine::Direction)direction2);
    timeLine.setDirection((BOBUIimeLine::Direction)direction3);
    BOBUIest::qWait(wait2);
    int endValue = timeLine.currentFrame();
    QVERIFY(endValue < firstValue);


}

void tst_BOBUIimeLine::toggleDirection()
{
    BOBUIimeLine timeLine;
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Forward);
    timeLine.toggleDirection();
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Backward);
    timeLine.toggleDirection();
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Forward);
}

void tst_BOBUIimeLine::bindableDirection()
{
    // Note: enum values are cast to int so that QCOMPARE will show
    // the values if they don't match.
    BOBUIimeLine timeLine;
    QProperty<BOBUIimeLine::Direction> directionObserver([&]() { return timeLine.direction(); });
    QProperty<BOBUIimeLine::Direction> referenceDirection(BOBUIimeLine::Forward);
    timeLine.bindableDirection().setBinding([&]() { return referenceDirection.value(); });

    QCOMPARE(referenceDirection.value(), BOBUIimeLine::Forward);
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Forward);
    QCOMPARE(directionObserver.value(), BOBUIimeLine::Forward);

    referenceDirection = BOBUIimeLine::Backward;

    QCOMPARE(referenceDirection.value(), BOBUIimeLine::Backward);
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Backward);
    QCOMPARE(directionObserver.value(), BOBUIimeLine::Backward);

    referenceDirection = BOBUIimeLine::Forward;

    QCOMPARE(referenceDirection.value(), BOBUIimeLine::Forward);
    QCOMPARE(timeLine.direction(), BOBUIimeLine::Forward);
    QCOMPARE(directionObserver.value(), BOBUIimeLine::Forward);
}

void tst_BOBUIimeLine::frameChanged()
{
    BOBUIimeLine timeLine;
    timeLine.setEasingCurve(QEasingCurve::Linear);
    timeLine.setFrameRange(0,9);
    timeLine.setUpdateInterval(800);
    QSignalSpy spy(&timeLine, &BOBUIimeLine::frameChanged);
    QVERIFY(spy.isValid());

    // Test what happens when duration expires before all frames are emitted.
    timeLine.start();
    BOBUIest::qWait(timeLine.duration()/2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QCOMPARE(spy.size(), 0);
    BOBUIest::qWait(timeLine.duration());
    if (timeLine.state() != BOBUIimeLine::NotRunning)
        QEXPECT_FAIL("", "BOBUIBUG-24796: BOBUIimeLine runs slower than it should", Abort);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    if (spy.size() != 1)
        QEXPECT_FAIL("", "BOBUIBUG-24796: BOBUIimeLine runs slower than it should", Abort);
    QCOMPARE(spy.size(), 1);

    // Test what happens when the frames are all emitted well before duration expires.
    timeLine.setUpdateInterval(5);
    spy.clear();
    timeLine.setCurrentTime(0);
    timeLine.start();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(spy.size(), 10);
}

void tst_BOBUIimeLine::stopped()
{
    BOBUIimeLine timeLine;
    timeLine.setFrameRange(0, 9);
    qRegisterMetaType<BOBUIimeLine::State>("BOBUIimeLine::State");
    QSignalSpy spy(&timeLine, &BOBUIimeLine::stateChanged);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    QCOMPARE(spy.size(), 2);
    spy.clear();
    timeLine.start();
    timeLine.stop();
    QCOMPARE(spy.size(), 2);
    timeLine.setDirection(BOBUIimeLine::Backward);
    QCOMPARE(timeLine.loopCount(), 1);
}

void tst_BOBUIimeLine::finished()
{
    BOBUIimeLine timeLine;
    timeLine.setFrameRange(0,9);
    QSignalSpy spy(&timeLine, &BOBUIimeLine::finished);
    QVERIFY(spy.isValid());
    timeLine.start();
    BOBUIRY_COMPARE(spy.size(), 1);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);

    spy.clear();
    timeLine.start();
    timeLine.stop();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIimeLine::isRunning()
{
    BOBUIimeLine timeLine;
    timeLine.setFrameRange(0,9);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
    timeLine.start();
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    timeLine.stop();
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);

    timeLine.start();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);
}

void tst_BOBUIimeLine::multipleTimeLines()
{
    // Stopping a timer shouldn't affect the other timers
    BOBUIimeLine timeLine(200);
    timeLine.setFrameRange(0,99);
    QSignalSpy spy(&timeLine, &BOBUIimeLine::finished);
    QVERIFY(spy.isValid());

    BOBUIimeLine timeLineKiller;
    timeLineKiller.setFrameRange(0,99);

    timeLineKiller.start();
    timeLine.start();
    timeLineKiller.stop();
    BOBUIest::qWait(timeLine.duration()*2);
    QCOMPARE(spy.size(), 1);
}

void tst_BOBUIimeLine::sineCurve()
{
    BOBUIimeLine timeLine(1000);
    timeLine.setEasingCurve(QEasingCurve::SineCurve);
    QCOMPARE(timeLine.valueForTime(0), qreal(0));
    QCOMPARE(timeLine.valueForTime(250), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(500), qreal(1));
    QCOMPARE(timeLine.valueForTime(750), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(1000), qreal(0));
}

void tst_BOBUIimeLine::cosineCurve()
{
    BOBUIimeLine timeLine(1000);
    timeLine.setEasingCurve(QEasingCurve::CosineCurve);
    QCOMPARE(timeLine.valueForTime(0), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(250), qreal(1));
    QCOMPARE(timeLine.valueForTime(500), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(750), qreal(0));
    QCOMPARE(timeLine.valueForTime(1000), qreal(0.5));
}

void tst_BOBUIimeLine::outOfRange()
{
    BOBUIimeLine timeLine(1000);
    QCOMPARE(timeLine.valueForTime(-100), qreal(0));
    QCOMPARE(timeLine.valueForTime(2000), qreal(1));

    timeLine.setEasingCurve(QEasingCurve::SineCurve);
    QCOMPARE(timeLine.valueForTime(2000), qreal(0));
}

void tst_BOBUIimeLine::stateInFinishedSignal()
{
    BOBUIimeLine timeLine(50);

    connect(&timeLine, SIGNAL(finished()), this, SLOT(finishedSlot()));
    state = BOBUIimeLine::State(-1);

    timeLine.start();
    BOBUIest::qWait(250);

    QCOMPARE(state, BOBUIimeLine::NotRunning);
}

void tst_BOBUIimeLine::finishedSlot()
{
    BOBUIimeLine *timeLine = qobject_cast<BOBUIimeLine *>(sender());
    if (timeLine)
        state = timeLine->state();
}

void tst_BOBUIimeLine::resume()
{
    BOBUIimeLine timeLine(1000);
    {
        QCOMPARE(timeLine.currentTime(), 0);
        timeLine.start();
        BOBUIRY_VERIFY(timeLine.currentTime() > 0);
        timeLine.stop();
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime > 0);
        QVERIFY(oldCurrentTime < 1000);
        timeLine.resume();
        BOBUIRY_VERIFY(timeLine.currentTime() > oldCurrentTime);
        timeLine.stop();
        int currentTime = timeLine.currentTime();
        QVERIFY(currentTime < 1000);
    }
    timeLine.setDirection(BOBUIimeLine::Backward);
    {
        timeLine.setCurrentTime(1000);
        QCOMPARE(timeLine.currentTime(), 1000);
        timeLine.start();
        BOBUIRY_VERIFY(timeLine.currentTime() < 1000);
        timeLine.stop();
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime < 1000);
        QVERIFY(oldCurrentTime > 0);
        timeLine.resume();
        BOBUIRY_VERIFY(timeLine.currentTime() < oldCurrentTime);
        timeLine.stop();
        int currentTime = timeLine.currentTime();
        QVERIFY(currentTime < oldCurrentTime);
        QVERIFY(currentTime > 0);
    }
}

void tst_BOBUIimeLine::restart()
{
    BOBUIimeLine timeLine(100);
    timeLine.setFrameRange(0,9);

    timeLine.start();
    BOBUIRY_COMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);

    // A restart with the same duration
    timeLine.start();
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
    BOBUIRY_COMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);

    // Set a smaller duration and restart
    timeLine.setDuration(50);
    timeLine.start();
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
    BOBUIRY_COMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), BOBUIimeLine::NotRunning);

    // Set a longer duration and restart
    timeLine.setDuration(150);
    timeLine.start();
    QCOMPARE(timeLine.state(), BOBUIimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
}

void tst_BOBUIimeLine::setPaused()
{
    const int EndTime = 10000;
    BOBUIimeLine timeLine(EndTime);
    {
        QCOMPARE(timeLine.currentTime(), 0);
        timeLine.start();
        BOBUIRY_VERIFY(timeLine.currentTime() != 0);  // wait for start
        timeLine.setPaused(true);
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime > 0);
        QVERIFY(oldCurrentTime < EndTime);
        BOBUIest::qWait(1000);
        timeLine.setPaused(false);
        BOBUIRY_VERIFY(timeLine.currentTime() > oldCurrentTime);
        QVERIFY(timeLine.currentTime() > 0);
        QVERIFY(timeLine.currentTime() < EndTime);
        timeLine.stop();
    }
}

void tst_BOBUIimeLine::automatedBindableTests()
{
    BOBUIimeLine timeLine(200);

    BOBUIestPrivate::testReadWritePropertyBasics(timeLine, 1000, 2000, "duration");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for duration";
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics(timeLine, 10, 20, "updateInterval");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for updateInterval";
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics(timeLine, 10, 20, "currentTime");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for currentTime";
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics(timeLine, BOBUIimeLine::Forward, BOBUIimeLine::Backward,
                                              "direction");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for direction";
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics(timeLine, 4, 5, "loopCount");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for loopCount";
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics<BOBUIimeLine, QEasingCurve>(
            timeLine, QEasingCurve::InQuad, QEasingCurve::OutQuad, "easingCurve");
    if (BOBUIest::currentTestFailed()) {
        qDebug() << "Failed property test for easingCurve";
        return;
    }
}

BOBUIEST_MAIN(tst_BOBUIimeLine)

#include "tst_bobuiimeline.moc"
