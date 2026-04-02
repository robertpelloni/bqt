#ifndef OMNICHECKBOX_H
#define OMNICHECKBOX_H

#include <QCheckBox>
#include <QDebug>

class OmniCheckBox : public QCheckBox {
    Q_OBJECT
public:
    explicit OmniCheckBox(QWidget *parent = nullptr);
    virtual ~OmniCheckBox();
};

#endif // OMNICHECKBOX_H
