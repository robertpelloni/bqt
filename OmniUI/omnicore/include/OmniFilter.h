#ifndef OMNIFILTER_H
#define OMNIFILTER_H

#include <QObject>
#include <QDebug>

class OmniFilter : public QObject {
    Q_OBJECT
public:
    explicit OmniFilter(QQuickItem *parent = nullptr);
    virtual ~OmniFilter();
};

#endif // OMNIFILTER_H
