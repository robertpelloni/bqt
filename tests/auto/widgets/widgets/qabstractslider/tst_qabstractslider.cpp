// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <QAbstractSlider>
#include <BobUIWidgets/qdial.h>
#include <QScrollBar>
#include <QSlider>
#include <QStyle>
#include <QStyleOption>

#include <QElapsedTimer>
#include <QDebug>
#include <QSignalSpy>

#include <BobUITest/private/bobuiesthelpers_p.h>

using namespace BOBUIestPrivate;

// defined to be 120 by the wheel mouse vendors according to the docs
#define WHEEL_DELTA 120

class Slider : public QAbstractSlider
{
    public:
        Slider(QWidget *parent)
            : QAbstractSlider(parent) {}
        using QAbstractSlider::setRepeatAction;
        using QAbstractSlider::repeatAction;
};

class tst_QAbstractSlider: public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();
    void cleanupTestCase();
    void actionTriggered(int action);
    void rangeChanged(int min, int max);
    void valueChanged(int value);
    void sliderMoved(int value);

private slots:
    void triggerAction_data();
    void triggerAction();
    void minimum_maximum_data();
    void minimum_maximum();
    void keyPressed_data();
    void keyPressed();
#if BOBUI_CONFIG(wheelevent)
    void wheelEvent_data();
    void wheelEvent();
    void fineGrainedWheelEvent_data();
    void fineGrainedWheelEvent();
#endif
    void sliderPressedReleased_data();
    void sliderPressedReleased();
    void setOrientation();
    void sliderMoved_data();
    void sliderMoved();
    void rangeChanged_data();
    void rangeChanged();
    void setSliderPosition_data();
    void setSliderPosition();
    void setValue_data();
    void setValue();
    void setRepeatAction();
    void connectedSliders();

    void sliderChange_QDial() { sliderChange_impl<QDial>(); }
    void sliderChange_QScrollBar() { sliderChange_impl<QScrollBar>(); }
    void sliderChange_QSlider() { sliderChange_impl<QSlider>(); }

private:
    template <typename Slider>
    void sliderChange_impl();
    void waitUntilTimeElapsed(const QElapsedTimer &t, int ms);

    QWidget *topLevel;
    Slider *slider;
    int previousAction;
    int reportedMinimum;
    int reportedMaximum;
    int reportedValue;
    int reportedSliderPosition;
    qint64 timeStamp;
    qint64 actionTriggeredTimeStamp;
    qint64 valueChangedTimeStamp;
    qint64 rangeChangedTimeStamp;
    qint64 sliderMovedTimeStamp;
};

Q_DECLARE_METATYPE(QList<BobUI::Key>)

void tst_QAbstractSlider::initTestCase()
{
    topLevel = new QWidget;
    slider = new Slider(topLevel);
    slider->setObjectName("testWidget");
    slider->resize(100,100);
    slider->show();

    previousAction = QAbstractSlider::SliderNoAction;
    timeStamp = 0;

    connect(slider,SIGNAL(actionTriggered(int)),this,SLOT(actionTriggered(int)));
    connect(slider,SIGNAL(rangeChanged(int,int)),this,SLOT(rangeChanged(int,int)));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(sliderMoved(int)));
}

void tst_QAbstractSlider::cleanupTestCase()
{
    delete topLevel;
}

void tst_QAbstractSlider::actionTriggered(int action)
{
    previousAction = action;
    actionTriggeredTimeStamp = timeStamp++;
}

void tst_QAbstractSlider::rangeChanged(int min,int max)
{
    reportedMinimum = min;
    reportedMaximum = max;
    rangeChangedTimeStamp = timeStamp++;
}

void tst_QAbstractSlider::valueChanged(int value)
{
    reportedValue = value;
    valueChangedTimeStamp = timeStamp++;
}

void tst_QAbstractSlider::sliderMoved(int value)
{
    reportedSliderPosition = value;
    sliderMovedTimeStamp = timeStamp++;
}

