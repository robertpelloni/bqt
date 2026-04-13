// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSPLASHSCREEN_H
#define QSPLASHSCREEN_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/qpixmap.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(splashscreen);

BOBUI_BEGIN_NAMESPACE

class QSplashScreenPrivate;

class Q_WIDGETS_EXPORT QSplashScreen : public QWidget
{
    Q_OBJECT
public:
    explicit QSplashScreen(const QPixmap &pixmap = QPixmap(), BobUI::WindowFlags f = BobUI::WindowFlags());
    QSplashScreen(QScreen *screen, const QPixmap &pixmap = QPixmap(), BobUI::WindowFlags f = BobUI::WindowFlags());
    virtual ~QSplashScreen();

    void setPixmap(const QPixmap &pixmap);
    const QPixmap pixmap() const;
    void finish(QWidget *w);
    void repaint();
    QString message() const;

public Q_SLOTS:
    void showMessage(const QString &message, int alignment = BobUI::AlignLeft,
                  const QColor &color = BobUI::black);
    void clearMessage();

Q_SIGNALS:
    void messageChanged(const QString &message);

protected:
    bool event(QEvent *e) override;
    virtual void drawContents(QPainter *painter);
    void mousePressEvent(QMouseEvent *) override;

private:
    Q_DISABLE_COPY(QSplashScreen)
    Q_DECLARE_PRIVATE(QSplashScreen)
};

BOBUI_END_NAMESPACE

#endif // QSPLASHSCREEN_H
