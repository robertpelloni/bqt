#ifndef OMNIBUTTON_H
#define OMNIBUTTON_H

#include <QQuickPaintedItem>
#include <QString>
#include <QColor>

class OmniStyleSheet;

class OmniButton : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString omniStyle READ omniStyle WRITE setOmniStyle NOTIFY omniStyleChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)
    Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet NOTIFY styleSheetChanged)

public:
    explicit OmniButton(QQuickItem *parent = nullptr);
    virtual ~OmniButton();

    QString text() const;
    void setText(const QString& text);

    QString omniStyle() const;
    void setOmniStyle(const QString& styleName);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    int cornerRadius() const;
    void setCornerRadius(int radius);

    QString styleSheet() const;
    void setStyleSheet(const QString& style);

    void paint(QPainter *painter) override;

signals:
    void textChanged();
    void omniStyleChanged();
    void accentColorChanged();
    void cornerRadiusChanged();
    void styleSheetChanged();
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

private:
    QString m_text;
    QString m_omniStyle;
    QColor m_accentColor;
    int m_cornerRadius;
    
    QString m_styleSheetString;
    OmniStyleSheet* m_parsedStyle;

    bool m_isHovered;
    bool m_isPressed;
};

#endif // OMNIBUTTON_H
