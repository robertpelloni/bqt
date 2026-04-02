#ifndef OMNIPROGRESSBAR_H
#define OMNIPROGRESSBAR_H

#include <QQuickPaintedItem>
#include <QColor>

class OmniProgressBar : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniProgressBar(QQuickItem *parent = nullptr);
    virtual ~OmniProgressBar();

    qreal value() const;
    void setValue(qreal val);

    qreal minimum() const;
    void setMinimum(qreal min);

    qreal maximum() const;
    void setMaximum(qreal max);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    void paint(QPainter *painter) override;

signals:
    void valueChanged();
    void minimumChanged();
    void maximumChanged();
    void accentColorChanged();

private:
    qreal m_value;
    qreal m_minimum;
    qreal m_maximum;
    QColor m_accentColor;
};

#endif // OMNIPROGRESSBAR_H
