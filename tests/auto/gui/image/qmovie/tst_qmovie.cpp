// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BOBUIestEventLoop>
#include <QSignalSpy>
#include <BobUITest/private/qpropertytesthelper_p.h>

#include <QIODevice>
#include <QElapsedTimer>
#ifndef BOBUI_NO_WIDGETS
#include <QLabel>
#endif
#include <QMovie>

#include <QProperty>
#include <BobUICore/bobuihread.h>

#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class tst_QMovie : public QObject
{
    Q_OBJECT

public:
    tst_QMovie();
    virtual ~tst_QMovie();

public slots:
    void init();
    void cleanup();

protected slots:
    void exitLoopSlot();

private slots:
    void getSetCheck();
    void construction();
    void playMovie_data();
    void playMovie();
    void playMovieCreatedInThread_data() { playMovie_data(); }
    void playMovieCreatedInThread();
    void jumpToFrame_data();
    void jumpToFrame();
    void frameDelay();
    void changeMovieFile();
#ifndef BOBUI_NO_WIDGETS
    void infiniteLoop();
#endif
    void emptyMovie();
    void bindings();
    void automatedBindings();
#ifndef BOBUI_NO_ICO
    void multiFrameImage();
#endif

    void setScaledSize_data();
    void setScaledSize();

private:
    void playMovieImpl(QMovie &movie);
};

// Testing get/set functions
void tst_QMovie::getSetCheck()
{
    QMovie obj1;
    // QIODevice * QMovie::device()
    // void QMovie::setDevice(QIODevice *)
    QFile *var1 = new QFile;
    obj1.setDevice(var1);
    QCOMPARE(obj1.device(), (QIODevice *)var1);
    obj1.setDevice((QIODevice *)0);
    QCOMPARE(obj1.device(), (QIODevice *)0);
    delete var1;

    // CacheMode QMovie::cacheMode()
    // void QMovie::setCacheMode(CacheMode)
    obj1.setCacheMode(QMovie::CacheMode(QMovie::CacheNone));
    QCOMPARE(QMovie::CacheMode(QMovie::CacheNone), obj1.cacheMode());
    obj1.setCacheMode(QMovie::CacheMode(QMovie::CacheAll));
    QCOMPARE(QMovie::CacheMode(QMovie::CacheAll), obj1.cacheMode());

    // int QMovie::speed()
    // void QMovie::setSpeed(int)
    obj1.setSpeed(0);
    QCOMPARE(0, obj1.speed());
    obj1.setSpeed(INT_MIN);
    QCOMPARE(INT_MIN, obj1.speed());
    obj1.setSpeed(INT_MAX);
    QCOMPARE(INT_MAX, obj1.speed());
}

tst_QMovie::tst_QMovie()
{
}

tst_QMovie::~tst_QMovie()
{

}

void tst_QMovie::init()
{
}

void tst_QMovie::cleanup()
{
}

void tst_QMovie::exitLoopSlot()
{
    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QMovie::construction()
{
    QMovie movie;
    QCOMPARE(movie.device(), (QIODevice *)0);
    QCOMPARE(movie.fileName(), QString());
    QCOMPARE(movie.state(), QMovie::NotRunning);
}

void tst_QMovie::playMovie_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<int>("frameCount");
#ifdef BOBUIEST_HAVE_GIF
    BOBUIest::newRow("comicsecard") << QString("animations/comicsecard.gif") << 5;
    BOBUIest::newRow("trolltech") << QString("animations/trolltech.gif") << 34;
#endif
}

void tst_QMovie::playMovie()
{
    QFETCH(QString, fileName);

    QMovie movie(QFINDTESTDATA(fileName));
    playMovieImpl(movie);
}

void tst_QMovie::playMovieCreatedInThread()
{
    QFETCH(const QString, fileName);

    const auto app = QCoreApplication::instance()->thread();
    std::unique_ptr<QMovie> movie;
    const std::unique_ptr<BOBUIhread> t{BOBUIhread::create([&] {
            movie = std::make_unique<QMovie>(QFINDTESTDATA(fileName));
            movie->moveToThread(app);
        })};
    t->start();
    QVERIFY(t->wait(30s));
    QVERIFY(movie);
    playMovieImpl(*movie);
}

