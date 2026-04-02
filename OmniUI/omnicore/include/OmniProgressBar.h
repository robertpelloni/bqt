#ifndef OMNIPROGRESSBAR_H
#define OMNIPROGRESSBAR_H

#include <QProgressBar>
#include <QDebug>

class OmniProgressBar : public QProgressBar {
    Q_OBJECT
public:
    explicit OmniProgressBar(QWidget *parent = nullptr);
    virtual ~OmniProgressBar();
};

#endif // OMNIPROGRESSBAR_H
