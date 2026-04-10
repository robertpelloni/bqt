// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#pragma once

#include <QObject>
#include <QImage>
#include <QJsonObject>

BOBUI_BEGIN_NAMESPACE

class QWidget;

class QWidgetBaselineTest : public QObject
{
    Q_OBJECT

public:
    QWidgetBaselineTest();

    void takeStandardSnapshots();
    QWidget *testWindow() const { return window; }

    void reportDebugRect(const QString &type, const QColor &color,
        const QString &label, QRect widgetRect, const QWidget *widget, QPainter *painter);

protected:
    virtual void doInit() {}
    virtual void doCleanup() {}

protected slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

protected:
    void makeVisible();
    QImage takeSnapshot();
    QImage takeScreenSnapshot(const QRect& rect = QRect());

    QJsonObject debugRects;
private:
    QWidget *background = nullptr;
    QWidget *window = nullptr;
};

BOBUI_END_NAMESPACE
