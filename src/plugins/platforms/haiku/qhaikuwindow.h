// Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Tobias Koenig <tobias.koenig@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QHAIKUWINDOW_H
#define QHAIKUWINDOW_H

#include <qpa/qplatformwindow.h>

#include <Window.h>

BOBUI_BEGIN_NAMESPACE

class HaikuWindowProxy : public QObject, public BWindow
{
    Q_OBJECT

public:
    explicit HaikuWindowProxy(QWindow *window, const QRect &rect, QObject *parent = nullptr);

    void FrameMoved(BPoint pos) override;
    void FrameResized(float width, float height) override;
    void WindowActivated(bool activated) override;
    void Minimize(bool minimize) override;
    void Zoom(BPoint pos, float width, float height) override;
    bool QuitRequested() override;

    void zoomByBobUI();

Q_SIGNALS:
    void moved(const QPoint &pos);
    void resized(const QSize &size, bool zoomInProgress);
    void windowActivated(bool activated);
    void minimized(bool minimize);
    void zoomed();
    void quitRequested();

private:
    bool m_bobuiCalledZoom;
    bool m_zoomInProgress;
};

class QHaikuWindow : public QObject, public QPlatformWindow
{
    Q_OBJECT

public:
    explicit QHaikuWindow(QWindow *window);
    virtual ~QHaikuWindow();

    void setGeometry(const QRect &rect) override;
    QMargins frameMargins() const override;
    void setVisible(bool visible) override;

    bool isExposed() const override;
    bool isActive() const override;

    WId winId() const override;
    BWindow* nativeHandle() const;

    void requestActivateWindow() override;
    void setWindowState(BobUI::WindowStates state) override;
    void setWindowFlags(BobUI::WindowFlags flags) override;

    void setWindowTitle(const QString &title) override;

    void propagateSizeHints() override;

protected:
    HaikuWindowProxy *m_window;

private Q_SLOTS:
    void haikuWindowMoved(const QPoint &pos);
    void haikuWindowResized(const QSize &size, bool zoomInProgress);
    void haikuWindowActivated(bool activated);
    void haikuWindowMinimized(bool minimize);
    void haikuWindowZoomed();
    void haikuWindowQuitRequested();

    void haikuMouseEvent(const QPoint &localPosition, const QPoint &globalPosition, BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers, BobUI::MouseEventSource source);
    void haikuWheelEvent(const QPoint &localPosition, const QPoint &globalPosition, int delta, BobUI::Orientation orientation, BobUI::KeyboardModifiers modifiers);
    void haikuKeyEvent(QEvent::Type type, int key, BobUI::KeyboardModifiers modifiers, const QString &text);
    void haikuEnteredView();
    void haikuExitedView();
    void haikuDrawRequest(const QRect &rect);

private:
    BobUI::WindowStates m_windowState;
};

BOBUI_END_NAMESPACE

#endif
