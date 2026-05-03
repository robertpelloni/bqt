// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuiimeline.h"

#include <private/qproperty_p.h>
#include <private/qobject_p.h>
#include <BobUICore/qbasictimer.h>
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qmath.h>
#include <BobUICore/qelapsedtimer.h>

using namespace std::chrono_literals;

BOBUI_BEGIN_NAMESPACE

class BOBUIimeLinePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(BOBUIimeLine)
public:
    QElapsedTimer timer;
    QBasicTimer basicTimer;
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, QEasingCurve, easingCurve,
                                         QEasingCurve::InOutSine)

    int startTime = 0;
    void setDuration(int duration) { q_func()->setDuration(duration); }
    Q_OBJECT_COMPAT_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, int, duration,
                                       &BOBUIimeLinePrivate::setDuration, 1000)
    int startFrame = 0;
    int endFrame = 0;
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, int, updateInterval, 1000 / 25)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, int, loopCount, 1)
    int currentLoopCount = 0;

    void setCurrentTimeForwardToQ(int time) { q_func()->setCurrentTime(time); }
    Q_OBJECT_COMPAT_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, int, currentTime,
                                       &BOBUIimeLinePrivate::setCurrentTimeForwardToQ, 0)

    void setDirection(BOBUIimeLine::Direction direction) { q_func()->setDirection(direction); }
    Q_OBJECT_COMPAT_PROPERTY_WITH_ARGS(BOBUIimeLinePrivate, BOBUIimeLine::Direction, direction,
                                       &BOBUIimeLinePrivate::setDirection, BOBUIimeLine::Forward)
    BOBUIimeLine::State state = BOBUIimeLine::NotRunning;
    inline void setState(BOBUIimeLine::State newState)
    {
        Q_Q(BOBUIimeLine);
        if (newState != state)
            emit q->stateChanged(state = newState, BOBUIimeLine::QPrivateSignal());
    }

    void setCurrentTime(int msecs);
};

/*!
    \internal
*/
void BOBUIimeLinePrivate::setCurrentTime(int msecs)
{
    Q_Q(BOBUIimeLine);
    currentTime.removeBindingUnlessInWrapper();
    const auto previousCurrentTime = currentTime.valueBypassingBindings();

    const qreal lastValue = q->valueForTime(previousCurrentTime);
    const int lastFrame = q->frameForTime(previousCurrentTime);

    // Determine if we are looping.
    const int elapsed = (direction == BOBUIimeLine::Backward) ? (-msecs + duration) : msecs;
    const int loopCountNow = elapsed / duration;

    const bool looping = (loopCountNow != currentLoopCount);
#ifdef BOBUIIMELINE_DEBUG
    qDebug() << "BOBUIimeLinePrivate::setCurrentTime:" << msecs << duration << "with loopCountNow"
             << loopCountNow << "currentLoopCount" << currentLoopCount << "looping" << looping;
#endif
    if (looping)
        currentLoopCount = loopCountNow;

    // Normalize msecs to be between 0 and duration, inclusive.
    currentTime.setValueBypassingBindings(elapsed % duration);
    if (direction.value() == BOBUIimeLine::Backward)
        currentTime.setValueBypassingBindings(duration - currentTime.valueBypassingBindings());

    // Check if we have reached the end of loopcount.
    bool finished = false;
    if (loopCount && currentLoopCount >= loopCount) {
        finished = true;
        currentTime.setValueBypassingBindings((direction == BOBUIimeLine::Backward) ? 0 : duration);
        currentLoopCount = loopCount - 1;
    }

    const int currentFrame = q->frameForTime(currentTime.valueBypassingBindings());
#ifdef BOBUIIMELINE_DEBUG
    qDebug() << "BOBUIimeLinePrivate::setCurrentTime: frameForTime"
             << currentTime.valueBypassingBindings() << currentFrame;
#endif
    const qreal currentValue = q->valueForTime(currentTime.valueBypassingBindings());
    if (!qFuzzyCompare(lastValue, currentValue))
        emit q->valueChanged(currentValue, BOBUIimeLine::QPrivateSignal());
    if (lastFrame != currentFrame) {
        const int transitionframe = (direction == BOBUIimeLine::Forward ? endFrame : startFrame);
        if (looping && !finished && transitionframe != currentFrame) {
#ifdef BOBUIIMELINE_DEBUG
            qDebug("BOBUIimeLinePrivate::setCurrentTime: transitionframe");
#endif
            emit q->frameChanged(transitionframe, BOBUIimeLine::QPrivateSignal());
        }
#ifdef BOBUIIMELINE_DEBUG
        else {
            QByteArray reason;
            if (!looping)
                reason += " not looping";
            if (finished) {
                if (!reason.isEmpty())
                    reason += " and";
                reason += " finished";
            }
            if (transitionframe == currentFrame) {
                if (!reason.isEmpty())
                    reason += " and";
                reason += " transitionframe is equal to currentFrame: " + QByteArray::number(currentFrame);
            }
            qDebug("BOBUIimeLinePrivate::setCurrentTime: not transitionframe because %s",  reason.constData());
        }
#endif
        emit q->frameChanged(currentFrame, BOBUIimeLine::QPrivateSignal());
    }
    if (finished && state == BOBUIimeLine::Running) {
        q->stop();
        emit q->finished(BOBUIimeLine::QPrivateSignal());
    }
    if (currentTime.valueBypassingBindings() != previousCurrentTime)
        currentTime.notify();
}
QBindable<int> BOBUIimeLine::bindableCurrentTime()
{
    Q_D(BOBUIimeLine);
    return &d->currentTime;
}

