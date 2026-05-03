#include "OmniRaymarcher.h"
#include <QPainter>
#include <BobUIConcurrent>
#include <cmath>
#include <algorithm>
#include <QDebug>

OmniRaymarcher::OmniRaymarcher(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_isRunning(false),
      m_resolutionScale(0.25),
      m_cameraPos(0.0, 0.0, -3.0),
      m_time(0.0),
      m_isRendering(false)
{
    setObjectName("OmniRaymarcher");
    setFlag(ItemHasContents, true);
    setWidth(400);
    setHeight(400);

    connect(&m_frameTimer, &BOBUIimer::timeout, this, &OmniRaymarcher::renderNextFrame);
    connect(&m_watcher, &QFutureWatcher<void>::finished, this, &OmniRaymarcher::onFrameRendered);
}

OmniRaymarcher::~OmniRaymarcher() {
    setIsRunning(false);
    m_watcher.waitForFinished();
}

bool OmniRaymarcher::isRunning() const { return m_isRunning; }
void OmniRaymarcher::setIsRunning(bool running) {
    if (m_isRunning != running) {
        m_isRunning = running;
        if (m_isRunning) {
            allocateBuffer();
            m_frameTimer.start(33); // ~30fps target
            qDebug() << "OmniRaymarcher: 3D Engine Started. Thread-Pooled Raymarching Active.";
        } else {
            m_frameTimer.stop();
        }
        emit isRunningChanged();
    }
}

qreal OmniRaymarcher::resolutionScale() const { return m_resolutionScale; }
void OmniRaymarcher::setResolutionScale(qreal scale) {
    qreal clamped = std::clamp(scale, 0.05, 1.0);
    if (!qFuzzyCompare(m_resolutionScale, clamped)) {
        m_resolutionScale = clamped;
        allocateBuffer();
        emit resolutionScaleChanged();
    }
}

QVector3D OmniRaymarcher::cameraPosition() const { return m_cameraPos; }
void OmniRaymarcher::setCameraPosition(const QVector3D& pos) {
    if (m_cameraPos != pos) {
        m_cameraPos = pos;
        emit cameraPositionChanged();
    }
}

void OmniRaymarcher::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
    if (newGeometry.size() != oldGeometry.size()) {
        allocateBuffer();
    }
}

void OmniRaymarcher::allocateBuffer() {
    m_watcher.waitForFinished(); 
    
    int w = std::max(1, static_cast<int>(width() * m_resolutionScale));
    int h = std::max(1, static_cast<int>(height() * m_resolutionScale));
    
    m_buffer = QImage(w, h, QImage::Format_RGB32);
    m_buffer.fill(BobUI::black);
}

double OmniRaymarcher::mapSDF(const QVector3D& p, double time) {
    QVector3D q = p;
    double spacing = 2.0;
    q.setX(std::fmod(std::abs(q.x()), spacing) - spacing * 0.5);
    q.setY(std::fmod(std::abs(q.y()), spacing) - spacing * 0.5);
    q.setZ(std::fmod(std::abs(q.z() + time), spacing) - spacing * 0.5);
    double radius = 0.5 + 0.2 * std::sin(time * 2.0);
    return q.length() - radius;
}

QVector3D OmniRaymarcher::calcNormal(const QVector3D& p, double time) {
    const double eps = 0.001;
    double d = mapSDF(p, time);
    double nx = mapSDF(p + QVector3D(eps, 0, 0), time) - d;
    double ny = mapSDF(p + QVector3D(0, eps, 0), time) - d;
    double nz = mapSDF(p + QVector3D(0, 0, eps), time) - d;
    QVector3D n(nx, ny, nz);
    n.normalize();
    return n;
}

void OmniRaymarcher::renderNextFrame() {
    if (m_isRendering || !m_isRunning || m_buffer.isNull()) return;

    m_isRendering = true;
    m_time += 0.05;

    // We must pass raw pointers to the BobUIConcurrent map to avoid QImage copy detaching
    struct RenderData {
        QRgb* scanline;
        int y;
        int w;
        int h;
        double t;
        QVector3D camPos;
    };

    QList<RenderData> rows;
    for (int y = 0; y < m_buffer.height(); ++y) {
        rows.append({reinterpret_cast<QRgb*>(m_buffer.scanLine(y)), y, m_buffer.width(), m_buffer.height(), m_time, m_cameraPos});
    }

    QFuture<void> future = BobUIConcurrent::map(rows, [](const RenderData& data) {
        for (int x = 0; x < data.w; ++x) {
            double uvX = (2.0 * x - data.w) / static_cast<double>(data.h);
            double uvY = (2.0 * data.y - data.h) / static_cast<double>(data.h);

            QVector3D ro = data.camPos;
            QVector3D rd(uvX, -uvY, 1.0);
            rd.normalize();

            double tDist = 0.0;
            const int maxSteps = 64;
            const double maxDist = 20.0;
            const double surfDist = 0.01;

            bool hit = false;
            QVector3D p;

            for (int i = 0; i < maxSteps; ++i) {
                p = ro + rd * tDist;
                double dS = mapSDF(p, data.t);
                tDist += dS;
                if (dS < surfDist) { hit = true; break; }
                if (tDist > maxDist) break;
            }

            QColor pixelColor(11, 11, 12); // Background

            if (hit) {
                QVector3D normal = calcNormal(p, data.t);
                QVector3D lightDir(0.0, 1.0, -1.0);
                lightDir.normalize();

                double diff = std::max(0.0, static_cast<double>(QVector3D::dotProduct(normal, lightDir)));
                
                QVector3D shadowRo = p + normal * (surfDist * 2.0);
                double shadowT = 0.0;
                for (int i = 0; i < 32; ++i) {
                    QVector3D sp = shadowRo + lightDir * shadowT;
                    double sD = mapSDF(sp, data.t);
                    shadowT += sD;
                    if (sD < surfDist) { diff *= 0.1; break; }
                    if (shadowT > maxDist) break;
                }

                int cVal = std::clamp(static_cast<int>(diff * 255), 0, 255);
                pixelColor = QColor(0, cVal, std::min(255, static_cast<int>(cVal * 1.5)));
            }
            data.scanline[x] = pixelColor.rgb();
        }
    });

    m_watcher.setFuture(future);
}

void OmniRaymarcher::onFrameRendered() {
    m_isRendering = false;
    update();
}

void OmniRaymarcher::paint(QPainter *painter) {
    if (!m_buffer.isNull()) {
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
        painter->drawImage(boundingRect(), m_buffer);
    }
}
