#ifndef OMNIIMGUICANVAS_H
#define OMNIIMGUICANVAS_H

#include <QQuickPaintedItem>
#include <QJSValue>

class OmniImGuiCanvas : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QJSValue onPaint READ onPaint WRITE setOnPaint NOTIFY onPaintChanged)

public:
    explicit OmniImGuiCanvas(QQuickItem *parent = nullptr);
    virtual ~OmniImGuiCanvas();

    QJSValue onPaint() const;
    void setOnPaint(const QJSValue& callback);

    void paint(QPainter *painter) override;

    // IMMEDIATE MODE API (Exposed to Javascript)
    Q_INVOKABLE bool button(const QString& label);
    Q_INVOKABLE void text(const QString& label);

signals:
    void onPaintChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QJSValue m_onPaintCallback;
    
    // Minimal internal Immediate Mode state tracker
    QPointF m_mousePos;
    bool m_mouseDown;
    bool m_mouseClicked;
    
    int m_currentY;
    QPainter* m_currentPainter;
};

#endif // OMNIIMGUICANVAS_H
