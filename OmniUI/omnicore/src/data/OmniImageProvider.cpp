#include "OmniImageProvider.h"
#include "OmniAssetManager.h"
#include <QDebug>

OmniImageProvider::OmniImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

OmniImageProvider::~OmniImageProvider() = default;

QImage OmniImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    // The "id" passed from QML is the string after "image://omni/"
    // Example: Image { source: "image://omni/icons/home.png" } -> id = "icons/home.png"

    QByteArray rawData = OmniAssetManager::instance()->getFileBytes(id);
    if (rawData.isEmpty()) {
        qWarning() << "OmniImageProvider: Failed to locate asset in VFS:" << id;
        return QImage();
    }

    QImage image;
    if (!image.loadFromData(rawData)) {
        qWarning() << "OmniImageProvider: Failed to decode image data for:" << id;
        return QImage();
    }

    if (size) {
        *size = image.size();
    }

    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = image.scaled(requestedSize.width(), requestedSize.height(), BobUI::KeepAspectRatio);
    }

    return image;
}
