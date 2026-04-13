// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLINUXFBSCREEN_H
#define QLINUXFBSCREEN_H

#include <BobUIFbSupport/private/qfbscreen_p.h>

BOBUI_BEGIN_NAMESPACE

class QPainter;
class QFbCursor;

class QLinuxFbScreen : public QFbScreen
{
    Q_OBJECT
public:
    QLinuxFbScreen(const QStringList &args);
    ~QLinuxFbScreen();

    bool initialize() override;

    QPixmap grabWindow(WId wid, int x, int y, int width, int height) const override;

    QRegion doRedraw() override;

private:
    QStringList mArgs;
    int mFbFd;
    int mTtyFd;

    QImage mFbScreenImage;
    int mBytesPerLine;
    int mOldTtyMode;

    struct {
        uchar *data;
        int offset, size;
    } mMmap;

    QPainter *mBlitter;
};

BOBUI_END_NAMESPACE

#endif // QLINUXFBSCREEN_H