void tst_QAbstractSlider::triggerAction_data()
{
    BOBUIest::addColumn<int>("sliderAction");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("initialSliderPosition");
    BOBUIest::addColumn<int>("singleStep");
    BOBUIest::addColumn<int>("pageStep");
    BOBUIest::addColumn<int>("expectedSliderPosition");

    BOBUIest::newRow("No action") << int(QAbstractSlider::SliderNoAction) // sliderAction
                            << 1000                                 // max
                            << 900                                  // min
                            << 987                                  // initial position
                            << 237                                  // single step size
                            << 234                                  // page step size
                            << 987;                                 // expected position after

    BOBUIest::newRow("Move action") << int(QAbstractSlider::SliderMove) // sliderAction
                              << 1000                                 // max
                              << 900                                  // min
                              << 988                                  // initial position
                              << 237                                  // single step size
                              << 234                                  // page step size
                              << 988;                                 // expected position after

    BOBUIest::newRow("Empty step add") << int(QAbstractSlider::SliderSingleStepAdd) // sliderAction
                                 << 1000                                      // max
                                 << 900                                       // min
                                 << 988                                       // initial position
                                 << 0                                         // single step size
                                 << 234                                       // page step size
                                 << 988;                                      // expected position after

     BOBUIest::newRow("Empty step sub") << int(QAbstractSlider::SliderSingleStepSub) // sliderAction
                                 << 1000                                       // max
                                 << 900                                        // min
                                 << 987                                        // initial position
                                 << 0                                          // single step size
                                 << 234                                        // page step size
                                 << 987;                                       // expected position after

    BOBUIest::newRow("Empty page add") << int(QAbstractSlider::SliderPageStepAdd) // sliderAction
                                 << 1000                                    // max
                                 << 900                                     // min
                                 << 988                                     // initial position
                                 << 234                                     // single step size
                                 << 0                                       // page step size
                                 << 988;                                    // expected position after

     BOBUIest::newRow("Empty page sub") << int(QAbstractSlider::SliderPageStepSub) // sliderAction
                                 << 1000                                     // max
                                 << 900                                      // min
                                 << 987                                      // initial position
                                 << 234                                      // single step size
                                 << 0                                        // page step size
                                 << 987;                                     // expected position after

    BOBUIest::newRow("Legal step add") << int(QAbstractSlider::SliderSingleStepAdd) // sliderAction
                                 << 1000                                      // max
                                 << 900                                       // min
                                 << 988                                       // initial position
                                 << 5                                         // single step size
                                 << 234                                       // page step size
                                 << 993;                                      // expected position after

     BOBUIest::newRow("Legal step sub") << int(QAbstractSlider::SliderSingleStepSub) // sliderAction
                                 << 1000                                       // max
                                 << 900                                        // min
                                 << 987                                        // initial position
                                 << 5                                          // single step size
                                 << 234                                        // page step size
                                 << 982;                                       // expected position after

    BOBUIest::newRow("Legal page add") << int(QAbstractSlider::SliderPageStepAdd) // sliderAction
                                 << 1000                                    // max
                                 << 900                                     // min
                                 << 988                                     // initial position
                                 << 234                                     // single step size
                                 << 5                                       // page step size
                                 << 993;                                    // expected position after

     BOBUIest::newRow("Legal page sub") << int(QAbstractSlider::SliderPageStepSub) // sliderAction
                                 << 1000                                     // max
                                 << 900                                      // min
                                 << 987                                      // initial position
                                 << 234                                      // single step size
                                 << 5                                        // page step size
                                 << 982;                                     // expected position after

    BOBUIest::newRow("Illegal step add") << int(QAbstractSlider::SliderSingleStepAdd) // sliderAction
                                   << 1000                                      // max
                                   << 900                                       // min
                                   << 988                                       // initial position
                                   << 500                                       // single step size
                                   << 234                                       // page step size
                                   << 1000;                                     // expected position after

     BOBUIest::newRow("Illegal step sub") << int(QAbstractSlider::SliderSingleStepSub) // sliderAction
                                    << 1000                                      // max
                                    << 900                                       // min
                                    << 987                                       // initial position
                                    << 500                                       // single step size
                                    << 234                                       // page step size
                                    << 900;                                      // expected position after

    BOBUIest::newRow("Illegal page add") << int(QAbstractSlider::SliderPageStepAdd) // sliderAction
                                   << 1000                                    // max
                                   << 900                                     // min
                                   << 988                                     // initial position
                                   << 234                                     // single step size
                                   << 500                                     // page step size
                                   << 1000;                                   // expected position after

     BOBUIest::newRow("Illegal page sub") << int(QAbstractSlider::SliderPageStepSub) // sliderAction
                                    << 1000                                    // max
                                    << 900                                     // min
                                    << 987                                     // initial position
                                    << 234                                     // single step size
                                    << 500                                     // page step size
                                    << 900;                                    // expected position after

    // Negative steps will also be abs()'d so, check that case.
    BOBUIest::newRow("Negative step add") << int(QAbstractSlider::SliderSingleStepAdd) // sliderAction
                                    << 1000                                      // max
                                    << 900                                       // min
                                    << 988                                       // initial position
                                    << -1                                        // single step size
                                    << 234                                       // page step size
                                    << 989;                                      // expected position after

     BOBUIest::newRow("Negative step sub") << int(QAbstractSlider::SliderSingleStepSub) // sliderAction
                                     << 1000                                      // max
                                     << 900                                       // min
                                     << 987                                       // initial position
                                     << -1                                        // single step size
                                     << 234                                       // page step size
                                     << 986;                                      // expected position after

    BOBUIest::newRow("Negative page add") << int(QAbstractSlider::SliderPageStepAdd) // sliderAction
                                    << 1000                                    // max
                                    << 900                                     // min
                                    << 988                                     // initial position
                                    << 234                                     // single step size
                                    << -1                                      // page step size
                                    << 989;                                    // expected position after

     BOBUIest::newRow("Negative page sub") << int(QAbstractSlider::SliderPageStepSub) // sliderAction
                                     << 1000                                    // max
                                     << 900                                     // min
                                     << 987                                     // initial position
                                     << 234                                     // single step size
                                     << -1                                      // page step size
                                     << 986;                                    // expected position after

    BOBUIest::newRow("Illegal negative step add") << int(QAbstractSlider::SliderSingleStepAdd) // sliderAction
                                            << 1000                                      // max
                                            << 900                                       // min
                                            << 988                                       // initial position
                                            << -500                                      // single step size
                                            << 234                                       // page step size
                                            << 1000;                                      // expected position after


    BOBUIest::newRow("Illegal negative step sub") << int(QAbstractSlider::SliderSingleStepSub) // sliderAction
                                            << 1000                                      // max
                                            << 900                                       // min
                                            << 988                                       // initial position
                                            << -500                                      // single step size
                                            << 234                                       // page step size
                                            << 900;                                     // expected position after

    BOBUIest::newRow("Illegal negative page add") << int(QAbstractSlider::SliderPageStepAdd) // sliderAction
                                            << 1000                                    // max
                                            << 900                                     // min
                                            << 988                                     // initial position
                                            << 234                                     // single step size
                                            << -500                                    // page step size
                                            << 1000;                                    // expected position after

    BOBUIest::newRow("Illegal negative page sub") << int(QAbstractSlider::SliderPageStepSub) // sliderAction
                                            << 1000                                    // max
                                            << 900                                     // min
                                            << 988                                     // initial position
                                            << 245                                     // single step size
                                            << -500                                    // page step size
                                            << 900;                                   // expected position after

    BOBUIest::newRow("Slider to minimum") << int(QAbstractSlider::SliderToMinimum)   // sliderAction
                                    << 1000                                    // max
                                    << 900                                     // min
                                    << 988                                     // initial position
                                    << 245                                     // single step size
                                    << 1                                       // page step size
                                    << 900;                                    // expected position after

    BOBUIest::newRow("Slider to maximum") << int(QAbstractSlider::SliderToMaximum)   // sliderAction
                                    << 1000                                    // max
                                    << 900                                     // min
                                    << 988                                     // initial position
                                    << 245                                     // single step size
                                    << 1                                       // page step size
                                    << 1000;                                   // expected position after

}

