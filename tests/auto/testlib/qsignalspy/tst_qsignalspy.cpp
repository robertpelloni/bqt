// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <BOBUIimer>

#include <qdatetime.h>

using namespace std::chrono_literals;
using namespace BobUI::StringLiterals;

class tst_QSignalSpy : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void sigFoo();

private slots:
    void spyWithoutArgs();
    void spyWithBasicArgs();
    void spyWithPointers();
    void spyWithBobUIClasses();
    void spyWithBasicBobUIClasses();
    void spyWithBobUITypedefs();

    void wait_signalEmitted();
    void wait_timeout();
    void wait_signalEmittedLater();
    void wait_signalEmittedTooLate();
    void wait_signalEmittedMultipleTimes();

    void spyFunctionPointerWithoutArgs();
    void spyFunctionPointerWithBasicArgs();
    void spyFunctionPointerWithPointers();
    void spyFunctionPointerWithBobUIClasses();
    void spyFunctionPointerWithCustomClass();
    void spyFunctionPointerWithBasicBobUIClasses();
    void spyFunctionPointerWithBobUITypedefs();

    void waitFunctionPointer_signalEmitted();
    void waitFunctionPointer_timeout();
    void waitFunctionPointer_signalEmittedLater();
    void waitFunctionPointer_signalEmittedTooLate();
    void waitFunctionPointer_signalEmittedMultipleTimes();

    void spyOnMetaMethod();

    void spyOnMetaMethod_invalid();
    void spyOnMetaMethod_invalid_data();

    void signalSpyDoesNotRaceOnCrossThreadSignal();
};

struct CustomType {};

class BobUITestObject: public QObject
{
    Q_OBJECT

public:
    BobUITestObject();

signals:
    void sig0();
    void sig1(int, int);
    void sigLong(long, long);
    void sig2(int *, int *);

public:
    QString slotResult;
    friend class tst_QSignalSpy;
};

BobUITestObject::BobUITestObject()
{
}

void tst_QSignalSpy::spyWithoutArgs()
{
    BobUITestObject obj;

    QSignalSpy spy(&obj, SIGNAL(sig0()));
    QCOMPARE(spy.size(), 0);

    emit obj.sig0();
    QCOMPARE(spy.size(), 1);
    emit obj.sig0();
    QCOMPARE(spy.size(), 2);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 0);
}

void tst_QSignalSpy::spyWithBasicArgs()
{
    BobUITestObject obj;
    QSignalSpy spy(&obj, SIGNAL(sig1(int,int)));

    emit obj.sig1(1, 2);
    QCOMPARE(spy.size(), 1);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(args.at(0).toInt(), 1);
    QCOMPARE(args.at(1).toInt(), 2);

    QSignalSpy spyl(&obj, SIGNAL(sigLong(long,long)));

    emit obj.sigLong(1l, 2l);
    args = spyl.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(qvariant_cast<long>(args.at(0)), 1l);
    QCOMPARE(qvariant_cast<long>(args.at(1)), 2l);
}


void tst_QSignalSpy::spyWithPointers()
{
    qRegisterMetaType<int *>("int*");

    BobUITestObject obj;
    QSignalSpy spy(&obj, SIGNAL(sig2(int*,int*)));

    int i1 = 1;
    int i2 = 2;

    emit obj.sig2(&i1, &i2);
    QCOMPARE(spy.size(), 1);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(*static_cast<int * const *>(args.at(0).constData()), &i1);
    QCOMPARE(*static_cast<int * const *>(args.at(1).constData()), &i2);
}

struct CustomType2;

class BobUITestObject2: public QObject
{
    Q_OBJECT
    friend class tst_QSignalSpy;

signals:
    void sig(QString);
    void sig2(const QDateTime &dt);
    void sig3(QObject *o);
    void sig4(QChar c);
    void sig5(const QVariant &v);
    void sig6(CustomType );
    void sig7(CustomType2 *);
};

void tst_QSignalSpy::spyWithBasicBobUIClasses()
{
    BobUITestObject2 obj;

    QSignalSpy spy(&obj, SIGNAL(sig(QString)));
    emit obj.sig(QString("bubu"));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).size(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("bubu"));

    QSignalSpy spy2(&obj, SIGNAL(sig5(QVariant)));
    QVariant val(45);
    emit obj.sig5(val);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy2.at(0).size(), 1);
    QCOMPARE(spy2.at(0).at(0), val);
    QCOMPARE(qvariant_cast<QVariant>(spy2.at(0).at(0)), val);
}

