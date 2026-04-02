#ifndef OMNILINECHART_H
#define OMNILINECHART_H

#include <QQuickItem>
#include <QDebug>

class OmniLineChart : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniLineChart(QQuickItem *parent = nullptr);
    virtual ~OmniLineChart();
};

#endif // OMNILINECHART_H
