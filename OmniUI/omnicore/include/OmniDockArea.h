#ifndef OMNIDOCKAREA_H
#define OMNIDOCKAREA_H

#include <QQuickItem>
#include <QString>

class OmniDockArea : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString areaName READ areaName WRITE setAreaName NOTIFY areaNameChanged)

public:
    explicit OmniDockArea(QQuickItem *parent = nullptr);
    virtual ~OmniDockArea();

    QString areaName() const;
    void setAreaName(const QString& name);

signals:
    void areaNameChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    QString m_areaName;
    void layoutChildren();
};

#endif // OMNIDOCKAREA_H