/*!
    \class BOBUIimeLine
    \inmodule BobUICore
    \brief The BOBUIimeLine class provides a timeline for controlling animations.
    \since 4.2
    \ingroup animation

    It's most commonly used to animate a GUI control by calling a slot
    periodically. You can construct a timeline by passing its duration in
    milliseconds to BOBUIimeLine's constructor. The timeline's duration describes
    for how long the animation will run. Then you set a suitable frame range
    by calling setFrameRange(). Finally connect the frameChanged() signal to a
    suitable slot in the widget you wish to animate (for example, \l {QProgressBar::}{setValue()}
    in QProgressBar). When you proceed to calling start(), BOBUIimeLine will enter
    Running state, and start emitting frameChanged() at regular intervals,
    causing your widget's connected property's value to grow from the lower
    end to the upper and of your frame range, at a steady rate. You can
    specify the update interval by calling setUpdateInterval(). When done,
    BOBUIimeLine enters NotRunning state, and emits finished().

    Example:

    \snippet code/src_corelib_tools_bobuiimeline.cpp 0

    By default the timeline runs once, from its beginning to its end,
    upon which you must call start() again to restart from the beginning. To
    make the timeline loop, you can call setLoopCount(), passing the number of
    times the timeline should run before finishing. The direction can also be
    changed, causing the timeline to run backward, by calling
    setDirection(). You can also pause and unpause the timeline while it's
    running by calling setPaused(). For interactive control, the
    setCurrentTime() function is provided, which sets the time position of the
    time line directly. Although most useful in NotRunning state (e.g.,
    connected to a valueChanged() signal in a QSlider), this function can be
    called at any time.

    The frame interface is useful for standard widgets, but BOBUIimeLine can be
    used to control any type of animation. The heart of BOBUIimeLine lies in the
    valueForTime() function, which generates a \e value between 0 and 1 for a
    given time. This value is typically used to describe the steps of an
    animation, where 0 is the first step of an animation, and 1 is the last
    step. When running, BOBUIimeLine generates values between 0 and 1 by calling
    valueForTime() and emitting valueChanged(). By default, valueForTime()
    applies an interpolation algorithm to generate these value. You can choose
    from a set of predefined timeline algorithms by calling setEasingCurve().

    Note that, by default, BOBUIimeLine uses QEasingCurve::InOutSine, which
    provides a value that grows slowly, then grows steadily, and finally grows
    slowly. For a custom timeline, you can reimplement valueForTime(), in which
    case BOBUIimeLine's easingCurve property is ignored.

    \sa QProgressBar, QProgressDialog
*/

