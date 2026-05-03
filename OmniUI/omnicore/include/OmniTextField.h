#ifndef OMNITEXTFIELD_H
#define OMNITEXTFIELD_H

#include <QQuickPaintedItem>
#include <QString>
#include <QColor>

class OmniTextField : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText NOTIFY placeholderTextChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniTextField(QQuickItem *parent = nullptr);
    virtual ~OmniTextField();

    QString text() const;
    void setText(const QString& text);

    QString placeholderText() const;
    void setPlaceholderText(const QString& text);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    void paint(QPainter *painter) override;

signals:
    void textChanged();
    void placeholderTextChanged();
    void accentColorChanged();
    void returnPressed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QString m_text;
    QString m_placeholderText;
    QColor m_accentColor;
    int m_cursorPosition;
    bool m_isFocused;
};

#endif // OMNITEXTFIELD_H