void tst_QAbstractSlider::triggerAction()
{
    QFETCH(int,sliderAction);
    QFETCH(int,maximum);
    QFETCH(int,minimum);
    QFETCH(int,initialSliderPosition);
    QFETCH(int,singleStep);
    QFETCH(int,pageStep);
    QFETCH(int,expectedSliderPosition);

    slider->setTracking(true);
    slider->setRange(minimum,maximum);
    slider->setSingleStep(singleStep);
    slider->setPageStep(pageStep);
    QCOMPARE(slider->singleStep(), qAbs(singleStep));
    QCOMPARE(slider->pageStep(), qAbs(pageStep));

    int oldPosition = slider->sliderPosition();
    slider->setSliderPosition(initialSliderPosition);

    QVERIFY( (oldPosition == initialSliderPosition && previousAction == int(QAbstractSlider::SliderNoAction)) ||
            (oldPosition != initialSliderPosition && previousAction == int(QAbstractSlider::SliderMove)));
    previousAction = int(QAbstractSlider::SliderNoAction);

    QAbstractSlider::SliderAction *action = reinterpret_cast<QAbstractSlider::SliderAction*>(&sliderAction);
    QVERIFY(action != 0);

    slider->triggerAction(*action);
    QCOMPARE(previousAction,sliderAction); // previousAction set in the actionTriggered() slot
    QCOMPARE(slider->sliderPosition(),expectedSliderPosition);
    QCOMPARE(slider->value(),expectedSliderPosition);
    QCOMPARE(reportedValue,expectedSliderPosition);
    previousAction = int(QAbstractSlider::SliderNoAction);
    if (initialSliderPosition != expectedSliderPosition)
        QVERIFY(actionTriggeredTimeStamp < valueChangedTimeStamp);
}

void tst_QAbstractSlider::minimum_maximum_data()
{
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("expectedMinimum");
    BOBUIest::addColumn<int>("expectedMaximum");

    BOBUIest::newRow("Normal range") << 100 << 200 << 100 << 200;
    BOBUIest::newRow("Minimum higher") << 100 << 0 << 100 << 100;
    BOBUIest::newRow("Negative minimum") << -100 << 100 << -100 << 100;
    BOBUIest::newRow("Negative range") << -100 << -50 << -100 << -50;
}

void tst_QAbstractSlider::minimum_maximum()
{
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, expectedMinimum);
    QFETCH(int, expectedMaximum);

    slider->setRange(minimum,maximum);
    QCOMPARE(slider->minimum(),expectedMinimum);
    QCOMPARE(slider->maximum(),expectedMaximum);
    QCOMPARE(reportedMinimum,expectedMinimum);
    QCOMPARE(reportedMaximum,expectedMaximum);

    slider->setRange(minimum,maximum);
    slider->setMaximum(slider->minimum() - 1);
    QCOMPARE(slider->maximum(),slider->minimum());
    QCOMPARE(reportedMinimum,slider->minimum());
    QCOMPARE(reportedMaximum,slider->maximum());

    slider->setRange(minimum,maximum);
    slider->setMinimum(slider->maximum() + 1);
    QCOMPARE(slider->minimum(),slider->maximum());
    QCOMPARE(reportedMinimum,slider->minimum());
    QCOMPARE(reportedMaximum,slider->maximum());

    slider->setRange(minimum,maximum);
    slider->setSliderPosition(slider->maximum() + 1);
    QCOMPARE(slider->sliderPosition(), slider->maximum());
    QCOMPARE(slider->value(), slider->maximum());
    QCOMPARE(reportedValue, slider->maximum());

    slider->setRange(minimum,maximum);
    slider->setSliderPosition(slider->minimum() - 1);
    QCOMPARE(slider->sliderPosition(), slider->minimum());
    QCOMPARE(slider->value(), slider->minimum());
    QCOMPARE(reportedValue, slider->minimum());

    slider->setRange(minimum,maximum);
    int oldPosition = slider->sliderPosition();
    slider->setMaximum(oldPosition - 1);
    QCOMPARE(slider->sliderPosition(),oldPosition - 1);

    slider->setRange(minimum,maximum);
    oldPosition = slider->sliderPosition();
    slider->setMinimum(oldPosition + 1);
    QCOMPARE(slider->sliderPosition(), oldPosition + 1);
}

