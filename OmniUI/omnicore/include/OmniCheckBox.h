#ifndef OMNICHECKBOX_H
#define OMNICHECKBOX_H

#include <QQuickItem>
#include <QDebug>

class OmniCheckBox : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniCheckBox(QQuickItem *parent = nullptr);
    virtual ~OmniCheckBox();
};

#endif // OMNICHECKBOX_H
