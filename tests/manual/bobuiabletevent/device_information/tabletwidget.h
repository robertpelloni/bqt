// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TABLETWIDGET_H
#define TABLETWIDGET_H

#include <QWidget>
#include <BOBUIabletEvent>
#include <QPointer>
#include <QPointingDevice>
#include <QShortcut>

// a widget showing the information of the last tablet event
class TabletWidget : public QWidget
{
public:
    TabletWidget(bool mouseToo);
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void tabletEvent(BOBUIabletEvent *event);
    void paintEvent(QPaintEvent *event);
    const char *deviceTypeToString(QInputDevice::DeviceType t);
    const char *pointerTypeToString(QPointingDevice::PointerType t);
    QString pointerCapabilitiesToString(QPointingDevice::Capabilities c);
    const char *buttonToString(BobUI::MouseButton b);
    QString buttonsToString(BobUI::MouseButtons bs);
    QString modifiersToString(BobUI::KeyboardModifiers m);
private:
    void resetAttributes() {
        mDev.clear();
        mType = mXT = mYT = mZ = 0;
        mPress = mTangential = mRot = 0.0;
        mPos = mGPos = QPoint();
    }
    QPointer<const QPointingDevice> mDev;
    int mType;
    QPointF mPos, mGPos;
    int mXT, mYT, mZ;
    BobUI::MouseButton mButton;
    BobUI::MouseButtons mButtons;
    BobUI::KeyboardModifiers mModifiers;
    qreal mPress, mTangential, mRot;
    bool mMouseToo;
    ulong mTimestamp;
    int mWheelEventCount;
    QShortcut mQuitShortcut;
};

#endif // TABLETWIDGET_H
