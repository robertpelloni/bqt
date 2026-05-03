// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMWINDOWNONCLIENTAREA_H
#define QWASMWINDOWNONCLIENTAREA_H

#include <BobUICore/qrect.h>
#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/qnamespace.h>

#include <emscripten/val.h>

#include <functional>
#include <memory>
#include <string_view>
#include <vector>

BOBUI_BEGIN_NAMESPACE

namespace qstdweb {
class EventCallback;
}

struct PointerEvent;
class QWindow;
class Resizer;
class TitleBar;
class QWasmWindow;

class NonClientArea
{
public:
    NonClientArea(QWasmWindow *window, emscripten::val containerElement);
    ~NonClientArea();

    void onClientAreaWidthChange(int width);
    void propagateSizeHints();
    TitleBar *titleBar() const { return m_titleBar.get(); }

private:
    void updateResizability();

    emscripten::val m_bobuiWindowElement;
    std::unique_ptr<Resizer> m_resizer;
    std::unique_ptr<TitleBar> m_titleBar;
};

class WebImageButton
{
public:
    class Callbacks
    {
    public:
        Callbacks();
        Callbacks(std::function<void()> onInteraction, std::function<void()> onClick);
        ~Callbacks();

        Callbacks(const Callbacks &) = delete;
        Callbacks(Callbacks &&);
        Callbacks &operator=(const Callbacks &) = delete;
        Callbacks &operator=(Callbacks &&);

        operator bool() const { return !!m_onInteraction; }

        void onInteraction();
        void onClick();

    private:
        std::function<void()> m_onInteraction;
        std::function<void()> m_onClick;
    };

    WebImageButton();
    ~WebImageButton();

    void setCallbacks(Callbacks callbacks);
    void setImage(std::string_view imageData, std::string_view format);
    void setVisible(bool visible);

    emscripten::val htmlElement() const { return m_containerElement; }
    emscripten::val imageElement() const { return m_imgElement; }

private:
    emscripten::val m_containerElement;
    emscripten::val m_imgElement;

    std::unique_ptr<qstdweb::EventCallback> m_webMouseMoveEventCallback;
    std::unique_ptr<qstdweb::EventCallback> m_webMouseDownEventCallback;
    std::unique_ptr<qstdweb::EventCallback> m_webClickEventCallback;

    Callbacks m_callbacks;
};

struct ResizeConstraints {
    QPoint minShrink;
    QPoint maxGrow;
    int maxGrowTop;
};

class Resizer
{
public:
    class ResizerElement
    {
    public:
        static constexpr const char *cssClassNameForEdges(BobUI::Edges edges)
        {
            switch (edges) {
            case BobUI::TopEdge | BobUI::LeftEdge:;
                return "nw";
            case BobUI::TopEdge:
                return "n";
            case BobUI::TopEdge | BobUI::RightEdge:
                return "ne";
            case BobUI::LeftEdge:
                return "w";
            case BobUI::RightEdge:
                return "e";
            case BobUI::BottomEdge | BobUI::LeftEdge:
                return "sw";
            case BobUI::BottomEdge:
                return "s";
            case BobUI::BottomEdge | BobUI::RightEdge:
                return "se";
            default:
                return "";
            }
        }

        ResizerElement(emscripten::val parentElement, BobUI::Edges edges, Resizer *resizer);
        ~ResizerElement();
        ResizerElement(const ResizerElement &other) = delete;
        ResizerElement(ResizerElement &&other);
        ResizerElement &operator=(const ResizerElement &other) = delete;
        ResizerElement &operator=(ResizerElement &&other) = delete;

        bool onPointerDown(const PointerEvent &event);
        bool onPointerMove(const PointerEvent &event);
        bool onPointerUp(const PointerEvent &event);

    private:
        emscripten::val m_element;

        int m_capturedPointerId = -1;

        const BobUI::Edges m_edges;

        Resizer *m_resizer;

        std::unique_ptr<qstdweb::EventCallback> m_mouseDownEvent;
        std::unique_ptr<qstdweb::EventCallback> m_mouseMoveEvent;
        std::unique_ptr<qstdweb::EventCallback> m_mouseUpEvent;
    };

    using ClickCallback = std::function<void()>;

    Resizer(QWasmWindow *window, emscripten::val parentElement);
    ~Resizer();

    ResizeConstraints getResizeConstraints();

private:
    void onInteraction();
    void startResize(BobUI::Edges resizeEdges, const PointerEvent &event);
    void continueResize(const PointerEvent &event);
    void finishResize();

    struct ResizeData
    {
        BobUI::Edges edges = BobUI::Edges::fromInt(0);
        QPointF originInScreenCoords;
        QPoint minShrink;
        QPoint maxGrow;
        QRect initialBounds;
    };
    std::unique_ptr<ResizeData> m_currentResizeData;

    QWasmWindow *m_window;
    emscripten::val m_windowElement;
    std::vector<std::unique_ptr<ResizerElement>> m_elements;
};

class TitleBar
{
public:
    TitleBar(QWasmWindow *window, emscripten::val parentElement);
    ~TitleBar();

    void setTitle(const QString &title);
    void setRestoreVisible(bool visible);
    void setMaximizeVisible(bool visible);
    void setCloseVisible(bool visible);
    void setIcon(std::string_view imageData, std::string_view format);
    void setWidth(int width);

    QRectF geometry() const;

private:
    bool onPointerDown(const PointerEvent &event);
    bool onPointerMove(const PointerEvent &event);
    bool onPointerUp(const PointerEvent &event);
    bool onDoubleClick();

    QPointF clipPointWithScreen(const QPointF &pointInTitleBarCoords) const;

    QWasmWindow *m_window;

    emscripten::val m_element;
    emscripten::val m_label;

    std::unique_ptr<WebImageButton> m_close;
    std::unique_ptr<WebImageButton> m_maximize;
    std::unique_ptr<WebImageButton> m_restore;
    std::unique_ptr<WebImageButton> m_icon;

    int m_capturedPointerId = -1;
    QPointF m_moveStartPoint;
    QPoint m_moveStartWindowPosition;

    std::unique_ptr<qstdweb::EventCallback> m_mouseDownEvent;
    std::unique_ptr<qstdweb::EventCallback> m_mouseMoveEvent;
    std::unique_ptr<qstdweb::EventCallback> m_mouseUpEvent;
    std::unique_ptr<qstdweb::EventCallback> m_doubleClickEvent;
};

BOBUI_END_NAMESPACE
#endif // QWASMWINDOWNONCLIENTAREA_H