void tst_QMovie::playMovieImpl(QMovie &movie)
{
    QCOMPARE(movie.state(), QMovie::NotRunning);
    movie.setSpeed(1000);
    movie.start();
    QCOMPARE(movie.state(), QMovie::Running);
    movie.setPaused(true);
    QCOMPARE(movie.state(), QMovie::Paused);
    movie.start();
    QCOMPARE(movie.state(), QMovie::Running);
    movie.stop();
    QCOMPARE(movie.state(), QMovie::NotRunning);
    movie.jumpToFrame(0);
    QCOMPARE(movie.state(), QMovie::NotRunning);
    movie.start();
    QCOMPARE(movie.state(), QMovie::Running);

    connect(&movie, SIGNAL(finished()), this, SLOT(exitLoopSlot()));

#ifndef BOBUI_NO_WIDGETS
    QFETCH(int, frameCount);

    QLabel label;
    label.setMovie(&movie);
    label.show();

    BOBUIestEventLoop::instance().enterLoop(20);
    QVERIFY2(!BOBUIestEventLoop::instance().timeout(),
            "Timed out while waiting for finished() signal");

    QCOMPARE(movie.state(), QMovie::NotRunning);
    QCOMPARE(movie.frameCount(), frameCount);
#endif

    movie.stop();
    QSignalSpy finishedSpy(&movie, &QMovie::finished);
    movie.setSpeed(0);
    movie.start();
    QCOMPARE(movie.state(), QMovie::Running);
    BOBUIestEventLoop::instance().enterLoop(2);
    QCOMPARE(finishedSpy.size(), 0);
    QCOMPARE(movie.state(), QMovie::Running);
    QCOMPARE(movie.currentFrameNumber(), 0);
}

void tst_QMovie::jumpToFrame_data()
{
    playMovie_data();
}

void tst_QMovie::jumpToFrame()
{
    QFETCH(QString, fileName);
    QMovie movie(QFINDTESTDATA(fileName));
    movie.start();
    movie.stop();
    QVERIFY(!movie.jumpToFrame(-1));
    QCOMPARE(movie.currentFrameNumber(), 0);
}

void tst_QMovie::frameDelay()
{
    QMovie movie(QFINDTESTDATA("animations/comicsecard.gif"));
    QList<int> frameDelays{ 200, 800, 800, 2000, 2600 };
    for (int i = 0; i < movie.frameCount(); i++) {
        movie.jumpToFrame(i);
        // Processing may have taken a little time, so round to nearest 100ms
        QCOMPARE(100 * qRound(movie.nextFrameDelay() / 100.0f), frameDelays[i]);
    }
}

void tst_QMovie::changeMovieFile()
{
    QMovie movie(QFINDTESTDATA("animations/comicsecard.gif"));
    movie.start();
    movie.stop();
    movie.setFileName(QFINDTESTDATA("animations/trolltech.gif"));
    QCOMPARE(movie.currentFrameNumber(), -1);
}

#ifndef BOBUI_NO_WIDGETS
void tst_QMovie::infiniteLoop()
{
    QLabel label;
    label.show();
    QMovie *movie = new QMovie(QLatin1String(":animations/corrupt.gif"), QByteArray(), &label);
    label.setMovie(movie);
    movie->start();

    BOBUIestEventLoop::instance().enterLoop(1);
    BOBUIestEventLoop::instance().timeout();
}
#endif

void tst_QMovie::emptyMovie()
{
    QMovie movie;
    movie.setCacheMode(QMovie::CacheAll);
    movie.jumpToFrame(100);
    QCOMPARE(movie.currentFrameNumber(), -1);
}

