// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmcursor.h"
#include "qwasmscreen.h"
#include "qwasmwindow.h"

#include <BobUICore/qbuffer.h>
#include <BobUICore/qdebug.h>
#include <BobUICore/qstring.h>
#include <BobUIGui/qwindow.h>

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

BOBUI_BEGIN_NAMESPACE
using namespace emscripten;

namespace {
QByteArray cursorToCss(const QCursor *cursor)
{
    auto shape = cursor->shape();
    switch (shape) {
    case BobUI::ArrowCursor:
        return "default";
    case BobUI::UpArrowCursor:
        return "n-resize";
    case BobUI::CrossCursor:
        return "crosshair";
    case BobUI::WaitCursor:
        return "wait";
    case BobUI::IBeamCursor:
        return "text";
    case BobUI::SizeVerCursor:
        return "ns-resize";
    case BobUI::SizeHorCursor:
        return "ew-resize";
    case BobUI::SizeBDiagCursor:
        return "nesw-resize";
    case BobUI::SizeFDiagCursor:
        return "nwse-resize";
    case BobUI::SizeAllCursor:
        return "move";
    case BobUI::BlankCursor:
        return "none";
    case BobUI::SplitVCursor:
        return "row-resize";
    case BobUI::SplitHCursor:
        return "col-resize";
    case BobUI::PointingHandCursor:
        return "pointer";
    case BobUI::ForbiddenCursor:
        return "not-allowed";
    case BobUI::WhatsThisCursor:
        return "help";
    case BobUI::BusyCursor:
        return "progress";
    case BobUI::OpenHandCursor:
        return "grab";
    case BobUI::ClosedHandCursor:
        return "grabbing";
    case BobUI::DragCopyCursor:
        return "copy";
    case BobUI::DragMoveCursor:
        return "default";
    case BobUI::DragLinkCursor:
        return "alias";
    case BobUI::BitmapCursor: {
        auto pixmap = cursor->pixmap();
        QByteArray cursorAsPng;
        QBuffer buffer(&cursorAsPng);
        buffer.open(QBuffer::WriteOnly);
        pixmap.save(&buffer, "PNG");
        buffer.close();
        auto cursorAsBase64 = cursorAsPng.toBase64();
        auto hotSpot = cursor->hotSpot();
        auto encodedCursor =
            QString("url(data:image/png;base64,%1) %2 %3, auto")
                .arg(QString::fromUtf8(cursorAsBase64),
                     QString::number(hotSpot.x()),
                     QString::number(hotSpot.y()));
        return encodedCursor.toUtf8();
        }
    default:
        static_assert(BobUI::CustomCursor == 25,
                      "New cursor type added, handle it");
        qWarning() << "QWasmCursor: " << shape << " unsupported";
        return "default";
    }
}
} // namespace

void QWasmCursor::changeCursor(QCursor *windowCursor, QWindow *window)
{
    if (!window)
        return;
    if (QWasmWindow *wasmWindow = static_cast<QWasmWindow *>(window->handle()))
        wasmWindow->setWindowCursor(windowCursor ? cursorToCss(windowCursor) : "default");
}

BOBUI_END_NAMESPACE