/*!
    \enum BOBUIimeLine::State

    This enum describes the state of the timeline.

    \value NotRunning The timeline is not running. This is the initial state
    of BOBUIimeLine, and the state BOBUIimeLine reenters when finished. The current
    time, frame and value remain unchanged until either setCurrentTime() is
    called, or the timeline is started by calling start().

    \value Paused The timeline is paused (i.e., temporarily
    suspended). Calling setPaused(false) will resume timeline activity.

    \value Running The timeline is running. While control is in the event
    loop, BOBUIimeLine will update its current time at regular intervals,
    emitting valueChanged() and frameChanged() when appropriate.

    \sa state(), stateChanged()
*/

/*!
    \enum BOBUIimeLine::Direction

    This enum describes the direction of the timeline when in \l Running state.

    \value Forward The current time of the timeline increases with time (i.e.,
    moves from 0 and towards the end / duration).

    \value Backward The current time of the timeline decreases with time (i.e.,
    moves from the end / duration and towards 0).

    \sa setDirection()
*/

/*!
    \fn void BOBUIimeLine::valueChanged(qreal value)

    BOBUIimeLine emits this signal at regular intervals when in \l Running state,
    but only if the current value changes. \a value is the current value. \a value is
    a number between 0.0 and 1.0

    \sa BOBUIimeLine::setDuration(), BOBUIimeLine::valueForTime(), BOBUIimeLine::updateInterval
*/

/*!
    \fn void BOBUIimeLine::frameChanged(int frame)

    BOBUIimeLine emits this signal at regular intervals when in \l Running state,
    but only if the current frame changes. \a frame is the current frame number.

    \sa BOBUIimeLine::setFrameRange(), BOBUIimeLine::updateInterval
*/

/*!
    \fn void BOBUIimeLine::stateChanged(BOBUIimeLine::State newState)

    This signal is emitted whenever BOBUIimeLine's state changes. The new state
    is \a newState.
*/

/*!
    \fn void BOBUIimeLine::finished()

    This signal is emitted when BOBUIimeLine finishes (i.e., reaches the end of
    its time line), and does not loop.
*/

/*!
    Constructs a timeline with a duration of \a duration milliseconds. \a
    parent is passed to QObject's constructor. The default duration is 1000
    milliseconds.
 */
BOBUIimeLine::BOBUIimeLine(int duration, QObject *parent)
    : QObject(*new BOBUIimeLinePrivate, parent)
{
    setDuration(duration);
}

/*!
    Destroys the timeline.
 */
BOBUIimeLine::~BOBUIimeLine()
{
    Q_D(BOBUIimeLine);

    if (d->state == Running)
        stop();
}

/*!
    Returns the state of the timeline.

    \sa start(), setPaused(), stop()
*/
BOBUIimeLine::State BOBUIimeLine::state() const
{
    Q_D(const BOBUIimeLine);
    return d->state;
}

/*!
    \property BOBUIimeLine::loopCount
    \brief the number of times the timeline should loop before it's finished.

    A loop count of 0 means that the timeline will loop forever.

    By default, this property contains a value of 1.
*/
int BOBUIimeLine::loopCount() const
{
    Q_D(const BOBUIimeLine);
    return d->loopCount;
}

void BOBUIimeLine::setLoopCount(int count)
{
    Q_D(BOBUIimeLine);
    d->loopCount = count;
}

QBindable<int> BOBUIimeLine::bindableLoopCount()
{
    Q_D(BOBUIimeLine);
    return &d->loopCount;
}

