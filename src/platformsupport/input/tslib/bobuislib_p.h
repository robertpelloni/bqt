// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUISLIB_H
#define BOBUISLIB_H

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

#include <QObject>
#include <private/qglobal_p.h>

struct tsdev;

BOBUI_BEGIN_NAMESPACE

class QSocketNotifier;

class BOBUIsLibMouseHandler : public QObject
{
    Q_OBJECT

public:
    BOBUIsLibMouseHandler(const QString &key, const QString &specification, QObject *parent = nullptr);
    ~BOBUIsLibMouseHandler();

private slots:
    void readMouseData();

private:
    QSocketNotifier * m_notify = nullptr;
    tsdev *m_dev;
    int m_x = 0;
    int m_y = 0;
    bool m_pressed = false;
    const bool m_rawMode;
};

BOBUI_END_NAMESPACE

#endif // BOBUISLIB_H
