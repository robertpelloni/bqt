// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/qsignaldumper_p.h>

#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qvariant.h>

#include <BobUITest/private/bobuiestlog_p.h>

#include <BobUICore/private/qmetaobject_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{

inline static void qPrintMessage(const QByteArray &ba)
{
    BOBUIestLog::info(ba.constData(), nullptr, 0);
}

Q_GLOBAL_STATIC(QList<QByteArray>, ignoreClasses)
Q_CONSTINIT static QBasicMutex ignoreClassesMutex;
Q_CONSTINIT thread_local int iLevel = 0;
Q_CONSTINIT thread_local int ignoreLevel = 0;
enum { IndentSpacesCount = 4 };

static bool classIsIgnored(const char *className)
{
    if (Q_LIKELY(!ignoreClasses.exists()))
        return false;
    QMutexLocker locker(&ignoreClassesMutex);
    if (ignoreClasses()->isEmpty())
        return false;
    return ignoreClasses()->contains(QByteArrayView(className));
}

static void qSignalDumperCallback(QObject *caller, int signal_index, void **argv)
{
    Q_ASSERT(caller);
    Q_ASSERT(argv);
    Q_UNUSED(argv);
    const QMetaObject *mo = caller->metaObject();
    Q_ASSERT(mo);
    QMetaMethod member = QMetaObjectPrivate::signal(mo, signal_index);
    Q_ASSERT(member.isValid());

    if (classIsIgnored(mo->className())) {
        ++BOBUIest::ignoreLevel;
        return;
    }

    QByteArray str;
    str.fill(' ', BOBUIest::iLevel++ * BOBUIest::IndentSpacesCount);
    str += "Signal: ";
    str += mo->className();
    str += '(';

    QString objname = caller->objectName();
    str += objname.toLocal8Bit();
    if (!objname.isEmpty())
        str += ' ';
    str += QByteArray::number(quintptr(caller), 16).rightJustified(8, '0');

    str += ") ";
    str += member.name();
    str += " (";

    QList<QByteArray> args = member.parameterTypes();
    for (int i = 0; i < args.size(); ++i) {
        const QByteArray &arg = args.at(i);
        int typeId = QMetaType::fromName(args.at(i).constData()).id();
        if (arg.endsWith('*') || arg.endsWith('&')) {
            str += '(';
            str += arg;
            str += ')';
            if (arg.endsWith('&'))
                str += '@';

            quintptr addr = quintptr(*reinterpret_cast<void **>(argv[i + 1]));
            str.append(QByteArray::number(addr, 16).rightJustified(8, '0'));
        } else if (typeId != QMetaType::UnknownType) {
            Q_ASSERT(typeId != QMetaType::Void); // void parameter => metaobject is corrupt
            str.append(arg)
                .append('(')
                .append(QVariant(QMetaType(typeId), argv[i + 1]).toString().toLocal8Bit())
                .append(')');
        }
        str.append(", ");
    }
    if (str.endsWith(", "))
        str.chop(2);
    str.append(')');
    qPrintMessage(str);
}

static void qSignalDumperCallbackSlot(QObject *caller, int method_index, void **argv)
{
    Q_ASSERT(caller);
    Q_ASSERT(argv);
    Q_UNUSED(argv);
    const QMetaObject *mo = caller->metaObject();
    Q_ASSERT(mo);
    QMetaMethod member = mo->method(method_index);
    if (!member.isValid())
        return;

    if (BOBUIest::ignoreLevel || classIsIgnored(mo->className()))
        return;

    QByteArray str;
    str.fill(' ', BOBUIest::iLevel * BOBUIest::IndentSpacesCount);
    str += "Slot: ";
    str += mo->className();
    str += '(';

    QString objname = caller->objectName();
    str += objname.toLocal8Bit();
    if (!objname.isEmpty())
        str += ' ';
    str += QByteArray::number(quintptr(caller), 16).rightJustified(8, '0');

    str += ") ";
    str += member.methodSignature();
    qPrintMessage(str);
}

static void qSignalDumperCallbackEndSignal(QObject *caller, int /*signal_index*/)
{
    Q_ASSERT(caller); Q_ASSERT(caller->metaObject());
    if (classIsIgnored(caller->metaObject()->className())) {
        --BOBUIest::ignoreLevel;
        Q_ASSERT(BOBUIest::ignoreLevel >= 0);
        return;
    }
    --BOBUIest::iLevel;
    Q_ASSERT(BOBUIest::iLevel >= 0);
}

}

void QSignalDumper::setEnabled(bool enabled)
{
    s_isEnabled = enabled;
}

void QSignalDumper::startDump()
{
    if (!s_isEnabled)
        return;

    static QSignalSpyCallbackSet set = { BOBUIest::qSignalDumperCallback,
        BOBUIest::qSignalDumperCallbackSlot, BOBUIest::qSignalDumperCallbackEndSignal, nullptr };
    bobui_register_signal_spy_callbacks(&set);
}

void QSignalDumper::endDump()
{
    bobui_register_signal_spy_callbacks(nullptr);
}

void QSignalDumper::ignoreClass(const QByteArray &klass)
{
    QMutexLocker locker(&BOBUIest::ignoreClassesMutex);
    if (BOBUIest::ignoreClasses())
        BOBUIest::ignoreClasses()->append(klass);
}

void QSignalDumper::clearIgnoredClasses()
{
    QMutexLocker locker(&BOBUIest::ignoreClassesMutex);
    if (BOBUIest::ignoreClasses.exists())
        BOBUIest::ignoreClasses()->clear();
}

bool QSignalDumper::s_isEnabled = false;

BOBUI_END_NAMESPACE
