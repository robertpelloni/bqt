#ifndef OMNIIMAGEPROVIDER_H
#define OMNIIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QString>

// An insanely powerful QML hook that intercepts `image://omni/logo.png`
// and pulls the raw binary data physically out of the OmniAssetManager VFS
// directly onto the QML SceneGraph without writing to the disk.
class OmniImageProvider : public QQuickImageProvider {
public:
    OmniImageProvider();
    ~OmniImageProvider() override;

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // OMNIIMAGEPROVIDER_H
