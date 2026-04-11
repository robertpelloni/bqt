#ifndef OMNIRHINODE_H
#define OMNIRHINODE_H

#include <QQuickItem>

// Advanced RHI Hardware Node (Qt 7 Parity)
// Allows direct injection of external GPU textures into the Scene Graph.
class OmniRhiNode : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(quintptr nativeTextureHandle READ nativeTextureHandle WRITE setNativeTextureHandle NOTIFY textureChanged)

public:
    explicit OmniRhiNode(QQuickItem *parent = nullptr);
    virtual ~OmniRhiNode();

    quintptr nativeTextureHandle() const { return m_handle; }
    void setNativeTextureHandle(quintptr h);

signals:
    void textureChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

private:
    quintptr m_handle;
};

#endif // OMNIRHINODE_H
