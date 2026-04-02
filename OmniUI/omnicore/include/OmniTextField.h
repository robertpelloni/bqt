#ifndef OMNITEXTFIELD_H
#define OMNITEXTFIELD_H

#include <QQuickItem>
#include <QDebug>

class OmniTextField : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniTextField(QQuickItem *parent = nullptr);
    virtual ~OmniTextField();
};

#endif // OMNITEXTFIELD_H