/*!
    \property BOBUIimeLine::direction
    \brief the direction of the timeline when BOBUIimeLine is in \l Running
    state.

    This direction indicates whether the time moves from 0 towards the
    timeline duration, or from the value of the duration and towards 0 after
    start() has been called.

    Any binding of direction will be removed not only by setDirection(),
    but also by toggleDirection().

    By default, this property is set to \l Forward.
*/
BOBUIimeLine::Direction BOBUIimeLine::direction() const
{
    Q_D(const BOBUIimeLine);
    return d->direction;
}
void BOBUIimeLine::setDirection(Direction direction)
{
    Q_D(BOBUIimeLine);
    d->direction.removeBindingUnlessInWrapper();
    const auto previousDirection = d->direction.valueBypassingBindings();
    d->direction.setValueBypassingBindings(direction);
    d->startTime = d->currentTime;
    d->timer.start();
    if (previousDirection != d->direction.valueBypassingBindings())
        d->direction.notify();
}

QBindable<BOBUIimeLine::Direction> BOBUIimeLine::bindableDirection()
{
    Q_D(BOBUIimeLine);
    return &d->direction;
}

/*!
    \property BOBUIimeLine::duration
    \brief the total duration of the timeline in milliseconds.

    By default, this value is 1000 (i.e., 1 second), but you can change this
    by either passing a duration to BOBUIimeLine's constructor, or by calling
    setDuration(). The duration must be larger than 0.

    \note Changing the duration does not cause the current time to be reset
    to zero or the new duration. You also need to call setCurrentTime() with
    the desired value.
*/
int BOBUIimeLine::duration() const
{
    Q_D(const BOBUIimeLine);
    return d->duration;
}
void BOBUIimeLine::setDuration(int duration)
{
    Q_D(BOBUIimeLine);
    if (duration <= 0) {
        qWarning("BOBUIimeLine::setDuration: cannot set duration <= 0");
        return;
    }
    d->duration.removeBindingUnlessInWrapper();
    if (duration != d->duration.valueBypassingBindings()) {
        d->duration.setValueBypassingBindings(duration);
        d->duration.notify();
    }
}

QBindable<int> BOBUIimeLine::bindableDuration()
{
    Q_D(BOBUIimeLine);
    return &d->duration;
}

/*!
    Returns the start frame, which is the frame corresponding to the start of
    the timeline (i.e., the frame for which the current value is 0).

    \sa setStartFrame(), setFrameRange()
*/
int BOBUIimeLine::startFrame() const
{
    Q_D(const BOBUIimeLine);
    return d->startFrame;
}

/*!
    Sets the start frame, which is the frame corresponding to the start of the
    timeline (i.e., the frame for which the current value is 0), to \a frame.

    \sa startFrame(), endFrame(), setFrameRange()
*/
void BOBUIimeLine::setStartFrame(int frame)
{
    Q_D(BOBUIimeLine);
    d->startFrame = frame;
}

/*!
    Returns the end frame, which is the frame corresponding to the end of the
    timeline (i.e., the frame for which the current value is 1).

    \sa setEndFrame(), setFrameRange()
*/
int BOBUIimeLine::endFrame() const
{
    Q_D(const BOBUIimeLine);
    return d->endFrame;
}

/*!
    Sets the end frame, which is the frame corresponding to the end of the
    timeline (i.e., the frame for which the current value is 1), to \a frame.

    \sa endFrame(), startFrame(), setFrameRange()
*/
void BOBUIimeLine::setEndFrame(int frame)
{
    Q_D(BOBUIimeLine);
    d->endFrame = frame;
}

/*!
    Sets the timeline's frame counter to start at \a startFrame, and end and
    \a endFrame. For each time value, BOBUIimeLine will find the corresponding
    frame when you call currentFrame() or frameForTime() by interpolating,
    using the return value of valueForTime().

    When in Running state, BOBUIimeLine also emits the frameChanged() signal when
    the frame changes.

    \sa startFrame(), endFrame(), start(), currentFrame()
*/
void BOBUIimeLine::setFrameRange(int startFrame, int endFrame)
{
    Q_D(BOBUIimeLine);
    d->startFrame = startFrame;
    d->endFrame = endFrame;
}

