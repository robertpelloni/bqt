// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWINDOWSDRAG_H
#define QWINDOWSDRAG_H

#include <private/qstdweb_p.h>
#include <private/qsimpledrag_p.h>

#include <qpa/qplatformdrag.h>
#include <BobUIGui/qdrag.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

struct DragEvent;

class QWasmDrag final : public QSimpleDrag
{
public:
    QWasmDrag();
    ~QWasmDrag() override;
    QWasmDrag(const QWasmDrag &other) = delete;
    QWasmDrag(QWasmDrag &&other) = delete;
    QWasmDrag &operator=(const QWasmDrag &other) = delete;
    QWasmDrag &operator=(QWasmDrag &&other) = delete;

    static QWasmDrag *instance();

    void onNativeDragOver(DragEvent *event);
    void onNativeDrop(DragEvent *event);
    void onNativeDragStarted(DragEvent *event);
    void onNativeDragFinished(DragEvent *event);
    void onNativeDragEnter(DragEvent *event);
    void onNativeDragLeave(DragEvent *event);

    // QPlatformDrag:
    BobUI::DropAction drag(QDrag *drag) final;

private:
    struct DragState;

    std::shared_ptr<DragState> m_dragState;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDRAG_H