void tst_QSignalSpy::spyWithBobUIClasses()
{
    BobUITestObject2 obj;


    QSignalSpy spy(&obj, SIGNAL(sig2(QDateTime)));
    QDateTime dt = QDateTime::currentDateTime();
    emit obj.sig2(dt);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).size(), 1);
    QCOMPARE(spy.at(0).at(0).typeName(), "QDateTime");
    QCOMPARE(*static_cast<const QDateTime *>(spy.at(0).at(0).constData()), dt);
    QCOMPARE(spy.at(0).at(0).toDateTime(), dt);

    QSignalSpy spy2(&obj, SIGNAL(sig3(QObject*)));
    emit obj.sig3(this);
    QCOMPARE(*static_cast<QObject * const *>(spy2.value(0).value(0).constData()),
            (QObject *)this);
    QCOMPARE(qvariant_cast<QObject *>(spy2.value(0).value(0)), (QObject*)this);

    QSignalSpy spy3(&obj, SIGNAL(sig4(QChar)));
    emit obj.sig4(QChar('A'));
    QCOMPARE(qvariant_cast<QChar>(spy3.value(0).value(0)), QChar('A'));
}

class BobUITestObject3: public QObject
{
    Q_OBJECT
    friend class tst_QSignalSpy;

signals:
    void sig1(quint16);
    void sig2(qlonglong, qulonglong);
    void sig3(qint64, quint64);
};

void tst_QSignalSpy::spyWithBobUITypedefs()
{
    BobUITestObject3 obj;

//    QSignalSpy spy1(&obj, SIGNAL(sig1(quint16)));
//    emit obj.sig1(42);
//    QCOMPARE(spy1.value(0).value(0).toInt(), 42);

    QSignalSpy spy2(&obj, SIGNAL(sig2(qlonglong,qulonglong)));
    emit obj.sig2(42, 43);
    QCOMPARE(spy2.value(0).value(0).toInt(), 42);
    QCOMPARE(spy2.value(0).value(1).toInt(), 43);

//    QSignalSpy spy3(&obj, SIGNAL(sig3(qint64,quint64)));
//    emit obj.sig3(44, 45);
//    QCOMPARE(spy3.value(0).value(0).toInt(), 44);
//    QCOMPARE(spy3.value(0).value(1).toInt(), 45);
}

void tst_QSignalSpy::wait_signalEmitted()
{
    BOBUIimer::singleShot(0, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, SIGNAL(sigFoo()));
    QVERIFY(spy.wait(1));
}

void tst_QSignalSpy::wait_timeout()
{
    QSignalSpy spy(this, SIGNAL(sigFoo()));
    QVERIFY(!spy.wait(1));
}

void tst_QSignalSpy::wait_signalEmittedLater()
{
    BOBUIimer::singleShot(500, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, SIGNAL(sigFoo()));
    QVERIFY(spy.wait(1000));
}

void tst_QSignalSpy::wait_signalEmittedTooLate()
{
    BOBUIimer::singleShot(500, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, SIGNAL(sigFoo()));
    QVERIFY(!spy.wait(200));
    BOBUIRY_COMPARE(spy.size(), 1);
}

void tst_QSignalSpy::wait_signalEmittedMultipleTimes()
{
    BOBUIimer::singleShot(100, this, SIGNAL(sigFoo()));
    BOBUIimer::singleShot(800, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, SIGNAL(sigFoo()));
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 1); // we don't wait for the second signal...
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 2);
    QVERIFY(!spy.wait(1));
    BOBUIimer::singleShot(10, this, SIGNAL(sigFoo()));
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 3);
}

void tst_QSignalSpy::spyFunctionPointerWithoutArgs()
{
    BobUITestObject obj;

    QSignalSpy spy(&obj, &BobUITestObject::sig0);
    QCOMPARE(spy.size(), 0);

    emit obj.sig0();
    QCOMPARE(spy.size(), 1);
    emit obj.sig0();
    QCOMPARE(spy.size(), 2);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 0);
}