/*!
    \property BOBUIimeLine::updateInterval
    \brief the time in milliseconds between each time BOBUIimeLine updates its
    current time.

    When updating the current time, BOBUIimeLine will emit valueChanged() if the
    current value changed, and frameChanged() if the frame changed.

    By default, the interval is 40 ms, which corresponds to a rate of 25
    updates per second.
*/
int BOBUIimeLine::updateInterval() const
{
    Q_D(const BOBUIimeLine);
    return d->updateInterval;
}
void BOBUIimeLine::setUpdateInterval(int interval)
{
    Q_D(BOBUIimeLine);
    d->updateInterval = interval;
}
QBindable<int> BOBUIimeLine::bindableUpdateInterval()
{
    Q_D(BOBUIimeLine);
    return &d->updateInterval;
}

/*!
    \property BOBUIimeLine::easingCurve

    \since 4.6

    Specifies the easing curve that the timeline will use.
    If valueForTime() is reimplemented, this value is ignored.

    \sa valueForTime()
*/

QEasingCurve BOBUIimeLine::easingCurve() const
{
    Q_D(const BOBUIimeLine);
    return d->easingCurve;
}

void BOBUIimeLine::setEasingCurve(const QEasingCurve &curve)
{
    Q_D(BOBUIimeLine);
    d->easingCurve = curve;
}

QBindable<QEasingCurve> BOBUIimeLine::bindableEasingCurve()
{
    Q_D(BOBUIimeLine);
    return &d->easingCurve;
}

/*!
    \property BOBUIimeLine::currentTime
    \brief the current time of the time line.

    When BOBUIimeLine is in Running state, this value is updated continuously as
    a function of the duration and direction of the timeline. Otherwise, it is
    value that was current when stop() was called last, or the value set by
    setCurrentTime().

    \note You can bind other properties to currentTime, but it is not
    recommended setting bindings to it. As animation progresses, the currentTime
    is updated automatically, which cancels its bindings.

    By default, this property contains a value of 0.
*/
int BOBUIimeLine::currentTime() const
{
    Q_D(const BOBUIimeLine);
    return d->currentTime;
}
void BOBUIimeLine::setCurrentTime(int msec)
{
    Q_D(BOBUIimeLine);
    d->startTime = 0;
    d->currentLoopCount = 0;
    d->timer.start();
    d->setCurrentTime(msec);
}

/*!
    Returns the frame corresponding to the current time.

    \sa currentTime(), frameForTime(), setFrameRange()
*/
int BOBUIimeLine::currentFrame() const
{
    Q_D(const BOBUIimeLine);
    return frameForTime(d->currentTime);
}

/*!
    Returns the value corresponding to the current time.

    \sa valueForTime(), currentFrame()
*/
qreal BOBUIimeLine::currentValue() const
{
    Q_D(const BOBUIimeLine);
    return valueForTime(d->currentTime);
}

/*!
    Returns the frame corresponding to the time \a msec. This value is
    calculated using a linear interpolation of the start and end frame, based
    on the value returned by valueForTime().

    \sa valueForTime(), setFrameRange()
*/
int BOBUIimeLine::frameForTime(int msec) const
{
    Q_D(const BOBUIimeLine);
    if (d->direction == Forward)
        return d->startFrame + int((d->endFrame - d->startFrame) * valueForTime(msec));
    return d->startFrame + qCeil((d->endFrame - d->startFrame) * valueForTime(msec));
}

/*!
    Returns the timeline value for the time \a msec. The returned value, which
    varies depending on the curve shape, is always between 0 and 1. If \a msec
    is 0, the default implementation always returns 0.

    Reimplement this function to provide a custom curve shape for your
    timeline.

    \sa easingCurve, frameForTime()
*/
qreal BOBUIimeLine::valueForTime(int msec) const
{
    Q_D(const BOBUIimeLine);
    msec = qBound(0, msec, d->duration.value());

    qreal value = msec / qreal(d->duration.value());
    return d->easingCurve.value().valueForProgress(value);
}