void tst_QAbstractSlider::keyPressed_data()
{
    BOBUIest::addColumn<int>("initialSliderPosition");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("stepSize");
    BOBUIest::addColumn<int>("pageSize");
    BOBUIest::addColumn<bool>("invertedAppearance");
    BOBUIest::addColumn<bool>("invertedControls");
    BOBUIest::addColumn<QList<BobUI::Key> >("keySequence");
    BOBUIest::addColumn<int>("expectedSliderPosition");

    QList<BobUI::Key> list;
    list << BobUI::Key_Down;
    BOBUIest::newRow("Step down once 1, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Step down once 1, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Step down once 1, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step down once 1, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step down once 1, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Step down once 1, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Step down once 1, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;   // expected position

    BOBUIest::newRow("Step down once 1, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Up;
    BOBUIest::newRow("Step up once 2, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step up once 2, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step up once 2, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step up once 2, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step up once 2, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step up once 2, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step up once 2, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step up once 2, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Left;
    BOBUIest::newRow("Step left once 2, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step left once 2, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step left once 2, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step left once 2, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step left once 2, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step left once 2, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step left once 2, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step left once 2, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Right;
    BOBUIest::newRow("Step right once 2, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step right once 2, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step right once 2, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step right once 2, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Step right once 2, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step right once 2, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Step right once 2, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Step right once 2, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 0    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;   // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_PageDown;
    BOBUIest::newRow("Page down once, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Page down once, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Page down once, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page down once, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page down once, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Page down once, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 7;   // expected position

    BOBUIest::newRow("Page down once, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page down once, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 13;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_PageUp;
    BOBUIest::newRow("Page up once, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page up once, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page up once, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Page up once, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Page up once, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page up once, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 13;  // expected position

    BOBUIest::newRow("Page up once, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    BOBUIest::newRow("Page up once, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 7;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Up << BobUI::Key_Up << BobUI::Key_PageDown << BobUI::Key_PageDown  << BobUI::Key_Left << BobUI::Key_Left
         << BobUI::Key_Right << BobUI::Key_Down << BobUI::Key_PageUp << BobUI::Key_PageUp << BobUI::Key_Down << BobUI::Key_Right;
    BOBUIest::newRow("Symmetric seq, horizontal")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, horizontal, appearance inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, horizontal, controls inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, horizontal, both inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, vertical")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, vertical, appearance inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, vertical, controls inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 50;  // expected position

    BOBUIest::newRow("Symmetric seq, vertical, both inverted")
            << 50   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 50;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Home;
    BOBUIest::newRow("Home, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Home, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 0;   // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_End;
    BOBUIest::newRow("End, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("End, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 0    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 100; // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_End << BobUI::Key_Up;
    BOBUIest::newRow("Past end, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("Past end, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("Past end, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 97;  // expected position

    BOBUIest::newRow("Past end, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 97;  // expected position

    BOBUIest::newRow("Past end, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("Past end, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 100; // expected position

    BOBUIest::newRow("Past end, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 97;  // expected position

    BOBUIest::newRow("Past end, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 97;  // expected position

    list = QList<BobUI::Key>();
    list << BobUI::Key_Home << BobUI::Key_Down;
    BOBUIest::newRow("Past home, horizontal")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Past home, horizontal, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Past home, horizontal, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 3;   // expected position

    BOBUIest::newRow("Past home, horizontal, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 3;   // expected position

    BOBUIest::newRow("Past home, vertical")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Past home, vertical, appearance inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << false// inverted controls
            << list // key sequence
            << 0;   // expected position

    BOBUIest::newRow("Past home, vertical, controls inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << false// inverted appearance
            << true // inverted controls
            << list // key sequence
            << 3;   // expected position

    BOBUIest::newRow("Past home, vertical, both inverted")
            << 10   // initial position
            << 0    // minimum
            << 100  // maximum
            << 3    // single step size
            << 3    // page step size
            << true // inverted appearance
            << true // inverted controls
            << list // key sequence
            << 3;   // expected position
}

void tst_QAbstractSlider::keyPressed()
{
    QFETCH(int, initialSliderPosition);
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, stepSize);
    QFETCH(int, pageSize);
    QFETCH(bool, invertedAppearance);
    QFETCH(bool, invertedControls);
    QFETCH(QList<BobUI::Key>, keySequence);
    QFETCH(int, expectedSliderPosition);

    slider->setRange(minimum,maximum);
    slider->setSliderPosition(initialSliderPosition);
    slider->setSingleStep(stepSize);
    slider->setPageStep(pageSize);
    slider->setOrientation(BobUI::Horizontal);
    slider->setInvertedAppearance(invertedAppearance);
    slider->setInvertedControls(invertedControls);
    for (int i=0;i<keySequence.size();i++) {
        BOBUIest::keyClick(slider, keySequence.at(i));
    }
    QCOMPARE(slider->sliderPosition(), expectedSliderPosition);
}

#if BOBUI_CONFIG(wheelevent)
void tst_QAbstractSlider::wheelEvent_data()
{
    BOBUIest::addColumn<int>("initialSliderPosition");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("singleStep");
    BOBUIest::addColumn<int>("pageStep");
    BOBUIest::addColumn<bool>("invertedControls");
    BOBUIest::addColumn<int>("wheelScrollLines");
    BOBUIest::addColumn<bool>("withModifiers"); // use keyboard modifiers while scrolling? (CTRL and SHIFT)
    BOBUIest::addColumn<int>("deltaMultiple"); // multiples of WHEEL_DELTA
    BOBUIest::addColumn<BobUI::Orientation>("sliderOrientation");
    BOBUIest::addColumn<BobUI::Orientation>("wheelOrientation");
    BOBUIest::addColumn<int>("expectedSliderPosition");
    BOBUIest::addColumn<QPoint>("distanceFromBottomRight"); // mpointer's distance from bottom-right corner of widget

    BOBUIest::newRow("Normal data step") << 0                                  // initial position
                                   << 0                                  // minimum
                                   << 100                                // maximum
                                   << 1                                  // single step
                                   << 100                                // page step
                                   << false                              // inverted controls
                                   << 20                                 // wheel scroll lines
                                   << false                              // with modifiers
                                   << 1                                  // delta
                                   << BobUI::Vertical                       // orientation of slider
                                   << BobUI::Vertical                       // orientation of wheel
                                   << 20                                 // expected position after
                                   << QPoint(0,0);

    BOBUIest::newRow("Normal data page") << 0                                  // initial position
                                   << 0                                  // minimum
                                   << 100                                // maximum
                                   << 100                                // single step
                                   << 1                                  // page step
                                   << false                              // inverted controls
                                   << 20                                 // wheel scroll lines
                                   << false                              // with modifiers
                                   << 1                                  // delta
                                   << BobUI::Vertical                       // orientation of slider
                                   << BobUI::Vertical                       // orientation of wheel
#ifndef Q_OS_MAC
                                   << 1                                  // expected position after
#else
                                   // We don't restrict scrolling to pageStep on Mac
                                   << 100                                // expected position after
#endif
                                   << QPoint(1,1);
    BOBUIest::newRow("Different orientation") << 0                             // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 100                           // single step
                                        << 1                             // page step
                                        << false                         // inverted controls
                                        << 20                            // wheel scroll lines
                                        << false                         // with modifiers
                                        << 1                             // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Vertical                  // orientation of wheel
#ifndef Q_OS_MAC
                                        << 1                             // expected position after
#else
                                        // We don't restrict scrolling to pageStep on Mac
                                        << 100                           // expected position after
#endif
                                        << QPoint(1,1);

    BOBUIest::newRow("Different orientation2")<< 0                             // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 100                           // single step
                                        << 1                             // page step
                                        << false                         // inverted controls
                                        << 20                            // wheel scroll lines
                                        << false                         // with modifiers
                                        << 1                             // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Vertical                  // orientation of wheel
#ifndef Q_OS_MAC
                                        << 1                             // expected position after
#else
                                        // We don't restrict scrolling to pageStep on Mac
                                        << 100                           // expected position after
#endif
                                        << QPoint(0,0);

    BOBUIest::newRow("Inverted controls")     << 50                            // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 1                             // single step
                                        << 100                           // page step
                                        << true                          // inverted controls
                                        << 20                            // wheel scroll lines
                                        << false                         // with modifiers
                                        << -1                            // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Horizontal                // orientation of wheel
                                        << 30                            // expected position after
                                        << QPoint(1,1);

    BOBUIest::newRow("Past end")              << 50                            // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 26                            // single step
                                        << 100                           // page step
                                        << false                         // inverted controls
                                        << 1                             // wheel scroll lines
                                        << false                         // with modifiers
                                        << -2                            // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Horizontal                // orientation of wheel
                                        << 100                           // expected position after
                                        << QPoint(0,0);

    BOBUIest::newRow("Past start")            << 50                            // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 26                            // single step
                                        << 100                           // page step
                                        << false                         // inverted controls
                                        << 1                             // wheel scroll lines
                                        << false                         // with modifiers
                                        << 2                             // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Horizontal                // orientation of wheel
                                        << 0                             // expected position after
                                        << QPoint(0,0);

    BOBUIest::newRow("With modifiers")        << 50                            // initial position
                                        << 0                             // minimum
                                        << 100                           // maximum
                                        << 1                             // single step
                                        << 40                            // page step
                                        << false                         // inverted controls
                                        << 20                            // wheel scroll lines
                                        << true                          // with modifiers
                                        << -1                            // delta
                                        << BobUI::Horizontal                // orientation of slider
                                        << BobUI::Horizontal                // orientation of wheel
                                        << 90                            // expected position after
                                        << QPoint(0,0);

}

void tst_QAbstractSlider::wheelEvent()
{
    QFETCH(int,initialSliderPosition);
    QFETCH(int,minimum);
    QFETCH(int,maximum);
    QFETCH(int,singleStep);
    QFETCH(int,pageStep);
    QFETCH(bool,invertedControls);
    QFETCH(int,wheelScrollLines);
    QFETCH(bool,withModifiers);
    QFETCH(int,deltaMultiple);
    QFETCH(BobUI::Orientation, sliderOrientation);
    QFETCH(BobUI::Orientation, wheelOrientation);
    QFETCH(int,expectedSliderPosition);
    QFETCH(QPoint,distanceFromBottomRight);

    QCoreApplication *applicationInstance = QCoreApplication::instance();
    QVERIFY(applicationInstance != 0);
    QApplication::setWheelScrollLines(wheelScrollLines);

    slider->setRange(minimum,maximum);
    slider->setSliderPosition(initialSliderPosition);
    slider->setSingleStep(singleStep);
    slider->setPageStep(pageStep);
    slider->setInvertedControls(invertedControls);
    slider->setOrientation(sliderOrientation);

    BobUI::KeyboardModifier k = withModifiers ? BobUI::ControlModifier : BobUI::NoModifier;

    const QPoint wheelPoint = slider->rect().bottomRight() + distanceFromBottomRight;
    const QPoint angleDelta(wheelOrientation == BobUI::Horizontal ? WHEEL_DELTA * deltaMultiple : 0,
                            wheelOrientation == BobUI::Vertical ? WHEEL_DELTA * deltaMultiple : 0);
    QWheelEvent event(wheelPoint, slider->mapToGlobal(wheelPoint), QPoint(), angleDelta,
                      BobUI::NoButton, k, BobUI::NoScrollPhase, false);
    QVERIFY(applicationInstance->sendEvent(slider,&event));
#ifdef Q_OS_MAC
    QEXPECT_FAIL("Normal data page", "BOBUIBUG-23679", Continue);
    QEXPECT_FAIL("Different orientation", "BOBUIBUG-23679", Continue);
    QEXPECT_FAIL("Different orientation2", "BOBUIBUG-23679", Continue);
#endif
    QCOMPARE(slider->sliderPosition(),expectedSliderPosition);

    slider->setSliderPosition(initialSliderPosition);
    k = withModifiers ? BobUI::ShiftModifier : BobUI::NoModifier;
    QWheelEvent event2 = QWheelEvent(wheelPoint, slider->mapToGlobal(wheelPoint), QPoint(), angleDelta,
                                     BobUI::NoButton, k, BobUI::NoScrollPhase, false);
    QSignalSpy spy1(slider, SIGNAL(actionTriggered(int)));
    QSignalSpy spy2(slider, SIGNAL(valueChanged(int)));
    QVERIFY(applicationInstance->sendEvent(slider,&event2));
#ifdef Q_OS_MAC
    QEXPECT_FAIL("Normal data page", "BOBUIBUG-23679", Continue);
    QEXPECT_FAIL("Different orientation", "BOBUIBUG-23679", Continue);
    QEXPECT_FAIL("Different orientation2", "BOBUIBUG-23679", Continue);
#endif
    QCOMPARE(slider->sliderPosition(),expectedSliderPosition);
    int expectedSignalCount = (initialSliderPosition == expectedSliderPosition) ? 0 : 1;
    QCOMPARE(spy1.size(), expectedSignalCount);
    QCOMPARE(spy2.size(), expectedSignalCount);
    if (expectedSignalCount)
        QVERIFY(actionTriggeredTimeStamp < valueChangedTimeStamp);
}

void tst_QAbstractSlider::fineGrainedWheelEvent_data()
{
    BOBUIest::addColumn<bool>("invertedControls");
    BOBUIest::newRow("invertedControls=false") << false;
    BOBUIest::newRow("invertedControls=true") << true;
}

void tst_QAbstractSlider::fineGrainedWheelEvent()
{
    QFETCH(bool, invertedControls);

    QCoreApplication *applicationInstance = QCoreApplication::instance();
    QVERIFY(applicationInstance != 0);
    QApplication::setWheelScrollLines(3);

    slider->setRange(0, 10);
    slider->setSingleStep(1);
    slider->setPageStep(10);
    slider->setInvertedControls(invertedControls);
    slider->setOrientation(BobUI::Vertical);
    slider->setSliderPosition(0);

    const int singleStepDelta = invertedControls ? (-WHEEL_DELTA / 3) : (WHEEL_DELTA / 3);
    const QPoint wheelPoint = slider->rect().bottomRight();
    QWheelEvent eventDown(wheelPoint, slider->mapToGlobal(wheelPoint), QPoint(), QPoint(0, singleStepDelta / 2),
                          BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    QVERIFY(applicationInstance->sendEvent(slider,&eventDown));
    QCOMPARE(slider->sliderPosition(), 0);
    QVERIFY(applicationInstance->sendEvent(slider,&eventDown));
    QCOMPARE(slider->sliderPosition(), 1);

    QWheelEvent eventUp(wheelPoint, slider->mapToGlobal(wheelPoint), QPoint(), QPoint(0, -singleStepDelta / 2),
                          BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    QVERIFY(applicationInstance->sendEvent(slider,&eventUp));
    QCOMPARE(slider->sliderPosition(), 1);
    QVERIFY(applicationInstance->sendEvent(slider,&eventUp));
    QCOMPARE(slider->sliderPosition(), 0);
    QVERIFY(applicationInstance->sendEvent(slider,&eventUp));
    QCOMPARE(slider->sliderPosition(), 0);
    QVERIFY(applicationInstance->sendEvent(slider,&eventUp));
    QCOMPARE(slider->sliderPosition(), 0);

    QVERIFY(applicationInstance->sendEvent(slider,&eventDown));
    QCOMPARE(slider->sliderPosition(), 0);
    QVERIFY(applicationInstance->sendEvent(slider,&eventDown));
    QCOMPARE(slider->sliderPosition(), 1);
}

#endif // BOBUI_CONFIG(wheelevent)

void tst_QAbstractSlider::sliderPressedReleased_data()
{
    BOBUIest::addColumn<int>("control");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<uint>("subControl");
    BOBUIest::addColumn<int>("expectedCount");

    BOBUIest::newRow("slider on the handle") << int(QStyle::CC_Slider)
                               << 0
                               << 20
                               << uint(QStyle::SC_SliderHandle)
                               << 1;

    BOBUIest::newRow("slider on the groove") << int(QStyle::CC_Slider)
                               << 0
                               << 20
                               << uint(QStyle::SC_SliderGroove)
                               << ((qApp->style()->styleHint(QStyle::SH_Slider_AbsoluteSetButtons) & BobUI::LeftButton) ? 1 : 0);

    BOBUIest::newRow("scrollbar on the handle") << int(QStyle::CC_ScrollBar)
                               << 0
                               << 20
                               << uint(QStyle::SC_ScrollBarSlider)
                               << 1;

    BOBUIest::newRow("scrollbar on the groove") << int(QStyle::CC_ScrollBar)
                               << 0
                               << 20
                               << uint(QStyle::SC_ScrollBarGroove)
                               << 0;
}

void tst_QAbstractSlider::sliderPressedReleased()
{
    QFETCH(int, control);
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(uint, subControl);
    QFETCH(int, expectedCount);

    QWidget topLevel;
    setFrameless(&topLevel);
    QAbstractSlider *slider;
    switch (control) {
    default:
        qWarning("Bad input into test, leaving");
        return;
        break;
    case QStyle::CC_Slider:
        slider = new QSlider(&topLevel);
        slider->setLayoutDirection(BobUI::LeftToRight);   // Makes "upside down" much easier to compute
        break;
    case QStyle::CC_ScrollBar:
        slider = new QScrollBar(&topLevel);
        break;
    }


    slider->setMinimum(minimum);
    slider->setMaximum(maximum);
    slider->setValue(0);
    slider->setOrientation(BobUI::Vertical);
    slider->resize(slider->sizeHint().width(), slider->sizeHint().height() + 100);
    QSignalSpy spy1(slider, SIGNAL(sliderPressed()));
    QSignalSpy spy2(slider, SIGNAL(sliderReleased()));

    // Mac Style requires the control to be active to get the correct values...
    topLevel.show();
    slider->activateWindow();

    QStyleOptionSlider option;
    option.initFrom(slider);
    option.upsideDown = control == QStyle::CC_Slider ? !slider->invertedAppearance()
                                                     : slider->invertedAppearance();
    option.subControls = QStyle::SC_None;
    option.activeSubControls = QStyle::SC_None;
    option.orientation = slider->orientation();
    option.maximum = maximum;
    option.minimum = minimum;
    option.sliderPosition = slider->value();
    option.sliderValue = slider->value();
    option.singleStep = slider->singleStep();
    option.pageStep = slider->pageStep();
    QRect rect = slider->style()->subControlRect(QStyle::ComplexControl(control), &option,
                                                 QStyle::SubControl(subControl), slider);

    if (qApp->style()->styleHint(QStyle::SH_ScrollBar_LeftClickAbsolutePosition))
        QSKIP("The result depends on system setting on mac");

    BOBUIest::mousePress(slider, BobUI::LeftButton, {},
                      QPoint(rect.center().x() + 2, rect.center().y() + 2));
    QCOMPARE(spy1.size(), expectedCount);
    BOBUIest::mouseRelease(slider, BobUI::LeftButton, {}, rect.center());
    QCOMPARE(spy2.size(), expectedCount);

    delete slider;
}

void tst_QAbstractSlider::sliderMoved_data()
{
    BOBUIest::addColumn<int>("control");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("position");
    BOBUIest::addColumn<bool>("sliderDown");
    BOBUIest::addColumn<int>("expectedCount");

    BOBUIest::newRow("slider pressed") << int(QStyle::CC_Slider)
                               << 0
                               << 20
                               << 10
                               << true
                               << 1;

    BOBUIest::newRow("slider not pressed") << int(QStyle::CC_Slider)
                               << 0
                               << 20
                               << 10
                               << false
                               << 0;

    BOBUIest::newRow("scrollbar pressed") << int(QStyle::CC_ScrollBar)
                               << 0
                               << 20
                               << 10
                               << true
                               << 1;

    BOBUIest::newRow("scrollbar not pressed") << int(QStyle::CC_ScrollBar)
                               << 0
                               << 20
                               << 10
                               << false
                               << 0;
}

void tst_QAbstractSlider::sliderMoved()
{
    QFETCH(int, control);
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, position);
    QFETCH(bool, sliderDown);
    QFETCH(int, expectedCount);
    QAbstractSlider *slider;
    switch (control) {
    default:
        slider = 0;
        break;
    case QStyle::CC_Slider:
        slider = new QSlider;
        break;
    case QStyle::CC_ScrollBar:
        slider = new QScrollBar;
        break;
    }
    QSignalSpy spy(slider, SIGNAL(sliderMoved(int)));

    slider->setMinimum(minimum);
    slider->setMaximum(maximum);
    slider->setSliderDown(sliderDown);
    slider->setSliderPosition(position);
    QCOMPARE(spy.size(), expectedCount);

    delete slider;
}

void tst_QAbstractSlider::setOrientation()
{
    QSlider slider(0);

    QSizePolicy sp = slider.sizePolicy();
    slider.setOrientation(slider.orientation());
    QSizePolicy sp2 = slider.sizePolicy();
    QCOMPARE(sp, sp2);

    slider.setOrientation(BobUI::Horizontal);
    sp = slider.sizePolicy();
    slider.setOrientation(BobUI::Vertical);
    sp2 = slider.sizePolicy();

    QVERIFY(sp != sp2);
    sp2.transpose();
    QCOMPARE(sp, sp2);
}


void tst_QAbstractSlider::rangeChanged_data()
{
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("newMin");
    BOBUIest::addColumn<int>("newMax");
    BOBUIest::addColumn<int>("expectedCount");

    BOBUIest::newRow("no change")
                               << 0
                               << 20
                               << 0
                               << 20
                               << 0;

    BOBUIest::newRow("min change")
                               << 0
                               << 20
                               << 10
                               << 20
                               << 1;
    BOBUIest::newRow("max change")
                               << 0
                               << 20
                               << 0
                               << 30
                               << 1;

    BOBUIest::newRow("both change")
                               << 0
                               << 20
                               << 10
                               << 30
                               << 1;
}

void tst_QAbstractSlider::rangeChanged()
{
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, newMin);
    QFETCH(int, newMax);
    QFETCH(int, expectedCount);
    QSlider slider;
    slider.setRange(minimum, maximum);
    QSignalSpy spy(&slider, SIGNAL(rangeChanged(int,int)));
    slider.setRange(newMin, newMax);
    QCOMPARE(spy.size(), expectedCount);
}

void tst_QAbstractSlider::setSliderPosition_data()
{
    BOBUIest::addColumn<bool>("tracking");
    BOBUIest::addColumn<bool>("down");

    BOBUIest::newRow("tracking, slider down")
                               << true
                               << true;
    BOBUIest::newRow("tracking, slider not down")
                               << true
                               << false;
    BOBUIest::newRow("no tracking, slider down")
                               << false
                               << true;
    BOBUIest::newRow("no tracking, slider not down")
                               << false
                               << false;
}

void tst_QAbstractSlider::setSliderPosition()
{
    QFETCH(bool, tracking);
    QFETCH(bool, down);
    const int minimum = 0;
    const int maximum = 100;
    const int initialValue = 50;
    const int targetPosition = 75;
    slider->setRange(minimum, maximum);
    slider->setTracking(tracking);
    slider->setSliderDown(down);
    slider->setValue(initialValue);
    QCOMPARE(slider->sliderPosition(), initialValue);
    QSignalSpy spy1(slider, SIGNAL(sliderMoved(int)));
    QSignalSpy spy2(slider, SIGNAL(valueChanged(int)));
    slider->setSliderPosition(targetPosition);
    QCOMPARE(slider->sliderPosition(), targetPosition);
    QCOMPARE(spy1.size(), down ? 1 : 0);
    QCOMPARE(spy2.size(), tracking ? 1 : 0);
    QCOMPARE(slider->value(), tracking ? targetPosition : initialValue);
    if (tracking && down)
        QVERIFY(sliderMovedTimeStamp < valueChangedTimeStamp);
}

void tst_QAbstractSlider::setValue_data()
{
    BOBUIest::addColumn<bool>("down");

    BOBUIest::newRow("slider down")
                               << true;
    BOBUIest::newRow("slider not down")
                               << false;
}

void tst_QAbstractSlider::setValue()
{
    QFETCH(bool, down);
    const int minimum = 0;
    const int maximum = 100;
    slider->setRange(minimum, maximum);
    slider->setSliderDown(down);
    slider->setValue(49); // to force a valueChanged() below
    QSignalSpy spy1(slider, &QAbstractSlider::sliderMoved);
    QSignalSpy spy2(slider, &QAbstractSlider::valueChanged);
    QSignalSpy spy3(slider, &QAbstractSlider::actionTriggered);
    slider->setValue(50);
    QCOMPARE(spy1.size(), down ? 1 : 0);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy3.size(), 0);
    QCOMPARE(slider->value(), reportedValue);
    QCOMPARE(slider->sliderPosition(), reportedSliderPosition);
    if (down)
        QVERIFY(sliderMovedTimeStamp < valueChangedTimeStamp);

    slider->setValue(50);
    QApplication::processEvents();
    QCOMPARE(spy2.size(), 1);
}

void tst_QAbstractSlider::waitUntilTimeElapsed(const QElapsedTimer &t, int ms)
{
    const int eps = 80;
    while (t.elapsed() < ms + eps)
        BOBUIest::qWait(qMax(int(ms - t.elapsed() + eps), 25));
}

void tst_QAbstractSlider::setRepeatAction()
{
    slider->setRange(0, 1000);
    slider->setValue(55);
    slider->setPageStep(10);
    QSignalSpy spy(slider, SIGNAL(actionTriggered(int)));

    // Start repeat action with initial delay of 500 ms, and then repeating
    // every 250 ms.
    slider->setRepeatAction(QAbstractSlider::SliderPageStepAdd, 500, 250);
    QCOMPARE(spy.size(), 0);
    QCOMPARE(slider->value(), 55);

    QElapsedTimer t;
    t.start();
    BOBUIest::qWait(300);
    QCOMPARE(spy.size(), 0);
    QCOMPARE(slider->value(), 55);

    waitUntilTimeElapsed(t, 550);
    BOBUIRY_COMPARE(spy.size(), 1);
    QCOMPARE(slider->value(), 65);
    QCOMPARE(spy.at(0).at(0).toUInt(), (uint)QAbstractSlider::SliderPageStepAdd);

    waitUntilTimeElapsed(t, 790);
    BOBUIRY_COMPARE(spy.size(), 2);
    QCOMPARE(slider->value(), 75);
    QCOMPARE(spy.at(1).at(0).toUInt(), (uint)QAbstractSlider::SliderPageStepAdd);

    waitUntilTimeElapsed(t, 1790);
    BOBUIRY_COMPARE(spy.size(), 6);
    QCOMPARE(slider->value(), 115);
    QCOMPARE(spy.at(4).at(0).toUInt(), (uint)QAbstractSlider::SliderPageStepAdd);
    QCOMPARE(spy.at(5).at(0).toUInt(), (uint)QAbstractSlider::SliderPageStepAdd);

    slider->setRepeatAction(QAbstractSlider::SliderNoAction);
    QCOMPARE(spy.size(), 6);
    QCOMPARE(slider->value(), 115);

    BOBUIest::qWait(300);
    QCOMPARE(spy.size(), 6);
    QCOMPARE(slider->value(), 115);
}

void tst_QAbstractSlider::connectedSliders()
{
    Slider *slider2 = new Slider(topLevel);
    connect(slider, SIGNAL(rangeChanged(int,int)), slider2, SLOT(setRange(int,int)));
    const int sliderlow = 13;
    const int sliderhigh = 1017;
    slider->setRange(sliderlow, sliderhigh);
    QCOMPARE(slider2->minimum(), sliderlow);
    QCOMPARE(slider2->maximum(), sliderhigh);
    delete slider2;
}



template<typename Slider>
void tst_QAbstractSlider::sliderChange_impl()
{
    struct Tracker : Slider {
        using Slider::Slider;
        using QAbstractSlider::SliderChange; // make it public

        std::vector<QAbstractSlider::SliderChange> changes;

        void sliderChange(QAbstractSlider::SliderChange c) override
        {
            Slider::sliderChange(c);
            changes.push_back(c);
        }
    };
    using SliderChange = typename Tracker::SliderChange;

    Tracker sl;
    QCOMPARE(sl.changes.size(), 0U);

    sl.setRange(0, 100'000);
    QCOMPARE(sl.changes.size(), 1U);
    QCOMPARE(sl.changes.back(), SliderChange::SliderRangeChange);
    sl.changes.clear();

    sl.setOrientation(sl.orientation() == BobUI::Horizontal ? BobUI::Vertical
                      /* else */                         : BobUI::Horizontal);
    QCOMPARE(sl.changes.size(), 1U);
    QCOMPARE(sl.changes.back(), SliderChange::SliderOrientationChange);
    sl.changes.clear();

    sl.setPageStep(1025); // unlikely to be the default ;)
    QCOMPARE(sl.changes.size(), 1U);
    QCOMPARE(sl.changes.back(), SliderChange::SliderStepsChange);
    sl.changes.clear();

    sl.setSingleStep(42); // unlikely to be the default ;)
    QCOMPARE(sl.changes.size(), 1U);
    QCOMPARE(sl.changes.back(), SliderChange::SliderStepsChange);
    sl.changes.clear();

    sl.setValue(131); // unlikely to be the default ;)
    QCOMPARE(sl.changes.size(), 1U);
    QCOMPARE(sl.changes.back(), SliderChange::SliderValueChange);
    sl.changes.clear();
}

BOBUIEST_MAIN(tst_QAbstractSlider)
#include "tst_qabstractslider.moc"
