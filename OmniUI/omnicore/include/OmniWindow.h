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
    Q_PROPERTY(bool isModal READ isModal WRITE setIsModal NOTIFY isModalChanged)

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

    bool isModal() const;
    void setIsModal(bool modal);

    Q_INVOKABLE void close();
    Q_INVOKABLE void bringToFront();

    void paint(QPainter *painter) override;

signals:
    void titleChanged();
    void accentColorChanged();
    void isMaximizedChanged();
    void ownerIdChanged();
    void collaborativeChanged();
    void isModalChanged();
    void windowClosed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_isModal;
    QString m_title;
    QColor m_accentColor;
    bool m_isMaximized;
    QString m_ownerId;
    bool m_collaborative;
    
    QRectF m_preMaximizeGeometry;
    bool m_isDragging;
    bool m_isResizing;
    int m_resizeEdge; 
    QPointF m_dragStartPos;

    bool checkInteractionPermission(QMouseEvent* event);
};

#endif // OMNIWINDOW_H
