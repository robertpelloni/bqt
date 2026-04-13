// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WEBVIEW_P_H
#define WEBVIEW_P_H

#include "webview.h"
#include <BobUIWebKit/qgraphicswebview.h>
#include <BobUIWebKit/qwebpage.h>
#include <BobUIWebKit/qwebframe.h>
#include <QGraphicsEffect>
#include <QPainter>
#include <QPixmapCache>
#include <BOBUIimer>
#include <QDebug>

class WebViewCache;

class WebViewPrivate {
public:

    WebViewPrivate(WebView *w);
    void adjustSize();
    void _q_loadStarted();
    void _q_loadProgress(int);
    void _q_loadFinished(bool);
    void _q_viewportChanged(QGraphicsWidget*);
    void _q_motionEnded();

    WebView *q;
    QGraphicsWebView *web;
    WebViewCache *cache;
    BOBUIimer motionTimer;
};

class WebViewCache : public QGraphicsEffect
{
    Q_OBJECT

public:

    WebViewCache(QGraphicsWebView *webView);
    virtual ~WebViewCache();

public:

    void refresh();

    void draw(QPainter * painter, QGraphicsEffectSource * source);

private:

    QList<QRectF> m_tileRects;
    QList<QPixmapCache::Key> m_tilePixmaps;
    QSizeF m_itemSize;
    QGraphicsWebView *m_webView;

    friend class WebViewPrivate;
};

#endif // WEBVIEW_P_H