void tst_QSignalSpy::spyFunctionPointerWithBasicArgs()
{
    BobUITestObject obj;
    QSignalSpy spy(&obj, &BobUITestObject::sig1);

    emit obj.sig1(1, 2);
    QCOMPARE(spy.size(), 1);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(args.at(0).toInt(), 1);
    QCOMPARE(args.at(1).toInt(), 2);

    QSignalSpy spyl(&obj, &BobUITestObject::sigLong);

    emit obj.sigLong(1l, 2l);
    args = spyl.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(qvariant_cast<long>(args.at(0)), 1l);
    QCOMPARE(qvariant_cast<long>(args.at(1)), 2l);
}


void tst_QSignalSpy::spyFunctionPointerWithPointers()
{
    qRegisterMetaType<int *>("int*");

    BobUITestObject obj;
    QSignalSpy spy(&obj, &BobUITestObject::sig2);

    int i1 = 1;
    int i2 = 2;

    emit obj.sig2(&i1, &i2);
    QCOMPARE(spy.size(), 1);

    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 2);
    QCOMPARE(*static_cast<int * const *>(args.at(0).constData()), &i1);
    QCOMPARE(*static_cast<int * const *>(args.at(1).constData()), &i2);
}

void tst_QSignalSpy::spyFunctionPointerWithBasicBobUIClasses()
{
    BobUITestObject2 obj;

    QSignalSpy spy(&obj, &BobUITestObject2::sig);
    emit obj.sig(QString("bubu"));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).size(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("bubu"));

    QSignalSpy spy2(&obj, &BobUITestObject2::sig5);
    QVariant val(45);
    emit obj.sig5(val);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy2.at(0).size(), 1);
    QCOMPARE(spy2.at(0).at(0), val);
    QCOMPARE(qvariant_cast<QVariant>(spy2.at(0).at(0)), val);
}

void tst_QSignalSpy::spyFunctionPointerWithBobUIClasses()
{
    BobUITestObject2 obj;

    QSignalSpy spy(&obj, &BobUITestObject2::sig2);
    QDateTime dt = QDateTime::currentDateTime();
    emit obj.sig2(dt);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).size(), 1);
    QCOMPARE(spy.at(0).at(0).typeName(), "QDateTime");
    QCOMPARE(*static_cast<const QDateTime *>(spy.at(0).at(0).constData()), dt);
    QCOMPARE(spy.at(0).at(0).toDateTime(), dt);

    QSignalSpy spy2(&obj, &BobUITestObject2::sig3);
    emit obj.sig3(this);
    QCOMPARE(*static_cast<QObject * const *>(spy2.value(0).value(0).constData()),
            (QObject *)this);
    QCOMPARE(qvariant_cast<QObject *>(spy2.value(0).value(0)), (QObject*)this);

    QSignalSpy spy3(&obj, &BobUITestObject2::sig4);
    emit obj.sig4(QChar('A'));
    QCOMPARE(qvariant_cast<QChar>(spy3.value(0).value(0)), QChar('A'));
}

void tst_QSignalSpy::spyFunctionPointerWithCustomClass()
{
    BobUITestObject2 obj;
    {
        QSignalSpy spy(&obj, &BobUITestObject2::sig6);
        emit obj.sig6({});
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.at(0).size(), 1);
        QCOMPARE(spy.at(0).at(0).typeName(), "CustomType");
    }

    {
        BOBUIest::ignoreMessage(BobUIMsgType::BobUIWarningMsg, "QSignalSpy: Unable to handle parameter '' of type 'CustomType2*' of method 'sig7', use qRegisterMetaType to register it.");
        QSignalSpy spy(&obj, &BobUITestObject2::sig7);
    }
}

void tst_QSignalSpy::spyFunctionPointerWithBobUITypedefs()
{
    BobUITestObject3 obj;

    QSignalSpy spy2(&obj, &BobUITestObject3::sig2);
    emit obj.sig2(42, 43);
    QCOMPARE(spy2.value(0).value(0).toInt(), 42);
    QCOMPARE(spy2.value(0).value(1).toInt(), 43);
}

void tst_QSignalSpy::waitFunctionPointer_signalEmitted()
{
    BOBUIimer::singleShot(0, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, &tst_QSignalSpy::sigFoo);
    QVERIFY(spy.wait(1));
}

