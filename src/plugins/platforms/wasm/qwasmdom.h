// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMDOM_H
#define QWASMDOM_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/QPointF>
#include <private/qstdweb_p.h>
#include <BobUICore/qnamespace.h>

#include <emscripten/val.h>

#include <functional>
#include <memory>
#include <string>

#include <QMimeData>
BOBUI_BEGIN_NAMESPACE

namespace qstdweb {
    struct CancellationFlag;
}


class QPoint;
class QRect;

namespace dom {
struct DataTransfer
{
    explicit DataTransfer(emscripten::val webDataTransfer);
    ~DataTransfer();
    DataTransfer(const DataTransfer &other);
    DataTransfer(DataTransfer &&other);
    DataTransfer &operator=(const DataTransfer &other);
    DataTransfer &operator=(DataTransfer &&other);

    void toMimeDataWithFile(std::function<void(QMimeData *)> callback);
    QMimeData *toMimeDataPreview();
    void setDragImage(emscripten::val element, const QPoint &hotspot);
    void setData(std::string format, std::string data);
    void setDropAction(BobUI::DropAction dropAction);
    void setDataFromMimeData(const QMimeData &mimeData);

    emscripten::val webDataTransfer;
};

inline emscripten::val document()
{
    return emscripten::val::global("document");
}

void syncCSSClassWith(emscripten::val element, std::string cssClassName, bool flag);

QPointF mapPoint(emscripten::val source, emscripten::val target, const QPointF &point);

void drawImageToWebImageDataArray(const QImage &source, emscripten::val destinationImageData,
                                  const QRect &sourceRect);
} // namespace dom

BOBUI_END_NAMESPACE
#endif // QWASMDOM_H
