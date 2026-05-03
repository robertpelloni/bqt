#ifndef OMNIRAYMARCHER_H
#define OMNIRAYMARCHER_H

#include <QQuickPaintedItem>
#include <QImage>
#include <BOBUIimer>
#include <QVector3D>
#include <QFutureWatcher>

// The ultimate CPU-bound 3D rendering engine.
// Uses Signed Distance Fields (SDF) and Raymarching split across 
// a BobUIConcurrent thread pool to render mathematically pure 3D scenes 
// natively into the QML SceneGraph without breaking WebAssembly GPU sandboxes.
class OmniRaymarcher : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    Q_PROPERTY(qreal resolutionScale READ resolutionScale WRITE setResolutionScale NOTIFY resolutionScaleChanged)
    Q_PROPERTY(QVector3D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)

public:
    explicit OmniRaymarcher(QQuickItem *parent = nullptr);
    virtual ~OmniRaymarcher();

    bool isRunning() const;
    void setIsRunning(bool running);

    qreal resolutionScale() const;
    void setResolutionScale(qreal scale);

    QVector3D cameraPosition() const;
    void setCameraPosition(const QVector3D& pos);

    void paint(QPainter *painter) override;

signals:
    void isRunningChanged();
    void resolutionScaleChanged();
    void cameraPositionChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private slots:
    void renderNextFrame();
    void onFrameRendered();

private:
    void allocateBuffer();
    
    // Core math functions for the Raymarcher
    static double mapSDF(const QVector3D& p, double time);
    static QVector3D calcNormal(const QVector3D& p, double time);

    bool m_isRunning;
    qreal m_resolutionScale;
    QVector3D m_cameraPos;
    
    double m_time;
    QImage m_buffer;
    BOBUIimer m_frameTimer;
    QFutureWatcher<void> m_watcher;
    bool m_isRendering;
};

#endif // OMNIRAYMARCHER_H
