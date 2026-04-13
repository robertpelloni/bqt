// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOLUMNVIEWGRIP_P_H
#define QCOLUMNVIEWGRIP_P_H

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
#include <private/qwidget_p.h>

BOBUI_REQUIRE_CONFIG(columnview);

BOBUI_BEGIN_NAMESPACE

class QColumnViewGripPrivate;

class Q_AUTOTEST_EXPORT QColumnViewGrip : public QWidget {

Q_OBJECT

Q_SIGNALS:
    void gripMoved(int offset);

public:
    explicit QColumnViewGrip(QWidget *parent = nullptr);
    ~QColumnViewGrip();
    int moveGrip(int offset);

protected:
    QColumnViewGrip(QColumnViewGripPrivate &, QWidget *parent = nullptr, BobUI::WindowFlags f = { });
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Q_DECLARE_PRIVATE(QColumnViewGrip)
    Q_DISABLE_COPY_MOVE(QColumnViewGrip)
};

class QColumnViewGripPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QColumnViewGrip)

public:
    QColumnViewGripPrivate();
    ~QColumnViewGripPrivate() {}

    int originalXLocation;
};

BOBUI_END_NAMESPACE

#endif //QCOLUMNVIEWGRIP_P_H
