#ifndef OMNILINECHART_H
#define OMNILINECHART_H

#include <QWidget>
#include <QDebug>

class OmniLineChart : public QWidget {
    Q_OBJECT
public:
    explicit OmniLineChart(QWidget *parent = nullptr);
    virtual ~OmniLineChart();
};

#endif // OMNILINECHART_H
