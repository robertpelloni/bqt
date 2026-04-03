#ifndef OMNICHECKBOX_H
#define OMNICHECKBOX_H

#include <QQuickPaintedItem>
#include <QString>
#include <QColor>

class OmniCheckBox : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(QString valueTreeKey READ valueTreeKey WRITE setValueTreeKey NOTIFY valueTreeKeyChanged)

public:
    explicit OmniCheckBox(QQuickItem *parent = nullptr);
    virtual ~OmniCheckBox();

    bool checked() const;
    void setChecked(bool c);

    QString text() const;
    void setText(const QString& t);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    QString valueTreeKey() const;
    void setValueTreeKey(const QString& key);

    void paint(QPainter *painter) override;

signals:
    void checkedChanged();
    void textChanged();
    void accentColorChanged();
    void valueTreeKeyChanged();
    void toggled(bool checked);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

private:
    bool m_checked;
    QString m_text;
    QColor m_accentColor;
    QString m_valueTreeKey;
    bool m_isHovered;
    bool m_isPressed;
};

#endif // OMNICHECKBOX_H
