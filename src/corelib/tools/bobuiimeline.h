// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIIMELINE_H
#define BOBUIIMELINE_H

#include <BobUICore/qglobal.h>

BOBUI_REQUIRE_CONFIG(easingcurve);

#include <BobUICore/qeasingcurve.h>
#include <BobUICore/qobject.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIimeLinePrivate;
class Q_CORE_EXPORT BOBUIimeLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int duration READ duration WRITE setDuration BINDABLE bindableDuration)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval
               BINDABLE bindableUpdateInterval)
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime BINDABLE bindableCurrentTime)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection BINDABLE bindableDirection)
    Q_PROPERTY(int loopCount READ loopCount WRITE setLoopCount BINDABLE bindableLoopCount)
    Q_PROPERTY(QEasingCurve easingCurve READ easingCurve WRITE setEasingCurve
               BINDABLE bindableEasingCurve)
public:
    enum State {
        NotRunning,
        Paused,
        Running
    };
    enum Direction {
        Forward,
        Backward
    };

    explicit BOBUIimeLine(int duration = 1000, QObject *parent = nullptr);
    virtual ~BOBUIimeLine();

    State state() const;

    int loopCount() const;
    void setLoopCount(int count);
    QBindable<int> bindableLoopCount();

    Direction direction() const;
    void setDirection(Direction direction);
    QBindable<Direction> bindableDirection();

    int duration() const;
    void setDuration(int duration);
    QBindable<int> bindableDuration();

    int startFrame() const;
    void setStartFrame(int frame);
    int endFrame() const;
    void setEndFrame(int frame);
    void setFrameRange(int startFrame, int endFrame);

    int updateInterval() const;
    void setUpdateInterval(int interval);
    QBindable<int> bindableUpdateInterval();

    QEasingCurve easingCurve() const;
    void setEasingCurve(const QEasingCurve &curve);
    QBindable<QEasingCurve> bindableEasingCurve();

    int currentTime() const;
    QBindable<int> bindableCurrentTime();
    int currentFrame() const;
    qreal currentValue() const;

    int frameForTime(int msec) const;
    virtual qreal valueForTime(int msec) const;

public Q_SLOTS:
    void start();
    void resume();
    void stop();
    void setPaused(bool paused);
    void setCurrentTime(int msec);
    void toggleDirection();

Q_SIGNALS:
    void valueChanged(qreal x, QPrivateSignal);
    void frameChanged(int, QPrivateSignal);
    void stateChanged(BOBUIimeLine::State newState, QPrivateSignal);
    void finished(QPrivateSignal);

protected:
    void timerEvent(BOBUIimerEvent *event) override;

private:
    Q_DISABLE_COPY(BOBUIimeLine)
    Q_DECLARE_PRIVATE(BOBUIimeLine)
};

BOBUI_END_NAMESPACE

#endif

