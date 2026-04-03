#ifndef OMNIWINDOW_H
#define OMNIWINDOW_H

#include <QQuickPaintedItem>
#include <QString>
#include <QColor>

class OmniWindow : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(bool isMaximized READ isMaximized WRITE setIsMaximized NOTIFY isMaximizedChanged)
    Q_PROPERTY(QString ownerId READ ownerId WRITE setOwnerId NOTIFY ownerIdChanged)
    Q_PROPERTY(bool collaborative READ collaborative WRITE setCollaborative NOTIFY collaborativeChanged)
    Q_PROPERTY(QString exclusiveDeviceId READ exclusiveDeviceId WRITE setExclusiveDeviceId NOTIFY exclusiveDeviceIdChanged)

public:
    explicit OmniWindow(QQuickItem *parent = nullptr);
    virtual ~OmniWindow();

    QString title() const;
    void setTitle(const QString& title);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    bool isMaximized() const;
    void setIsMaximized(bool max);

    QString ownerId() const;
    void setOwnerId(const QString& id);

    bool collaborative() const;
    void setCollaborative(bool collabo);

    // If set, ONLY this physical/virtual device can interact with the window (Modal Pinning)
    QString exclusiveDeviceId() const;
    void setExclusiveDeviceId(const QString& deviceId);

    Q_INVOKABLE void close();
    Q_INVOKABLE void bringToFront();

    void paint(QPainter *painter) override;

signals:
    void titleChanged();
    void accentColorChanged();
    void isMaximizedChanged();
    void ownerIdChanged();
    void collaborativeChanged();
    void exclusiveDeviceIdChanged();
    void windowClosed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

private:
    QString m_title;
    QColor m_accentColor;
    bool m_isMaximized;
    QString m_ownerId;
    bool m_collaborative;
    QString m_exclusiveDeviceId;
    
    QRectF m_preMaximizeGeometry;
    bool m_isDragging;
    bool m_isResizing;
    int m_resizeEdge; 
    QPointF m_dragStartPos;

    QRectF titleBarRect() const;
    QRectF closeButtonRect() const;
    
    bool checkPermission(QMouseEvent* event);
};

#endif // OMNIWINDOW_H
