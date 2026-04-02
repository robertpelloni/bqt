#ifndef OMNIPROGRESSBAR_H
#define OMNIPROGRESSBAR_H

#include <QQuickItem>
#include <QDebug>

class OmniProgressBar : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniProgressBar(QQuickItem *parent = nullptr);
    virtual ~OmniProgressBar();
};

#endif // OMNIPROGRESSBAR_H