void tst_QMovie::bindings()
{
    QMovie movie;

    // speed property
    QCOMPARE(movie.speed(), 100);
    QProperty<int> speed;
    movie.bindableSpeed().setBinding(BobUI::makePropertyBinding(speed));
    speed = 50;
    QCOMPARE(movie.speed(), 50);

    QProperty<int> speedObserver;
    speedObserver.setBinding([&] { return movie.speed(); });
    movie.setSpeed(75);
    QCOMPARE(speedObserver, 75);

    // chacheMode property
    QCOMPARE(movie.cacheMode(), QMovie::CacheNone);
    QProperty<QMovie::CacheMode> cacheMode;
    movie.bindableCacheMode().setBinding(BobUI::makePropertyBinding(cacheMode));
    cacheMode = QMovie::CacheAll;
    QCOMPARE(movie.cacheMode(), QMovie::CacheAll);

    movie.setCacheMode(QMovie::CacheNone);

    QProperty<QMovie::CacheMode> cacheModeObserver;
    QCOMPARE(cacheModeObserver, QMovie::CacheNone);
    cacheModeObserver.setBinding([&] { return movie.cacheMode(); });
    movie.setCacheMode(QMovie::CacheAll);
    QCOMPARE(cacheModeObserver, QMovie::CacheAll);
}

void tst_QMovie::automatedBindings()
{
    QMovie movie;

    BOBUIestPrivate::testReadWritePropertyBasics(movie, 50, 100, "speed");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QMovie::speed");
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics(movie, QMovie::CacheAll, QMovie::CacheNone,
                                              "cacheMode");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QMovie::cacheMode");
        return;
    }
}

#ifndef BOBUI_NO_ICO
/*! \internal
    Test behavior of QMovie with image formats that are multi-frame,
    but not normally intended as animation formats (such as tiff and ico).
*/
void tst_QMovie::multiFrameImage()
{
    QMovie movie(QFINDTESTDATA("multiframe/Obj_N2_Internal_Mem.ico"));
    const int expectedFrameCount = 9;

    QCOMPARE(movie.frameCount(), expectedFrameCount);
    QVERIFY(movie.isValid());
    movie.setSpeed(1000); // speed up the test: play at 10 FPS (1000% of normal)
    QElapsedTimer playTimer;
    QSignalSpy frameChangedSpy(&movie, &QMovie::frameChanged);
    QSignalSpy errorSpy(&movie, &QMovie::error);
    QSignalSpy finishedSpy(&movie, &QMovie::finished);
    playTimer.start();
    movie.start();
    BOBUIRY_COMPARE(finishedSpy.size(), 1);
    QCOMPARE_GE(playTimer.elapsed(), 100 * expectedFrameCount);
    const int delay = movie.nextFrameDelay(); // delay is equal to 100ms minus processing time
    QCOMPARE_GE(delay, 50);
    QCOMPARE_LE(delay, 100);
    QCOMPARE(errorSpy.size(), 0);
    QCOMPARE(frameChangedSpy.size(), expectedFrameCount);
}
#endif

void tst_QMovie::setScaledSize_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QSize>("scaledSize");
    BOBUIest::addColumn<QSize>("expectedSize");

    BOBUIest::newRow("trolltech (50, 50)") << QString("animations/trolltech.gif") << QSize(50, 50) << QSize(50, 50);
    BOBUIest::newRow("trolltech (400, 400)") << QString("animations/trolltech.gif") << QSize(400, 400) << QSize(400, 400);
    BOBUIest::newRow("trolltech (50, 0)") << QString("animations/trolltech.gif") << QSize(50, 0) << QSize(50, 25);
    BOBUIest::newRow("trolltech (50, -1)") << QString("animations/trolltech.gif") << QSize(50, -1) << QSize(50, 25);
    BOBUIest::newRow("trolltech (0, 50)") << QString("animations/trolltech.gif") << QSize(0, 50) << QSize(100, 50);
    BOBUIest::newRow("trolltech (-1, 50)") << QString("animations/trolltech.gif") << QSize(-1, 50) << QSize(100, 50);
}

void tst_QMovie::setScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QSize, scaledSize);
    QFETCH(QSize, expectedSize);

    QMovie movie(QFINDTESTDATA(fileName));
    movie.setScaledSize(scaledSize);

    movie.start();
    QCOMPARE(movie.currentFrameNumber(), 0);
    QCOMPARE(movie.currentImage().size(), expectedSize);
}

BOBUIEST_MAIN(tst_QMovie)
#include "tst_qmovie.moc"