/*!
    Starts the timeline. BOBUIimeLine will enter Running state, and once it
    enters the event loop, it will update its current time, frame and value at
    regular intervals. The default interval is 40 ms (i.e., 25 times per
    second). You can change the update interval by calling
    setUpdateInterval().

    The timeline will start from position 0, or the end if going backward.
    If you want to resume a stopped timeline without restarting, you can call
    resume() instead.

    \sa resume(), updateInterval(), frameChanged(), valueChanged()
*/
void BOBUIimeLine::start()
{
    Q_D(BOBUIimeLine);
    if (d->basicTimer.isActive()) {
        qWarning("BOBUIimeLine::start: already running");
        return;
    }
    int curTime = 0;
    if (d->direction == Backward)
        curTime = d->duration;
    d->basicTimer.start(d->updateInterval * 1ms, this);
    d->startTime = curTime;
    d->currentLoopCount = 0;
    d->timer.start();
    d->setState(Running);
    d->setCurrentTime(curTime);
}

/*!
    Resumes the timeline from the current time. BOBUIimeLine will reenter Running
    state, and once it enters the event loop, it will update its current time,
    frame and value at regular intervals.

    In contrast to start(), this function does not restart the timeline before
    it resumes.

    \sa start(), updateInterval(), frameChanged(), valueChanged()
*/
void BOBUIimeLine::resume()
{
    Q_D(BOBUIimeLine);
    if (d->basicTimer.isActive()) {
        qWarning("BOBUIimeLine::resume: already running");
        return;
    }
    d->basicTimer.start(d->updateInterval * 1ms, this);
    d->startTime = d->currentTime;
    d->timer.start();
    d->setState(Running);
}

/*!
    Stops the timeline, causing BOBUIimeLine to enter NotRunning state.

    \sa start()
*/
void BOBUIimeLine::stop()
{
    Q_D(BOBUIimeLine);
    d->basicTimer.stop();
    d->setState(NotRunning);
}

/*!
    If \a paused is true, the timeline is paused, causing BOBUIimeLine to enter
    Paused state. No updates will be signaled until either start() or
    setPaused(false) is called. If \a paused is false, the timeline is resumed
    and continues where it left.

    \sa state(), start()
*/
void BOBUIimeLine::setPaused(bool paused)
{
    Q_D(BOBUIimeLine);
    if (d->state == NotRunning) {
        qWarning("BOBUIimeLine::setPaused: Not running");
        return;
    }
    if (paused && d->state != Paused) {
        d->startTime = d->currentTime;
        d->basicTimer.stop();
        d->setState(Paused);
    } else if (!paused && d->state == Paused) {
        // Same as resume()
        d->basicTimer.start(d->updateInterval * 1ms, this);
        d->startTime = d->currentTime;
        d->timer.start();
        d->setState(Running);
    }
}

/*!
    Toggles the direction of the timeline. If the direction was Forward, it
    becomes Backward, and vice verca.

    Existing bindings of \l direction are removed.

    \sa setDirection()
*/
void BOBUIimeLine::toggleDirection()
{
    Q_D(BOBUIimeLine);
    setDirection(d->direction == Forward ? Backward : Forward);
}

/*!
    \reimp
*/
void BOBUIimeLine::timerEvent(BOBUIimerEvent *event)
{
    Q_D(BOBUIimeLine);
    if (event->id() != d->basicTimer.id()) {
        event->ignore();
        return;
    }
    event->accept();

    if (d->direction == Forward) {
        d->setCurrentTime(d->startTime + d->timer.elapsed());
    } else {
        d->setCurrentTime(d->startTime - d->timer.elapsed());
    }
}

BOBUI_END_NAMESPACE

#include "moc_bobuiimeline.cpp"
