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

public:
    explicit OmniWindow(QQuickItem *parent = nullptr);
    virtual ~OmniWindow();

    QString title() const;
    void setTitle(const QString& title);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    bool isMaximized() const;
    void setIsMaximized(bool max);

    Q_INVOKABLE void close();
    Q_INVOKABLE void bringToFront();

    void paint(QPainter *painter) override;

signals:
    void titleChanged();
    void accentColorChanged();
    void isMaximizedChanged();
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
    
    QRectF m_preMaximizeGeometry;

    bool m_isDragging;
    bool m_isResizing;
    int m_resizeEdge; // 0=None, 1=Right, 2=Bottom, 3=BottomRight
    QPointF m_dragStartPos;

    QRectF titleBarRect() const;
    QRectF closeButtonRect() const;
};

#endif // OMNIWINDOW_H