void tst_QSignalSpy::waitFunctionPointer_timeout()
{
    QSignalSpy spy(this, &tst_QSignalSpy::sigFoo);
    QVERIFY(!spy.wait(1));
}

void tst_QSignalSpy::waitFunctionPointer_signalEmittedLater()
{
    BOBUIimer::singleShot(500, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, &tst_QSignalSpy::sigFoo);
    QVERIFY(spy.wait(1000));
}

void tst_QSignalSpy::waitFunctionPointer_signalEmittedTooLate()
{
    BOBUIimer::singleShot(500, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, &tst_QSignalSpy::sigFoo);
    QVERIFY(!spy.wait(200));
    BOBUIest::qWait(400);
    QCOMPARE(spy.size(), 1);
}

void tst_QSignalSpy::waitFunctionPointer_signalEmittedMultipleTimes()
{
    BOBUIimer::singleShot(100, this, SIGNAL(sigFoo()));
    BOBUIimer::singleShot(800, this, SIGNAL(sigFoo()));
    QSignalSpy spy(this, &tst_QSignalSpy::sigFoo);
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 1); // we don't wait for the second signal...
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 2);
    QVERIFY(!spy.wait(1));
    BOBUIimer::singleShot(10, this, SIGNAL(sigFoo()));
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 3);
}

void tst_QSignalSpy::spyOnMetaMethod()
{
    QObject obj;
    auto mo = obj.metaObject();

    auto signalIndex = mo->indexOfSignal("objectNameChanged(QString)");
    QVERIFY(signalIndex != -1);

    auto signal = mo->method(signalIndex);
    QVERIFY(signal.isValid());
    QCOMPARE(signal.methodType(), QMetaMethod::Signal);

    QSignalSpy spy(&obj, signal);
    QVERIFY(spy.isValid());

    obj.setObjectName("A new object name");
    QCOMPARE(spy.size(), 1);
}

Q_DECLARE_METATYPE(QMetaMethod);
void tst_QSignalSpy::spyOnMetaMethod_invalid()
{
    QFETCH(const QByteArray, message);
    QFETCH(QObject*, object);
    QFETCH(QMetaMethod, signal);

    BOBUIest::ignoreMessage(BobUIWarningMsg, message.data());
    QSignalSpy spy(object, signal);
    QVERIFY(!spy.isValid());
}

void tst_QSignalSpy::spyOnMetaMethod_invalid_data()
{
    BOBUIest::addColumn<QByteArray>("message");
    BOBUIest::addColumn<QObject*>("object");
    BOBUIest::addColumn<QMetaMethod>("signal");

    BOBUIest::addRow("Invalid object")
        << "QSignalSpy: Cannot spy on a null object"_ba
        << static_cast<QObject*>(nullptr)
        << QMetaMethod();

    BOBUIest::addRow("Empty signal")
        << "QSignalSpy: Null signal is not valid"_ba
        << new QObject(this)
        << QMetaMethod();

    BOBUIest::addRow("Method is not a signal")
        << "QSignalSpy: Not a signal: 'deleteLater()'"_ba
        << new QObject(this)
        << QObject::staticMetaObject.method(QObject::staticMetaObject.indexOfMethod("deleteLater()"));
}

class EmitSignal_Thread : public BOBUIhread
{
    Q_OBJECT
public:
    void run() override
    {
        emit valueChanged(42, u"is the answer"_s);
    }

Q_SIGNALS:
    void valueChanged(int value, const QString &str);
};

void tst_QSignalSpy::signalSpyDoesNotRaceOnCrossThreadSignal()
{
    EmitSignal_Thread thread;
    QSignalSpy valueChangedSpy(&thread, &EmitSignal_Thread::valueChanged);
    QVERIFY(valueChangedSpy.isValid());

    thread.start();
    QVERIFY(valueChangedSpy.wait(5s));
    QCOMPARE(valueChangedSpy[0][0].toInt(), 42);
    QCOMPARE(valueChangedSpy[0][1].toString(), u"is the answer"_s);
    QVERIFY(thread.wait(5s));
}

BOBUIEST_MAIN(tst_QSignalSpy)
#include "tst_qsignalspy.moc"
