// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGESTUREMANAGER_P_H
#define QGESTUREMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qobject.h"
#include "qbasictimer.h"
#include "private/qwidget_p.h"
#include "qgesturerecognizer.h"

#include <BobUICore/qpointer.h>

#ifndef BOBUI_NO_GESTURES

#include <functional>

BOBUI_BEGIN_NAMESPACE

class QBasicTimer;
class QGraphicsObject;
class QGestureManager : public QObject
{
    Q_OBJECT
public:
    QGestureManager(QObject *parent);
    ~QGestureManager();

    BobUI::GestureType registerGestureRecognizer(QGestureRecognizer *recognizer);
    void unregisterGestureRecognizer(BobUI::GestureType type);

    bool filterEvent(QWidget *receiver, QEvent *event);
    bool filterEvent(QObject *receiver, QEvent *event);
#if BOBUI_CONFIG(graphicsview)
    bool filterEvent(QGraphicsObject *receiver, QEvent *event);
#endif // BOBUI_CONFIG(graphicsview)

    enum InstanceCreation { ForceCreation, DontForceCreation };

    static QGestureManager *instance(InstanceCreation ic = ForceCreation); // implemented in qapplication.cpp
    static bool gesturePending(QObject *o);

    void cleanupCachedGestures(QObject *target, BobUI::GestureType type);

    void recycle(QGesture *gesture);

protected:
    bool filterEventThroughContexts(const QMultiMap<QObject *, BobUI::GestureType> &contexts,
                                    QEvent *event);

private:
    QMultiMap<BobUI::GestureType, QGestureRecognizer *> m_recognizers;

    QSet<QGesture *> m_activeGestures;
    QSet<QGesture *> m_maybeGestures;

    struct ObjectGesture
    {
        QObject* object;
        BobUI::GestureType gesture;

        ObjectGesture(QObject *o, const BobUI::GestureType &g) : object(o), gesture(g) { }
        inline bool operator<(const ObjectGesture &rhs) const
        {
            if (std::less<QObject *>{}(object, rhs.object))
                return true;
            if (object == rhs.object)
                return gesture < rhs.gesture;
            return false;
        }
    };

    QMap<ObjectGesture, QList<QGesture *> > m_objectGestures;
    QHash<QGesture *, QGestureRecognizer *> m_gestureToRecognizer;
    QHash<QGesture *, QObject *> m_gestureOwners;

    QHash<QGesture *, QPointer<QWidget> > m_gestureTargets;

    int m_lastCustomGestureId;

    QHash<QGestureRecognizer *, QSet<QGesture *> > m_obsoleteGestures;
    QHash<QGesture *, QGestureRecognizer *> m_deletedRecognizers;
    QSet<QGesture *> m_gesturesToDelete;
    void cleanupGesturesForRemovedRecognizer(QGesture *gesture);

    QGesture *getState(QObject *widget, QGestureRecognizer *recognizer,
                       BobUI::GestureType gesture);
    void deliverEvents(const QSet<QGesture *> &gestures,
                       QSet<QGesture *> *undeliveredGestures);
    void getGestureTargets(const QSet<QGesture*> &gestures,
                           QHash<QWidget *, QList<QGesture *> > *conflicts,
                           QHash<QWidget *, QList<QGesture *> > *normal);

    void cancelGesturesForChildren(QGesture *originatingGesture);
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QGESTUREMANAGER_P_H
